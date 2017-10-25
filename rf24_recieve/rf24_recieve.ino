#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8);

const byte rxAddr[6] = "00001";
int ledPin(13);
int delayOn(500);
int delayOff(200);


void setup()
{
  while (!Serial);
  Serial.begin(9600);
  
  radio.begin();
  radio.openReadingPipe(0, rxAddr);
  pinMode(ledPin,OUTPUT);
  radio.startListening();
}

void loop()
{
  if (radio.available())
  {
    char text[32] = {0};
    radio.read(&text, sizeof(text));
    
    Serial.println(text);
    digitalWrite(ledPin,HIGH);
    delay(delayOn);
    digitalWrite(ledPin,LOW);
    delay(delayOff);
  }
}
