const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keyMap[ROWS][COLS] = {
  {'1', '4', '0', '0'},
  {'2', '5', '0', '0'},
  {'3', '6', '0', '0'},
  {'D', 'C', '0', '0'}
};

byte keyCodePins[] = {
  38, 39, 40, 41
};

byte brailleCode = 0;

#define keyPressInterruptPin 18
boolean keyPress = false;
void setup() {
  Serial.begin(115200);
  for (byte i = 0; i < sizeof(keyCodePins); i++) {
    pinMode(keyCodePins[i], INPUT);
  }
  attachInterrupt(digitalPinToInterrupt(keyPressInterruptPin), ISRKeyPress, RISING);
}

void loop() {
  if (keyPress) {
    byte keyCode = getKeyPressCode();
    String key = String(keyMap[keyCode % ROWS][keyCode / COLS]);
    keyPress = false;
    switch (key[0]) {
      case 'C':
        Serial.println("Key Press: CONFIRM");
        writeBraille(brailleCode);
        brailleCode = 0;
        break;
      case 'D':
        Serial.println("Key Press: CLEAR");
        brailleCode = 0;
        Serial.println("Clearing Braille Code: " + String(brailleCode, BIN));
        break;
      default:
        Serial.println("Key Press: " + key);
        brailleCode |= key2Braille(key);
        Serial.println("Braille Code: " + String(brailleCode, BIN));
        break;
    }
  }
}

void writeBraille(byte brailleCode) {
  Serial.println("Writing BrailleCode : " + String(brailleCode, BIN));
}

byte key2Braille(String key) {
  switch (key[0]) {
    case '1':
      return 0b00000001;
    case '2':
      return 0b00000010;
    case '3':
      return 0b00000100;
    case '4':
      return 0b00001000;
    case '5':
      return 0b00010000;
    case '6':
      return 0b00100000;
    default:
      return 0x00;
  }
}

void ISRKeyPress() {
  //Serial.println("ISR In..!");
  keyPress = true;
}

byte getKeyPressCode() {
  byte keyCode = 0x00;
  for (byte i = 0; i < 4; i++) {
    keyCode |= digitalRead(keyCodePins[i]) << i;
  }
  //Serial.println("Key Code: " + String(keyCode, BIN));
  return keyCode;
}
