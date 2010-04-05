#ifndef _PIXMAP_H_
#define _PIXMAP_H_

#include "widget.h"

class Pixmap: public Widget {
	public:
		Pixmap(u8 _x, u8 _y, u8 _width, u8 _height, const u16* _image, u16 **_vram, bool _visible=true);
	
		~Pixmap();
		
		// Callback registration
		void registerPushCallback(void (*onPush_)(void));	
		
		// Event calls
		void penDown(u8 x, u8 y);
		
		// Drawing request
		void pleaseDraw(void);
		
	private:
		void draw(void);
		
		void (*onPush)(void);
		const u16 *image;
};

#endif
