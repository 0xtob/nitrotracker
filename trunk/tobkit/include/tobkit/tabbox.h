#ifndef TABBOX_H
#define TABBOX_H

#include "gui.h"
#include "widget.h"

#include <vector>

class TabBox: public Widget {
	public:
		TabBox(u8 _x, u8 _y, u8 _width, u8 _height, u16 **_vram, bool _visible=true);
		
		void addTab(const u8 *icon);
	
		// Adds a widget and specifies which button it listens to
		// Touches on widget's area are redirected to the widget
		void registerWidget(Widget *w, u16 listeningButtons, u8 tabidx, u8 screen=SUB_SCREEN);
		
		// Event calls
		void penDown(u8 px, u8 py);
		void penUp(u8 px, u8 py);
		void penMove(u8 px, u8 py);
		void buttonPress(u16 buttons);
		
		// Callback registration
		void registerTabChangeCallback(void (*onTabChange_)(u8 tab));
		
		// Drawing request
		void pleaseDraw(void);
		
		void show(void);
		void hide(void);
		void occlude(void);
		void reveal(void);
		
		void setTheme(Theme *theme_, u16 bgcolor_);
		
	private:
		void draw(void);	
		void updateVisibilities(void);
	
		u8 currenttab;
		std::vector<const u8*> icons;
		std::vector<GUI> guis;
	
		void (*onTabChange)(u8 tab);
};

#endif
