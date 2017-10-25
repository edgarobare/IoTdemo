int ledPin(13);
int delayOn(900);
int delayOff(100);

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(ledPin,HIGH);
delay(delayOn);
digitalWrite(ledPin,LOW);
delay(delayOff);

}
