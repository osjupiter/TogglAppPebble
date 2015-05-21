#include <pebble.h>
#include "main_window.h"
#include "core.h"
#include "history_window.h"

Window *my_window;
TextLayer *content_layer;
TextLayer *title_layer;
ActionBarLayer* action_bar;

static const int TITLE_HEIGHT=50;


GBitmap *icon_start;
GBitmap *icon_stop;

void start_handler(ClickRecognizerRef recognizer, void *context) {
	start(NULL);
  set_content_string( "Timer is being started");
	setStopActionBar();
}
void history_handler(ClickRecognizerRef recognizer, void *context) {
  history();
}

void stop_handler(ClickRecognizerRef recognizer, void *context) {
	stop();
	tick_timer_service_unsubscribe();
	static char buf[64] = "";
	snprintf(buf, sizeof(buf), "Your timer for '%s' was stopped after %s", Gdescription, calculateDuration(last_time,Gduration));
  set_content_string( buf);
	setStartActionBar();
}
void start_click_config_provider(void *context) {
	window_single_click_subscribe(BUTTON_ID_UP, start_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, history_handler);
}

void stop_click_config_provider(void *context) {
	window_single_click_subscribe(BUTTON_ID_DOWN, stop_handler);
}
void setStartActionBar() {
	action_bar_layer_set_icon(action_bar, BUTTON_ID_UP, icon_start);
  action_bar_layer_set_icon(action_bar, BUTTON_ID_SELECT , icon_start);
 	action_bar_layer_set_click_config_provider(action_bar, (ClickConfigProvider) start_click_config_provider);
	action_bar_layer_clear_icon(action_bar, BUTTON_ID_DOWN);
 }

void setStopActionBar() {
	action_bar_layer_set_icon(action_bar, BUTTON_ID_DOWN, icon_stop);
 	action_bar_layer_set_click_config_provider(action_bar, (ClickConfigProvider) stop_click_config_provider);
	action_bar_layer_clear_icon(action_bar, BUTTON_ID_UP);
  action_bar_layer_clear_icon(action_bar, BUTTON_ID_SELECT);
 }








void set_content_string(char *str){
  text_layer_set_text(content_layer,str);
}

void my_window_load(Window *window) {
	Layer *window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_frame(window_layer);
	GRect content_layer_bounds = GRect(3,bounds.origin.y+TITLE_HEIGHT,bounds.size.w-ACTION_BAR_WIDTH-10-3,bounds.size.h-TITLE_HEIGHT+10);
	
	title_layer = text_layer_create(GRect(0,0,bounds.size.w-ACTION_BAR_WIDTH-10,TITLE_HEIGHT));
	text_layer_set_background_color(title_layer, GColorWhite);
	text_layer_set_text_color(title_layer, GColorBlack);
	text_layer_set_text_alignment(title_layer, GTextAlignmentCenter);
	text_layer_set_font(title_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT));
	text_layer_set_text(title_layer, "Toggl");
	
	content_layer = text_layer_create(content_layer_bounds);
	text_layer_set_background_color(content_layer, GColorWhite);
	text_layer_set_text_color(content_layer, GColorBlack);
	text_layer_set_text_alignment(content_layer, GTextAlignmentCenter);
	text_layer_set_font(content_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
	text_layer_set_text(content_layer, "time to Toggl!");
	
	icon_start = gbitmap_create_with_resource (RESOURCE_ID_ICON_START_BLACK);
  icon_stop = gbitmap_create_with_resource (RESOURCE_ID_ICON_STOP_BLACK);
	
	action_bar = action_bar_layer_create();
  	
	action_bar_layer_add_to_window(action_bar, window);
 	layer_add_child(window_layer, text_layer_get_layer(title_layer));
	layer_add_child(window_layer, text_layer_get_layer(content_layer));

}

void my_window_unload(Window *window) {
	text_layer_destroy(title_layer);
	text_layer_destroy(content_layer);
	action_bar_layer_destroy(action_bar);
  window_destroy(window);
}

Window *create_main_window(){
  if(my_window)return my_window;
  my_window = window_create();
	window_set_window_handlers(my_window, (WindowHandlers) {
		.load = my_window_load,
		.unload = my_window_unload,
	});
  return my_window;
}
