#include <WiFi.h>
#include "DHT.h"
#include "LedControl.h"
LedControl lc = LedControl(13, 14, 12, 1);

#define DHTPIN 2
#define DHTTYPE DHT11
#define LED_1 19
#define LED_2 21
#define LED_3 22
#define LED_4 23

const char* ssid = "SunLuCiFer";
const char* password = "0827704588";
String apiKey = "G86U0T965V4O4EWC";
const char* url = "/channels/348678/fields/3/last";
const char* host = "api.thingspeak.com";;

DHT dht(DHTPIN, DHTTYPE);
String line = "";
const int PORT = 80;

WiFiServer server(80);
WiFiClient client;

void setup() {
  Serial.begin(115200);
  delay(10);
  dht.begin();
  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(LED_4, OUTPUT);
  Serial.println("\n\n");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi Connected!!");
  Serial.print("IP address : ");
  Serial.println(WiFi.localIP());
}
void loop()
{ Serial.println(" - POST ----------------------------------");
  PostData();
  DelayShow(30);
  Serial.println(" - GET -----------------------------------");
  GetData();
  DelayShow(30);
}
//====================================================
void DelayShow(int nSec)
{ Serial.print("Delay >>");
  for (int i = nSec; i >= 0; i--)
  { Serial.print(" ");
    Serial.print(i);
    delay(1000);
  }
  Serial.println();
}
//====================================================
void PostData(void)
{ 
  Serial.println("Requesting temperatures...");
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  int t2 = t * 10;
  int h2 = h* 10;
  lc.setDigit(0, 7, t2 / 100, false);
  lc.setDigit(0, 6, (t2 % 100) / 10, true);
  lc.setDigit(0, 5, t2 % 10, false);
  lc.setDigit(0, 4, 12, false);
  lc.setDigit(0, 3, h2 / 100, false);
  lc.setDigit(0, 2, (h2 / 10) % 10, true);
  lc.setDigit(0, 1, h2 % 10, false);
  lc.setRow(0, 0,B00010111);
  
  Serial.print("Temperature is: "); Serial.println(t, 2);
  Serial.print("Humidity is: "); Serial.println(h, 2);
  if (client.connect(host, 80))
  { Serial.print("On POST........ ");
    String postStr = apiKey;
    postStr += "&field1="; // Fields 1
    postStr += String(t);
    postStr += "&field2="; // Fields 2
    postStr += String(h);
    postStr += "\r\n\r\n";
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
    Serial.println("POST Compleat");
  }
  client.stop();
}
//====================================================
void GetData(void) {
  WiFiClient client = server.available();
  Serial.print("Connecting to "); Serial.println(host);
  Serial.print("Requesting URL : "); Serial.println(url);
  if (!client.connect(host, PORT))
  { Serial.print(".");
    return;
  }
  else
  { client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
    delay(50);
    Serial.println("SEND !!"); // เมื่อส่งไปแล้วให้แสดงข้อความ SEND !! ออกมา
  }
  int i = 400;
  while (!client.available()) //ถ้าหากไม่มี response ตอบกลับมา ก็จะให้วนลูป จนกว่าจะมี responce ตอบกลับมา
  { delay(10);
    i--;
    if (i == 0) return;
  }
  while (client.available()) // และเมื่อมี response ตอบกลับมาก็จะให้เข้า loop while แล้วทำการเก็บค่าทั้งหมดไว้ที่ line
  { line += (char)client.read();
  }
  Serial.println();
  Serial.println("GET SUCCESSFULLY !!");
  Serial.print("###### rawData >> "); Serial.println(line);
  int lengthData = line.length();
  char data = line[lengthData - 1];
  String s = (String)"Position " + lengthData + (String)", Data " + data;
  Serial.print("###### splitted >> "); Serial.println(s);
  switch (data) {
    case '1':
      digitalWrite(LED_1, HIGH); // ถ้าเป็น 1 led1 ติด
      break;
    case '2':
      digitalWrite(LED_1, LOW); // ถ้าเป็น 2 led1 ดับ
      break;
    case '3':
      digitalWrite(LED_2, HIGH); // ถ้าเป็น 3 led2 ติด
      break;
    case '4':
      digitalWrite(LED_2, LOW); // ถ้าเป็น 4 led2 ดับ
      break;
    case '5':
      digitalWrite(LED_3, HIGH); // ถ้าเป็น 5 led3 ติด
      break;
    case '6':
      digitalWrite(LED_3, LOW); // ถ้าเป็น 6 led3 ดับ
      break;
    case '7':
      digitalWrite(LED_4, HIGH); // ถ้าเป็น 7 led4 ติด
      break;
    case '8':
      digitalWrite(LED_4, LOW); // ถ้าเป็น 8 led4 ดับ
      break;
    case '9':
      digitalWrite(LED_1, HIGH); // ถ้าเป็น 9 All LED On
      digitalWrite(LED_2, HIGH);
      digitalWrite(LED_3, HIGH);
      digitalWrite(LED_4, HIGH);
      break;
    case '0':
      digitalWrite(LED_1, LOW); // ถ้าเป็น 0 All LED Off
      digitalWrite(LED_2, LOW);
      digitalWrite(LED_3, LOW);
      digitalWrite(LED_4, LOW);
      break;
  }
  line = ""; // เคลียข้อมูลในตัวแปร line ให้เท่ากับ "" เพื่อรอรับค่าใหม่ใน loop หน้า
  Serial.println();
  Serial.println("Closing Connection");
}
