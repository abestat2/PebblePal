#ifndef RINGER_CONTROL_LAYER_H
#define RINGER_CONTROL_LAYER_H
	
typedef enum {
	SILENT = 0,
	VIBRATE,
	NORMAL,
}RingerModes;

typedef enum {
	INIT = 0,
	MAIN,
	SENDING,
}RingerStates;

void init_ringer_control_layer(void);
void ringer_layer_handle_command(RingerModes);
void send_ringer_command();
void ringer_control_set_status_text_from_int(uint16_t);
void send_command_get_ringer_mode(void);
#endif