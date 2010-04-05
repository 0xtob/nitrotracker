/*
 * NitroTracker - An FT2-style tracker for the Nintendo DS
 *
 *                                by Tobias Weyand (0xtob)
 *
 * http://nitrotracker.tobw.net
 * http://code.google.com/p/nitrotracker
 */

/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * splash.cpp
 *
 *  Created on: Jun 29, 2008
 *      Author: tob
 */
#include "splash.h"

#include <nds.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "nitrotracker_logo_bin.h"
#include "nitrotracker_logo_pal_bin.h"

#include "04_img_bin.h"
#include "04_pal_bin.h"

#define max(x,y) ((x)>(y)?(x):(y))

u16 *vram1, *vram2;

u8 *randmem = 0;
int r = 0;
bool hdist = true;

SpriteEntry sprites[128];

void splash_init_sprites(void)
{
	for(int i = 0; i < 128; i++)
	{
		sprites[i].attribute[0] = ATTR0_DISABLED;
		sprites[i].attribute[1] = 0;
		sprites[i].attribute[2] = 0;
	}
}

void splash_update_OAM(void)
{
	DC_FlushRange(sprites,128*sizeof(SpriteEntry));
	dmaCopy(sprites, OAM_SUB, 128 * sizeof(SpriteEntry));
}


int splash_rand(void)
{
	static int seed = 2701;
	return seed = ((seed * 1103515245) + 12345) & 0x7fffffff;
}

void splash_vcount(void)
{
	hdist = true;
}

void splash_hblank(void)
{
	hdist = not hdist;
	if(not hdist)
		return;

	int linerand = randmem[r++] / 85 - 1; // [-1,1]
	if(linerand > 0)
		linerand = (int)randmem[r++] >> 7; // [0,1]
	else if(linerand < 0)
		linerand = - ((int)randmem[r++] >> 7); // [-1,0]

	bgTransform[7]->dx = (-26 - linerand) << 8;
	//bgSetScroll(7,-26 - linerand, 0);
}

void splash_vblank(void)
{
	// Move the sprite
	static int c=0;
	int ypos = cosLerp(degreesToAngle(c)) >> 9;
	c+=2;
	c %= 360;
	sprites[0].attribute[0] = ATTR0_COLOR_16 | ATTR0_TYPE_BLENDED | (55 + ypos);

	// Generate background noise
	u16 col;
	for(int y=0;y<96;++y)
	{
		for(int x=0;x<128;++x)
		{
			if(randmem[r++] > 204)
				col = RGB15(6,6,6) | BIT(15);
			else if(randmem[r++] > 245)
				col = RGB15(12,12,12) | BIT(15);
			else
				col = 0 | BIT(15);

			vram1[128*y+x] = col;
		}
	}

	// Decide whether to distort the frame
	if(randmem[r++] < 8)
		irqEnable(IRQ_HBLANK);
	else
		irqDisable(IRQ_HBLANK);

	// Reset Logo position
	bgTransform[7]->dx = -26  << 8;

	// Reset random counter
	if(r > 2000*1024)
	{
		r = r - 2000*1024 + 2*randmem[r];
	}
	splash_update_OAM();
}

void splash_show(void)
{
	// Set up video
	videoSetMode(MODE_5_2D);
	videoSetModeSub(MODE_5_2D | DISPLAY_BG2_ACTIVE | DISPLAY_BG3_ACTIVE | DISPLAY_SPR_ACTIVE | DISPLAY_SPR_1D | DISPLAY_SPR_1D_BMP);
	vramSetBankC(VRAM_C_SUB_BG);
	vramSetBankD(VRAM_D_SUB_SPRITE);

	// ERB1
	int erb1 = bgInitSub(2, BgType_Bmp16, BgSize_B16_128x128, 1, 0);
	bgSetPriority(erb1, 2);
	bgSetScale(erb1, 1 << 7, 1 << 7);
	vram1 = (uint16*)BG_BMP_RAM_SUB(1);

	for(int i=0; i<128*128; ++i)
		vram1[i] = 0;

	// ERB2
	int erb2 = bgInitSub(3, BgType_Bmp8, BgSize_B8_128x128, 3, 0);
	bgSetPriority(erb2, 1);
	bgSetScroll(erb2, -26, -37);
	bgSetScale(erb2, 1 << 7, 1 << 7);
	vram2 = (u16*)BG_BMP_RAM_SUB(3);

	bgUpdate();

	for(int i=0; i<64*128; ++i)
		vram2[i] = 0;

	memcpy(BG_PALETTE_SUB, nitrotracker_logo_pal_bin, nitrotracker_logo_pal_bin_size);
	for(int y=0; y<17;++y)
	{
		for(int x=0;x<76/2;++x)
		{
			vram2[(128/2)*y+x] = ((u16*)(nitrotracker_logo_bin))[(76*y)/2+x];
		}
	}

	// Fill Random Memory
	randmem = (u8*)malloc(sizeof(u8)*1024*2048);

	for(int i=0; i<1024*2048; ++i)
		randmem[i] = splash_rand() >> 8;

	// Interrupts
	irqEnable(IRQ_VBLANK);
	irqSet(IRQ_VBLANK, splash_vblank);
	irqSet(IRQ_HBLANK, splash_hblank);

	irqEnable(IRQ_VCOUNT);
	SetYtrigger(0);
	irqSet(IRQ_VCOUNT, splash_vcount);

	// Init Sprites

	REG_BLDCNT_SUB = BLEND_ALPHA | BLEND_SRC_SPRITE | BLEND_DST_BG2;
	REG_BLDALPHA_SUB = 16 << 8 | 14; // Nice additive blending!

	memcpy(SPRITE_GFX_SUB, _04_img_bin, _04_img_bin_size);
	memcpy(SPRITE_PALETTE_SUB, _04_pal_bin, _04_pal_bin_size);

	splash_init_sprites();

	sprites[0].attribute[0] = ATTR0_COLOR_16 | ATTR0_TYPE_BLENDED | 52;
	sprites[0].attribute[1] = ATTR1_SIZE_32 | 190;
	sprites[0].attribute[2] = ATTR2_PALETTE(0) | ATTR2_PRIORITY(1);

	splash_update_OAM();

	// Fade in
	for(int i=-16; i <= 0; ++i)
	{
	    setBrightness(3, i);
		swiWaitForVBlank();
	}

	// Wait for the user to do something
	u16 keysdown = 0;
	while(keysdown == 0)
	{
		scanKeys();
		keysdown = keysDown();
		swiWaitForVBlank();
	}

	// Fade out
	for(int i=0; i >= -16; --i)
	{
	    setBrightness(3, i);
		swiWaitForVBlank();
	}

	// Clean up
	irqDisable(IRQ_HBLANK);
	irqDisable(IRQ_VCOUNT);
	irqDisable(IRQ_VBLANK);
	irqClear(IRQ_HBLANK);
	irqClear(IRQ_VBLANK);
	irqClear(IRQ_VCOUNT);

	//consoleClear();

	free(randmem);
}
