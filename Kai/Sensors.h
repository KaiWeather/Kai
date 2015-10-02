#ifndef Sensors_h
#define Sensors_h
#include "Sensors.cpp"

	//Rain Fall
	#ifdef RAIN_FALL
		void rainIRQ();
		int rain_fall();
	#endif

	//Wind Speed
	#ifdef WIND_SPEED
		int wind_speed();
	#endif

	//Wind Directions
	#ifdef WIND_DIRECTION
		int wind_direction();
	#endif

	//Water Temp functions
	#ifdef WATER_TEMP
		int water_temp(DallasTemperature sensors);
		void printTemperature(DallasTemperature sensors,DeviceAddress deviceAddress);
	#endif



#endif
