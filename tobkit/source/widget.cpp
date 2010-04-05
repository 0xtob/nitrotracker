#include <string.h>
#include <stdio.h>

#include "tobkit/widget.h"
#include "tobkit/fontchars.h"
#include "font_8x11_raw.h"

#define	abs(x)	(x<0?(-x):(x))

/* ===================== PUBLIC ===================== */

Widget::Widget(u8 _x, u8 _y, u8 _width, u8 _height, u16 **_vram, bool _visible, bool _occluded)
	:x(_x), y(_y), width(_width), height(_height), enabled(true), vram(_vram), visible(_visible), occluded(_occluded)
{

}

// Get position
void Widget::getPos(u8 *_x, u8 *_y, u8 *_width, u8 *_height)
{
	*_x = x;
	*_y = y;
	*_width = width;
	*_height = height;
}

void Widget::setPos(u8 _x, u8 _y)
{
	x = _x;
	y = _y;
}

// Toggle visibility
void Widget::show(void)
{
	if(!visible)
	{
		visible = true;
		if(!occluded)
			pleaseDraw();
	}
}

void Widget::hide(void)
{
	if(isExposed())
		overdraw();
	visible = false;
}

void Widget::occlude(void)
{
	if(isExposed())
			overdraw();
	occluded = true;
}

void Widget::reveal(void)
{
	if(occluded)
	{
		occluded = false;
		if(visible)
			pleaseDraw();
	}
}

void Widget::resize(u16 w, u16 h)
{
	overdraw();
	width = w;
	height = h;
	pleaseDraw();
}

// Toggle enabled/disabled
void Widget::enable(void)
{
	if(!enabled)
	{
		enabled = true;
		pleaseDraw();
	}
}

void Widget::disable(void)
{
	if(enabled)
	{
		enabled = false;
		pleaseDraw();
	}
}

/* ===================== PROTECTED ===================== */

// Draw utility functions

void Widget::drawString(const char* str, u8 tx, u8 ty, u8 maxwidth, u16 color)
{
	// Draw text
	u8 pos=0, charidx, i, j;
	u16 drawpos = 0; u8 col;
	char *charptr;

	while( (pos<strlen(str)) && (drawpos+6<maxwidth) )
	{
		charptr = strchr(fontchars, str[pos]);
		if(charptr==0) {
			charidx = 66; // '?'
		} else {
			charidx = charptr - fontchars;
		}

		for(j=0;j<11;++j) {
			for(i=0;i<8;++i) {
				// Print a character from the bitmap font
				// each char is 8 pixels wide, and 8 pixels
				// are in a byte.
				col = font_8x11_raw[N_FONT_CHARS*j+charidx];
				if(col & BIT(i)) {
					*(*vram+SCREEN_WIDTH*(j+y+ty)+(i+x+tx+drawpos)) = color;
				}
				/*
				col = font_8x11[512*j+charidx*8+i];
				if(col & BIT(15)) { // Alpha bit test
					*(*vram+SCREEN_WIDTH*(j+y+ty)+(i+x+tx+drawpos)) = col;
				}*/
			}
		}

		drawpos += charwidths_8x11[charidx]+1;
		pos++;
	}
}

void Widget::drawBox(u8 tx, u8 ty, u8 tw, u8 th, u16 col)
{
	u8 i,j;
	for(i=0;i<tw;++i) {
		*(*vram+SCREEN_WIDTH*(y+ty)+i+(x+tx)) = col;
		*(*vram+SCREEN_WIDTH*((y+ty)+th-1)+i+(x+tx)) = col;
	}
	for(j=0;j<th;++j) {
		*(*vram+SCREEN_WIDTH*((y+ty)+j)+(x+tx)) = col;
		*(*vram+SCREEN_WIDTH*((y+ty)+j)+(x+tx)+tw-1) = col;
	}
}

void Widget::drawFullBox(u8 tx, u8 ty, u8 tw, u8 th, u16 col)
{
	u8 i,j;
	for(i=0;i<tw;++i) {
		for(j=0;j<th;++j) {
			*(*vram+SCREEN_WIDTH*(y+ty+j)+i+(x+tx)) = col;
		}
	}
}

void Widget::drawBorder(u16 col) {
	drawBox(0, 0, width, height, col);
}

void Widget::drawLine(u8 tx, u8 ty, u8 length, u8 horizontal, u16 col) {

	u8 i;
	if(horizontal) {
		for(i=0;i<length;++i) {
			*(*vram+SCREEN_WIDTH*(y+ty)+i+(x+tx)) = col;
		}
	} else {
		for(i=0;i<length;++i) {
			*(*vram+SCREEN_WIDTH*(y+ty+i)+(x+tx)) = col;
		}
	}
}

void Widget::drawBresLine(u8 tx1, u8 ty1, u8 tx2, u8 ty2, u16 col)
{
	u32 x1, y1, x2, y2;
	x1 = tx1 + x;
	x2 = tx2 + x;
	y1 = ty1 + y;
	y2 = ty2 + y;

	// Guarantees that all lines go from left to right
	if ( x2 < x1 )
	{
		u32 tmp;
		tmp = x2; x2 = x1; x1 = tmp;
		tmp = y2; y2 = y1; y1 = tmp;
	}

	s32 dy,dx;
	dy = y2 - y1;
	dx = x2 - x1;

	// If the gradient is greater than one we have to flip the axes
	if ( abs(dy) < dx )
	{
		u16 xp,yp;
		s32 d;
		s32 add = 1;

		xp = x1;
		yp = y1;

		if(dy < 0)
		{
			dy = -dy;
			add =- 1;
		}

		d = 2*dy - dx;

		for(; xp<=x2; xp++)
		{
			if(d > 0)
			{
				yp += add;
				d -= 2 * dx;
			}

			//PxlOn(xp,yp,r1,g1,b1,r2,g2,b2);
			*(*vram+SCREEN_WIDTH*yp+xp) = col;

			d += 2 * dy;
		}
	}
	else
	{
		u16 tmp;
		tmp = x1; x1 = y1; y1 = tmp;
		tmp = x2; x2 = y2; y2 = tmp;

		if ( x2 < x1 )
		{
			tmp = x2; x2 = x1; x1 = tmp;
			tmp = y2; y2 = y1; y1 = tmp;
		}

		u16 xp,yp;
		s32 d;

		dy = y2 - y1;
		dx = x2 - x1;

		s32 add = 1;

		if(dy < 0)
		{
			dy = -dy;
			add=-1;
		}

		xp = x1;
		yp = y1;

		d = 2 * dy - dx;

		for(xp=x1; xp<=x2; xp++) {

			if(d > 0)
			{
				yp += add;
				d -= 2 * dx;
			}

			//PxlOn(yp,xp,r1,g1,b1,r2,g2,b2);
			*(*vram+SCREEN_WIDTH*xp+yp) = col;

			d += 2 * dy;
		}
	}
}

void Widget::drawPixel(u8 tx, u8 ty, u16 col) {
	*(*vram+SCREEN_WIDTH*(y+ty)+x+tx) = col;
}

void Widget::drawGradient(u16 col1, u16 col2, u8 tx, u8 ty, u8 tw, u8 th) {

	u8 i, j;
	u16 col;
	for(j=0;j<th;++j) {
		col = interpolateColor(col1, col2, j*255/th);
		for(i=0;i<tw;++i) {
			*(*vram+SCREEN_WIDTH*(y+ty+j)+x+tx+i) = col;
		}
	}
}

// How wide is the string when rendered?
u8 Widget::getStringWidth(const char *str, u16 limit)
{
	u8 i,res=0,charidx,len = strlen(str);
	if((limit!=USHRT_MAX)&&(limit<len)) {
		len = limit;
	}
	char *charptr;
	for(i=0;i<len;++i) {
		charptr = strchr(fontchars, str[i]);
		if(charptr==0) {
			charidx = 39; // '_'
		} else {
			charidx = charptr - fontchars;
		}
		res += charwidths_8x11[charidx] + 1;
	}
	return res;
}

void Widget::drawMonochromeIcon(u8 tx, u8 ty, u8 tw, u8 th, const u8 *icon, u16 color) {

	for(u8 j=0;j<th;++j) {
		for(u8 i=0;i<tw;++i) {
			u16 pixelidx = tw*j+i;
			if(icon[pixelidx/8] & BIT(pixelidx%8) ) {
				*(*vram+SCREEN_WIDTH*(y+ty+j)+x+tx+i) = color;
			}
		}
	}
}

// Stylus utility functions
bool Widget::isInRect(u8 x, u8 y, u8 x1, u8 y1, u8 x2, u8 y2)
{
	return ( (x >= x1) && (x <= x2) && (y >= y1) && (y <= y2) );
}

bool Widget::isExposed(void)
{
	return visible && !occluded;
}

/* ===================== PRIVATE ===================== */

void Widget::overdraw(void)
{
	drawFullBox(0, 0, width, height, bgcolor);
}
