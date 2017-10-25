#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8);

const byte rxAddr[6] = "00001";
int ledPin(11);
int delayOn(900);
int delayOff(100);
int msg[1];

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

if (radio.available()){
    
     radio.read(msg, 1);     
      
     Serial.println(msg[0]);
     if (msg[0] == 111){
      Serial.println("Light ON");
      digitalWrite(ledPin,HIGH);
      }
     else {
      Serial.println("Light OFF");
      digitalWrite(ledPin,LOW);
      }
     delay(10);}


}
