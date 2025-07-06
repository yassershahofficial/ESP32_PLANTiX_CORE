#include <DHT.h>

//DHT Sensor
#define DHTpin 21
#define DHTtype DHT11
DHT dht(DHTpin, DHTtype);

//Mist Module Relay
#define MistRelay 23

//DHT-MistModule Variable                                                                                     s
int DHTstable = 100;
int DHTrange = 1;
int DHTcounter = 5;

//---------------------------

//PH Sensor
#define PHpin 33

//Fertilizer Pump Relay
#define FertPumpRelay 25

//PH-FertilizerPump Variables
int PHstable = 8;
int PHrange = 1;
int PHcounter = 5;

//---------------------------

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
  
  //DHT-MistModule
  dht.begin();
  pinMode(MistRelay, OUTPUT);
  
  //PH-FertilizerPump
  pinMode(FertPumpRelay, OUTPUT);
  digitalWrite(FertPumpRelay, LOW);
  
  //SOIL-WaterPump
  pinMode(WaterPumpRelay, OUTPUT);
}

void loop() {
  dht11_MistModule();
  ph_fertilizer();
  soil_water();
  delay(10000);
  Serial.println("-------------------------------");
}

void dht11_MistModule(){
    float temperature = dht.readTemperature(); // Celsius
    float humidity = dht.readHumidity()-20; // Percentage
  
    if(isnan(humidity)||isnan(temperature)){
      if(isnan(humidity)){
        Serial.println("Failed to read Humidity from DHT11 sensor!");
      }
      if(isnan(temperature)){
        Serial.println("Failed to read Temperature from DHT11 sensor!");
      }
      return;
    }
  
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println("°C");
  
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println("%");
  
    if((humidity < (DHTstable - DHTrange)) && (DHTcounter >= 5)){
      digitalWrite(MistRelay , HIGH);
      DHTcounter = 0;
    }
    else{
      digitalWrite(MistRelay , LOW);
    }
    if(DHTcounter < 100) DHTcounter++; //overflow
}

void ph_fertilizer(){
  int PHanalogValue = analogRead(PHpin); // Read raw analog value (0–1023)
  float voltage = PHanalogValue * (3.3 / 4095.0); // Convert to voltage

  //Adjust the slope/offset
  float pH = 7 + ((2 - voltage) / 0.18);  // Assumes 2.5V = pH 7

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
}

void soil_water(){
  int SOILsensorValue = analogRead(SOILpin);  // Read raw analog value (0-1023)

  //Map raw value to percentage moisture
  int moisturePercent = map(SOILsensorValue, 2200, 0, 0, 100);

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
}
