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

#ifndef _MESSAGEBOX_H_
#define _MESSAGEBOX_H_

#include "widget.h"
#include "gui.h"
#include "label.h"
#include "button.h"

class MessageBox: public Widget {
	public:
		MessageBox(u16 **_vram, const char *message, u8 n_buttons, ...);
		~MessageBox(void);
		
		// Drawing request
		void pleaseDraw(void);
	
		// Event calls
		void penDown(u8 x, u8 y);
		void penUp(u8 x, u8 y);
	
		void show(void);
		void reveal(void);
		void setTheme(Theme *theme_, u16 bgcolor_);
		
	private:
		void draw(void);	
	
		GUI gui;
		//Label *label;
		char *msg;
		Button **buttons;
		void (**callbacks)(void);
		u8 n_buttons;
};

#endif
