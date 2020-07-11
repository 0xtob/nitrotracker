/*====================================================================
Copyright 2006 Tobias Weyand

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    https://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
======================================================================*/

#ifndef PIANO_H
#define PIANO_H

#include <nds.h>
#include "widget.h"

class Piano: public Widget {
	public:
		Piano(u8 _x, u8 _y, u8 _width, u8 _height, u16 *_char_base, u16 *_screen_base, u16 **_vram);
	
		// Drawing request
		void pleaseDraw(void);
		
		// Event calls
		void penDown(u8 px, u8 py);
		void penUp(u8 px, u8 py);
		
		// Callback registration
		void registerNoteCallback(void (*onNote_)(u8));
		void registerReleaseCallback(void (*onRelease_)(u8));
		
		// Key label handling
		void showKeyLabels(void);
		void hideKeyLabels(void);
		void setKeyLabel(u8 key, char label);
		
	private:
		void (*onNote)(u8);
		void (*onRelease)(u8);
		uint16 *char_base, *map_base;
		
		void draw(void);
		void setKeyPal(u8 note);
		u8 isHalfTone(u8 note);
		void resetPals(void);
		
		void drawKeyLabel(u8 key, bool visible=true);
		void eraseKeyLabel(u8 key);
		
		char key_labels[24];
		bool key_labels_visible;
		u16 curr_note;
};

#endif
