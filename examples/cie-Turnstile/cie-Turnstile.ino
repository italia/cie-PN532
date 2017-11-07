/**************************************************************************/
/*! 
  @file     CIE-Turnstile.ino
  @author   Developers italia
  @license  BSD (see license) 
  This example moves a servo when a CIE is read and print its ID_SERVIZI on
  a 7-segment led display.

  Here's the tutorial and wiring:
  Servo: https://learn.adafruit.com/adafruit-arduino-lesson-14-servo-motors/the-breadboard-layout-for-sweep
  Display: https://learn.adafruit.com/adafruit-led-backpack/0-dot-56-seven-segment-backpack
 

This is an example sketch for the Adafruit PN532 NFC/RFID breakout boards
This library works with the Adafruit NFC breakout 
  ----> https://www.adafruit.com/products/364
 
Check out the links above for our tutorials and wiring diagrams 

*/
/**************************************************************************/
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LEDBackpack.h>
#include <Servo.h>
#include <cie_PN532.h>

// SPI communication is the only supported one at the moment
#define PN532_SCK  (2)
#define PN532_MOSI (3)
#define PN532_SS   (4)
#define PN532_MISO (5)

#define SERVO      (9)
#define GREEN_LED  (10)
#define RED_LED    (11)

// I2C is not supported in this release
//#define PN532_IRQ   (2)
//#define PN532_RESET (3)  // Not connected by default on the NFC Shield

cie_PN532 cie(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);
Adafruit_7segment matrix = Adafruit_7segment();
Servo servo;
word count;

void setup(void) {
  #ifndef ESP8266
    while (!Serial); // for Leonardo/Micro/Zero
  #endif
  Serial.begin(115200);
  cie.begin();  
  //Uncomment this to output the APDU commands sent to the terminal
  //cie.verbose = true;

  matrix.begin(0x70);
  count = 0;
  clearMatrix();

  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);

  servo.attach(SERVO); 
}


void loop(void) {
  
  bool cardDetected = cie.detectCard();
  if (!cardDetected) {
    //No card present, we wait for one
    delay(100);
    return;
  }

  //Good! A card is present, let's read the ID!
  word bufferLength = EF_ID_SERVIZI_LENGTH;
  byte buffer[EF_ID_SERVIZI_LENGTH];
  if (!cie.read_EF_ID_Servizi(buffer, &bufferLength)) {
    Serial.print(F("Error reading EF.ID_SERVIZI"));
    blinkRedLed();
    delay(100);
    return;
  }
  Serial.print(F("EF.ID_Servizi: "));
  cie.printHex(buffer, bufferLength);

  turnGreenLedOn();
  openTurnstile();
  printHi();
  count++;
  delay(1000);
  clearMatrix();
  delay(200);
  printId(buffer);
  delay(2000);
  turnGreenLedOff();
  closeTurnstile();
  printCount();
  

  Serial.println();
  Serial.println(F("Read complete, you can remove the card now"));
  delay(1000);
}

void printHi() {
  if (count > 0) {
    for (byte i = 15; i > 0; i--) {
      matrix.setBrightness(i);
      delay(30);
    }
  }
  matrix.writeDigitRaw(4,0x3F);
  matrix.writeDigitRaw(3,0x77);
  matrix.writeDigitRaw(1,0x30);
  matrix.writeDigitRaw(0,0x39);
  matrix.setBrightness(0);
  matrix.writeDisplay();
  for (byte i = 1; i <= 15; i++) {
    matrix.setBrightness(i);
    delay(30);
  }
}

void clearMatrix() {
  matrix.writeDigitRaw(4,0x00);
  matrix.writeDigitRaw(3,0x00);
  matrix.writeDigitRaw(1,0x00);
  matrix.writeDigitRaw(0,0x00);
  matrix.writeDisplay();
}

void printCount() {
  if (count == 0) {
    clearMatrix();
  } else {
    matrix.printNumber(count, DEC);
    matrix.setBrightness(0);
    matrix.writeDisplay();
    for (byte i = 1; i <= 15; i++) {
      matrix.setBrightness(i);
      delay(30);
    }
  }
}

void printId(byte *buffer) {
  for (int i = 0; i < EF_ID_SERVIZI_LENGTH; i+=2) {
    word actualWord = 0x00;
    actualWord = actualWord | (buffer[i] << 8);
    actualWord = actualWord | buffer[i+1];
    matrix.print(actualWord,HEX);
    matrix.writeDisplay();
    delay(500);
    clearMatrix();
    delay(50);
  }
}

/*void printId(byte *buffer) {
  for (int i = 0; i < (EF_ID_SERVIZI_LENGTH*2)+5; i++) {
    word actualWord = 0x00;
    for (int j = 0; j < EF_ID_SERVIZI_LENGTH; j++) {
      int position = (j*2)-i;
      if (position < 0 && position > -6) {
        //visible!
        word value = (word) buffer[j];
        switch (position) {
          case -1:
          actualWord = actualWord | (value >> 4);
          break;
          case -2:
          actualWord = actualWord | value;
          break;
          case -3:
          actualWord = actualWord | (value << 4);
          break;
          case -4:
          actualWord = actualWord | (value << 8);
          break;
          case -5:
          actualWord = actualWord | ((value & 0x0F) << 12);
          break;
        }
      }
    }

    if (actualWord == 0) {
      clearMatrix();
    } else {
      matrix.print(actualWord,HEX);
      if ((i > 4) && ((actualWord & 0xF000) == 0x00)) {
        matrix.writeDigitNum(0, 0);
      }
      if (i > (EF_ID_SERVIZI_LENGTH*2)) {
        matrix.writeDigitRaw(4,0x00);
      }
      if (i > (EF_ID_SERVIZI_LENGTH*2)+1) {
        matrix.writeDigitRaw(3,0x00);
      }
      if (i > (EF_ID_SERVIZI_LENGTH*2)+2) {
        matrix.writeDigitRaw(1,0x00);
      }
      if (i > (EF_ID_SERVIZI_LENGTH*2)+3) {
        matrix.writeDigitRaw(0,0x00);
      }
      matrix.writeDisplay();
    }
   
   delay(100);
 }
 delay(100);
}*/

void blinkRedLed() {
  digitalWrite(RED_LED, HIGH);
  delay(200);
  digitalWrite(RED_LED, LOW);
  delay(200);
  digitalWrite(RED_LED, HIGH);
  delay(200);
  digitalWrite(RED_LED, LOW);
}

void turnGreenLedOn() {
  digitalWrite(GREEN_LED, HIGH);
}

void turnGreenLedOff() {
  digitalWrite(GREEN_LED, LOW);
}

void openTurnstile() {
  for(byte angle = 90; angle > 0; angle--)    
  {                                
    servo.write(angle);           
    delay(5);       
  } 

}

void closeTurnstile() {
  for(byte angle = 0; angle < 90; angle++)  
  {                                  
    servo.write(angle);               
    delay(5);                   
  }  
}