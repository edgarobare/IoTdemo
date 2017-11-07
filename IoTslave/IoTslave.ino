#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h> 


// buzzer

#define  c3    7634
#define  d3    6803
#define  e3    6061
#define  f3    5714
#define  g3    5102
#define  a3    4545
#define  b3    4049
#define  c4    3816    // 261 Hz 
#define  d4    3401    // 294 Hz 
#define  e4    3030    // 329 Hz 
#define  f4    2865    // 349 Hz 
#define  g4    2551    // 392 Hz 
#define  a4    2272    // 440 Hz 
#define  a4s   2146
#define  b4    2028    // 493 Hz 
#define  c5    1912    // 523 Hz
#define  d5    1706
#define  d5s   1608
#define  e5    1517    // 659 Hz
#define  f5    1433    // 698 Hz
#define  g5    1276
#define  a5    1136
#define  a5s   1073
#define  b5    1012
#define  c6    955
 
#define  R     0      // Define a special note, 'R', to represent a rest
 
 
// SETUP //
 
int speakerOut = 9;    // Set up speaker on digital pin 


// End buzzer 

 
//int angle = 0;   // servo position in degrees 

RF24 radio(7, 8);
const uint64_t pipe = 0xE6E6E6E6E6E6; // Needs to be the same for communicating between 2 NRF24L01 
int ReceivedMessage[1] = {000}; // Used to store value received by the NRF24L01

Servo myservo;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 
 
int pos = 0;
 
Servo servoright; 


void setup()
{
  servoright.attach (10); 
  while (!Serial);
  Serial.begin(9600);
  
  radio.begin(); // Start the NRF24L01
  radio.openReadingPipe(1,pipe); // Get NRF24L01 ready to receive
  radio.startListening(); // Listen to see if information received

  pinMode(speakerOut, OUTPUT);
   
}

// Buzzer

// MELODIES and TIMING //
//  melody[] is an array of notes, accompanied by beats[], 
//  which sets each note's relative length (higher #, longer note) 
 
// Melody 1: Star Wars Imperial March
//int melody1[] = {  a4, R,  a4, R,  a4, R,  f4, R, c5, R,  a4, R,  f4, R, c5, R, a4, R,  e5, R,  e5, R,  e5, R,  f5, R, c5, R,  g5, R,  f5, R,  c5, R, a4, R};
//int beats1[]  = {  50, 20, 50, 20, 50, 20, 40, 5, 20, 5,  60, 10, 40, 5, 20, 5, 60, 80, 50, 20, 50, 20, 50, 20, 40, 5, 20, 5,  60, 10, 40, 5,  20, 5, 60, 40};

int melody1[] = {  a4, R,  a4, R,  a4, R};
int beats1[]  = {  50, 20, 50, 20, 50, 20};
 
// Melody 2: Star Wars Theme
int melody2[] = {  f4,  f4, f4,  a4s,   f5,  d5s,  d5,  c5, a5s, f5, d5s,  d5,  c5, a5s, f5, d5s, d5, d5s,   c5};
int beats2[]  = {  21,  21, 21,  128,  128,   21,  21,  21, 128, 64,  21,  21,  21, 128, 64,  21, 21,  21, 128 }; 
 
//int MAX_COUNT1 = sizeof(melody1) / 2; // Melody length, for looping.

int MAX_COUNT1 =1; // Melody length, for looping.
int MAX_COUNT2 = sizeof(melody2) / 2; // Melody length, for looping.
 
long tempo = 10000; // Set overall tempo
 
int pause = 1000; // Set length of pause between notes
 
int rest_count = 50; // Loop variable to increase Rest length (BLETCHEROUS HACK; See NOTES)
 
// Initialize core variables
int toneM = 0;
int beat = 0;
long duration  = 0;
//int potVal = 0;
 
// PLAY TONE  //
// Pulse the speaker to play a tone for a particular duration

void playTone() {
  long elapsed_time = 0;
  if (toneM > 0) { // if this isn't a Rest beat, while the tone has 
    //  played less long than 'duration', pulse speaker HIGH and LOW
    while (elapsed_time < duration) {
 
      digitalWrite(speakerOut,HIGH);
      delayMicroseconds(toneM / 2);
 
      // DOWN
      digitalWrite(speakerOut, LOW);
      delayMicroseconds(toneM / 2);
 
      // Keep track of how long we pulsed
      elapsed_time += (toneM);
    } 
  }
  else { // Rest beat; loop times delay
    for (int j = 0; j < rest_count; j++) { // See NOTE on rest_count
      delayMicroseconds(duration);  
    }                                
  }                                 
}

// End Buzzer

void loop()
{
  
analogWrite(speakerOut,255);

while (radio.available())
 {
      radio.read(ReceivedMessage, 1); // Read information from the NRF24L01   
      
    
     if (ReceivedMessage[0] == 111){
      Serial.println("RIGHT SWIPE");

      forward ();
  
      }
      
     if (ReceivedMessage[0] == 000) {
      Serial.println("LEFT SWIPE");
         
      reverse (); 
      
      }

      if (ReceivedMessage[0] == 222) { 
        Serial.println("Intruder detected: Motion");
        Serial.println("STAR WARS");
        //delay (10);
        
        int ring = 0;
        ring++;
        if (ring = 4) {
          
         siren();
                      }
        
        
      }
      }

    
     }

  


void forward () 
{ 

servoright.write (180);
delay (1000);
 
} 
void reverse () 
{ 

servoright.write (0);
delay (1000);

}

void siren()//This function will make the alarm sound using the piezo buzzer
{
  
  // Set up a counter to pull from melody1[] and beats1[]
  for (int i=0; i<MAX_COUNT1; i++) {
    toneM = melody1[i];
    beat = beats1[i];
 
    duration = beat * tempo; // Set up timing
 
    playTone(); // A pause between notes
    delayMicroseconds(pause);
    }
 
    
    analogWrite(speakerOut,255);
    
}

