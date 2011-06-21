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

