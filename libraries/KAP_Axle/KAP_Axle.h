/*
 * KAP_Axle.h
 *
 *  Created on: 15 juin 2011
 *      Author: deldude
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
		void init(byte Type, byte Pin, byte Min, byte Center, byte Max, byte Degree);
		void reset();
		void move(int dir);
		void setMax(byte val);
		void setMin(byte val);
		void setCenter(byte val);
		void setDegree(byte val);
	private:
  	  	#define NORMAL 1
     	#define CONTINIOUS 2
		byte Max;
		byte Min;
		byte Center;
		byte Degree;
		byte Type;
		byte Angle;
		Servo myservo;     	
};


#endif

