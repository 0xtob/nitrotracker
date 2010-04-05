#ifndef _CHECKBOX_H_
#define _CHECKBOX_H_

#include "widget.h"

class CheckBox: public Widget
{
	public:
		CheckBox(u8 _x, u8 _y, u8 _width, u8 _height, u16 **_vram,
			 bool _visible=true, bool checked=false, bool albino=false);
		~CheckBox();
		
		void setCaption(const char *_label);
		void setChecked(bool checked_);
		
		// Drawing request
		void pleaseDraw(void);
		
		// Event calls
		void penDown(u8 px, u8 py);
		
		void registerToggleCallback(void (*onToggle_)(bool));
		
	private:
		char *label;
		bool checked;
		bool albino; // different color setting for dark bg
		
		void (*onToggle)(bool);
		
		void draw(void);
};

#endif
