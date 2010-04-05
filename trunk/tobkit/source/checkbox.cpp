#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "tobkit/checkbox.h"
#include "haken_raw.h"

/* ===================== PUBLIC ===================== */

CheckBox::CheckBox(u8 _x, u8 _y, u8 _width, u8 _height, u16 **_vram, bool _visible, bool checked,
		  bool albino)
	:Widget(_x, _y, _width, _height, _vram, _visible),
	label(0), checked(checked), albino(albino), onToggle(0)
{
	
}

CheckBox::~CheckBox()
{
	if(label)
		free(label);
}

void CheckBox::setCaption(const char *_label)
{
	label = (char*)calloc(256, 1);
	strncpy(label, _label, 256);
}

void CheckBox::setChecked(bool checked_)
{
	checked = checked_;
	draw();
}

// Drawing request
void CheckBox::pleaseDraw(void)
{
	draw();
}

// Event calls
void CheckBox::penDown(u8 px, u8 py)
{
	if(!enabled)
		return;
	
	checked = !checked;
	
	if(onToggle != 0)
		onToggle(checked);
	
	draw();
}

void CheckBox::registerToggleCallback(void (*onToggle_)(bool))
{
	onToggle = onToggle_;
}

/* ===================== PRIVATE ===================== */

void CheckBox::draw(void)
{
	if(!isExposed())
		return;
	
	// Draw the box
	if(enabled)
		drawGradient(theme->col_light_ctrl, theme->col_dark_ctrl ,2, 4, 7, 7);
	else
		drawGradient(theme->col_light_ctrl_disabled, theme->col_dark_ctrl_disabled,2, 4, 7, 7);
	
	drawLine(1, 3, 9, 1, theme->col_outline);
	drawLine(1,11, 9, 1, theme->col_outline);
	drawLine(1, 3, 9, 0, theme->col_outline);
	drawLine(9, 3, 9, 0, theme->col_outline);
	
	// Clear up
	if(!albino)
		drawFullBox(0, 0, 11, 3, theme->col_light_bg);
	else
		drawFullBox(0, 0, 11, 3, theme->col_dark_bg);
	
	// Checked or not
	if(checked == true)
		drawMonochromeIcon(1, 0, 10, 10, haken_raw);
	
	// Text
	if(!albino)
		drawString(label, 13, 2, 255, theme->col_text);
	else
		drawString(label, 13, 2, 255, theme->col_lighter_bg);
}
