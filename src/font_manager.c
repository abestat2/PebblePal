#include "globals.h"
#include "font_manager.h"
	
GFont fontStyled15;
GFont font20;
GFont font15;
GFont fontStyledBold40;
GFont fontStyled20;
	
GFont * get_font_styled_15(void) {
	return &fontStyled15;
}

GFont * get_font_20(void){
	return &font20;
}

GFont * get_font_15(void){
	return &font15;
}
GFont * get_font_styled_bold_40(void){
	return &fontStyledBold40;
}
GFont * get_font_styled_20(void) {
	return &fontStyled20;
}

void init_fonts(void) {
	fontStyled15 = fonts_load_custom_font(resource_get_handle(FONT_BASE_STYLED_15));
	font20 = fonts_load_custom_font(resource_get_handle(FONT_BASE_20));
	font15 = fonts_load_custom_font(resource_get_handle(FONT_BASE_15));
	fontStyledBold40 = fonts_load_custom_font(resource_get_handle(FONT_BASE_STYLED_BOLD_40));
	fontStyled20 = fonts_load_custom_font(resource_get_handle(FONT_BASE_STYLED_20));
}

void deinit_fonts(void) {
	fonts_unload_custom_font(fontStyled15);
	fonts_unload_custom_font(font20);
	fonts_unload_custom_font(font15);
	fonts_unload_custom_font(fontStyledBold40);
	fonts_unload_custom_font(fontStyled20);	
}