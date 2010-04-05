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
