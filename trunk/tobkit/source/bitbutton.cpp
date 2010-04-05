#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <nds.h>

#include "tobkit/bitbutton.h"

/* ===================== PUBLIC ===================== */

BitButton::BitButton(u8 _x, u8 _y, u8 _width, u8 _height, uint16 **_vram, const u8 *_bitmap, u8 _bmpwidth,
	u8 _bmpheight, u8 _bmpx, u8 _bmpy, bool _visible)
	:Widget(_x, _y, _width, _height, _vram, _visible),
	 penIsDown(false), bitmap(_bitmap), bmpwidth(_bmpwidth), bmpheight(_bmpheight), bmpx(_bmpx), bmpy(_bmpy)
{
	onPush = 0;
}

void BitButton::registerPushCallback(void (*onPush_)(void)) {
	onPush = onPush_;
}

// Drawing request
void BitButton::pleaseDraw(void) {
	draw(penIsDown);
}

// Event calls
void BitButton::penDown(u8 x, u8 y)
{
	if(!enabled)
		return;
	penIsDown = true;
	draw(1);
}

void BitButton::penUp(u8 x, u8 y)
{
	penIsDown = false;
	draw(0);
	if(onPush) {
		onPush();
	}
}

void BitButton::buttonPress(u16 button)
{
	if(onPush) {
		onPush();
	}
}

/* ===================== PRIVATE ===================== */

void BitButton::draw(u8 down)
{
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

	drawMonochromeIcon(bmpx, bmpy, bmpwidth, bmpheight, bitmap);
}
