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

#ifndef KAP_AXLE_H
#define KAP_AXLE_H

#include "WProgram.h"
#include "Servo.h"

class KAP_Axle
{
	public:
		KAP_Axle();
		~KAP_Axle();
		void init(byte Type, byte Pin, byte Min, int Center, byte Max, byte Degree);
		void reset();
		void moveRelative(int dir);
		void moveAbsolute(int deg);
		void setMax(byte val);
		void setMin(byte val);
		void setCenter(int val);
		void setDegree(byte val);
	private:
  	  	#define NORMAL 1
     	#define CONTINIOUS 2
		byte Max;
		byte Min;
		int Center;
		byte Degree;
		byte Type;
		byte Angle;
		Servo myservo;     	
};


#endif

