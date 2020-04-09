#include <SPI.h>
#include <SD.h>

/*
Arduino Mega SPI Pins
MISO - 50
MOSI - 51
SCK - 52
SS - 53
*/

const int chipSelect = 53;
File myFile;
void setup()
{
    // Open serial communications and wait for port to open:
    Serial.begin(9600);
    while (!Serial)
    {
        ; // wait for serial port to connect. Needed for native USB port only
    }

    Serial.print("Initializing SD card...");
    //pinMode(chipSelect, OUTPUT);

    // see if the card is present and can be initialized:
    if (!SD.begin(chipSelect))
    {
        Serial.println("Card failed, or not present");
        // don't do anything more:
        while (1)
            ;
    }
    Serial.println("card initialized.");
    String nameFileWelcome = "welcome.txt";
    String messageWelcome = "Welcome to eBraille. abcdefghijklmnopqrstuvwxyz1234567890";
    SD.remove(nameFileWelcome);

    delay(1000);

    if (SD.exists(nameFileWelcome))
    {
        Serial.println(nameFileWelcome + " exists.");
        Serial.println(readFromFile(nameFileWelcome));
    }
    else
    {
        Serial.println(nameFileWelcome + " doesn't exist.");
        Serial.println("Creating file: " + nameFileWelcome + "...");
        writeToFile(nameFileWelcome, messageWelcome);
        Serial.println(readFromFile(nameFileWelcome));
    }
}

void loop()
{
}

void writeToFile(String nameFile, String dataString)
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
        myFile.println(dataString);
        myFile.close();

    }
    // if the file isn't open, pop up an error:
    else
    {
        Serial.println("error opening " + nameFile);
    }
}

String readFromFile(String nameFile)
{
    String stringRead = "";
    myFile = SD.open(nameFile);
    if (myFile)
    {
        Serial.println(nameFile + ":");

        // read from the file until there's nothing else in it:
        while (myFile.available())
        {
            stringRead += String(char(myFile.read()));
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