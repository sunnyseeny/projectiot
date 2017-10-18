#include <WiFi.h>
#define LED_1 19
#define LED_2 21
#define LED_3 22
#define LED_4 23
const char* ssid = "SunLuCiFer";
const char* password = "0827704588";
const char* url = "/channels/348678/fields/3/last";
const char* host = "api.thingspeak.com";
//https://api.thingspeak.com/channels/348678/fields/3/last
String line = "";
const int PORT = 80;
WiFiServer server(80);
void setup() {
  Serial.begin(115200);
  delay(10);
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
{ delay(5000);
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
  while (!client.available()) //ถ้าหากไม่มี response ตอบกลับมา ก็จะให้วนลูป จนกว่าจะมี responce ตอบกลับมา
  {}
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
