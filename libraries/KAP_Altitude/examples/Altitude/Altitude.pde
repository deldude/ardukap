#include <KAP_Altitude.h>
#include <Wire.h>

KAP_Altitude alt;

void setup() 
{
	Serial.begin(9600);
	Wire.begin();
	TWBR = 12;
	delay(500);
  	alt.init();
  	alt.measureGround();
}

void loop() 
{
	alt.measureCurrent();
	Serial.print("ground : ");
	Serial.print(round(alt.groundAltitude));
	Serial.print(" m -- absolute : ");
	Serial.print(round(alt.absolute));
	Serial.print(" m -- relative :");
	Serial.print(round(alt.relative));
	Serial.println(" m");
	delay(100);
}
