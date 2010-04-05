#ifndef _THEME_H_
#define _THEME_H_

#include <nds.h>

class Theme
{
	public:
		Theme();
		
		u16 col_bg;
		u16 col_dark_bg;
		u16 col_medium_bg;
		u16 col_light_bg;
		u16 col_lighter_bg;
		u16 col_light_ctrl;
		u16 col_dark_ctrl;
		u16 col_light_ctrl_disabled;
		u16 col_dark_ctrl_disabled;
		u16 col_list_highlight1;
		u16 col_list_highlight2;
		u16 col_outline;
		u16 col_sepline;
		u16 col_icon;
		u16 col_text;
		u16 col_signal;
};

#endif
