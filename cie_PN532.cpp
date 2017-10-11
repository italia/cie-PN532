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

cie_PN532::cie_PN532 (byte clk, byte miso, byte mosi, byte ss) :
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
    Serial.print(F("Didn't find PN53x board"));
    while (1); // halt
  }
  // Got ok data, print it out!
  Serial.print(F("Found chip PN5")); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print(F("Firmware ver. ")); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
  _nfc.SAMConfig();

  Serial.println(F("Waiting for a CIE card..."));
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
void cie_PN532::printHex(byte* buffer, word length) {
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
bool cie_PN532::read_EF_DH(byte* contentBuffer, word* contentLength) {
  byte efid[] = { 0xD0, 0x04 };
  return readElementaryFile(ROOT_MF, efid, contentBuffer, contentLength, AUTODETECT_BER_LENGTH);
}


/**************************************************************************/
/*!
  @brief  Reads the binary content of the EF_ATR elementary file

  @param  contentBuffer The pointer to data containing the contents of the file
  @param  contentLength The lenght of the file

  @returns  The file content
*/
/**************************************************************************/
bool cie_PN532::read_EF_ATR(byte* contentBuffer, word* contentLength) {
  byte efid[] = { 0x2F, 0x01 };
  return readElementaryFile(ROOT_MF, efid, contentBuffer, contentLength, AUTODETECT_BER_LENGTH);
}


/**************************************************************************/
/*!
  @brief  Reads the binary content of the SN_ICC elementary file

  @param  contentBuffer The pointer to data containing the contents of the file
  @param  contentLength The lenght of the file

  @returns  The file content
*/
/**************************************************************************/
bool cie_PN532::read_EF_SN_ICC(byte* contentBuffer, word* contentLength) {
  byte efid[] = { 0xD0, 0x03 };
  clamp(contentLength, EF_SN_ICC_LENGTH);
  return readElementaryFile(ROOT_MF, efid, contentBuffer, contentLength, FIXED_LENGTH);
}


/**************************************************************************/
/*!
  @brief  Reads the binary content of the EF_ID_Servizi elementary file

  @param  contentBuffer The pointer to data containing the contents of the file
  @param  contentLength The lenght of the file

  @returns  The file content
*/
/**************************************************************************/
bool cie_PN532::read_EF_ID_Servizi(byte* contentBuffer, word* contentLength) {
  byte efid[] = { 0x10, 0x01 };
  clamp(contentLength, EF_ID_SERVIZI_LENGTH);
  return readElementaryFile(CIE_DF, efid, contentBuffer, contentLength, FIXED_LENGTH);
}


/**************************************************************************/
/*!
  @brief  Reads the binary content of the EF_Int_Kpub elementary file

  @param  contentBuffer The pointer to data containing the contents of the file
  @param  contentLength The lenght of the file

  @returns  The file content
*/
/**************************************************************************/
bool cie_PN532::read_EF_Int_Kpub(byte* contentBuffer, word* contentLength) {
  byte efid[] = { 0x10, 0x04 };
  return readElementaryFile(CIE_DF, efid, contentBuffer, contentLength, AUTODETECT_BER_LENGTH);
}


/**************************************************************************/
/*!
  @brief  Reads the binary content of the EF_Servizi_Int_Kpub elementary file

  @param  contentBuffer The pointer to data containing the contents of the file
  @param  contentLength The lenght of the file

  @returns  The file content
*/
/**************************************************************************/
bool cie_PN532::read_EF_Servizi_Int_Kpub(byte* contentBuffer, word* contentLength) {
  byte efid[] = { 0x10, 0x05 };
  return readElementaryFile(CIE_DF, efid, contentBuffer, contentLength, AUTODETECT_BER_LENGTH);
}


/**************************************************************************/
/*!
  @brief  Reads the binary content of the EF_SOD elementary file

  @param  contentBuffer The pointer to data containing the contents of the file
  @param  contentLength The lenght of the file
	
  @returns  The file content
*/
/**************************************************************************/
bool cie_PN532::read_EF_SOD(byte* contentBuffer, word* contentLength) {
  byte efid[] = { 0x10, 0x06 };
  return readElementaryFile(CIE_DF, efid, contentBuffer, contentLength, AUTODETECT_BER_LENGTH);
}


/**************************************************************************/
/*!
  @brief  Reads the binary content of the EF by its SFI under the current DF
	
  @param df The containing Dedicated File (either ROOT_MF or CIE_DF)
  @param efid The efid of the Elementary File (must be two bytes)
	@param contentBuffer Pointer to the response data
  @param response The maximum desired length of the response
  @param lengthStrategy How to determine the length of the file (either FIXED_LENGTH or AUTODETECT_BER_LENGTH)
	
  @returns  A value indicating whether the operation succeeded or not
*/
/**************************************************************************/
bool cie_PN532::readElementaryFile(const byte df, const byte efid[], byte* contentBuffer, word* contentLength, const byte lengthStrategy) {

  if (!selectElementaryFile(df, efid) || 
      !determineLength(contentLength, lengthStrategy) ||
      !fetchElementaryFileContent(contentBuffer, *contentLength)) {
    return false;
  }

  return true;
}


/**************************************************************************/
/*!
  @brief  Selects an Elementary file
  
  @param df The containing Dedicated File (either ROOT_MF or CIE_DF)
  @param efid The efid of the Elementary File (must be two bytes)
  
  @returns  A value indicating whether the operation succeeded or not
*/
/**************************************************************************/
bool cie_PN532::selectElementaryFile(const byte df, const byte efid[]) {
  if (!selectIasApplication()) {
    return false;
  }
  
  switch (df) {
    case ROOT_MF:
    if (!selectRootMasterFile()) {
      return false;
    }
    break;

    case CIE_DF:
    if (!selectCieDedicatedFile()) {
      return false;
    }
    break;

    default:
      PN532DEBUGPRINT.println(F("The DF must be either ROOT_MF or CIE_DF"));
      return false;
  }

  byte selectCommand[] = {
    0x00, //CLA
    0xA4, //INS: SELECT FILE
    0x02, //P1: Select by EFID
    0x0C, //P2: No data in response
    0x02, //Lc: Length of EFID
    efid[0], efid[1] //EFID
  };
  byte selectResponseLength = 2;
  byte* selectResponseBuffer = new byte[selectResponseLength];
  bool success = _nfc.inDataExchange(selectCommand, sizeof(selectCommand), selectResponseBuffer, &selectResponseLength);
  success = success && hasSuccessStatusWord(selectResponseBuffer, selectResponseLength);
  delete [] selectResponseBuffer;
  if (!success) {
    PN532DEBUGPRINT.print(F("Couldn't select the EF by its EFID "));
    _nfc.PrintHex(efid, 2);
    return false;
  }
  return true;
}


/**************************************************************************/
/*!
  @brief  Sets the length value of a currently selected Elementary File
	
  @param contentLength The pointer to the length value
  @param lengthStrategy The chosen strategy by user (either AUTODETECT_BER_LENGTH or FIXED_LENGTH)

  @returns  A value indicating whether the operation succeeded or not
*/
/**************************************************************************/
bool cie_PN532::determineLength(word* contentLength, const byte lengthStrategy) {
  switch (lengthStrategy) {
    case FIXED_LENGTH:
    //do nothing, size is already known
    break;

    case AUTODETECT_BER_LENGTH:
      autodetectBerLength(contentLength);
    break;

    default:
      PN532DEBUGPRINT.println(F("The length strategy must be either AUTODETECT_BER_LENGTH or FIXED_LENGTH"));
      return false;
  }
  return true;
}


/**************************************************************************/
/*!
  @brief  Detects the length of an Elementary File by reading its BET encoded content
	
  @param contentLength The pointer to the length value

  @returns  A value indicating whether the operation succeeded or not
*/
/**************************************************************************/
bool cie_PN532::autodetectBerLength(word* contentLength) {
  //TODO: unit test this
  //Please refer to https://en.wikipedia.org/wiki/X.690#Identifier_octets
  const byte berHeaderLength = 6; //6 will do for the CIE
  byte buffer[berHeaderLength];
  if (!fetchElementaryFileContent(buffer, berHeaderLength)) {
    return false;
  }
  
  byte lengthOctectsOffset = 0;
  if ((buffer[0] & 0x1F) == 0x1F)
  {
    //Tag is defined in the next octects
    do
    {
      lengthOctectsOffset++;
      //we keep going until we find an octet with a 0 in the bit 8
      //that will be the last tag octect
    } while ((buffer[lengthOctectsOffset] & 0x80) == 0x80);
    //and then we move forward to the first length octet
    lengthOctectsOffset++;
  } else
  {
    //tag was made of a single octet
    lengthOctectsOffset = 1;
  }
  
  //Now we start reading the length octects
  if (buffer[lengthOctectsOffset] == 0x80)
  {
      //Indefinite mode, we don't currently support this
      PN532DEBUGPRINT.println(F("Indefinite length for BER encoded file not supported"));
      return false;
  }
  else if ((buffer[lengthOctectsOffset] & 0x80) == 0x80)
  {
      //Definite, long
      byte lengthOctets = buffer[lengthOctectsOffset] & (byte) 0x7F;
      word length = 0;
      for (byte i = 1; i <= lengthOctets; i++)
      {
          length += buffer[lengthOctectsOffset + i] << ((lengthOctets - i) * 8);
      }
      length += lengthOctectsOffset + lengthOctets + 1;
      *contentLength = length;
  } else  {
      //Definite, short
      word length = buffer[lengthOctectsOffset];
      length += lengthOctectsOffset + 1;
      *contentLength = length;
  }
  
  return true;
}


/**************************************************************************/
/*!
  @brief Reads the binary content of the currently selected Elementary File
  
  @param contentBuffer The pointer to the data buffer
  @param contentLength The number of bytes to read

  @returns  A value indicating whether the operation succeeded or not
*/
/**************************************************************************/
bool cie_PN532::fetchElementaryFileContent(byte* contentBuffer, const word contentLength) {
  word offset = 0x0;
  bool success = false;
  byte contentPageLength = 0;
  do {
    byte contentPageLength = contentLength-offset > PAGE_LENGTH ? PAGE_LENGTH : (byte) contentLength;
    byte readCommand[] = {
        0x00, //CLA
        0xB0, //INS: READ BINARY
        (byte) (offset >> 8), //P1: Read currently selected file
        (byte) (offset & 0xFF), //P2: Current offset
        contentPageLength //Le: content length
    };

    byte responseLength = contentPageLength + 2;
    byte* responseBuffer = new byte[responseLength];
    success = _nfc.inDataExchange(readCommand, sizeof(readCommand), responseBuffer, &responseLength);
    success = success && hasSuccessStatusWord(responseBuffer, responseLength);

    //Copy data over to the buffer
    for (word i = 0; i < contentPageLength; i++) {
      contentBuffer[offset+i] = responseBuffer[i];
    }
    delete [] responseBuffer;

    offset += contentPageLength;
    
  } while(success && (offset < contentLength));

  if (!success) {
    PN532DEBUGPRINT.println(F("Couldn't fetch the elementary file content"));
  }
  return success;
}


/**************************************************************************/
/*!
  @brief  Selects the ROOT Master File
	
  @returns  A value indicating whether the operation succeeded or not
*/
/**************************************************************************/
bool cie_PN532::selectRootMasterFile(void) {
    byte command[] = { 
        0x00, //CLA
        0xA4, //INS: SELECT FILE
        0x00, //P1: Select root
        0x0C, //P2: No data in response field
        0x02, //Lc: Length of root id
        0x3F, 0x00 //root id
    };
    byte responseLength = 2;
    byte* responseBuffer = new byte[responseLength];
    bool success = _nfc.inDataExchange(command, sizeof(command), responseBuffer, &responseLength);
    success = success && hasSuccessStatusWord(responseBuffer, responseLength);
    delete [] responseBuffer;
    if (!success) {
      PN532DEBUGPRINT.println(F("Couldn't select root"));
    }
    return success;
}


/**************************************************************************/
/*!
  @brief  Selects the IAS Application
	
  @returns  A value indicating whether the operation succeeded or not
*/
/**************************************************************************/
bool cie_PN532::selectIasApplication(void) {
  byte command[] = { 
      0x00, //CLA
      0xA4, //INS: SELECT FILE
      0x04, //P1: Select by AID
      0x0C, //P2: No data in response field
      0x0D, //Lc: Length of AID
      0xA0, 0x00, 0x00, 0x00, 0x30, 0x80, 0x00, 0x00, 0x00, 0x09, 0x81, 0x60, 0x01 //AID
  };
  byte responseLength = 2;
  byte* responseBuffer = new byte[responseLength];
  bool success = _nfc.inDataExchange(command, sizeof(command), responseBuffer, &responseLength);
  success = success && hasSuccessStatusWord(responseBuffer, responseLength);
  delete [] responseBuffer;
  if (!success) {
    PN532DEBUGPRINT.println(F("Couldn't select the IAS application"));
  }
  return success;
}


/**************************************************************************/
/*!
  @brief  Selects the CIE DF
	
  @returns  A value indicating whether the operation succeeded or not
*/
/**************************************************************************/
bool cie_PN532::selectCieDedicatedFile(void) {
  byte command[] = { 
      0x00, //CLA
      0xA4, //INS: SELECT FILE
      0x04, //P1: Select by AID
      0x0C, //P2: No data in response field
      0x06, //Lc: length of AID
      0xA0, 0x00, 0x00, 0x00, 0x00, 0x39 //AID
  };
  byte responseLength = 2;
  byte* responseBuffer = new byte[responseLength];
  bool success = _nfc.inDataExchange(command, sizeof(command), responseBuffer, &responseLength);
  success = success && hasSuccessStatusWord(responseBuffer, responseLength);
  delete [] responseBuffer;
  if (!success) {
    PN532DEBUGPRINT.println(F("Couldn't select the CIE DF"));
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
bool cie_PN532::hasSuccessStatusWord(byte* response, const word responseLength) {
  byte msByte = response[responseLength-2];
  byte lsByte = response[responseLength-1];
  bool success = msByte == 0x90 && lsByte == 0x00;
  if (success) {
    return true;
  }

  if (msByte == 0x62 && lsByte == 0x83) {
    PN532DEBUGPRINT.print(F("Warning selected file deactivated"));
  } else if (msByte == 0x62 && lsByte == 0x85) {
    PN532DEBUGPRINT.print(F("The selected file is in terminate state"));
  } else if (msByte == 0x62 && lsByte == 0x82) {
    PN532DEBUGPRINT.print(F("End of file/record reached before reading Le bytes"));
  } else if (msByte == 0x6A && lsByte == 0x82) {
    PN532DEBUGPRINT.print(F("File or application not found"));
  } else if (msByte == 0x6A && lsByte == 0x86) {
    PN532DEBUGPRINT.print(F("Incorrect parameters P1-P2"));
  } else if (msByte == 0x6A && lsByte == 0x87) {
    PN532DEBUGPRINT.print(F("Nc inconsistent with parameters P1-P2"));
  } else {
    PN532DEBUGPRINT.print(F("Unknow error"));
  }
  PN532DEBUGPRINT.print(F(" "));
  byte statusWord[2] = { msByte, lsByte };
  _nfc.PrintHex(statusWord, 2);
  
  return false;
}


/**************************************************************************/
/*!
    @brief  Ensures a value is less or equal than a max value
	
    @param  value The value that should be checked
    @param  maxValue The maximum value

*/
/**************************************************************************/
void cie_PN532::clamp(word* value, const byte maxValue) {
  if (*value > maxValue) {
    *value = maxValue;
  }
}