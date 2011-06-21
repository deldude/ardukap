/*
 * KAP_Axle.cpp
 *
 *  Created on: 15 juin 2011
 *      Author: deldude
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
void KAP_Axle::init(byte Type, byte Pin, byte Min, byte Center, byte Max, byte Degree)
{
    Angle = Center;
    myservo.attach(Pin);
    myservo.write(Angle);
}

void KAP_Axle::reset(void)
{
    myservo.write(Center);
    delay(15);
}

void KAP_Axle::move(int dir)
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

void KAP_Axle::setMax(byte val)
{
  Max = val;
}

void KAP_Axle::setMin(byte val)
{
  Min = val;
}

void KAP_Axle::setCenter(byte val)
{
  Center = val;
}

void KAP_Axle::setDegree(byte val)
{
  Degree = val;
}