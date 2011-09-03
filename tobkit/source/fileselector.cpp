#include <sys/syslimits.h>
#include <unistd.h>

#include <fat.h>
#include <sys/types.h>
#include <sys/dir.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <algorithm>

#include "tobkit/fileselector.h"

//#include "tools.h"

// TODO:
/*
on pendown, this thing is actually drawn twice. the first draw is done
by ListBox::penDown(px, py);, the second draw from the end of read_directory().
Make this only one draw. Frameskips are at stake!
*/

/* ===================== PUBLIC ===================== */

FileSelector::FileSelector(u8 _x, u8 _y, u8 _width, u8 _height, uint16 **_vram, bool visible)
	:ListBox(_x, _y, _width, _height, _vram, 0, false, visible),
	current_directory("/"), active_filterset("")
{
	onFileSelect = 0;
	onDirChange  = 0;
}

// Drawing request
void FileSelector::pleaseDraw(void) {
	read_directory();
}

// Calls fileselect callback or changes the directory
void FileSelector::penDown(u8 px, u8 py)
{
	ListBox::penDown(px, py);

	// Don't do anything if the scrollthingy is touched!
	u8 relx = px-x;
	if(relx>=width-SCROLLBAR_WIDTH) {
		return;
	}

	// Quit if an out of range element was tapped
	if(activeelement>=elements.size()) return;

	// If it is a dir, enter it
	if((filelist.at(activeelement).is_dir == true)&&(filelist.at(activeelement).name != "..")) {
		//iprintf("element %u\n",activeelement);
		current_directory += filelist.at(activeelement).name + "/";
		//iprintf("newdir: %s\n",current_directory.c_str());
		activeelement = 0;

		if(onDirChange != NULL) {
			onDirChange(current_directory.c_str());
		}

		read_directory();

	// If it is "..", go down a directory
	} else if(filelist.at(activeelement).name == "..") {
		std::string name = current_directory;
		u8 slashpos = name.find_last_of("/", name.length()-2);
		name.erase(slashpos, name.length()-slashpos-1);
		current_directory = name;
		//iprintf("%s\n",current_directory.c_str());
		activeelement = 0;

		if(onDirChange != NULL) {
			onDirChange(current_directory.c_str());
		}

		read_directory();

	// If it is a file, call the callback
	} else if(onFileSelect != 0) {
		onFileSelect(filelist.at(activeelement));
	}
}

// Sets the file select callback
void FileSelector::registerFileSelectCallback(void (*onFileSelect_)(File)) {
	onFileSelect = onFileSelect_;
}

// Sets the dir change callback
void FileSelector::registerDirChangeCallback(void (*onDirChange_)(const char *newdir)) {
	onDirChange = onDirChange_;
}

// Defines a filter rule, selects it if it's the first rule, updates view
void FileSelector::addFilter(std::string filtername, std::vector<std::string> extensions)
{
	filters[filtername] = extensions;
	if(filters.size()==1) {
		active_filterset = filtername;
	}

	if(isExposed()) {
		read_directory();
	}
}

// Selects a filter rule and upates view
void FileSelector::selectFilter(std::string filtername)
{
	active_filterset = filtername;

	if(isExposed()) {
		read_directory();
	}
}

// Get pointer to the selcted file, 0 is no file selected
File *FileSelector::getSelectedFile(void)
{
	if(activeelement < filelist.size()) {
		if(filelist.at(activeelement).name != "..") {
			return &(filelist.at(activeelement));
		} else {
			return 0;
		}
	} else {
		return 0;
	}
}

// Get current dir
std::string FileSelector::getDir(void) {
	return current_directory;
}

// Set current dir
void FileSelector::setDir(std::string dir)
{
	current_directory = dir;
}

/* ===================== PRIVATE ===================== */

std::string stringtolowercase(std::string str)
{
	std::string outstr = str;
	for(u8 i=0;i<str.size();++i) {
		if((outstr[i]>=65)&&(outstr[i]<=90)) {
			outstr[i]+=32;
		}
	}
	return outstr;
}

inline bool compare_filenames(File f1, File f2)
{
	char *fn1, *fn2;
	fn1 = (char*)malloc(256);
	fn2 = (char*)malloc(256);

	strcpy(fn1, stringtolowercase(f1.name).c_str());
	strcpy(fn2, stringtolowercase(f2.name).c_str());

	bool res;
	if((f1.is_dir)&&(!f2.is_dir)) {
		res = true;
	} else if ((!f1.is_dir)&&(f2.is_dir)) {
		res = false;
	} else if(strcmp(fn1,fn2)<0) {
		res = true;
	} else {
		res = false;
	}

	free(fn1);
	free(fn2);

	return res;
}

// Reads the current directory
// stores the alpabetically sorted list of files/dirs in filelist and updates view
// Does not handle anything not in the filter
// Handles everything if there's no filter
void FileSelector::read_directory(void)
{
	//iprintf("%d\n", __LINE__);
	// Go though dir and collect files
	filelist.clear();
//iprintf("%d\n", __LINE__);

	if( chdir(current_directory.c_str()) == -1 ) {
		iprintf("cwd to %s failed\n", current_directory.c_str());
		return;
	}
	//iprintf("%d\n", __LINE__);
	char *filename = (char*)malloc(PATH_MAX);

	DIR *dir;
	struct stat filestats;
	//iprintf("%d\n", __LINE__);
	if((dir = opendir(current_directory.c_str())) == NULL)
	{
		iprintf("Dir read error!\n");
		return;
	}
	//iprintf("%d\n", __LINE__);
	struct dirent *direntry = readdir(dir);

	if(direntry == NULL)
	{
		iprintf("No files found!\n");
		return;
	}
	//iprintf("%d\n", __LINE__);
	while(direntry != NULL)
	{
		if(direntry->d_name[0] != '.') { // Hidden and boring files

			File newfile;
			newfile.name = direntry->d_name;
			newfile.name_with_path = current_directory + direntry->d_name;
			if(direntry->d_type & DT_DIR)
				newfile.is_dir = true;
			else
				newfile.is_dir = false;
            
            if(!newfile.is_dir) {
                int stat_res = stat(newfile.name_with_path.c_str(), &filestats);
                if(stat_res != -1) {
        			newfile.size = filestats.st_size;
                }
            }
			//iprintf("%d\n", __LINE__);
			filelist.push_back(newfile);
			//iprintf("%d\n", __LINE__);
		}
		//iprintf("%d\n", __LINE__);
		direntry = readdir(dir);
		//iprintf("%d\n", __LINE__);
	}

	closedir(dir);
	free(filename);
	//iprintf("%d\n", __LINE__);
	// Apply filter if there is one
	if(active_filterset != "") {
		std::vector<File> newfilelist;
		std::vector<File>::const_iterator fileit;
		std::string extension;
		for(fileit=filelist.begin();fileit!=filelist.end();++fileit) {
			if(fileit->is_dir == true)
			{
				// Don't filter dirs
				newfilelist.push_back(*fileit);
			} else {
				u8 extensionsize;
				u32 lastdot = fileit->name.find_last_of(".");
				if(lastdot != std::string::npos) {
					extensionsize = fileit->name.size() - fileit->name.find_last_of(".") - 1;
				} else {
					extensionsize = 0;
				}
				extension = stringtolowercase(fileit->name.substr(fileit->name.size()-extensionsize,extensionsize));
				if(find(filters[active_filterset].begin(), filters[active_filterset].end(),extension) != filters[active_filterset].end()) {
					newfilelist.push_back(*fileit);
				}
			}
		}

		filelist = newfilelist;
	}

	// Add ".."
	if(current_directory != "/") {
		File dotdot;
		dotdot.name = "..";
		dotdot.is_dir = true;
		dotdot.size = 0;
		filelist.push_back(dotdot);
	}

	// Sort
	sort(filelist.begin(), filelist.end(), compare_filenames);
	//iprintf("%d\n", __LINE__);
	// Display
	elements.clear();
	//iprintf("%d\n", __LINE__);
	activeelement = 0;
	scrollpos = 0;
	//iprintf("%d\n", __LINE__);
	filename = (char*)malloc(256);
	std::string newentry;
	for(u16 i=0;i<filelist.size();++i) {
		newentry = filelist.at(i).name;
		if(filelist.at(i).is_dir == true) {
			newentry = "[" + newentry + "]";
		}
		strncpy(filename, newentry.c_str(), 255);
		elements.push_back(filename);
	}
	free(filename);
	//iprintf("%d\n", __LINE__);
	calcScrollThingy();
	//iprintf("%d\n", __LINE__);
	draw();
	//iprintf("%d\n", __LINE__);
}
