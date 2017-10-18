#include <AntoIO.h>
#include <dht11.h>
#include "LedControl.h"
LedControl lc=LedControl(13,14,12,1);
#define LED1 19
#define LED2 21
#define LED3 22
#define LED4 23
#define sw1_mo 18
#define sw2_mo 5
dht11 DHT;
#define DHT11_PIN 2
#define vcc 15
unsigned long lastMillis = 0;
unsigned long preMillis = 0;
const char *ssid = "SunLuCiFer";
const char *pass = "0827704588";
const char *user = "sunnyseeny";
const char *token = "SbIUPXKgfI7AoF7ySg6oMYwt9KvvLsvNtLmTFyWC";
const char *thing = "LabIOT";
const char *channel_LED1 = "LED1";
const char *channel_LED2 = "LED2";
const char *channel_LED3 = "LED3";
const char *channel_LED4 = "LED4";
const char *channel_sw1 = "SW1";
const char *channel_sw2 = "SW2";
const char *channel_Temp = "Temp";
const char *channel_Humd = "Humid";

boolean sw1=0,sw2=0;
AntoIO anto(user, token, thing);

void setup() {
  pinMode(vcc,OUTPUT);
  digitalWrite(vcc,HIGH);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(sw1_mo,INPUT_PULLUP);
  pinMode(sw2_mo,INPUT_PULLUP);
  Serial.begin(115200);
  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);
  Serial.print("\nTrying to connect ");
  Serial.print(ssid);
  Serial.println("...");
  while (!anto.wifi.begin(ssid, pass));
  Serial.println("-- Connected, trying to connect to broker...");
  while (!anto.mqtt.connect(user, token, true));
  Serial.println("-- Connected");
  /* change "digital-channel" to your channel in switch type */
  anto.mqtt.sub(channel_LED1);
  anto.mqtt.sub(channel_LED2);
  anto.mqtt.sub(channel_LED3);
  anto.mqtt.sub(channel_LED4);
  anto.mqtt.sub(channel_sw1);
  anto.mqtt.sub(channel_sw2);
}
void loop() {
  int chk;
  Serial.print("DHT11, \t");
  chk = DHT.read(DHT11_PIN);    // READ DATA
  switch (chk){
    case DHTLIB_OK:  
                Serial.println("OK,\t"); 
                break;
    case DHTLIB_ERROR_CHECKSUM: 
                Serial.println("Checksum error,\t"); 
                break;
    case DHTLIB_ERROR_TIMEOUT: 
                Serial.println("Time out error,\t"); 
                break;
    default: 
                Serial.println("Unknown error,\t"); 
                break;
  }
  preMillis = millis();
  anto.mqtt.loop();
  
  /*
    for proper functionality,
    10-second delay should be added after loop()
  */
  delay(10);
  if (sw1 != digitalRead(sw1_mo))
  { sw1 = digitalRead(sw1_mo);
    anto.mqtt.pub(channel_sw1, sw1);
    Serial.print(" Status D18 = ");
    Serial.println(sw1);
    delay(10);
  }
  if (sw2 != digitalRead(sw2_mo))
  { sw2 = digitalRead(sw2_mo);
    anto.mqtt.pub(channel_sw2, sw2);
    Serial.print(" Status D5 = ");
    Serial.println(sw2);
    delay(10);
  }
  if ((preMillis - lastMillis) >= 1000) {
    lastMillis = preMillis;
    
    float h = DHT.humidity;
    float t = DHT.temperature;
    
    anto.mqtt.pub(channel_Temp,int(t)); delay(200);
    anto.mqtt.pub(channel_Humd,int(h)); delay(200);

    Serial.print("Temperature is: "); Serial.println(t, 2);
    Serial.print("Humidity is: "); Serial.println(h, 2);
    // lc.setDigit(0,adress,charracter,dot (false dont have));
    lc.setDigit(0, 7, int(t) / 10, false);
    lc.setDigit(0, 6, int(t) % 10, true);
    lc.setDigit(0, 5, 0, false);
    lc.setDigit(0, 4, 12, false);
    lc.setDigit(0, 3, int(h) / 10, false);
    lc.setDigit(0, 2, int(h) % 10, true);
    lc.setDigit(0, 1, 0, false);
    lc.setRow(0, 0, B00010111);   //lc.setRow(0,0,B00010111);
  }
  //delay(2000);
}

void messageReceived(String topic, String payload, char * bytes, unsigned int length) {
  Serial.print("incoming: ");
  Serial.print(topic);
  Serial.print(" - ");
  Serial.print(payload);
  Serial.println();
  //led1
  if (topic.indexOf(channel_LED1) != -1)
    if (payload.toInt() == 1) {
      digitalWrite(LED1, HIGH);
      Serial.println("HIGH");
    } else {
      digitalWrite(LED1, LOW);
      Serial.println("LOW");
    }
  //led2
  if (topic.indexOf(channel_LED2) != -1)
    if (payload.toInt() == 1) {
      digitalWrite(LED2, HIGH);
      Serial.println("HIGH");
    } else {
      digitalWrite(LED2, LOW);
      Serial.println("LOW");
    }
  //led3
  if (topic.indexOf(channel_LED3) != -1)
    if (payload.toInt() == 1) {
      digitalWrite(LED3, HIGH);
      Serial.println("HIGH");
    } else {
      digitalWrite(LED3, LOW);
      Serial.println("LOW");
    }
  //led4
  if (topic.indexOf(channel_LED4) != -1)
    if (payload.toInt() == 1) {
      digitalWrite(LED4, HIGH);
      Serial.println("HIGH");
    } else {
      digitalWrite(LED4, LOW);
      Serial.println("LOW");
    }
}
