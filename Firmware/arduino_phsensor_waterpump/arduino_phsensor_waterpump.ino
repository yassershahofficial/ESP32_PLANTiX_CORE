
//PH Sensor
#define PHpin 33

//Fertilizer Pump Relay
#define FertPumpRelay 25

//PH-FertilizerPump Variables
int PHstable = 8;
int PHrange = 1;
int PHcounter = 5;

void setup() {
  Serial.begin(9600);

  //PH-FertilizerPump
  pinMode(FertPumpRelay, OUTPUT);
  digitalWrite(FertPumpRelay, LOW);
}

void loop() {
  int PHanalogValue = analogRead(PHpin); // Read raw analog value (0–1023)
  float voltage = PHanalogValue * (3.3 / 4095.0); // Convert to voltage

  //Adjust the slope/offset
  float pH = 7 + ((2.5 - voltage) / 0.18);  // Assumes 2.5V = pH 7

  Serial.print("PH Analog: ");
  Serial.println(PHanalogValue);
  
  Serial.print("PH Voltage: ");
  Serial.println(voltage, 2);
  
  Serial.print("PH: ");
  Serial.println(pH, 2);

  if((pH < (PHstable - PHrange)) && (PHcounter >= 5)){
    digitalWrite(FertPumpRelay, HIGH);
    PHcounter = 0;
  }
  else{
    digitalWrite(FertPumpRelay, LOW);
  }
  if(PHcounter < 100) PHcounter++; //overflow

  delay(2000);
}
