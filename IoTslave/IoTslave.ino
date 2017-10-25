#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h> 

int ledPin(13);

 

 
//int angle = 0;   // servo position in degrees 

RF24 radio(7, 8);
const uint64_t pipe = 0xE6E6E6E6E6E6; // Needs to be the same for communicating between 2 NRF24L01 
int ReceivedMessage[1] = {000}; // Used to store value received by the NRF24L01

//Servo myservo;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 
 
//int pos = 0;
 
Servo servoright; 
int pos = 0; 

//    Buzzer 
int speakerOut = 10;//Piezo buzzer's positive terminal is connected to digital pin 10               
byte names[] = {'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C'};  
int tones[] = {1915, 1700, 1519, 1432, 1275, 1136, 1014, 956};
byte melody[] = "2d2a1f2c2d2a2d2c2f2d2a2c2d2a1f2c2d2a2a2g2p8p8p8p";
// count length: 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0
//                                10                  20                  30

int count = 0;
int count2 = 0;
int count3 = 0;
int MAX_COUNT = 24;
int statePin = LOW;
void siren();

// volatile byte intruder;
// intruder = 0;
//    End Buzzer

void setup()
{
  servoright.attach (9); 
  while (!Serial);
  Serial.begin(9600);
  
  radio.begin(); // Start the NRF24L01
  radio.openReadingPipe(1,pipe); // Get NRF24L01 ready to receive
  radio.startListening(); // Listen to see if information received
  
  pinMode(ledPin,OUTPUT);
  //sweeper1.Attach(9);
  //Servo1.attach(servoPin); 
 // myservo.attach(9);  // attaches the servo on pin 9 to the servo object  
}

void loop()
{
  
analogWrite(speakerOut,255);

while (radio.available())
 {
      radio.read(ReceivedMessage, 1); // Read information from the NRF24L01   
      
    
     if (ReceivedMessage[0] == 111){
      Serial.println("RIGHT SWIPE");
      digitalWrite(ledPin,HIGH);

      forward ();
      

//    for(pos = 0; pos < 90; pos += 1)  // goes from 0 degrees to 180 degrees 
//      {                                  // in steps of 1 degree 
//        myservo.write(pos);              // tell servo to go to position in variable 'pos' 
//       delay(15);                       // waits 15ms for the servo to reach the position 
//  } 
  
      }
      
     if (ReceivedMessage[0] == 000) {
      Serial.println("LEFT SWIPE");
      digitalWrite(ledPin,LOW);

      reverse (); 

//    for(pos = 90; pos>=1; pos-=1)     // goes from 180 degrees to 0 degrees 
//      {                                
//        myservo.write(pos);              // tell servo to go to position in variable 'pos' 
//        delay(15);                       // waits 15ms for the servo to reach the position 
//  } 
      
      }

      if (ReceivedMessage[0] == 149) { 
        Serial.println("Intruder detected: Motion");
        
       // intruder++;
        for(int i=0; i<1; i++)   //Play the alarm # of times
        siren();

      }
      }
     //delay(10);

     
     }

  


void forward () 
{ 
 Serial.println("FORWARD");
servoright.write (180);
delay (1000);
stopRobot (); 
} 
void reverse () 
{ 
Serial.println("REVERSE");
servoright.write (0);
delay (1000);
stopRobot () ; 
}
void stopRobot () 
{ 
Serial.println("STOP");
servoright.write (90); 
//delay (3000); 
}

void siren()     //This function will make the alarm sound using the piezo buzzer
{
for (count = 0; count < MAX_COUNT; count++) {
      for (count3 = 0; count3 <= (melody[count*2] - 48) * 30; count3++) {
      for (count2=0;count2<8;count2++) {
        if (names[count2] == melody[count*2 + 1]) {       
          analogWrite(speakerOut,1023);
          delayMicroseconds(tones[count2]);
          analogWrite(speakerOut, 0);
          delayMicroseconds(tones[count2]);
        } 
        if (melody[count*2 + 1] == 'p') {
          // make a pause of a certain size
          analogWrite(speakerOut, 0);
          delayMicroseconds(100);
          
        }
      }
    }
  }
}


