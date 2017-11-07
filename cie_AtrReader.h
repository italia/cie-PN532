/**************************************************************************/
/*! 
    @file     cie_AtrReader.h
    @author   Developers Italia
	  @license  BSD (see License)
	

	Reads and parses an ATR encoded binary content
	
	@section  HISTORY

	v1.0  - First definition
	
*/
/**************************************************************************/
#ifndef CIE_ATR_READER
#define CIE_ATR_READER

#include "cie_PN532.h"
#include "cie_EFPath.h"

#ifndef cie_PN532
class cie_PN532;
#endif

class cie_AtrReader
{
  public:
	cie_AtrReader(cie_PN532 *cie);
	bool detectLength(const cie_EFPath filePath, word *contentLength);

  private:
	  cie_PN532 *_cie;
};

#endif