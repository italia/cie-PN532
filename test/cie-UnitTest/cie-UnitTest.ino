/**************************************************************************/
/*! 
  @file     CIE-UnitTest.ino
  @author   Developers italia
  @license  BSD (see license) 
  Unit tests for cie-PN532.

This is an example sketch for the Adafruit PN532 NFC/RFID breakout boards
This library works with the Adafruit NFC breakout 
  ----> https://www.adafruit.com/products/364
 
Check out the links above for our tutorials and wiring diagrams 

*/
/**************************************************************************/
#include <Wire.h>
#include <SPI.h>
#include <cie_PN532.h>
#include <ArduinoUnit.h>
#include "Fake_PN532.h"

Fake_PN532 fake;
cie_PN532 cie(fake);

test(ok) 
{
  int x=3;
  int y=3;
  assertEqual(x,y);
}

test(bad)
{
  bool result = cie.detectCard();
  assertNotEqual(result,true);
}

void setup(void) {
  #ifndef ESP8266
    while (!Serial); // for Leonardo/Micro/Zero
  #endif
  Serial.begin(115200);
}


void loop(void) {
  Test::run();
}
