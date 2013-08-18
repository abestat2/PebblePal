#include "globals.h"
#include "settings_menu.h"
#include "window_manager.h"
	
MenuLayer settingsMenuLayer;
Window settingsMenuWindow;	
	
uint16_t settings_menu_get_num_sections_callback(MenuLayer *me, void *data) {
	return SETTINGS_MENU_NUM_SECTIONS;
}

uint16_t settings_menu_get_num_rows_callback(MenuLayer *me, uint16_t section_index, void *data) {
	return SETTINGS_MENU_NUM_ROWS;
}

int16_t settings_menu_get_header_height_callback(MenuLayer *me, uint16_t section_index, void *data) {
    return MENU_CELL_BASIC_HEADER_HEIGHT;
}

void settings_menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
	menu_cell_basic_header_draw(ctx,cell_layer,"Settings");
}

void settings_menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
	switch(cell_index->row) {
		case 0:
			if(get_window_theme() == DARK) {
				menu_cell_basic_draw(ctx,cell_layer,"Color Theme",LIGHT_SUMMARY_STRING,NULL);
			}
			else {
				menu_cell_basic_draw(ctx,cell_layer,"Color Theme",DARK_SUMMARY_STRING,NULL);
			}
			break;
		default:
			break;
	}
}

void settings_menu_select_callback(MenuLayer *me, MenuIndex *cell_index, void *data) {
	switch(cell_index->row) {
		case 0:
			if(get_window_theme() == DARK) {
				set_window_theme(LIGHT);
			}
			else {
				set_window_theme(DARK);			
			}
			layer_mark_dirty(&me->scroll_layer.layer);
			break;
		default:
			break;
	}
}

void settings_menu_window_load(Window *me) {
	GRect bounds = me->layer.bounds;
	menu_layer_init(&settingsMenuLayer, bounds);
	menu_layer_set_callbacks(&settingsMenuLayer, NULL, (MenuLayerCallbacks) {
		.get_num_sections = settings_menu_get_num_sections_callback,
		.get_num_rows = settings_menu_get_num_rows_callback,
		.get_header_height = settings_menu_get_header_height_callback,
		.draw_header = settings_menu_draw_header_callback,
		.draw_row = settings_menu_draw_row_callback,
		.select_click = settings_menu_select_callback,
	});
	
	menu_layer_set_click_config_onto_window(&settingsMenuLayer, me);
	layer_add_child(&me->layer, menu_layer_get_layer(&settingsMenuLayer));
}

void settings_menu_window_unload(Window *me) {
	return;
}

void init_settings_menu_layer() {
	window_init(&settingsMenuWindow, "Settings Menu Layer");
	window_set_fullscreen(&settingsMenuWindow, true);
	window_set_window_handlers(&settingsMenuWindow, (WindowHandlers){
    	.load = settings_menu_window_load,
    	.unload = settings_menu_window_unload,
    });
	window_stack_push(&settingsMenuWindow, true);
}