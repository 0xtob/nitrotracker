#include <stdlib.h>
#include <stdarg.h>

#include "tobkit/messagebox.h"

#include <cstring>
#include <cstdlib>

#define MB_MIN_WIDTH	150
#define MB_HEIGHT	46

/* ===================== PUBLIC ===================== */

// Takes a list of alternating button captions and callbacks
MessageBox::MessageBox(u16 **_vram, const char *message, u8 n_buttons, ...)
	:Widget((SCREEN_WIDTH-MB_MIN_WIDTH)/2, (SCREEN_HEIGHT-MB_HEIGHT)/2, MB_MIN_WIDTH, MB_HEIGHT, _vram),
	n_buttons(n_buttons)
{
	msg = (char*)calloc(1, strlen(message)+1);
	strcpy(msg, message);

	// Go through the list of given captions/callback pointers and add a button for each of them
	callbacks = (void (**)(void))malloc(sizeof(void (*)(void))*n_buttons);

	va_list marker;
	va_start(marker, n_buttons);

	// Calculate width
	width = 10;
	char *str;
	void (*onPush)(void);
	for(u8 i=0;i<n_buttons;++i)
	{
		str = va_arg(marker, char*);
		onPush = va_arg(marker, void (*)(void));
		width += getStringWidth(str) + 14;
	}
	va_end(marker);

	u8 fixedbuttonwidth = 0;
	if(width < MB_MIN_WIDTH)
	{
		width = MB_MIN_WIDTH;
		if(n_buttons>0) {
			fixedbuttonwidth = (width - 10) / n_buttons - 10;
		}
	} else if(width > 256) {
		width = 256;
	}

	// Set x
	x = (SCREEN_WIDTH-width)/2;

	// Position label and buttons
	//u8 labelx = x+(width-getStringWidth(message))/2;
	//label = new Label(labelx, y+2, width-20, 12, _vram, false);
	//label->setCaption(message);
	//gui.registerWidget(label, 0);

	if(n_buttons > 0)
	{
		u8 xpos = x + 10;
		buttons = (Button**)malloc(sizeof(Button*)*n_buttons);
		u8 buttonwidth;

		va_start(marker, n_buttons);

		for(u8 i=0; i<n_buttons; ++i)
		{
			char* caption = va_arg(marker, char*);
			void (*onPush)(void) = va_arg(marker, void (*)(void));

			if(fixedbuttonwidth == 0) {
				buttonwidth = getStringWidth(caption)+4;
			} else {
				buttonwidth = fixedbuttonwidth;
			}
			buttons[i] = new Button(xpos, y+24, buttonwidth, 14, _vram, true);
			gui.registerWidget(buttons[i], 0);
			buttons[i]->setCaption(caption);
			buttons[i]->registerPushCallback(onPush);

			xpos += buttonwidth + 10;
		}

		va_end(marker);

	}
}

MessageBox::~MessageBox(void)
{
	free(msg);

	//delete label;
	free(callbacks);
	for(u8 i=0; i<n_buttons; ++i) {
		delete buttons[i];
	}
	if(n_buttons > 0) {
		free(buttons);
	}
}


// Drawing request
void MessageBox::pleaseDraw(void) {
	draw();
}


// Event calls
void MessageBox::penDown(u8 x, u8 y) {
	gui.penDown(x, y);
}

void MessageBox::penUp(u8 x, u8 y) {
	gui.penUp(x, y);
}

void MessageBox::show(void)
{
	gui.showAll();
	if(!isExposed())
	{
		Widget::show();
		pleaseDraw();
	}
}

void MessageBox::reveal(void)
{
	Widget::reveal();
	gui.revealAll();
}

void MessageBox::setTheme(Theme *theme_, u16 bgcolor_)
{
	theme = theme_;
	bgcolor = bgcolor_;
	gui.setTheme(theme, theme->col_light_bg);
}

/* ===================== PRIVATE ===================== */

void MessageBox::draw(void)
{
	drawGradient(theme->col_list_highlight1, theme->col_list_highlight2, 0, 0, width, 16);
	drawLine(0,16, width, 1, RGB15(0,0,0)|BIT(15));
	drawFullBox(0, 17, width, MB_HEIGHT-17, theme->col_light_bg);
	drawBorder();
	u8 labelx = (width-getStringWidth(msg))/2;
	drawString(msg, labelx, 3, width);
	gui.draw();
}
