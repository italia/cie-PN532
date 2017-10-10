/**************************************************************************/
/*!
    @file     cie_PN532.cpp
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
#include <cie_PN532.h>

#define PN532DEBUGPRINT Serial

cie_PN532::cie_PN532 (uint8_t clk, uint8_t miso, uint8_t mosi, uint8_t ss) :
_nfc(clk, miso, mosi, ss) 
{
}


/**************************************************************************/
/*!
    @brief  Setups the HW
*/
/**************************************************************************/
void cie_PN532::begin() {
  _nfc.begin();
  uint32_t versiondata = _nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  // Got ok data, print it out!
  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
  _nfc.SAMConfig();

  Serial.println("Waiting for a CIE card...");
}


/**************************************************************************/
/*!
    @brief  Dumps all values present in the CIE. Call this in your loop
*/
/**************************************************************************/
bool cie_PN532::detectCard() {
  return _nfc.inListPassiveTarget();
}


/**************************************************************************/
/*!
    @brief  Dumps all values present in the CIE
*/
/**************************************************************************/
void cie_PN532::dumpAll() {
  PN532DEBUGPRINT.println("*** DUMPING ALL VALUES ***");
}

/**************************************************************************/
/*!
    @brief  Dumps all values present in the CIE
*/
/**************************************************************************/
void cie_PN532::printHex(uint8_t* buffer, uint8_t length) {
  _nfc.PrintHex(buffer, length);
}

/**************************************************************************/
/*!
    @brief  Reads the binary content the EF_DH elementary file

	@param  contentBuffer The pointer to data containing the contents of the file
	@param  contentLength The lenght of the file
	
	@returns  The file content
*/
/**************************************************************************/
bool cie_PN532::read_EF_DH(uint8_t* contentBuffer, uint8_t* contentLength) {
}


/**************************************************************************/
/*!
    @brief  Reads the binary content of the EF_ATR elementary file

	@param  contentBuffer The pointer to data containing the contents of the file
	@param  contentLength The lenght of the file

    @returns  The file content
*/
/**************************************************************************/
bool cie_PN532::read_EF_ATR(uint8_t* contentBuffer, uint8_t* contentLength) {
}


/**************************************************************************/
/*!
    @brief  Reads the binary content of the SN_ICC elementary file

	@param  contentBuffer The pointer to data containing the contents of the file
	@param  contentLength The lenght of the file

    @returns  The file content
*/
/**************************************************************************/
bool cie_PN532::read_SN_ICC(uint8_t* contentBuffer, uint8_t* contentLength) {
}


/**************************************************************************/
/*!
    @brief  Reads the binary content of the EF_ID_Servizi elementary file

	@param  contentBuffer The pointer to data containing the contents of the file
	@param  contentLength The lenght of the file

    @returns  The file content
*/
/**************************************************************************/
bool cie_PN532::read_EF_ID_Servizi(uint8_t* contentBuffer, uint8_t* contentLength) {

  if (!select_IAS_Application() || !select_CIE_DF()) {
    return false;
  }
  uint8_t responseLength = EF_ID_SERVIZI_LENGTH;
  uint8_t response[responseLength];
  bool success = read_EF(0x01, response, &responseLength);	
  
  if (!success || !hasSuccessStatusWord(response, responseLength)) {
    *contentLength = 0;
    return false;
  }
   
  *contentLength = responseLength-2;
  for (uint8_t i = 0; i < *contentLength; i++) {
    contentBuffer[i] = response[i];
  }
  
  return true;
}


/**************************************************************************/
/*!
    @brief  Reads the binary content of the EF_Int_Kpub elementary file

	@param  contentBuffer The pointer to data containing the contents of the file
	@param  contentLength The lenght of the file

    @returns  The file content
*/
/**************************************************************************/
bool cie_PN532::read_EF_Int_Kpub(uint8_t* contentBuffer, uint8_t* contentLength) {
}


/**************************************************************************/
/*!
    @brief  Reads the binary content of the EF_Servizi_Int_Kpub elementary file

	@param  contentBuffer The pointer to data containing the contents of the file
	@param  contentLength The lenght of the file

    @returns  The file content
*/
/**************************************************************************/
bool cie_PN532::read_EF_Servizi_Int_Kpub(uint8_t* contentBuffer, uint8_t* contentLength) {
}


/**************************************************************************/
/*!
    @brief  Reads the binary content of the EF_SOD elementary file

	@param  contentBuffer The pointer to data containing the contents of the file
	@param  contentLength The lenght of the file
	
    @returns  The file content
*/
/**************************************************************************/
bool cie_PN532::read_EF_SOD(uint8_t* contentBuffer, uint8_t* contentLength) {
}


/**************************************************************************/
/*!
    @brief  Selects the ROOT MF
	
	@returns  A value indicating whether the operation succeeded or not
*/
/**************************************************************************/
bool cie_PN532::select_ROOT(void) {
  return true;
}


/**************************************************************************/
/*!
    @brief  Selects the IAS Application
	
	@returns  A value indicating whether the operation succeeded or not
*/
/**************************************************************************/
bool cie_PN532::select_IAS_Application(void) {
  uint8_t command[] = { 0x00, 0xA4, 0x04, 0x0C, 0x0D, 0xA0, 0x00, 0x00, 0x00, 0x30, 0x80, 0x00, 0x00, 0x00, 0x09, 0x81, 0x60, 0x01 };
  uint8_t responseLength = 2;
  uint8_t response[responseLength];
  bool commandResult = _nfc.inDataExchange(command, sizeof(command), response, &responseLength);
  bool result = commandResult && hasSuccessStatusWord(response, responseLength);
  if (!result) {
    PN532DEBUGPRINT.println("Couldn't select the IAS application");
  }
  return result;
}


/**************************************************************************/
/*!
    @brief  Selects the CIE DF
	
	@returns  A value indicating whether the operation succeeded or not
*/
/**************************************************************************/
bool cie_PN532::select_CIE_DF(void) {
  uint8_t command[] = { 0x00, 0xA4, 0x04, 0x0C, 0x06, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x39 };
  uint8_t response[2];
  uint8_t contentLength = 2;
  bool commandResult = _nfc.inDataExchange(command, sizeof(command), response, &contentLength);
  bool result = commandResult && hasSuccessStatusWord(response, contentLength);
  if (!result) {
    PN532DEBUGPRINT.println("Couldn't select the CIE DF");
  }
  return result;
}


/**************************************************************************/
/*!
    @brief  Reads the EF contents by its SFI under the current DF
	
	@param  sfi The short file id
	@param  contentBuffer Pointer to the response data
	@param  response The maximum desired length of the response
	
    @returns  A value indicating whether the operation succeeded or not
*/
/**************************************************************************/
bool cie_PN532::read_EF(uint8_t sfi, uint8_t* contentBuffer, uint8_t* contentLength) {

  uint8_t command[] = { 0x00, 0xB0, 0x81, 0x00, 0x0c };
  bool commandResult = _nfc.inDataExchange(command, sizeof(command), contentBuffer, contentLength);

  bool success = commandResult && hasSuccessStatusWord(contentBuffer, *contentLength);
  if (!success) {
    PN532DEBUGPRINT.println("Couldn't select the file");
  }
  return success;
}

/**************************************************************************/
/*!
    @brief  Reads the EF contents by its SFI under the current DF
	
	@param  response Pointer to the response data
	
    @returns  A value indicating whether the operation succeeded or not
*/
/**************************************************************************/
bool cie_PN532::hasSuccessStatusWord(uint8_t* response, uint8_t responseLength) {

  uint8_t firstByte = response[responseLength-2];
  uint8_t secondByte = response[responseLength-1];
  bool success = firstByte == 0x90 && secondByte == 0x00;
  if (!success) {
    PN532DEBUGPRINT.print("Error ");
	_nfc.PrintHex(response, responseLength);
  }
  return success;
}