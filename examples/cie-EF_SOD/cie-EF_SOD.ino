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

byte triplesCount;
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

  //Good! A card is present, let's read the structure of the EF.SOD file!
  //It's the largest Elementary File in the CIE card
  triplesCount = 0;

  //Take the time
  unsigned long startedAt = millis();
  if (!cie.parse_EF_SOD(printTriple)) {
    Serial.print(F("Error reading triples"));
  }
  Serial.print("It took ");
  Serial.print(millis()-startedAt);
  Serial.print(" ms, ");
  Serial.print(triplesCount);
  Serial.print(" triples found, we have ");
  Serial.print(freeRam());
  Serial.print(" bytes of free memory.");

  

  Serial.println();
  Serial.println(F("Read complete, you can remove the card now"));
  delay(1000);
}

bool printTriple(cie_BerTriple *triple) {
  triplesCount+=1;
  if (triple->depth > 0) {
    for (byte i = 0; i < triple->depth; i++) {
      Serial.print("  ");
    }
  }

  printNameForType(triple->classification, triple->type);
  Serial.print(" (offset: ");
  Serial.print(triple->offset);
  Serial.print(", length: ");
  Serial.print(triple->contentOffset - triple->offset);
  Serial.print("+");
  Serial.print(triple->contentLength);
  if (triple->encoding == 0x01) {
    Serial.println(", constructed)");
  } else {
    Serial.println(", primitive)");
  }

  return true;

}

void printNameForType(const byte classification, const byte type) {

  if (classification > 0 ) {
    switch (classification) {
      case 1:
        Serial.print(F("APPLICATION"));
        return;
      case 2: 
        //TODO: return the actual offset
        Serial.print(F("["));
        Serial.print(type);
        Serial.print(F("]"));
        return;
      case 3:
      Serial.print(F("PRIVATE"));
      return;
    }
  }

  switch(type) {
    case 0x00:
      Serial.print(F("END OF CONTENT"));
      break;
    case 0x01:
      Serial.print(F("BOOLEAN"));
      break;
    case 0x02:
      Serial.print(F("INTEGER"));
      break;
    case 0x03:
      Serial.print(F("BIT STRING"));
      break;
    case 0x04:
      Serial.print(F("OCTET STRING"));
      break;
    case 0x05:
      Serial.print(F("NULL"));
      break;
    case 0x06:
      Serial.print(F("OBJECT IDENTIFIER"));
      break;
    case 0x07:
      Serial.print(F("OBJECT DESCRIPTOR"));
      break;
    case 0x08:
      Serial.print(F("EXTERNAL"));
      break;
    case 0x09:
      Serial.print(F("REAL"));
      break;
    case 0x0A:
      Serial.print(F("ENUMERATED"));
      break;
    case 0x0B:
      Serial.print(F("EMBEDDED PDV"));
      break;
    case 0x0C:
      Serial.print(F("UTF8 STRING"));
      break;
    case 0x0D:
      Serial.print(F("RELATIVE OID"));
      break;
    case 0x10:
      Serial.print(F("SEQUENCE"));
      break;
    case 0x11:
      Serial.print(F("SET"));
      break;
    case 0x12:
      Serial.print(F("NUMERIC STRING"));
      break;
    case 0x13:
      Serial.print(F("PRINTABLE STRING"));
      break;
    case 0x14:
      Serial.print(F("T61 STRING"));
      break;
    case 0x15:
      Serial.print(F("VIDEOTEX STRING"));
      break;
    case 0x16:
      Serial.print(F("IA5 STRING"));
      break;
    case 0x17:
      Serial.print(F("UTC TIME"));
      break;
    case 0x18:
      Serial.print(F("GENERALIZED TIME"));
      break;
    case 0x19:
      Serial.print(F("GRAPHIC STRING"));
      break;
    case 0x1A:
      Serial.print(F("VISIBLE STRING"));
      break;
    case 0x1B:
      Serial.print(F("GENERAL STRING"));
      break;
    case 0x1C:
      Serial.print(F("UNIVERSAL STRING"));
      break;
    case 0x1D:
      Serial.print(F("CHARACTER STRING"));
      break;
    case 0x1E:
      Serial.print(F("BMP STRING"));
      break;
    default:
      Serial.print(F("UNKNOWN"));
      break;
  }
}

int freeRam ()
{
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}