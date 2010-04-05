#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "tobkit/button.h"

/* ===================== PUBLIC ===================== */

Button::Button(u8 _x, u8 _y, u8 _width, u8 _height, uint16 **_vram, bool _visible)
	:Widget(_x, _y, _width, _height, _vram, _visible),
	 penIsDown(false), caption(0)
{
	onPush = 0;
}

Button::~Button()
{
  if(caption != 0) {
    free(caption);
  }
}

void Button::registerPushCallback(void (*onPush_)(void)) {
	onPush = onPush_;
}
		
// Drawing request
void Button::pleaseDraw(void) {
	draw(penIsDown);
}
		
// Event calls
void Button::penDown(u8 x, u8 y)
{
	penIsDown = true;
	draw(1);
}

void Button::penUp(u8 x, u8 y)
{
	penIsDown = false;
	draw(0);
	if(onPush) {
		onPush();
	}
}

void Button::penMove(u8 x, u8 y) {

}

void Button::buttonPress(u16 button) {
	if(onPush) {
		onPush();
	}
}

void Button::setCaption(const char *_caption) {
	caption = (char*)malloc(strlen(_caption)+1);
	strcpy(caption, _caption);
}

/* ===================== PRIVATE ===================== */

void Button::draw(u8 down) {
	if(!isExposed()) return;
	
	if(enabled)
	{
		if(down) {
			drawGradient(theme->col_light_ctrl, theme->col_dark_ctrl, 0, 0, width, height);
		} else {
			drawGradient(theme->col_dark_ctrl, theme->col_light_ctrl, 0, 0, width, height);
		}
	} else {
		drawGradient(theme->col_light_ctrl_disabled, theme->col_dark_ctrl_disabled, 0, 0, width, height);
	}
	drawBorder();
	
	drawString(caption, (width-getStringWidth(caption))/2, height/2-5);
}
