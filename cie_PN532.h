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

//Lengths of fixed size elementary Files
#define EF_ID_SERVIZI_LENGTH                  (0x0C)
#define EF_SN_ICC_LENGTH                      (0x0C)

//Lengths
#define PAGE_LENGTH                           (0x36)
#define AUTODETECT_BER_LENGTH                 (0x00)
#define FIXED_LENGTH                          (0x01)

//Paths
#define ROOT_MF                               (0x00)
#define CIE_DF                                (0x01)

class cie_PN532
{
 public:
  cie_PN532(byte clk, byte miso, byte mosi, byte ss);
  void begin(void);
  bool detectCard();
  
  // Binary read unencrypted elementary files
  bool     read_EF_DH(byte* contentBuffer, word* contentLength);
  bool     read_EF_ATR(byte* contentBuffer, word* contentLength);
  bool     read_EF_SN_ICC(byte* contentBuffer, word* contentLength);
  bool     read_EF_ID_Servizi(byte* contentBuffer, word* contentLength);
  bool     read_EF_Int_Kpub(byte* contentBuffer, word* contentLength);
  bool     read_EF_Servizi_Int_Kpub(byte* contentBuffer, word* contentLength);
  bool     read_EF_SOD(byte* contentBuffer, word* contentLength);
  void     printHex(byte* buffer, word length);

 private:
  Adafruit_PN532 _nfc;
  bool     readElementaryFile(const byte df, const byte efid[], byte* contentBuffer, word* contentLength, const byte lengthStrategy);
  bool     selectIasApplication(void);
  bool     selectRootMasterFile(void);
  bool     selectCieDedicatedFile(void);
  bool     selectElementaryFile(const byte df, const byte efid[]);
  bool     determineLength(word* contentLength, const byte lengthStrategy);
  bool     autodetectBerLength(word* contentLength);
  bool     fetchElementaryFileContent(byte* contentBuffer, const word contentLength);
  bool     hasSuccessStatusWord(byte* response, const word responseLength);
  void     clamp(word* value, byte maxValue);
};
