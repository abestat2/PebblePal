#ifndef DATA_LAYER_H
#define DATA_LAYER_H

typedef enum {
	DATA_ICON_DISCONNECTED = 0,
	DATA_ICON_WIFI,
	DATA_ICON_4G,
	DATA_ICON_3G,
	DATA_ICON_2G,
	DATA_ICON_NULL,
}DataIcon;
	
typedef struct {
	Layer baseLayer;
	BmpContainer iconContainer;
	bool iconLoaded;
	DataIcon currentIcon;
}DataLayer;

void data_layer_init(Window*);
void data_layer_deinit(void);
void data_layer_set_icon(DataIcon);
void data_layer_update_display(void);
#endif