/**************************************************************************/
/*! 
  @file     CIE-ReadID.ino
  @author   Developers italia
  @license  BSD (see license) 
  This example will wait for a CIE card and read its ID used as a 
  fast but low security service identifier.
 

This is an example sketch for the Adafruit PN532 NFC/RFID breakout boards
This library works with the Adafruit NFC breakout 
  ----> https://www.adafruit.com/products/364
 
Check out the links above for our tutorials and wiring diagrams 

*/
/**************************************************************************/
#include <Wire.h>
#include <SPI.h>
#include <cie_PN532.h>

// SPI communication is the only supported one at the moment
#define PN532_SCK  (2)
#define PN532_MOSI (3)
#define PN532_SS   (4)
#define PN532_MISO (5)

// I2C is not supported in this release
//#define PN532_IRQ   (2)
//#define PN532_RESET (3)  // Not connected by default on the NFC Shield


cie_PN532 cie(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);

void setup(void) {
  #ifndef ESP8266
    while (!Serial); // for Leonardo/Micro/Zero
  #endif
  Serial.begin(115200);
  cie.begin();
  //Uncomment this to output the APDU commands sent to the terminal
  //cie.verbose = true;
}


void loop(void) {
  
  bool cardDetected = cie.detectCard();
  if (!cardDetected) {
    //No card present, we wait for one
    delay(100);
    return;
  }

  //Good! A card is present, let's read the NIS!
  word bufferLength = EF_ID_SERVIZI_LENGTH;
  byte buffer[EF_ID_SERVIZI_LENGTH];
  unsigned long startTime = millis();
  if (!cie.read_EF_ID_Servizi(buffer, &bufferLength)) {
    Serial.print(F("Error reading EF.ID_SERVIZI"));
    delay(1000);
    return;
  }
  Serial.print(F("EF.ID_Servizi: "));
  cie.printHex(buffer, bufferLength);
  Serial.print(F("Reading the ID_Servizi took "));
  Serial.print(millis()-startTime);
  Serial.println(F("ms"));

  //Verify the card is valid
  //WARNING: CARD VERIFICATION HAS NOT BEEN IMPLEMENTED YET
  /*startTime = millis();
  bool isValid = cie.isCardValid();
  Serial.print(F("Card is "));
  Serial.println(isValid ? F("valid") : F("NOT valid - it's probably a clone"));
  Serial.print(F("Checking the card for validity took "));
  Serial.print(millis()-startTime);
  Serial.println(F("ms"));*/

  Serial.println();
  Serial.print(F("Free memory left: "));
  Serial.print(freeRam());
  Serial.println(F(" bytes"));
  Serial.println(F("Read complete, you can remove the card now"));
  delay(1000);
}

int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}