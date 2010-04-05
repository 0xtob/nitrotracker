#include "tobkit/tabbox.h"

/* ===================== PUBLIC ===================== */

TabBox::TabBox(u8 _x, u8 _y, u8 _width, u8 _height, u16 **_vram, bool _visible)
	:Widget(_x, _y, _width, _height, _vram, _visible),
	currenttab(0)
{
	onTabChange = 0;
}

void TabBox::addTab(const u8 *icon)
{
	icons.push_back(icon);
	GUI gui;
	gui.setTheme(theme, theme->col_light_bg);
	guis.push_back(gui);
}

// Adds a widget and specifies which button it listens to
// Touches on widget's area are redirected to the widget
void TabBox::registerWidget(Widget *w, u16 listeningButtons, u8 tabidx, u8 screen)
{
	guis.at(tabidx).registerWidget(w, listeningButtons, screen);
	
	if(tabidx!=currenttab) {
		w->occlude();
	} else {
		if(isExposed())
			w->reveal();
	}
}

// Event calls
void TabBox::penDown(u8 px, u8 py)
{
	// If it's on the tabs
	if(py-y<=18) {
		u8 tab_hit;
		tab_hit = (px-x-1)/19;
		if(tab_hit<guis.size()) {
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

void TabBox::penUp(u8 px, u8 py) {
	guis.at(currenttab).penUp(px,py);
}

void TabBox::penMove(u8 px, u8 py) {
	// If it's on the tabs
	
	// If its in the box
	guis.at(currenttab).penMove(px,py);	
}

void TabBox::buttonPress(u16 buttons) {
	guis.at(currenttab).buttonPress(buttons);
}

// Callback registration
void TabBox::registerTabChangeCallback(void (*onTabChange_)(u8 tab)) {
	onTabChange = onTabChange_;
}

// Drawing request
void TabBox::pleaseDraw(void)
{
	draw();
	updateVisibilities();
}

void TabBox::show(void)
{
	Widget::show();
	guis.at(currenttab).showAll();
}

void TabBox::hide(void)
{
	Widget::hide();
	guis.at(currenttab).hideAll();
}

void TabBox::occlude(void)
{
	Widget::occlude();
	guis.at(currenttab).occludeAll();
}

void TabBox::reveal(void)
{
	Widget::reveal();
	guis.at(currenttab).revealAll();
}


void TabBox::setTheme(Theme *theme_, u16 bgcolor_)
{
	theme = theme_;
	bgcolor = bgcolor_;
	
	for(u8 tab_id=0; tab_id<guis.size(); ++tab_id)
	{
		guis.at(tab_id).setTheme(theme_, theme->col_light_bg);
	}
}

/* ===================== PRIVATE ===================== */

void TabBox::draw(void)
{
	// Draw box
	drawFullBox(1, 19, width-2, height-20, theme->col_light_bg);
	drawBox(0, 18, width, height-18);
	
	// Draw tabs
	u16 black = RGB15(0,0,0)|BIT(15);
	
	drawFullBox(0, 0, 3+19*guis.size(), 3, theme->col_dark_bg);
	
	for(u8 tabidx=0;tabidx<guis.size();++tabidx) {
		if(tabidx==currenttab) {
			drawFullBox(3+19*tabidx, 1, 18, 18, theme->col_light_bg);
			drawLine(2+19*tabidx, 1, 18, 0, black);
			drawLine(3+19*tabidx, 0, 18, 1, black);
			drawLine(21+19*tabidx, 1, 18, 0, black);
			drawMonochromeIcon(4+19*tabidx, 2, 16, 16, icons.at(tabidx));
		} else {
			drawFullBox(3+19*tabidx, 3, 18, 15, theme->col_medium_bg);
			drawLine(2+19*tabidx, 3, 16, 0, black);
			drawLine(3+19*tabidx, 2, 18, 1, black);
			drawLine(21+19*tabidx, 3, 16, 0, black);
			drawMonochromeIcon(4+19*tabidx, 4, 16, 14, icons.at(tabidx));
		}
	}
	
	// Draw gui
	guis.at(currenttab).draw();
}

void TabBox::updateVisibilities(void)
{
	for(u8 tab_id=0; tab_id<guis.size(); ++tab_id) {
		if(tab_id != currenttab) {
			guis.at(tab_id).occludeAll();
		}
	}
	guis.at(currenttab).revealAll();
}
