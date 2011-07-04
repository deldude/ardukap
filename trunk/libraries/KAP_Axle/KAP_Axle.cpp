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
#include "KAP_Axle.h"


#define NORMAL 1
#define CONTINIOUS 2
byte Max;
byte Min;
byte Center;
byte Degree;
byte Type;
byte Angle;
Servo myservo;

// Constructors ////////////////////////////////////////////////////////////////
KAP_Axle::KAP_Axle()
{
	/*nothing to construct*/
}

// Destructors ////////////////////////////////////////////////////////////////
KAP_Axle::~KAP_Axle()
{
	/*nothing to destruct*/
}

// Public Methods //////////////////////////////////////////////////////////////
void KAP_Axle::init(byte Type, byte Pin, byte Min, int Center, byte Max, byte Degree)
{
    Angle = Center;
    myservo.attach(Pin);
    myservo.write(Angle);
}

void KAP_Axle::reset(void)
{
    if(Center > 1000)
    {
      myservo.writeMicroseconds(Center);
    }else
    {
      myservo.write(Center);
    }    
    delay(15);
}

void KAP_Axle::moveRelative(int dir)
{
    int move = Degree * dir;
    if(Type == CONTINIOUS)
    {
      Angle = Center + move;
    }else
    {
      if(((Angle + Degree) > Min) && ((Angle + Degree) < Max))
      {
        Angle = Angle + move;
      }
    }
    myservo.write(Angle);
    delay(15);
    if(Type == CONTINIOUS)
    {
      reset();
    }
}

void KAP_Axle::moveAbsolute(int deg)
{
    if((deg > Min) && (deg < Max))
    {
        Angle = deg;
        myservo.write(Angle);
        delay(15);        
    }
}


void KAP_Axle::setMax(byte val)
{
  Max = val;
}

void KAP_Axle::setMin(byte val)
{
  Min = val;
}

void KAP_Axle::setCenter(int val)
{
  Center = val;
}

void KAP_Axle::setDegree(byte val)
{
  Degree = val;
}