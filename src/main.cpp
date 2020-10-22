#include <Arduino.h>

#include <WEMOS_SHT3X.h>

#include <ESP8266WiFi.h>          //ESP8266 Core WiFi Library (you most likely already have this in your sketch)
#include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the configuration portal
//#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal

#include "wifi_manager.h"

#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "LittleFS.h" // LittleFS is declared

SHT3X sht30(0x45);
AsyncWebServer server(80);

String buf = "{}";
 
// Serving Hello world
void getRealTimeJson(AsyncWebServerRequest *request) {
  request->send(200, "text/json", buf);
}

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

// Define routing
void restServerRouting() {
  server.on("/rt", HTTP_GET, getRealTimeJson);
  server.serveStatic("/", LittleFS, "/www/").setDefaultFile("index.html");
  server.onNotFound(notFound);
}
 

void setup() {
  Serial.begin(115200);
  setup_wifi();
  LittleFS.begin(); // Before routing!

  restServerRouting();
  server.begin();
  // timeClient.begin();

  Serial.println("");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // timeClient.update();
  sht30.get();
  StaticJsonDocument<200> doc;
  doc["sensor"] = "sht30";
  doc["temperature"] = String(sht30.cTemp);
  doc["humidity"] = String(sht30.humidity);
  doc["mac"] = WiFi.macAddress();
  buf = "";
  serializeJsonPretty(doc, buf);
  delay(1000);
}

