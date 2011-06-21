/*
 * KAP_Altitude.h
 *
 *  Created on: 15 juin 2011
 *      Author: deldude
 */

#ifndef KAP_ALTITUDE_H
#define KAP_ALTITUDE_H

#include "WProgram.h"

class KAP_Altitude
{
	public:
		double absolute;
		double relative;
		double rawAltitude;
		float groundTemperature ;
		float groundPressure ;
		float groundAltitude;
		float smoothFactor;
		KAP_Altitude();
		~KAP_Altitude();
		void init();
		void measureCurrent();
		void measureGround();
	private:
		#define TEMPERATURE 0
		#define PRESSURE 1
		byte overSamplingSetting;
		int ac1, ac2, ac3;
		unsigned int ac4, ac5, ac6;
		int b1, b2, mb, mc, md;
		long pressure;
		long temperature;
		int altitudeAddress;
		long rawPressure, rawTemperature;
		byte select, pressureCount;
		float pressureFactor;
		float filterSmooth(float currentData, float previousData, float smoothFactor);
		void sendByteI2C(int deviceAddress, byte dataValue);
		byte readByteI2C(int deviceAddress);
		int readWordI2C(int deviceAddress);
		int readWordWaitI2C(int deviceAddress);
		int readReverseWordI2C(int deviceAddress);
		byte readWhoI2C(int deviceAddress);
		void updateRegisterI2C(int deviceAddress, byte dataAddress, byte dataValue);
		void requestRawPressure(void);
		long readRawPressure(void);
		void requestRawTemperature(void);
		unsigned int readRawTemperature(void);
};


#endif

