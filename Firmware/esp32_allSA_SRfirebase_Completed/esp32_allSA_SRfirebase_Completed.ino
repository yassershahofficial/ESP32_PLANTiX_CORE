/**
 * @file GreenProbe_Controller.ino
 * @brief Automated Climate Control System for Smart Plant Monitoring
 *
 * This Arduino sketch is designed for the ESP32 and interfaces with DHT11, 
 * analog soil and pH sensors, and relays to automate watering, misting, 
 * and fertilization based on real-time sensor readings and threshold values 
 * retrieved from Firebase.
 *
 * Features:
 * - Reads temperature and humidity using DHT11
 * - Reads soil moisture and pH using analog sensors
 * - Sends live sensor data to Firebase Realtime Database
 * - Retrieves configured threshold values from Firebase
 * - Controls actuators (mist module, water pump, fertilizer pump) accordingly
 * - Handles connection retry logic and displays status via Serial Monitor
 *
 * @author
 * Yasser Shah — https://github.com/yassershahofficial
 *
 * @version 1.13
 * @date July 2025
 *
 * NOTE:
 * - Requires WiFi credentials in a separate `secrets.h` file.
 * - Uses custom Firebase wrapper class for database operations.
 * - Ensure proper voltage dividers or level shifters for 3.3V compatibility.
 * - If use higher volts, you can use NPN transistor inverter method
 */

#include <WiFi.h>
#include <Firebase.h>
#include <ArduinoJson.h>
#include <DHT.h>
#include "secrets.h"

#define DHTpin 21
#define PHpin 33
#define SOILpin 32
#define MistRelay 23
#define FertPumpRelay 25
#define WaterPumpRelay 26

#define DHTtype DHT11
DHT dht(DHTpin, DHTtype);
Firebase fb(FIREBASE_URL);

const int dot_delay = 500;
const int con_delay = 1000;
const int state_delay = 1000;

//Variables
float temperature, humidity, soil, ph; //main
float stableTemperature, stableHumidity, stableSoil, stablePH; //requirements
float PHvoltage; //add-on
int PHanalogValue, rawSoil; //add-on
int OffsetHumidity = 15;

int humidity_range = 1;
int ph_range = 1;
int soil_range = 5;

int humidity_counter = 5;
int ph_counter = 5;
int soil_counter = 5;

void DotLoops(int countdown){
  for(int i = 0; i < countdown; i++){
    Serial.print(".");
    delay(dot_delay);
  }
}

void setup() {  
  Serial.begin(9600);
  dht.begin();
  pinMode(MistRelay, OUTPUT);
  pinMode(FertPumpRelay, OUTPUT);
  pinMode(WaterPumpRelay, OUTPUT);

  digitalWrite(MistRelay, LOW);
  digitalWrite(FertPumpRelay, LOW);
  digitalWrite(WaterPumpRelay, LOW);

  int con_counter = 0;
  delay(con_delay);

  WiFi.begin(WIFI_ID, WIFI_PW);
  Serial.print("Connecting to the Wifi");
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    con_counter++;
    delay(con_delay);
    if(con_counter > 20){
      Serial.println("\nFailed to connect Wi-Fi");
      return;
    }
  }
  
  Serial.println("\nWi-Fi is connected!");
  Serial.print("WiFi Status: ");
  Serial.println(WiFi.status());
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println("...................");
}

void loop() {
  RetriveData();
  delay(state_delay);
  
  ReadingSensor();
  delay(state_delay);
  
  ShowingReading();
  delay(state_delay);
  
  SendingData();
  delay(state_delay);

  Actuators();
  delay(state_delay);
}

void ReadingSensor(){
  temperature = dht.readTemperature();
  humidity = dht.readHumidity()- OffsetHumidity;
  
  if(isnan(humidity)||isnan(temperature)){
    if(isnan(humidity)){
      Serial.println("Failed to read Humidity from DHT11 sensor!");
    }
    if(isnan(temperature)){
      Serial.println("Failed to read Temperature from DHT11 sensor!");
    }
  }

  rawSoil = analogRead(SOILpin);
  soil = map(rawSoil, 2200, 0, 0, 100);
  
  PHanalogValue = analogRead(PHpin);
  PHvoltage = PHanalogValue * (3.3 / 4095.0); 
  ph = 7 + ((2 - PHvoltage) / 0.18);
}

void ShowingReading(){
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println("°C");
  
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%");
  
  Serial.print("PH: ");
  Serial.println(ph, 2);
  
  Serial.print("Moisture: ");
  Serial.print(soil);
  Serial.println("%");
  
  Serial.println("...................");
}

void SendingData(){
  JsonDocument Plant1;
  Plant1["Status"] = "Connected";
  Plant1["Temperature"] = temperature;
  Plant1["Humidity"] = humidity;
  Plant1["Soil_Moisture"] = soil;
  Plant1["PH_Value"] = ph;
  
  Serial.print("Sending data to Firebase."); 
  DotLoops(4);
  String Serialized_JSON;
  serializeJson(Plant1, Serialized_JSON);
  fb.setJson("Plant1", Serialized_JSON);
  DotLoops(2);
  Serial.println("\nData sent to Firebase.");
  
  Serial.println("...................");
}

void RetriveData(){
  Serial.print("Retrieving data from Firebase.");
  DotLoops(4);
  String Retrieved_Serialized_JSON = fb.getJson("Plant1Requirements");
  JsonDocument Retrieved_Plant1Requirements;
  deserializeJson(Retrieved_Plant1Requirements, Retrieved_Serialized_JSON);
  
  DotLoops(2);
  stableTemperature = Retrieved_Plant1Requirements["StableTemperature"];
  stableHumidity = Retrieved_Plant1Requirements["StableHumidity"];
  stableSoil = Retrieved_Plant1Requirements["StableSoil_Moisture"];
  stablePH = Retrieved_Plant1Requirements["StablePH_Value"];
  
  Serial.println("\nData retrieved from Firebase.");
  Serial.println("Required Temperature : " + String(stableTemperature));
  Serial.println("Required Humidity : " + String(stableHumidity));
  Serial.println("Required Soil : " + String(stableSoil));
  Serial.println("Required PH : " + String(stablePH));
  
  Serial.println("...................");
}

void Actuators(){
  if((humidity < (stableHumidity - humidity_range)) && (humidity_counter >= 5)){
    digitalWrite(MistRelay , HIGH);
    humidity_counter = 0;
  }
  else{
    digitalWrite(MistRelay , LOW);
  }
  if(humidity_counter < 100) humidity_counter++; //overflow
  
  if((ph < (stablePH - ph_range)) && (ph_counter >= 5)){
    digitalWrite(FertPumpRelay, HIGH);
    ph_counter = 0;
  }
  else{
    digitalWrite(FertPumpRelay, LOW);
  }
  if(ph_counter < 100) ph_counter++; //overflow
  
  if((soil < (stableSoil - soil_range)) && (soil_counter >= 5)){
    digitalWrite(WaterPumpRelay, HIGH);
    soil_counter = 0;
  }
  else{
    digitalWrite(WaterPumpRelay, LOW);
  }
  if(soil_counter < 100) soil_counter++; //overflow
}
