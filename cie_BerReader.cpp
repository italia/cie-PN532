/**************************************************************************/
/*!
    @file     cie_BerReader.cpp
    @author   Developers Italia
    @license  BSD (see License)

	
	Reads and parses a BER encoded binary content

	@section  HISTORY

	v1.0  - Reading of fragments and binary values
*/
/**************************************************************************/
#include "cie_BerReader.h"


#define PN532DEBUGPRINT Serial

//Can we chain constructors in this version of C++ to avoid repetitions?
//SomeType() : SomeType(42) {}
cie_BerReader::cie_BerReader (cie_PN532* cie) :
_cie(cie)
{   
}

cie_BerFragment cie_BerReader::readFragments(const cie_EFPath, word* contentLength) {

}
bool cie_BerReader::readFragmentValue(cie_BerFragment fragment, byte* buffer) {

}

/**************************************************************************/
/*!
  @brief Detects the length of an Elementary File by reading its BER encoded content
  
  @param filePath a structure indicating the parent Dedicated File (either ROOT_MF or CIE_DF), the selection mode (either SELECT_BY_EFID or SELECT_BY_SFI) and the file identifier (either a sfi or an efid)	
  @param contentLength The pointer to the length value

  @returns  A value indicating whether the operation succeeded or not
*/
/**************************************************************************/
bool cie_BerReader::detectLength(const cie_EFPath filePath, word* contentLength) {
  //Please refer to https://en.wikipedia.org/wiki/X.690#Identifier_octets
  const byte berHeaderLength = 6; //6 will do for the CIE
  byte buffer[berHeaderLength];
  if (!_cie->readBinaryContent(filePath, buffer, READ_FROM_START, berHeaderLength)) {
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