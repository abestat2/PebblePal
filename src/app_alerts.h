#ifndef APP_ALERTS_H
#define APP_ALERTS_H
	
typedef enum {
	SHORT_PULSE,
	LONG_PULSE,
	DOUBLE_PULSE,
}VibeModes;

void app_alerts_send_alert(VibeModes);
void app_alerts_enabled(bool);
void app_alert_warn_disconnected(void);

#endif