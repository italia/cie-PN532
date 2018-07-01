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


class cie_BerTriple;
class cie_BerReader;
class cie_AtrReader;

#include "cie_EFPath.h"
#include "cie_AtrReader.h"
#include "cie_BerReader.h"
#include "cie_Key.h"
#include "cie_Nfc_Adafruit.h"

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
#define PAGE_LENGTH                           (0xE4)
#define READ_FROM_START                       (0x00)
#define STATUS_WORD_LENGTH                    (0x02)

//Random values lengths
#define CHALLENGE_LENGTH                      (0x08)
#define K_LENGTH                              (0x20)
#define SK_LENGTH                             (0x10)

//Elementary File length detection modes
#define FIXED_LENGTH                          (0x00)
#define AUTODETECT_BER_LENGTH                 (0x01)
#define AUTODETECT_ATR_LENGTH                 (0x02)

//Selection modes
#define SELECT_BY_EFID                        (0x01)
#define SELECT_BY_SFI                         (0x02)
#define SELECT_BY_SDOID                       (0x03)

//Paths
#define NULL_DF                               (0x00)
#define NULL_EF                               (0x00)
#define ROOT_MF                               (0x01)
#define CIE_DF                                (0x02)

//Session keys
#define SK_ENC                                (0x01)
#define SK_MAC                                (0x02)

class cie_PN532
{
 public:
  cie_PN532();
  cie_PN532(byte clk, byte miso, byte mosi, byte ss);
  cie_PN532(cie_Nfc *nfc);
  ~cie_PN532();
  bool verbose;

  //PN532 data exchange methods
  virtual void begin(void);
  virtual bool detectCard();

  
  // Read binary content of unencrypted Elementary Files
  bool     read_EF_DH(byte *contentBuffer, word *contentLength);
  bool     read_EF_ATR(byte *contentBuffer, word *contentLength);
  bool     read_EF_SN_ICC(byte *contentBuffer, word *contentLength);
  bool     read_EF_ID_Servizi(byte *contentBuffer, word *contentLength);
  bool     read_EF_Int_Kpub(cie_Key *key);
  bool     read_EF_Servizi_Int_Kpub(cie_Key *key);
  bool     isCardValid(); //Call this to verify it's not a clone

  // File access
  bool     readElementaryFile(const cie_EFPath filePath, byte *contentBuffer, word *contentLength, const byte lengthStrategy);
  bool     readBinaryContent(const cie_EFPath filePath, byte *contentBuffer, word offset, const word contentLength);
  bool     readKey(const cie_EFPath filePath, cie_Key *key);

  // Utility
  void     printHex(byte *buffer, const word length);
  bool     print_EF_SOD(word *contentLength);
  bool     parse_EF_SOD(cieBerTripleCallbackFunc callback);

 private:
  //fields
  cie_Nfc *_nfc;
  cie_BerReader *_berReader;
  cie_AtrReader *_atrReader;
  byte _currentDedicatedFile;
  unsigned long _currentElementaryFile;

  //PN532 data exchange methods
  virtual bool sendCommand(byte *command, const byte commandLength, byte *response, word *responseLength);


  //methods
  void initFields();
  bool sendCommand(byte *command, const word commandLength);
  bool select_SDO_Servizi_Int_Kpriv();
  bool ensureSelected(const cie_EFPath filePath);
  bool ensureDedicatedFileIsSelected(const byte df);
  bool ensureElementaryFileIsSelected(const cie_EFPath filePath);
  bool ensureSdoIsSelected(const cie_EFPath filePath);
  bool selectIasApplication(void);
  bool selectRootMasterFile(void);
  bool selectCieDedicatedFile(void);
  bool determineLength(const cie_EFPath filePath, word *contentLength, const byte lengthStrategy);
  bool hasSuccessStatusWord(byte *response, const word responseLength);
  word clamp(const word value, const byte maxValue);
  
  //authentication related methods
  bool establishSecureMessaging();
  bool getChallenge(byte *contentBuffer, word *contentLength);
  bool mutualAuthenticate(byte *snIccBuffer, const byte snIccBufferLength, byte *rndIccBuffer, const byte rndIccBufferLength);
  bool internalAuthenticate(byte *responseBuffer, word *responseLength, byte *challenge, const byte challengeLength);
  bool verifyInternalAuthenticateResponse(cie_Key *pubKey, byte *cypher, const word cypherLength, const byte *message, const word messageLength);
  void calculateSk(const byte valueType, byte *kIfd, byte *kIcc, byte *sk, byte *skLength);
};


#endif

