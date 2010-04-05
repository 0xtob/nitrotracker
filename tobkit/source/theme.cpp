#include "tobkit/theme.h"

Theme::Theme(void)
{
	// Set default colors
	col_bg                  = RGB15(4,6,15)|BIT(15);
	col_dark_bg             = col_bg;
	col_medium_bg           = RGB15(9,11,17)|BIT(15);
	col_light_bg            = RGB15(16,18,24)|BIT(15);
	col_lighter_bg          = RGB15(23,25,31) | BIT(15);
	col_light_ctrl          = RGB15(31,31,0)|BIT(15); // RGB15(26,26,26)|BIT(15)
	col_dark_ctrl           = RGB15(31,18,0)|BIT(15); // RGB15(31,31,31)|BIT(15)
	col_light_ctrl_disabled = col_light_bg;
	col_dark_ctrl_disabled  = col_medium_bg;
	col_list_highlight1     = RGB15(28,15,0)|BIT(15);
	col_list_highlight2     = RGB15(28,28,0)|BIT(15);
	col_outline             = RGB15(0,0,0)|BIT(15);
	col_sepline             = RGB15(31,31,0)|BIT(15);
	col_icon                = RGB15(0,0,0)|BIT(15);
	col_text                = RGB15(0,0,0)|BIT(15);
	col_signal              = RGB15(31,0,0)|BIT(15);
}
