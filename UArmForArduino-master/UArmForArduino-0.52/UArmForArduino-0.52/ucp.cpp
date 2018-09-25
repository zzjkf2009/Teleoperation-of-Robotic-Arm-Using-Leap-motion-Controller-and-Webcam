/******************************************************************************
* File Name          : UCP.cpp
* Author             : Alex
* Updated            : Alex
* Version            : V0.5
* Created Date       : 24 Aug, 2015
* Description        : uArm Communication Protocol
* License            : GNU
* Copyright(C) 2015 UFactory Team. All right reserved.
*******************************************************************************/

#include "ucp.h"
#include "HardwareSerial.h"

extern "C" {
#include <string.h>
#include <stdlib.h>
}


//******************************************************************************
//* Support Functions
//******************************************************************************

UCPClass ucp;

void UCPClass::sendValueAsTwo7bitBytes(short value)
{
  UCPStream->write(value & B01111111); // LSB
  UCPStream->write(value >> 7 & B01111111); // MSB
}

void UCPClass::startSysex(void)
{
  UCPStream->write(START_SYSEX);
}

void UCPClass::endSysex(void)
{
  UCPStream->write(END_SYSEX);
}

// //******************************************************************************
// //* Constructors
// //******************************************************************************

UCPClass::UCPClass()
{
  firmwareVersionCount = 0;
  firmwareVersionVector = 0;
  debugMode = false;
  systemReset();
}

//******************************************************************************
//* Public Methods
//******************************************************************************

/* begin method with default serial bitrate */
void UCPClass::begin(void)
{
  begin(57600);
}

// /* begin method for overriding default serial bitrate */
void UCPClass::begin(long speed)
{
  Serial.begin(speed);
  UCPStream = &Serial;
  // blinkVersion();
  printVersion();
  printFirmwareVersion();
}

//  begin method for overriding default stream 
void UCPClass::begin(Stream &s)
{
  UCPStream = &s;
  // do not call blinkVersion() here because some hardware such as the
  // Ethernet shield use pin 13
  printVersion();
  printFirmwareVersion();
}

void UCPClass::printFirmwareVersion(void)
{
  byte i;

  if (firmwareVersionCount) { // make sure that the name has been set before reporting
    startSysex();
    UCPStream->write(REPORT_FIRMWARE);
    UCPStream->write(firmwareVersionVector[0]); // major version number
    UCPStream->write(firmwareVersionVector[1]); // minor version number
    for (i = 2; i < firmwareVersionCount; ++i) {
      sendValueAsTwo7bitBytes(firmwareVersionVector[i]);
    }
    endSysex();
  }
}

void UCPClass::setFirmwareNameAndVersion(const char *name, byte major, byte minor)
{
  const char *firmwareName;
  const char *extension;

  // parse out ".cpp" and "applet/" that comes from using __FILE__
  extension = strstr(name, ".cpp");
  firmwareName = strrchr(name, '/');

  if (!firmwareName) {
    // windows
    firmwareName = strrchr(name, '\\');
  }
  if (!firmwareName) {
    // user passed firmware name
    firmwareName = name;
  } else {
    firmwareName ++;
  }

  if (!extension) {
    firmwareVersionCount = strlen(firmwareName) + 2;
  } else {
    firmwareVersionCount = extension - firmwareName + 2;
  }

  // in case anyone calls setFirmwareNameAndVersion more than once
  free(firmwareVersionVector);

  firmwareVersionVector = (byte *) malloc(firmwareVersionCount + 1);
  firmwareVersionVector[firmwareVersionCount] = 0;
  firmwareVersionVector[0] = major;
  firmwareVersionVector[1] = minor;
  strncpy((char *)firmwareVersionVector + 2, firmwareName, firmwareVersionCount - 2);
}

//------------------------------------------------------------------------------
// Serial Receive Handling

int UCPClass::available(void)
{
  return UCPStream->available();
}


void UCPClass::processSysexMessage(void)
{
  switch (storedInputData[0]) { //first byte in buffer is command
    default:
      if (currentSysexCallback)
        (*currentSysexCallback)(storedInputData[0], sysexBytesRead - 1, storedInputData + 1);
  }
}

// output the protocol version message to the serial port
void UCPClass::printVersion(void)
{
  UCPStream->write(REPORT_VERSION);
  UCPStream->write((byte)UCP_MAJOR_VERSION);
  UCPStream->write((byte)UCP_MINOR_VERSION);
  UCPStream->write((byte)UCP_BUGFIX_VERSION);
}

// expose the write method
void UCPClass::write(byte c)
{
  UCPStream->write(c);
}

void UCPClass::processInput(void)
{
  int inputData = UCPStream->read(); // this is 'int' to handle -1 when no data
  int command;

  // TODO make sure it handles -1 properly

  if (parsingSysex) {
    if (inputData == END_SYSEX) {
      //stop sysex byte
      parsingSysex = false;
      //fire off handler function
      processSysexMessage();
    } else {
      //normal data byte - add to buffer
      storedInputData[sysexBytesRead] = inputData;
      sysexBytesRead++;
    }
  } 
  else {
    // remove channel info from command byte if less than 0xF0
    if (inputData < 0xF0) {
      command = inputData & 0xF0;
      // multiByteChannel = inputData & 0x0F;
    } else {
      command = inputData;
      // commands in the 0xF* range don't use channel data
    }
    switch (command) {
      case START_SYSEX:
        parsingSysex = true;
        sysexBytesRead = 0;
        break;
      case SYSTEM_RESET:
        systemReset();
        break;
      case REPORT_VERSION:
        ucp.printVersion();
        break;
    }
  }
}



void UCPClass::systemReset(void)
{
  byte i;
  for (i = 0; i < MAX_DATA_BYTES; i++) {
    storedInputData[i] = 0;
  }

  parsingSysex = false;
  sysexBytesRead = 0;

}



void UCPClass::attach(byte command, systemResetCallbackFunction newFunction)
{
  switch (command) {
    case SYSTEM_RESET: currentSystemResetCallback = newFunction; break;
  }
}

void UCPClass::attach(byte command, sysexCallbackFunction newFunction)
{
  currentSysexCallback = newFunction;
}

void UCPClass::detach(byte command)
{
  switch (command) {
    // case STRING_DATA: currentStringCallback = NULL; break;
    case START_SYSEX: currentSysexCallback = NULL; break;
    default:
      attach(command, (callbackFunction)NULL);
  }
}

short UCPClass::convertNumToCM(unsigned short num)
{
  
     short dis = num;
     if(dis<=8191)
     {
         dis=dis;
     }
     else
     {
         dis-=8192;
         dis=-dis;
     }
     return dis;
} 


double UCPClass::convertDMSToSec(unsigned short dms){
    return dms /10;
}

void UCPClass::sendSysex(byte command, byte bytec, byte *bytev)
{
  byte i;
  startSysex();
  UCPStream->write(command);
  for (i = 0; i < bytec; i++) {
    sendValueAsTwo7bitBytes(bytev[i]);
  }
  endSysex();
}

void UCPClass::sendString(byte command, const char *string)
{
  sendSysex(command, strlen(string), (byte *)string);
}


// send a string as the protocol string type
void UCPClass::sendString(const char *string)
{
  sendString(STRING_DATA, string);
}

unsigned short UCPClass::getValuesAsTwobitBytes(byte LSBb, byte MSBb){
  if(LSBb < 0x7F || MSBb < 0x7F){
      return (LSBb + (MSBb << 7));
  }
}
