#ifndef BATTERY_LAYER_H
#define BATTERY_LAYER_H
	
#define DONE		0x00
#define BASE_LAYER	0x01
#define METER_LAYER	0x02
#define ICON_LAYER	0x04
	
typedef struct {
	Layer baseLayer;
	Layer iconLayer;
	Layer meterLayer;
	uint16_t batteryLevel;
	BmpContainer iconContainer;
	uint16_t batteryWarnLevel;
	bool imageLoaded;
	bool pluggedIn;
}BatteryLayer;

void battery_layer_init(Window*);
void battery_layer_deinit(void);
void battery_layer_set_battery_level(uint16_t);
void battery_layer_set_warn_level(uint16_t);	
void battery_layer_set_plugged_in(bool);
void battery_layer_update_display(void);

#endif