/**************************************************************************/
/*!
    @file     cie_Key.cpp
    @author   Developers Italia
    @license  BSD (see License)

	Implementation of the cie_Key class describing an encryption key

	@section  HISTORY

	v1.0  - First implementation of the class
*/
/**************************************************************************/
#include "cie_Key.h"

cie_Key::~cie_Key() {
    delete [] exponent;
    delete [] modulus;
}