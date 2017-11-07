/**************************************************************************/
/*! 
    @file     cie_Nfc.h
    @author   Developers Italia
	@license  BSD (see License)
	
	An interface needed by cie_PN532 as a decoupling abstraction to the underlying PN532 communication library

	@section  HISTORY

	v1.0  - Methods for initializing the library, detecting the card and sending APDU commands
	
*/
/**************************************************************************/
#ifndef CIE_NFC
#define CIE_NFC

#include <Arduino.h>
class cie_Nfc {
public:
  virtual void begin() = 0;
  virtual bool detectCard() = 0;
  virtual bool sendCommand(byte *command, byte commandLength, byte *response, word *responseLength) = 0;
  virtual void generateRandomBytes(byte *buffer, const word offset, const byte length) = 0;
};

#endif