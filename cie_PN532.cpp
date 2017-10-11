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
  if (!select_IAS_Application() || !select_ROOT_DF()) {
    return false;
  }
  clamp(contentLength, EF_DH_LENGTH);
  uint8_t efid[] = { 0xD0, 0x04 };
  bool success = read_EF(efid, contentBuffer, *contentLength);
  if (!success) {
    *contentLength = 0;
    return false;
  }
  return true;
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
  if (!select_IAS_Application() || !select_ROOT_DF()) {
    return false;
  }
  clamp(contentLength, EF_ATR_LENGTH);
  uint8_t efid[] = { 0x2F, 0x01 };
  bool success = read_EF(efid, contentBuffer, *contentLength);
  if (!success) {
    *contentLength = 0;
    return false;
  }
  return true;
}


/**************************************************************************/
/*!
  @brief  Reads the binary content of the SN_ICC elementary file

  @param  contentBuffer The pointer to data containing the contents of the file
  @param  contentLength The lenght of the file

  @returns  The file content
*/
/**************************************************************************/
bool cie_PN532::read_EF_SN_ICC(uint8_t* contentBuffer, uint8_t* contentLength) {
  if (!select_IAS_Application() || !select_ROOT_DF()) {
    return false;
  }
  clamp(contentLength, EF_SN_ICC_LENGTH);
  uint8_t efid[] = { 0xD0, 0x03 };
  bool success = read_EF(efid, contentBuffer, *contentLength);
  if (!success) {
    *contentLength = 0;
    return false;
  }
  return true;
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
  clamp(contentLength, EF_ID_SERVIZI_LENGTH);
  uint8_t efid[] = { 0x10, 0x01 };
  bool success = read_EF(efid, contentBuffer, *contentLength);
  if (!success) {
    *contentLength = 0;
    return false;
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
  if (!select_IAS_Application() || !select_CIE_DF()) {
    return false;
  }
  clamp(contentLength, EF_INT_KPUB_LENGTH);
  uint8_t efid[] = { 0x10, 0x04 };
  bool success = read_EF(efid, contentBuffer, *contentLength);
  if (!success) {
    *contentLength = 0;
    return false;
  }
  return true;
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
  if (!select_IAS_Application() || !select_CIE_DF()) {
    return false;
  }
  clamp(contentLength, EF_SERVIZI_INT_KPUB_LENGTH);
  uint8_t efid[] = { 0x10, 0x05 };
  bool success = read_EF(efid, contentBuffer, *contentLength);
  if (!success) {
    *contentLength = 0;
    return false;
  }
  return true;
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
  if (!select_IAS_Application() || !select_CIE_DF()) {
    return false;
  }
  clamp(contentLength, EF_SOD_LENGTH);
  uint8_t efid[] = { 0x10, 0x06 };
  bool success = read_EF(efid, contentBuffer, *contentLength);
  if (!success) {
    *contentLength = 0;
    return false;
  }
  return true;
}


/**************************************************************************/
/*!
  @brief  Selects the ROOT MF
	
  @returns  A value indicating whether the operation succeeded or not
*/
/**************************************************************************/
bool cie_PN532::select_ROOT_DF(void) {
    uint8_t command[] = { 
        0x00, //CLA
        0xA4, //INS: SELECT FILE
        0x00, //P1: Select root
        0x0C, //P2: No data in response field
        0x02, //Lc: Length of root id
        0x3F, 0x00 //root id
    };
    uint8_t responseLength = 2;
    uint8_t responseBuffer[responseLength];
    bool success = _nfc.inDataExchange(command, sizeof(command), responseBuffer, &responseLength);
    success = success && hasSuccessStatusWord(responseBuffer, responseLength);
    if (!success) {
      PN532DEBUGPRINT.println("Couldn't select root");
    }
    return success;
}


/**************************************************************************/
/*!
  @brief  Selects the IAS Application
	
  @returns  A value indicating whether the operation succeeded or not
*/
/**************************************************************************/
bool cie_PN532::select_IAS_Application(void) {
  uint8_t command[] = { 
      0x00, //CLA
      0xA4, //INS: SELECT FILE
      0x04, //P1: Select by AID
      0x0C, //P2: No data in response field
      0x0D, //Lc: Length of AID
      0xA0, 0x00, 0x00, 0x00, 0x30, 0x80, 0x00, 0x00, 0x00, 0x09, 0x81, 0x60, 0x01 //AID
  };
  uint8_t responseLength = 2;
  uint8_t responseBuffer[responseLength];
  bool success = _nfc.inDataExchange(command, sizeof(command), responseBuffer, &responseLength);
  success = success && hasSuccessStatusWord(responseBuffer, responseLength);
  if (!success) {
    PN532DEBUGPRINT.println("Couldn't select the IAS application");
  }
  return success;
}


/**************************************************************************/
/*!
  @brief  Selects the CIE DF
	
  @returns  A value indicating whether the operation succeeded or not
*/
/**************************************************************************/
bool cie_PN532::select_CIE_DF(void) {
  uint8_t command[] = { 
      0x00, //CLA
      0xA4, //INS: SELECT FILE
      0x04, //P1: Select by AID
      0x0C, //P2: No data in response field
      0x06, //Lc: length of AID
      0xA0, 0x00, 0x00, 0x00, 0x00, 0x39 //AID
  };
  uint8_t responseLength = 2;
  uint8_t responseBuffer[responseLength];
  bool success = _nfc.inDataExchange(command, sizeof(command), responseBuffer, &responseLength);
  success = success && hasSuccessStatusWord(responseBuffer, responseLength);
  if (!success) {
    PN532DEBUGPRINT.println("Couldn't select the CIE DF");
  }
  return success;
}


/**************************************************************************/
/*!
  @brief  Reads the binary content of the EF by its SFI under the current DF
	
	@param  sfi The short file id
	@param  contentBuffer Pointer to the response data
	@param  response The maximum desired length of the response
	
  @returns  A value indicating whether the operation succeeded or not
*/
/**************************************************************************/
bool cie_PN532::read_EF(uint8_t efid[], uint8_t* contentBuffer, uint8_t contentLength) {
  uint8_t selectCommand[] = {
      0x00, //CLA
      0xA4, //INS: SELECT FILE
      0x02, //P1: Select by EFID
      0x0C, //P2: No data in response
      0x02, //Lc: Length of EFID
      efid[0], efid[1] //EFID
  };
  uint8_t selectResponseLength = 2;
  uint8_t selectResponseBuffer[selectResponseLength];
  bool selectCommandResult = _nfc.inDataExchange(selectCommand, sizeof(selectCommand), selectResponseBuffer, &selectResponseLength);
  selectCommandResult = selectCommandResult && hasSuccessStatusWord(selectResponseBuffer, selectResponseLength);
  if (!selectCommandResult) {
    PN532DEBUGPRINT.println("Couldn't select the EF by its ID");
    return false;
  }

  uint8_t readCommand[] = {
      0x00, //CLA
      0xB0, //INS: READ BINARY
      0x00, //P1: Read currently selected file
      0x00, //P2: Offset 0
      contentLength //Le: content length
  };
  uint8_t responseLength = contentLength + 2;
  uint8_t responseBuffer[responseLength];
  bool success = _nfc.inDataExchange(readCommand, sizeof(readCommand), responseBuffer, &responseLength);
  success = success && hasSuccessStatusWord(responseBuffer, responseLength);
  if (!success) {
    PN532DEBUGPRINT.println("Couldn't read the file");
  }
  for (int8_t i = 0; i < contentLength; i++) {
      contentBuffer[i] = responseBuffer[i];
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
  uint8_t msByte = response[responseLength-2];
  uint8_t lsByte = response[responseLength-1];
  bool success = msByte == 0x90 && lsByte == 0x00;
  if (success)
    return true;

  if (msByte == 0x62 && msByte == 0x83) {
    PN532DEBUGPRINT.print("Warning selected file deactivated");
  } else if (msByte == 0x62 && msByte == 0x85) {
    PN532DEBUGPRINT.print("The selected file is in terminate state");
  } else if (msByte == 0x6A && msByte == 0x82) {
    PN532DEBUGPRINT.print("File or application not found");
  } else if (msByte == 0x6A && msByte == 0x86) {
    PN532DEBUGPRINT.print("Incorrect parameters P1-P2");
  } else if (msByte == 0x6A && msByte == 0x87) {
    PN532DEBUGPRINT.print("Nc inconsistent with parameters P1-P2");
  } else {
    PN532DEBUGPRINT.print("Unknow error");
  }
  PN532DEBUGPRINT.print(" ");
  //uint8_t statusWord[2] = { msByte, lsByte };
  //_nfc.PrintHex(statusWord, 2);
  
  return false;
}


/**************************************************************************/
/*!
    @brief  Ensures a value is less or equal than a max value
	
    @param  value The value that should be checked
    @param  maxValue The maximum value

*/
/**************************************************************************/
void cie_PN532::clamp(uint8_t* value, const uint8_t maxValue) {
  if (*value > maxValue) {
    *value = maxValue;
  }
}