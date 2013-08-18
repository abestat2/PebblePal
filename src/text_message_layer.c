#include "globals.h"
#include "text_message_layer.h"
#include "window_manager.h"
#include "message.h"
#include "util.h"
#include "timer.h"

	
uint16_t selectedPerson;
	
//***************************************************************************************
//***************************************************************************************
// Texters Menu
//***************************************************************************************
//***************************************************************************************

//---------------------------------------------------------------------------------------
// Private variables and methods	
//---------------------------------------------------------------------------------------
	
static MenuLayer textersMenuLayer;
static Window textersMenuWindow;
static bool texturesLoaded = false;
static char texters[81];
static uint8_t  numTexters = 0;

uint8_t get_num_texters() {
	char * p = &texters[0];
	uint8_t count = 0;
	while(*p != '\0') {
		if(*p == '^') {
			count++;
		}
		p++;
	}
	return count;
}

char * get_sub_string_from_raw_string(uint16_t stringIndex, char * rawString) {
	static char thisBuffer[MAX_TEXTER_STRING_SIZE];
	char * pBuffer = &thisBuffer[0];
	char * p = rawString;
	bool found = false;
	uint16_t count = 0;
	
	while(!found) {
		if(count == stringIndex) {
			if((*p == '^') || (*p == '\0')) {
				*pBuffer = '\0';
				found = true;
			}
			else {
				*pBuffer++ = *p;
			}	
		}
		else if(*p == '^') {
			count++;
		}
		p++;
	}
	return &thisBuffer[0];
}

uint16_t texters_menu_get_num_sections_callback(MenuLayer *me, void *data) {
	return 1;
}

uint16_t texters_menu_get_num_rows_callback(MenuLayer *me, uint16_t section_index, void *data) {
	if(texturesLoaded) {
		return numTexters;
	}
	else {
		return 1;
	}
}

int16_t texters_menu_get_header_height_callback(MenuLayer *me, uint16_t section_index, void *data) {
  // This is a define provided in pebble_os.h that you may use for the default height
  return MENU_CELL_BASIC_HEADER_HEIGHT;
}

void texters_menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
	if(texturesLoaded) {	
		menu_cell_basic_header_draw(ctx,cell_layer,"Send Message To");
	}
	else {
		menu_cell_basic_header_draw(ctx,cell_layer,"Gathering Data...");
	}
}

void texters_menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
	if(texturesLoaded) {	
		menu_cell_basic_draw(ctx,cell_layer,(const char*)get_sub_string_from_raw_string(cell_index->row,&texters[0]),"",NULL);
	}
	else {
		menu_cell_basic_draw(ctx,cell_layer,"Loading...","",NULL);
	}	
}

void texters_menu_select_callback(MenuLayer *me, MenuIndex *cell_index, void *data) {
	selectedPerson = cell_index->row;
	init_messages_message_layer();
}

void texters_window_load(Window *me) {
	GRect bounds = me->layer.bounds;
	menu_layer_init(&textersMenuLayer, bounds);
	menu_layer_set_callbacks(&textersMenuLayer, NULL, (MenuLayerCallbacks) {
		.get_num_sections = texters_menu_get_num_sections_callback,
		.get_num_rows = texters_menu_get_num_rows_callback,
		.get_header_height = texters_menu_get_header_height_callback,
		.draw_header = texters_menu_draw_header_callback,
		.draw_row = texters_menu_draw_row_callback,
		.select_click = texters_menu_select_callback,
	});
	
	menu_layer_set_click_config_onto_window(&textersMenuLayer, me);
	layer_add_child(&textersMenuWindow.layer, menu_layer_get_layer(&textersMenuLayer));
}

void texters_window_unload(Window *me) {
	texturesLoaded = false;
	numTexters = 0;
	menu_layer_reload_data(&textersMenuLayer);
}

//---------------------------------------------------------------------------------------
// Public variables and methods	
//---------------------------------------------------------------------------------------

void send_request_for_texters() {
	uint32_t requestfortexters[] = {1,CMD_TEXT_RECEIVERS};
	if(get_message_state() == FREE) {
		sendNextMessage(&requestfortexters[0]);
	}
	else {
		start_timer(SEND_TEXT_MESSAGE_TIME,SEND_TEXTER_MESSAGE_COOKIE);
	}
}

void text_message_layer_set_texters(char * textersString) {
	uint16_t size = strlen(textersString);
	memcpy(&texters[0],textersString,size);
	texturesLoaded = true;
	numTexters = get_num_texters();
	menu_layer_reload_data(&textersMenuLayer);
}

void init_texters_message_layer() {
	window_init(&textersMenuWindow, "Texters Menu Layer");
	window_set_fullscreen(&textersMenuWindow, true);
	window_set_window_handlers(&textersMenuWindow, (WindowHandlers){
    	.load = texters_window_load,
    	.unload = texters_window_unload,
    });
	window_stack_push(&textersMenuWindow, true);
	send_request_for_texters();

}

//***************************************************************************************
//***************************************************************************************
// Messages Menu
//***************************************************************************************
//***************************************************************************************

//---------------------------------------------------------------------------------------
// Private variables and methods	
//---------------------------------------------------------------------------------------
bool messagesLoaded = false;
char messages[NUMBER_OF_MESSAGES*MESSAGE_STRING_SIZE + 1];
MenuLayer messagesMenuLayer;
Window messagesMenuWindow;
uint16_t selectedText;

uint16_t messages_menu_get_num_sections_callback(MenuLayer *me, void *data) {
	return 1;
}

uint16_t messages_menu_get_num_rows_callback(MenuLayer *me, uint16_t section_index, void *data) {
	if(messagesLoaded) {
		return NUMBER_OF_MESSAGES;
	}
	else {
		return 1;
	}
}

int16_t messages_menu_get_header_height_callback(MenuLayer *me, uint16_t section_index, void *data) {
  // This is a define provided in pebble_os.h that you may use for the default height
  return MENU_CELL_BASIC_HEADER_HEIGHT;
}

void messages_menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
	if(messagesLoaded) {	
		menu_cell_basic_header_draw(ctx,cell_layer,"Message to Send");
	}
	else {
		menu_cell_basic_header_draw(ctx,cell_layer,"Gathering Data...");
	}
}

void messages_menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
	if(messagesLoaded) {	
		menu_cell_basic_draw(ctx,cell_layer,(const char*)get_sub_string_from_raw_string(cell_index->row,&messages[0]),"",NULL);
	}
	else {
		menu_cell_basic_draw(ctx,cell_layer,"Loading...","",NULL);
	}	
}

void messages_menu_select_callback(MenuLayer *me, MenuIndex *cell_index, void *data) {
	selectedText = cell_index->row;
	window_stack_pop(true);
	send_request_to_send_message();
}

void messages_window_load(Window *me) {
	GRect bounds = me->layer.bounds;
	menu_layer_init(&messagesMenuLayer, bounds);
	menu_layer_set_callbacks(&messagesMenuLayer, NULL, (MenuLayerCallbacks) {
		.get_num_sections = messages_menu_get_num_sections_callback,
		.get_num_rows = messages_menu_get_num_rows_callback,
		.get_header_height = messages_menu_get_header_height_callback,
		.draw_header = messages_menu_draw_header_callback,
		.draw_row = messages_menu_draw_row_callback,
		.select_click = messages_menu_select_callback,
	});
	
	menu_layer_set_click_config_onto_window(&messagesMenuLayer, me);
	layer_add_child(&messagesMenuWindow.layer, menu_layer_get_layer(&messagesMenuLayer));
}

void messages_window_unload(Window *me) {
	messagesLoaded = false;
	menu_layer_reload_data(&messagesMenuLayer);
}

//---------------------------------------------------------------------------------------
// Public variables and methods	
//---------------------------------------------------------------------------------------

void send_request_to_send_message() {
	DictionaryIterator *iter;
	if(get_message_state() == FREE) {
		AppMessageResult getBuffResult = app_message_out_get(&iter);
		if(getBuffResult != APP_MSG_OK) {
			start_timer(SEND_TEXT_MESSAGE_TIME,SEND_SMS_MESSAGE_COOKIE);
			return;
		}
		else {
			if (getBuffResult == APP_MSG_INVALID_ARGS) {
				return;
			} 
		}
		char buffer[]= "0^0";
		buffer[0] = '0' + selectedPerson;
		buffer[2] = '0' + selectedText;
		dict_write_cstring(iter, CMD_SEND_TEXT, &buffer[0]);
		
		app_message_out_send();
		app_message_out_release();
	}
	else {
		start_timer(SEND_TEXT_MESSAGE_TIME,SEND_SMS_MESSAGE_COOKIE);
	}
}

void send_request_for_messages() {
	uint32_t requestformessages[] = {1,CMD_TEXT_MESSAGES};
	if(get_message_state() == FREE) {
		sendNextMessage(&requestformessages[0]);
	}
	else {
		start_timer(SEND_TEXT_MESSAGE_TIME,SEND_MESSAGE_MESSAGE_COOKIE);
	}
}

void text_message_layer_set_messages(char * messagesString) {
	uint16_t size = strlen(messagesString);
	memcpy(&messages[0],messagesString,size);
	messagesLoaded = true;
	menu_layer_reload_data(&messagesMenuLayer);
}

void init_messages_message_layer() {
	window_init(&messagesMenuWindow, "Messages Menu Layer");
	window_set_fullscreen(&messagesMenuWindow, true);
	window_set_window_handlers(&messagesMenuWindow, (WindowHandlers){
    	.load = messages_window_load,
    	.unload = messages_window_unload,
    });
	window_stack_push(&messagesMenuWindow, true);
	send_request_for_messages();
}
