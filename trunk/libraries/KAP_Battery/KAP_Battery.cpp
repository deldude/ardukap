/*
 * KAP_Battery.cpp
 *
 *  Created on: 15 juin 2011
 *      Author: deldude
 */

#include "WProgram.h"
#include "KAP_Battery.h"

float Voltage;

// Constructors ////////////////////////////////////////////////////////////////
KAP_Battery::KAP_Battery()
{
	/*nothing to construct*/
}

// Destructors ////////////////////////////////////////////////////////////////
KAP_Battery::~KAP_Battery()
{
	/*nothing to destruct*/
}

// Private Methods /////////////////////////////////////////////////////////////
float KAP_Battery::filterSmooth(float currentData, float previousData, float smoothFactor)
{
	if (smoothFactor != 1.0)
	{
		return (previousData * (1.0 - smoothFactor) + (currentData * smoothFactor));
	}else
	{
		return currentData; // if smoothFactor == 1.0, do not calculate, just bypass!
	}
}

// Public Methods //////////////////////////////////////////////////////////////
void KAP_Battery::init(byte pin,float R1, float R2, float Aref)
{
	ScaleFactor = ((Aref / 1024.0) * ((R1 + R2) / R2));
	measure();
	_pin = pin;
}

void KAP_Battery::measure()
{
	float tmp = analogRead(_pin) * ScaleFactor;
	Voltage = filterSmooth(tmp, Voltage, 0.1);
}
