#include <stdlib.h>
#include <stdio.h>

#include "tobkit/numberbox.h"

/* ===================== PUBLIC ===================== */

NumberBox::NumberBox(u8 _x, u8 _y, u8 _width, u8 _height, uint16 **_vram, u8 _value, u8 _min, u8 _max, u8 _digits)
	:Widget(_x, _y, _width, _height, _vram),
	value(_value), min(_min), max(_max), digits(_digits), btnstate(0)
{
	onChange = 0;
}	

// Drawing request
void NumberBox::pleaseDraw(void) {
	draw();
}

// Event calls
void NumberBox::penDown(u8 px, u8 py) {
	
	u8 oldvalue = value;
	
	if((px>x)&&(px<x+width)&&(py>y)&&(py<y+9)) {
		btnstate = 1;
		if(value<max) value++;
	} else if((px>x)&&(px<x+width)&&(py>y+9)&&(py<y+18)) {
		btnstate = 2;
		if(value>min) value--;
	}
	
	if(value!=oldvalue) {
		onChange(value);
		draw();
	}
}

void NumberBox::penUp(u8 px, u8 py) {
	
	btnstate = 0;
	draw();
}

void NumberBox::setValue(u8 val)
{
	s32 oldval = value;
	
	if(val > max)
		value = max;
	else if (val < min)
		value = min;
	else
		value = val;

	if(oldval != value)
	{
		if(onChange!=0)
			onChange(val);
		if(isExposed())
			draw();
	}
}

// Callback registration
void NumberBox::registerChangeCallback(void (*onChange_)(u8)) {
	onChange = onChange_;
}

/* ===================== PRIVATE ===================== */

void NumberBox::draw(void)
{
	// Upper Button
	if(btnstate==1) {
		drawGradient(theme->col_dark_ctrl, theme->col_light_ctrl, 1, 1, 8, 8);
	} else {
		drawGradient(theme->col_light_ctrl, theme->col_dark_ctrl, 1, 1, 8, 8);
	}
	
	// This draws the up-arrow
	s8 i,j;
	for(j=0;j<3;j++) {
		for(i=-j;i<=j;++i) {
			*(*vram+SCREEN_WIDTH*(y+j+3)+x+4+i) = theme->col_icon;
		}
	}
	
	drawBox(0, 0, 9, 9);
	
	// Lower Button
	if(btnstate==2) {
		drawGradient(theme->col_dark_ctrl, theme->col_light_ctrl, 1, 8, 8, 8);
	} else {
		drawGradient(theme->col_light_ctrl, theme->col_dark_ctrl, 1, 8, 8, 8);
	}
	
	// This draws the down-arrow
	for(j=2;j>=0;j--) {
		for(i=-j;i<=j;++i) {
			*(*vram+SCREEN_WIDTH*(y-j+13)+x+4+i) = theme->col_icon;
		}
	}
	
	drawBox(0, 8, 9, 9);
	
	// Number display
	drawFullBox(9, 1, width-9, height-1, theme->col_lighter_bg);
	char *numberstr = (char*)malloc(4);
	
	char formatstr[] = "%_u";
	// Set no of digits (hacky, but there's no other way)
	formatstr[1] = digits+48;
	
	sprintf(numberstr, formatstr, value);
	drawString(numberstr, 10, 5);
	free(numberstr);
	
	// Border
	drawBorder();
	
}
