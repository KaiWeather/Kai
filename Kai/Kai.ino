/*
  Kai Arduino project fine
  Refer to Configurations.h for configuring your sensor
*/
#include "Configuration.h"
#include "Sensors.h"

//Header Setup
#ifdef RAIN_FALL
	#include <PinChangeInt.h>
	volatile float dailyrainin = 0;
	volatile unsigned long raintime, rainlast, raininterval;
void rainIRQ()
{
	//debounce the signal, check when the last IRQ
	raintime = millis();
	raininterval = raintime - rainlast;
	if (raininterval > 10) 
	{
		//dailyrainin += 0.2794; // mm of rain
		dailyrainin += 0.011; //Each dump is 0.011" of water
		rainlast = raintime; // set up for next event
	}
}
#endif

#ifdef HUMID_HTU
	#include <Wire.h>
	#include "Adafruit_HTU21DF.h"
	Adafruit_HTU21DF htu = Adafruit_HTU21DF();
#endif

//#ifdef WATER_TEMP
	#include <OneWire.h>
	#include <DallasTemperature.h>
	OneWire oneWire(WATER_TEMP);
	DallasTemperature sensors(&oneWire);
	DeviceAddress Temp_0;
//#endif

#ifdef WIFI
	Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT, SPI_CLOCK_DIVIDER);
#endif

	senslib SENS;

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
		Serial.println("Setting up Rain Fall INT");
		pinMode(RAIN_FALL, INPUT_PULLUP); // input from wind meters rain gauge sensor
		attachPinChangeInterrupt(RAIN_FALL, rainIRQ, FALLING);
	#endif

	#ifdef HUMID_HTU
		Serial.println("Setting up htu Sensor");
		if (!htu.begin()) {
			Serial.println("Couldn't find sensor!");
			SENS.ATEMP_A = false;
		}
		else
		{
			SENS.ATEMP_A = true;
		}
	#endif

	#ifdef WATER_TEMP
		Serial.println("Setting up Water Temp Sensor");
		sensors.begin();
		if (!sensors.getAddress(Temp_0, 0))
		{
			Serial.println("Unable to find address for Device 0");
			SENS.WTEMP_A = false;
		}
		else
		{
			sensors.setResolution(Temp_0, 12);
			SENS.WTEMP_A = true;
		}
	#endif
}

void loop() {
	//update sensor data
	SENS.update(htu.readTemperature(),htu.readHumidity(),0,dailyrainin);
	SENS.print();
	// wait 2 seconds
	delay(2000);                     
}
