#include "FirebaseESP8266.h" 
#include <ESP8266WiFi.h>
 
#define FIREBASE_HOST "posa-6d047-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "kblMk5DlvKqqBGGr0C1UvlZ5LHXTP3OmuYsCCkEW"
#define WIFI_SSID "JJW"
#define WIFI_PASSWORD "j1234567!!"

#define pinresist  A0

int ANALOG_PIN = 0;
int time_v = 0;                     //저항값 번호 인덱스
int i = 0;
int k = 0;
int resist_val[288];                //저항값 실제로 저장할 배열

int a = 0;
int b = 0;
int c = 0;
int d = 0;


FirebaseData firebaseData;
FirebaseJson json;
 
void setup() // wifi 접속 과정.
{
  Serial.begin(9600);
 
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println();
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
 
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
 
  firebaseData.setBSSLBufferSize(1024, 1024);
  firebaseData.setResponseSize(1024);
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  Firebase.setwriteSizeLimit(firebaseData, "tiny");
}

void loop()
{
  String key = "resist" + String(time_v);

  int readValue = analogRead(ANALOG_PIN);
  if (!Firebase.setInt(firebaseData, key.c_str(), readValue)) {
    Serial.print("Firebase write failed:");
    Serial.println(firebaseData.errorReason()); // 실패 원인 로깅
  }

  Serial.println(readValue);
  Serial.println(time_v);

  time_v = time_v + 1;

  if (time_v == 100) {
    time_v = 0;
  }

  delay(1000); // 1초마다 반복
}