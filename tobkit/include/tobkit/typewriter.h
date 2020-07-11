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

#ifndef _TYPEWRITER_H_
#define _TYPEWRITER_H_

#include "widget.h"
#include "label.h"
#include "button.h"
#include "gui.h"

#define MODE_NORMAL	0
#define MODE_SHIFT	1
#define MODE_CAPS	2

class Typewriter: public Widget {
	public:
		Typewriter(/*u8 _x, u8 _y,*/const char *_msg, u16 *_char_base,
			u16 *_map_base, u8 _palette_offset, uint16 **_vram,
			vuint16* _trans_reg_x, vuint16* _trans_reg_y);
	
		~Typewriter(void);
	
		// Drawing request
		void pleaseDraw(void);
		
		// Event calls
		void penDown(u8 px, u8 py);
		void penUp(u8 px, u8 py);
	
		void buttonPress(u16 button);
	
		// Callback registration
		void registerOkCallback(void (*onOk_)(void));
		void registerCancelCallback(void (*onCancel_)(void));
	
		void setText(const char *text);
		char *getText(void);
	
		void show(void);
		void reveal(void);
		void setTheme(Theme *theme_, u16 bgcolor_);
		
	private:
		void (*onOk)(void);
		void (*onCancel)(void);
	
		u16 *char_base, *map_base;
		u8 palette_offset;
		
		u16 col_bg;
	
		u8 kx, ky; // Keyboard offset in tiles (1t=8px)
	
		Label *label, *msglabel;
		GUI gui;
		Button *buttonok, *buttoncancel;
	
		u8 mode;
		vuint16 *trans_reg_x, *trans_reg_y;

		char *text;
		u16 cursorpos, strlength;
	
		u8 tilex, tiley;
		
		void draw(void);
		void redraw(void);
		void drawCursor(void);
		void setTile(int x, int y, int pal);
};

#endif
