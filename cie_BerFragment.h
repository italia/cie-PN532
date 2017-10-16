/**************************************************************************/
/*! 
    @file     cie_BerFragment.h
    @author   Developers Italia
	@license  BSD (see License)
	
	Definition of the cie_BerFragment structure used to describe an individual piece of information in BER encoded binary content
	
	@section  HISTORY

	v1.0  - First definition of the structure
	
*/
/**************************************************************************/
#ifndef CIE_BER_FRAGMENT
#define CIE_BER_FRAGMENT
#include <Arduino.h>
struct cie_BerFragment {
	cie_EFPath filePath;
	byte tag;
	word valueLength;
	byte childrenCount;
	cie_BerFragment* children;
};

#endif