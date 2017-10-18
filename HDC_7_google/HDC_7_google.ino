#include <Wire.h>
#include <IFTTTMaker.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "LedControl.h"
LedControl lc = LedControl(13, 14, 12, 1);

#include "ClosedCube_HDC1080.h"
ClosedCube_HDC1080 hdc1080;

char ssid[] = "SunLuCiFer"; // your network SSID (name)
char password[] = "0827704588"; // your network key
#define KEY "c5FFIp-BgDeWnKEM8sUCsW"

WiFiClientSecure client;
IFTTTMaker ifttt(KEY, client);

unsigned long int lasttime = 0;

void setup()
{ Serial.begin(115200);

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

  Serial.println("ClosedCube HDC1080 Arduino Test");
  hdc1080.begin(0x40); // 14 bit Temperature and Humidity MeasurementResolutions

}

void loop()
{ unsigned long int pretime = millis();
  Serial.print("Temp('C) = "); Serial.print(hdc1080.readTemperature());
  Serial.print(", RH(%) = "); Serial.print(hdc1080.readHumidity());
  Serial.println();
  float temp = hdc1080.readTemperature();
  float humid = hdc1080.readHumidity();
  int t = temp * 100;
  int h = humid * 100;
  lc.setDigit(0, 7, t / 1000, false);
  lc.setDigit(0, 6, (t / 100) % 10, true);
  lc.setDigit(0, 5, (t % 100) / 10, false);
  lc.setDigit(0, 4, 12, false);
  lc.setDigit(0, 3, h / 1000, false);
  lc.setDigit(0, 2, (h / 100) % 10, true);
  lc.setDigit(0, 1, (h % 100) / 10, false);
  lc.setRow(0, 0, B00010111);

  Serial.println((pretime - lasttime) / 1000.0);

  if ((pretime - lasttime) >= 40000)
  { lasttime = pretime;
    Serial.print("Temperature Save Tepperature..... ");
    if(ifttt.triggerEvent("HDC",String(humid), String(temp)))
      Serial.println(" > Successfully sent");
    else
      Serial.println(" > Failed!");
  }
  delay(2000);
}
