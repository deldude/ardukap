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
#include "KAP_Shutter.h"


#define SERVO 1
#define CHDK 2
#define CA_1 3
byte srvUp;
byte srvDown;
byte shtType;
byte shtPin;
Servo shtServo; 

// Constructors ////////////////////////////////////////////////////////////////
KAP_Shutter::KAP_Shutter()
{
	/*nothing to construct*/
}

// Destructors ////////////////////////////////////////////////////////////////
KAP_Shutter::~KAP_Shutter()
{
	/*nothing to destruct*/
}

// Public Methods //////////////////////////////////////////////////////////////
void KAP_Shutter::init(byte Type, byte Pin, byte Up, byte Down)
{
    shtType = Type;
    shtPin = Pin;
    srvUp = Up;
    srvDown = Down;
    if(shtType == SERVO)
    {
      shtServo.attach(shtPin);
      shtServo.write(srvUp);
    }else if(shtType == CHDK)
    {
        pinMode(shtPin, OUTPUT);
    }else if(shtType == CA_1)   
    {
        pinMode(shtPin, OUTPUT);
    }
}

void KAP_Shutter::click(void)
{
    if(shtType == SERVO)
    {
        servo();
    }else if(shtType == CHDK)
    {
        chdk();
    }else if(shtType == CA_1)   
    {
        ca_1();
    }
}

// Private Methods //////////////////////////////////////////////////////////////

void KAP_Shutter::servo(void)
{
    shtServo.write(srvDown);
    delay(15);
    shtServo.write(srvUp);
    delay(15);
}

void KAP_Shutter::chdk(void)
{
  digitalWrite(shtPin, HIGH);   // pin APN ON
  delay(200);                   // durée 200ms
  digitalWrite(shtPin, LOW);    // pin APN OFF
}

void KAP_Shutter::ca_1(void)
{
  digitalWrite(shtPin, HIGH);   // pin APN ON
  delay(30);                     // durée 30ms (half-press et focus)
  digitalWrite(shtPin, LOW);    // pin APN OFF
  delay(200);                    // pause
  digitalWrite(shtPin, HIGH);   // pin APN ON
  delay(150);                    // durée 150ms (full-press)
  digitalWrite(shtPin, LOW);    // pin APN OFF
  delay(100);                    // delai après photo 2s (a régler)
  digitalWrite(shtPin, HIGH);   // pin APN ON
  delay(30);                     // durée 30ms (release)
  digitalWrite(shtPin, LOW);    // pin APN OFF
  delay(30);                     // durée 30ms 
  digitalWrite(shtPin, HIGH);   // pin APN ON
  delay(30);                     // durée 30ms 
  digitalWrite(shtPin, LOW);    // pin APN OFF
}
