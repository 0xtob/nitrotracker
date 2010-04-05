#include <stdio.h>

#include "tobkit/pixmap.h"

/* ===================== PUBLIC ===================== */

Pixmap::Pixmap(u8 _x, u8 _y, u8 _width, u8 _height, const u16* _image, u16 **_vram, bool _visible)
	:Widget(_x, _y, _width, _height, _vram, _visible),
	onPush(0), image(_image)
{
	
}
	
Pixmap::~Pixmap()
{
	
}

// Callback registration
void Pixmap::registerPushCallback(void (*onPush_)(void)) {
	onPush = onPush_;
}

// Event calls
void Pixmap::penDown(u8 x, u8 y) {
	if(onPush) onPush();
}

// Drawing request
void Pixmap::pleaseDraw(void)
{
	draw();
}

/* ===================== PRIVATE ===================== */

void Pixmap::draw(void)
{
	for(u16 j=0; j<height; ++j) {
		for(u16 i=0; i<width; ++i) {
			if(image[width*j+i] & BIT(15))
				(*vram)[SCREEN_WIDTH*(y+j)+x+i] = image[width*j+i];
		}
	}
}
