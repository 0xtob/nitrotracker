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

#ifndef _TOOLS_H_
#define _TOOLS_H_

#include <stdlib.h>
#include <stdio.h>
#include <fat.h>
#include <nds.h>

// A collection of utilities for everyday DS coding

#define DMA0FILL	(*(vuint32*)0x040000E0)

#define min(x,y) ((x)<(y)?(x):(y))
#define max(x,y) ((x)>(y)?(x):(y))

void lowercase(char *str);
void dbgWaitButton(void);

void PrintFreeMem(void);
void printMallInfo(void);

// The fastest known clear method on the DS known to man (or to me, at least)
// I found out that the version in libnds doesn't use the special fill
// register that has fast, cache-less read access.
inline void dmaFillWordsDamnFast(const u32 word, void* dest, uint32 size)
{
	DMA0FILL = word;
	DMA_SRC(0) = (u32)(&DMA0FILL);
	DMA_DEST(0) = (uint32)dest;
	DMA_CR(0) = DMA_SRC_FIX | DMA_COPY_WORDS | size >> 2;
	while(DMA_CR(0) & DMA_BUSY);
}

#endif
