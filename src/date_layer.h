#ifndef DATE_LAYER_H
#define DATE_LAYER_H
	
typedef struct {
	TextLayer layer;
	char date[12];
}DateLayer;

void date_layer_set_date(PblTm*);
void date_layer_init(Window*,PblTm*);
void date_layer_deinit(void);
void date_layer_update_display(void);

#endif