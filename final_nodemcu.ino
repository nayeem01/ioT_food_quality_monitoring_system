#define BLYNK_PRINT Serial
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>
#include <SoftwareSerial.h>
SoftwareSerial s(D6,D5);
#include <ArduinoJson.h>

char auth[]="Ml1-fUdl_o0H9S7UitKsMAalTChLM4i3";
char ssid[]="Toufique";
char pass[]="toufique123";
//SimpleTimer timer;
BlynkTimer timer;

void sendSensor()
{
StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(s);
 
  if (root == JsonObject::invalid())
  {
    return;
  }
  
  Serial.println("JSON received and parsed");
  root.prettyPrintTo(Serial);
  
  Serial.print("Ldr ");
  int data1=root["l"];
  Blynk.virtualWrite(V4, data1); //ldr
  int data2 = root["m"];
  Blynk.virtualWrite(V7, data2); // mq3
  Serial.print(data2);
  int data3 = root["t"];
  Blynk.virtualWrite(V6, data3); // temp
  int data4 = root["h"];
  Blynk.virtualWrite(V5, data4); // hum
  int data5 = root["p"];
  Blynk.virtualWrite(V3, data5); //ph
}

void setup() {
   Serial.begin(115200);
  s.begin(115200);
  while (!Serial) continue;
  Blynk.begin(auth,ssid,pass);
  delay(1000);
  timer.setInterval(100L,sendSensor);

}

void loop() {
  Blynk.run();
  timer.run();

}
