#include <AntoIO.h>
#define LED1 19
#define LED2 21
#define LED3 22
#define LED4 23
#define sw1_mo 18
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

bool sw1=0;
AntoIO anto(user, token, thing);

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(sw1_mo,INPUT_PULLUP);
  Serial.begin(115200);
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
}
void loop() {
  anto.mqtt.loop();
  /*
    for proper functionality,
    10-second delay should be added after loop()
  */
  delay(10);
  if (sw1 != digitalRead(sw1_mo))
  { sw1 = digitalRead(sw1_mo);
    anto.mqtt.pub(channel_sw1, sw1);
    Serial.print(" Status D23 = ");
    Serial.println(sw1);
    delay(10);
  }
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
