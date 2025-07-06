#include <WiFi.h>
#include <Firebase.h>
#include <ArduinoJson.h>
#include <DHT.h>

//wifi credentials
#define WIFI_ID "Honor"
#define WIFI_PW "bbyhensem"
#define FIREBASE_URL "https://esp32-db-e792f-default-rtdb.asia-southeast1.firebasedatabase.app/"

//Firebase object - Test Mode
Firebase fb(FIREBASE_URL);

//DHT Sensor
#define DHTpin 21
#define DHTtype DHT11
DHT dht(DHTpin, DHTtype);

//Mist Module Relay
#define MistRelay 23

//PH Sensor
#define PHpin 33

//Fertilizer Pump Relay
#define FertPumpRelay 25

//Soil Sensor
#define SOILpin 32

//Water Pump Relay
#define WaterPumpRelay 26

const int dotdelay = 500;
const int systemdelay = 1000;
const int statedelay = 1000;

//Variables
float temperature, humidity, soil, ph;
float stableTemperature, stableHumidity, stableSoil, stablePH;
float voltage;
int PHanalogValue, SOILsensorValue;

void DotLoops(int countdown){
  for(int i = 0; i < countdown; i++){
    Serial.print(".");
    delay(dotdelay);
  }
}

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
  
  int counter = 0;
  delay(systemdelay);

  //Wifi connection
  WiFi.begin(WIFI_ID, WIFI_PW);
  Serial.print("Connecting to the Wifi");
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    counter++;
    delay(systemdelay);

    if(counter > 20){
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
  ReadingSensor();
  delay(statedelay);
  
  ShowingReading();
  delay(statedelay);
  
  SendingData();
  delay(statedelay);
}

void ReadingSensor(){
  SOILsensorValue = analogRead(SOILpin);  // Read raw analog value (0-1023)
  //Map raw value to percentage moisture
  soil = map(SOILsensorValue, 2200, 0, 0, 100);
  
  PHanalogValue = analogRead(PHpin); // Read raw analog value (0–1023)
  voltage = PHanalogValue * (3.3 / 4095.0); // Convert to voltage

  //Adjust the slope/offset
  ph = 7 + ((2 - voltage) / 0.18);  // Assumes 2V = pH 7
  
  temperature = dht.readTemperature(); // Celsius
  humidity = dht.readHumidity()-20; // Percentage

  if(isnan(humidity)||isnan(temperature)){
      if(isnan(humidity)){
        Serial.println("Failed to read Humidity from DHT11 sensor!");
      }
      if(isnan(temperature)){
        Serial.println("Failed to read Temperature from DHT11 sensor!");
      }
      return;
    }
}

void ShowingReading(){
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println("°C");
    
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println("%");
    
    Serial.print("PH Analog: ");
    Serial.println(PHanalogValue);
    
    Serial.print("PH Voltage: ");
    Serial.println(voltage, 2);
    
    Serial.print("PH: ");
    Serial.println(ph, 2);
    
    Serial.print("Analog Value: ");
    Serial.println(SOILsensorValue);
    
    Serial.print("Moisture: ");
    Serial.print(soil);
    Serial.println("%");

    Serial.println("...................");
}

void SendingData(){
    //Create JSON document to record Plant1
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
