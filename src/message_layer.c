#include "globals.h"
#include "message_layer.h"
#include "window_manager.h"
#include "util.h"
#include "font_manager.h"
	
//---------------------------------------------------------------------------------------
// Private variables and methods	
//---------------------------------------------------------------------------------------

MessageLayer messageLayer;
GRect baseLayerBounds = {.origin={.x=13,.y=118},.size={.w=106,.h=52}};
GRect smsImageBounds = {.origin={.x=0,.y=0},.size={.w=26,.h=25}};
GRect smsTextBounds = {.origin={.x=0,.y=25},.size={.w=26,.h=27}};
GRect gmailImageBounds = {.origin={.x=26,.y=0},.size={.w=27,.h=25}};
GRect gmailTextBounds = {.origin={.x=26,.y=25},.size={.w=27,.h=27}};
GRect missedCallImageBounds = {.origin={.x=53,.y=0},.size={.w=27,.h=25}};
GRect missedCallTextBounds = {.origin={.x=53,.y=25},.size={.w=27,.h=27}};
GRect hangoutImageBounds = {.origin={.x=80,.y=0},.size={.w=26,.h=25}};
GRect hangoutTextBounds = {.origin={.x=80,.y=25},.size={.w=26,.h=27}};

void unload_sms_image(void) {
	if(messageLayer.smsImageLoaded) {
		layer_remove_from_parent(&messageLayer.smsImageContainer.layer.layer);
		bmp_deinit_container(&messageLayer.smsImageContainer);
		messageLayer.smsImageLoaded = false;
	}
}

void load_sms_image(Layer* layer, GContext* ctx) {
	unload_sms_image();
	bmp_init_container(RESOURCE_ID_IMAGE_SMS, &messageLayer.smsImageContainer);
	layer_set_frame(&messageLayer.smsImageContainer.layer.layer,GRect(0,0,26,25));
	layer_add_child(layer, &messageLayer.smsImageContainer.layer.layer);
	messageLayer.smsImageLoaded = true;
}

void unload_gmail_image(void) {
	if(messageLayer.gmailImageLoaded) {
		layer_remove_from_parent(&messageLayer.gmailImageContainer.layer.layer);
		bmp_deinit_container(&messageLayer.gmailImageContainer);
		messageLayer.gmailImageLoaded = false;
	}
}

void load_gmail_image(Layer* layer, GContext* ctx) {
	unload_gmail_image();
	bmp_init_container(RESOURCE_ID_IMAGE_ENVELOPE, &messageLayer.gmailImageContainer);
	layer_set_frame(&messageLayer.gmailImageContainer.layer.layer,GRect(0,0,26,25));
	layer_add_child(layer, &messageLayer.gmailImageContainer.layer.layer);
	messageLayer.gmailImageLoaded = true;
}

void unload_phone_image(void) {
	if(messageLayer.missedCallImageLoaded) {
		layer_remove_from_parent(&messageLayer.missedCallImageContainer.layer.layer);
		bmp_deinit_container(&messageLayer.missedCallImageContainer);
		messageLayer.missedCallImageLoaded = false;
	}
}

void load_phone_image(Layer* layer, GContext* ctx) {
	unload_phone_image();
	bmp_init_container(RESOURCE_ID_IMAGE_PHONE, &messageLayer.missedCallImageContainer);
	layer_set_frame(&messageLayer.missedCallImageContainer.layer.layer,GRect(0,0,26,25));
	layer_add_child(layer, &messageLayer.missedCallImageContainer.layer.layer);
	messageLayer.missedCallImageLoaded = true;
}

void unload_hangout_image(void) {
	if(messageLayer.hangoutImageLoaded) {
		layer_remove_from_parent(&messageLayer.hangoutImageContainer.layer.layer);
		bmp_deinit_container(&messageLayer.hangoutImageContainer);
		messageLayer.hangoutImageLoaded = false;
	}
}

void load_hangout_image(Layer* layer, GContext* ctx) {
	unload_hangout_image();
	bmp_init_container(RESOURCE_ID_IMAGE_HANGOUT, &messageLayer.hangoutImageContainer);
	layer_set_frame(&messageLayer.hangoutImageContainer.layer.layer,GRect(0,0,26,25));
	layer_add_child(layer, &messageLayer.hangoutImageContainer.layer.layer);
	messageLayer.hangoutImageLoaded = true;
}

//---------------------------------------------------------------------------------------
// Public variables and methods	
//---------------------------------------------------------------------------------------

void message_layer_init(Window * window) {
	messageLayer.unreadGmails = 0;
	messageLayer.unreadSms = 0;
	messageLayer.missedCalls = 0;
	messageLayer.hangouts = 0;
	//Load font
	messageLayer.messageFont = get_font_15();
	// initialize base layer
	layer_init(&messageLayer.baseLayer, baseLayerBounds);
	layer_add_child(&window->layer, &messageLayer.baseLayer);
	// initalize sms layers
	layer_init(&messageLayer.smsImageLayer, smsImageBounds);
	messageLayer.smsImageLayer.update_proc = load_sms_image;
	layer_add_child(&messageLayer.baseLayer, &messageLayer.smsImageLayer);	
	text_layer_init(&messageLayer.smsTextLayer, smsTextBounds);
	text_layer_set_font(&messageLayer.smsTextLayer, *messageLayer.messageFont);
	text_layer_set_text_alignment(&messageLayer.smsTextLayer, GTextAlignmentCenter);
	text_layer_set_background_color(&messageLayer.smsTextLayer, GColorBlack);
	text_layer_set_text_color(&messageLayer.smsTextLayer, GColorWhite);
	layer_add_child(&messageLayer.baseLayer, &messageLayer.smsTextLayer.layer);
	// initalize gmail layers
	layer_init(&messageLayer.gmailImageLayer, gmailImageBounds);
	messageLayer.gmailImageLayer.update_proc = load_gmail_image;
	layer_add_child(&messageLayer.baseLayer, &messageLayer.gmailImageLayer);	
	text_layer_init(&messageLayer.gmailTextLayer, gmailTextBounds);
	text_layer_set_font(&messageLayer.gmailTextLayer, *messageLayer.messageFont);
	text_layer_set_text_alignment(&messageLayer.gmailTextLayer, GTextAlignmentCenter);
	text_layer_set_background_color(&messageLayer.gmailTextLayer, GColorBlack);
	text_layer_set_text_color(&messageLayer.gmailTextLayer, GColorWhite);
	layer_add_child(&messageLayer.baseLayer, &messageLayer.gmailTextLayer.layer);
	// initalize missed calls layers
	layer_init(&messageLayer.missedCallImageLayer, missedCallImageBounds);
	messageLayer.missedCallImageLayer.update_proc = load_phone_image;
	layer_add_child(&messageLayer.baseLayer, &messageLayer.missedCallImageLayer);	
	text_layer_init(&messageLayer.missedCallTextLayer, missedCallTextBounds);
	text_layer_set_font(&messageLayer.missedCallTextLayer, *messageLayer.messageFont);
	text_layer_set_text_alignment(&messageLayer.missedCallTextLayer, GTextAlignmentCenter);
	text_layer_set_background_color(&messageLayer.missedCallTextLayer, GColorBlack);
	text_layer_set_text_color(&messageLayer.missedCallTextLayer, GColorWhite);
	layer_add_child(&messageLayer.baseLayer, &messageLayer.missedCallTextLayer.layer);
	// initalize hangout layers
	layer_init(&messageLayer.hangoutImageLayer, hangoutImageBounds);
	messageLayer.hangoutImageLayer.update_proc = load_hangout_image;
	layer_add_child(&messageLayer.baseLayer, &messageLayer.hangoutImageLayer);	
	text_layer_init(&messageLayer.hangoutTextLayer, hangoutTextBounds);
	text_layer_set_font(&messageLayer.hangoutTextLayer, messageLayer.messageFont);
	text_layer_set_text_alignment(&messageLayer.hangoutTextLayer, GTextAlignmentCenter);
	text_layer_set_background_color(&messageLayer.hangoutTextLayer, GColorBlack);
	text_layer_set_text_color(&messageLayer.hangoutTextLayer, GColorWhite);
	layer_add_child(&messageLayer.baseLayer, &messageLayer.hangoutTextLayer.layer);

}

void message_layer_deinit(void) {
	unload_sms_image();
	unload_gmail_image();
	unload_phone_image();
	unload_hangout_image();
}

void message_layer_set_sms(uint16_t unreadSms) {
	messageLayer.unreadSms = unreadSms;
}

void message_layer_set_gmail(uint16_t unreadGmail) {
	messageLayer.unreadGmails = unreadGmail;
}

void message_layer_set_hangout(uint16_t unreadHangouts) {
	messageLayer.hangouts = unreadHangouts;
}

void message_layer_set_missed_calls(uint16_t missedCalls) {
	messageLayer.missedCalls = missedCalls;
}

void write_int_to_message_text_layer(char * inBuff, uint16_t variable, TextLayer * textLayer) {
	char* p = inBuff;
	if(variable >= 1000) {
		*p++ = '9';
		*p++ = '9';
		*p++ = '9';
	}
	else {
		if(variable > 100) {
			*p++ = '0' + (uint8_t)((variable % 1000) / 100);
		}
		if((variable % 100) > 10) {
			*p++ = '0' +(uint8_t)((variable % 100) / 10);
		}
		*p++ = '0' + (uint8_t)(variable % 10);
	}
	*p = '\0';
	p = inBuff;
	text_layer_set_text(textLayer, p);
}

void message_layer_update_sms(void) {
	static char smsTemp[4];
	write_int_to_message_text_layer(&smsTemp[0],messageLayer.unreadSms,&messageLayer.smsTextLayer);
	window_manager_mark_layer_done(UPDATE_UNREAD_SMS);
}

void message_layer_update_gmail(void) {
	static char gmailTemp[4];
	write_int_to_message_text_layer(&gmailTemp[0],messageLayer.unreadGmails,&messageLayer.gmailTextLayer);
	window_manager_mark_layer_done(UPDATE_UNREAD_GMAIL);
}

void message_layer_update_hangout(void) {
	static char hangoutTemp[4];
	write_int_to_message_text_layer(&hangoutTemp[0],messageLayer.hangouts,&messageLayer.hangoutTextLayer);
	window_manager_mark_layer_done(UPDATE_HANGOUT);
}

void message_layer_update_missed_calls(void) {
	static char missedCallsTemp[4];
	write_int_to_message_text_layer(&missedCallsTemp[0],messageLayer.missedCalls,&messageLayer.missedCallTextLayer);
	window_manager_mark_layer_done(UPDATE_MISSED_CALLS);
}
