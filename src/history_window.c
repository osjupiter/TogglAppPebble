#include <pebble.h>
#include "history_window.h"
#include "core.h"
  #include"main_window.h"
  
Window *history_window;
MenuLayer *menu_layer;

char names[20][200];
int size=0;
void draw_row_callback(GContext *ctx, Layer *cell_layer, MenuIndex *cell_index, void *callback_context)
{

    menu_cell_basic_draw(ctx, cell_layer,names[cell_index->row],NULL, NULL);
   //menu_cell_basic_draw(ctx, cell_layer,"sdfsadfsf",NULL, NULL);
}
 
uint16_t num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *callback_context)
{
 return size;
}
 
void select_click_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context)
{
  start(names[cell_index->row]);
window_stack_pop(true);
  set_content_string( "Timer is being started");
	setStopActionBar();
  
}
void history_window_load(Window *window) {
  //added
  //Create it - 12 is approx height of the top bar
    menu_layer = menu_layer_create(GRect(0, 0, 144, 168 - 16));
 
    //Let it receive clicks
    menu_layer_set_click_config_onto_window(menu_layer, window);
 
    //Give it its callbacks
    MenuLayerCallbacks callbacks = {
        .draw_row = (MenuLayerDrawRowCallback) draw_row_callback,
        .get_num_rows = (MenuLayerGetNumberOfRowsInSectionsCallback) num_rows_callback,
        .select_click = (MenuLayerSelectCallback) select_click_callback
    };
    menu_layer_set_callbacks(menu_layer, NULL, callbacks);
 
    //Add to Window
    layer_add_child(window_get_root_layer(window), menu_layer_get_layer(menu_layer));

}
void history_window_unload(Window *window) {
  menu_layer_destroy(menu_layer);
  window_destroy(window);
  get();
}

Window *create_history_window(DictionaryIterator *Items){
  int i=0;
  APP_LOG(APP_LOG_LEVEL_INFO,"create_history_window!!!!!!");
  size=0;
  //if(history_window){ APP_LOG(APP_LOG_LEVEL_INFO,"already history_window exist!!!!!!!!");return history_window;}
  Tuple *item = dict_read_first(Items);
  while(true){
    if(!item)break;
    if(item->key==APPMESS_history){
      item =dict_read_next(Items);
      continue;
    }
    strncpy(names[i],item->value->cstring,200);
    APP_LOG(APP_LOG_LEVEL_INFO,"Loaded Item!! %lu=>%s",item->key,item->value->cstring); 
    item =dict_read_next(Items);
    i++;
  }
  size=i;
  
  APP_LOG(APP_LOG_LEVEL_INFO,"Loaded Items!!");
  history_window = window_create();
	window_set_window_handlers(history_window, (WindowHandlers) {
		.load = history_window_load,
		.unload = history_window_unload,
	});
  return history_window;
  
}
  