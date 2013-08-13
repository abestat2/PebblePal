#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H
	
// Window update todo flags	
#define NOTHING_TO_UPDATE		0x0000
#define UPDATE_BATTERY		 	0x0001
#define UPDATE_TIME				0x0002
#define UPDATE_DATA			 	0x0004
#define UPDATE_WEATHER		 	0x0008
#define UPDATE_UNREAD_GMAIL		0x0010
#define UPDATE_UNREAD_SMS		0x0020
#define UPDATE_MISSED_CALLS		0x0040
#define UPDATE_DATE				0x0080
#define UPDATE_HANGOUT			0x0100
	
// Window manager States
typedef enum {
	WM_INIT = 0,
	WM_BAD,
	WM_MAIN,
}WindowStates;

typedef enum {
	WM_S_MAIN = 0,
	WM_S_MENU,
	WM_S_RINGER,
	WM_S_TEXT_MESSAGE,
}WindowSubStates;

typedef struct {
	Window splashWindow;
	BmpContainer logoContainer;
	GFont splashFont;
	TextLayer versionLayer;
	TextLayer loadingLayer;
}SplashScreen;
	
void init_window_manager(void);	
void deinit_window_manager(void);
void window_manager_mark_layer_for_update(uint16_t);
WindowStates get_window_manager_state(void);
void window_manager_set_state(WindowStates);
void window_manager_mark_layer_done(uint16_t);
void window_manager_push_menu(void);
uint16_t window_manager_get_layers_to_process(void);
void start_appropriate_update(void);
#endif