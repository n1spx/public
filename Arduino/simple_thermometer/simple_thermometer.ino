// This is a battery backed portable thermometer,
// using a Dallas 18B20 chip to read the temperature
// 
// It connects to the WiFi and then listens for connections,
// answering with a JSON formatted string:
// {
//    "wifi":{
//       "rssi":-47,
//       "ssid":"n1spx"
//    },
//    "voltage":4.17,
//    "temperature":67.18
// }

// Create a file with the following structure:
// For ONE WiFi network:
//   const char* ssid1 = "SSID1_HERE";
//   const char* password1 = "PASSWORD1_HERE";
// For TWO WiFi networks, above plus:
//   const char* ssid2 = "SSID2_HERE";
//   const char* password2 = "PASSWORD2_HERE";
// At the bottom, add the appropriate one:
//   bool backup_wifi = true;
//   bool backup_wifi = false;

#include "wifiCreds.h"

// Then include n1spxWifi.h here
#include <n1spxWiFi.h>

// Which pin is the DS18B20 data pin connected to?
#define PROBE_PIN 15

// Return temperature in C or F?
// #define C_or_F probe.getTempC
// or
#define C_or_F probe.getTempF

// Which pin is the voltage divider connected to?
#define VOLTAGE_PIN 32
// With this set to 1/1, measure the true battery voltage
// then set this to ( MEASURED_VOLTAGE / REPORTED_VOLTAGE )
// This accounts for resistor variance, and the input impedance
// of the ESP32 pin
#define VOLTAGE_CORRECTION ( 4.22 / 3.84 )

// How often the average should be updated? (milliseconds)
#define ROLL_DELAY 6000
// How many samples should be considered?
#define ROLL_COUNT 10

// What port should the device listen on?
// Setting this to 80 allows simple calls:
// http://IP.OF.THIS.DEVICE/
#define SERVER_PORT 80

// Seems to run OK at this speed, saving quite a bit of power
// If we add an OLED display later, probably needs to be 160
#define CPU_FREQ 80


// **************
// Typical per-use configuration ends here
// **************

// Include functions for rolling averages
#include <rolling.h>

#include <ArduinoJson.h>
#include <TimedAction.h>
#include <DS18B20.h>

StaticJsonDocument<300> JSONbuffer;
JsonObject JSONWiFi;

// Fix JSON digits
inline double round2(double value) {
   return (int)(value * 100 + 0.5) / 100.0;
}

DS18B20 probe(PROBE_PIN);

WiFiServer server(SERVER_PORT);

Rolling temperature(10);
Rolling ADC(10);

void getRolls() {
  float newTemp = C_or_F();
  Serial.println(newTemp);
  temperature.upd(newTemp);
  ADC.upd(analogRead(VOLTAGE_PIN));
}

TimedAction updateRolls(ROLL_DELAY, getRolls);

void setup() {
  setCpuFrequencyMhz(CPU_FREQ);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFiConnect();

  pinMode(VOLTAGE_PIN, INPUT);
  ADC.init(analogRead(VOLTAGE_PIN));

  server.begin();
  // DS18B20 always returns garbage on the first read, so eat it
  C_or_F();
  temperature.init(C_or_F());

  JSONWiFi = JSONbuffer.createNestedObject("wifi");
}

void loop() {
  updateRolls.check();
  WiFiCheck();
  WiFiClient client = server.available();
  if (client) {
    char buf[100];
    JSONWiFi["rssi"] = WiFi.RSSI();
    JSONWiFi["ssid"] = WiFi.SSID();
    JSONbuffer["voltage"] = round2(ADC.avg * 3.3 * VOLTAGE_CORRECTION / 2048.0); // Voltage divider 50/50 plus correction
    JSONbuffer["temperature"] = round2(temperature.avg);
    serializeJson(JSONbuffer, buf, sizeof(buf));
    client.print(buf);
    client.flush();
    client.stop();
  }
}
