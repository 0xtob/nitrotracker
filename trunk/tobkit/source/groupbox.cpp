#include <stdlib.h>
#include <string.h>

#include "tobkit/groupbox.h"

/* ===================== PUBLIC ===================== */

GroupBox::GroupBox(u8 _x, u8 _y, u8 _width, u8 _height, u16 **_vram, bool _visible)
	:Widget(_x, _y, _width, _height, _vram, _visible)
{
	text = (char*)malloc(256);
}

GroupBox::~GroupBox()
{
	free(text);
}

void GroupBox::pleaseDraw(void)
{
	if(isExposed())
		draw();
}

void GroupBox::setText(const char *text)
{
	strncpy(this->text, text, 256);
	pleaseDraw();
}

/* ===================== PRIVATE ===================== */

void GroupBox::draw(void)
{
	drawBox(0, 4, width, height, theme->col_lighter_bg);
	u16 strwidth = getStringWidth(text);
	drawFullBox(10, 0, strwidth+2, 10, theme->col_light_bg);
	drawString(text, 11, 0, width - 12);
}
