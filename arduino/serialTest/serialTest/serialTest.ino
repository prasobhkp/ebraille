String inputString = "";         // a String to hold incoming data
boolean stringComplete = false;  // whether the string is complete
boolean t = false;
#define ledPin 13

void setup() {
  // initialize serial:
  Serial.begin(115200);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  serialEvent();// print the string when a newline arrives:

  if (stringComplete) {
    Serial.println(inputString);
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}

void serialEvent() {
  while (Serial.available()) {
  digitalWrite(ledPin, t);
  t = !t;
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == 0x0D) {
      stringComplete = true;
    }
  }

}
