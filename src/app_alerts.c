#include "globals.h"
#include "app_alerts.h"

//---------------------------------------------------------------------------------------
// Private variables and methods	
//---------------------------------------------------------------------------------------

bool alertsEnabled = true;

//---------------------------------------------------------------------------------------
// Public variables and methods	
//---------------------------------------------------------------------------------------
	
void app_alerts_send_alert(VibeModes mode) {
	if(alertsEnabled) {
		switch(mode) {
		case SHORT_PULSE:
			vibes_short_pulse();
			break;
		case LONG_PULSE:
			vibes_long_pulse();
			break;
		case DOUBLE_PULSE:
			vibes_double_pulse();
			break;
		default:
			break;
		}
	}
}

void app_alert_warn_disconnected(void) {
//	data_layer_set_icon(0);
//	battery_layer_update_battery_level(0);
//	battery_layer_set_plugged_in(false);
//	battery_layer_update_screen();
//	weather_layer_clear_info();
}

void app_alerts_enabled(bool alertsEn) {
	if(alertsEn && !alertsEnabled) {
		vibes_double_pulse(); // alert user the setting took place
	}
	alertsEnabled = alertsEn;
}