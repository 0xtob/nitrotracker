#ifndef FILESELECTOR_H
#define FILESELECTOR_H

#include "listbox.h"
#include <nds.h>

#include <vector>
#include <map>
#include <string>

typedef std::map<std::string, std::vector<std::string> > FilterSet;

class File {
	public:
		std::string name;
		std::string name_with_path;
		bool is_dir;
		u32 size;
};

class FileSelector: public ListBox {
	public:
		FileSelector(u8 _x, u8 _y, u8 _width, u8 _height, uint16 **_vram, bool visible=true);
		
		// Calls fileselect callback or changes the directory
		void penDown(u8 px, u8 py);
	
		// Drawing request
		void pleaseDraw(void);	
	
		// Sets the file select callback
		void registerFileSelectCallback(void (*onFileSelect_)(File));
		
		// Sets the dir change callback
		void registerDirChangeCallback(void (*onDirChange_)(const char *newdir));
		
		// Defines a filter rule, selects it if it's the first rule, updates view
		void addFilter(std::string filtername, std::vector<std::string> extensions);
	
		// Selects a filter rule and upates view
		void selectFilter(std::string filtername);
	
		// Get pointer to the selcted file, 0 is no file selected
		File *getSelectedFile(void);
	
		// Get current dir
		std::string getDir(void);
		
		// Set current dir
		void setDir(std::string dir);
	
	private:
		// Helper for converting a string to lowercase
		//void lowercase(char *str);
	
		// Reads the current directory
		// stores the alpabetically sorted list of files/dis in filelist and updates view
		// Does not handle anything not in the filter
		// Handles everything if there's no filter
		void read_directory(void);
		
		std::string current_directory;
		FilterSet filters;
		std::string active_filterset;
		void (*onFileSelect)(File);
		void (*onDirChange)(const char *newdir);
		
		std::vector<File> filelist; // Files that are displayed
};

#endif
