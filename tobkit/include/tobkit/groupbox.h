#ifndef _GROUPBOX_H_
#define _GROUPBOX_H_

#include "widget.h"

class GroupBox: public Widget
{
	public:
		GroupBox(u8 _x, u8 _y, u8 _width, u8 _height, u16 **_vram, bool _visible=true);
		~GroupBox();
		
		void pleaseDraw(void);
		
		void setText(const char *text);
		
	private:
		char *text;
		
		void draw(void);
};

#endif
