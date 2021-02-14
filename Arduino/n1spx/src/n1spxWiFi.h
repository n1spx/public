#ifndef _N1SPXWIFI_H_
#define _N1SPXWIFI_H_

// #define WIFI_VERBOSE

#include <WiFi.h>

// These will defined in the actual sketch directory
extern const char* ssid1;
extern const char* ssid2;
extern const char* password1;
extern const char* password2;
extern bool backup_wifi;

// This lets you check which WiFi (if) you are connected to at any time
extern int ssid;

// Internal calls for WiFiConnect to pick from
int _oneWiFiConnect();
int _twoWiFiConnect();
void _WiFiConnect();

int WiFiConnect();

int WiFiCheck();

#endif