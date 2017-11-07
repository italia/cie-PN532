/**************************************************************************/
/*!
    @file     cie_Nfc_Adafruit.cpp
    @author   Developers Italia
    @license  BSD (see License)

	Definition of the cie_Nfc abstract class using the Adafruit_PN532 library	

	@section  HISTORY

	v1.0  - First definition
*/
/**************************************************************************/
#include <Adafruit_PN532.h>
#include "cie_Nfc.h"

#ifndef CIE_NFC_ADAFRUIT
#define CIE_NFC_ADAFRUIT

class cie_Nfc_Adafruit : public cie_Nfc {
  public:
    cie_Nfc_Adafruit();
    cie_Nfc_Adafruit(byte clk, byte miso, byte mosi, byte ss);
    ~cie_Nfc_Adafruit();

    void begin();
    bool detectCard();
    bool sendCommand(byte *command, byte commandLength, byte *response, word *responseLength);
    void generateRandomBytes(byte *buffer, const word offset, const byte length);

  private:
    Adafruit_PN532 *_nfc;
};

#endif