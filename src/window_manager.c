#include "globals.h"
#include "window_manager.h"
#include "time_layer.h"
#include "date_layer.h"
#include "battery_layer.h"
#include "weather_layer.h"
#include "data_layer.h"
#include "util.h"
#include "message_layer.h"
#include "message.h"
#include "app_alerts.h"
#include "button_config.h"
#include "text_message_layer.h"
#include "ringer_control_layer.h"
#include "timer.h"
#include "main_menu.h"
	
//---------------------------------------------------------------------------------------
// Private variables and methods	
//---------------------------------------------------------------------------------------

static uint16_t layersToProcess = NOTHING_TO_UPDATE;
static Window mainWindow;
static WindowStates windowManagerState = WM_INIT;
static Window errorWindow;
static SplashScreen splashScreen;
static WindowSubStates windowManagerSubState = WM_S_MAIN;

void redraw_error_window(Layer* layer, GContext* ctx) {
	graphics_context_set_fill_color(ctx, GColorBlack);
	graphics_context_set_stroke_color(ctx, GColorWhite);
	graphics_fill_rect(ctx, GRect(0,0,144,168), 0,GCornersAll);
	//draw phone
	graphics_draw_round_rect(ctx, GRect(35,20,72,126), 20);
	graphics_draw_round_rect(ctx, GRect(36,21,70,124), 19);
	graphics_draw_round_rect(ctx, GRect(37,22,68,122), 18);
	//draw button
	graphics_draw_round_rect(ctx, GRect(58,129,26,10), 3);
	graphics_draw_round_rect(ctx, GRect(59,130,24,8), 3);
	//draw screen
	graphics_draw_round_rect(ctx,GRect(42,35,58,91),2);
	graphics_draw_round_rect(ctx,GRect(43,36,56,89),2);
	//draw cross-out line
	graphics_draw_line(ctx, GPoint(25,168),GPoint(117,7));
	graphics_draw_line(ctx, GPoint(25,167),GPoint(117,6));
	graphics_draw_line(ctx, GPoint(25,166),GPoint(117,5));
	graphics_draw_line(ctx, GPoint(25,165),GPoint(117,4));
	graphics_draw_line(ctx, GPoint(25,164),GPoint(117,3));
	graphics_draw_line(ctx, GPoint(25,163),GPoint(117,2));
	graphics_draw_line(ctx, GPoint(25,162),GPoint(117,1));
	graphics_draw_line(ctx, GPoint(25,161),GPoint(117,-1));
	graphics_draw_line(ctx, GPoint(25,160),GPoint(117,-2));
	graphics_draw_line(ctx, GPoint(25,159),GPoint(117,-3));
	graphics_draw_line(ctx, GPoint(25,158),GPoint(117,-4));
}

void build_splash_screen(void) {
	window_init(&splashScreen.splashWindow, "Splash Window");
	window_set_fullscreen(&splashScreen.splashWindow, true);
	window_set_background_color(&splashScreen.splashWindow, GColorBlack);
//	window_set_click_config_provider(&splashScreen.splashWindow, (ClickConfigProvider)splash_window_config_provider);
	bmp_init_container(RESOURCE_ID_IMAGE_LOGO,&splashScreen.logoContainer);
	layer_set_frame(&splashScreen.logoContainer.layer.layer, GRect(31,5,81,51));
	layer_add_child(&splashScreen.splashWindow.layer,&splashScreen.logoContainer.layer.layer);
	
	splashScreen.splashFont = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_NEVIS_20));

	text_layer_init(&splashScreen.versionLayer, GRect(0,71,144,25));
	text_layer_set_font(&splashScreen.versionLayer,splashScreen.splashFont);
	text_layer_set_text_alignment(&splashScreen.versionLayer, GTextAlignmentCenter);
	text_layer_set_text_color(&splashScreen.versionLayer, GColorWhite);
	text_layer_set_background_color(&splashScreen.versionLayer, GColorBlack);
	text_layer_set_text(&splashScreen.versionLayer, APP_VERSION);
	layer_add_child(&splashScreen.splashWindow.layer,&splashScreen.versionLayer.layer);
	
	text_layer_init(&splashScreen.loadingLayer, GRect(0,106,144,50));
	text_layer_set_font(&splashScreen.loadingLayer,splashScreen.splashFont);
	text_layer_set_text_alignment(&splashScreen.loadingLayer, GTextAlignmentCenter);
	text_layer_set_text_color(&splashScreen.loadingLayer, GColorWhite);
	text_layer_set_background_color(&splashScreen.loadingLayer, GColorBlack);
	text_layer_set_text(&splashScreen.loadingLayer, "Loading...");
	layer_add_child(&splashScreen.splashWindow.layer,&splashScreen.loadingLayer.layer);	
}

void unload_window_manager_resources(void) {
	layer_remove_from_parent(&splashScreen.logoContainer.layer.layer);
	bmp_deinit_container(&splashScreen.logoContainer);
	fonts_unload_custom_font(splashScreen.splashFont);
}

Window* push_error_window() {
	Window* topWindow = NULL;
	if(!window_stack_contains_window(&errorWindow)) {
		topWindow = window_stack_get_top_window();
		window_stack_pop(true);
		window_stack_push(&errorWindow, true);
	}
	return topWindow;
}

//---------------------------------------------------------------------------------------
// Public variables and methods	
//---------------------------------------------------------------------------------------

void start_appropriate_update(void) {

	if(layersToProcess & UPDATE_TIME) {
		time_layer_update_display();
	}	
	if(layersToProcess & UPDATE_DATE) {
		date_layer_update_display();
	}	
	else if(layersToProcess & UPDATE_WEATHER) {
		weather_layer_update_display();
	}	
	else if(layersToProcess & UPDATE_UNREAD_GMAIL) {
		message_layer_update_gmail();
	}	
	else if(layersToProcess & UPDATE_UNREAD_SMS) {
		message_layer_update_sms();
	}	
	else if(layersToProcess & UPDATE_MISSED_CALLS) {
		message_layer_update_missed_calls();
	}
	else if(layersToProcess & UPDATE_HANGOUT) {
		message_layer_update_hangout();
	}
	else if(layersToProcess & UPDATE_DATA) {
		data_layer_update_display();
	}
	else if(layersToProcess & UPDATE_BATTERY) {
		battery_layer_update_display();
	}
}

void window_manager_push_menu(void) {
	init_main_menu_layer();
}

void init_window_manager(void) {
	layersToProcess = NOTHING_TO_UPDATE;
	windowManagerState = WM_INIT;
	
	window_init(&mainWindow, "PebblePal Main");
	window_set_fullscreen(&mainWindow, true);
	window_set_background_color(&mainWindow, GColorBlack);
	window_set_click_config_provider(&mainWindow, (ClickConfigProvider)main_window_config_provider);
//	window_stack_push(&mainWindow, true);
	
	build_splash_screen();
	window_stack_push(&splashScreen.splashWindow, true);
	
	window_init(&errorWindow, "Error Window");
	window_set_fullscreen(&errorWindow, true);
	window_set_background_color(&errorWindow, GColorBlack);
	errorWindow.layer.update_proc = redraw_error_window;
	
	PblTm tempTime;
	get_time(&tempTime);
	
	message_layer_init(&mainWindow);
	time_layer_init(&mainWindow,&tempTime);
	date_layer_init(&mainWindow, &tempTime);
	battery_layer_init(&mainWindow);
	data_layer_init(&mainWindow);
	weather_layer_init(&mainWindow);
}

void deinit_window_manager(void) {
	time_layer_deinit();
	date_layer_deinit();
	battery_layer_deinit();
	data_layer_deinit();
	weather_layer_deinit();
	unload_window_manager_resources();
	message_layer_deinit();
}

void window_manager_set_state(WindowStates state) {
	static Window * previousWindow = NULL;
	if(state == WM_BAD) {
		if(window_stack_contains_window(&splashScreen.splashWindow)) {
			window_stack_pop(true);
			previousWindow = push_error_window();
			previousWindow = &mainWindow;
		}
		else {
			previousWindow = push_error_window();
		}
		app_alerts_send_alert(LONG_PULSE);
	}
	else if(state == WM_MAIN){
		if(&splashScreen.splashWindow == window_stack_get_top_window()) {
			window_stack_pop(true);
			window_stack_push(&mainWindow, true);
			windowManagerSubState = WM_S_MAIN;
		}
		else if(&errorWindow == window_stack_get_top_window()) {
			window_stack_pop(true);
			window_stack_push(previousWindow, true);
		}
	}
	windowManagerState = state;
}

WindowStates get_window_manager_state() {
	return windowManagerState;
}

void window_manager_mark_layer_for_update(uint16_t layerFlag) {
	layersToProcess |= layerFlag;	
}

void window_manager_mark_layer_done(uint16_t layerFlag) {
	layersToProcess &= ~layerFlag;	
}

uint16_t window_manager_get_layers_to_process() {
	return layersToProcess;
}
