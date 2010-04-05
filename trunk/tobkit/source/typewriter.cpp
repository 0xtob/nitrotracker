#include <nds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tobkit/typewriter.h"
#include "tobkit/typewriter.hit.h"
#include "tobkit/typewriter.map.h"
#include "tobkit/typewriter.pal.h"
#include "tobkit/typewriter.raw.h"

#define BSP	0x8 // Backspace
#define CAP	0x2 // Caps
#define RET	'\n' // Enter
#define SHF	0x4 // Shift
#define NOK 0x0 // No key

#define MAX_TEXT_LEN	20

#define TW_WIDTH	216
#define TW_HEIGHT	127

/* ===================== PUBLIC ===================== */

Typewriter::Typewriter(const char *_msg, u16 *_char_base, 
	u16 *_map_base, u8 _palette_offset, uint16 **_vram, vuint16* _trans_reg_x,
	vuint16* _trans_reg_y)
	:Widget((SCREEN_WIDTH-TW_WIDTH)/2, (SCREEN_HEIGHT-TW_HEIGHT)/2-15, TW_WIDTH, TW_HEIGHT, _vram),
	char_base(_char_base), map_base(_map_base),
	kx(x+4), ky(y+16),
	mode(MODE_NORMAL),
	trans_reg_x(_trans_reg_x), trans_reg_y(_trans_reg_y), cursorpos(0), strlength(0)
{
	//char_base = (u16*)CHAR_BASE_BLOCK_SUB(1);
	//map_base = (u16*)SCREEN_BASE_BLOCK_SUB(12);
	palette_offset = 3; // I have no clue why, but reading the _palette_offset parameter causes an immediate crash.
	//palette_offset = _palette_offset;
	
	onOk = 0;
	onCancel = 0;

	dmaCopy((uint16*)typewriter_Palette, (uint16*)BG_PALETTE_SUB+palette_offset*16, 32);
	dmaCopy((uint16*)typewriter_Palette_Hilight, (uint16 *)BG_PALETTE_SUB+palette_offset*16+16, 32);
	
	//dmaCopy((uint16*)keyboard_highlight_Palette, (uint16*)BG_PALETTE_SUB+16, 32);
	dmaCopy((uint16*)typewriter_Tiles, char_base, 7776);

	u8 msglength = getStringWidth(_msg);
	
	msglabel = new Label(x+4, y+6, msglength+4, 12, _vram, false);
	msglabel->setCaption(_msg);
	gui.registerWidget(msglabel, 0, SUB_SCREEN);
	
	label = new Label(x+msglength+8, y+4, TW_WIDTH-msglength-12, 15, _vram, true);
	gui.registerWidget(label, 0, SUB_SCREEN);
	
	buttonok = new Button(x+TW_WIDTH/2+2, y+TW_HEIGHT-12-4, 50, 12, _vram);
	buttonok->setCaption("ok");
	gui.registerWidget(buttonok, 0, SUB_SCREEN);
	
	buttoncancel = new Button(x+TW_WIDTH/2-50-2, y+TW_HEIGHT-12-4, 50, 12, _vram);
	buttoncancel->setCaption("cancel");
	gui.registerWidget(buttoncancel, 0, SUB_SCREEN);
	
	// Set the tile bg translation registers to move the typewriter to (x,y)
	*_trans_reg_x = -kx;
	*_trans_reg_y = -ky;
	
	text = (char*)calloc(1, MAX_TEXT_LEN+1);
}

Typewriter::~Typewriter(void)
{
	delete label;
	delete msglabel;
	delete buttonok;
	delete buttoncancel;
	free(text);
	
	for(u8 py=0; py<12; ++py) {
		for(u8 px=0; px<26; ++px) {
		  map_base[32*py+px] = 0;
		}
	}
}


// Drawing request
void Typewriter::pleaseDraw(void) {
	draw();
}

// Event calls
void Typewriter::penDown(u8 px, u8 py)
{
	// Inside the kb?
	if((px>=kx)&&(px<=kx+26*8)&&(py>=ky)&&(py<=ky+12*8))
	{
		tilex = (px-kx)/8;
		tiley = (py-ky)/8;
		
		setTile(tilex, tiley, 4);
		
		if(tilex>=1 && tilex<=24 && tiley<=12)
		{
			char c;
			if((mode==MODE_CAPS)||(mode==MODE_SHIFT))
				c = typewriter_Hit_Shift[tilex+(tiley*26)];
			else
				c = typewriter_Hit[tilex+(tiley*26)];
			
			if(c==CAP)
			{
				if((mode==MODE_CAPS)||(mode==MODE_SHIFT)) {
					mode = MODE_NORMAL;
				} else {
					mode = MODE_CAPS;
				}
				redraw();
			}
			else if (c==SHF)
			{
				if((mode==MODE_CAPS)||(mode==MODE_SHIFT)) {
					mode = MODE_NORMAL;
				} else {
					mode = MODE_SHIFT;
				}
				redraw();
			}
			else if (c==BSP)
			{
				if(cursorpos > 0) {
					for(u16 i=cursorpos-1;i<=strlength;++i) {
						text[i] = text[i+1];
					}
					text[strlength+1] = 0;
					cursorpos--;
					strlength--;
					label->setCaption(text);
					drawCursor();
				}
			}
			else if (c==RET)
			{
				if(onOk!=0) {
					onOk();
				}
			}
			else if(c!=NOK)
			{
				if(mode==MODE_SHIFT) {
					mode = MODE_NORMAL;
					redraw();
				}
				if(strlength<MAX_TEXT_LEN)
				{
					for(u16 i=strlength;i>cursorpos;i--) {
						text[i] = text[i-1];
					}
					text[cursorpos] = c;
					cursorpos++;
					strlength++;
					label->setCaption(text);
					drawCursor();
				}
			}
		}
		
	// Inside the button area?
	} else if ((px>x)&&(px<x+TW_WIDTH)&&(py<y+TW_HEIGHT)&&(py>ky+12*8)) {
		gui.penDown(px, py);
	}
}

void Typewriter::penUp(u8 px, u8 py)
{
	setTile(tilex, tiley, 3);
	gui.penUp(px, py);
}

void Typewriter::buttonPress(u16 button)
{
	if((button==KEY_LEFT)&&(cursorpos>0)) {
		cursorpos--;
		redraw();
	} else if((button==KEY_RIGHT)&&(cursorpos<=strlength)) {
		cursorpos++;
		redraw();
	}
}

// Callback registration
void Typewriter::registerOkCallback(void (*onOk_)(void))
{
	onOk = onOk_;
	buttonok->registerPushCallback(onOk);
}

void Typewriter::registerCancelCallback(void (*onCancel_)(void))
{
	onCancel = onCancel_;
	buttoncancel->registerPushCallback(onCancel);
}

void Typewriter::setText(const char *text_)
{
	for(u16 i=0;i<MAX_TEXT_LEN+1;++i) {
		text[i] = 0;
	}
	u8 len = MAX_TEXT_LEN;
	if(strlen(text_) < MAX_TEXT_LEN) {
		len = strlen(text_);
	}
	strncpy(text, text_, len);
	strlength = len;
	cursorpos = strlength;
	label->setCaption(text);
	redraw();
}

char *Typewriter::getText(void) {
	return text;
}

void Typewriter::show(void)
{
	Widget::show();
	gui.showAll();
	redraw();
}

void Typewriter::reveal(void)
{
	Widget::reveal();
	gui.revealAll();
	redraw();
}

void Typewriter::setTheme(Theme *theme_, u16 bgcolor_)
{
	theme = theme_;
	bgcolor = bgcolor_;
	
	label->setTheme(theme, theme->col_light_bg);
	msglabel->setTheme(theme, theme->col_light_bg);
	buttonok->setTheme(theme, theme->col_light_bg);
	buttoncancel->setTheme(theme, theme->col_light_bg);
}

/* ===================== PRIVATE ===================== */

void Typewriter::draw(void)
{
	drawFullBox(0, 0, TW_WIDTH, TW_HEIGHT, theme->col_light_bg);
	drawBorder();
	gui.draw();
	
	redraw();
}

// don't draw the box new
void Typewriter::redraw(void)
{
	if(!isExposed())
		return;
	
	label->pleaseDraw();
	msglabel->pleaseDraw();
	
	drawCursor();
	
	u16 map_offset;
	if((mode == MODE_CAPS)||(mode == MODE_SHIFT)) {
		map_offset = 312;
	} else {
		map_offset = 0;
	}
	
    for(u8 py=0; py<12; ++py) {
		for(u8 px=0; px<26; ++px) {
		  map_base[32*py+px] = typewriter_Map[map_offset+26*py+px];
          map_base[32*py+px] |= (3 << 12); // Write the pal index to bits 12 and 13
        }
    }
}

// Don't try this at home!
void Typewriter::drawCursor(void)
{
	u8 lx, ly, lw, lh;
	u16 cursorx, cursory, cursorheight;
	label->getPos(&lx, &ly, &lw, &lh);
	cursorx = lx - x + getStringWidth(text, cursorpos) + 1;
	cursory = ly - y + 1;
	cursorheight = lh - 2;
	if(cursorx<lx -x + lw) {
		drawLine(cursorx, cursory, cursorheight, 0, RGB15(0,0,0)|BIT(15));
	}
}

// This function is kindly sponsored by Headspin
void Typewriter::setTile(int x, int y, int pal)
{
	char c;
	int x2, y2;

	c = typewriter_Hit[(y*26)+x];

	if(!c) return;

	map_base[(y*32)+x] &= ~(7 << 12);
	map_base[(y*32)+x] |= (pal << 12);

	x2 = x; y2 = y;
	while(typewriter_Hit[(y2*26)+x2]==c)
	{
		map_base[(y2*32)+x2] &= ~(7 << 12);
		map_base[(y2*32)+x2] |= (pal << 12);

		x2 = x;
		while(typewriter_Hit[(y2*26)+x2]==c)
		{
			map_base[(y2*32)+x2] &= ~(7 << 12);
			map_base[(y2*32)+x2] |= (pal << 12);
			x2++;
		}
		x2 = x;
		while(typewriter_Hit[(y2*26)+x2]==c)
		{
			map_base[(y2*32)+x2] &= ~(7 << 12);
			map_base[(y2*32)+x2] |= (pal << 12);
			x2--;
		}

		x2 = x;
		y2++;
	}

	x2 = x; y2 = y;
	while(typewriter_Hit[(y2*26)+x2]==c)
	{
		map_base[(y2*32)+x2] &= ~(7 << 12);
		map_base[(y2*32)+x2] |= (pal << 12);

		x2 = x;
		while(typewriter_Hit[(y2*26)+x2]==c)
		{
			map_base[(y2*32)+x2] &= ~(7 << 12);
			map_base[(y2*32)+x2] |= (pal << 12);
			x2++;
		}
		x2 = x;
		while(typewriter_Hit[(y2*26)+x2]==c)
		{
			map_base[(y2*32)+x2] &= ~(7 << 12);
			map_base[(y2*32)+x2] |= (pal << 12);
			x2--;
		}

		x2 = x;
		y2--;
	}
}