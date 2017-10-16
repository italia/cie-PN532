/**************************************************************************/
/*! 
    @file     cie_EFPath.h
    @author   Developers Italia
	@license  BSD (see License)
	
	Definition of the cie_EFPath structure used to describe paths in the CIE filesystem
	
	@section  HISTORY

	v1.0  - First definition of the structure
	
*/
/**************************************************************************/
#ifndef CIE_EF_PATH
#define CIE_EF_PATH
#include <Arduino.h>

struct cie_EFPath {
    byte df;
    byte selectionMode;
    word id;
};

#endif