/*
 * NitroTracker - An FT2-style tracker for the Nintendo DS
 *
 *                                by Tobias Weyand (0xtob)
 *
 * http://nitrotracker.tobw.net
 * http://code.google.com/p/nitrotracker
 */

/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "settings.h"

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/dir.h>

#include "tools.h"

/* ===================== PUBLIC ===================== */

Settings::Settings(bool use_fat)
: handedness(RIGHT_HANDED),
sample_preview(true),
theme(new Theme()),
fat(use_fat)
{
	memset(songpath, 0, 255);
	memset(samplepath, 0, 255);

	strcpy(songpath,"/");
	strcpy(samplepath,"/");

	if(fat == true)
	{
		// Check if the config file exists and, if not, create it
		if(!opendir("/data"))
		{
			mkdir("/data", 777);
		}
		if(!opendir("/data/NitroTracker"))
		{
			mkdir("/data/NitroTracker", 777);
		}
		FILE *conf = fopen("/data/NitroTracker/NitroTracker.conf", "r");
		if(conf == NULL)
		{
			write();
		}
		else
		{
			char hstring[20], prevstring[20];

			char *confstr = (char*)calloc(1, 10240);
			fread(confstr, 10240, 1, conf);
			fclose(conf);

			bool success = getConfigValue(confstr, "Samplepath", samplepath, 255);
			if(!success)
			{
				free(confstr);
				iprintf("Config file invalid\n");
				return;
			}

			success = getConfigValue(confstr, "Songpath", songpath, 255);
			if(!success)
			{
				free(confstr);
				iprintf("Config file invalid\n");
				return;
			}

			success = getConfigValue(confstr, "Handedness", hstring, 20);
			if(!success)
			{
				free(confstr);
				iprintf("Config file invalid\n");
				return;
			}
			handedness = stringToHandedness(hstring);

			success = getConfigValue(confstr, "Sample Preview", prevstring, 20);
			if(!success)
			{
				free(confstr);
				iprintf("Config file invalid\n");
				return;
			}
			sample_preview = stringToBool(prevstring);

			free(confstr);
		}
	}
}

Handedness Settings::getHandedness(void)
{
	return handedness;
}

void Settings::setHandedness(Handedness handedness_)
{
	handedness = handedness_;
    if(fat) {
    	write();
    }
}

bool Settings::getSamplePreview(void)
{
	return sample_preview;
}

void Settings::setSamplePreview(bool sample_preview_)
{
	sample_preview =  sample_preview_;
    if(fat) {
    	write();
    }
}

Theme *Settings::getTheme(void)
{
	return theme;
}

void Settings::setTheme(Theme *theme_)
{
	theme = theme_;
    if(fat) {
    	write();
    }
}

char *Settings::getSongPath(void)
{
    if(!opendir(songpath)) {
        strcpy(songpath, "/");
    }
    return songpath;
}

void Settings::setSongPath(const char* songpath_)
{
	strncpy(songpath, songpath_, 255);
	songpath[min(255, strlen(songpath_))] = 0;
    if(fat) {
    	write();
    }
}

char *Settings::getSamplePath(void)
{
    if(!opendir(samplepath)) {
        strcpy(samplepath, "/");
    }
	return samplepath;
}

void Settings::setSamplePath(const char* samplepath_)
{
	strncpy(samplepath, samplepath_, 255);
	samplepath[min(255, strlen(samplepath_))] = 0;
    if(fat) {
    	write();
    }
}

/* ===================== PRIVATE ===================== */

void Settings::write(void)
{
	FILE *conf = fopen("/data/NitroTracker/NitroTracker.conf", "w");
	if(conf == NULL)
	{
		printf("Error opening config for writing!\n");
		return;
	}

	char hstring[20], prevstring[20];
	handednessToString(hstring);
	boolToString(sample_preview, prevstring);
	fprintf(conf, "Samplepath = %s\nSongpath = %s\nHandedness = %s\nSample Preview = %s\n",
			samplepath, songpath, hstring, prevstring);

	fclose(conf);
}

void Settings::handednessToString(char *str)
{
	if(handedness == LEFT_HANDED)
		strcpy(str, "Left");
	else if(handedness == RIGHT_HANDED)
		strcpy(str, "Right");
}

Handedness Settings::stringToHandedness(char *str)
{
	if(strcasecmp(str, "Left") == 0)
		return LEFT_HANDED;
	else
		return RIGHT_HANDED;
}

void Settings::boolToString(bool b, char *str)
{
	if(b == true)
		strcpy(str, "True");
	else
		strcpy(str, "False");
}

bool Settings::stringToBool(char *str)
{
	if(strcasecmp(str, "True") == 0)
		return true;
	else
		return false;
}

bool Settings::getConfigValue(char *config, const char *attribute, char *value, size_t maxlen)
{
	// Oh goodness, this is going to be some badass C code
	char *attrptr = 0;
	char *valstart = 0;
	char *valend = 0;

	// Find the attribute string
	attrptr = strstr(config, attribute);
	if(attrptr == NULL)
		return false;

	// Find the '=' sign after that
	valstart = strchr(attrptr, '=');
	if(valstart == NULL)
		return false;

	// Skip forward to the next non-space character
	valstart++;
	while(*valstart == ' ')
		valstart++;

	// Find the end of the line
	valend = strchr(attrptr, '\n');
	if(valend == NULL)
		return false;

	// Skip backward to the last non-space character
	valend--;
	while(*valend == ' ')
		valend--;

	// Clear the destination string just to be safe
	memset(value, 0, maxlen);

	size_t vallen = valend - valstart + 1;
	size_t len = min(maxlen-1, vallen);
	strncpy(value, valstart, len);
	value[len] = 0;

	//iprintf("'%s' : '%s'\n", attribute, value);

	return true;
}
