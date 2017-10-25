#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h> // Must include Wire library for I2C
#include <SFE_MMA8452Q.h> // Includes the SFE_MMA8452Q library

MMA8452Q accel;

RF24 radio(7, 8);

const byte rxAddr[6] = "00001";
int msg[1];

void setup()
{
  radio.begin();
  radio.setRetries(15, 15);
  radio.openWritingPipe(rxAddr);
  
  radio.stopListening();

  accel.init();
}

void loop()
{
   if (accel.available())
  {
    accel.read();
    orientation();
    
  }
  
  delay(1000);
}

void orientation()
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
      
      msg[0] = 111; //Send the 111 to the reciever 
      radio.write(msg, 1);
      
    break;
  case PORTRAIT_D:
    Serial.print("Portrait Down");  
 
    msg[0] = 100; //Send number to the reciever 
    radio.write(msg, 1);
    break;
  case LANDSCAPE_R:
    Serial.print("Landscape Right");

    msg[0] = 100; //Send number to the reciever 
    radio.write(msg, 1);
    break;
  case LANDSCAPE_L:
    Serial.print("Landscape Left");

    msg[0] = 100; //Send number to the reciever 
    radio.write(msg, 1);
    break;
  case LOCKOUT:
    Serial.print("Flat");

    msg[0] = 111; //Send the 111 to the reciever 
    radio.write(msg, 1);
    break;
  }

}

