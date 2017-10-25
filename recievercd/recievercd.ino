//Receiver Code 

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
}

void loop()
{
  if (Serial1.available()) {
        Serial.write("Serial CONNECTED");
        Serial.println("");    
        Serial.write(Serial1.read());
  } else {
        Serial.write("No serial connected");
        Serial.println("");     
  }
  delay(1000);
}
