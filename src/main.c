#include "globals.h"
#include "main.h"
#include "message.h"
#include "window_manager.h"
#include "date_layer.h"
#include "time_layer.h"
#include "util.h"
#include "timer.h"
	
//---------------------------------------------------------------------------------------
// Private variables and methods	
//---------------------------------------------------------------------------------------


#define MY_UUID { 0xA3, 0x51, 0xBD, 0x6D, 0x8C, 0xA7, 0x49, 0xA0, 0x8D, 0xBB, 0x32, 0xBC, 0x5A, 0xF6, 0x5F, 0xA5 }
PBL_APP_INFO(MY_UUID,
             "Pebble Pal", "Davin Nicholas",
             1, 0, /* App version */
             RESOURCE_ID_ICON_APP_IMAGE,
             APP_INFO_STANDARD_APP);

//---------------------------------------------------------------------------------------
// System startup methods	
//---------------------------------------------------------------------------------------

void handle_tick(AppContextRef ctx, PebbleTickEvent* tick) {
	static int day = 0;
	static int updateCounter = 0;
	PblTm time;
	get_time(&time);
	uint8_t updateRate = message_get_update_rate();
	uint16_t updateFlags = NOTHING_TO_UPDATE;
	
	if(get_window_manager_state() != WM_INIT) {	
		if(day != time.tm_yday) { 		//Date needs updated if the day changes
			day = time.tm_yday;
			date_layer_set_date(&time);
			updateFlags |= UPDATE_DATE;
		}
		time_layer_set_time(&time);	// Update time every minute
		updateFlags |= UPDATE_TIME;
		window_manager_mark_layer_for_update(updateFlags);

		// ping for the app every minute when not connected
		if(get_window_manager_state() == WM_BAD) {
			send_buffer();
		}
		else if(updateRate != 0) {
			if(++updateCounter >= updateRate) {  
				updateCounter = 0;
				if(get_message_state() == FREE) { // this check might not be needed
					send_buffer();
				}
			}
		}
	}
}

void handle_init(AppContextRef ctx) {
	resource_init_current_app(&APP_RESOURCES);
	set_app_ctx_ref(ctx);
	init_window_manager();	
	app_timer_send_event(ctx,(uint32_t)500, COLD_START_TIMER_COOKIE);
}

void handle_deinit(AppContextRef ctx) {
	deinit_window_manager();
}


void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
	.init_handler = &handle_init,
	.deinit_handler = &handle_deinit,
	.timer_handler = &timer_handler,
	.tick_info = {
		.tick_handler = &handle_tick,
		.tick_units = MINUTE_UNIT,
	},
	.messaging_info = {
		.buffer_sizes = {
			.inbound = 128, // inbound buffer size in bytes
			.outbound = 128, // outbound buffer size in bytes
		},
		.default_callbacks.callbacks = {
			.out_sent = app_send_out,
			.out_failed = app_send_failed,
			.in_received = app_received_msg,
			.in_dropped = app_received_dropped
		},
	},
};
  app_event_loop(params, &handlers);
}
