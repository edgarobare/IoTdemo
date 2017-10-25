#include <Servo.h> 


Servo servoright; 
int pos = 0; 

void setup()
{ 
// servoleft.write (0); 
servoright.attach (9); 
// servoright.write (0);
Serial.begin(9600); 
} 


void loop()

{  

forward (); 
delay (1000);
stopRobot ();  
reverse (); 
delay (1000); 
//turnRight (); 
//delay (1000); 
// trunLeft (); 
// delay (1000); 
//turnLeft (); 
//delay (1000); 
// turnRight (); 
// delay (1000); 
stopRobot (); 
// delay (2000); 

} 

void forward () 
{ 
 Serial.println("FORWARD");
servoright.write (180); 
} 

void reverse () 
{ 
Serial.println("REVERSE");
servoright.write (0); 
} 
void turnRight () 
{ 
Serial.println("RIGHT");
servoright.write (180); 
} 
void turnLeft () 
{ 
Serial.println("LEFT");
servoright.write (0); 
} 
void stopRobot () 
{ 
Serial.println("STOP");
servoright.write (90); 
delay (3000); 
}
