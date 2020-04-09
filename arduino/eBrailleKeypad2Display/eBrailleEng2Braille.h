// binary Braille Dot patters for ASCII from 32 to 122
// https://create.arduino.cc/projecthub/CesareBrizio/ascii-braille-real-time-translation-via-arduino-dd97a9
byte brailleCodes[] = {
  64,    14,   7,    2,    15,   15,   8,    13,
  28,   32,   40,   48,   52,   36,   56,   60,
  44,   24,   12,   10,   11,   32,   40,   48,
  52,   36,   56,   60,   44,   24,   28,   34,
  42,   50,   54,   38,   58,   62,   46,   26,
  30,   35,   43,   29,   51,   55,   39,   32,
  40,   48,   52,   36,   56,   60,   44,   24,
  28,   34,   42,   50,   54,   38,   58,   62,
  46,   26,   30,   35,   43,   29,   51,   55,
  39
};

byte asciiCodes[] = {
  32,   33,   34,   34,   40,   41,   44,   46,
  48,   49,   50,   51,   52,   53,   54,   55,
  56,   57,   58,   59,   63,   65,   66,   67,
  68,   69,   70,   71,   72,   73,   74,   75,
  76,   77,   78,   79,   80,   81,   82,   83,
  84,   85,   86,   87,   88,   89,   90,   97,
  98,   99,   100,  101,  102,  103,  104,  105,
  106,  107,  108,  109,  110,  111,  112,  113,
  114,  115,  116,  117,  118,  119,  120,  121,
  122
};

byte ascii2Braille[255];

void setupASCII2Braille() {
  for (byte i = 0; i < 73; i++) {
    ascii2Braille[asciiCodes[i]] = brailleCodes[i];
  }
}

byte reverseByte(byte inByte) {
  byte outByte = 0;
  byte j = 0;
  for (byte i = 0; i < nDots; i++) {
    if (inByte & (1 << ((nDots - 1) - i))) {
      outByte |= 0x20 >> ((nDots - 1) - i);
    }
    //Serial.println(pPrint(String(inByte & (1 << i), BIN),6) + ", " + pPrint(String(outByte, BIN),6));
  }
  //Serial.println(pPrint(String(inByte, BIN), 6) + ", " + pPrint(String(outByte, BIN), 6));
  //Serial.println();
  return outByte;
}

boolean isSpecial(char ch) {
  String specialChars = " ~!@#$%^&*()<>?:\"|{}_+[];'\\,./";
  for (byte i=0; i<specialChars.length(); i++) {
    if (ch == specialChars.charAt(i)) {
      return true;
    }
  }
  return false;
}

String converTextEng2BytesBraille(String textEng) {
  setupASCII2Braille();
  String textBraille = "";
  byte countSpace = 0;
  for (byte i = 0; i < min(textEng.length(), nCells * nLines);) {
    byte c = textEng.charAt(i++);
    if (isDigit(c)) {
      textBraille += String(char(reverseByte(0b00010111)));
      textBraille += String(char(reverseByte(ascii2Braille[c])));
    } else if (isSpecial(c)) {
      textBraille += String(char(0x40));
    } else {
      textBraille += String(char(reverseByte(ascii2Braille[c])));
    }
  }
  return textBraille;
}
