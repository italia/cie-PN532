/**************************************************************************/
/*! 
  @file     CIE-ReadAll.ino
  @author   Developers italia
  @license  BSD (see license) 
  This example will wait for a CIE card and read all of its unencrypted
  values.
 

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

  //Good! A card is present, let's dump some info!
  print_EF_ID_Servizi();
  print_EF_SN_ICC();
  print_EF_Int_Kpub();
  print_EF_Servizi_Int_Kpub();
  print_EF_SOD();
  print_EF_DH();
  print_EF_ATR();

  Serial.println();
  Serial.println("Work complete, remove the card");
  delay(1000);
}
//This example should use function pointers to reduce the amount of lines of code
/*void printValue(void (*pFunc)(uint8_t* buffer, uint8_t* bufferLength), char[] name, uint8_t bufferLength) {
  uint8_t buffer[bufferLength];
  if (!(*pFunc)(buffer, &bufferLength)) {
    Serial.print("Error reading ");
    Serial.println(name);
    return;
  }
  Serial.print(name);
  Serial.print(": ");
  cie.printHex(buffer, bufferLength);
}*/

void print_EF_ID_Servizi() {
  uint8_t bufferLength = EF_ID_SERVIZI_LENGTH;
  uint8_t buffer[bufferLength];
  if (!cie.read_EF_ID_Servizi(buffer, &bufferLength)) {
    Serial.print("Error reading EF.ID_SERVIZI");
    return;
  }
  Serial.print("EF.ID_Servizi: ");
  cie.printHex(buffer, bufferLength);
}

void print_EF_Int_Kpub() {
  uint8_t bufferLength = EF_INT_KPUB_LENGTH;
  uint8_t buffer[bufferLength];
  if (!cie.read_EF_Int_Kpub(buffer, &bufferLength)) {
    Serial.print("Error reading EF.Int.Kpub");
    return;
  }
  Serial.print("EF.Int.Kpub: ");
  cie.printHex(buffer, bufferLength);
}

void print_EF_Servizi_Int_Kpub() {
  uint8_t bufferLength = EF_SERVIZI_INT_KPUB_LENGTH;
  uint8_t buffer[bufferLength];
  if (!cie.read_EF_Servizi_Int_Kpub(buffer, &bufferLength)) {
    Serial.print("Error reading EF.Servizi_Int.Kpub");
    return;
  }
  Serial.print("EF.Servizi_Int.Kpub: ");
  cie.printHex(buffer, bufferLength);
}

void print_EF_SOD() {
  uint8_t bufferLength = EF_SOD_LENGTH;
  uint8_t buffer[bufferLength];
  if (!cie.read_EF_SOD(buffer, &bufferLength)) {
    Serial.print("Error reading EF.SOD");
    return;
  }
  Serial.print("EF.SOD: ");
  cie.printHex(buffer, bufferLength);
}

void print_EF_DH() {
  uint8_t bufferLength = EF_DH_LENGTH;
  uint8_t buffer[bufferLength];
  if (!cie.read_EF_DH(buffer, &bufferLength)) {
    Serial.print("Error reading EF.DH");
    return;
  }
  Serial.print("EF.DH: ");
  cie.printHex(buffer, bufferLength);
}

void print_EF_ATR() {
  uint8_t bufferLength = EF_ATR_LENGTH;
  uint8_t buffer[bufferLength];
  if (!cie.read_EF_ATR(buffer, &bufferLength)) {
    Serial.print("Error reading EF.ATR");
    return;
  }
  Serial.print("EF.ATR: ");
  cie.printHex(buffer, bufferLength);
}

void print_EF_SN_ICC() {
  uint8_t bufferLength = EF_SN_ICC_LENGTH;
  uint8_t buffer[bufferLength];
  if (!cie.read_EF_SN_ICC(buffer, &bufferLength)) {
    Serial.print("Error reading EF.SN.ICC");
    return;
  }
  Serial.print("EF.SN.ICC: ");
  cie.printHex(buffer, bufferLength);
}
