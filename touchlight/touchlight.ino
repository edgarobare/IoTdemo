
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_CAP1188.h>
#define CAP1188_RESET  9
#define CAP1188_CS  10
#define CAP1188_MOSI  11
#define CAP1188_MISO  12
#define CAP1188_CLK  13

int ledPin(5);
int ledPin1(8);
int ledPin2(7);
int ledPin3(6);

Adafruit_CAP1188 cap = Adafruit_CAP1188();

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
  Serial.println("Touch Light");

// Initialize the sensor, if using i2c you can pass in the i2c address
  // if (!cap.begin(0x28)) {
  if (!cap.begin()) {
    Serial.println("CAP1188 not found");
    while (1);
  }
  Serial.println("CAP1188 found!");
   pinMode(ledPin,OUTPUT);
   pinMode(ledPin1,OUTPUT);
   pinMode(ledPin2,OUTPUT);
   pinMode(ledPin3,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
   uint8_t touched = cap.touched();

  if (touched == 0) {
    // No touch detected
    return;
  }
  
  for (uint8_t i=0; i<8; i++) {
    if (touched & (1 << i)) {
      Serial.print("C"); Serial.print(i+1); Serial.print("\t");
      digitalWrite(ledPin,HIGH);
      digitalWrite(ledPin1,HIGH);
      digitalWrite(ledPin2,HIGH);
      digitalWrite(ledPin3,HIGH);
      
    }
    else {
    
      digitalWrite(ledPin,LOW);
      digitalWrite(ledPin1,LOW);
      digitalWrite(ledPin2,LOW);
      digitalWrite(ledPin3,LOW);
    }
    
  }
  Serial.println();
  delay(50);

}
