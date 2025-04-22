#include <WiFi.h>
#include <Firebase.h>
#include <ArduinoJson.h>

//wifi credentials
#define WIFI_ID "id"
#define WIFI_PW "password"
#define FIREBASE_URL "https://firebasedatabase.url/"

//Firebase object - Test Mode
Firebase fb(FIREBASE_URL);

const int dotdelay = 1000;

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
    delay(dotdelay);

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
  SendingData();
  delay(5000);
  RetriveData();
  delay(10000);
}

void SendingData(){
//Create JSON document to record Plant1
  JsonDocument Plant1;

  Plant1["Status"] = "Connected";
  Plant1["Temperature"] = (float)random(2400,2600)*0.01;
  Plant1["Humidity"] = (float)random(3000,3500)*0.01;
  
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
void DotLoops(int countdown){
  for(int i = 0; i < countdown; i++){
    Serial.print(".");
    delay(dotdelay);
  }
}
