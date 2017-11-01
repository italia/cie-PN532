#include "cie_Nfc_Mock.h"

void cie_Nfc_Mock::begin() {
  //Do nothing, we do not need to initialize anything in this mock class
}

bool cie_Nfc_Mock::detectCard() {
  //Always return true, simulate a card is present
  return true;
}

bool cie_Nfc_Mock::sendCommand(byte* command, byte commandLength, byte* response, byte* responseLength) {

  _attemptedCommandsCount += 1;
  if (_attemptedCommandsCount > _expectedCommandsCount) {
    Serial.println("cie_PN532 tried to execute more commands than expected");
    return false;
  }
  
  bool isSameCommand = areEqual(command, _expectedCommands[_executedCommandsCount].command, _expectedCommands[_executedCommandsCount].commandOffset, _expectedCommands[_executedCommandsCount].commandLength);
  if (isSameCommand) {
    for (byte i = 0; i < _expectedCommands[_executedCommandsCount].responseLength; i++) {
      response[i] = _expectedCommands[_executedCommandsCount].response[i];
    }
    *responseLength = _expectedCommands[_executedCommandsCount].responseLength;
    _executedCommandsCount += 1;
    return true;
  } else {
    return false;
  }
}

void cie_Nfc_Mock::expectCommands(const byte count) {
  _executedCommandsCount = 0;
  _attemptedCommandsCount = 0;
  _expectedCommandsCount = count;
  _expectedCommands = new cie_Command[_expectedCommandsCount];
}

void cie_Nfc_Mock::expectCommand(byte* command, const byte commandOffset, const byte commandLength, byte* response, const byte responseLength) {
  _expectedCommands[_executedCommandsCount].command = command;
  _expectedCommands[_executedCommandsCount].commandLength = commandLength;
  _expectedCommands[_executedCommandsCount].commandOffset = commandOffset;
  _expectedCommands[_executedCommandsCount].response = response;
  _expectedCommands[_executedCommandsCount].responseLength = responseLength;
}


bool cie_Nfc_Mock::allExpectedCommandsExecuted() {
  return _expectedCommandsCount == _executedCommandsCount && _expectedCommandsCount == _attemptedCommandsCount;
}

bool cie_Nfc_Mock::areEqual(byte* originalBuffer, byte* comparedBuffer, const byte offset, const byte length) {
  for (byte i = 0; i < length; i++) {
    if (originalBuffer[i+offset] != comparedBuffer[i]) {
      Serial.print(F("Command at index "));
      Serial.print(_executedCommandsCount);
      Serial.print(F(" was not expected (byte "));
      Serial.print(i);
      Serial.println(F(" was different)"));
      return false;
    }
  }
  return true;
}

cie_Nfc_Mock::~cie_Nfc_Mock() {
  for (byte i = 0; i < _expectedCommandsCount; i++) {
    delete [] _expectedCommands[i].command;
    delete [] _expectedCommands[i].response;
  }
  delete [] _expectedCommands;
}