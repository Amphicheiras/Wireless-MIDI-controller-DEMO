#ifndef UF0_MIDI_h
#define UF0_MIDI_h

// Preliminary
// MIDI state transmition (must be over BLACKMAGIC or undefined error)
bool pitchTransmit{true}, rollTransmit{true}, yawTransmit{true};
bool xTransmit{true}, yTransmit{true}, zTransmit{true};
// Enterprise
// #include "UF0_HARDWARE/UF0_GYRO.h"
#include "UF0_HARDWARE/UF0_GYRO.h"
#include "UF0_OS/UF0_BLACKMAGIC.h"
// Wi-Fi
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
// MIDI over Wi-Fi (rtpMIDI)
#include <AppleMIDI.h>
APPLEMIDI_CREATE_DEFAULTSESSION_INSTANCE();
// 176 = 10110000 in binary, MIDI_CC command
#define MIDI_CC 176
// Is connected to rtp session?
bool isConnected{false};
// YRP After degrees2MIDI
float pitchMIDI{0}, rollMIDI{0}, yawMIDI{0};
// XYZ After position2MIDI
float xMIDI{0}, yMIDI{0}, zMIDI{0};
// A MIDI Note buffer has 1 value
int MIDINoteOn{0}, MIDINoteOff{0};
// Timers
unsigned long MIDI_t0 = millis();
bool controlsActive = false;

class UF0_MIDI
{
private:
	void setup()
	{
		DBG(F("rtpMIDI RRrruUULLEeezzZZZzz!!"));
		DBG(F("Add rtpMIDI device named Arduino with IP:"));
		DBG(WiFi.localIP());
		DBG(F("@ Port"), AppleMIDI.getPort());

		MIDI.begin();

		AppleMIDI.setHandleConnected([](const APPLEMIDI_NAMESPACE::ssrc_t &ssrc, const char *name)
									 {
        isConnected = true;
        DBG(F("Connected to rtpMIDI session with"), name); });
		AppleMIDI.setHandleDisconnected([](const APPLEMIDI_NAMESPACE::ssrc_t &ssrc)
										{
        isConnected = false;
        DBG(F("Disconnected from rtpMIDI session")); });

		// // // Lambda functions?
		//   // MIDI.setHandleNoteOn([](byte channel, byte note, byte velocity) {
		//   //   DBG(F("NoteOn"), note);
		//   // });
	}

public:
	UF0_MIDI() {}

	// Construct MIDI class
	UF0_MIDI(bool goo)
	{
		setup();
	}
	// enableMIDIControl?
	void enableControl()
	{
		controlsActive = true;
	}

	void disableControl()
	{
		controlsActive = false;
	}
	// Drum hit gesture
	void drumHitMIDI()
	{
		// If drum hit
		if (drumHit() > 0)
		{
			// Send MIDI Note On when drum hit
			MIDI.sendNoteOn(40 + virtualTarget, 100, 1);
			// Send MIDI Note Off 10ms after drum hit
			if ((millis() - MIDI_t0) > 10)
			{
				MIDI.sendNoteOff(40 + virtualTarget, 100, 1);
				MIDI_t0 = millis();
			}
		}
	}

	void loop()
	{
		// Listen to incoming notes
		MIDI.read();
		// drumHitMIDI();
		if (controlsActive)
		{
			sendControls();
		}
	}

	void sendControls()
	{
		// Transmit YPR
		if (pitchTransmit)
		{
			pitchMIDI = degrees2MIDI(pitch, -90, 90);
			MIDI.sendControlChange(MIDI_CC, pitchMIDI, 1);
		}
		if (rollTransmit)
		{
			rollMIDI = degrees2MIDI(roll, -90, 90);
			MIDI.sendControlChange(MIDI_CC, rollMIDI, 2);
		}
		if (yawTransmit)
		{
			yawMIDI = degrees2MIDI(yaw, -90, 90, true);
			MIDI.sendControlChange(MIDI_CC, yawMIDI, 3);
		}
		// Transmit XYZ
		if (xTransmit)
		{
			xMIDI = position2MIDI(xPosition, -90, 90, false);
			MIDI.sendControlChange(MIDI_CC, xMIDI, 4);
		}
		if (yTransmit)
		{
			yMIDI = position2MIDI(yPosition, -90, 90, false);
			MIDI.sendControlChange(MIDI_CC, yMIDI, 5);
		}
		if (zTransmit)
		{
			zMIDI = position2MIDI(zPosition, -90, 90, false);
			MIDI.sendControlChange(MIDI_CC, zMIDI, 6);
		}
	}
	// Send toggle on from trigger 3
	void toggle_T3_on()
	{
		MIDI.sendControlChange(MIDI_CC, 127, 15);
	}
	// Send toggle off from trigger 3
	void toggle_T3_off()
	{
		MIDI.sendControlChange(MIDI_CC, 0, 15);
	}

	// MIDI transmission control (mostly from website)
	// Enable MIDI transmission for toStart parameter
	void startTransmission(bool *toStart)
	{
		*toStart = true;
	}
	// Disable MIDI transmission for toStart parameter
	void stopTransmission(bool *toStop)
	{
		*toStop = false;
	}
	// Enable all MIDI transmissions
	void startTransmissionAll()
	{
		startTransmission(&pitchTransmit);
		startTransmission(&rollTransmit);
		startTransmission(&yawTransmit);
		startTransmission(&xTransmit);
		startTransmission(&yTransmit);
		startTransmission(&zTransmit);
	}
	// Disable all MIDI transmissions
	void stopTransmissionAll()
	{
		stopTransmission(&pitchTransmit);
		stopTransmission(&rollTransmit);
		stopTransmission(&yawTransmit);
		stopTransmission(&xTransmit);
		stopTransmission(&yTransmit);
		stopTransmission(&zTransmit);
	}
	// Solo MIDI transmission for toSolo parameter
	void soloTransmission(bool *toSolo)
	{
		stopTransmissionAll();
		// *toSolo = true;
		startTransmission(toSolo);
	}
	// Send MIDI note on
	void sendMIDINoteOn(int note, int velocity, int channel)
	{
		MIDI.sendNoteOn(note, velocity, channel);
	}
	// Send MIDI note off
	void sendMIDINoteOff(int note, int velocity, int channel)
	{
		MIDI.sendNoteOff(note, velocity, channel);
	}
	// Send MIDI note on
	void sendMIDIChordOn(int note, int velocity, int channel)
	{
		MIDI.sendNoteOn(note, velocity, channel);
		MIDI.sendNoteOn(note, velocity, channel);
		MIDI.sendNoteOn(note+3, velocity, channel);
		MIDI.sendNoteOn(note+5, velocity, channel);
	}
	// Send MIDI note off
	void sendMIDIChordOff(int note, int velocity, int channel)
	{
		MIDI.sendNoteOff(note, velocity, channel);
		MIDI.sendNoteOff(note, velocity, channel);
		MIDI.sendNoteOff(note+3, velocity, channel);
		MIDI.sendNoteOff(note+5, velocity, channel);
	}
	// tilt: Gyro input, fromAngle: lowerLimit, toAngle: higherLimit, shift: move angle by 'shift', invert: invert tilt direction
	double degrees2MIDI(double tilt, int fromAngle, int toAngle, bool shift = false, bool invert = false)
	{
		// Map [0, 360] -> [-180, 180]
		if (shift)
		{
			if (tilt > 180)
				tilt = map(tilt, 360, 181, -1, -180);
		}
		// Return if out of bounds
		if (tilt < fromAngle)
			return 0;
		if (tilt > toAngle)
			return 127;
		// Map [-fromAngle, toAngle] -> [0, 127]
		if (tilt < 0)
			tilt = map(tilt, 0, fromAngle, 64, 0);
		else
			tilt = map(tilt, 0, toAngle, 64, 127);
		// Invert movement direction
		if (invert)
			return (127 - tilt);
		else
			return tilt;
	}
	// position: Gyro input, fromPosition: lowerLimit, toPosition: higherLimit, invert: invert position direction
	double position2MIDI(double position, int fromPosition, int toPosition, bool invert)
	{
		// Return if out of bounds
		if (position < fromPosition)
			return 0;
		if (position > toPosition)
			return 127;
		// Map [-fromAngle, toAngle] -> [0, 127]
		if (position < 0)
		{
			position = map(position, 0, fromPosition, 64, 0);
		}
		else
		{
			position = map(position, 0, toPosition, 64, 127);
		}
		// Invert movement direction
		if (invert)
		{
			return (127 - position);
		}
		else
		{
			return position;
		}
	}
};

#endif
