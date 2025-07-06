#include <WiFi.h>

//wifi credentials
#define WIFI_ID "Honor"
#define WIFI_PW "bbyhensem"

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
    delay(300);

    if(counter > 20){
      Serial.println("\nFailed to connect Wi-Fi");
      return;
    }
  }
  Serial.println("Wi-Fi is connected!");
  Serial.print("WiFi Status: ");
  Serial.println(WiFi.status());
}

void loop() {

}
