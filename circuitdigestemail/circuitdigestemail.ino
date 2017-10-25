#include<LiquidCrystal.h>
LiquidCrystal lcd(14,15,16,17,18,19);
#include <SoftwareSerial.h>
SoftwareSerial client(0, 1); // RX, TX
boolean No_IP=false;
String IP="";
void check4IP(int t1)
{
  int t2=millis();
  client.flush();
  while(t2+t1>millis())
  {
    while(client.available()>0)
    {
      if(client.find("WIFI GOT IP"))
      {
        No_IP=true;
      }
    }
  }
}
void get_ip()
{
  IP="";
  char ch=0;
  while(1)
  {
    client.println("AT+CIFSR");
    while(client.available()>0)
    {
      if(client.find("STAIP,"))
      {
        delay(1000);
        Serial.print("IP Address:");
        while(client.available()>0)
        {
          ch=client.read();
          if(ch=='+')
          break;
          IP+=ch;
        }
      }
      if(ch=='+')
      break;
    }
    if(ch=='+')
    break;
    delay(1000);
  }
  Serial.print(IP);
  Serial.print("Port:");
  Serial.println(80);
}
void connect_wifi(String cmd, int t)
{
  int temp=0,i=0;
  while(1)
  {
    client.println(cmd); 
    while(client.available())
    {
      if(client.find("OK"))
      i=8;
    }
    delay(t);
    if(i>5)
    break;
    i++;
  }
  if(i==8)
  Serial.println("OK");
  else
  Serial.println("Error");
}
void setup()
{
  client.begin(9600);
     //Serial.begin(9600);
     lcd.begin(16,2);
     lcd.print("Sending Email by");
     lcd.setCursor(0,1);
     lcd.print(" Arduino & WIFI ");
     delay(2000);
     lcd.clear();
     lcd.print("  Circuit Digest  ");
     delay(2000);
     lcd.clear();
     lcd.print("Finding ESP8266");
     connect_wifi("AT",100);
     connect_wifi("ATE1",100);
     lcd.clear();
     lcd.print("Connected");
     delay(1000);
     connect_wifi("AT+CWMODE=3",100);
     connect_wifi("AT+CWQAP",100);  
     connect_wifi("AT+RST",5000);
     lcd.clear();
     lcd.print("Connecting WiFi");
     check4IP(5000);
     if(!No_IP)
     {
        Serial.println("Connecting Wifi....");
        connect_wifi("AT+CWJAP=\"Vodafone-33359967\",\"hpbl6t7v4ii8yxy\"",7000);    //provide your WiFi username and password here
      }
      else
        {
        }
      lcd.clear();
      lcd.print("WIFI Connected...");
      Serial.println("Wifi Connected");
      delay(1000);
      lcd.clear();
      lcd.print("Getting IP Add.");
      Serial.println("Getting IP Address....");
      get_ip();
      delay(1000);
      lcd.clear();
      lcd.print("IP:");
      lcd.print(IP);
      lcd.setCursor(0,1);
      lcd.print("PORT: 80");
      connect_wifi("AT+CIPMUX=1",100);
      connect_wifi("AT+CIPSERVER=1,80",100);
      delay(2000);
      lcd.clear();
      lcd.print("Configuring Email..");
  client.println("AT+CIPSTART=4,\"TCP\",\"mail.smtp2go.com\",2525");
  delay(2000);
  client.println("AT+CIPSEND=4,20");
  delay(2000);
  client.println("EHLO 192.168.1.123");
  delay(2000);
  client.println("AT+CIPSEND=4,12");
  delay(2000);
  lcd.clear();
  lcd.print("Try To Login.....");
  client.println("AUTH LOGIN");
  delay(2000);
  client.println("AT+CIPSEND=4,30");
  delay(2000);
  client.println("ZWRnYXJvYmFyZUBpY2xvdWQuY29t");     //base64 encoded username
  delay(2000);
  client.println("AT+CIPSEND=4,18");
  delay(2000);
  client.println("bkdtLTZ6OS1XN3MtV1lT");                    //base64 encoded password
  lcd.clear();
  lcd.print("Login Success");
  delay(2000);
  client.println("AT+CIPSEND=4,34");
  delay(2000);
  client.println("MAIL FROM:<edgarrobare@gmail.com>");  // use your email address
  delay(2000);  
  client.println("AT+CIPSEND=4,32");
  delay(2000);
  lcd.clear();
  lcd.print("Sending Email 2");
  lcd.setCursor(0,1);
  lcd.print("Saddam4201@ gmail");
  client.println("RCPT To:<saddam4201@ gmail.com>");
  delay(2000);
  client.println("AT+CIPSEND=4,6");
  delay(2000);
  client.println("DATA");
  delay(2000);
  client.println("AT+CIPSEND=4,24");
  delay(2000);
  client.println("Testing Success");
  delay(2000);
  client.println("AT+CIPSEND=4,3");
  delay(2000);
  client.println('.');
  delay(10000);
  client.println("AT+CIPSEND=4,6");
  delay(2000);
  client.println("QUIT");
  delay(2000);
  lcd.clear();
  lcd.print("Email Sent...");
}
void loop()
{
   
}
