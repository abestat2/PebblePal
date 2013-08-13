#ifndef MESSAGE_LAYER_H
#define MESAAGE_LAYER_H
	
typedef struct {
	Layer baseLayer;
	Layer gmailImageLayer;
	Layer smsImageLayer;
	Layer missedCallImageLayer;
	Layer hangoutImageLayer;
	TextLayer gmailTextLayer;
	TextLayer smsTextLayer;
	TextLayer missedCallTextLayer;
	TextLayer hangoutTextLayer;
	BmpContainer gmailImageContainer;
	BmpContainer smsImageContainer;
	BmpContainer missedCallImageContainer;
	BmpContainer hangoutImageContainer;
	uint16_t unreadGmails;
	uint16_t unreadSms;
	uint16_t missedCalls;
	uint16_t hangouts;
	bool gmailImageLoaded;
	bool smsImageLoaded;
	bool missedCallImageLoaded;
	bool hangoutImageLoaded;
	GFont messageFont;
}MessageLayer;

void message_layer_init(Window*);
void message_layer_deinit(void);
void message_layer_update_sms(void);
void message_layer_update_gmail(void);
void message_layer_update_hangout(void);
void message_layer_update_missed_calls(void);
void message_layer_set_sms(uint16_t);
void message_layer_set_gmail(uint16_t);
void message_layer_set_hangout(uint16_t);
void message_layer_set_missed_calls(uint16_t);

#endif