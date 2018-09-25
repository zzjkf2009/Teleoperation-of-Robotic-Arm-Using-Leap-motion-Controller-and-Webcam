/************************************************************************
* File Name          : Standard
* Author             : Evan
* Updated            : Evan
* Version            : V0.0.2
* Date               : 15 June, 2014
* Description        : Mouse Control or Leap Motion Control(Processing), 
                       Calibration Mode & Rrecording Mode.
* License            : 
* Copyright(C) 2014 UFactory Team. All right reserved.
* *************************************************************************
* Updated            : Alex
* Date               : 04 Mar, 2015
* Version            : V0.0.2
* Description        : CtrlData 0x80 for RESET
*************************************************************************/
#include <EEPROM.h>
#include <UF_uArm_Metal.h>

int  heightTemp  = 0, stretchTemp = 0, rotationTemp = 0, handRotTemp = 0;
char stateMachine = 0, counter = 0;
char dataBuf[9] = {0};
UF_uArm uarm;           // initialize the uArm library 

void setup() 
{
  Serial.begin(9600);  // start serial port at 9600 bps
//  while(!Serial);     // wait for serial port to connect. Needed for Leonardo only
  uarmReset();
}

void loop()
{
  uarm.calibration();      // if corrected, you could remove it, no harm though
  uarm.recordingMode(50);  // sampling time: 50ms, Recording time: 17 seconds. 
  if(Serial.available())
  {
    byte rxBuf = Serial.read();
    if(stateMachine == 0)
    {
      stateMachine = rxBuf == 0xFF? 1:0;
    }
    else if(stateMachine == 1)
    {
      stateMachine = rxBuf == 0xAA? 2:0;
    }
    else if(stateMachine == 2)
    {
      dataBuf[counter++] = rxBuf;
      if(counter > 8)  // receive 9 byte data
      {
        stateMachine = 0;
        counter=0;
        if(dataBuf[8] & RESET)
        {
           uarmReset();
         }
         else{
            *((char *)(&rotationTemp)  )  = dataBuf[1]; // recevive 1byte
            *((char *)(&rotationTemp)+1)  = dataBuf[0]; 
            *((char *)(&stretchTemp )  )  = dataBuf[3]; 
            *((char *)(&stretchTemp )+1)  = dataBuf[2]; 
            *((char *)(&heightTemp  )  )  = dataBuf[5]; 
            *((char *)(&heightTemp  )+1)  = dataBuf[4]; 
            *((char *)(&handRotTemp )  )  = dataBuf[7]; 
            *((char *)(&handRotTemp )+1)  = dataBuf[6]; 
           // uarm.setPosition(stretchTemp, heightTemp, rotationTemp, handRotTemp);
           uarm.setPosition( heightTemp,stretchTemp, rotationTemp, handRotTemp);
            /* pump action, Valve Stop. */
            if(dataBuf[8] & CATCH)   uarm.gripperCatch();
            /* pump stop, Valve action. 
               Note: The air relief valve can not work for a long time, 
               should be less than ten minutes. */
            if(dataBuf[8] & RELEASE) uarm.gripperRelease();
          }
      }
    }
  }
  /* delay release valve, this function must be in the main loop */
  uarm.gripperDetach();  
} 

void uarmReset(){
  uarm.init();          // initialize the uArm position
  uarm.setServoSpeed(SERVO_R,    0);  // 0=full speed, 1-255 slower to faster
  uarm.setServoSpeed(SERVO_L,    0);  // 0=full speed, 1-255 slower to faster
  uarm.setServoSpeed(SERVO_ROT, 50);  // 0=full speed, 1-255 slower to faster
}

