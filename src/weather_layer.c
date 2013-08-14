#include "globals.h"
#include "weather_layer.h"
#include "window_manager.h"
	
//---------------------------------------------------------------------------------------
// Private variables and methods	
//---------------------------------------------------------------------------------------
	
#define DEFAULT_TEMP_TEXT "---/--%"
	
const uint8_t weatherImages[] = {
	RESOURCE_ID_IMAGE_NO_WEATHER,
	RESOURCE_ID_IMAGE_CLEAR_SKY_DAY,
	RESOURCE_ID_IMAGE_CLEAR_SKY_NIGHT,
	RESOURCE_ID_IMAGE_BLOWING_SNOW,
	RESOURCE_ID_IMAGE_BROKEN_CLOUDS_DAY,
	RESOURCE_ID_IMAGE_BROKEN_CLOUDS_NIGHT,
	RESOURCE_ID_IMAGE_BROKEN_CLOUDS_RAIN_DAY,
	RESOURCE_ID_IMAGE_BROKEN_CLOUDS_RAIN_NIGHT,
	RESOURCE_ID_IMAGE_BROKEN_CLOUDS_SNOW_DAY,
	RESOURCE_ID_IMAGE_BROKEN_CLOUDS_SNOW_NIGHT,
	RESOURCE_ID_IMAGE_BROKEN_CLOUDS_THUNDERSTORM_DAY,
	RESOURCE_ID_IMAGE_BROKEN_CLOUDS_THUNDERSTORM_NIGHT,
	RESOURCE_ID_IMAGE_FOG,
	RESOURCE_ID_IMAGE_FULL_CLOUDS,
	RESOURCE_ID_IMAGE_HEAVY_RAIN,
	RESOURCE_ID_IMAGE_HEAVY_SNOW,
	RESOURCE_ID_IMAGE_LIGHT_FOG_DAY,
	RESOURCE_ID_IMAGE_LIGHT_FOG_NIGHT,
	RESOURCE_ID_IMAGE_RAIN_SLEET,
	RESOURCE_ID_IMAGE_RAIN,
	RESOURCE_ID_IMAGE_SLEET,
	RESOURCE_ID_IMAGE_SNOW_RAIN,
	RESOURCE_ID_IMAGE_THUNDERSTORM,
	RESOURCE_ID_IMAGE_WIND,
	RESOURCE_ID_IMAGE_WINTER_MIX};

GRect baseWeatherLayerBounds = {.origin={.x=13,.y=65},.size={.w=131,.h=50}};
GRect imageWeatherLayerBounds = {.origin={.x=0,.y=0},.size={.w=50,.h=50}};
GRect tempHumidWeatherLayerBounds = {.origin={.x=50,.y=0},.size={.w=81,.h=50}};
GFont weatherFont; 
WeatherLayer weatherLayer;
char * weather_layer_get_temp_string(bool);
char * weather_layer_get_humid_string();

void weather_layer_write_image(void) {
	static WeatherImages previousImage = START_CONDITION;
	if(weatherLayer.currentImage != previousImage) {
		previousImage = weatherLayer.currentImage;
		if(weatherLayer.imageLoaded) {
			layer_remove_from_parent(&weatherLayer.imageContainer.layer.layer);
			bmp_deinit_container(&weatherLayer.imageContainer);
			weatherLayer.imageLoaded = false;
		}
		bmp_init_container(weatherImages[weatherLayer.currentImage],&weatherLayer.imageContainer);
		layer_set_frame(&weatherLayer.imageContainer.layer.layer, imageWeatherLayerBounds);
		layer_add_child(&weatherLayer.baseLayer,&weatherLayer.imageContainer.layer.layer);
		weatherLayer.imageLoaded = true;
	}
}

void redraw_weather_layer(Layer* layer, GContext* ctx) {
	weather_layer_write_image();
	text_layer_set_text(&weatherLayer.tempHumidLayer, (const char *)weatherLayer.humidTemp);
	window_manager_mark_layer_done(UPDATE_WEATHER);
}

//---------------------------------------------------------------------------------------
// Public variables and methods	
//---------------------------------------------------------------------------------------

void weather_layer_init(Window * window) {
	weatherLayer.currentImage = NO_WEATHER;
	weatherLayer.imageLoaded = false;
		
	layer_init(&weatherLayer.baseLayer,baseWeatherLayerBounds);
	text_layer_init(&weatherLayer.tempHumidLayer, tempHumidWeatherLayerBounds);
	
	weatherLayer.baseLayer.update_proc = redraw_weather_layer;
	
	text_layer_set_text_alignment(&weatherLayer.tempHumidLayer, GTextAlignmentLeft);
	text_layer_set_text_color(&weatherLayer.tempHumidLayer, GColorWhite);
	text_layer_set_background_color(&weatherLayer.tempHumidLayer, GColorBlack);	  
	weatherFont = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TIMES_ROMAN_20));
	text_layer_set_font(&weatherLayer.tempHumidLayer, weatherFont);
	text_layer_set_text(&weatherLayer.tempHumidLayer, DEFAULT_TEMP_TEXT);
	
	layer_add_child(&weatherLayer.baseLayer,&weatherLayer.tempHumidLayer.layer);
	layer_add_child(&window->layer,&weatherLayer.baseLayer);
}

void weather_layer_set_icon(WeatherImages newImage) {
	weatherLayer.currentImage = newImage;
}

void weather_layer_update_display() {
	layer_mark_dirty(&weatherLayer.baseLayer);
}

void weather_layer_set_temperature_humidity_string(char * humidTemp, uint8_t size) {
	memcpy(&weatherLayer.humidTemp[0],humidTemp,size);
}

void weather_layer_deinit() {
	if(weatherLayer.imageLoaded) {
		layer_remove_from_parent(&weatherLayer.imageContainer.layer.layer);
		bmp_deinit_container(&weatherLayer.imageContainer);
		weatherLayer.imageLoaded = false;
	}
	fonts_unload_custom_font(weatherFont);
}
