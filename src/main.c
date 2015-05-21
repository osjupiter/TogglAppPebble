#include <pebble.h>
#include <main.h>
#include<core.h>
#include<main_window.h>
#include<history_window.h>


void updateCurrentTimer(struct tm *tick_time, TimeUnits units_changed) {
	static char buf[64] = "";
	last_time = tick_time;
	snprintf(buf, sizeof(buf), "A timer for '%s' is running for %s", Gdescription, calculateDuration(tick_time,Gduration));
    set_content_string(buf);
}

void registerSecondTimeUnit() {
	tick_timer_service_subscribe(SECOND_UNIT ,updateCurrentTimer);
}

void in_received_handler(DictionaryIterator *it, void *context) {
	APP_LOG(APP_LOG_LEVEL_INFO, "Goodie I received somethingy!");	
	Tuple *currentTimer = dict_find(it,APPMESS_start);
	Tuple *timezoneOffset = dict_find(it,APPMESS_offset);
  Tuple *historyList = dict_find(it,APPMESS_history);
	if ( currentTimer) {
		if ( strcmp(currentTimer->value->cstring,"1") == 0)  {
			APP_LOG(APP_LOG_LEVEL_INFO, "Goodie I received a running timer");	
			Tuple *id = dict_find(it,APPMESS_id);
			Tuple *duration = dict_find(it,APPMESS_duration);
			Tuple *description = dict_find(it,APPMESS_description);
			updateSysCurrentTimer(id,duration,description);
      registerSecondTimeUnit();
			setStopActionBar();
		} else {
			APP_LOG(APP_LOG_LEVEL_INFO, "Goodie I received not a timer at all!");
			tick_timer_service_unsubscribe();
			setStartActionBar();
      set_content_string("No timer is currently running");
		}
	} else if ( timezoneOffset ) {
		TIME_ZONE_OFFSET = timezoneOffset->value->int32;
		APP_LOG(APP_LOG_LEVEL_INFO,"Timezone offset set! %d",TIME_ZONE_OFFSET);
	} else if (historyList){
    APP_LOG(APP_LOG_LEVEL_INFO,"Get History!!");
    window_stack_push(create_history_window(it), true);
    
  }
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Finished inhandler");
}

void in_dropped_handler(AppMessageResult reason, void *context) {
	APP_LOG(APP_LOG_LEVEL_WARNING, "Something was received incorrectly");
}

void out_sent_handler(DictionaryIterator *it, void *context) {
	APP_LOG(APP_LOG_LEVEL_INFO, "I sent somethingy to theh interwebs");
}

void out_failed_handler(DictionaryIterator *it, AppMessageResult reasion, void *context) {
	APP_LOG(APP_LOG_LEVEL_WARNING, "Something was sent incorrectly");
}

void accel_tap_handler(AccelAxisType axis, int32_t direction) {
	get();
	set_content_string("Fetching newest data from Toggl... Hang on");
}

void handle_init(void) {

	app_message_register_inbox_received(in_received_handler);
	app_message_register_inbox_dropped(in_dropped_handler);
	app_message_register_outbox_sent(out_sent_handler);
	app_message_register_outbox_failed(out_failed_handler);
	
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
    
	accel_tap_service_subscribe(&accel_tap_handler);
	
    window_stack_push(create_main_window(), true);
	
}

int main(void) {
	  handle_init();
	  app_event_loop();
}