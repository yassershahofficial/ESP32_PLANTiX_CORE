
//Soil Sensor
#define SOILpin 32

//Water Pump Relay
#define WaterPumpRelay 26

//SOIL-WaterPump Variables
int SOILstable = 40;
int SOILrange = 5;
int SOILcounter = 5;

void setup() {
  Serial.begin(9600);

  //SOIL-WaterPump
  pinMode(WaterPumpRelay, OUTPUT);
}

void loop() {
  int SOILsensorValue = analogRead(SOILpin);  // Read raw analog value (0-1023)

  //Map raw value to percentage moisture
  int moisturePercent = map(SOILsensorValue, 1023, 0, 0, 100);

  Serial.print("Analog Value: ");
  Serial.println(SOILsensorValue);
  
  Serial.print("Moisture: ");
  Serial.print(moisturePercent);
  Serial.println("%");

  if((moisturePercent < (SOILstable - SOILrange)) && (SOILcounter >= 5)){
    digitalWrite(WaterPumpRelay, HIGH);
    SOILcounter = 0;
  }
  else{
    digitalWrite(WaterPumpRelay, LOW);
  }
  if(SOILcounter < 100) SOILcounter++; //overflow

  delay(2000);
}
