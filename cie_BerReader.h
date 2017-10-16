/**************************************************************************/
/*! 
    @file     cie_BerReader.h
    @author   Developers Italia
	  @license  BSD (see License)
	

	Reads and parses a BER encoded binary content
	
	@section  HISTORY

	v1.0  - First definition
	
*/
/**************************************************************************/
#ifndef CIE_BER_READER
#define CIE_BER_READER

#include "cie_EFPath.h"
#include "cie_BerFragment.h"
#include "cie_PN532.h"

#ifndef cie_PN532
class cie_PN532;
#endif

class cie_BerReader
{
  public:
    cie_BerReader(cie_PN532* cie);
    cie_BerFragment readFragments(const cie_EFPath, word* contentLength);
    bool readFragmentValue(cie_BerFragment fragment, byte* buffer);
	  bool detectLength(const cie_EFPath filePath, word* contentLength);

  private:
	  cie_PN532* _cie;
};

#endif