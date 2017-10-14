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
//#include <EFPath.h>

#define PN532DEBUGPRINT Serial

//Can we chain constructors in this version of C++ to avoid repetitions?
//SomeType() : SomeType(42) {}
cie_PN532::cie_PN532 (byte clk, byte miso, byte mosi, byte ss) :
_nfc(clk, miso, mosi, ss),
_currentDedicatedFile(NULL_DF),
_currentElementaryFile(NULL_EF)
{
}

cie_PN532::cie_PN532 (Adafruit_PN532 nfc) :
_nfc(nfc),
_currentDedicatedFile(NULL_DF),
_currentElementaryFile(NULL_EF)
{
}



/**************************************************************************/
/*!
  @brief  The SDK for the CIE (Italian Electronic ID Card)
*/
/**************************************************************************/
void cie_PN532::begin() {
  _nfc.begin();
  uint32_t versiondata = _nfc.getFirmwareVersion();
  if (! versiondata) {
    PN532DEBUGPRINT.print(F("Didn't find PN53x board"));
    while (1); // halt
  }
  // Got ok data, print it out!
  PN532DEBUGPRINT.print(F("Found chip PN5")); PN532DEBUGPRINT.println((versiondata>>24) & 0xFF, HEX); 
  PN532DEBUGPRINT.print(F("Firmware ver. ")); PN532DEBUGPRINT.print((versiondata>>16) & 0xFF, DEC); 
  PN532DEBUGPRINT.print('.'); PN532DEBUGPRINT.println((versiondata>>8) & 0xFF, DEC);
  _nfc.SAMConfig();

  PN532DEBUGPRINT.println(F("PN53x initialized, waiting for a CIE card..."));
}


/**************************************************************************/
/*!
  @brief  Dumps all values present in the CIE. Call this in your loop
*/
/**************************************************************************/
bool cie_PN532::detectCard() {
  bool success = _nfc.inListPassiveTarget();
  if (success) {
    _currentDedicatedFile = NULL_DF;
    _currentDedicatedFile = NULL_EF;
  }
  return success;
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
  //EFPath filePath = { ROOT_MF, SELECT_BY_SFI, 0x1B }; //efid 0xD004
  EFPath filePath = { ROOT_MF, SELECT_BY_EFID, 0xD004 }; //efid 0xD004
  return readElementaryFile(filePath, contentBuffer, contentLength, AUTODETECT_BER_LENGTH);
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
  EFPath filePath = { ROOT_MF, SELECT_BY_SFI, 0x1D }; //efid 0x2F01
  return readElementaryFile(filePath, contentBuffer, contentLength, AUTODETECT_ATR_LENGTH);
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
  EFPath filePath = { ROOT_MF, SELECT_BY_EFID, 0xD003 }; //What's the sfi for this file?
  *contentLength = clamp(*contentLength, EF_SN_ICC_LENGTH);
  return readElementaryFile(filePath, contentBuffer, contentLength, FIXED_LENGTH);
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
  EFPath filePath = { CIE_DF, SELECT_BY_SFI, 0x01 }; //efid 0x1001
  *contentLength = clamp(*contentLength, EF_ID_SERVIZI_LENGTH);
  return readElementaryFile(filePath, contentBuffer, contentLength, FIXED_LENGTH);
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
  EFPath filePath = { CIE_DF, SELECT_BY_SFI, 0x04 }; //efid 0x1004
  return readElementaryFile(filePath, contentBuffer, contentLength, AUTODETECT_BER_LENGTH);
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
  EFPath filePath = { CIE_DF, SELECT_BY_SFI, 0x05 }; //efid 0x1005
  return readElementaryFile(filePath, contentBuffer, contentLength, AUTODETECT_BER_LENGTH);
}


/**************************************************************************/
/*!
  @brief  Print the binary content of the EF_SOD elementary file

  @param  contentBuffer The pointer to data containing the contents of the file
  @param  contentLength The lenght of the file
	
  @returns  The file content
*/
/**************************************************************************/
bool cie_PN532::print_EF_SOD(word* contentLength) {
  EFPath filePath = { CIE_DF, SELECT_BY_SFI, 0x06 }; //efid 0x1006
  if (!determineLength(filePath, contentLength, AUTODETECT_BER_LENGTH)) {
        return false;
  }

  word offset = READ_FROM_START;
  while (offset < *contentLength) {
    word contentPageLength = clamp(*contentLength-offset, PAGE_LENGTH);
    byte* pageBuffer = new byte[contentPageLength];
    bool success = readBinaryContent(filePath, pageBuffer, offset, contentPageLength);
    if (success) {
      _nfc.PrintHex(pageBuffer, contentPageLength);
    }
    delete [] pageBuffer;

    offset += contentPageLength;
    if (!success) {
      return false;
    }
  }
  return true;
}

/**************************************************************************/
/*!
  @brief  Reads the binary content of an Elementary File given its Dedicated File and EFID or SFI identifier
	
  @param df The containing Dedicated File (either ROOT_MF or CIE_DF)
  @param filePath a structure indicating the parent Dedicated File (either ROOT_MF or CIE_DF), the selection mode (either SELECT_BY_EFID or SELECT_BY_SFI) and the file identifier (either a sfi or an efid)
  @param contentBuffer Pointer to the response data
  @param response The maximum desired length of the response
  @param lengthStrategy How to determine the length of the file (either FIXED_LENGTH or AUTODETECT_BER_LENGTH or AUTODETECT_ATR_LENGTH)
	
  @returns  A value indicating whether the operation succeeded or not
*/
/**************************************************************************/
bool cie_PN532::readElementaryFile(EFPath filePath, byte* contentBuffer, word* contentLength, const byte lengthStrategy) {
  //Some arguments passed around but more testable
  if (!determineLength(filePath, contentLength, lengthStrategy) ||
      !readBinaryContent(filePath, contentBuffer, READ_FROM_START, *contentLength)) {
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
bool cie_PN532::ensureElementaryFileIsSelected(EFPath filePath) {

  if (filePath.selectionMode != SELECT_BY_EFID) {
    PN532DEBUGPRINT.println(F("This method should be called just for EFID selection"));
    return false;
  }

  if (!ensureDedicatedFileIsSelected(filePath.df)) {
    return false;
  }

  //If it's already selected, no need to select it again 
  if (_currentElementaryFile == filePath.id) {
    return true;
  }

  byte efid[2] = { (byte) (filePath.id >> 8), (byte) (filePath.id & 0xFF) };
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
  _currentElementaryFile = filePath.id;
  return true;
}


/**************************************************************************/
/*!
  @brief  Selects a dedicated file in the IAS application
  
  @param df The Dedicated File (either ROOT_MF or CIE_DF)
  
  @returns  A value indicating whether the operation succeeded or not
*/
/**************************************************************************/
bool cie_PN532::ensureDedicatedFileIsSelected (const byte df) {
  //No need to re-select the same Dedicated File if it's already the selected one
  if (_currentDedicatedFile == df) {
    return true;
  } else {
    //We're chaging Dedicated File, the current Elementary File will be deselected to prevent id collision
    _currentElementaryFile = NULL_EF;
  }

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
  _currentDedicatedFile = df;
  return true;
}


/**************************************************************************/
/*!
  @brief  Sets the length value of a currently selected Elementary File
  
  @param filePath a structure indicating the parent Dedicated File (either ROOT_MF or CIE_DF), the selection mode (either SELECT_BY_EFID or SELECT_BY_SFI) and the file identifier (either a sfi or an efid)
  @param contentLength The pointer to the length value
  @param lengthStrategy The chosen strategy by user (either AUTODETECT_BER_LENGTH or FIXED_LENGTH)

  @returns  A value indicating whether the operation succeeded or not
*/
/**************************************************************************/
bool cie_PN532::determineLength(const EFPath filePath, word* contentLength, const byte lengthStrategy) {
  switch (lengthStrategy) {
    case FIXED_LENGTH:
    //do nothing, size is already known
    break;

    case AUTODETECT_BER_LENGTH:
      autodetectBerLength(filePath, contentLength);
    break;

    case AUTODETECT_ATR_LENGTH:
      autodetectAtrLength(filePath, contentLength);
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
  
  @param filePath a structure indicating the parent Dedicated File (either ROOT_MF or CIE_DF), the selection mode (either SELECT_BY_EFID or SELECT_BY_SFI) and the file identifier (either a sfi or an efid)	
  @param contentLength The pointer to the length value

  @returns  A value indicating whether the operation succeeded or not
*/
/**************************************************************************/
bool cie_PN532::autodetectBerLength(const EFPath filePath, word* contentLength) {
  //TODO: unit test this
  //Please refer to https://en.wikipedia.org/wiki/X.690#Identifier_octets
  const byte berHeaderLength = 6; //6 will do for the CIE
  byte buffer[berHeaderLength];
  if (!readBinaryContent(filePath, buffer, READ_FROM_START, berHeaderLength)) {
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
  @brief  Detects the length of the EF.ATR Elementary File

  @param filePath a structure indicating the parent Dedicated File (either ROOT_MF or CIE_DF), the selection mode (either SELECT_BY_EFID or SELECT_BY_SFI) and the file identifier (either a sfi or an efid)
  @param contentLength The pointer to the length value

  @returns  A value indicating whether the operation succeeded or not
*/
/**************************************************************************/
bool cie_PN532::autodetectAtrLength(const EFPath filePath, word* contentLength) {
  //TODO: unit test this
  //The EF.ATR record has a minimum of 33 bytes
  //Please refer to EF.ATR content here http://www.unsads.com/specs/IASECC/IAS_ECC_v1.0.1_UK.pdf#page=19
  const byte chunkSize = 4;
  byte endingSequence[chunkSize] = { 0x82, 0x02, 0x90, 0x00 };
  byte buffer[chunkSize];
  word offset = 0x21;
  while (true) {

    if (!readBinaryContent(filePath, buffer, offset, chunkSize)) {
      *contentLength = 0;
      return false;
    }
    byte matchingOctets = 0;
    for (byte i = 0; i < chunkSize; i++) {
      if (buffer[i] == endingSequence[matchingOctets]) {
        matchingOctets++;
      } else {
        matchingOctets = 0;
      }
    }
    if (matchingOctets == chunkSize) {
       //Match found for all octects in the sequence! We're at the end of the file.
       *contentLength = offset + chunkSize;
       return true;
    } else {
      //let's position ourself in a place where we can read all of the matching octects
      offset += chunkSize - matchingOctets;
    }
  }
}


/**************************************************************************/
/*!
  @brief Reads the binary content of the currently selected Elementary File

  @param filePath a structure indicating the parent Dedicated File (either ROOT_MF or CIE_DF), the selection mode (either SELECT_BY_EFID or SELECT_BY_SFI) and the file identifier (either a sfi or an efid)  
  @param contentBuffer The pointer to the data buffer
  @param contentLength The number of bytes to read

  @returns  A value indicating whether the operation succeeded or not
*/
/**************************************************************************/
bool cie_PN532::readBinaryContent(const EFPath filePath, byte* contentBuffer, word startingOffset, const word contentLength) {
  byte fileId;
  switch (filePath.selectionMode) {
    case SELECT_BY_EFID:
      if (!ensureElementaryFileIsSelected(filePath)) {
        return false;
      }
      fileId = 0x00; //Current selected file
    break;

    case SELECT_BY_SFI:
      if (!ensureDedicatedFileIsSelected(filePath.df)) {
        return false;
      }
      fileId = (byte) (filePath.id & 0x1F);
    break;

    default:
      PN532DEBUGPRINT.println(F("The selection mode must be either SELECT_BY_EFID or SELECT_BY_SFI"));
      return false;
  }
  bool success = false;
  word offset = startingOffset;
  byte preambleOctects = 2;
  byte statusWordOctects = 2;
  do {
    word contentPageLength = clamp(contentLength+startingOffset-offset, PAGE_LENGTH);
    byte readCommand[] = {
      0x00, //CLA
      0xB1, //INS: READ BINARY (ODD INS)
      0x00, //P1: zeroes
      fileId, //P2: sfi to select or zeroes (i.e. keep current selected file)
      0x04, //Lc: data field is made of 4 bytes
      0x54, 0x02, //Data field: here comes an offset of 2 bytes
      (byte) (offset >> 8), (byte) (offset & 0xFF), //the offset
      (byte) (contentPageLength + preambleOctects) //Le: bytes to be returned in the response
    };
    byte responseLength = ((byte) contentPageLength) + preambleOctects + statusWordOctects;
    byte* responseBuffer = new byte[responseLength];  
    success = _nfc.inDataExchange(readCommand, 10, responseBuffer, &responseLength);
    success = success && hasSuccessStatusWord(responseBuffer, responseLength);
    //Copy data over to the buffer
    if (success) {
      //The read binary command with ODD INS incapsulated the response with two preamble octets. Don't include them, they're not part of the content.
      for (word i = preambleOctects; i < contentPageLength + preambleOctects; i++) {
        contentBuffer[offset - startingOffset + i - preambleOctects] = responseBuffer[i];
      }
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
      0x04, //P1: Select by ID
      0x0C, //P2: No data in response field
      0x06, //Lc: length of ID
      0xA0, 0x00, 0x00, 0x00, 0x00, 0x39 //ID
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

    @returns The clamped value

*/
/**************************************************************************/
word cie_PN532::clamp(const word value, const byte maxValue) {
  if (value > maxValue) {
    return maxValue;
  } else {
    return value;
  }
}