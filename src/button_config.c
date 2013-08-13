#include "globals.h"
#include "window_manager.h"
#include "button_config.h"

void button_main_down(ClickConfigProvider recognizer, Window* window) {
	light_enable_interaction();
}

void button_main_up(ClickConfigProvider recognizer, Window* window) {
	light_enable_interaction();
}

void button_main_select(ClickConfigProvider recognizer, Window* window) {
	window_manager_push_menu();	
}
	
void main_window_config_provider(ClickConfig** config, Window* window)  {
	config[BUTTON_ID_DOWN]->click.handler = (ClickHandler) button_main_down;
	config[BUTTON_ID_UP]->click.handler = (ClickHandler) button_main_up;
	config[BUTTON_ID_SELECT]->click.handler = (ClickHandler) button_main_select;
}
