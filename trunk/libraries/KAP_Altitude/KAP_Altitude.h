/*
  arduKAP - June 2011
  https://code.google.com/p/ardukap/
  An Open Source Arduino based KAP Controler.
 
  This program is free software: you can redistribute it and/or modify 
  it under the terms of the GNU General Public License as published by 
  the Free Software Foundation, either version 3 of the License, or 
  (at your option) any later version. 

  This program is distributed in the hope that it will be useful, 
  but WITHOUT ANY WARRANTY; without even the implied warranty of 
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
  GNU General Public License for more details. 

  You should have received a copy of the GNU General Public License 
  along with this program. If not, see <http://www.gnu.org/licenses/>. 
  
  Thanks to the www.AeroQuad.com for providing such a nice code which
  provide a lot of inspiration for the arduKAP controler code.
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

