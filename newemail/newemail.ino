////Adapted from SurferTim's Ethernet email program by Ty Tower May 2015
//Register at smtp2go.com --easy. Encode user and password for smtp2go.com in base 64 utf8--go here "https://www.base64encode.org/".
//Put those in the right places in code and change the IP details to yours
//Put in where you want to send the email and who it is from.
//Change the mac address to the one from your ESP8266 module
////#include
//#include <SPI.h>
#include <WiFi.h>

const char* ssid = "Vodafone-33359967";
const char* password = "hpbl6t7v4ii8yxy"; // command to connect to router and get IP: AT+CWJAP="Vodafone-33359967","hpbl6t7v4ii8yxy"

// Create an instance of the server
// specify the port to listen on as an argument
//WiFiServer server(80);
WiFiClient client;
// this must be unique put yours in here
byte mac[] = { 0x5c, 0xcf, 0x7f, 0x93 ,0xf6, 0xd0 }; //from ESP8266 The command: AT+CIPSTAMAC?

// change network settings to yours
// command to connect to router and get IP: AT+CWJAP="Vodafone-33359967","hpbl6t7v4ii8yxy"
// Get Ip command: AT+CIPSTA?

IPAddress ip(10, 69, 69, 19);
IPAddress gateway(10, 69, 69, 100);
IPAddress subnet(255, 255, 255, 0);
//IPAddress server(?,?,?,?); // numeric IP for smtpcorp.com (no DNS)

char server[] = "smtpcorp.com";
int port = 2525;

void setup()
{
Serial.begin(115200);
pinMode(13,OUTPUT);
digitalWrite(13,HIGH);
WiFi.config(mac, ip, gateway, subnet); 
//server.begin();
delay(10000);
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
  client.println("10.69.69.19");
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
