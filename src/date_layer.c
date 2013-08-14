#include "globals.h"
#include "date_layer.h"
#include "window_manager.h"
#include "util.h"

//---------------------------------------------------------------------------------------
// Private variables and methods	
//---------------------------------------------------------------------------------------
	
char * monthString[] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
char * dayString[] = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
GRect dateLayerBounds = {.origin={.x=13,.y=40},.size={.w=131,.h=25}};
GFont dateFont;
DateLayer dateLayer;

//---------------------------------------------------------------------------------------
// Public variables and methods	
//---------------------------------------------------------------------------------------

void date_layer_set_date(PblTm* time){

	char *destination = &dateLayer.date[0];
	int i = 0;
	for(i=0;i<3;i++) {
		*destination++ = dayString[time->tm_wday][i];
	}
	*destination++ = ',';
	*destination++ = ' ';
	char * temp = itoa(time->tm_mday);
	if(time->tm_mday < 10){
		*destination++ = '0';
		*destination++ = *temp;
	}
	else {
		*destination++ = *temp;
		*destination++ = *(temp+1);
	}
	*destination++ = ' ';
	for(i=0;i<3;i++) {
		*destination++ = monthString[time->tm_mon][i];
	}
	*destination = 0;
}


void date_layer_update_display(void) {
	text_layer_set_text(&dateLayer.layer,&dateLayer.date[0]);
	window_manager_mark_layer_done(UPDATE_DATE);
}

void date_layer_init(Window* window, PblTm* time) {
	date_layer_set_date(time);
	text_layer_init(&dateLayer.layer, dateLayerBounds);
	dateFont = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TIMES_ROMAN_20));
	text_layer_set_font(&dateLayer.layer, dateFont);
	text_layer_set_background_color(&dateLayer.layer, GColorBlack);
	text_layer_set_text_color(&dateLayer.layer, GColorWhite);
	text_layer_set_text_alignment(&dateLayer.layer, GTextAlignmentCenter);
	layer_add_child(&window->layer,&dateLayer.layer.layer);
}

void date_layer_deinit(void) {
	fonts_unload_custom_font(dateFont);
}