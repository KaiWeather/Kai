#ifndef Sensors_h
#define Sensors_h

class senslib
{
public:
	void update(int htu,int htuh, int dts, int R);
	void print();
	void csv();
	//Sensor Helper functions
	void ambient_temp(int htu);
	void water_temp(int dts);
	void humidity(int htuh);
	void rain_fall(int R);
	void wind_speed();
	void wind_direction();
	int wind_dir_lookup();
	void water_depth();
	void water_flow();
	//Setup Bool to see what is attached
	bool ATEMP_A; // Ambient Temperature or HUMID Attached?
	bool WTEMP_A; // Water Temperature Attached?

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
