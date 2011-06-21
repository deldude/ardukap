/*
 * KAP_Altitude.cpp
 *
 *  Created on: 15 juin 2011
 *      Author: deldude
 */

#include "WProgram.h"
#include "KAP_Altitude.h"
#include <Wire.h>

double absolute;
double relative;
double rawAltitude;
float groundTemperature ;
float groundPressure ;
float groundAltitude;
float smoothFactor;
#define TEMPERATURE 0
#define PRESSURE 1
byte overSamplingSetting;
int ac1, ac2, ac3;
unsigned int ac4, ac5, ac6;
int b1, b2, mb, mc, md;
long pressure, temperature;
int altitudeAddress;
long rawPressure, rawTemperature;
byte select, pressureCount;
float pressureFactor;

// Constructors ////////////////////////////////////////////////////////////////
KAP_Altitude::KAP_Altitude()
{
	  absolute = 0;
	  relative = 0;
	  overSamplingSetting = 3;
	  pressure = 0;
	  temperature = 0;
	  altitudeAddress = 0x77;
	  pressureFactor = 1/5.255;
	  groundTemperature = 0;
	  groundPressure = 0;
	  groundAltitude = 0;
}

// Destructors ////////////////////////////////////////////////////////////////
KAP_Altitude::~KAP_Altitude()
{
	/*nothing to destruct*/
}

// Private Methods /////////////////////////////////////////////////////////////
float KAP_Altitude::filterSmooth(float currentData, float previousData, float smoothFactor)
{
	if (smoothFactor != 1.0)
    {
      // only apply time compensated filter if smoothFactor is applied
      return (previousData * (1.0 - smoothFactor) + (currentData * smoothFactor));
    }else
    {
      return currentData; // if smoothFactor == 1.0, do not calculate, just bypass!
    }
}

void KAP_Altitude::sendByteI2C(int deviceAddress, byte dataValue)
{
  Wire.beginTransmission(deviceAddress);
  Wire.send(dataValue);
  Wire.endTransmission();
}

byte KAP_Altitude::readByteI2C(int deviceAddress)
{
  Wire.requestFrom(deviceAddress, 1);
  return Wire.receive();
}

int KAP_Altitude::readWordI2C(int deviceAddress)
{
  Wire.requestFrom(deviceAddress, 2);
  return (Wire.receive() << 8) | Wire.receive();
}

int KAP_Altitude::readWordWaitI2C(int deviceAddress)
{
  unsigned char msb, lsb;
  Wire.requestFrom(deviceAddress, 2); // request two bytes
  while(!Wire.available()); // wait until data available
  msb = Wire.receive();
  while(!Wire.available()); // wait until data available
  lsb = Wire.receive();
  return (((int)msb<<8) | ((int)lsb));
}

int KAP_Altitude::readReverseWordI2C(int deviceAddress)
{
  byte lowerByte;
  Wire.requestFrom(deviceAddress, 2);
  lowerByte = Wire.receive();
  return (Wire.receive() << 8) | lowerByte;
}

byte KAP_Altitude::readWhoI2C(int deviceAddress)
{
  // read the ID of the I2C device
  Wire.beginTransmission(deviceAddress);
  Wire.send(0x00);
  Wire.endTransmission();
  delay(100);
  Wire.requestFrom(deviceAddress, 1);
  return Wire.receive();
}

void KAP_Altitude::updateRegisterI2C(int deviceAddress, byte dataAddress, byte dataValue)
{
  Wire.beginTransmission(deviceAddress);
  Wire.send(dataAddress);
  Wire.send(dataValue);
  Wire.endTransmission();
}

void KAP_Altitude::requestRawPressure(void)
{
  updateRegisterI2C(altitudeAddress, 0xF4, 0x34+(overSamplingSetting<<6));
}

long KAP_Altitude::readRawPressure(void)
{
  unsigned char msb, lsb, xlsb;
  sendByteI2C(altitudeAddress, 0xF6);
  Wire.requestFrom(altitudeAddress, 3); // request three bytes
  while(!Wire.available()); // wait until data available
  msb = Wire.receive();
  while(!Wire.available()); // wait until data available
  lsb = Wire.receive();
  while(!Wire.available()); // wait until data available
  xlsb = Wire.receive();
  return (((long)msb<<16) | ((long)lsb<<8) | ((long)xlsb)) >>(8-overSamplingSetting);
}

void KAP_Altitude::requestRawTemperature(void)
{
  updateRegisterI2C(altitudeAddress, 0xF4, 0x2E);
}

unsigned int KAP_Altitude::readRawTemperature(void)
{
  sendByteI2C(altitudeAddress, 0xF6);
  return readWordWaitI2C(altitudeAddress);
}

// Public Methods //////////////////////////////////////////////////////////////
void KAP_Altitude::init(void)
{
  sendByteI2C(altitudeAddress, 0xAA);
  ac1 = readWordWaitI2C(altitudeAddress);
  sendByteI2C(altitudeAddress, 0xAC);
  ac2 = readWordWaitI2C(altitudeAddress);
  sendByteI2C(altitudeAddress, 0xAE);
  ac3 = readWordWaitI2C(altitudeAddress);
  sendByteI2C(altitudeAddress, 0xB0);
  ac4 = readWordWaitI2C(altitudeAddress);
  sendByteI2C(altitudeAddress, 0xB2);
  ac5 = readWordWaitI2C(altitudeAddress);
  sendByteI2C(altitudeAddress, 0xB4);
  ac6 = readWordWaitI2C(altitudeAddress);
  sendByteI2C(altitudeAddress, 0xB6);
  b1 = readWordWaitI2C(altitudeAddress);
  sendByteI2C(altitudeAddress, 0xB8);
  b2 = readWordWaitI2C(altitudeAddress);
  sendByteI2C(altitudeAddress, 0xBA);
  mb = readWordWaitI2C(altitudeAddress);
  sendByteI2C(altitudeAddress, 0xBC);
  mc = readWordWaitI2C(altitudeAddress);
  sendByteI2C(altitudeAddress, 0xBE);
  md = readWordWaitI2C(altitudeAddress);
  requestRawTemperature(); // setup up next measure() for temperature
  select = TEMPERATURE;
  pressureCount = 0;
  measureCurrent();
  delay(5); // delay for temperature
  measureCurrent();
  delay(26); // delay for pressure
  measureGround();
  // check if measured ground altitude is valid
  while (abs(rawAltitude - groundAltitude) > 10)
  {
    delay(26);
    measureGround();
  }
  absolute = groundAltitude;
}

void KAP_Altitude::measureCurrent(void)
{
  long x1, x2, x3, b3, b5, b6, p;
  unsigned long b4, b7;
  int32_t tmp;
  if (select == PRESSURE)
  {
    rawPressure = readRawPressure();
    if (pressureCount == 4)
    {
      requestRawTemperature();
      pressureCount = 0;
      select = TEMPERATURE;
    }else
    {
      requestRawPressure();
    }
    pressureCount++;
  }else
  { // select must equal TEMPERATURE
    rawTemperature = (long)readRawTemperature();
    requestRawPressure();
    select = PRESSURE;
  }
 //calculate true temperature
  x1 = ((long)rawTemperature - ac6) * ac5 >> 15;
  x2 = ((long) mc << 11) / (x1 + md);
  b5 = x1 + x2;
  temperature = ((b5 + 8) >> 4);
  //calculate true pressure
  b6 = b5 - 4000;
  x1 = (b2 * (b6 * b6 >> 12)) >> 11;
  x2 = ac2 * b6 >> 11;
  x3 = x1 + x2;
  // Real Bosch formula - b3 = ((((int32_t)ac1 * 4 + x3) << overSamplingSetting) + 2) >> 2;
  // The version below is the same, but takes less program space
  tmp = ac1;
  tmp = (tmp * 4 + x3) << overSamplingSetting;
  b3 = (tmp + 2) >> 2;
  x1 = ac3 * b6 >> 13;
  x2 = (b1 * (b6 * b6 >> 12)) >> 16;
  x3 = ((x1 + x2) + 2) >> 2;
  b4 = (ac4 * (uint32_t) (x3 + 32768)) >> 15;
  b7 = ((uint32_t) rawPressure - b3) * (50000 >> overSamplingSetting);
  p = b7 < 0x80000000 ? (b7 << 1) / b4 : (b7 / b4) >> 1;
  x1 = (p >> 8) * (p >> 8);
  x1 = (x1 * 3038) >> 16;
  x2 = (-7357 * p) >> 16;
  pressure = (p + ((x1 + x2 + 3791) >> 4));
  rawAltitude = 44330 * (1 - pow(pressure/101325.0, pressureFactor));
  absolute = filterSmooth(rawAltitude, absolute, 0.02);
  relative = absolute - groundAltitude;
}

void KAP_Altitude::measureGround(void)
{
  groundAltitude = 0;
  for (int i=0; i < 30; i++)
  {
	measureCurrent();
    delay(26);
    groundAltitude += rawAltitude;
  }
  groundAltitude = groundAltitude / 30.0;
}

