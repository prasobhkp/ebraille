const byte brailleDotPins[] = {
  //32, 33, 34, 35, 36, 37
  36, 38, 40, 42, 44, 46
};

const byte cellEnablePins[] = {
  23, 25, 27, 29, 31, 33, 35, 37, 39, 41
  //22, 23, 24, 25, 26, 27, 28, 29, 30, 31
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
    clearEBraille();
    testPattern = 0x01;
    //Serial.println("Writing Test Pattern to Braille Cell " + String(i + 1) + " ...");
    for (byte j = 0; j < nDots; j++) {
      printEBrailleChar(i, testPattern << j);
      delay(100);
    }
    clearEBraille();
    testPattern = 0x20;
    for (byte j = 0; j < nDots; j++) {
      printEBrailleChar(i, testPattern >> j);
      delay(100);
    }
  }
  testPattern = 0x3F;
  for (byte i  = 0; i < nCells; i++) {
    printEBrailleChar(i, testPattern);
  }
  delay(2000);
  //Serial.println("Test Pattern Complete!!!");
  clearEBraille();
}
