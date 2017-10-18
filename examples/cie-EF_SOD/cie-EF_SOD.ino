/**************************************************************************/
/*! 
  @file     CIE-EF_SOD.ino
  @author   Developers italia
  @license  BSD (see license) 
  This example will wait for a CIE card and read its BER-Encoded hierarcy 
 

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
}


void loop(void) {
  
  bool cardDetected = cie.detectCard();
  if (!cardDetected) {
    //No card present, we wait for one
    delay(100);
    return;
  }

  //Good! A card is present, let's read the structure of the EF.SOD file!
  //It's the largest Elementary File in the CIE card
  cie_BerTriple* rootTriple;

  //Take the time
  unsigned long startedAt = millis();
  if (cie.parse_EF_SOD(rootTriple)) {
    printTriple(*rootTriple, 0);


  } else {
    Serial.print(F("Error reading triples"));
  }
  Serial.print("It took ");
  Serial.print(millis()-startedAt);
  Serial.println(" ms");

  Serial.println();
  Serial.println(F("Read complete, you can remove the card now"));
  delay(1000);
}

void printTriple(const cie_BerTriple triple, byte depth) {
  if (depth > 0) {
    for (byte i = 0; i < depth; i++) {
      Serial.print("  ");
    }
  }

  Serial.print(getStringForType(triple.classification, triple.type));
  Serial.print(" (offset: ");
  Serial.print(triple.contentOffset);
  Serial.print(", length: ");
  Serial.print(triple.contentLength);
  if (triple.encoding == 0x01) {
    Serial.print(", children: ");
    Serial.print(triple.childrenCount);
  }
  Serial.println(")");

  for (byte i = 0; i < triple.childrenCount; i++) {
    cie_BerTriple* child = triple.children[i];
    printTriple(*child, depth+1);
  }

}

__FlashStringHelper* getStringForType(const byte classification, const byte type) {

  if (classification > 0 ) {
    switch (classification) {
      case 1:
        return F("APPLICATION");
      case 2: 
        //TODO: return the actual offset
        return F("[0]");
      case 3:
        return F("PRIVATE");
    }
  }

  switch(type) {
    case 0x00:
      return F("END OF CONTENT");
    case 0x01:
      return F("BOOLEAN");
    case 0x02:
      return F("INTEGER");
    case 0x03:
      return F("BIT STRING");
    case 0x04:
      return F("OCTET STRING");
    case 0x05:
      return F("NULL");
    case 0x06:
      return F("OBJECT IDENTIFIER");
    case 0x07:
      return F("OBJECT DESCRIPTOR");
    case 0x08:
      return F("EXTERNAL");
    case 0x09:
      return F("REAL");
    case 0x0A:
      return F("ENUMERATED");
    case 0x0B:
      return F("EMBEDDED PDV");
    case 0x0C:
      return F("UTF8 STRING");
    case 0x0D:
      return F("RELATIVE OID");
    case 0x10:
      return F("SEQUENCE");
    case 0x11:
      return F("SET");
    case 0x12:
      return F("NUMERIC STRING");
    case 0x13:
      return F("PRINTABLE STRING");
    case 0x14:
      return F("T61 STRING");
    case 0x15:
      return F("VIDEOTEX STRING");
    case 0x16:
      return F("IA5 STRING");
    case 0x17:
      return F("UTC TIME");
    case 0x18:
      return F("GENERALIZED TIME");
    case 0x19:
      return F("GRAPHIC STRING");
    case 0x1A:
      return F("VISIBLE STRING");
    case 0x1B:
      return F("GENERAL STRING");
    case 0x1C:
      return F("UNIVERSAL STRING");
    case 0x1D:
      return F("CHARACTER STRING");
    case 0x1E:
      return F("BMP STRING");
    default:
      return F("UNKNOWN");
  }
}