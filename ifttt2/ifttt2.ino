/*
  Web Server

 A simple web server that shows the value of the analog input pins.
 using an Arduino Wiznet Ethernet shield.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)

 */

#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 }; 

IPAddress ip(10, 69, 69, 30);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

void send_event(const char *event);

// IFTTT setup
const char *host = "http://maker.ifttt.com";
const char *privateKey = "dviaPIZNIyQ0ggM-Gmnd8y";

// Hardware setup
const int buttonPin = 2;     // the number of the pushbutton pin
const int ledPin = 3;        // the number of the LED pin

// Variables will change:
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

//EthernetClient client;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());

  
}


void loop() {
//  // listen for incoming clients
//  EthernetClient client = server.available();
//  if (client) {
//    Serial.println("new client");
//    // an http request ends with a blank line
//    boolean currentLineIsBlank = true;
//    while (client.connected()) {
//      if (client.available()) {
//        char c = client.read();
//        Serial.write(c);
//        // if you've gotten to the end of the line (received a newline
//        // character) and the line is blank, the http request has ended,
//        // so you can send a reply
//        if (c == '\n' && currentLineIsBlank) {
//          // send a standard http response header
//          client.println("HTTP/1.1 200 OK");
//          client.println("Content-Type: text/html");
//          client.println("Connection: close");  // the connection will be closed after completion of the response
//          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
//          client.println();
//          client.println("<!DOCTYPE HTML>");
//          client.println("<html>");
//          // output the value of each analog input pin
//          for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
//            int sensorReading = analogRead(analogChannel);
//            client.print("analog input ");
//            client.print(analogChannel);
//            client.print(" is ");
//            client.print(sensorReading);
//            client.println("<br />");
//          }
//          client.println("</html>");
//          break;
//        }
//        if (c == '\n') {
//          // you're starting a new line
//          currentLineIsBlank = true;
//        } else if (c != '\r') {
//          // you've gotten a character on the current line
//          currentLineIsBlank = false;
//        }
//      }
//    }
//    // give the web browser time to receive the data
//    delay(1);
//    // close the connection:
//    client.stop();
//    Serial.println("client disconnected");
//  }

  ////extra////////////////////////////////////////

  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) 
  {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) 
    {
      Serial.print("Button now ");
      Serial.println(HIGH == reading ? "HIGH" : "LOW");
      buttonState = reading;

      // When the button is in the LOW state (pulled high) the button 
      // has been pressed so send the event.
      if (buttonState == LOW) {
        send_event("button_pressed");
      }
    }
  }

  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonState = reading;
}

void send_event(const char *event)
{
  // set the LED on whle we are sending the event
  digitalWrite(ledPin, HIGH);

  Serial.print("Connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  
  EthernetClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("Connection failed");
    return;
  }
  
  // We now create a URI for the request
  String url = "/trigger/";
  url += event;
  url += "/with/key/";
  url += privateKey;
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");

  // Read all the lines of the reply from server and print them to Serial,
  // the connection will close when the server has sent all the data.
  while(client.connected())
  {
    if(client.available())
    {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    } else {
      // No data yet, wait a bit
      delay(50);
    };
  }
  
  // All done
  Serial.println();
  Serial.println("closing connection");

  client.stop();
  
  // Finished sending the message, turn off the LED
  digitalWrite(ledPin, LOW);
}

