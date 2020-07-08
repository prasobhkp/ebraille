#include <TimerOne.h>

#define nDots 6
#define nLines 5
#define nCells 10
#define nCharsBraillePage nLines *nCells

#include "eBrailleFunctions.h"
#include "ebrailleDisplay.h"
#include "eBrailleKeyPad.h"
#include "eBrailleEng2Braille.h"
#include "eBrailleFileIOSDCard.h"

byte brailleCode = 0;
String blueToothText = "";
byte braillePage[nLines][nCells];
byte indexCharBraillePage = 0;
byte indexLineBraillePage = 0;
char key;
byte keyCursor = 1;
boolean updateCursor = false;
byte keyCode = 0;

#define countTestModeKeyPress 5
#define secondsWaitForTestMode 3

#define bt Serial
#define baudrateBT 9600
#define btRX1 19
#define baudrateSM 9600

byte testInput[] = {
    0, 13, 1, 4, 5, 13, 8, 13,
    9, 13, 2, 13, 6, 10, 7, 1,
    14, 13, 5, 18, 15, 2, 13, 6,
    10, 4, 19, 14, 2, 10, 14, 13};

byte dotsIndex[] = {
    0, 1, 4, 5, 8, 9};

byte cursorPositions[] = {
    2, 3, 6, 7, 10, 11, 14, 15, 18, 19};

byte indexInput = 0;

boolean isPrint = true;

void setup()
{
  Serial.begin(baudrateSM);
  pinMode(LED_BUILTIN, OUTPUT);

  for (byte i = 0; i < sizeof(cellEnablePins); i++)
  {
    pinMode(cellEnablePins[i], OUTPUT);
  }

  for (byte i = 0; i < sizeof(brailleDotPins); i++)
  {
    pinMode(brailleDotPins[i], OUTPUT);
  }
  for (byte i = 0; i < nLines; i++)
  {
    for (byte j = 0; j < nCells; j++)
    {
      braillePage[i][j] = 0;
    }
  }

  for (byte i = 0; i < nCells; i++)
  {
    brailleDRAM[i] = 0x00;
  }
  //if (isTestMode()) {
    testMode();
  //}
  //testRun(50);

  initSDCard();
  myFile = SD.open("infofile.txt", FILE_WRITE);
  Serial.println(myFile.position());
  myFile.seek(10);
  Serial.println(myFile.position());
  myFile.write('A');
  Serial.println(readFromFile("infofile.txt", 0));

  /*
  String textEng = "";
  if (!checkEBrailleStorage()) {
    Serial.println("Warning!: Storage error!");
    textEng = "Welcome to eBraille";
  } else {
    textEng = readFromFile("ebraille.txt", 0);
    Serial.println("Current Filename :" + currentFilename + ", Position: "+ positionStartByteCurrentFile);
  }

  
  initTimer();
  Serial.println(textEng);
  loadBrailleFromText(textEng);
  printEBrailleText(braillePage[indexLineBraillePage]);
  sOl("Waiting for User Key Press...");
  initBlueToothReceive();
  */
}

void initTimer()
{
  Timer1.initialize(10000);
  Timer1.attachInterrupt(scanKeyPad); // Keypad Scanning to run every 10 milliseconds
  Serial.println("Timer initialized for Key Pad Scanning");
}

void loop()
{
  //runEBraille();
  testDisplay();
}

void initBlueToothReceive()
{
  pinMode(btRX1, INPUT_PULLUP);
  bt.begin(baudrateBT);
  bt.println("Welcome to eBraille :)...\nNow I am Ready to receive!!!");
}

boolean loadBluetoothText()
{
  //Serial.println("Waiting for Text from BlueTooth...");
  while (bt.available())
  {
    byte rd = bt.read();
    if (rd != '\n')
    {
      blueToothText += String(char(rd));
    }
    else
    {
      Serial.println("Text from BlueTooth: " + blueToothText);
      loadBrailleFromText(blueToothText);
      blueToothText = "";
      indexCharBraillePage = 0;
      indexLineBraillePage = 0;
      return true;
    }
  }
  return false;
}

void serialEvent1()
{
  if (loadBluetoothText())
  {
    printSerialSingleLineBraillePattern(braillePage[indexLineBraillePage]);
    printEBrailleText(braillePage[indexLineBraillePage]);
  }
}

void loadBrailleFromText(String m)
{
  String textBraille = converTextEng2BytesBraille(m);
  loadTextBraille2BraillePage(textBraille);
}

void testRun(byte count)
{
  for (byte i = 0; i < nCells * nLines; i++)
  {
    brailleCode = 0;
    for (byte j = 0; j < random(3, 6); j++)
    {
      brailleCode |= (1 << random(0, 5));
    }
    //Serial.println(pPrint(String(brailleCode, BIN), 6));
    braillePage[i / nCells][i % nCells] = brailleCode;
  }
  isPrint = true;
  printSerialBraillePage();
}

void loadTextBraille2BraillePage(String textBraille)
{
  byte countBytes = nLines * nCells;
  for (byte i = 0; i < countBytes; i++)
  {
    //Serial.println(pPrint(String(textBraille[i], BIN), 6));
    if (i <= textBraille.length())
    {
      braillePage[i / nCells][i % nCells] = byte(textBraille.charAt(i));
    }
    else
    {
      braillePage[i / nCells][i % nCells] = 0;
    }
  }
  isPrint = true;
  printSerialBraillePage();
}

void testLoadKey()
{
  key = keyMap[keyCode / COLS][keyCode % COLS];
  if (key)
  {
    keyPress = true;
  }
  //delay(200);
}

void runEBraille()
{
  if (keyPress)
  {
    keyPress = false;
    sOl("Key Press : " + String(key));
    processKeyInput();
  }
}

void processKeyInput()
{
  byte columnPage = indexCharBraillePage % nCells;
  switch (key)
  {
  case 'A':
    sOl("Key Press: CLEAR");
    brailleCode = 0;
    sOl("Clearing Braille Code: " + pPrint(String(brailleCode, BIN), 6));
    break;
  case 'B':
    indexCursor = indexCharBraillePage % nCells;
    sOl("Key Press: CONFIRM");
    braillePage[indexLineBraillePage][columnPage] = brailleCode;
    sOl("Updating Char " + String(columnPage + 1) + " of Line " + String(indexLineBraillePage + 1) + " ...");
    sOl("Printing Braille Dots: ");
    printSerialSingleLineBraillePattern(braillePage[indexLineBraillePage]);
    printEBrailleText(braillePage[indexLineBraillePage]);
    indexCharBraillePage++;
    if (indexCharBraillePage % nCells == 0)
    {
      sOl("Line " + String(indexLineBraillePage + 1) + " Complete!!!");
      indexLineBraillePage++;
      if (indexCharBraillePage >= nCharsBraillePage)
      {
        indexCharBraillePage %= nCharsBraillePage;
        indexLineBraillePage %= nLines;
      }
    }
    brailleCode = 0;
    printSerialBraillePage();
    break;
  case 'C':
    sOl("Key Press: Prev. Line");
    if (indexLineBraillePage > 0)
    {
      indexLineBraillePage--;
    }
    sOl("Printing Line: " + String(indexLineBraillePage + 1));
    sOl("Printing Braille Dots: ");
    printSerialSingleLineBraillePattern(braillePage[indexLineBraillePage]);
    printEBrailleText(braillePage[indexLineBraillePage]);
    printSerialBraillePage();
    break;
  case 'D':
    sOl("Key Press: Next Line");
    if (indexLineBraillePage < nLines - 1)
    {
      indexLineBraillePage++;
    }
    sOl("Printing Line: " + String(indexLineBraillePage + 1));
    sOl("Printing Braille Dots: ");
    printSerialSingleLineBraillePattern(braillePage[indexLineBraillePage]);
    printEBrailleText(braillePage[indexLineBraillePage]);
    printSerialBraillePage();
    break;
  case 'O':
    sOl("Key Press: Next File");
    break;
  case 'P':
    sOl("Key Press: Prev. File");
    break;
  default:
    if (key >= '0' && key <= '6')
    {
      brailleCode |= key2Braille(String(key));
      sOl("Braille Code: " + pPrint(String(brailleCode, BIN), 6));
    }
    else if (key >= 'E' && key <= 'N')
    {
      keyCursor = key - 'E';
      sOl("Cursor Position : " + String(keyCursor));
      updateCursor = true;
      indexCharBraillePage = (indexLineBraillePage * nCells) + keyCursor - 1;
    }
    break;
  }
  sOl("");
}

void printSerialBraillePage()
{
  sOl("------------------------------");
  sOl("     Braille Page [" + String(nCells) + "x" + String(nLines) + "]");
  sOl("------------------------------");
  for (byte i = 0; i < nLines; i++)
  {
    printSerialSingleLineBraillePattern(braillePage[i]);
  }
  sOl("------------------------------");
}

void printSerialSingleLineBraillePattern(byte *brailleLine)
{
  for (byte row = 0; row < nDots / 2; row++)
  {
    for (byte j = 0; j < nCells; j++)
    {
      for (byte k = 2 * row; k < 2 * row + 2; k++)
      {
        if (brailleLine[j] >> k & 0x01)
        {
          sO("*");
        }
        else
        {
          sO(" ");
        }
      }
      sO("|");
    }
    sOl("");
  }
  sOl("");
}

boolean isTestMode()
{
  sOl("Waiting for Tester key press...");
  long t1 = millis();
  byte countKeyPress = 0;
  byte countSeconds = 0;
  while ((countKeyPress < countTestModeKeyPress) && (countSeconds < secondsWaitForTestMode))
  {
    if (keyPress)
    {
      keyPress = false;
      if (key == 'C')
      {
        sOl("Key Press Count: " + String(countKeyPress) + ", " + String(countTestModeKeyPress - countKeyPress) + " more required");
        countKeyPress++;
      }
    }
    if ((millis() - t1) > 1000)
    {
      countSeconds++;
      sOl(String(secondsWaitForTestMode - countSeconds) + "s remainig...");
      t1 = millis();
    }
  }
  if (countKeyPress == countTestModeKeyPress)
  {
    return true;
  }
  sOl("Exiting testMode...");
  return false;
}

void testMode()
{
  /*
    long t1 = millis();
    byte countTimer = 0;
    sOl("Test Mode In!!!");
    sOl("Testing Going On...");
    while (countTimer < 5) {
    if (millis() - t1 > 1000) {
      Serial.print("..." + String(countTimer++));
      t1 = millis();
    }
    }
  */
  testDisplay();
  sOl("Test Mode Out!!!");
}

void blinkCursor()
{
  cursorState = ~cursorState;
  byte brailleCodeAtCursor;
  if (brailleDRAM[indexCursor])
  {
    brailleCodeAtCursor = brailleDRAM[indexCursor] & cursorState;
  }
  else
  {
    brailleCodeAtCursor = 0xff & cursorState;
  }
  printEBrailleChar(indexCursor, brailleCodeAtCursor);
  boolean moveCursor = ((brailleCodeAtCursor == 0) and (not(cursorState))) or ((brailleCodeAtCursor > 0) and (brailleCodeAtCursor < 255) and cursorState) and updateCursor;
  if (moveCursor)
  {
    indexCursor = keyCursor - 1;
    updateCursor = false;
  }
}

void scanKeyPad()
{
  key = customKeypad.getKey();
  if (key)
  {
    keyPress = true;
  }
}

void sO(String m)
{
  if (isPrint)
  {
    Serial.print(m);
  }
}

void sOl(String m)
{
  sO(m);
  //sO(String(char(0x0d)));
  sO("\r\n");
}
