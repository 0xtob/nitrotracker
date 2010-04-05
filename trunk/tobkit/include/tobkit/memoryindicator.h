#ifndef MEMORYINDICATOR_H_
#define MEMORYINDICATOR_H_

#include "widget.h"

class MemoryIndicator : public Widget
{
	public:
		MemoryIndicator(u8 _x, u8 _y, u8 _width, u8 _height, u16 **_vram, bool _visible=true);
		~MemoryIndicator();
	
	// Drawing request
	void pleaseDraw(void);
			
	private:
		void draw(void);
		u32 getFreeMem(void);
		
		u32 total_ram;
};

#endif /*MEMORYINDICATOR_H_*/
