#include <stdio.h>
#include <string.h>

#include "tobkit/piano.h"

#include "tobkit/piano.raw.h"
#include "tobkit/piano.pal.h"
#include "tobkit/piano.map.h"
#include "tobkit/piano_hit.h"

u8 halftones[] = {1, 3, 6, 8, 10, 13, 15, 18, 20, 22};
u8 x_offsets[] = {0, 11, 16, 27, 32, 48, 59, 64, 75, 80, 91, 96};

/* ===================== PUBLIC ===================== */
Piano::Piano(u8 _x, u8 _y, u8 _width, u8 _height, u16 *_char_base, u16 *_map_base, u16 **_vram)
:Widget(_x, _y, _width, _height, _vram),
char_base(_char_base), map_base(_map_base), key_labels_visible(false)
{
	onNote = 0;
	onRelease = 0;
	dmaCopy((uint16*)piano_Palette, (uint16*)BG_PALETTE_SUB, 32);
	dmaCopy((uint16*)piano_fullnotehighlight_Palette, (uint16*)BG_PALETTE_SUB+16, 32);
	dmaCopy((uint16*)piano_halfnotehighlight_Palette, (uint16*)BG_PALETTE_SUB+32, 32);
	dmaCopy((uint16*)piano_Tiles, char_base, 736);
	
	memset(key_labels, ' ', 24);
}


// Drawing request
void Piano::pleaseDraw(void) {
	draw();
}


// Event calls
void Piano::penDown(u8 px, u8 py)
{
	// Look up the note in the hit-array
	u8 kbx, kby;
	kbx = (px - x)/8;
	kby = (py - y)/8;
	
	u8 note = piano_hit[kby][kbx];
	
	setKeyPal(note);
	
	if(onNote) {
		onNote(note);
	}
	
	curr_note = note;
}

void Piano::penUp(u8 px, u8 py)
{
	resetPals();
	
	if(onRelease)
	{
		onRelease(curr_note);
	}
}

// Callback registration
void Piano::registerNoteCallback(void (*onNote_)(u8)) {
	onNote = onNote_;
}

void Piano::registerReleaseCallback(void (*onRelease_)(u8)) {
	onRelease = onRelease_;
}

// Key label handling
void Piano::showKeyLabels(void)
{
	key_labels_visible = true;
	
	for(u8 key=0; key<24; ++key)
		drawKeyLabel(key);
}

void Piano::hideKeyLabels(void)
{
	key_labels_visible = false;
	
	for(u8 key=0; key<24; ++key)
		eraseKeyLabel(key);
}

void Piano::setKeyLabel(u8 key, char label)
{
	eraseKeyLabel(key);
	
	key_labels[key] = label;
	
	drawKeyLabel(key);
}

/* ===================== PRIVATE ===================== */

void Piano::draw(void)
{
	// Fill screen with empty tiles
	u16 i;
	for(i=0;i<768;++i) map_base[i] = 28;
	
	// Copy the piano to the screen
	u8 px, py;
	for(py=0; py<5; ++py)
	{
		for(px=0; px<28; ++px)
		{
			map_base[32*(py+y/8)+(px+x/8)] = piano_Map[29*py+px+1];
		}
	}
}

// Set the key corresp. to note to palette corresp. to pal_idx
void Piano::setKeyPal(u8 note)
{
  u8 px, py, hit_row, pal_idx;

  if(isHalfTone(note))
  {
    hit_row = 0;
    pal_idx = 2;
  }
  else
  {
    hit_row = 4;
    pal_idx = 1;
  }

  for(px=0; px<28; ++px)
  {
    if(piano_hit[hit_row][px] == note)
	{
      for(py=0; py<5; ++py)
	  {
      	map_base[32*(py+y/8)+(px+x/8)] &= ~(3 << 12); // Clear bits 12 and 13 (from the left)
        map_base[32*(py+y/8)+(px+x/8)] |= (pal_idx << 12); // Write the pal index to bits 12 and 13
      }
    }
  }
}

// 1 for halftones, 0 for fulltones
u8 Piano::isHalfTone(u8 note)
{
	u8 i;
	for(i=0;i<10;++i) {
		if(note==halftones[i]) return 1;
	}
	return 0;
}

// Reset piano colors to normal
void Piano::resetPals(void)
{
  u8 px,py;
  for(px=0; px<28; ++px) {
    for(py=0; py<5; ++py) {
      map_base[32*(py+y/8)+(px+x/8)] &= ~(3 << 12); // Clear bits 12 and 13 (from the left)
    }
  }
}

void Piano::drawKeyLabel(u8 key, bool visible)
{
	u8 xpos, ypos, offset;
	u16 col;
	
	if(isHalfTone(key) == true)
	{
		ypos = 12;
		col = RGB15(31, 31, 31);
		offset = 3;
	}
	else
	{
		ypos = 28;
		col = RGB15(0, 0, 0);
		offset = 5;
	}
	
	if(visible == true)
		col |= BIT(15);
	
	xpos = offset + x_offsets[key % 12];
	
	if(key > 11)
		xpos += 111;
	
	char label[] = {key_labels[key], 0};
	
	drawString(label, xpos, ypos, 255, col);
}

void Piano::eraseKeyLabel(u8 key)
{
	drawKeyLabel(key, false);
}
