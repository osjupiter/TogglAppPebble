#include <pebble.h>
#include "core.h"



int GcurrentTid;
uint32_t Gduration;
struct tm *last_time;
char *Gdescription;
int TIME_ZONE_OFFSET=0;
  bool nowFetch=false;

char* calculateDuration(struct tm *current_time, int duration) {
	unsigned int unix_time;
    unsigned int running_time;
    /* Convert time to seconds since epoch. */
    unix_time = ((0-TIME_ZONE_OFFSET)*3600) + /* time zone offset */ 
              + current_time->tm_sec /* start with seconds */
              + current_time->tm_min*60 /* add minutes */
              + current_time->tm_hour*3600 /* add hours */
              + current_time->tm_yday*86400 /* add days */
              + (current_time->tm_year-70)*31536000 /* add years since 1970 */
              + ((current_time->tm_year-69)/4)*86400 /* add a day after leap years, starting in 1973 */
              - ((current_time->tm_year-1)/100)*86400 /* remove a leap day every 100 years, starting in 2001 */
              + ((current_time->tm_year+299)/400)*86400; /* add a leap day back every 400 years, starting in 2001*/

    running_time = unix_time+duration;
	
	static char buf[64] = "";
	
	if (  running_time/(60*60) > 0 ) {
		snprintf(buf, sizeof(buf), "%u:%02u:%02u", running_time/(60*60), (running_time/60)%60, running_time%60);
	} else if ((running_time/60) > 0 ) {
		snprintf(buf, sizeof(buf), "%u:%02u", (running_time/60)%60, running_time%60);
	} else {
		snprintf(buf, sizeof(buf), "%02u seconds", running_time%60);
	}
	
	return buf;
}


void updateSysCurrentTimer(Tuple *id, Tuple *duration, Tuple *description) {
	// Discover the current timer and display the duration and description
	// If no timer is present, offer the possibility to start a new one
	// Other wise offer the possibility to stop the current timer
	int idValue = id->value->int32;
	int durationValue = duration->value->uint32;
	char *descValue = description->value->cstring;
	
	GcurrentTid = idValue;
	Gduration = durationValue;
	Gdescription = descValue;
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Id is %d and duration is %d",idValue,durationValue);
	
	//registerSecondTimeUnit();
}

void start(char *desc) {
	DictionaryIterator *it;
	app_message_outbox_begin(&it);
	Tuplet p1 = TupletInteger(APPMESS_start, 1);
  dict_write_tuplet(it, &p1);
  if(desc){
    	Tuplet p2 = TupletCString(APPMESS_description, desc);
      dict_write_tuplet(it, &p2);
  }

	
	app_message_outbox_send();
}
void stop() {
  int id=GcurrentTid;
	DictionaryIterator *it;
	app_message_outbox_begin(&it);
	Tuplet p1 = TupletInteger(APPMESS_stop, 1);
	Tuplet p2 = TupletInteger(APPMESS_id, GcurrentTid);

	dict_write_tuplet(it, &p1);
	dict_write_tuplet(it, &p2);
	app_message_outbox_send();
}

void get() {
	DictionaryIterator *it;
	app_message_outbox_begin(&it);
	Tuplet p1 = TupletInteger(APPMESS_get, 1);

	dict_write_tuplet(it, &p1);
	app_message_outbox_send();
}
void history() {
  if(nowFetch)return;
  nowFetch=true;
	DictionaryIterator *it;
	app_message_outbox_begin(&it);
	Tuplet p1 = TupletInteger(APPMESS_history, 1);

	dict_write_tuplet(it, &p1);
	app_message_outbox_send();
}
