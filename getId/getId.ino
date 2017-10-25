/*******************************************
* function:get the id of RFID key
* RFID SunFounder Uno
* VCC 3.3V
* RST 2
* GND GND
* MISO 3
* MOSI 4
* SCK 5
* NSS 6
* IRQ 7
****************************************/
#include"rfid1.h"
RFID1 rfid;//create a variable type of RFID1
uchar serNum[5]; // array to store your ID
void setup()
{
Serial.begin(9600); //initialize the serial
rfid.begin(7, 5, 4, 3, 6, 2); ////rfid.begin(IRQ_PIN,SCK_PIN,MOSI_PIN,MISO_PIN,NSS_PIN,RST_PIN)
delay(100);//delay 1s
rfid.init(); //initialize the RFID
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
Serial.print("The card's number is: ");
memcpy(serNum, str, 5);
rfid.showCardID(serNum);//show the card ID
Serial.println();
Serial.println();
}
delay(500);
rfid.halt(); //command the card into sleep mode 
}
