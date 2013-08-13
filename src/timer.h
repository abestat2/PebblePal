#ifndef TIMER_H
#define TIMER_H
	
//timer timeout in ms
#define RINGER_GET_BUFFER_RETRY_TIME 	100
#define SEND_TEXT_MESSAGE_TIME			100
#define WAIT_ON_WINDOW_MS				100
#define WAIT_FOR_BUFFER_TIME 			50

// timer cookies (random unique numbers)
#define WAIT_FOR_BUFFER_COOKIE 			0x11001100
#define UPDATE_SCREEN_RATE_COOKIE		0xB98A104F	
#define COLD_START_TIMER_COOKIE	 		0xC92A104F
#define RINGER_GET_BUFFER_RETRY_COOKIE	0x15151515
#define RINGER_MAIN_COOKIE				0x51515151
#define SEND_TEXTER_MESSAGE_COOKIE 		0x20202020
#define SEND_MESSAGE_MESSAGE_COOKIE 	0x40404040
#define SEND_SMS_MESSAGE_COOKIE 		0x41414141

void start_timer(uint32_t, uint32_t);
void timer_handler(AppContextRef, AppTimerHandle, uint32_t);
void set_app_ctx_ref(AppContextRef ctx);
#endif