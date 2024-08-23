// Timestamp
#include <NTPClient.h>
#include <WiFi.h>
#include <WiFiUdp.h>

// Disk access
#include <Preferences.h>
Preferences preferences;

const char *ssid     = "Teles PC";
const char *password = "987654321";

int initTime, newTime, timelapse0, timelapse1, timelapse2;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

void setup(){
  Serial.begin(115200);

  // Visual feedback
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  WiFi.begin(ssid, password);

  Serial.print("Connecting to Wi-Fi");
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  timeClient.begin();
  timeClient.update();
  initTime = timeClient.getEpochTime();

  // pref.begin("",T/F): True -> Read only | False -> Read/Write
  preferences.begin("Timelapse", false);
  // Couple of buffers in case of power flicker (maybe just keep the longest timelapse instead)
  timelapse0 = preferences.getInt("timelapse0", 0);
  timelapse1 = preferences.getInt("timelapse1", 0);
  timelapse2 = preferences.getInt("timelapse2", 0);
  preferences.putInt("timelapse2", timelapse1);
  preferences.putInt("timelapse1", timelapse0);
  preferences.end();

  Serial.print("Last timelapse was ");
  Serial.print(timelapse0);
  Serial.println(" seconds long.");

  Serial.print("The timelapse before that was ");
  Serial.print(timelapse1);
  Serial.println(" seconds long.");

  Serial.print("And before that ");
  Serial.print(timelapse2);
  Serial.println(" seconds long.");
}

void loop() {
  timeClient.update();
  newTime = timeClient.getEpochTime();
  timelapse0 = newTime - initTime;
  Serial.println(timelapse0);

  // pref.begin("",T/F): True -> Read only | False -> Read/Write
  preferences.begin("Timelapse", false);
  preferences.putInt("timelapse0", timelapse0);
  preferences.end();

  // Update every 1 second
  delay(1000);
}