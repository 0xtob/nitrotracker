/*====================================================================
Copyright 2006 Tobias Weyand

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    https://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
======================================================================*/

#include <string.h>
#include <stdlib.h>

#include "tobkit/togglebutton.h"

/* ===================== PUBLIC ===================== */

ToggleButton::ToggleButton(u8 _x, u8 _y, u8 _width, u8 _height, u16 **_vram, bool _visible)
	:Widget(_x, _y, _width, _height, _vram, _visible),
	penIsDown(false), on(false), has_bitmap(false)
{
	onToggle = 0;
	caption = (char*)calloc(1, 1);
}

ToggleButton::~ToggleButton()
{
	if (caption) free(caption);
}

// Callback registration
void ToggleButton::registerToggleCallback(void (*onToggle_)(bool)) {
	onToggle = onToggle_;
}

// Drawing request
void ToggleButton::pleaseDraw(void) {
	draw();
}

// Event calls
void ToggleButton::penDown(u8 x, u8 y)
{
	penIsDown = true;
	on = !on;
	draw();
	if(onToggle) {
		onToggle(on);
	}
}

void ToggleButton::penUp(u8 x, u8 y)
{
	penIsDown = false;
	draw();
}

void ToggleButton::buttonPress(u16 button)
{
	on = !on;
	draw();
	if(onToggle) {
		onToggle(on);
	}
}

void ToggleButton::setCaption(const char *_caption)
{
	if (caption) free(caption);
	caption = (char*)malloc(strlen(_caption)+1);
	strcpy(caption, _caption);
}

void ToggleButton::setBitmap(const u8 *_bmp, int _width, int _height)
{
	has_bitmap = true;
	bitmap = _bmp;
	bmpwidth = _width;
	bmpheight = _height;
}

void ToggleButton::setState(bool _on)
{
	if(on != _on)
	{
		on = _on;
		draw();
		if(onToggle) {
			onToggle(on);
		}
	}
}

bool ToggleButton::getState(void)
{
	return on;
}

/* ===================== PRIVATE ===================== */

#define MAX(x,y)	((x)>(y)?(x):(y))

void ToggleButton::draw(void)
{
	if(!isExposed()) return;

	drawGradient(theme->col_dark_ctrl, theme->col_dark_ctrl, 0, 0, width, height);
	drawBorder();

	u16 col;
	if(penIsDown) {
		if(on) {
			col = theme->col_dark_ctrl;
		} else {
			col = theme->col_light_ctrl;
		}
	} else {
		if(on) {
			col = theme->col_light_ctrl;
		} else {
			col = theme->col_text;
		}
	}
	if(has_bitmap) {
		drawMonochromeIcon(2, 2, bmpwidth, bmpheight, bitmap, col);
	}

	drawString(caption, MAX(2, ((width-getStringWidth(caption))/2) ), height/2-5, 255, col);

	/*
	if(penIsDown) {
		if(on) {
			drawGradient(theme->col_dark_ctrl, theme->col_dark_ctrl, 0, 0, width, height);
		} else {
			drawGradient(theme->col_light_ctrl, theme->col_light_ctrl, 0, 0, width, height);
		}
	} else {
		if(on) {
			drawGradient(theme->col_dark_ctrl, theme->col_dark_ctrl, 0, 0, width, height);
		} else {
			drawGradient(theme->col_light_ctrl, theme->col_light_ctrl, 0, 0, width, height);
		}
	}
	drawBorder();

	if(has_bitmap) {
		drawMonochromeIcon(2, 2, bmpwidth, bmpheight, bitmap);
	}

	drawString(caption, MAX(2, ((width-getStringWidth(caption))/2) ), height/2-5, 255, theme->col_light_ctrl);
	*/
}

