#include "n1spxWiFi.h"

int ssid = 0;

void _WiFiConnect(const char* ssid, const char* password) {
  int tryCount = 0;
  #ifdef WIFI_VERBOSE
    Serial.println("");
    Serial.print("Connecting to ");
    Serial.println(ssid);
  #endif
  WiFi.begin(ssid, password);
  while ((WiFi.status() != WL_CONNECTED) && (tryCount < 5)) {
    delay(500);
    #ifdef WIFI_VERBOSE
      Serial.print(".");
    #endif
    tryCount++;
  }
}

int _oneWiFiConnect() {
  _WiFiConnect(ssid1, password1);
  if (WiFi.status() == WL_CONNECTED) {
    ssid = 1;
    #ifdef WIFI_VERBOSE
      Serial.println("");
      Serial.println("WiFi connected.");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
    #endif
    return 1;
  }
  else {
    #ifdef WIFI_VERBOSE
      Serial.println("");
      Serial.println("WiFi NOT connected.");
    #endif
    ssid = 0;
    return 0;
  }
}

int _twoWiFiConnect() {
  _WiFiConnect(ssid1, password1);
  if (WiFi.status() == WL_CONNECTED) {
    ssid = 1;
  }
  else {
    _WiFiConnect(ssid2, password2);
    if (WiFi.status() == WL_CONNECTED) {
      ssid = 2;
    }
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi connected.\nIP address: ");
    Serial.println(WiFi.localIP());
    return 1;
  }
  else {
    #ifdef WIFI_VERBOSE
      Serial.println("");
      Serial.println("WiFi NOT connected.");
    #endif
    ssid = 0;
    return 0;
  }
}

int WiFiConnect() {
  if (backup_wifi) {
    return _twoWiFiConnect();
  }
  else {
    return _oneWiFiConnect();
  }
}

int WiFiCheck() {
  if (WiFi.status() != WL_CONNECTED) {
    ssid = 0;
    #ifdef WIFI_VERBOSE
      Serial.println("WiFi NOT connected.");
    #endif
    return (WiFiConnect() + 1);
  }
  else {
    return 0;
  }
}