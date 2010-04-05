#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <nds.h>

#include "tobkit/label.h"

/* ===================== PUBLIC ===================== */

Label::Label(u8 _x, u8 _y, u8 _width, u8 _height, uint16 **_vram, bool _has_border, bool _albino, bool _no_bg)
	:Widget(_x, _y, _width, _height, _vram),
	onPush(0), caption(0), has_border(_has_border), is_albino(_albino), no_bg(_no_bg)
{
	caption = (char*)calloc(1, 1);
}

Label::~Label(void)
{
	if(caption!=0) {
		free(caption);
	}
}

// Callback registration
void Label::registerPushCallback(void (*onPush_)(void))
{
	onPush = onPush_;
}

// Drawing request
void Label::pleaseDraw(void) {
	if(isExposed())
		draw();
}

// Event calls
void Label::penDown(u8 x, u8 y)
{
	if(onPush != 0) {
		onPush();
	}
}

void Label::setCaption(const char *_caption)
{
	if(caption!=0) {
		free(caption);
	}

	caption = (char*)malloc(strlen(_caption)+1);
	strcpy(caption, _caption);

	draw();
}

char *Label::getCaption(void) {
	return caption;
}

/* ===================== PRIVATE ===================== */

void Label::draw(void)
{
	if(!isExposed())
		return;

	u16 col_bg, col_text;
	if(is_albino)
	{
		col_bg = theme->col_dark_bg;
		col_text = theme->col_lighter_bg;
	}
	else
	{
		col_bg = theme->col_light_bg;
		col_text = theme->col_text;
	}

	if(has_border)
	{
		if(!no_bg)
			drawFullBox(0, 0, width, height, theme->col_lighter_bg);
		drawBorder();
		if(caption) {
			drawString(caption,2,2,width/*-4*/,col_text);
		}
	}
	else
	{
		if(!no_bg)
			drawFullBox(0, 0, width, height, col_bg);

		if(caption) {
			drawString(caption,0,0,width/*-4*/,col_text);
		}
	}

}
