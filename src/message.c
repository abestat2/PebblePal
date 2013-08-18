#include "globals.h"
#include "message.h"
#include "window_manager.h"
#include "main.h"
#include "battery_layer.h"
#include "weather_layer.h"
#include "data_layer.h"
#include "date_layer.h"
#include "time_layer.h"
#include "app_alerts.h"
#include "message_layer.h"
#include "text_message_layer.h"
#include "ringer_control_layer.h"
#include "timer.h"
	
//---------------------------------------------------------------------------------------
// Private variables and methods	
//---------------------------------------------------------------------------------------
	
SendMessageState sendStates = FREE;
Termination termination = NOT_SET;
uint32_t sendCommandChkSum;
uint32_t readCommands[] = {	NUM_BUFFER_COMMANDS,			// first element of array is number of parameters to read	
						  	CMD_WEATHER_ICON,
						  	CMD_WEATHER_TEMP_HUMID,
						    /*The parameters below are for app-start only
						    After the first Com then the number of commands
							sent will change to 5, not sending the ones below*/
							CMD_NUM_SMS,
							CMD_NUM_MISSED_CALLS,
						 	CMD_NUM_GMAILS,
						  	CMD_NUM_HANGOUTS,
							CMD_BATTERY,
						  	CMD_PLUGGED_IN,
						  	CMD_DATA_CONN_TYPE,
							CMD_MIL_TIME,
							CMD_BAT_WARN_LEVEL,
							CMD_UPDATE_RATE,
							CMD_VIBERATE,
						    CMD_WATCHFACE_THEME,
						 }; 
uint16_t updateRate = 60;
uint32_t * commandPointer = &readCommands[0];

void sendNextMessage(uint32_t* cmds) {
	uint16_t numCommands = *cmds;
	static uint16_t bufferAllocateAttempts = 0;
	static uint32_t * curCommand = 0;
	uint32_t commandOrderLocation, numCommandsToSend, i, localChkSum;
	DictionaryIterator *iter;
	
	if(sendStates == FREE) {
		sendStates = NEXT_COMMAND;
		curCommand = cmds+1;
		bufferAllocateAttempts = 0;
		termination = NOT_SET;
	}
	
	localChkSum = 0;
	commandOrderLocation = (uint32_t)(curCommand - cmds);
	
	AppMessageResult getBuffResult = app_message_out_get(&iter);
	if(getBuffResult != APP_MSG_OK) {
		bufferAllocateAttempts++;
		if(bufferAllocateAttempts < MAX_BUFFER_ALLOCATION_ATTEMPTS) {
			//set timer to wait for another attempt if needed
			start_timer(WAIT_FOR_BUFFER_TIME,WAIT_FOR_BUFFER_COOKIE);
			return;
		}
		else {
			if (getBuffResult == APP_MSG_INVALID_ARGS) {
//				set_message("Invalid Args");
			} else {
//				set_message("App Message Busy");
			}
			return;
		}
	}
	else {
		bufferAllocateAttempts = 0;
	}
	if((numCommands - commandOrderLocation) < COMMANDS_PER_MESSAGE) {
		numCommandsToSend = numCommands - commandOrderLocation + 1;
	}
	else {
		numCommandsToSend = COMMANDS_PER_MESSAGE;
	}
	
	for(i=0; i<numCommandsToSend; i++) {
		dict_write_uint16(iter,*curCommand,*curCommand);
		localChkSum ^= *curCommand;
		if((curCommand - cmds) >= numCommands) {
			termination = SET;
		}	
		curCommand++;
	}
	sendCommandChkSum = localChkSum;
	sendStates = AWAITING_ACK_NACK;
	app_message_out_send();
	app_message_out_release();
}

void send_watch_version_response_via_timer() {
	start_timer(SEND_APP_VERSION_TIME,SEND_APP_VERSION_COOKIE);
}


//---------------------------------------------------------------------------------------
// Public variables and methods	
//---------------------------------------------------------------------------------------

SendMessageState get_message_state(void) {
	return sendStates;
}

void main_set_update_rate(uint16_t uR) {
	updateRate = uR;	
}

uint8_t message_get_update_rate(void) {
	return updateRate;
}

void send_buffer(void) {
	sendNextMessage(commandPointer);
}

void app_send_out(DictionaryIterator* send, void* context) {
	if(sendStates == AWAITING_ACK_NACK) {
		sendStates = AWAITING_RESPONSE;
		start_gap_timer();
	}
	else {
//		set_message("ACK out of place");
	}
	return;
}

void resetStateMachine() {
	sendStates = FREE;
	sendCommandChkSum = 0;
	readCommands[0] = NUM_BUFFER_COMMANDS;
}

void app_send_failed(DictionaryIterator* failed, AppMessageResult reason, void* context) {
	static int failRetries = 0;
	
	// reset state machine back to starting
	failRetries++;
	resetStateMachine();
	// retry connecting to phone if bad command as long as allowed
	if(failRetries < NACK_RETRY_ATTEMPTS) {
		send_buffer();
	}
	else {
		// connection to phone verified bad. Stop trying until next minute
		window_manager_set_state(WM_BAD);
		failRetries = 0;
	}
}

SendMessageState get_state_machine() {
	return sendStates;
}

void app_received_msg(DictionaryIterator* received, void* context) {
	stop_gap_timer();
	Tuple* receivedTuple = dict_read_first(received);
	uint16_t currentMessageScreenUpdateFlags = DONE;
	uint32_t localChkSum = 0;
	
	while(receivedTuple) {
		localChkSum ^= receivedTuple->key;
		switch (receivedTuple->key) {
			case CMD_DATA_CONN_TYPE:
				data_layer_set_icon(receivedTuple->value->int16);
				currentMessageScreenUpdateFlags |= UPDATE_DATA;
				break;
			case CMD_BATTERY:
				battery_layer_set_battery_level(receivedTuple->value->int16);
				currentMessageScreenUpdateFlags |= UPDATE_BATTERY;
				break;
			case CMD_MIL_TIME:
				time_layer_set_mil_time((bool)receivedTuple->value->int16);
				currentMessageScreenUpdateFlags |= UPDATE_TIME;
				break;
			case CMD_UPDATE_RATE:
				main_set_update_rate(receivedTuple->value->int16);
				break;
			case CMD_BAT_WARN_LEVEL:
				battery_layer_set_warn_level(receivedTuple->value->int16);
				currentMessageScreenUpdateFlags |= UPDATE_BATTERY;
				break;
			case CMD_VIBERATE:
				app_alerts_enabled((bool)receivedTuple->value->int16);
				break;
			case CMD_NUM_SMS:
				message_layer_set_sms(receivedTuple->value->int16);
				currentMessageScreenUpdateFlags |= UPDATE_UNREAD_SMS;
				break;
			case CMD_NUM_MISSED_CALLS:
				message_layer_set_missed_calls(receivedTuple->value->int16);
				currentMessageScreenUpdateFlags |= UPDATE_MISSED_CALLS;
				break;
			case CMD_NUM_GMAILS:
				message_layer_set_gmail(receivedTuple->value->int16);
				currentMessageScreenUpdateFlags |= UPDATE_UNREAD_GMAIL;
				break;
			case CMD_NUM_HANGOUTS:
				message_layer_set_hangout(receivedTuple->value->int16);
				currentMessageScreenUpdateFlags |= UPDATE_HANGOUT;
				break;
			case CMD_WEATHER_ICON:
				weather_layer_set_icon((WeatherImages)receivedTuple->value->int16);
				currentMessageScreenUpdateFlags |= UPDATE_WEATHER;
				break;
			case CMD_WEATHER_TEMP_HUMID:
				weather_layer_set_temperature_humidity_string(receivedTuple->value->cstring,receivedTuple->length);
				currentMessageScreenUpdateFlags |= UPDATE_WEATHER;
				break;
			case CMD_PLUGGED_IN:
				battery_layer_set_plugged_in((bool)receivedTuple->value->int16);
				currentMessageScreenUpdateFlags |= UPDATE_BATTERY;
				break;
			case CMD_TEXT_RECEIVERS:
				text_message_layer_set_texters(receivedTuple->value->cstring);
				break;
			case CMD_TEXT_MESSAGES:
				text_message_layer_set_messages(receivedTuple->value->cstring);
				break;
			case CMD_RINGER_MODE_READ:
			case CMD_RINGER_MODE_SET:
			case CMD_RINGER_CHANGE_VOLUME:
				ringer_layer_handle_command((RingerModes)receivedTuple->value->int16);
				break;
			case CMD_APP_VERSION:
				send_watch_version_response_via_timer();
				break;
			case CMD_WATCHFACE_THEME:
				set_window_theme((WindowThemes)receivedTuple->value->int16);
			default:
				break;
		}
		receivedTuple = dict_read_next(received);
	}
	
	window_manager_mark_layer_for_update(currentMessageScreenUpdateFlags);

	// handle watch update state machine if response is to a wathc request
	// if the phone app sends data on its own then it shouldn't effect the
	// state machine
	if(sendCommandChkSum == localChkSum) {
		if(termination == SET) {
			sendStates = FREE;
			termination = NOT_SET;
			sendCommandChkSum = 0;
			if(get_window_manager_state() != WM_MAIN) {
				window_manager_set_state(WM_MAIN);
				readCommands[0] = 2; // only pull for data not pushed by phone after init
			}
		} 
		else {
			sendStates = NEXT_COMMAND;
			send_buffer();
		}
	}
	return;
}

void app_received_dropped(void* context, AppMessageResult reason) {
	return;
}

void send_app_version() {
	DictionaryIterator *iter;
	if(get_message_state() == FREE) {
		AppMessageResult getBuffResult = app_message_out_get(&iter);
		if(getBuffResult != APP_MSG_OK) {
			start_timer(SEND_APP_VERSION_TIME,SEND_APP_VERSION_COOKIE);
			return;
		}
		else {
			if (getBuffResult == APP_MSG_INVALID_ARGS) {
				return;
			} 
		}
		dict_write_cstring(iter, CMD_APP_VERSION, APP_VERSION_NUMBER);		
		app_message_out_send();
		app_message_out_release();
	}
	else {
		start_timer(SEND_APP_VERSION_TIME,SEND_APP_VERSION_COOKIE);
	}

}

