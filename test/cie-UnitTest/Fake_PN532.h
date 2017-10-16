#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>

// SPI communication is the only supported one at the moment
#define PN532_SCK  (2)
#define PN532_MOSI (3)
#define PN532_SS   (4)
#define PN532_MISO (5)

class Fake_PN532: public Adafruit_PN532 {
public:
    Fake_PN532 ()
    : Adafruit_PN532(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS)
    {
    }
   void begin() {
       Serial.println(F("begin invoked"));
   }
   bool inListPassiveTarget() { 
      Serial.println("inListPassiveTarget invoked");
      return true;
   }
   bool inDataExchange(uint8_t * send, uint8_t sendLength, uint8_t * response, uint8_t * responseLength) {
     Serial.println(F("inDataExchange invoked"));
     return true;
   }
};
