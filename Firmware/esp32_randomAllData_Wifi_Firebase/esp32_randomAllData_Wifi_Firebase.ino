#include <WiFi.h>
#include <Firebase.h>
#include <ArduinoJson.h>
#include "secret.h"

//Firebase object - Test Mode
Firebase fb(FIREBASE_URL);

const int dotdelay = 500;
const int systemdelay = 1000;
const int statedelay = 200;

float temperature, humidity, soil, ph;

void DotLoops(int countdown){
  for(int i = 0; i < countdown; i++){
    Serial.print(".");
    delay(dotdelay);
  }
}

void setup() {  
  Serial.begin(115200);
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
}

void loop() {
  //RetriveData();
  //delay(statedelay);
  
  ReadingSensor();
  delay(statedelay);
  
  ShowingReading();
  delay(statedelay);
  
  SendingData();
  delay(statedelay);
}

void ReadingSensor(){
  Serial.print("Reading data from sensor.");
  DotLoops(2);
  
  temperature = random(240,260)*0.1; //average 24-26°C
  humidity = random(300,350)*0.1; //average 30-35%
  soil = random(400, 450)*0.1; //average 40-45%
  ph = random(60,80)*0.1; //average 6-8
  
  DotLoops(1);
}

void ShowingReading(){
  Serial.print("\nData read and now showing.");
  DotLoops(2);

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println("°C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%");
  
  Serial.print("Soil Moisture: ");
  Serial.print(soil);
  Serial.println("%");

  Serial.print("PH: ");
  Serial.println(ph);

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

/**
void RetriveData(){
  Serial.print("Retrieving data from Firebase.");
  DotLoops(4);
  String Retrieved_Serialized_JSON =  fb.getJson("Plant1");
  JsonDocument Retrieved_Plant1;
  deserializeJson(Retrieved_Plant1, Retrieved_Serialized_JSON);

  DotLoops(2);
  String Status = Retrieved_Plant1["Status"];
  float Temperature = Retrieved_Plant1["Temperature"];
  float Humidity = Retrieved_Plant1["Humidity"];
  Serial.println("\nData retrieved from Firebase.");

  Serial.println("Status : " + String(Status));
  Serial.println("Temperature : " + String(Temperature));
  Serial.println("Humidity : " + String(Humidity));
  
  Serial.println("...................");
}
**/
