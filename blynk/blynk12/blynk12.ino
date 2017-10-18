#define BLYNK_PRINT Serial
#include <SimpleTimer.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "3ee27a61491f478fa4edd9eba0b9bdf5";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "SunLuCiFer";
char pass[] = "0827704588";
SimpleTimer timer;
void setup()
{
  // Debug console
  Serial.begin(9600);
  dht.begin();
  //delay(10);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000, sendData);
  pinMode(19, OUTPUT);
  pinMode(21, OUTPUT);
  pinMode(22, OUTPUT);
  pinMode(23, OUTPUT);
}
void sendData(){

  //Read the Temp and Humidity from DHT
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  //Write values to Vxx and Vxx
  Blynk.virtualWrite(1, h);
  Blynk.virtualWrite(0, t);
  Serial.print(t);
  Serial.print("\t");
  Serial.println(h);

}

void loop()
{
  Blynk.run();
  timer.run();
}

