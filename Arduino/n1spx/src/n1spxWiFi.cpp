#include "n1spxWiFi.h"

int ssid = 0;

int oneWiFiConnect() {
  int tryCount = 0;
  Serial.println("");
  Serial.print("Connecting to ");
  Serial.println(ssid1);
  WiFi.begin(ssid1, password1);
  while ((WiFi.status() != WL_CONNECTED) && (tryCount < 5)) {
    delay(500);
    Serial.print(".");
    tryCount++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    ssid = 1;
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    return 1;
  }
  else {
    Serial.println("");
    Serial.println("WiFi NOT connected.");
    ssid = 0;
    return 0;
  }
}

int twoWiFiConnect() {
  int tryCount = 0;
  Serial.println("");
  Serial.print("Connecting to ");
  Serial.println(ssid1);
  WiFi.begin(ssid1, password1);
  while ((WiFi.status() != WL_CONNECTED) && (tryCount < 5)) {
    delay(500);
    Serial.print(".");
    tryCount++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    ssid = 1;
  }
  else {
    tryCount = 0;
    Serial.println("");
    Serial.println("Backup: ");
    Serial.println(ssid2);
    WiFi.begin(ssid2, password2);
    while ((WiFi.status() != WL_CONNECTED) && (tryCount < 5)) {
      delay(500);
      Serial.print(".");
      tryCount++;
    }
    if (WiFi.status() == WL_CONNECTED) {
      ssid = 2;
    }
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    return 1;
  }
  else {
    Serial.println("");
    Serial.println("WiFi NOT connected.");
    ssid = 0;
    return 0;
  }
}

int WiFiConnect() {
  if (backup_wifi) {
    return twoWiFiConnect();
  }
  else {
    return oneWiFiConnect();
  }
}

int WiFiCheck() {
  if (WiFi.status() != WL_CONNECTED) {
    ssid = 0;
    Serial.println("WiFi NOT connected.");
    return (WiFiConnect() + 1);
  }
  else {
    return 0;
  }
}