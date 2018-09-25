
/******************************************************************************
* File Name          : Firmata-UCP.ino
* Author             : Alex
* Updated            : Alex
* Version            : V0.5
* Created Date       : 24 Aug, 2015
* Description        : Firmata-UCP(uArm Communication Protocol)
* License            : GNU
* Copyright(C) 2015 UFactory Team. All right reserved.
*******************************************************************************/

#include <Servo.h>
#include <Wire.h>

// Firmata-UCP
#include <EEPROM.h>
#include "uarm_library.h"
#include "ucp.h"
#include "Firmata.h"


void setup()
{
	ucp.setFirmwareVersion(UCP_MAJOR_VERSION, UCP_MINOR_VERSION);
	ucp.attach(START_SYSEX, sysexCallback);

	ucp.begin(57600);

}

void loop()
{

  /* STREAMREAD - processing incoming messagse as soon as possible, while still
   * checking digital inputs.  */	
  while (ucp.available())
    ucp.processInput();
}


void sysexCallback(byte command, byte argc, byte *argv)
{

	switch (command) {
    case ACTION_CONTROL:

        switch(argv[0]){
          case AC_COOR_RELA_XYZ:
          {
            short x = ucp.convertNumToCM(ucp.getValuesAsTwobitBytes(argv[1],argv[2]));
            short y = ucp.convertNumToCM(ucp.getValuesAsTwobitBytes(argv[3],argv[4]));
            short z = ucp.convertNumToCM(ucp.getValuesAsTwobitBytes(argv[5],argv[6]));
            short t = ucp.convertDMSToSec(ucp.getValuesAsTwobitBytes(argv[7],argv[8]));
            uarm.moveTo(x,y,z,RELATIVE,t);
           if(ucp.debugMode)
            {
              // Serial.print("X is: ");
              // Serial.println(x);
              // Serial.print("Y is: ");
              // Serial.println(y);
              // Serial.print("Z is: ");
              // Serial.println(z);
              // Serial.print("time is: ");
              // Serial.println(t);
            }
            break;
          }

          case AC_COOR_ABS_XYZ:
          {
            short x = ucp.convertNumToCM(ucp.getValuesAsTwobitBytes(argv[1],argv[2]));
            short y = ucp.convertNumToCM(ucp.getValuesAsTwobitBytes(argv[3],argv[4]));
            short z = ucp.convertNumToCM(ucp.getValuesAsTwobitBytes(argv[5],argv[6]));
            short t = ucp.convertDMSToSec(ucp.getValuesAsTwobitBytes(argv[7],argv[8]));
            uarm.moveTo(x,y,z,ABSOLUTE,t); //   
            if(ucp.debugMode)
            {
               // ucp.sendString("Debug Mode Test");
               // Serial.print("X is: ");
               // Serial.println(x);
               // Serial.print("Y is: ");
               // Serial.println(y);
               // Serial.print("Z is: ");
               // Serial.println(z);
               // Serial.print("time is: ");
               // Serial.println(t);
            }
            break;
          }

          case AC_ANGLE_WITHOUT_OFFSET:
          {
            short servo_rot = constrain(ucp.getValuesAsTwobitBytes(argv[1],argv[2]),0,180);
            short servo_left = constrain(ucp.getValuesAsTwobitBytes(argv[3],argv[4]),0,180);
            short servo_right = constrain(ucp.getValuesAsTwobitBytes(argv[5],argv[6]),0,180);
            Serial.println("");
            Serial.println(servo_rot);
            Serial.println(servo_left);
            Serial.println(servo_right);
            uarm.writeAngle(servo_rot,servo_left,servo_right,servo_rot);

          }

          case AC_PUMP:
          {
            if (argv[1] == UCP_SWITCH_ON)
              uarm.gripperCatch();
            else
              uarm.gripperRelease();
            break;
          }
        }
        break;
    case REPORT_FIRMWARE:
      ucp.printFirmwareVersion();
      break;
    case DEBUG_MODE:
      switch(argv[0]){
        case UCP_SWITCH_ON:
          ucp.debugMode = true;
          break;
        case UCP_SWITCH_OFF:
          ucp.debugMode = false;
          break;
        }
      break;    
    case UCP_TEST:
        // Serial.print("UCP TEST");
        break;
  	}
}


