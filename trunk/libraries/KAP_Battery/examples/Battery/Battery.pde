#include <KAP_Battery.h>

KAP_Battery batt;

void setup() 
{
	Serial.begin(9600);
	batt.init(A2,10000,10000,3.3f);
}

void loop() 
{
	batt.measure();
	Serial.print("voltage :");
	Serial.println(batt.Voltage);
	delay(1000);
}