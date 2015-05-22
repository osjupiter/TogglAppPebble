#pragma once
#include <pebble.h>  

  enum keys {
	APPMESS_start,
	APPMESS_stop,
	APPMESS_get,
	APPMESS_id,
	APPMESS_duration,
	APPMESS_description,
	APPMESS_offset,
  APPMESS_history
};
int TIME_ZONE_OFFSET;
int GcurrentTid;
uint32_t Gduration;
struct tm *last_time;
char *Gdescription;
  bool nowFetch;

char* calculateDuration(struct tm *current_time, int duration) ;
void updateSysCurrentTimer(Tuple *id, Tuple *duration, Tuple *description) ;
void start(char* str);
void stop() ;
void history() ;

void get() ;
