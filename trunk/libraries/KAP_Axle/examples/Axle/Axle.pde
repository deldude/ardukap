#include <KAP_Axle.h>
#include <Servo.h>

KAP_Axle axle;

void setup() 
{
	Serial.begin(9600);
  	axle.init(1,9,0,90,180,10);
	axle.reset();
}

void loop() 
{
	axle.moveAbsolute(1);
	delay(1000);
}
