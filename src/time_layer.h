#ifndef TIME_LAYER_H
#define TIME_LAYER_H
	
typedef struct {
	TextLayer layer;
	PblTm time;
	char timeStr[5];
	bool milTime;
}TimeLayer;

void time_layer_init(Window*,PblTm*);
void time_layer_set_time(PblTm*);
void time_layer_deinit(void);
void time_layer_set_mil_time(bool);
void time_layer_update_display(void);
	
#endif