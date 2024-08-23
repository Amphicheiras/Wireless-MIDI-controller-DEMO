#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>

#define SerialMon Serial
#include <AppleMIDI_Debug.h>

#include <AppleMIDI.h>

// Timestamp
#include <NTPClient.h>

// Disk access
#include <Preferences.h>
Preferences preferences;

// long timeIn{0}, timeOut{0}, timeDiff{0};

const char *ssid     = "Teles PC";
const char *password = "987654321";

unsigned long initTime{0}, newTime{0}, timelapse0{0}, timelapse1{0}, timelapse2{0};

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

unsigned long t0 = millis();
int8_t isConnected{0};
byte note{45}, velocity{55}, channel{1};

APPLEMIDI_CREATE_DEFAULTSESSION_INSTANCE();

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void setup()
{
  DBG_SETUP(115200);
  DBG("Booting");

  // Visual feedback
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
    DBG("Establishing connection to WiFi..");
  }
  DBG("Connected to network");

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
  
  /////////////////// TIMELAPSE //////////////////
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

  DBG("\n\nLast timelapse was", timelapse0, "seconds long.");
  DBG("Timelapse before that was", timelapse1, "seconds long.");
  DBG("And before that", timelapse2, "seconds long.");

  // WiFi.setSleep(WIFI_PS_MIN_MODEM);
  // while (!WiFi.setSleep(false)) {
  //    delay(500);
  //   DBG("Setting sleep..");
  // }
  // DBG("Modem sleep type is", WiFi.getSleep());
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

void loop()
{
  // Listen to incoming notes
  MIDI.read();

  if ((isConnected > 0) && (millis() - t0) > 20)
  {
    t0 = millis();
  
    timeClient.update();
    newTime = timeClient.getEpochTime();
    timelapse0 = newTime - initTime;

    // pref.begin("",T/F): True -> Read only | False -> Read/Write
    preferences.begin("Timelapse", false);
    preferences.putInt("timelapse0", timelapse0);
    preferences.end();

    MIDI.sendNoteOn(note, velocity, 1);
    MIDI.sendNoteOff(note, velocity, 1);

    MIDI.sendNoteOn(note, velocity, 2);
    MIDI.sendNoteOff(note, velocity, 2);
    
    MIDI.sendNoteOn(note, velocity, 3);
    MIDI.sendNoteOff(note, velocity, 3);
    
    MIDI.sendNoteOn(note, velocity, 4);
    MIDI.sendNoteOff(note, velocity, 4);
    
    MIDI.sendNoteOn(note, velocity, 5);
    MIDI.sendNoteOff(note, velocity, 5);
  }
}
