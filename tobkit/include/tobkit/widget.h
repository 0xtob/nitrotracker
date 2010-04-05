/**********************
DS Tracker Widget Class
Has rectangular area
**********************/

#ifndef WIDGET_H
#define WIDGET_H

#include <nds.h>
#include <limits.h>

#include "theme.h"

class Widget {
	public:
		// Constructor sets base variables
		Widget(u8 _x, u8 _y, u8 _width, u8 _height, u16 **_vram, bool _visible=true, bool _occluded=true);
		virtual ~Widget(void) {}

		// Callback registration
		// ... is done in the class

		// Drawing request
		virtual void pleaseDraw(void) {};

		// Get/set position
		void getPos(u8 *_x, u8 *_y, u8 *_width, u8 *_height);
		void setPos(u8 _x, u8 _y);

		// Toggle visibility
		// Objects can be hidden either explicitly (using show/hide) or implicitly
		// (using reveal/occlude). Reveal/occlude are used eg if the object is on
		// a tab of a tabbox that is currently not selected. Show/hide is used to
		// hide controls that shall not be seen at the moment.

		virtual void show(void);
		virtual void hide(void);
		bool is_visible(void) { return visible; }

		virtual void occlude(void);
		virtual void reveal(void);
		bool is_occluded(void) { return occluded; }

		// Resize
		void resize(u16 w, u16 h);

		// Toggle enabled/disabled
		virtual void enable(void);
		virtual void disable(void);
		bool is_enabled(void) { return enabled; }

		// Event calls
		virtual void penDown(u8 px, u8 py) {};
		virtual void penUp(u8 px, u8 py) {};
		virtual void penMove(u8 px, u8 py) {};
		virtual void buttonPress(u16 button) {};
		virtual void buttonRelease(u16 button) {};

		virtual void setTheme(Theme *theme_, u16 bgcolor_) { theme = theme_; bgcolor = bgcolor_;}

	protected:
		u16 x, y, width, height;
		bool enabled;
		u16 **vram;
		Theme *theme;
		u16 bgcolor; // Color of the background (for hiding the widget)

		// Draw utility functions
		void drawString(const char* str, u8 tx, u8 ty, u8 maxwidth=255, u16 color=RGB15(0,0,0)|BIT(15));
		void drawBox(u8 tx, u8 ty, u8 tw, u8 th, u16 col=RGB15(0,0,0)|BIT(15));
		void drawFullBox(u8 tx, u8 ty, u8 tw, u8 th, u16 col);
		void drawBorder(u16 col = RGB15(0,0,0)|BIT(15));
		void drawLine(u8 tx, u8 ty, u8 length, u8 horizontal, u16 col);
		void drawBresLine(u8 tx1, u8 ty1, u8 tx2, u8 ty2, u16 col);
		void drawPixel(u8 tx, u8 ty, u16 col);
		void drawGradient(u16 col1, u16 col2, u8 tx, u8 ty, u8 tw, u8 th);

		inline const u16 interpolateColor(u16 col1, u16 col2, u8 alpha) {
			/*
			u8 r1,g1,b1,r2,g2,b2,rn,gn,bn;
			r1 = col1 & 0x001F;
			g1 = (col1 >> 5) & 0x001F;
			b1 = (col1 >> 10) & 0x001F;
			r2 = col2 & 0x001F;
			g2 = (col2 >> 5) & 0x001F;
			b2 = (col2 >> 10) & 0x001F;
			rn = r1*alpha/255 + r2-r2*alpha/255;
			gn = g1*alpha/255 + g2-g2*alpha/255;
			bn = b1*alpha/255 + b2-b2*alpha/255;
			return RGB15(rn,gn,bn)|BIT(15);
			*/

			// This is the above code in 1 Line (saves variable allocation time and mem)
			return RGB15((col1 & 0x001F)*alpha/255 + (col2 & 0x001F)-(col2 & 0x001F)*alpha/255,
				     ((col1 >> 5) & 0x001F)*alpha/255 + ((col2 >> 5) & 0x001F)-((col2 >> 5) & 0x001F)*alpha/255,
				     ((col1 >> 10) & 0x001F)*alpha/255 + ((col2 >> 10) & 0x001F)-((col2 >> 10) & 0x001F)*alpha/255
				     )|BIT(15);
		}

		void drawMonochromeIcon(u8 tx, u8 ty, u8 tw, u8 th, const u8 *icon, u16 color=RGB15(0,0,0)|BIT(15));

		// Stylus utility functions
		bool isInRect(u8 x, u8 y, u8 x1, u8 y1, u8 x2, u8 y2);

		// How wide is the string when rendered?
		u8 getStringWidth(const char *str, u16 limit=USHRT_MAX);

		// Can the widget be seen by the user?
		bool isExposed(void);

	private:
		bool visible, occluded;

		// Overdraw the object with its background color
		void overdraw(void);
};

#endif
