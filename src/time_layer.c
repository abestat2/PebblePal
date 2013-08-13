#include "globals.h"
#include "time_layer.h"
#include "window_manager.h"

//---------------------------------------------------------------------------------------
// Private variables and methods	
//---------------------------------------------------------------------------------------
	
TimeLayer timeLayer;
GRect timeLayerBounds = {.origin={.x=13,.y=0},.size={.w=131,.h=40}};
GFont timeFont;	
void format_time_in_string(void);
bool first = true;

void time_layer_set_time(PblTm* time) {
	timeLayer.time = *time;	
}

void format_time_in_string(void) {
	char * p = &timeLayer.timeStr[0];	
	if(!timeLayer.milTime) {
		int nonMilHour = timeLayer.time.tm_hour % 12;
		if(nonMilHour == 0){
			*p++ = '1';
			*p++ = '2';
		}
		else {
			if((nonMilHour / 10) != 0){
				*p++ = '0' + (nonMilHour / 10);
			}
			*p++ = '0'+(nonMilHour % 10);
		}
		*p++ = ':';
		if(timeLayer.time.tm_min < 10) {
			*p++ = '0';
			//memcpy(&timeLayer.timeStr[p-&timeLayer.timeStr[0]],itoa(timeLayer.time.tm_min),1);
		} 
		else {
			*p++ = '0' + (int)(timeLayer.time.tm_min / 10);
	//		memcpy(&timeLayer.timeStr[p-&timeLayer.timeStr[0]],itoa(timeLayer.time.tm_min),2);
		}
		*p++ = '0' + (timeLayer.time.tm_min % 10);
		
		if((p-&timeLayer.timeStr[0]) <= 4) {
			*p = '\0';	//terminate the screen
		}
	}
	else {
		timeLayer.timeStr[0] = '0' + (int)(timeLayer.time.tm_hour / 10);
		timeLayer.timeStr[1] = '0' + (int)(timeLayer.time.tm_hour % 10);
		timeLayer.timeStr[2] = ':';
		timeLayer.timeStr[3] = '0' + (int)(timeLayer.time.tm_min / 10);
		timeLayer.timeStr[4] = '0' + (int)(timeLayer.time.tm_min % 10);
	}
}

//---------------------------------------------------------------------------------------
// Public variables and methods	
//---------------------------------------------------------------------------------------

void time_layer_set_mil_time(bool milTime) {
	timeLayer.milTime = milTime;
}

void time_layer_update_display(void) {
	format_time_in_string();
	text_layer_set_text(&timeLayer.layer, &timeLayer.timeStr[0]);
	window_manager_mark_layer_done(UPDATE_TIME);
}

void time_layer_init(Window* window, PblTm* time) {
	first = true;
	timeLayer.milTime = false;
	time_layer_set_time(time);
	text_layer_init(&timeLayer.layer, timeLayerBounds);
	timeFont = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_NEVIS_BOLD_40));
	text_layer_set_font(&timeLayer.layer, timeFont);
	text_layer_set_background_color(&timeLayer.layer, GColorBlack);
	text_layer_set_text_color(&timeLayer.layer, GColorWhite);
	text_layer_set_text_alignment(&timeLayer.layer, GTextAlignmentCenter);
	layer_add_child(&window->layer,&timeLayer.layer.layer);
}

void time_layer_deinit(void) {
	fonts_unload_custom_font(timeFont);
}
