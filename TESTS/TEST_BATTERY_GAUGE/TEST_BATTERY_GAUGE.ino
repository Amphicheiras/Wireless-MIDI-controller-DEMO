//
// A simple server implementation showing how to:
//  * serve static messages
//  * read GET and POST parameters
//  * handle missing pages / 404s
//#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>

#include <Preferences.h>
#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include <Adafruit_NeoPixel.h>
#define LED_BUILTIN 22

#define LED_STRIP_PIN  4   // (Yellow)
#define LED_NUM_PIXELS 8
Adafruit_NeoPixel LED_STRIP(LED_NUM_PIXELS, LED_STRIP_PIN, NEO_GRB + NEO_KHZ800);

AsyncWebServer server(80);
#define SerialMon Serial
#include <AppleMIDI_Debug.h>

#include <AppleMIDI.h>

unsigned long t0 = millis();
int8_t isConnected = 0;

APPLEMIDI_CREATE_DEFAULTSESSION_INSTANCE();
Preferences preferences;
const char* ssid = "Teles PC";
const char* password = "987654321";

const char* PARAM_MESSAGE = "message";
const int Analog_channel_pin= 32;
float voltage{0.0};
float voltage_percent{0.0};

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);
    delay(1000);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.printf("WiFi Failed!\n");
        return;
    }

    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", String(voltage_percent));
    });

    server.on("/v", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", String(voltage));
    });

    // Send a GET request to <IP>/get?message=<message>
    server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
        String message;
        if (request->hasParam(PARAM_MESSAGE)) {
            message = request->getParam(PARAM_MESSAGE)->value();
        } else {
            message = "No message sent";
        }
        request->send(200, "text/plain", "Hello, GET: " + message);
    });

    // Send a POST request to <IP>/post with a form field message set to <message>
    server.on("/post", HTTP_POST, [](AsyncWebServerRequest *request){
        String message;
        if (request->hasParam(PARAM_MESSAGE, true)) {
            message = request->getParam(PARAM_MESSAGE, true)->value();
        } else {
            message = "No message sent";
        }
        request->send(200, "text/plain", "Hello, POST: " + message);
    });

    server.onNotFound(notFound);

    server.begin();

  LED_STRIP.clear();
  LED_STRIP.setPixelColor(0, LED_STRIP.Color(255,255,255));
  LED_STRIP.setPixelColor(1, LED_STRIP.Color(255,255,255));
  LED_STRIP.setPixelColor(2, LED_STRIP.Color(255,255,255));
  LED_STRIP.setPixelColor(3, LED_STRIP.Color(255,255,255));
  LED_STRIP.setPixelColor(4, LED_STRIP.Color(255,255,255));
  LED_STRIP.setPixelColor(5, LED_STRIP.Color(255,255,255));
  LED_STRIP.setPixelColor(6, LED_STRIP.Color(255,255,255));
  LED_STRIP.setPixelColor(7, LED_STRIP.Color(255,255,255));
  LED_STRIP.setBrightness(255);
  LED_STRIP.show();
  digitalWrite(LED_BUILTIN, LOW);

  // preferences.begin("my-app", false);

  // voltage = preferences.getLong("voltage", 0);
  // voltage_percent = preferences.getLong("voltage_percent", 0);

  // voltage = 2 * ((analogRead(Analog_channel_pin) * 3.3 ) / (4095) + 0.14);
  // voltage_percent = map(voltage, 2.9, 4.5, 0, 100);

  // preferences.putLong("voltage", voltage);
  // preferences.putLong("voltage_percent", voltage_percent);

  // preferences.end();

  // Serial.println(voltage);
  // Serial.println(voltage_percent);

  DBG(F("OK, now make sure you an rtpMIDI session that is Enabled"));
  DBG(F("Add device named Arduino with Host"), WiFi.localIP(), "Port", AppleMIDI.getPort(), "(Name", AppleMIDI.getName(), ")");
  DBG(F("Select and then press the Connect button"));
  DBG(F("Then open a MIDI listener and monitor incoming notes"));
  DBG(F("Listen to incoming MIDI commands"));

  MIDI.begin();

  AppleMIDI.setHandleConnected([](const APPLEMIDI_NAMESPACE::ssrc_t & ssrc, const char* name) {
    isConnected++;
    DBG(F("Connected to session"), ssrc, name);
  });
  AppleMIDI.setHandleDisconnected([](const APPLEMIDI_NAMESPACE::ssrc_t & ssrc) {
    isConnected--;
    DBG(F("Disconnected"), ssrc);
  });
  
  MIDI.setHandleNoteOn([](byte channel, byte note, byte velocity) {
    DBG(F("NoteOn"), note);
  });
  MIDI.setHandleNoteOff([](byte channel, byte note, byte velocity) {
    DBG(F("NoteOff"), note);
  });

  DBG(F("Sending NoteOn/Off of note 45, every second"));
}

void loop(){
  // Listen to incoming notes
  MIDI.read();
  t0 = millis();

  byte note = 45;
  byte velocity = 55;
  byte channel = 1;

  MIDI.sendNoteOn(note, velocity, channel);
  MIDI.sendNoteOff(note, velocity, channel);
  MIDI.sendNoteOn(note, velocity, channel+1);
  MIDI.sendNoteOff(note, velocity, channel+1);
  MIDI.sendNoteOn(note, velocity, channel+2);
  MIDI.sendNoteOff(note, velocity, channel+2);

  voltage = 2 * ((analogRead(Analog_channel_pin) * 3.3 ) / (4095) + 0.14);
  voltage_percent = map(voltage, 2.9, 4.2, 0, 100);
  // Serial.println(voltage);
  // Serial.println(voltage_percent);
  // delay(1000);
}