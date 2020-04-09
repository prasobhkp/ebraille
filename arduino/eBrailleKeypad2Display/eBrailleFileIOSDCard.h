#include <SPI.h>
#include <SD.h>

/*
  Arduino Mega SPI Pins
  MISO - 50
  MOSI - 51
  SCK - 52
  SS - 53
*/
#define chipSelect SS
#define countFiles 255
File myFile;
long positionStartByteCurrentFile = 0;
long positionEndByteCurrentFie = 0;
byte lastFile = 0;
String currentFilename = "ebraille.txt";
boolean initSDCard()
{
  Serial.print("Initializing SD card...");
  //pinMode(chipSelect, OUTPUT);

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect))
  {
    Serial.println("Card failed, or not present");
    return false;
  }
  else
  {
    Serial.println("card initialized.");
    return true;
  }
}

void writeToFile(String nameFile, String dataString, long position)
{
  // make a string for assembling the data to log:
  //String dataString = "Welcome to eBraille. abcdefghijklmnopqrstuvwxyz1234567890";

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open(nameFile, FILE_WRITE);

  // if the file is available, write to it:
  if (myFile)
  {
    Serial.println("Writing to file:...");
    myFile.seek(position);
    Serial.println("Writing position: " + String(myFile.position()));
    myFile.print(dataString);
    myFile.close();
  }
  // if the file isn't open, pop up an error:
  else
  {
    Serial.println("error opening " + nameFile);
  }
}

String readFromFile(String nameFile, long position)
{
  byte countBytesBraille = 0;
  String stringRead = "";
  myFile = SD.open(nameFile);
  if (myFile)
  {
    myFile.seek(position);
    Serial.println(nameFile + ":");

    // read from the file until there's nothing else in it:
    while (myFile.available() && countBytesBraille <= nCharsBraillePage)
    {
      char ch = myFile.read();
      stringRead += String(ch);
      if (isDigit(ch))
      {
        countBytesBraille++;
      }
      countBytesBraille++;
    }

    if (myFile.available())
    {
      positionStartByteCurrentFile = myFile.position();
    }
    else
    {
      positionStartByteCurrentFile = 0;
    }
    // close the file:
    myFile.close();
  }
  else
  {
    // if the file didn't open, print an error:
    Serial.println("error opening " + nameFile);
  }
  return stringRead;
}

byte charBase36ToNum(char chBase36)
{
  if ((chBase36 >= '0') && (chBase36 <= '9'))
  {
    return (chBase36 - '0');
  }
  else if ((chBase36 >= 'A') && (chBase36 <= 'Z'))
  {
    return (chBase36 - 'A') + 10;
  }
  else if ((chBase36 >= 'a') && (chBase36 <= 'z'))
  {
    return (chBase36 - 'a') + 10;
  }
  return 0;
}

boolean checkEBrailleStorage()
{
  if (initSDCard())
  {
    String nameFilesEBraille[] = {
      "ebraille.txt",
      "infofile.txt",
    };
    for (byte i=0; i<sizeof(nameFilesEBraille)/sizeof(String); i++) {
      if (SD.exists(nameFilesEBraille[i])) {
        Serial.println(nameFilesEBraille[i] + " exists.");
      }
      else {
        Serial.println(nameFilesEBraille[i] + " doesn't exists.");
        return false;
      }
    }
    return true;
  }
  else
  {
    return false;
  }
}

long getPositionFromFilename(String nameFile)
{
  long position = 0;
  for (byte i = 3; i <= 6; i++)
  {
    position +=  charBase36ToNum(nameFile.charAt(i)) * round(pow(36, (6-i)));
  }
  return position;
}
