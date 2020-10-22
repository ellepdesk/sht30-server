#include "wifi_manager.h"
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic

void setup_wifi()
{
  WiFiManager wifiManager;
  wifiManager.autoConnect("sensor");
}