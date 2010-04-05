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
