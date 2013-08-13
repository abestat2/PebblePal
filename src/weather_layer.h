#ifndef WEATHER_LAYER_H
#define WEATHER_LAYER_H

typedef enum {
	NO_WEATHER = 0,
	CLEAR_SKY_DAY,
	CLEAR_SKY_NIGHT,
	BLOWING_SNOW,
	BROKEN_CLOUDS_DAY,
	BROKEN_CLOUDS_NIGHT,
	BROKEN_CLOUDS_RAIN_DAY,
	BROKEN_CLOUDS_RAIN_NIGHT,
	BROKEN_CLOUDS_SNOW_DAY,
	BROKEN_CLOUDS_SNOW_NIGHT,
	BROKEN_CLOUDS_THUNDERSTORM_DAY,
	BROKEN_CLOUDS_THUNDERSTORM_NIGHT,
	FOG,
	FULL_CLOUDS,
	HEAVY_RAIN,
	HEAVY_SNOW,
	LIGHT_CLOUDS_DAY,
	LIGHT_CLOUDS_NIGHT,
	LIGHT_FOG_DAY,
	LIGHT_FOG_NIGHT,
	LIGHT_RAIN,
	LIGHT_SNOW,
	MEDIUM_SNOW,
	RAIN_SLEET,
	RAIN,
	SCATTERED_CLOUDS_DAY,
	SCATTERED_CLOUDS_NIGHT,
	SLEET,
	SNOW_RAIN,
	THUNDERSTORM,
	WIND,
	WINTER_MIX,
	START_CONDITION,
}WeatherImages;

typedef struct {
	Layer baseLayer;
	BmpContainer imageContainer;
	TextLayer tempHumidLayer;
	char humidTemp[20];
	WeatherImages currentImage;
	bool imageLoaded;
}WeatherLayer;

void weather_layer_set_icon(WeatherImages);
void weather_layer_init(Window *);
void weather_layer_deinit();
void weather_layer_update_display();
void weather_layer_set_temperature_humidity_string(char*,uint8_t);

#endif