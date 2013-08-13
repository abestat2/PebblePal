#ifndef MESSAGE_H
#define MESSAGE_H
	
#define MAX_BUFFER_ALLOCATION_ATTEMPTS 5
#define COMMANDS_PER_MESSAGE 5
	
typedef enum {
	FREE,
	NEXT_COMMAND,
	AWAITING_ACK_NACK,
	AWAITING_RESPONSE,
}SendMessageState;

typedef enum {
	SET,
	NOT_SET,
}Termination;

typedef enum {
	CONNECTED,
	NOT_CONNECTED,
}AppConnState;

void send_buffer(void);
uint8_t message_get_update_rate(void);
SendMessageState get_message_state(void);
void app_received_dropped(void*, AppMessageResult);
void app_received_msg(DictionaryIterator*, void*);
void app_send_failed(DictionaryIterator*, AppMessageResult, void*);
void app_send_out(DictionaryIterator*, void*);
void sendNextMessage(uint32_t*);
#endif