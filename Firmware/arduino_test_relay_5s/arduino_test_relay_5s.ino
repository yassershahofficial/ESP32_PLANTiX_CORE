void setup() {
  pinMode(22, OUTPUT);

}

void loop() {
  digitalWrite(22 , LOW);  // or try HIGH if LOW doesn't work
  delay(5000);
  digitalWrite(22 , HIGH);  // or try HIGH if LOW doesn't work
  delay(5000);
}
