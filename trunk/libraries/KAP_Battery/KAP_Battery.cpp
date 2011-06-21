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
