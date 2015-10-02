/*
  Kai Arduino project fine
  Refer to Configurations.h for configuring your sensor
*/
#include "Configuration.h"
#include "Sensors.h"

//Header Setup
#ifdef RAIN_FALL
	#include <PinChangeInt.h>
#endif

#ifdef HUMID_HTU
	#include <Wire.h>
	#include "Adafruit_HTU21DF.h"
	Adafruit_HTU21DF htu = Adafruit_HTU21DF();
#endif

#ifdef WATER_TEMP
	#include <OneWire.h>
	#include <DallasTemperature.h>
	OneWire oneWire(WATER_TEMP);
	DallasTemperature sensors(&oneWire);
	DeviceAddress Temp_0;
#endif

#ifdef WIFI
	Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT, SPI_CLOCK_DIVIDER);
#endif

void setup(void)
{
	//Setup the serial line
	#ifdef SERIAL_PORT
		while (!Serial) ; //waits for the serial line to be active
		Serial.begin(BAUDRATE);
		Serial.print(F("Powering up Kai Weather Station V"));
		Serial.print(VERSION);
		Serial.println("!\n"); 
	#endif
	//setup XBEE

	//setup Wifi

	//setup each available sensor if need be
	#ifdef RAIN_FALL
		pinMode(RAIN_FALL, INPUT_PULLUP); // input from wind meters rain gauge sensor
		attachPinChangeInterrupt(RAIN_FALL, rainIRQ, FALLING);
	#endif

	#ifdef HUMID_HTU
		if (!htu.begin()) {
			Serial.println("Couldn't find sensor!");
		}
	#endif

	#ifdef WATER_TEMP
		sensors.begin();
		if (!sensors.getAddress(Temp_0, 0))
			Serial.println("Unable to find address for Device 0");
		sensors.setResolution(Temp_0, 12);
	#endif
}

void loop() {

	#ifdef SERIAL_PORT
		#ifdef WIND_DIRECTION
			Serial.print("WindDir = ");      
			Serial.print(wind_direction());
			Serial.print(" deg \t");
		#endif
		#ifdef WIND_SPEED
			Serial.print("WindSpd = ");      
			Serial.print(wind_speed());
			Serial.print(" m/s \t");
		#endif
		#ifdef RAIN_FALL
			Serial.print("dailyrainin=");
			Serial.print(dailyrainin,3);
			Serial.print(" \" \t");
		#endif
		#ifdef HUMID_HTU
			Serial.print("ATemp = ");
			Serial.print(htu.readTemperature());
			Serial.print("C\t Hum = ");
			Serial.print(htu.readHumidity());
			Serial.print("\t");
		#endif
		#ifdef WATER_TEMP
			printTemperature(sensors,Temp_0);
		#endif
		Serial.println();
	#endif
	// wait 2 seconds
	delay(2000);                     
}
