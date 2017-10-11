/**************************************************************************/
/*! 
    @file     cie_PN532.h
    @author   Developers Italia
	@license  BSD (see License)
	

	This is a library for the PN532 NFC/RFID breakout boards
	This library works with the Adafruit NFC breakout 
	----> https://www.adafruit.com/products/364
	
	Check out the links above for our tutorials and wiring diagrams 
	These chips use SPI or I2C to communicate.
	This library uses SPI to communicate.

	@section  HISTORY

	v1.0  - Binary reading of unencrypted elementary files
	
*/
/**************************************************************************/
#include "Adafruit_PN532.h"

// If using the breakout with SPI, define the pins for SPI communication.
#define PN532_SCK  (2)
#define PN532_MOSI (3)
#define PN532_SS   (4)
#define PN532_MISO (5)

// If using the breakout or shield with I2C, define just the pins connected
// to the IRQ and reset lines.  Use the values below (2, 3) for the shield!
#define PN532_IRQ   (2)
#define PN532_RESET (3)  // Not connected by default on the NFC Shield

// Uncomment just _one_ line below depending on how your breakout or shield
// is connected to the Arduino:

#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
// also change #define in Adafruit_PN532.cpp library file
   #define Serial SerialUSB
#endif

//Lengths of Elementary Files found in Root DF
#define EF_DH_LENGTH                          (0x36)
#define EF_ATR_LENGTH                         (0x36)
#define EF_SN_ICC_LENGTH                      (0x0C)

//Lengths of Elementary Files found in CIE DF
#define EF_ID_SERVIZI_LENGTH                  (0x0C)
#define EF_INT_KPUB_LENGTH                    (0x36)
#define EF_SERVIZI_INT_KPUB_LENGTH            (0x36)
#define EF_SOD_LENGTH                         (0x36)

class cie_PN532
{
 public:
  cie_PN532(uint8_t clk, uint8_t miso, uint8_t mosi, uint8_t ss);
  void begin(void);
  bool detectCard();
  
  // Binary read unencrypted elementary files
  bool     read_EF_DH(uint8_t* contentBuffer, uint8_t* contentLength);
  bool     read_EF_ATR(uint8_t* contentBuffer, uint8_t* contentLength);
  bool     read_EF_SN_ICC(uint8_t* contentBuffer, uint8_t* contentLength);
  bool     read_EF_ID_Servizi(uint8_t* contentBuffer, uint8_t* contentLength);
  bool     read_EF_Int_Kpub(uint8_t* contentBuffer, uint8_t* contentLength);
  bool     read_EF_Servizi_Int_Kpub(uint8_t* contentBuffer, uint8_t* contentLength);
  bool     read_EF_SOD(uint8_t* contentBuffer, uint8_t* contentLength);
  void     printHex(uint8_t* buffer, uint8_t length);

 private:
  Adafruit_PN532 _nfc;
  bool     select_IAS_Application(void);
  bool     select_ROOT_DF(void);
  bool     select_CIE_DF(void);
  bool     read_EF(uint8_t efid[], uint8_t* contentBuffer, uint8_t contentLength);
  bool     hasSuccessStatusWord(uint8_t* response, uint8_t responseLength);
  void     clamp(uint8_t* value, uint8_t maxValue);
};
