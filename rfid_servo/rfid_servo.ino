#include"rfid1.h"
#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position


  RFID1 rfid; ////create a variable type of RFID

  uchar serNum[5]; // array to store your ID
void setup()
{
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
    Serial.begin(9600);
  rfid.begin(7, 5, 4, 3, 6, 2);//rfid.begin(IRQ_PIN,SCK_PIN,MOSI_PIN,MISO_PIN,NSS_PIN,RST_PIN)
    delay(100); 
  rfid.init(); //initialize the RFID

//Serial.begin(9600);

Serial.println(" Welcome! "); //print" Welcome! "
delay(2000);//delay 2s
}
void loop()
{
  uchar status;
  uchar str[MAX_LEN];
// Search card, return card types
  status = rfid.request(PICC_REQIDL, str);
if (status != MI_OK)
{
  return;
}
// Show card type
  rfid.showCardType(str);
//Prevent conflict, return the 4 bytes Serial number of the card
  status = rfid.anticoll(str);
if (status == MI_OK)
{
  
    Serial.print("Card Number: ");

//Serial.println(" ID: ");
  memcpy(serNum, str, 5);
  rfid.showCardID(serNum);//show the card ID
// Serial.println();
// Check people associated with card ID
  uchar* id = serNum;
 
//if the card id is E6F2888D,then relay connect 
  if (id[0]==0xE6 && id[1]==0xF2 && id[2]==0x88 && id[3]==0x8D) 
{
    Serial.println(" ");
    Serial.println("Hello AMdesign");
    
 for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to n degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }

} 
else
{
Serial.println(" ");
Serial.println("Hello unkown guy!");

}
}
Serial.println(" ");
Serial.println(" Welcome! ");
delay(2000);
rfid.halt(); //command the card into sleep mode 
}
