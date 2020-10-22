#include <Arduino.h>

#include <WEMOS_SHT3X.h>

#include <ESP8266WiFi.h>          //ESP8266 Core WiFi Library (you most likely already have this in your sketch)
#include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the configuration portal
//#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal

#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "LittleFS.h" // LittleFS is declared


SHT3X sht30(0x45);
ESP8266WebServer server(80);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.com");
 

// Serving Hello world
void getRealTimeJson() {
  if(sht30.get()==0){
    StaticJsonDocument<200> doc;
    doc["sensor"] = "sht30";
    doc["time"] = timeClient.getEpochTime();
    doc["temperature"] = String(sht30.cTemp);
    doc["humidity"] = String(sht30.humidity);
    doc["mac"] = WiFi.macAddress();
    String buf;
    serializeJsonPretty(doc, buf);
    server.send(200, F("text/json"), buf);
  }
  else
  {
    server.send(500, F("text/plain"), F("sensor not ready"));
  }
}


// Define routing
void restServerRouting() {
    server.on(F("/rt"), HTTP_GET, getRealTimeJson);
    server.serveStatic("/", LittleFS, "/index.html");
    server.serveStatic("/jquery.js", LittleFS, "/jquery-3.5.1.min.js");
    server.serveStatic("/local.js", LittleFS, "/local.js");
    server.serveStatic("/mini.css", LittleFS, "/mini-default.min.css");
}
 

void setup() {
  Serial.begin(115200);
  WiFiManager wifiManager;
  wifiManager.autoConnect("sensor");
  
  LittleFS.begin(); // Before routing!

  restServerRouting();
  server.begin();
  timeClient.begin();

  Serial.println("");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  server.handleClient();
  timeClient.update();
}

