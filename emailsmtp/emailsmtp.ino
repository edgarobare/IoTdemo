 /*
   Email client sketch for IDE v1.0.1 and w5100/w5200
   Posted December 2012 by SurferTim
*/

#include <SPI.h>
#include <Ethernet.h>

// this must be unique
byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };  
// change network settings to yours
IPAddress ip(10, 69, 69, 30);   
IPAddress gateway( 10, 69, 69, 100 );
IPAddress subnet( 255, 255, 255, 0 );

char server[] = "smtpcorp.com";
int port = 2525;

EthernetClient client;

void setup()
{
  Serial.begin(9600);
  pinMode(4,OUTPUT);
  digitalWrite(4,HIGH);
  Ethernet.begin(mac, ip, gateway, gateway, subnet); 
  delay(2000);
  Serial.println(F("Ready. Press 'e' to send."));
}

void loop()
{
  byte inChar;

  inChar = Serial.read();

  if(inChar == 'e')
  {
      if(sendEmail()) Serial.println(F("Email sent"));
      else Serial.println(F("Email failed"));
  }
}

byte sendEmail()
{
  byte thisByte = 0;
  byte respCode;

  if(client.connect(server,port) == 1) {
    Serial.println(F("connected"));
  } else {
    Serial.println(F("connection failed"));
    return 0;
  }

  if(!eRcv()) return 0;

  Serial.println(F("Sending hello"));
// replace 1.2.3.4 with your Arduino's ip
  client.println("EHLO 10.69.69.30");
  if(!eRcv()) return 0;

  Serial.println(F("Sending auth login"));
  client.println("auth login");
  if(!eRcv()) return 0;

  Serial.println(F("Sending User"));
// Change to your base64 encoded user
  client.println("ZWRnYXJvYmFyZUBpY2xvdWQuY29t");

  if(!eRcv()) return 0;

  Serial.println(F("Sending Password"));
// change to your base64 encoded password
  client.println("bkdtLTZ6OS1XN3MtV1lT");

  if(!eRcv()) return 0;

// change to your email address (sender)
  Serial.println(F("Sending From"));
  client.println("MAIL From: <edgarrobare@gmail.com>");
  if(!eRcv()) return 0;

// change to recipient address
  Serial.println(F("Sending To"));
  client.println("RCPT To: <edgarobare@icloud.com>");
  if(!eRcv()) return 0;

  Serial.println(F("Sending DATA"));
  client.println("DATA");
  if(!eRcv()) return 0;

  Serial.println(F("Sending email"));

// change to recipient address
  client.println("To: You <edgarobare@icloud.com>");

// change to your address
  client.println("From: Me <edgarrobare@gmail.comm>");

  client.println("Subject: Arduino email test\r\n");

  client.println("This is from my Arduino!");

  client.println(".");

  if(!eRcv()) return 0;

  Serial.println(F("Sending QUIT"));
  client.println("QUIT");
  if(!eRcv()) return 0;

  client.stop();

  Serial.println(F("disconnected"));

  return 1;
}

byte eRcv()
{
  byte respCode;
  byte thisByte;
  int loopCount = 0;

  while(!client.available()) {
    delay(1);
    loopCount++;

    // if nothing received for 10 seconds, timeout
    if(loopCount > 10000) {
      client.stop();
      Serial.println(F("\r\nTimeout"));
      return 0;
    }
  }

  respCode = client.peek();

  while(client.available())
  {  
    thisByte = client.read();    
    Serial.write(thisByte);
  }

  if(respCode >= '4')
  {
    efail();
    return 0;  
  }

  return 1;
}


void efail()
{
  byte thisByte = 0;
  int loopCount = 0;

  client.println(F("QUIT"));

  while(!client.available()) {
    delay(1);
    loopCount++;

    // if nothing received for 10 seconds, timeout
    if(loopCount > 10000) {
      client.stop();
      Serial.println(F("\r\nTimeout"));
      return;
    }
  }

  while(client.available())
  {  
    thisByte = client.read();    
    Serial.write(thisByte);
  }

  client.stop();

  Serial.println(F("disconnected"));
}
