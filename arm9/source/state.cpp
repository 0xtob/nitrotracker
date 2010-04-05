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

#include "state.h"

#include <stdlib.h>

State::State(void) {
	reset();
}

void State::reset(void)
{
	dsmi_connected = false;
	dsmi_send = true;
	dsmi_recv = true;
	preview_sample = 0;
	song_filename = (char*)calloc(1, 512);
	sample_filename = (char*)calloc(1, 512);

	resetSong();
}

void State::resetSong(void) // resets only song-specific settings
{
	potpos = 0;
	row = 0;
	channel = 0;
	add = 1;
	basenote = 4*12;
	instrument = 0;
	sample = 0;
	recording = false;
	playing = false;
	pause = false;
	map_samples = false;
}
