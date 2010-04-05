#ifndef NUMBERSLIDER_H
#define NUMBERSLIDER_H

#include "widget.h"

class NumberSlider: public Widget {
	public:
		NumberSlider(u8 _x, u8 _y, u8 _width, u8 _height, uint16 **_vram, s32 _value=0, s32 _min=0, s32 _max=255,
			     bool _hex=false);
	
		// Drawing request
		void pleaseDraw(void);
		
		// Event calls
		void penDown(u8 px, u8 py);
		void penUp(u8 px, u8 py);
		void penMove(u8 px, u8 py);

		void setValue(s32 val);
		s32 getValue(void);
		
		// Callback registration
		void registerChangeCallback(void (*onChange_)(s32));
		
	private:
		void draw(void);
		
		void (*onChange)(s32);
		
		s32 value;
		u8 lasty;
		bool btnstate;
		s32 min;
		s32 max;
		bool hex;
};

#endif
