#include <IFTTTMaker.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
//------- Replace the following! ------
char ssid[] = "SunLuCiFer"; // your network SSID (name)
char password[] = "0827704588"; // your network key
#define KEY "c5FFIp-BgDeWnKEM8sUCsW"
WiFiClientSecure client;
IFTTTMaker ifttt(KEY, client);
int Count = 15;
void setup() {
  Serial.begin(115200);
  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);
  dht.begin();
}
void loop() {
  delay(2000);
  float h = dht.readHumidity();
  float t = dht.readTemperature(); 
  float f = dht.readTemperature(true);
  float hif = dht.computeHeatIndex(f, h); 
  float hic = dht.computeHeatIndex(t, h, false);
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Serial.print("Count = "); Serial.print(Count);
  Serial.print("\tHumidity: "); Serial.print(h);
  Serial.print("\tTemperature: "); Serial.print(t);
  Serial.print(" *C, "); Serial.print(f);
  Serial.print(" *F\tHeat index: "); Serial.print(hic);
  Serial.print(" *C,"); Serial.print(hif);
  Serial.println(" *F");
  if (Count <= 0)
  { Count = 15;
    Serial.print("Temperature Save Tepperature..... ");
    int ChkStatus = ifttt.triggerEvent("TriggerSaveTempp", String(h), String(t), String(hic));
    if (ChkStatus == 0)
      Serial.println(" > Successfully sent");
    else
      Serial.println(" > Failed!");
  }
  
  Count--;
}
