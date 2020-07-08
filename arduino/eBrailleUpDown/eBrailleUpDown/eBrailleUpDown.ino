#include <TimerOne.h>
/*
  Dots Data: IO22 - IO27
  Cell Enable: I03 - I012
*/

const byte brailleDot[6] = {
  36, 38, 40, 42, 44, 46
};

const byte cellEnable[] = {
  23, 25, 27, 29, 31, 33, 35, 37, 39, 41
};

#define nCells 10
#define nDots 6
#define nextLinePin 48
#define prevLinePin 50

// binary Braille Dot patters for ASCII from 32 to 122
// https://create.arduino.cc/projecthub/CesareBrizio/ascii-braille-real-time-translation-via-arduino-dd97a9
byte brailleCodes[] = {
  0,    14,   7,    2,    15,   15,   8,    13,
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
#define maxLines 10
byte ascii2Braille[255];
volatile boolean nextLine = false;
volatile boolean prevLine = false;
boolean t = false;
static unsigned long lastKeyPressTimeNextLine = 0;
static unsigned long lastKeyPressTimePrevLine = 0;

String textEng = "abcdefghijklmnopqrstuvwxyz0123456789";
//String textEng = " ";

String inputString = "";         // a String to hold incoming data
boolean stringComplete = false;  // whether the string is complete
byte nChars = textEng.length();
byte charIndexAtLineStart[maxLines];
byte line = 0;
byte charIndex = 0;
byte cellIndex = 0;

void setup()
{
  // put your setup code here, to run once:
  for (byte i = 0; i < nDots; i++)
    pinMode(brailleDot[i], 1);
  for (byte i = 0; i < nCells; i++)
    pinMode(cellEnable[i], 1);

  for (byte i = 0; i < 73; i++) {
    ascii2Braille[asciiCodes[i]] = brailleCodes[i];
  }
  Serial.begin(115200);
  pinMode(nextLinePin, INPUT_PULLUP);
  pinMode(prevLinePin, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  initTimer();
  //attachInterrupt(digitalPinToInterrupt(nextLinePin), ISRNextLine, RISING);
  //attachInterrupt(digitalPinToInterrupt(prevLinePin), ISRPrevLine, RISING);
  clearBraille();
  delay(2000);
  charIndexAtLineStart[line] = charIndex;
}

void initTimer()
{
  Timer1.initialize(10000);
  Timer1.attachInterrupt(scanKeyPress); // Keypad Scanning to run every 10 milliseconds
  Serial.println("Timer initialized for Key Pad Scanning");
}
void loopA()
{
  
}

void loop() {
  charIndex = charIndexAtLineStart[line];
  for (cellIndex = 0; (cellIndex < nCells) and (charIndex < nChars);) {
    byte c = textEng.charAt(charIndex++);
    if (isDigit(c)) {
      writeBraille(cellIndex++ % nCells, 0b00010111);
    }
    writeBraille(cellIndex++ % nCells, ascii2Braille[c]);
    //Serial.print(String(line) + "-" + String(charIndex) + "-" + char(c) + ", ");
    Serial.print(char(c));
  }
  Serial.println();
  Serial.println("Character: " + String(charIndex) + " of " + String(nChars) + ", Line: " + String(line + 1));
  while (!nextLine and !prevLine);
  if (nextLine) {
    nextLine = false;
    if (charIndex < nChars) {
      line++;
      charIndexAtLineStart[line] = charIndex;
    }
    else
    {
      //line = 0;
      getSerialData();
    }
  }

  else if (prevLine) {
    prevLine = false;
    if (line > 0) {
      line--;
    }
  }
  clearBraille();
}

void clearBraille() {
  for (byte j = 0; j < nCells; j++) {
    digitalWrite(cellEnable[j], 1);
    for (int i = 0; i < nDots; i++) {
      digitalWrite(brailleDot[i], LOW);
    }
    digitalWrite(cellEnable[j], 0);
  }
}

void writeBraille(byte cellID, byte brailleCode) {
  if (brailleCode) {
    digitalWrite(cellEnable[cellID], 1);
    for (int i = 0; i < nDots; i++) {
      digitalWrite(brailleDot[i], brailleCode >> i & 0x01);
    }
    digitalWrite(cellEnable[cellID], 0);
  }
}

void scanKeyPress()
{
  boolean b1 = digitalRead(nextLinePin);
  boolean b2 = digitalRead(prevLinePin);
  if (b1 == 0)
  {
    keyPressNext();
  }
  if (b2 == 0)
  {
    keyPressPrev();
  }
}

void keyPressNext() {
  unsigned long keyPressTime = millis();
  if (keyPressTime - lastKeyPressTimeNextLine > 50) {
    Serial.println("Next Line");
    nextLine = true;
  }
  lastKeyPressTimeNextLine = keyPressTime;
}

void keyPressPrev() {
  unsigned long keyPressTime = millis();
  if (keyPressTime - lastKeyPressTimePrevLine > 50) {
    Serial.println("Prev Line");
    prevLine = true;
  }
  lastKeyPressTimePrevLine = keyPressTime;
}

void getSerialData() {
  while (Serial.available()) {
    digitalWrite(LED_BUILTIN, t);
    t = !t;
    char inChar = (char)Serial.read();
    if (inChar == '\n') {
      textEng = inputString;
      nChars = textEng.length();
      line = 0;
      inputString = "";
    }
    else {
      inputString += inChar;
    }
  }
}
