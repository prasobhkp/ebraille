const byte brailleDotPins[] = {
  32, 33, 34, 35, 36, 37
};

const byte cellEnablePins[] = {
  22, 23, 24, 25, 26, 27, 28, 29, 30, 31
};

byte brailleDRAM[nCells];
byte indexCursor = 0;
byte cursorState = 0xFF;

void printEBrailleChar(byte cellID, byte brailleCode) {
  digitalWrite(cellEnablePins[cellID], 1);
  for (byte j = 0; j < nDots; j++) {
    digitalWrite(brailleDotPins[j], brailleCode >> j & 0x01);
  }
  digitalWrite(cellEnablePins[cellID], 0);
}

void printEBrailleText(byte* brailleLine) {
  digitalWrite(LED_BUILTIN, HIGH);
  for (byte i = 0; i < nCells; i++) {
    printEBrailleChar(i, brailleLine[i]);
    //brailleDRAM[i] = brailleLine[i];
  }
  digitalWrite(LED_BUILTIN, LOW);
}

void clearEBraille() {
  for (byte j = 0; j < nCells; j++) {
    printEBrailleChar(j, 0x00);
  }
}

void testDisplay() {
  byte testPattern = 0x01;
  for (byte i  = 0; i < nCells; i++) {
    testPattern = 0x01;
    //Serial.println("Writing Test Pattern to Braille Cell " + String(i + 1) + " ...");
    for (byte j = 0; j < nDots; j++) {
      printEBrailleChar(i, testPattern << j);
      delay(20);
    }
    testPattern = 0x20;
    for (byte j = 0; j < nDots; j++) {
      printEBrailleChar(i, testPattern >> j);
      delay(20);
    }
  }
  testPattern = 0x3F;
  for (byte i  = 0; i < nCells; i++) {
    printEBrailleChar(i, testPattern);
  }
  delay(500);
  //Serial.println("Test Pattern Complete!!!");
  clearEBraille();
}
