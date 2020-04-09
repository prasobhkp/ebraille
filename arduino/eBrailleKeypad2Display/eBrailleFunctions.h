char braille2Key(byte brailleCode) {
  switch (brailleCode) {
    case 0x01:
      return '1';
    case 0x04:
      return '2';
    case 0x10:
      return '3';
    case 0x02:
      return '4';
    case 0x08:
      return '5';
    case 0x20:
      return '6';
    default:
      return ' ';
  }
}

byte key2Braille(String key) {
  switch (key[0]) {
    case '1':
      return 0b000001;
    case '2':
      return 0b000100;
    case '3':
      return 0b010000;
    case '4':
      return 0b000010;
    case '5':
      return 0b001000;
    case '6':
      return 0b100000;
    default:
      return 0x00;
  }
}

String pPrint(String inText, byte nDigits) {
  //byte nDigits = 8;
  byte len = inText.length();
  String outText = "";
  if (nDigits == 2) {
    outText = "0x";
  }
  for (byte i = 0; i < (nDigits - len); i++) {
    outText += '0';
  }
  outText += inText;
  return outText;
}

String braillePattern2NumStr(byte brailleCode) {
  String brailleNumStr = "";
  for (byte i = 0; i < nDots; i++) {
    byte code = (brailleCode & (0x01 << i));
    if (code) {
      brailleNumStr += String(braille2Key(code));
    }
  }
  return brailleNumStr;
}
