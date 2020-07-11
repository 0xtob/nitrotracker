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

#ifndef LABEL_H
#define LABEL_H

#include "widget.h"

class Label: public Widget {
	public:
		Label(u8 _x, u8 _y, u8 _width, u8 _height, uint16 **_vram, bool _has_border=true,
		      bool _albino=false, bool _no_bg=false);
		~Label(void);
		
		// Callback registration
		void registerPushCallback(void (*onPush_)(void));
		
		// Drawing request
		void pleaseDraw(void);
		
		// Event calls
		void penDown(u8 x, u8 y);
		
		void setCaption(const char *caption);
		char *getCaption(void);	
	
	private:
		void draw(void);
		
		void (*onPush)(void);
		char *caption;
	
		bool has_border;
		bool is_albino;
		bool no_bg;
};

#endif
