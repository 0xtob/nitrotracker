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

#include "tobkit/radiobutton.h"

/* ===================== PUBLIC ===================== */

RadioButton::RadioButton(u8 _x, u8 _y, u8 _width, u8 _height, u16 **_vram,
	RadioButtonGroup *_rbg, bool _visible)
	:Widget(_x, _y, _width, _height, _vram, _visible),
	rbg(_rbg), active(false)
{
	rbg->add(this);
}
	
// Drawing request
void RadioButton::pleaseDraw(void) {
	draw();
}

// Event calls
void RadioButton::penDown(u8 px, u8 py) {
	rbg->pushed(this);
}

void RadioButton::setCaption(const char *caption)
{
	label = caption;
	if(isExposed())
		draw();
}

void RadioButton::setActive(bool _active)
{
	active = _active;
	if(isExposed())
		draw();
}

bool RadioButton::getActive(void) {
	return active;
}

/* ===================== PRIVATE ===================== */

void RadioButton::draw(void)
{
	// Draw the dot
	//drawFullBox(2, 2, 7, 7, col);
	drawGradient(theme->col_light_ctrl, theme->col_dark_ctrl ,2, 2, 7, 7);
	
	drawLine(3, 1, 5, 1, theme->col_outline);
	drawLine(3, 9, 5, 1, theme->col_outline);
	drawLine(1, 3, 5, 0, theme->col_outline);
	drawLine(9, 3, 5, 0, theme->col_outline);
	
	drawPixel(2, 2, theme->col_outline);
	drawPixel(8, 2, theme->col_outline);
	drawPixel(2, 8, theme->col_outline);
	drawPixel(8, 8, theme->col_outline);
	
	// Filled or not
	if(active == true) {
		drawFullBox(4, 4, 3, 3, theme->col_outline);
	}
	
	// Text
	drawString(label, 13, 0);
}
