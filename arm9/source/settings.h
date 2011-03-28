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

// Persistent program settings

#ifndef SETTINGS_H
#define SETTINGS_H

#include "tobkit/theme.h"

#include <stdlib.h>

enum Handedness {LEFT_HANDED, RIGHT_HANDED};

class Settings {
	public:
		Settings(bool use_fat=true);
		void read(void);

		Handedness getHandedness(void);
		void setHandedness(Handedness handedness_);

		bool getSamplePreview(void);
		void setSamplePreview(bool sample_preview_);

		Theme *getTheme(void);
		void setTheme(Theme *theme_);

		char *getSongPath(void);
		void setSongPath(const char* songpath_);

		char *getSamplePath(void);
		void setSamplePath(const char* samplepath_);

	private:
		void write(void);

		void handednessToString(char *str);
		Handedness stringToHandedness(char *str);

		void boolToString(bool b, char *str);
		bool stringToBool(char *str);

		bool getConfigValue(char *config, const char *attribute, char *value, size_t maxlen);

		Handedness handedness;
		bool sample_preview;
		Theme *theme;
		char songpath[256];
		char samplepath[256];
        bool fat;
};

#endif
