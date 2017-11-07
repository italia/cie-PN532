#ifndef CIE_COMMAND
#define CIE_COMMAND

#include <Arduino.h>
struct cie_Command {
  public:
    byte *command;
    byte commandOffset;
    byte commandLength;
    byte *response;
    word responseLength;
};

#endif