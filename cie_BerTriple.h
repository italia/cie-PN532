/**************************************************************************/
/*! 
    @file     cie_BerTriple.h
    @author   Developers Italia
	@license  BSD (see License)
	
	Definition of the cie_BerTriple structure used to describe an individual piece of information in BER encoded binary content
	
	@section  HISTORY

	v1.0  - First definition of the structure
	
*/
/**************************************************************************/
#ifndef CIE_BER_TRIPLE
#define CIE_BER_TRIPLE

#include <Arduino.h>

class cie_BerTriple {
  public:
	byte classification;
	byte encoding;
	unsigned int type;

	word contentOffset;
	word contentLength;

	cie_EFPath filePath;
	byte childrenCount;
	cie_BerTriple** children;
};


#endif