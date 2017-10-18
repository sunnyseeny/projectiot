#include <IFTTTMaker.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "LedControl.h"
LedControl lc = LedControl(13, 14, 12, 1);
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

unsigned long pretime = 0, lasttime = 0;
void setup() {
  Serial.begin(115200);
  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);
  
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
  pretime = millis();
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
  int t2 = t * 10;
  int h2 = h * 10;
  lc.setDigit(0, 7, t2 / 100, false);
  lc.setDigit(0, 6, (t2 % 100) / 10, true);
  lc.setDigit(0, 5, t2 % 10, false);
  lc.setDigit(0, 4, 12, false);
  lc.setDigit(0, 3, h2 / 100, false);
  lc.setDigit(0, 2, (h2 / 10) % 10, true);
  lc.setDigit(0, 1, h2 % 10, false);
  lc.setRow(0, 0, B00010111);

  Serial.print("Humidity: "); Serial.print(h);
  Serial.println(" %, ");
  Serial.print("\tTemperature: "); Serial.print(t);
  Serial.println(" *C, ");
  Serial.print("Time: ");
  Serial.println(((pretime - lasttime) / 60000.0 ));
  if (((pretime - lasttime) / 60000.0 )>= 2.0)
  { lasttime = pretime;
    Serial.print("Temperature Save Tepperature..... ");
    int ChkStatus = ifttt.triggerEvent("TempandHumid", String(h), String(t));
    if (ChkStatus == 0)
      Serial.println(" > Successfully sent");
    else
      Serial.println(" > Failed!");
  }
}
