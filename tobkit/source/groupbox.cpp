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

#include <stdlib.h>
#include <string.h>

#include "tobkit/groupbox.h"

/* ===================== PUBLIC ===================== */

GroupBox::GroupBox(u8 _x, u8 _y, u8 _width, u8 _height, u16 **_vram, bool _visible)
	:Widget(_x, _y, _width, _height, _vram, _visible)
{
	text = (char*) calloc(1, 256);
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
	strncpy(this->text, text, 255);
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
