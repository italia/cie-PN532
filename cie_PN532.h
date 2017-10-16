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
#ifndef CIE_PN532
#define CIE_PN532

class cie_AtrReader;
class cie_BerReader;

#include <Adafruit_PN532.h>
#include "cie_EFPath.h"
#include "cie_AtrReader.h"
#include "cie_BerReader.h"

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

//Read lengths
#define PAGE_LENGTH                           (0x34)
#define READ_FROM_START                       (0x00)

//Elementary File length detection modes
#define FIXED_LENGTH                          (0x00)
#define AUTODETECT_BER_LENGTH                 (0x01)
#define AUTODETECT_ATR_LENGTH                 (0x02)

//Selection modes
#define SELECT_BY_EFID                        (0x01)
#define SELECT_BY_SFI                         (0x02)

//Paths
#define NULL_DF                               (0x00)
#define NULL_EF                               (0x00)
#define ROOT_MF                               (0x01)
#define CIE_DF                                (0x02)

class cie_PN532
{
 public:
  cie_PN532(byte clk, byte miso, byte mosi, byte ss);
  cie_PN532(Adafruit_PN532 nfc);
  void begin(void);
  bool detectCard();
  
  // Read binary content of unencrypted Elementary Files
  bool     read_EF_DH(byte* contentBuffer, word* contentLength);
  bool     read_EF_ATR(byte* contentBuffer, word* contentLength);
  bool     read_EF_SN_ICC(byte* contentBuffer, word* contentLength);
  bool     read_EF_ID_Servizi(byte* contentBuffer, word* contentLength);
  bool     read_EF_Int_Kpub(byte* contentBuffer, word* contentLength);
  bool     read_EF_Servizi_Int_Kpub(byte* contentBuffer, word* contentLength);

  // File access
  bool     readElementaryFile(const cie_EFPath filePath, byte* contentBuffer, word* contentLength, const byte lengthStrategy);
  bool     readBinaryContent(const cie_EFPath filePath, byte* contentBuffer, word offset, const word contentLength);

  // Utility
  void     printHex(byte* buffer, word length);
  bool     print_EF_SOD(word* contentLength);

 private:
  Adafruit_PN532 _nfc;
  cie_BerReader* _berReader;
  cie_AtrReader* _atrReader;
  byte     _currentDedicatedFile;
  word     _currentElementaryFile;
  bool     ensureSelected(const cie_EFPath filePath);
  bool     ensureDedicatedFileIsSelected(const byte df);
  bool     ensureElementaryFileIsSelected(const cie_EFPath filePath);
  bool     selectIasApplication(void);
  bool     selectRootMasterFile(void);
  bool     selectCieDedicatedFile(void);
  bool     determineLength(const cie_EFPath filePath, word* contentLength, const byte lengthStrategy);
  bool     hasSuccessStatusWord(byte* response, const word responseLength);
  word     clamp(const word value, const byte maxValue);
};

#endif

