#include <MsTimer2.h>
#include <Servo.h>

#define led LED_BUILTIN
#define servoAPin 8
Servo servoA;

void blinkLED(void)
{
  static boolean output = HIGH;

  digitalWrite(led, output);
  output = !output;
  Serial.println(millis()/1000);
}

void setup() {
  pinMode(led, OUTPUT);
  MsTimer2::set(1000, blinkLED); // 500ms period
  MsTimer2::start();
  Serial.begin(9600);
  servoA.attach(servoAPin);
}

void loop() {
  servoA.write(0);
  delay(2000);
  servoA.write(90);
  delay(2000);
  servoA.write(180);
  delay(2000);
}
