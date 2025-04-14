#include <WiFi.h>
#include <Firebase.h>
#include <ArduinoJson.h>

//wifi credentials
#define WIFI_ID "id"
#define WIFI_PW "password"
#define FIREBASE_URL "https://firebase_url/"

//Firebase object - Test Mode
Firebase fb(FIREBASE_URL);

void setup() {  
  Serial.begin(115200);
  int counter = 0;
  delay(1000);
  
  //wifi connection
  WiFi.begin(WIFI_ID, WIFI_PW);
  Serial.print("Connecting to the Wifi");
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    counter++;
    delay(500);

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

  //Create JSON document to record Plant1
  JsonDocument Plant1;

  Plant1["Status"] = "Connected";
  Plant1["Temperature"] = 28.4;
  Plant1["Humidity"] = 34.2;

  Serial.println("Sending data to Firebase.");
  String Serialized_JSON;
  serializeJson(Plant1, Serialized_JSON);
  fb.setJson("Plant1", Serialized_JSON);
  Serial.println("Data sent to Firebase.");

  Serial.println("Retrieving data from Firebase.");
  String Retrieved_Serialized_JSON =  fb.getJson("Plant1");
  JsonDocument Retrieved_Plant1;
  deserializeJson(Retrieved_Plant1, Retrieved_Serialized_JSON);
  
  String Status = Retrieved_Plant1["Status"];
  float Temperature = Retrieved_Plant1["Temperature"];
  float Humidity = Retrieved_Plant1["Humidity"];
  Serial.println("Data retrieved from Firebase.");

  Serial.println("Status : " + String(Status));
  Serial.println("Temperature : " + String(Temperature));
  Serial.println("Humidity : " + String(Humidity));
}
void loop() {
  // Required even if empty
}

/*
  fb.setString("Plant1/Status","Connected"); 
  fb.setFloat("Plant1/Data/Temperature",28.6);
  Serial.println("Data sent to Firebase.");

  Serial.println("Fetching Data");
  String Status = fb.getString("Plant1/Status");
  float Temperature = fb.getFloat("Plant1/Data/Temperature");
  Serial.println("Data Fetched");

  Serial.println("Status : " + String(Status));
  Serial.println("Temperature : " + String(Temperature));
 */
