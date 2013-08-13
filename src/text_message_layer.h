#ifndef TEXT_MESSAGE_LAYER_H
#define TEXT_MESSAGE_LAYER_H
	
#define STRING_SEPERATOR_CHAR '^'
#define MESSAGE_STRING_SIZE 15
#define MAX_TEXTER_STRING_SIZE 25
#define NUMBER_OF_MESSAGES 5
	
void text_message_layer_set_texters(char*);
void send_request_for_texters(void);	
void init_texters_message_layer(void);
void init_messages_message_layer(void);
void send_request_for_messages(void);
void text_message_layer_set_messages(char*);
void send_request_to_send_message(void);
#endif