/*****************************************************************************
* File Name          : UCP.h
* Author             : Alex
* Updated            : Alex
* Version            : V0.1
* Created Date       : 24 Aug, 2015
* Description        : uArm Communication Protocol
* License            : GNU
* Copyright(C) 2015 UFactory Team. All right reserved.
*******************************************************************************/



#ifndef UCP_h
#define UCP_h

#include <inttypes.h>
#include "Arduino.h"  // for digitalRead, digitalWrite, etc

#define UCP_MAJOR_VERSION           0 // for non-compatible changes
#define UCP_MINOR_VERSION           4 // for backwards compatible changes
#define UCP_BUGFIX_VERSION          0 // for bugfix releases

#define REPORT_VERSION          0xF9 // report protocol version
#define SYSTEM_RESET            0xFF // reset from MIDI
//
#define START_SYSEX             0xF0 // start a MIDI Sysex message
#define END_SYSEX               0xF7 // end a MIDI Sysex message


// Extend Firmata SYSEX Command
#define DEBUG_MODE                  0x2F // Enable Debug mode
#define REPORT_FIRMWARE             0x20 // report name and version of the firmware
#define ACTION_CONTROL              0x21 // action control
#define ACTION_CONTROL              0x21 // action control
#define STRING_DATA                 0x22 // String Data

#define PRODUCT_INFO_QUERY			0x24 // ask for product information
#define PRODUCT_INFO_RESPONSE		0x25 // reply with product information

#define UCP_TEST					0x10 // UCP TEST


#define MAX_DATA_BYTES              64 // max number of data bytes in incoming messages

// Extend Coordinate System/POSITION
#define AC_INIT                     0x00 // INIT
// Coordinate System 
#define AC_COOR_RELA_XYZ			0x01 // XYZ AXIS Realtive XYZ
#define AC_COOR_ABS_XYZ				0x02 // XYZ AXIS Absolute XYZ
#define AC_POSITION					0x03 // Position Arm Rotation, Arm Stretch, Arm Height, Arm Hand Rotation
#define AC_COOR_REAL_X				0x04 // XYZ AXIS Absolute X
#define AC_COOR_REAL_Y				0x05 // XYZ AXIS Absolute Y
#define AC_COOR_REAL_Z				0x06 // XYZ AXIS Absolute Z
#define AC_COOR_ABS_X				0x07 // XYZ AXIS Realtive X
#define AC_COOR_ABS_Y				0x08 // XYZ AXIS Realtive Y
#define AC_COOR_ABS_Z				0x09 // XYZ AXIS Realtive Z
#define AC_ANGLE_WITHOUT_OFFSET     0x11 // Angle without offset
// Pump control
#define AC_PUMP                     0x10 // Control PUMP ON /OFF
// Position



#define UCP_SWITCH_ON               0x00
#define UCP_SWITCH_OFF              0x01


extern "C" {
//   // callback function types
  typedef void (*callbackFunction)(byte, int);
  typedef void (*systemResetCallbackFunction)(void);
//   // typedef void (*stringCallbackFunction)(char *);
  typedef void (*sysexCallbackFunction)(byte command, byte argc, byte *argv);
}


// TODO make it a subclass of a generic Serial/Stream base class
class UCPClass
{
public:
	UCPClass();
    /* Arduino constructors */
    void begin();
    void begin(long);
    void begin(Stream &s);
    /* querying functions */
    void printVersion(void);
    // void blinkVersion(void);
    void printFirmwareVersion(void);    
    //void setFirmwareVersion(byte major, byte minor);  // see macro below
    void setFirmwareNameAndVersion(const char *name, byte major, byte minor);   
    /* serial receive handling */
    int available(void);
    void processInput(void);
    void write(byte c);
    /* attach & detach callback functions to messages */
    void attach(byte command, callbackFunction newFunction);
    void attach(byte command, systemResetCallbackFunction newFunction);
    // void attach(byte command, stringCallbackFunction newFunction);
    void attach(byte command, sysexCallbackFunction newFunction);
    void detach(byte command);

    /* utility methods */
    void sendValueAsTwo7bitBytes(short value);
    unsigned short getValuesAsTwobitBytes(byte LSBs, byte MSBb);
    short convertNumToCM(unsigned short number);
    double convertDMSToSec(unsigned short dms);
    void startSysex(void);
    void endSysex(void);  
    void sendString(byte command, const char *string);
    void sendString(const char *string);
    void sendSysex(byte command, byte bytec, byte *bytev);
    boolean debugMode;
private:
    Stream *UCPStream;
    /* firmware name and version */
    byte firmwareVersionCount;
    byte *firmwareVersionVector;
    /* input message handling */
    // byte waitForData; // this flag says the next serial input will be data
    // byte executeMultiByteCommand; // execute this after getting multi-byte data
    byte multiByteChannel; // channel data for multiByteCommands
    byte storedInputData[MAX_DATA_BYTES]; // multi-byte data
    /* sysex */
    boolean parsingSysex;
    int sysexBytesRead;
    /* callback functions */
    // callbackFunction currentAnalogCallback;
    // callbackFunction currentDigitalCallback;
    // callbackFunction currentReportAnalogCallback;
    // callbackFunction currentReportDigitalCallback;
    // callbackFunction currentPinModeCallback;
    systemResetCallbackFunction currentSystemResetCallback;
    // stringCallbackFunction currentStringCallback;
    sysexCallbackFunction currentSysexCallback;

    /* private methods ------------------------------ */
    void processSysexMessage(void);
    void systemReset(void);
    // void sysexCallback(byte command, byte argc, byte *argv);
    // void strobeBlinkPin(int count, int onInterval, int offInterval);
};

extern UCPClass ucp;

/*==============================================================================
 * MACROS
 *============================================================================*/

/* shortcut for setFirmwareNameAndVersion() that uses __FILE__ to set the
 * firmware name.  It needs to be a macro so that __FILE__ is included in the
 * firmware source file rather than the library source file.
 */
#define setFirmwareVersion(x, y)   setFirmwareNameAndVersion(__FILE__, x, y)

#endif 