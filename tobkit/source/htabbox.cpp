#include "tobkit/htabbox.h"

/* ===================== PUBLIC ===================== */

HTabBox::HTabBox(u8 _x, u8 _y, u8 _width, u8 _height, u16 **_vram, bool _visible)
	:Widget(_x, _y, _width, _height, _vram, _visible),
	currenttab(0)
{
	onTabChange = 0;
}

void HTabBox::addTab(const u8 *icon)
{
	icons.push_back(icon);
	GUI gui;
	gui.setTheme(theme, theme->col_light_bg);
	guis.push_back(gui);
}

// Adds a widget and specifies which button it listens to
// Touches on widget's area are redirected to the widget
void HTabBox::registerWidget(Widget *w, u16 listeningButtons, u8 tabidx, u8 screen)
{
	guis.at(tabidx).registerWidget(w, listeningButtons, screen);

	if(tabidx!=currenttab) {
		w->occlude();
	} else {
		w->reveal();
	}
}

// Event calls
void HTabBox::penDown(u8 px, u8 py)
{
	// If it's on the tabs
	if(px-x<=13)
	{
		u8 tab_hit;
		tab_hit = (py-y-1)/14;
		if(tab_hit<guis.size())
		{
			currenttab = tab_hit;
			draw();
			updateVisibilities();
			if(onTabChange!=0) {
				onTabChange(currenttab);
			}
		}
	} else {
		// If its in the box
		guis.at(currenttab).penDown(px,py);
	}
}

void HTabBox::penUp(u8 px, u8 py) {
	guis.at(currenttab).penUp(px,py);
}

void HTabBox::penMove(u8 px, u8 py) {
	// If it's on the tabs

	// If its in the box
	guis.at(currenttab).penMove(px,py);
}

void HTabBox::buttonPress(u16 buttons) {
	guis.at(currenttab).buttonPress(buttons);
}

// Callback registration
void HTabBox::registerTabChangeCallback(void (*onTabChange_)(u8 tab)) {
	onTabChange = onTabChange_;
}

// Drawing request
void HTabBox::pleaseDraw(void) {
	draw();
}

void HTabBox::show(void)
{
	Widget::show();
	guis.at(currenttab).showAll();
}

void HTabBox::hide(void)
{
	Widget::hide();
	guis.at(currenttab).hideAll();
}

void HTabBox::reveal(void)
{
	Widget::reveal();
	guis.at(currenttab).revealAll();
}

void HTabBox::occlude(void)
{
	Widget::occlude();
	guis.at(currenttab).occludeAll();
}

void HTabBox::setTheme(Theme *theme_, u16 bgcolor_)
{
	theme = theme_;
	bgcolor = bgcolor_;

	for(u8 tab_id=0; tab_id<guis.size(); ++tab_id)
	{
		guis.at(tab_id).setTheme(theme_, theme->col_light_bg);
	}
}

/* ===================== PRIVATE ===================== */
#include <stdio.h>
void HTabBox::draw(void)
{
	// Draw box
	drawFullBox(14, 1, width-15, height-2, theme->col_light_bg);
	drawBox(13, 0, width-13, height);

	// Draw tabs
	u16 black = RGB15(0,0,0)|BIT(15);

	drawFullBox(0, 0, 3, 3+13*guis.size(), theme->col_light_bg);

	for(u8 tabidx=0;tabidx<guis.size();++tabidx) {
		if(tabidx==currenttab) {
			drawFullBox(1, 2+13*tabidx, 13, 13, theme->col_light_bg);
			drawLine(1, 2+13*tabidx, 13, 1, black);
			drawLine(0, 3+13*tabidx, 12, 0, black);
			drawLine(1, 15+13*tabidx, 13, 1, black);
			drawMonochromeIcon(2, 4+13*tabidx, 11, 11, icons.at(tabidx));
		} else {
			drawFullBox(3, 3+13*tabidx, 10, 13, theme->col_medium_bg);
			drawLine(3, 2+13*tabidx, 11, 1, black);
			drawLine(2, 3+13*tabidx, 12, 0, black);
			drawLine(3, 15+13*tabidx, 11, 1, black);
			drawMonochromeIcon(4, 4+13*tabidx, 11, 11, icons.at(tabidx));
			drawFullBox(14, 4+13*tabidx, 1, 11, theme->col_light_bg);
		}
	}

	// Draw gui
	guis.at(currenttab).draw();
}

void HTabBox::updateVisibilities(void)
{
	for(u8 tab_id=0; tab_id<guis.size(); ++tab_id) {
		if(tab_id != currenttab) {
			guis.at(tab_id).occludeAll();
		}
	}
	guis.at(currenttab).revealAll();
}
