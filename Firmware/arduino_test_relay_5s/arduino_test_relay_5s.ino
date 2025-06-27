void setup() {
  pinMode(8, OUTPUT);

}

void loop() {
  digitalWrite(8 , HIGH);  // or try HIGH if LOW doesn't work
  delay(5000);
  digitalWrite(8 , LOW);  // or try HIGH if LOW doesn't work
  delay(5000);
}
