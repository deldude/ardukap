#include <Cmd.h>
#include <Servo.h>
#include <Wire.h>
#include <KAP_Altitude.h>
#include <KAP_Battery.h>
#include <KAP_Axle.h>
#include <KAP_Shutter.h>

#define BAUD 115200
#define BATT_PIN A2 
#define BATT_R1 10000
#define BATT_R2 10000
#define BATT_REF 3.3f
float G_Dt = 0.002;
unsigned long previousTime = 0;
unsigned long currentTime = 0;
unsigned long deltaTime = 0;
unsigned long fiveHZpreviousTime;
unsigned long tenHZpreviousTime;
unsigned long twentyFiveHZpreviousTime;
unsigned long fiftyHZpreviousTime;
unsigned long hundredHZpreviousTime;
unsigned long frameCounter = 0;
byte axleDef = 0;

KAP_Altitude alt;
KAP_Battery batt;
KAP_Axle SrvPan;
KAP_Axle SrvTilt;
KAP_Axle SrvHover;
KAP_Shutter shutt;

void setup()
{
  //Serial Configuration
  cmdInit(BAUD);
  cmdAdd("SET", CMD_set);
  cmdAdd("EXEC", CMD_exec);  
  //Altitude Configuration
  Wire.begin();
  TWBR = 12;
  delay(500);
  alt.init();
  delay(100);
  alt.measureGround();
  //Battery monitoring configuration
  batt.init(BATT_PIN,BATT_R1,BATT_R2,BATT_REF);
  //Telemetry
  TelemetrySend();
  previousTime = micros();
}

/*******************************************************************
  // tasks (microseconds of interval)
  read Receiver  (  20000); //  50Hz
  Read Baro      (  40000); //  25Hz
  Read Battery   ( 100000); //  10Hz
  Send Telemetry ( 200000); //   5Hz  
*******************************************************************/
void loop()
{
  currentTime = micros();
  deltaTime = currentTime - previousTime;
  if (deltaTime >= 10000) 
  {
    frameCounter++;
    // ================================================================
    // 100hz task loop
    // ================================================================
    if (frameCounter %   1 == 0) 
    {  //  100 Hz tasks
      G_Dt = (currentTime - hundredHZpreviousTime) / 1000000.0;
      hundredHZpreviousTime = currentTime;
    }
    // ================================================================
    // 50hz task loop
    // ================================================================
    if (frameCounter %   2 == 0) 
    {  //  50 Hz tasks
       cmdPoll();
    }
    // ================================================================
    // 25hz task loop
    // ================================================================
    if (frameCounter %   4 == 0) 
    {  //  25 Hz tasks
      G_Dt = (currentTime - twentyFiveHZpreviousTime) / 1000000.0;
      twentyFiveHZpreviousTime = currentTime;
      alt.measureCurrent();
    }
    // ================================================================
    // 10hz task loop
    // ================================================================
    if (frameCounter %  10 == 0) 
    {  //   10 Hz tasks
      G_Dt = (currentTime - tenHZpreviousTime) / 1000000.0;
      tenHZpreviousTime = currentTime;
      batt.measure();
    }
    // ================================================================
    // 5hz task loop
    // ================================================================
    if (frameCounter %  20 == 0) 
    {  //   10 Hz tasks
      G_Dt = (currentTime - fiveHZpreviousTime) / 1000000.0;
      fiveHZpreviousTime = currentTime;
      TelemetrySend();
    }
    previousTime = currentTime;
  }
  if (frameCounter >= 100) 
  {
    frameCounter = 0;
  } 
}

// SEND TELEMETRY PARAMS
void TelemetrySend()
{
  Serial.print("RX ");
  Serial.print(alt.groundAltitude);
  Serial.print(" ");
  Serial.print(alt.groundTemperature);
  Serial.println("");
  Serial.print(alt.absolute);
  Serial.println("");
  Serial.print(alt.relative);
  Serial.println("");
  Serial.print(batt.Voltage);
  Serial.println(""); 
}

// SET PARAMETERS FROM SERIAL INPUT
void CMD_set(int arg_cnt, char **args)
{
  int i;
  //DEBUG START
  for (i=0; i<arg_cnt; i++)
  {
    Serial.print("Arg ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(args[i]);
  }
  //DEBUG END
  if(args[1] == "CONF") { axleDef = cmdStr2Num(args[2], 10); }  
  if(args[1] == "INIT")
  {
    if(args[2] == "PAN")
    { 
      SrvPan.init(cmdStr2Num(args[3],10),cmdStr2Num(args[4],10),cmdStr2Num(args[5],10),cmdStr2Num(args[6],10),cmdStr2Num(args[7],10),cmdStr2Num(args[8],10));
    }else if(args[2] == "TILT")  
    { 
      SrvTilt.init(cmdStr2Num(args[3],10),cmdStr2Num(args[4],10),cmdStr2Num(args[5],10),cmdStr2Num(args[6],10),cmdStr2Num(args[7],10),cmdStr2Num(args[8],10));
    }else if(args[2] == "HOVER")     
    { 
      SrvHover.init(cmdStr2Num(args[3],10),cmdStr2Num(args[4],10),cmdStr2Num(args[5],10),cmdStr2Num(args[6],10),cmdStr2Num(args[7],10),cmdStr2Num(args[8],10));
    }else if(args[2] == "SHUTTER")  
    { 
      shutt.init(cmdStr2Num(args[3],10),cmdStr2Num(args[4],10),cmdStr2Num(args[5],10),cmdStr2Num(args[6],10));
    }
  }
  if(args[1] == "PARAM")
  {
    if(args[2] == "PAN")
    { 
      if(args[3] == "MIN") { SrvPan.setMin(cmdStr2Num(args[4], 10)); }      
      if(args[3] == "CENTER") { SrvPan.setCenter(cmdStr2Num(args[4], 10)); }   
      if(args[3] == "MAX") { SrvPan.setMax(cmdStr2Num(args[4], 10)); }   
      if(args[3] == "DEGREE") { SrvPan.setDegree(cmdStr2Num(args[4], 10)); }         
    }else if(args[2] == "TILT")  
    { 
      if(args[3] == "MIN") { SrvTilt.setMin(cmdStr2Num(args[4], 10)); }      
      if(args[3] == "CENTER") { SrvTilt.setCenter(cmdStr2Num(args[4], 10)); }   
      if(args[3] == "MAX") { SrvTilt.setMax(cmdStr2Num(args[4], 10)); }   
      if(args[3] == "DEGREE") { SrvTilt.setDegree(cmdStr2Num(args[4], 10)); }         
    }else if(args[2] == "HOVER")     
    { 
      if(args[3] == "MIN") { SrvHover.setMin(cmdStr2Num(args[4], 10)); }      
      if(args[3] == "CENTER") { SrvHover.setCenter(cmdStr2Num(args[4], 10)); }   
      if(args[3] == "MAX") { SrvHover.setMax(cmdStr2Num(args[4], 10)); }   
      if(args[3] == "DEGREE") { SrvHover.setDegree(cmdStr2Num(args[4], 10)); } 
    }
  }  
}

// COMMAND FROM SERIAL INPUT
void CMD_exec(int arg_cnt, char **args)
{
  //DEBUG START
  for (int i=0; i<arg_cnt; i++)
  {
    Serial.print("Arg ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(args[i]);
  }
  //DEBUG END
  if(args[1] == "U") { SrvTilt.move(1); }
  if(args[1] == "D") { SrvTilt.move(-1); }
  if(args[1] == "L") { SrvPan.move(-1); }
  if(args[1] == "R") { SrvPan.move(1); } 
  if(args[1] == "H") { SrvHover.move(-1); }
  if(args[1] == "V") { SrvHover.move(1); } 
  if(args[1] == "C") { shutt.click(); } 
}



