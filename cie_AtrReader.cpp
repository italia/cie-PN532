/**************************************************************************/
/*!
    @file     cie_AtrReader.cpp
    @author   Developers Italia
    @license  BSD (see License)

	
	Reads and parses an Atr encoded binary content

	@section  HISTORY

	v1.0  - Parsing of the most interesting parts
*/
/**************************************************************************/
#include "cie_AtrReader.h"

#define PN532DEBUGPRINT Serial

//Can we chain constructors in this version of C++ to avoid repetitions?
//SomeType() : SomeType(42) {}
cie_AtrReader::cie_AtrReader (cie_PN532 *cie) :
_cie(cie)
{
}


/**************************************************************************/
/*!
  @brief Detects the length of an Elementary File by reading its ATR encoded content
  
  @param filePath a structure indicating the parent Dedicated File (either ROOT_MF or CIE_DF), the selection mode (either SELECT_BY_EFID or SELECT_BY_SFI) and the file identifier (either a sfi or an efid)	
  @param contentLength The pointer to the length value

  @returns  A value indicating whether the operation succeeded or not
*/
/**************************************************************************/
bool cie_AtrReader::detectLength(const cie_EFPath filePath, word *contentLength) {
  //The EF.ATR record has a minimum of 33 bytes
  //Please refer to EF.ATR content here http://www.unsads.com/specs/IASECC/IAS_ECC_v1.0.1_UK.pdf#page=19
  const byte chunkSize = 4;
  byte endingSequence[chunkSize] = { 0x82, 0x02, 0x90, 0x00 };
  byte buffer[chunkSize];
  word offset = 0x21;
  while (true) {

    if (!_cie->readBinaryContent(filePath, buffer, offset, chunkSize)) {
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
       //Match found for all octets in the sequence! We're at the end of the file.
       *contentLength = offset + chunkSize;
       return true;
    } else {
      //let's position ourself in a place where we can read all of the matching octets
      offset += chunkSize - matchingOctets;
    }
  } 
}