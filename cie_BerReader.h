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
#include "cie_BerTriple.h"
#include "cie_PN532.h"

#ifndef cie_PN532
class cie_PN532;
#endif

#define BER_READER_MAX_OFFSET   (2048)
#define BER_READER_MAX_LENGTH   (2048)
#define BER_READER_MAX_COUNT    (200)


class cie_BerReader
{
  public:
    cie_BerReader(cie_PN532 *cie);
    bool readTriples(const cie_EFPath filePath, cieBerTripleCallbackFunc callback, word *length, const byte maxDepth);
    
  private:
    cie_PN532 *_cie;
    word _currentOffset;
    void resetCursor();
    bool areEqual(byte *buffer1, byte length1, byte *buffer2, byte length2);
    void readBinaryContent(const cie_EFPath filePath, const word offset, const word length);

    bool readTriple(const cie_EFPath filePath, cie_BerTriple *triple, word *length);
    bool readTripleValue(const cie_BerTriple triple, byte *buffer);
    bool detectLength(const cie_EFPath filePath, word *contentOffset, word *contentLength, byte *lengthOctets);
    bool detectTag (const cie_EFPath filePath, byte *classification, byte *encoding, unsigned int *type, byte *tagOctets);
    bool readOctets(const cie_EFPath filePath, byte *buffer, const word offset, const word length);
    bool readOctets(const cie_EFPath filePath, byte *buffer, const word length);
    bool readOctet(const cie_EFPath filePath, byte *octet);
    
};

#endif