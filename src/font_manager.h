#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H
	
#define FONT_BASE_STYLED_15			RESOURCE_ID_FONT_NEVIS_15
#define FONT_BASE_20				RESOURCE_ID_FONT_TIMES_ROMAN_20
#define FONT_BASE_15				RESOURCE_ID_FONT_TIMES_ROMAN_15
#define FONT_BASE_STYLED_BOLD_40	RESOURCE_ID_FONT_NEVIS_BOLD_40
#define FONT_BASE_STYLED_20			RESOURCE_ID_FONT_NEVIS_20
	
GFont * get_font_styled_15(void);
GFont * get_font_20(void);
GFont * get_font_15(void);
GFont * get_font_styled_bold_40(void);
GFont * get_font_styled_20(void);

void init_fonts(void);
void deinit_fonts(void);

#endif