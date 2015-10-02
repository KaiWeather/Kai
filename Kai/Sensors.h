#ifndef Sensors_h
#define Sensors_h
//#include "Sensors.cpp"
#include <WString.h>


class senspac
{
public:
	void update(float atemp,float wtemp, float humid, float rfall, float wspd, float wdir, int wdeb, int wflow);
	int wind_speed();
	int wind_direction();
	void print();
	String csv();
	String csv(int pos);

	int rain();

	int location;
	int csvlen;
	//char data[];

private:
	//Each output Sensor readings
	int ATEMP; // Ambient Temperature
	int WTEMP; // Water Temperature
	int HUMID; // Humidity
	int RFALL; // Rain Fall
	int WSPD;  // Wind Speed
	int WDIR;  // Wind Direction
	int WDEB;  // Water Depth
	int WFLOW; // Water Flow

};

#endif
