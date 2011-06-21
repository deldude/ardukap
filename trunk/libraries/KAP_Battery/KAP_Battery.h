/*
 * KAP_Battery.h
 *
 *  Created on: 15 juin 2011
 *      Author: deldude
 */

#ifndef KAP_BATTERY_H
#define KAP_BATTERY_H

#include "WProgram.h"

class KAP_Battery
{
	public:
		float Voltage;
		KAP_Battery();
		~KAP_Battery();
		void measure();
		void init(byte pin,float R1, float R2, float Aref);
	private:
		byte _pin;
		float ScaleFactor;
		float filterSmooth(float currentData, float previousData, float smoothFactor);
};


#endif

