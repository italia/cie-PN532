#include <Wire.h>
#include <SPI.h>
#include <cie_Nfc.h>

#ifndef CIE_NFC_MOCK
#define CIE_NFC_MOCK

class cie_Nfc_Mock: public cie_Nfc {
  public:
    void begin();
    bool detectCard();
    bool sendCommand(byte* send, byte sendLength, byte* response, byte* responseLength);
};

#endif