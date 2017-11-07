#include <cie_Nfc.h>
#include "cie_Command.h"

#ifndef CIE_NFC_MOCK
#define CIE_NFC_MOCK

class cie_Nfc_Mock: public cie_Nfc {
  public:
    ~cie_Nfc_Mock();
    void begin();
    bool detectCard();
    bool sendCommand(byte *command, byte commandLength, byte *response, word *responseLength);

    //unit testing helper functions
    void expectCommands(const byte count);
    void expectCommand(byte *command, const byte commandLength, const byte commandOffset, word *response, const byte responseLength);
    bool allExpectedCommandsExecuted();

  private:
    void clear();
    bool areEqual(byte *originalBuffer, byte *comparedBuffer, const byte offset, const byte length);
    void printHex(const byte  *data, const byte numBytes);
    void generateRandomBytes(byte *buffer, const word offset, const byte length);

    cie_Command *_expectedCommands;
    byte _expectedCommandsCount;
    byte _executedCommandsCount;
    byte _attemptedCommandsCount;
};

#endif