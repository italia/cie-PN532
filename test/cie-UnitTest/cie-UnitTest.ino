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
//With this hack we can test even private methods
#define private public

#include <Wire.h>
#include <SPI.h>
#include <ArduinoUnit.h>
#include <cie_PN532.h>
#include "cie_Nfc_Mock.h"
#include "cie_Command.h"

//cie_PN532
test(hasSuccessStatusWord_must_return_true_when_the_last_octets_in_a_response_are_0x9000)
{
  cie_Nfc_Mock mock;
  cie_PN532 cie(&mock);
  byte response1[] = {0x60, 0x03, 0x90, 0x00};
  byte response2[] = {0x90, 0x00, 0x60, 0x00};

  bool result1 = cie.hasSuccessStatusWord(response1, sizeof(response1));
  bool result2 = cie.hasSuccessStatusWord(response2, sizeof(response2));

  assertEqual(true, result1);
  assertEqual(false, result2);
}

test(clamp_must_return_the_lesser_of_the_two_values)
{
  cie_Nfc_Mock mock;
  cie_PN532 cie(&mock);
  word largeValue = 0xFFFF;
  byte smallValue = 0x04;
  byte maximumValue = 0x80;

  byte clampedValue1 = cie.clamp(largeValue, maximumValue);
  byte clampedValue2 = cie.clamp(smallValue, maximumValue);

  assertEqual(maximumValue, clampedValue1);
  assertEqual(smallValue, clampedValue2);
}

test(selectIasApplication_must_send_a_select_command) {
  cie_Nfc_Mock mock;
  cie_PN532 cie(&mock);
  mock.expectCommands(1);
  mock.expectCommand({0x00}, 1, 0, {0x00}, 1);
  bool success = cie.selectIasApplication();
  byte command[] = {0x00, 0x00};
  byte responseLength = 2;
  byte response[2];
  //fake.inDataExchange(command, sizeof(command), response, responseLength);
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
