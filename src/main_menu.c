#include "globals.h"
#include "main_menu.h"
#include "text_message_layer.h"
#include "ringer_control_layer.h"
#include "settings_menu.h"

MenuLayer menuLayer;
Window menuWindow;	
	
uint16_t main_menu_get_num_sections_callback(MenuLayer *me, void *data) {
	return MAIN_MENU_NUM_SECTIONS;
}

uint16_t main_menu_get_num_rows_callback(MenuLayer *me, uint16_t section_index, void *data) {
	return MAIN_MENU_NUM_ROWS;
}

int16_t main_menu_get_header_height_callback(MenuLayer *me, uint16_t section_index, void *data) {
    return MENU_CELL_BASIC_HEADER_HEIGHT;
}

void main_menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
	menu_cell_basic_header_draw(ctx,cell_layer,"Actions");
}

void main_menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
	switch(cell_index->row) {
		case 0:
			menu_cell_basic_draw(ctx,cell_layer,"Send SMS","Preset Messages",NULL);
			break;
		case 1:
			menu_cell_basic_draw(ctx,cell_layer,"Control Ringer","Volume & Mode",NULL);
			break;
		case 2:
			menu_cell_basic_draw(ctx, cell_layer, "Settings", "Watch App Settings", NULL);
			break;
		default:
			break;
	}
}

void main_menu_select_callback(MenuLayer *me, MenuIndex *cell_index, void *data) {
	switch(cell_index->row) {
		case 0:
			init_texters_message_layer();
			break;
		case 1:
			init_ringer_control_layer();
			break;
		case 2:
			init_settings_menu_layer();
			break;
		default:
			break;
	}
}

void main_menu_window_load(Window *me) {
	GRect bounds = me->layer.bounds;
	menu_layer_init(&menuLayer, bounds);
	menu_layer_set_callbacks(&menuLayer, NULL, (MenuLayerCallbacks) {
		.get_num_sections = main_menu_get_num_sections_callback,
		.get_num_rows = main_menu_get_num_rows_callback,
		.get_header_height = main_menu_get_header_height_callback,
		.draw_header = main_menu_draw_header_callback,
		.draw_row = main_menu_draw_row_callback,
		.select_click = main_menu_select_callback,
	});
	
	menu_layer_set_click_config_onto_window(&menuLayer, me);
	layer_add_child(&me->layer, menu_layer_get_layer(&menuLayer));
}

void main_menu_window_unload(Window *me) {
	return;
}

void init_main_menu_layer() {
	window_init(&menuWindow, "Main Menu Layer");
	window_set_fullscreen(&menuWindow, true);
	window_set_window_handlers(&menuWindow, (WindowHandlers){
    	.load = main_menu_window_load,
    	.unload = main_menu_window_unload,
    });
	window_stack_push(&menuWindow, true);
}
