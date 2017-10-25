#include <Wire.h> // Must include Wire library for I2C
#include <SFE_MMA8452Q.h> // Includes the SFE_MMA8452Q library

int ledPin(13);
int ledPin1(12);

MMA8452Q accel;

void setup()
{
  Serial.begin(9600);
  Serial.println("Test Motion!");

  accel.init();
  pinMode(ledPin,OUTPUT);
  pinMode(ledPin1,OUTPUT);

}

void loop() {

  if (accel.available())
  {
    accel.read();
    printOrientation();
    
   Serial.println(); // Print new line every time.
  }
}
  void printOrientation()
{
  // accel.readPL() will return a byte containing information
  // about the orientation of the sensor. It will be either
  // PORTRAIT_U, PORTRAIT_D, LANDSCAPE_R, LANDSCAPE_L, or
  // LOCKOUT.
  byte pl = accel.readPL();
  switch (pl)
  {
  case PORTRAIT_U:
    Serial.print("Portrait Up");
    digitalWrite(ledPin,HIGH);
    digitalWrite(ledPin1,LOW);
    break;
  case PORTRAIT_D:
    Serial.print("Portrait Down");
    digitalWrite(ledPin,LOW);
    digitalWrite(ledPin1,HIGH);
    break;
  case LANDSCAPE_R:
    Serial.print("Landscape Right");
    break;
  case LANDSCAPE_L:
    Serial.print("Landscape Left");
    break;
  case LOCKOUT:
    Serial.print("Flat");
    break;
  }
}

