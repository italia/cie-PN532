#include "cie_Nfc_Mock.h"

void cie_Nfc_Mock::begin() {
  //Do nothing, we do not need to initialize anything in this mock class
}

bool cie_Nfc_Mock::detectCard() {
  //Always return true, simulate a card is present
  return true;
}

bool cie_Nfc_Mock::sendCommand(byte* send, byte sendLength, byte* response, byte* responseLength) {
    
}