/**************************************************************************/
/*! 
  @file     CIE-MemoryDump.ino
  @author   Developers italia
  @license  BSD (see license) 
  This example will wait for any ISO14443A card or tag, and
  depending on the size of the UID will attempt to read from it.
 

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
  Serial.println("PN532 initialized, waiting for a CIE card...");
}


void loop(void) {
  
  bool cardDetected = cie.detectCard();
  if (!cardDetected) {
    //No card present, we wait for one
    delay(100);
    return;
  }

  //Good! A new card is present, let's read it!
  uint8_t bufferLength = EF_ID_SERVIZI_LENGTH;
  uint8_t buffer[bufferLength];

  //EF.ID_Servizi
  cie.read_EF_ID_Servizi(buffer, &bufferLength);
  Serial.print("Here's the card identifier: ");
  cie.printHex(buffer, bufferLength);

  //While we're at it, let's also dump all of its public data to the serial monitor
  cie.dumpAll();

  Serial.println("Work complete, remove the card");
  delay(1000);
}

