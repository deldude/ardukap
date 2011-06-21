#include <KAP_Axle.h>

KAP_Axle axle;

void setup() 
{
	Serial.begin(9600);
  	axle(1,9,0,90,180,10);
	axle.reset();
}

void loop() 
{
	axle.move(1);
	delay(1000);
}

