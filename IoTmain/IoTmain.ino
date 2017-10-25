#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h> // Must include Wire library for I2C
int ledPin(13);

//int SentMessage[1] = {000}; // Used to store value before being sent through the NRF24L01

//    Gesture Sensor 
#include <Wire.h>
#include <ZX_Sensor.h>

// Constants
const int ZX_ADDR = 0x10;    // ZX Sensor I2C address
const int INTERRUPT_NUM = 0; // Pin 2 on the UNO

// Global Variables
ZX_Sensor zx_sensor = ZX_Sensor(ZX_ADDR);
volatile GestureType gesture;
volatile bool interrupt_flag;
uint8_t gesture_speed;

//    End Gesture Sensor

// Nrf24

RF24 radio(7, 8); // NRF24L01 used SPI pins + Pin 7 and 8 on the Mega
const uint64_t pipe = 0xE6E6E6E6E6E6; // Needs to be the same for communicating between 2 NRF24L01
int SentMessage[1] = {000}; // Used to store value before being sent through the NRF24L01

// End nrf24

// Motion
 volatile byte intruder;

 //End Motion

 void setup()
 {
  
// nrf
   radio.begin(); // Start the NRF24L01
   radio.openWritingPipe(pipe); // Get NRF24L01 ready to transmit
   radio.setRetries(15, 15);

// nrf

    // declare the ledPin as an OUTPUT:
   pinMode(ledPin, OUTPUT);

// Motion   
   Serial.begin(9600);
   attachInterrupt(1, intruder_detect, RISING);//Initialize the interrupt pin for the motion sensor (Arduino digital pin 2)
   intruder = 0;   
// End Motion  
  
//  Gesture Sensor

   uint8_t ver;
  // Initialize gesture to no gesture
  gesture = NO_GESTURE;
  // Initialize Serial port
  Serial.begin(9600);
  Serial.println();
  Serial.println("---------------------------------------------");
  Serial.println("SparkFun/GestureSense - I2C Gesture Interrupt");
  Serial.println("Note: higher 'speed' numbers mean slower");
  Serial.println("---------------------------------------------");
  
  // Initialize ZX Sensor (configure I2C and read model ID)
  if ( zx_sensor.init(GESTURE_INTERRUPTS) ) {
    Serial.println("ZX Sensor initialization complete");
  } else {
    Serial.println("Something went wrong during ZX Sensor init!");
  }
  
  // Read the model version number and ensure the library will work
  ver = zx_sensor.getModelVersion();
  if ( ver == ZX_ERROR ) {
    Serial.println("Error reading model version number");
  } else {
    Serial.print("Model version: ");
    Serial.println(ver);
  }
  if ( ver != ZX_MODEL_VER ) {
    Serial.print("Model version needs to be ");
    Serial.print(ZX_MODEL_VER);
    Serial.print(" to work with this library. Stopping.");
    while(1);
  }
  
  // Read the register map version and ensure the library will work
  ver = zx_sensor.getRegMapVersion();
  if ( ver == ZX_ERROR ) {
    Serial.println("Error reading register map version number");
  } else {
    Serial.print("Register Map Version: ");
    Serial.println(ver);
  }
  if ( ver != ZX_REG_MAP_VER ) {
    Serial.print("Register map version needs to be ");
    Serial.print(ZX_REG_MAP_VER);
    Serial.print(" to work with this library. Stopping.");
    while(1);
  }
  
  // Initialize interrupt service routine
  interrupt_flag = false;
  zx_sensor.clearInterrupt();
  attachInterrupt(INTERRUPT_NUM, interruptRoutine, RISING);
  Serial.println("Interrupts now configured. Gesture away!");
//  End Gensture Sensor
   
 }
 
 void loop()
 {
  
   //analogWrite(speakerOut,255);
   gestureSensor();
  
 }
 
 void intruder_detect()   //This function is called whenever an intruder is detected by the arduino
 {
   Serial.println("Intruder detected: Motion");

   SentMessage[0] = 149; //Send the 333 to the reciever 
   radio.write(SentMessage, 1); // Send value through NRF24L01
   
   Serial.println("Sent signal: Motion");
   
   intruder++;
//   for(int i=0; i<1; i++)   //Play the alarm # of times
//   siren();
 }
 
 
void gestureSensor() {
  
  // If we have an interrupt, read and print the gesture
  if ( interrupt_flag ) {
    
    // Clear the interrupt flag
    interrupt_flag = false;
    
    // You MUST read the STATUS register to clear interrupt!
    zx_sensor.clearInterrupt();
    
    // Read last gesture
    gesture = zx_sensor.readGesture();
    gesture_speed = zx_sensor.readGestureSpeed();
    switch ( gesture ) {
      case NO_GESTURE:
        Serial.println("No Gesture");
        break;
        
      case RIGHT_SWIPE:
        Serial.print("Right Swipe. Speed: ");
        Serial.println(gesture_speed, DEC);
        
        SentMessage[0] = 111; //Send the 111 to the reciever 
        radio.write(SentMessage, 1); // Send value through NRF24L01
        
        break;
        
      case LEFT_SWIPE:
        Serial.print("Left Swipe. Speed: ");
        Serial.println(gesture_speed, DEC);

        SentMessage[0] = 000;
        radio.write(SentMessage, 1);
        
        break;
        
      case UP_SWIPE:
        Serial.print("Up Swipe. Speed: ");
        Serial.println(gesture_speed, DEC);
        
        SentMessage[0] = 222;
        radio.write(SentMessage, 1);
        
        break;
        
      default:
        break;
    }
  }
}

void interruptRoutine() {
  interrupt_flag = true;
}

