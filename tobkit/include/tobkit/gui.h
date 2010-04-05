/*******************
DS Tracker GUI Class

Contains the widgets
Gets pen/key events
Calls touched widget
********************/

#ifndef GUI_H
#define GUI_H

#include <nds.h>
#include <vector>
#include <map>
#include "widget.h"
#include "theme.h"

#define MAIN_SCREEN	0
#define SUB_SCREEN	1

class GUI {
	public:
		GUI();

		// Sets the theme - mandatory!
		void setTheme(Theme *theme_, u16 bgcolor_);

		// Adds a widget and specifies which button it listens to
		// Touches on widget's area are redirected to the widget
		void registerWidget(Widget *w, u16 listeningButtons=0, u8 screen = SUB_SCREEN);

		// Removes a widget from the GUI
		void unregisterWidget(Widget *w);

		// Registers a widget that is in top of all other widgets and has input
		// priority, like a popup-window or something.
		void registerOverlayWidget(Widget *w, u16 listeningButtons, u8 screen = SUB_SCREEN);

		// Remove the overlay widget
		void unregisterOverlayWidget(u8 screen = SUB_SCREEN);

		// Event calls
		void penDown(u8 x, u8 y);
		void penUp(u8 x, u8 y); // Remove the coordinates here!
		void penMove(u8 x, u8 y);
		void buttonPress(u16 buttons);
		void buttonRelease(u16 buttons);

		// Draw requests
		void draw(void);
		void drawMainScreen(void);
		void drawSubScreen(void);

		// Screen switch
		void switchScreens(void);
		u8 getActiveScreen(void);

		// Show/Hide all elements
		void showAll(void);
		void hideAll(void);

		void occludeAll(void);
		void revealAll(void);

	private:
		std::vector<Widget*> widgets_main, widgets_sub;
		std::vector<Widget*> shortcuts;
		Widget *activeWidget;
		u8 activeScreen;
		Widget *overlayWidgetMain, *overlayWidgetSub;
		u16 overlayShortcuts;
		Theme *theme;
		u16 bgcolor;

		// Find the widget that got hit
		Widget *getWidgetAt(u8 x, u8 y);
		Widget *getWidgetForButtons(u16 buttons);
};

#endif
