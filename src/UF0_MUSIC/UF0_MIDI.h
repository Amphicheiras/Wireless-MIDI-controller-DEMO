#ifndef UF0_MIDI_h
#define UF0_MIDI_h

// Preliminary
  // MIDI state transmition (must be over BLACKMAGIC or undefined error)
    bool pitchTransmit{true}, rollTransmit{true}, yawTransmit{true};
  // Enterprise
    #include "UF0_HARDWARE/UF0_GYRO.h"
    #include "UF0_OS/UF0_BLACKMAGIC.h"
  // Wi-Fi
    #include <WiFi.h>
    #include <WiFiClient.h>
    #include <WiFiUdp.h>
  // MIDI over Wi-Fi (rtpMIDI)
    #include <AppleMIDI.h>
    APPLEMIDI_CREATE_DEFAULTSESSION_INSTANCE();
  //176 = 10110000 in binary, MIDI_CC command
    #define MIDI_CC 176
  // Is connected to rtp session?
    bool isConnected{false};
  // YRP After degrees2MIDI
    float pitchMIDI{0}, rollMIDI{0}, yawMIDI{0};
  // A MIDI Note buffer has 1 value
    int MIDINoteOn{0}, MIDINoteOff{0};
    int rootNote{64};
  // A MIDI Chord buffer has 12 (maybe 24?) values (-12,-11,..,-1,0,1,..,11,12)
  // Are vector values absolute MIDI Note values (ex:{..,49,52,55,..}) or relative rootNote perturbations (ex:{..,-3,0,+3,..})?
    int MIDIChordOn[12] = {0,0,0,0,0,0,0,0,0,0,0,0}, MIDIChordOff[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
    int rootChord[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
    int chordSize{12};
  // Timers
    unsigned long MIDI_t0 = millis();
//

class UF0_MIDI{
  private:
    
    void setup(){
      DBG(F("rtpMIDI RRrruUULLEeezzZZZzz!!"));
      DBG(F("Add rtpMIDI device named Arduino with IP:"));
      DBG(WiFi.localIP());
      DBG(F("@ Port"), AppleMIDI.getPort());

      MIDI.begin();

      AppleMIDI.setHandleConnected([](const APPLEMIDI_NAMESPACE::ssrc_t & ssrc, const char* name) {
        isConnected = true;
        DBG(F("Connected to rtpMIDI session with"), name);
      });
      AppleMIDI.setHandleDisconnected([](const APPLEMIDI_NAMESPACE::ssrc_t & ssrc) {
        isConnected = false;
        DBG(F("Disconnected from rtpMIDI session"));
      });
      
      // // // Lambda functions?
      //   // MIDI.setHandleNoteOn([](byte channel, byte note, byte velocity) {
      //   //   DBG(F("NoteOn"), note);
      //   // });
    }

  public:

    UF0_MIDI(){}

    UF0_MIDI(bool goo){
      setup();
    }
    // enableMIDIControl?
    void enableControl(){
      //
    }

    void disableControl(){
      //
    }

    // Drum hit gesture
    void drumHitMIDI(){
      if (drumHit()>0){
      // Send MIDI Note On when drum hit
        MIDI.sendNoteOn(40 + virtualTarget, 100, 1);
      // Send MIDI Note Off 10ms after drum hit
        if ((millis() - MIDI_t0) > 10){
          MIDI.sendNoteOff(40 + virtualTarget, 100, 1);
          MIDI_t0 = millis();
        }
      }
    }

    void loop(){
    // Listen to incoming notes
      MIDI.read();
      drumHitMIDI();
    // Transmit YPR
      if (pitchTransmit){
        pitchMIDI = degrees2MIDI(pitch, -90, 90, false, false);
        MIDI.sendControlChange(MIDI_CC, pitchMIDI, 1);
      }
      if (rollTransmit){
        rollMIDI = degrees2MIDI(roll, -90, 90, false, false);
        MIDI.sendControlChange(MIDI_CC, rollMIDI, 2);
      }
      if (yawTransmit){
        yawMIDI = degrees2MIDI(yaw, -90, 90, true, false);
        MIDI.sendControlChange(MIDI_CC, yawMIDI, 3);
      }
    }

    void sendChordOn(int* inMIDIChord, DataByte inVelocity, Channel inChannel){
      for (int i=0; i<sizeof(inMIDIChord); i++){
        MIDI.sendNoteOn(inMIDIChord[i], inVelocity, inChannel);
      }
    }

    void sendChordOff(int* inMIDIChord, DataByte inVelocity, Channel inChannel){
      for (int i=0; i<sizeof(inMIDIChord); i++){
        MIDI.sendNoteOff(inMIDIChord[i], inVelocity, inChannel);
      }
    }

    void startTransmission(bool* toStart){
      *toStart = true;
    }
    void stopTransmission(bool* toStop){
      *toStop = false;
    }
    void toggleTransmission(bool* toToggle){
      *toToggle = !*toToggle;
    }
    void startTransmissionAll(){
      startTransmission(&pitchTransmit);
      startTransmission(&rollTransmit);
      startTransmission(&yawTransmit);
    }
    void stopTransmissionAll(){
      stopTransmission(&pitchTransmit);
      stopTransmission(&rollTransmit);
      stopTransmission(&yawTransmit);
    }
    void soloTransmission(bool* toSolo){
      stopTransmissionAll();
      // *toSolo = true;
      startTransmission(toSolo);
    }
    void sendMIDINoteOn(int note, int velocity, int channel){
      MIDI.sendNoteOn(note, velocity, channel);
    }
    void sendMIDINoteOff(int note, int velocity, int channel){
      MIDI.sendNoteOff(note, velocity, channel);
    }

    // tilt: Gyro input, fromAngle: lowerLimit, toAngle: higherLimit, shift: move angle by 'shift', invert: invert tilt direction
    double degrees2MIDI(double tilt, int fromAngle, int toAngle, bool shift, bool invert){
      // Map [0, 360] -> [-180, 180]
        if (shift) {
          if (tilt > 180)
            tilt = map(tilt, 360, 181, -1, -180);
        }
      // Return if out of bounds
        if (tilt < fromAngle) return 0;
        if (tilt > toAngle) return 127;
      // Map [-fromAngle, toAngle] -> [0, 127]
        if (tilt < 0)
          tilt = map(tilt, 0, fromAngle, 64, 0);
        else
          tilt = map(tilt, 0, toAngle, 64, 127);
      // Invert movement direction
        if (invert) return (127 - tilt);
        else return tilt;
    }
};

#endif