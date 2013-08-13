#include "globals.h"
#include "data_layer.h"
#include "window_manager.h"
#include "app_alerts.h"
	
//---------------------------------------------------------------------------------------
// Private variables and methods	
//---------------------------------------------------------------------------------------
	
const uint8_t dataIcons[] = {
	RESOURCE_ID_ICON_DISCONNECTED,
	RESOURCE_ID_ICON_WIFI,
	RESOURCE_ID_ICON_4G,
	RESOURCE_ID_ICON_3G,
	RESOURCE_ID_ICON_2G,
};

DataLayer dataLayer;
GRect iconDataLayerBounds = {.origin={.x=118,.y=142},.size={.w=25,.h=25}};

void redraw_data_layer(Layer* layer, GContext* ctx) {
	static DataIcon previousIcon = DATA_ICON_NULL;
	if(dataLayer.currentIcon != previousIcon) {
		previousIcon = dataLayer.currentIcon;
		if(dataLayer.iconLoaded) {
			layer_remove_from_parent(&dataLayer.iconContainer.layer.layer);
			bmp_deinit_container(&dataLayer.iconContainer);
			dataLayer.iconLoaded = false;
		}
		bmp_init_container(dataIcons[dataLayer.currentIcon], &dataLayer.iconContainer);
		layer_set_frame(&dataLayer.iconContainer.layer.layer,GRect(0,0,25,25));
		layer_add_child(&dataLayer.baseLayer, &dataLayer.iconContainer.layer.layer);
		dataLayer.iconLoaded = true;
		if((get_window_manager_state() == WM_MAIN) && (dataLayer.currentIcon == DATA_ICON_DISCONNECTED)){
			app_alerts_send_alert(DOUBLE_PULSE);
		}
	}
	window_manager_mark_layer_done(UPDATE_DATA);
}


//---------------------------------------------------------------------------------------
// Public variables and methods	
//---------------------------------------------------------------------------------------

void data_layer_update_display(void) {
	layer_mark_dirty(&dataLayer.baseLayer);
}

void data_layer_set_icon(DataIcon iconToWrite) {
	dataLayer.currentIcon = iconToWrite;
}

void data_layer_init(Window* window) {
	dataLayer.currentIcon = DATA_ICON_DISCONNECTED;
	dataLayer.iconLoaded = false;
	layer_init(&dataLayer.baseLayer, iconDataLayerBounds);
	dataLayer.baseLayer.update_proc = redraw_data_layer;
	layer_add_child(&window->layer, &dataLayer.baseLayer);
}

void data_layer_deinit(void) {
	if(dataLayer.iconLoaded) {
		layer_remove_from_parent(&dataLayer.iconContainer.layer.layer);
		bmp_deinit_container(&dataLayer.iconContainer);
		dataLayer.iconLoaded = false;		
	}
}
