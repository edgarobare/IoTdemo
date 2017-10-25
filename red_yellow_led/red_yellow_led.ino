
int redLed=9;
int redOn=250;
int redOff=250;
int yellowLed=11;
int yellowOff=250;
int yellowOn=250;
int redBlink;
int yellowBlink;
 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
pinMode (redLed,OUTPUT);
pinMode (yellowLed, OUTPUT);

  Serial.println("How many times do you want to blink the Red LED?");
  while (Serial.available()==0) { }
  redBlink = Serial.parseInt();
  
  Serial.println("How many times do you want to blink the Yellow LED?");
  while (Serial.available()==0) { }
  yellowBlink = Serial.parseInt();
}

void loop() {
  // put your main code here, to run repeatedly:
  
  Serial.println("The red LED is blinking.");
  
  for (int i=1;i<=redBlink;i++) {
    Serial.print("  You are on blink #: ");
    Serial.println(i);
  digitalWrite(redLed,HIGH);
  delay(redOn);
  digitalWrite(redLed,LOW);
  delay(redOff);
  }
Serial.println(" ");

Serial.println("The yellow LED is blinking.");
  for (int i=1;i<=yellowBlink;i++) {
    Serial.print("  You are on blink #: ");
    Serial.println(i);
  digitalWrite(yellowLed,HIGH);
  delay(yellowOn);
  digitalWrite(yellowLed,LOW);
  delay(yellowOff);
  }
  
  Serial.println(" ");

}
