#ifndef NUMBERBOX_H
#define NUMBERBOX_H

#include "widget.h"
#include <nds.h>

class NumberBox: public Widget {
	public:
		NumberBox(u8 _x, u8 _y, u8 _width, u8 _height, uint16 **_vram, u8 _value=0, u8 _min=0, u8 _max=255, u8 _digits=2);
	
		// Drawing request
		void pleaseDraw(void);
		
		// Event calls
		void penDown(u8 px, u8 py);
		void penUp(u8 px, u8 py);

		void setValue(u8 val);
		
		// Callback registration
		void registerChangeCallback(void (*onChange_)(u8));
	private:
		void draw(void);
		
		void (*onChange)(u8);
		
		u8 value;
		u8 min, max, digits;
		u8 btnstate;
};

#endif
