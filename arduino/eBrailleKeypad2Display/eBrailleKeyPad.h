#include <Keypad.h>

const byte ROWS = 5; //five rows
const byte COLS = 5; //five columns
char keyMap[ROWS][COLS] = {
  {'1', '4', 'E', 'J', 'O'},
  {'2', '5', 'F', 'K', 'P'},
  {'3', '6', 'G', 'L', 'Q'},
  {'A', 'B', 'H', 'M', 'R'},
  {'C', 'D', 'I', 'N', 'S'}
};

/*
  const byte ROWS = 5; //five rows
  const byte COLS = 4; //four columns
  char keyMap[ROWS][COLS] = {
  {'A', 'B', 'C', 'D'},
  {'E', 'F', 'G', 'H'},
  {'I', 'J', 'K', 'L'},
  {'M', 'N', 'O', 'Q'},
  {'R', 'S', 'T', 'U'},
  };
*/
byte keyPadRowPins[ROWS] = {38, 39, 40, 41, 42}; //connect to the row pinouts of the keypad
byte keyPadColPins[COLS] = {43, 44, 45, 46, 47}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(keyMap), keyPadRowPins, keyPadColPins, ROWS, COLS);

boolean keyPress = false;

/*
  boolean keyPress = false;
  byte keyPadID = 0;

  byte getKeyPressCode() {
  byte keyCode = 0x00;
  switch (keyPadID) {
    case 2:
      keyCode = 0x10;
      for (byte i = 0; i < COLS; i++) {
        keyCode |= digitalRead(keyCodePins2[i]) << i;
      }
      break;
    default:
      for (byte i = 0; i < COLS; i++) {
        keyCode |= digitalRead(keyCodePins1[i]) << i;
      }
  }
  Serial.println("Key Code: " + String(keyCode, BIN));
  return keyCode;
  }

  char getKeyPress() {
  byte keyCode = getKeyPressCode();
  String key = String(keyMap[keyCode / COLS][keyCode % COLS]);
  return key[0];
  }

  void ISRKeyPress1() {
  Serial.println("Key Press!...");
  keyPadID = 1;
  keyPress = true;
  }

  void ISRKeyPress2() {
  Serial.println("Key Press!...");
  keyPadID = 2;
  keyPress = true;
  }
*/
