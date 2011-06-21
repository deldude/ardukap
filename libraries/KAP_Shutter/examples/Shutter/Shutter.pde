#include <KAP_Shutter.h>

KAP_Shutter shutter;

void setup() 
{
	shutter.init(3,9,0,0);
}

void loop() 
{
	shutter.click();
	delay(1000);
}