/**********************
DS Tracker Button Class
Very basic
Only Push event
**********************/

#ifndef BUTTON_H
#define BUTTON_H

#include "widget.h"

class Button: public Widget {
	public:
		Button(u8 _x, u8 _y, u8 _width, u8 _height, u16 **_vram, bool _visible=true);
	
		~Button();
		
		// Callback registration
		void registerPushCallback(void (*onPush_)(void));
		
		// Drawing request
		void pleaseDraw(void);
		
		// Event calls
		void penDown(u8 x, u8 y);
		void penUp(u8 x, u8 y);
		void penMove(u8 x, u8 y);
		void buttonPress(u16 button);
		
		void setCaption(const char *caption);
		
	private:
		void (*onPush)(void);
		bool penIsDown;
		
		void draw(u8 down);
		char *caption;
};

#endif
