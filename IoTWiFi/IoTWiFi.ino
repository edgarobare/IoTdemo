#include <ESP8266WiFi.h>  // the ESP8266WiFi.h  lib

const char* SSID = "AMdesignoffice";
const char* PASS = "viamontetondo1";
char server[] = "mail.smtpcorp.com";
ADC_MODE(ADC_VCC);

WiFiClient client;

//    Photoresistor
int sensorPin = 4;    // select the input pin for the potentiometer
int ledPin = 5;      // select the pin for the LED
int sensorValue = 0;  // variable to store the value coming from the sensor
//    End Photoresistor

 void setup()
 {
    // declare the ledPin as an OUTPUT:
   pinMode(ledPin, OUTPUT);
  


  //email
  Serial.begin(115200);
  delay(10);
  Serial.println("");
  Serial.println("");
  Serial.print("Connecting To ");
  Serial.println(SSID);
  WiFi.begin(SSID, PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  //End Email

 }
 
void loop()
 {
  
   lightsense();


 }

  void lightsense() {
  // read the value from the sensor:
  sensorValue = analogRead(sensorPin);
  
  if (sensorValue > 512) {
  Serial.println("Intruder detected: light");
  // turn the ledPin on
  digitalWrite(ledPin, HIGH);
  // stop the program for <sensorValue> milliseconds:
  delay(sensorValue);
  byte ret = sendEmail();
 // siren();
  }
  
  else {
  // turn the ledPin off:
  Serial.println("Ready !!!");
  digitalWrite(ledPin, LOW);
  // stop the program for for <sensorValue> milliseconds:
  delay(sensorValue); 
  }
  
}

//  Email
byte sendEmail()
{
  byte thisByte = 0;
  byte respCode;

  if (client.connect(server, 2525) == 1) {
    Serial.println(F("connected"));
  } else {
    Serial.println(F("connection failed"));
    return 0;
  }
  if (!eRcv()) return 0;

  Serial.println(F("Sending EHLO"));
  client.println("EHLO www.example.com");
  if (!eRcv()) return 0;
  Serial.println(F("Sending auth login"));
  client.println("auth login");
  if (!eRcv()) return 0;
  Serial.println(F("Sending User"));
  // Change to your base64, ASCII encoded user
  client.println("ZWRnYXJvYmFyZUBpY2xvdWQuY29t"); // SMTP UserID
  if (!eRcv()) return 0;
  Serial.println(F("Sending Password"));
  // change to your base64, ASCII encoded password
  client.println("bkdtLTZ6OS1XN3MtV1lT");//  SMTP Passw
     if (!eRcv()) return 0;
    Serial.println(F("Sending From"));   // change to your email address (sender)
   client.println(F("MAIL From: hello@amdesignoffice.com"));// not important 
   if (!eRcv()) return 0;   // change to recipient address
    Serial.println(F("Sending To"));
    client.println(F("RCPT To: simonesimonelli@gmail.com"));
    if (!eRcv()) return 0;
    Serial.println(F("Sending DATA"));
    client.println(F("DATA"));
    if (!eRcv()) return 0;
    Serial.println(F("Sending email"));   // change to recipient address
   client.println(F("To: simonesimonelli@gmail.com"));   // change to your address
   client.println(F("From: hello@amdesignoffice.com"));
 client.println(F("Subject: Hello\r\n"));
    client.print(F("The notification was sent successfully!"));
    client.print(F(" "));
    client.print(F("Hello, from AM Design Office"));

    client.print(F("Power is: "));
    client.print(ESP.getVcc());
    client.println(F("mV"));
    client.print(F("Device Chip ID: "));
    client.println(ESP.getChipId());
    Serial.print(F("Voltage is: "));
    Serial.print(ESP.getVcc());
    client.println(F("."));
    
    if (!eRcv()) return 0;
    Serial.println(F("Sending QUIT"));
    client.println(F("QUIT"));
    if (!eRcv()) return 0;
    client.stop();
    Serial.println(F("disconnected"));
    return 1;
  }
  byte eRcv()
  {
    byte respCode;
    byte thisByte;
    int loopCount = 0;
    while (!client.available())
  {
      delay(1);
      loopCount++;     // if nothing received for 10 seconds, timeout
      if (loopCount > 10000) {
      client.stop();
      Serial.println(F("\r\nTimeout"));
      return 0;
    }
  }

  respCode = client.peek();
  while (client.available())
  {
    thisByte = client.read();
    Serial.write(thisByte);
  }

  if (respCode >= '4')
  {
    //  efail();
    return 0;
  }
  return 1;
}

// End Email

