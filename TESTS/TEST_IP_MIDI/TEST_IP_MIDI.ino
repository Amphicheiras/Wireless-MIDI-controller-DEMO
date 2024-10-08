#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>

#include <ipMIDI.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

const char ssid[] = "Teles PC";
const char pass[] = "987654321";

IPMIDI_CREATE_INSTANCE(WiFiUDP, MIDI, 21928);

unsigned long t1 = millis();

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void setup()
{
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Booting");

  Serial.println(F("Getting IP address..."));

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));

  }

  Serial.println(F("WiFi connected"));
  Serial.print(F("IP address is "));
  Serial.println(WiFi.localIP());

  // Listen for MIDI messages on channel 1
  MIDI.begin(1);

  MIDI.setHandleNoteOn(OnMidiNoteOn);
  MIDI.setHandleNoteOff(OnMidiNoteOff);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void loop()
{
  MIDI.read();

  if ((millis() - t1) > 500)
  {
    t1 = millis();

    byte note = random(1, 127);
    byte velocity = 55;
    byte channel = 1;

    Serial.print("wtf\n");
    MIDI.sendNoteOn(note, velocity, channel);
    MIDI.sendNoteOff(note, velocity, channel);
  }
}

// ====================================================================================
// Event handlers for incoming MIDI messages
// ====================================================================================

// -----------------------------------------------------------------------------
// received note on
// -----------------------------------------------------------------------------
void OnMidiNoteOn(byte channel, byte note, byte velocity) {
  Serial.print(F("Incoming NoteOn  from channel: "));
  Serial.print(channel);
  Serial.print(F(", note: "));
  Serial.print(note);
  Serial.print(F(", velocity: "));
  Serial.println(velocity);
}


// -----------------------------------------------------------------------------
// received note off
// -----------------------------------------------------------------------------
void OnMidiNoteOff(byte channel, byte note, byte velocity) {
  Serial.print(F("Incoming NoteOff from channel: "));
  Serial.print(channel);
  Serial.print(F(", note: "));
  Serial.print(note);
  Serial.print(F(", velocity: "));
  Serial.println(velocity);
}
