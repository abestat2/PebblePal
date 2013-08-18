#include "globals.h"
#include "battery_layer.h"
#include "window_manager.h"
#include "app_alerts.h"
#include "util.h"
	
//---------------------------------------------------------------------------------------
// Private variables and methods	
//---------------------------------------------------------------------------------------

BatteryLayer batteryLayer;
GRect layerBounds = {.origin = {.x=0, .y=0}, .size={.w=13, .h=168}};
GRect meterBounds = {.origin = {.x=1, .y=26}, .size={.w=11, .h=140}};
GRect iconBatLayerBounds = {.origin={.x=1, .y=2}, .size={.w=11, .h=21}};
const uint8_t batteryIcons[] = {
	RESOURCE_ID_ICON_BATTERY,
	RESOURCE_ID_ICON_BATTERY_LOW,
	RESOURCE_ID_ICON_BATTERY_PLUGGED,
};
uint8_t drawingProgressFlags = DONE;

void reload_icon(int icon_index) {
	if(batteryLayer.imageLoaded) {
		layer_remove_from_parent(&batteryLayer.iconContainer.layer.layer);
		bmp_deinit_container(&batteryLayer.iconContainer);
		batteryLayer.imageLoaded = false;
	}
	bmp_init_container(batteryIcons[icon_index], &batteryLayer.iconContainer);	
	layer_set_frame(&batteryLayer.iconContainer.layer.layer,GRect(0,0,11,21));
	layer_add_child(&batteryLayer.iconLayer, &batteryLayer.iconContainer.layer.layer);
	batteryLayer.imageLoaded = true;
}

void set_battery_icon(void) {
	static bool firstAlert = true;
	bool batteryWarn = batteryLayer.batteryLevel <= batteryLayer.batteryWarnLevel;
	if(!batteryLayer.pluggedIn) {
		reload_icon((int)batteryWarn);
		if(get_window_manager_state() == WM_MAIN) {
			if(batteryWarn && firstAlert){
				app_alerts_send_alert(DOUBLE_PULSE);
				firstAlert = false;
			}
			else if(!batteryWarn) {
				firstAlert = true;
			}
		}
	}
	else {
		reload_icon((int)2);
	}
}

void battery_layer_write_meter(Layer* layer, GContext* ctx) {
	GCornerMask cornerMask;
	float batteryPercent = (float)batteryLayer.batteryLevel / (float)100.0;
	uint8_t yNew = /*meterBounds.origin.y + */(uint8_t)(((float)1.00 - batteryPercent)*(float)(meterBounds.size.h));
	uint8_t hNew = (uint8_t)(batteryPercent * (float)(meterBounds.size.h));
	
	if((meterBounds.size.h - hNew) < 4) {
		cornerMask = GCornersAll;
	}
	else {
		cornerMask = GCornersBottom;
	}

	graphics_context_set_fill_color(ctx, GColorBlack);
	graphics_fill_rect(ctx, GRect(0,yNew,meterBounds.size.w,hNew), 4, cornerMask);
	
	graphics_context_set_stroke_color(ctx, GColorBlack);
	graphics_draw_round_rect(ctx, batteryLayer.meterLayer.bounds, 4);
	graphics_draw_round_rect(ctx,GRect(batteryLayer.meterLayer.bounds.origin.x+1,batteryLayer.meterLayer.bounds.origin.y+1,
									   batteryLayer.meterLayer.bounds.size.w-2,batteryLayer.meterLayer.bounds.size.h-2),4);
}

void clear_and_check_done_updating(uint8_t flag) {
	drawingProgressFlags &= ~flag;
	if(drawingProgressFlags == DONE) {
		window_manager_mark_layer_done(UPDATE_BATTERY);
	}
}

void redraw_battery_meter(Layer* layer, GContext* ctx) {
	graphics_context_set_fill_color(ctx, GColorWhite);
	graphics_fill_rect(ctx, layer->bounds, 4, GCornersAll);
	battery_layer_write_meter(layer, ctx);
	clear_and_check_done_updating(METER_LAYER);	
}

void redraw_battery_icon(Layer* layer, GContext *ctx) {
	set_battery_icon();
	clear_and_check_done_updating(ICON_LAYER);
}

void redraw_base_layer(Layer* layer, GContext *ctx) {
	graphics_context_set_fill_color(ctx, GColorWhite);
	graphics_fill_rect(ctx, layer->bounds, 0, GCornerNone);
	clear_and_check_done_updating(BASE_LAYER);
}

//---------------------------------------------------------------------------------------
// Public variables and methods	
//---------------------------------------------------------------------------------------

void battery_layer_update_display(void) {
	drawingProgressFlags = BASE_LAYER | METER_LAYER | ICON_LAYER;
	layer_mark_dirty(&batteryLayer.baseLayer);
}

void battery_layer_set_plugged_in(bool pluggedIn) {
	batteryLayer.pluggedIn = pluggedIn;
}

void battery_layer_set_warn_level(uint16_t warnLevel) {
	batteryLayer.batteryWarnLevel = warnLevel;
}

void battery_layer_set_battery_level(uint16_t level) {
	batteryLayer.batteryLevel = level;
}

void battery_layer_init(Window * window) {
	drawingProgressFlags = DONE;
	batteryLayer.batteryLevel = 0;
	batteryLayer.imageLoaded = false;
	batteryLayer.pluggedIn = false;	

	// Load Meter Backround Layer
	layer_init(&batteryLayer.baseLayer,layerBounds);
	layer_add_child(&window->layer,&batteryLayer.baseLayer);
	// Load icon base layer
	layer_init(&batteryLayer.iconLayer, iconBatLayerBounds);
	layer_add_child(&batteryLayer.baseLayer, &batteryLayer.iconLayer);	
	// Load meter layer
	layer_init(&batteryLayer.meterLayer,meterBounds);
	layer_add_child(&batteryLayer.baseLayer, &batteryLayer.meterLayer);
	// Set update functions
	batteryLayer.baseLayer.update_proc = redraw_base_layer;
	batteryLayer.iconLayer.update_proc = redraw_battery_icon;
	batteryLayer.meterLayer.update_proc = redraw_battery_meter;
}	
	
void battery_layer_deinit(void) {
	if(batteryLayer.imageLoaded) {
		layer_remove_from_parent(&batteryLayer.iconContainer.layer.layer);
		bmp_deinit_container(&batteryLayer.iconContainer);
		batteryLayer.imageLoaded = false;
	}
}
