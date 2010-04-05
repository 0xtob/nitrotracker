#ifndef TOGGLEBUTTON_H
#define TOGGLEBUTTON_H

#include "widget.h"

/* A button that can be toggled on and off, like a switch. It can have a caption and an icon. */
class ToggleButton: public Widget {
	public:
		ToggleButton(u8 _x, u8 _y, u8 _width, u8 _height, u16 **_vram, bool _visible=true);

		// Callback registration
		void registerToggleCallback(void (*onToggle_)(bool));

		// Drawing request
		void pleaseDraw(void);

		// Event calls
		void penDown(u8 x, u8 y);
		void penUp(u8 x, u8 y);
		void buttonPress(u16 button);

		void setCaption(const char *_caption);
		void setBitmap(const u8 *_bmp, int width=13, int height=13);

		void setState(bool _on);
		bool getState(void);

	private:
		void draw(void);

		void (*onToggle)(bool);

		bool penIsDown;
		bool on;
		char *caption;
		bool has_bitmap;
		const u8 *bitmap;
		u8 bmpwidth, bmpheight;
};

#endif
