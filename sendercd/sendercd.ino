//Sender Code
void setup() {
  Serial.begin(115200);
}

void loop() {
    Serial.write("Hello");
    Serial.println(""); 
    //delay(1000);
}
