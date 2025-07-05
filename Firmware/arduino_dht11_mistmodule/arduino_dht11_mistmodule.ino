#include <DHT.h>

//DHT Sensor
#define DHTpin 2
#define DHTtype DHT11
DHT dht(DHTpin, DHTtype);

//Mist Module Relay
#define MistRelay 3

//DHT-MistModule Variable                                                                                     s
int DHTstable = 100;
int DHTrange = 1;
int DHTcounter = 5;

void setup() {
  Serial.begin(9600);

  //DHT-MistModule
  dht.begin();
  pinMode(MistRelay, OUTPUT);
}

void loop() {
  delay(2000);
  
  float temperature = dht.readTemperature(); // Celsius
  float humidity = dht.readHumidity(); // Percentage

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
