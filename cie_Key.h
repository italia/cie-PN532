/**************************************************************************/
/*! 
    @file     cie_Key.h
    @author   Developers Italia
	@license  BSD (see License)
	
	Definition of the cie_Key class describing an encryption key
	
	@section  HISTORY

	v1.0  - First definition of the class
	
*/
/**************************************************************************/
#ifndef CIE_KEY
#define CIE_KEY

#include <Arduino.h>
class cie_Key {
  public:
	~cie_Key();
	byte *exponent;
	byte exponentLength;
	byte *modulus;
	word modulusLength;
};
#endif