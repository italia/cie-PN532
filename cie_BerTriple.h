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

#define BER_PRIMITIVE               (0x01)
#define BER_CONSTRUCTED             (0x02)

#define BER_CLASS_UNIVERSAL         (0x00)
#define BER_CLASS_APPLICATION       (0x01)
#define BER_CLASS_CONTEXT_SPECIFIC  (0x02)
#define BER_CLASS_PRIVATE           (0x03)

#include <Arduino.h>

enum BerClass { 
	Universal = 0x00, 
	Application = 0x01, 
	ContextSpecific = 0x02, 
	Private = 0x03 
};

enum BerEncoding { 
	Primitive = 0x00, 
	Constructed = 0x01 
}; 

enum BerType { 
	EndOfContent = 0x00,
	Boolean = 0x01,
	Integer = 0x02,
	BitString = 0x03,
	OctetString = 0x04,
	Null = 0x05,
	ObjectIdentifier = 0x06,
	ObjectDescriptor = 0x07,
	External = 0x08,
	Real = 0x09,
	Enumerated = 0x0A,
	EmbeddedPDV = 0x0B,
	UTF8String = 0x0C,
	RelativeOID = 0x0D,
	Sequence = 0X11,
	Set = 0x11,
	NumericString = 0x12,
	PrintableString = 0x13,
	T61String = 0x14,
	VideotexString = 0x15,
	IA5String = 0x16,
	UTCTime = 0x17,
	GeneralizedTime = 0x18,
	GraphicString = 0x19,
	VisibleString = 0x1A,
	GeneralString = 0x1B,
	UniversalString = 0x1C,
	CharacterString = 0x1D,
	BMPString = 0x1E 
};

struct cie_BerTriple {
	byte classification;
	byte encoding;
	unsigned int type;

	word length;
	cie_EFPath filePath;
	byte childrenCount;
	cie_BerTriple* children;
};


#endif