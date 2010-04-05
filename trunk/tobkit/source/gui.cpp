#include "tobkit/gui.h"

#include <stdio.h>

/* ===================== PUBLIC ===================== */

GUI::GUI()
	:activeWidget(0), activeScreen(SUB_SCREEN), overlayWidgetMain(0),
	overlayWidgetSub(0), overlayShortcuts(0)
{
	u8 i;
	for(i=0;i<14;++i) {
		shortcuts.push_back(0);
	}
}

// Sets the theme - mandatory!
void GUI::setTheme(Theme *theme_, u16 bgcolor_)
{
	theme = theme_;
	bgcolor = bgcolor_;

	for(std::vector<Widget*>::iterator w_it=widgets_sub.begin();w_it!=widgets_sub.end();++w_it)
	{
		(*w_it)->setTheme(theme_, bgcolor);
	}

	for(std::vector<Widget*>::iterator w_it=widgets_main.begin();w_it!=widgets_main.end();++w_it)
	{
		(*w_it)->setTheme(theme_, bgcolor);
	}
}

// Adds a widget and secifies which button it listens to
// Touches on widget's area are redirected to the widget
void GUI::registerWidget(Widget *w, u16 listeningButtons, u8 screen)
{
	if(screen == MAIN_SCREEN)
		widgets_main.push_back(w);
	else
		widgets_sub.push_back(w);

	if(listeningButtons != 0)
	{
		u8 i;
		for(i=0;i<14;++i)
		{
			if(listeningButtons & BIT(i))
				shortcuts[i] = w;
		}
	}

	w->setTheme(theme, bgcolor);
}

// Removes a widget from the GUI
void GUI::unregisterWidget(Widget *w)
{
  if(activeWidget == w) {
    activeWidget = 0;
  }

  std::vector<Widget*>::iterator wit = widgets_main.begin();
  for(;wit!=widgets_main.end()&&*wit != w; ++wit);
  if( *wit == w) {
    widgets_main.erase(wit);
  }

  wit = widgets_sub.begin();
  for(;wit!=widgets_sub.end()&&*wit != w; ++wit);
  if( *wit == w) {
    widgets_sub.erase(wit);
  }

  //TODO: This only deletes 1 shortcut
  wit = shortcuts.begin();
  for(;wit!=shortcuts.end()&&*wit == w; ++wit);
  if( *wit == w) {
    shortcuts.erase(wit);
  }
}

// Registers a widget that is in top of all other widgets and has input
// priority, like a popup-window or something.
void GUI::registerOverlayWidget(Widget *w, u16 listeningButtons, u8 screen)
{
	if(screen == SUB_SCREEN) {
		overlayWidgetSub = w;
	} else {
		overlayWidgetMain = w;
	}
	overlayShortcuts = listeningButtons;

	w->setTheme(theme, bgcolor);
}

// Remove the overlay widget
void GUI::unregisterOverlayWidget(u8 screen)
{
	if(screen == SUB_SCREEN) {
	        if(activeWidget==overlayWidgetSub) {
		  activeWidget = 0;
	        }
		overlayWidgetSub = 0;
	} else {
	        if(activeWidget==overlayWidgetMain) {
		  activeWidget = 0;
		}
		overlayWidgetMain = 0;
	}
	overlayShortcuts = 0;
}

// Event calls
void GUI::penDown(u8 x, u8 y)
{
	Widget *w = getWidgetAt(x,y);
	if(w!=0) {
		activeWidget = w;
		w->penDown(x, y);
	}
}

void GUI::penUp(u8 x, u8 y)
{
	if(activeWidget!=0) {
		if(activeWidget->is_visible()==true) {
			activeWidget->penUp(x, y);
			activeWidget = 0;
		}
	}
}

void GUI::penMove(u8 x, u8 y) {
	// Check if the pen moved off the active widget
	/*
	if((activeWidget!=0)&&(activeWidget->is_visible()==true)) {
		u8 ax, ay, aw, ah;
		activeWidget->getPos(&ax, &ay, &aw, &ah);
		if((x<ax)||(x>ax+aw)||(y<ay)||(y>ay+ah)) {
			activeWidget->penUp(x, y);
			activeWidget = 0;
		}
	}*/

	// If the pen is on a new widget give it a move event
	/*
	Widget *w = getWidgetAt(x,y);
	if(w!=0) {
		w->penMove(x, y);
	}*/
	if(activeWidget!=0)
		activeWidget->penMove(x, y);
}

void GUI::buttonPress(u16 buttons)
{
	Widget *w = getWidgetForButtons(buttons);
	if(w!=0) {
		if(w->is_visible()==true) {
			w->buttonPress(buttons);
		}
	}
}

void GUI::buttonRelease(u16 buttons)
{
	Widget *w = getWidgetForButtons(buttons);
	if(w!=0) {
		if(w->is_visible()==true) {
			w->buttonRelease(buttons);
		}
	}
}

// Draw request
void GUI::draw(void)
{
	drawMainScreen();
	drawSubScreen();
}

void GUI::drawMainScreen(void)
{
	std::vector<Widget*>::reverse_iterator w_it;

	for(w_it=widgets_main.rbegin();w_it!=widgets_main.rend();++w_it)
	{
		if((*w_it)->is_visible() && !(*w_it)->is_occluded())
			(*w_it)->pleaseDraw();
	}

	if(overlayWidgetMain!=0)
		overlayWidgetMain->pleaseDraw();
}

void GUI::drawSubScreen(void)
{
	u8 visible = 0;

	std::vector<Widget*>::reverse_iterator w_it;

	for(w_it=widgets_sub.rbegin();w_it!=widgets_sub.rend();++w_it)
	{
		if((*w_it)->is_visible() && !(*w_it)->is_occluded() ) {
			(*w_it)->pleaseDraw();
			visible++;
		}
	}

	if(overlayWidgetSub!=0)
		overlayWidgetSub->pleaseDraw();
}

void GUI::switchScreens(void) {
	activeScreen = 1-activeScreen;
}

u8 GUI::getActiveScreen(void) {
	return activeScreen;
}

// Show/Hide all elements
void GUI::showAll(void)
{
	for(std::vector<Widget*>::reverse_iterator w_it=widgets_main.rbegin(); w_it!=widgets_main.rend(); ++w_it) {
		(*w_it)->show();
	}

	for(std::vector<Widget*>::reverse_iterator w_it=widgets_sub.rbegin(); w_it!=widgets_sub.rend(); ++w_it) {
		(*w_it)->show();
	}
}

void GUI::hideAll(void)
{
	for(std::vector<Widget*>::reverse_iterator w_it=widgets_main.rbegin(); w_it!=widgets_main.rend(); ++w_it) {
		(*w_it)->hide();
	}

	for(std::vector<Widget*>::reverse_iterator w_it=widgets_sub.rbegin(); w_it!=widgets_sub.rend(); ++w_it) {
		(*w_it)->hide();
	}
}

void GUI::occludeAll(void)
{
	for(std::vector<Widget*>::reverse_iterator w_it=widgets_main.rbegin(); w_it!=widgets_main.rend(); ++w_it) {
		(*w_it)->occlude();
	}

	for(std::vector<Widget*>::reverse_iterator w_it=widgets_sub.rbegin(); w_it!=widgets_sub.rend(); ++w_it) {
		(*w_it)->occlude();
	}
}

void GUI::revealAll(void)
{
	for(std::vector<Widget*>::reverse_iterator w_it=widgets_main.rbegin(); w_it!=widgets_main.rend(); ++w_it) {
		(*w_it)->reveal();
	}

	for(std::vector<Widget*>::reverse_iterator w_it=widgets_sub.rbegin(); w_it!=widgets_sub.rend(); ++w_it) {
		(*w_it)->reveal();
	}
}

/* ===================== PRIVATE ===================== */

// Find the widget that got hit
// Does notreturn invisible widgets
Widget *GUI::getWidgetAt(u8 x, u8 y) {

	u8 wx, wy, ww, wh;

	// Do we have an overlay?
	if((activeScreen == MAIN_SCREEN)&&(overlayWidgetMain!=0)) {
		return overlayWidgetMain;
	}

	if((activeScreen == SUB_SCREEN)&&(overlayWidgetSub!=0)) {
		return overlayWidgetSub;
	}

	// Else follow the normal procedure
	bool found = false;
	std::vector<Widget*>::iterator w_it, end_it;
	if(activeScreen == MAIN_SCREEN) {
		w_it = widgets_main.begin();
		end_it = widgets_main.end();
	} else {
		w_it = widgets_sub.begin();
		end_it = widgets_sub.end();
	}
	while((w_it!=end_it) && (!found)) {
		(*w_it)->getPos(&wx, &wy, &ww, &wh);
		if((x>wx)&&(x<wx+ww)&&(y>wy)&&(y<wy+wh)&&((*w_it)->is_visible()==true)) {
			found = true;
		} else {
			w_it++;
		}
	}
	if(!found) {
		return 0;
	} else {
		return *w_it;
	}
}

Widget *GUI::getWidgetForButtons(u16 buttons) {

	if((overlayWidgetMain!=0)&&(overlayShortcuts&buttons)) {
		return overlayWidgetMain;
	}
	if((overlayWidgetSub!=0)&&(overlayShortcuts&buttons)) {
		return overlayWidgetSub;
	}

	Widget *w=0;
	u8 i;
	for(i=0;i<14;++i) {
		if(buttons & BIT(i)) {
			w = shortcuts[i];
		}
	}
	return w;
}
