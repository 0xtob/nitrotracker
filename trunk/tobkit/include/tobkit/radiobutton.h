#ifndef RADIOBUTTON_H
#define RADIOBUTTON_H

#include "widget.h"
#include <stdio.h>
#include <vector>

class RadioButton: public Widget {
	public:
		class RadioButtonGroup {
			public:
				RadioButtonGroup();
				
				void add(RadioButton *rb);
				void pushed(RadioButton *rb);
				void setActive(u8 idx);
				void registerChangeCallback(void (*onChange_)(u8));
	
			private:
				std::vector<RadioButton*> rbvec;
				void (*onChange)(u8);
		};
		
		RadioButton(u8 _x, u8 _y, u8 _width, u8 _height, u16 **_vram,
				RadioButtonGroup *_rbg, bool _visible=true);
		
		// Drawing request
		void pleaseDraw(void);
		
		// Event calls
		void penDown(u8 px, u8 py);
		
		void setCaption(const char *caption);
		void setActive(bool _active);
		bool getActive(void);
		
	private:
		void draw(void);
	
		RadioButtonGroup *rbg;
		bool active;
		const char *label;
};

#endif
