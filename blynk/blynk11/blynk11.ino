#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "3ee27a61491f478fa4edd9eba0b9bdf5";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "SunLuCiFer";
char pass[] = "0827704588";

void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);
  pinMode(19,OUTPUT);
  pinMode(21,OUTPUT);
  pinMode(22,OUTPUT);
  pinMode(23,OUTPUT);
}

void loop()
{
  Blynk.run();
}

