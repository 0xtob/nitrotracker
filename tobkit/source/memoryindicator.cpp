#include <stdio.h>
#include <malloc.h>

#include "tobkit/memoryindicator.h"

/* ===================== PUBLIC ===================== */

MemoryIndicator::MemoryIndicator(u8 _x, u8 _y, u8 _width, u8 _height, u16 **_vram, bool _visible)
	:Widget(_x, _y, _width, _height, _vram, _visible)
{
	total_ram = getFreeMem();
}

MemoryIndicator::~MemoryIndicator()
{
}
// Drawing request
void MemoryIndicator::pleaseDraw(void)
{
	draw();
}

/* ===================== PRIVATE ===================== */


void MemoryIndicator::draw(void)
{
	struct mallinfo mi = mallinfo();
	u32 used_ram = mi.uordblks; 
	
	int boxwidth = (width - 2) * used_ram / total_ram;
	int percentfull = 100 * used_ram / total_ram;
	//printf("%d\n",percentfull );
	
	// Color depends on percentage of full ram
	u16 col;
	if(percentfull < 70)
		col = RGB15(17,24,16) | BIT(15); // Green
	else if(percentfull < 90)
		col = RGB15(31,31,0) | BIT(15); // Yellow
	else
		col = RGB15(31,0,0) | BIT(15); // Red
	
	drawBorder(theme->col_outline);
	drawFullBox(1, 1, width-2, height-2, theme->col_light_bg);
	drawFullBox(1, 1, boxwidth, height-2, col);
}

bool testmalloc(int size)
{
	if(size<=0) return(false);
	
	void *ptr;
	u32 adr;
	
	ptr=malloc(size+(64*1024)); // 64kb
	
	if(ptr==NULL) return(false);
	
	adr=(u32)ptr;
	free(ptr);
	
	if((adr&3)!=0){ // 4byte
		return(false);
	}
	
	if((adr+size)<0x02000000){
		return(false);
	}
	
	if((0x02000000+(4*1024*1024))<=adr){
		return(false);
	}
	
	return(true);
}

#define PrintFreeMem_Seg (10240)

u32 MemoryIndicator::getFreeMem(void)
{
	s32 i;
	u32 FreeMemSize=0;
	
	for(i=1*PrintFreeMem_Seg;i<4096*1024;i+=PrintFreeMem_Seg){
		if(testmalloc(i)==false) break;
		FreeMemSize=i;
	}
	
	return FreeMemSize;
}
