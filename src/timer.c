#include "globals.h"
#include "timer.h"
#include "window_manager.h"
#include "message.h"
#include "text_message_layer.h"
#include "ringer_control_layer.h"
	
static AppContextRef ctxRef;
	
void timer_handler(AppContextRef ctx, AppTimerHandle handle, uint32_t cookie) {	
	if(cookie == UPDATE_SCREEN_RATE_COOKIE) {
		if(window_manager_get_layers_to_process() != NOTHING_TO_UPDATE) {
			start_appropriate_update();
		}
		start_timer((uint32_t)WAIT_ON_WINDOW_MS,UPDATE_SCREEN_RATE_COOKIE);
	}
	else if(cookie == WAIT_FOR_BUFFER_COOKIE) {
		send_buffer();
	}
	else if(cookie == COLD_START_TIMER_COOKIE) {
		// only two layers not by a message
		window_manager_mark_layer_for_update(UPDATE_TIME | UPDATE_DATE);	
		send_buffer();	// after startup communicate with phone
		start_timer((uint32_t)WAIT_ON_WINDOW_MS, UPDATE_SCREEN_RATE_COOKIE);
	}
	else if(cookie == SEND_TEXTER_MESSAGE_COOKIE) {
		send_request_for_texters();
	}
	else if(cookie == SEND_MESSAGE_MESSAGE_COOKIE) {
		send_request_for_messages();
	}
	else if(cookie == SEND_SMS_MESSAGE_COOKIE) {
		send_request_to_send_message();
	}
	else if(cookie == RINGER_MAIN_COOKIE) {
		send_command_get_ringer_mode();
	}
	else if(cookie == RINGER_GET_BUFFER_RETRY_COOKIE) {
		send_ringer_command();
	}
}

void start_timer(uint32_t timer_val, uint32_t cookie) {
	app_timer_send_event(ctxRef,timer_val, cookie);
}


void set_app_ctx_ref(AppContextRef ctx) {
	ctxRef = ctx;
}