#include "globals.h"
#include "ringer_control_layer.h"
#include "timer.h"
#include "message.h"
	
//---------------------------------------------------------------------------------------
// Private variables and methods	
//---------------------------------------------------------------------------------------

	
ActionBarLayer actionBarLayer;
RingerStates ringerState = INIT;
Window ringerWindow;
TextLayer volUpText;
TextLayer volDownText;
TextLayer ringerModeText;
TextLayer stateStatusText;
TextLayer ringerStateText;
TextLayer ringerHeaderText;
HeapBitmap volUp;
HeapBitmap volDown;
HeapBitmap ringer;
GFont ringerFont;
uint32_t ringerCommand;
RingerModes currentRingerMode;
bool ringerDirection;
RingerModes modesSequence[] = {NORMAL,VIBRATE,SILENT};

uint8_t get_next_ringer_index(RingerModes ringerMode) {
	bool found = false;
	int i = 0;
	do {
		if(modesSequence[i++] == ringerMode) {
			found = true;
		}
	}while(!found);
	if(i == 3) {
		i = 0;
	}
	return i;
}

void send_command_change_volume(bool increasing) {
	ringerCommand = CMD_RINGER_CHANGE_VOLUME;
	ringerDirection = increasing;
	text_layer_set_text(&ringerStateText, "Setting");
	send_ringer_command();
}

void send_command_set_ringer_mode(RingerModes ringerMode) {
	ringerCommand = CMD_RINGER_MODE_SET;
	currentRingerMode = ringerMode;
	text_layer_set_text(&ringerStateText, "Setting");
	send_ringer_command();
}

void volume_up_click_handler(int index, void* context) {
	send_command_change_volume(true);
}

void ringer_click_handler(int index, void* context) {
	currentRingerMode = modesSequence[get_next_ringer_index(currentRingerMode)];
	send_command_set_ringer_mode(currentRingerMode); 
}

void volume_down_click_handler(int index, void* context) {
	send_command_change_volume(false);
}

void click_config_provider(ClickConfig **config, void *context) {
	config[BUTTON_ID_DOWN]->click.handler = (ClickHandler) volume_down_click_handler;
	config[BUTTON_ID_SELECT]->click.handler = (ClickHandler) ringer_click_handler;
	config[BUTTON_ID_UP]->click.handler = (ClickHandler) volume_up_click_handler;
}

void configTextLayer(TextLayer* layer, uint8_t yLocation, uint8_t height,bool left,GTextAlignment alignment) {
	text_layer_init(layer, GRect((left ? 0 : 62),yLocation,62,height));
	text_layer_set_background_color(layer, GColorWhite);
	text_layer_set_text_color(layer, GColorBlack);
	text_layer_set_font(layer, ringerFont);
	text_layer_set_text_alignment(layer, alignment);
}

void ringer_window_load(Window *me) {
	action_bar_layer_init(&actionBarLayer);
	action_bar_layer_add_to_window(&actionBarLayer, me);
	action_bar_layer_set_click_config_provider(&actionBarLayer, click_config_provider);
	heap_bitmap_init(&volUp, RESOURCE_ID_IMAGE_VOL_UP);
	heap_bitmap_init(&volDown, RESOURCE_ID_IMAGE_VOL_DOWN);
	heap_bitmap_init(&ringer, RESOURCE_ID_IMAGE_RINGER);
	action_bar_layer_set_icon(&actionBarLayer, BUTTON_ID_UP, &volUp.bmp);
	action_bar_layer_set_icon(&actionBarLayer, BUTTON_ID_SELECT, &ringer.bmp);
	action_bar_layer_set_icon(&actionBarLayer, BUTTON_ID_DOWN, &volDown.bmp);
	ringerFont = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_NEVIS_15));
	configTextLayer(&volUpText,17,20,false,GTextAlignmentRight);
	text_layer_set_text(&volUpText, "Vol+");
	layer_add_child(&me->layer, &volUpText.layer);
	configTextLayer(&ringerModeText,64,40,false,GTextAlignmentRight);
	text_layer_set_text(&ringerModeText, "Change Ringer");
	layer_add_child(&me->layer, &ringerModeText.layer);
	configTextLayer(&volDownText,125,20,false,GTextAlignmentRight);
	text_layer_set_text(&volDownText, "Vol-");
	layer_add_child(&me->layer, &volDownText.layer);
	configTextLayer(&ringerHeaderText,0,32,true,GTextAlignmentLeft);
	text_layer_set_text(&ringerHeaderText, "Current State:");
	layer_add_child(&me->layer, &ringerHeaderText.layer);
	configTextLayer(&ringerStateText,32,16,true,GTextAlignmentLeft);
	text_layer_set_text(&ringerStateText, "Loading");
	layer_add_child(&me->layer, &ringerStateText.layer);
}

void ringer_window_unload(Window * me) {
	action_bar_layer_clear_icon(&actionBarLayer,BUTTON_ID_UP);
	action_bar_layer_clear_icon(&actionBarLayer,BUTTON_ID_SELECT);
	action_bar_layer_clear_icon(&actionBarLayer,BUTTON_ID_DOWN);
	heap_bitmap_deinit(&volUp);
	heap_bitmap_deinit(&ringer);
	heap_bitmap_deinit(&volDown);
	action_bar_layer_remove_from_window(&actionBarLayer);
	fonts_unload_custom_font(ringerFont);
}

void displayRingerMode(RingerModes ringerMode) {
	switch(ringerMode) {
		case SILENT:
			text_layer_set_text(&ringerStateText, "Silent");
			break;
		case VIBRATE:
			text_layer_set_text(&ringerStateText, "Vibrate");
			break;
		case NORMAL:
			text_layer_set_text(&ringerStateText, "Normal");			
			break;
	}
}

//---------------------------------------------------------------------------------------
// Public variables and methods	
//---------------------------------------------------------------------------------------

void send_command_get_ringer_mode() {
	ringerCommand = CMD_RINGER_MODE_READ;
	send_ringer_command();
}

void ringer_layer_handle_command(RingerModes ringerMode) {
	if(ringerState == INIT) {
		ringerState = MAIN;
	}
	else if(ringerState == SENDING) {
		ringerState = MAIN;
		currentRingerMode = ringerMode;
	}
	displayRingerMode(ringerMode);
//	start_timer(RINGER_MAIN_REFRESH,RINGER_MAIN_COOKIE);	
}

void init_ringer_control_layer() {
	window_init(&ringerWindow, "Ringer Menu Layer");
	window_set_fullscreen(&ringerWindow, true);
	window_set_window_handlers(&ringerWindow, (WindowHandlers){
    	.load = ringer_window_load,
    	.unload = ringer_window_unload,
    });
	window_stack_push(&ringerWindow, true);
	send_command_get_ringer_mode();
}

void send_ringer_command() {
	DictionaryIterator *iter;
	if(get_message_state() == FREE) {
		AppMessageResult getBuffResult = app_message_out_get(&iter);
		if(getBuffResult != APP_MSG_OK) {
			start_timer(RINGER_GET_BUFFER_RETRY_TIME,RINGER_GET_BUFFER_RETRY_COOKIE);
			return;
		}
		else {
			if (getBuffResult == APP_MSG_INVALID_ARGS) {
				return;
			} 
		}
		if(ringerCommand == CMD_RINGER_CHANGE_VOLUME) {
			dict_write_int16(iter, ringerCommand, (int16_t)(ringerDirection ? 1:0));
		}
		else {
			dict_write_int16(iter, ringerCommand, currentRingerMode);		
		}
		
		app_message_out_send();
		app_message_out_release();
	}
	else {
		start_timer(RINGER_GET_BUFFER_RETRY_TIME,RINGER_GET_BUFFER_RETRY_COOKIE);
	}
}
