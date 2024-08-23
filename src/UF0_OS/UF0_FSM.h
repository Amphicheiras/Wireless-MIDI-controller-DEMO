#ifndef UF0_FSM_h
#define UF0_FSM_h

// Preliminary
// Enterprise
#include "UF0_MUSIC/UF0_MIDI.h"
#include "UF0_HARDWARE/UF0_GYRO.h"
// I2C
#include <Wire.h>
#define I2C_SCL 22
#define I2C_SDA 23
// // SPI
// #include <SPI.h>
// #define SPI_CS 5
// #define SPI_CLK 22
// #define SPI_MOSI 19
// Disk access
#include <Preferences.h>
// Main DBG
#define monitorActive true
#define monitorHz 500
// Timers
unsigned long FSM_t0 = millis();

void init()
{
	// Begin I2C connection
	Wire.begin(I2C_SDA, I2C_SCL);
}

class UF0_FSM
{
private:
	void setup()
	{
	}

public:
	UF0_FSM() {}

	UF0_FSM(bool cmon)
	{
		setup();
	}

	void loop()
	{
		if ((millis() - FSM_t0) > monitorHz)
		{
			DBG(F("\nRaw Roll:"), roll, F("\tMIDI Roll:"), rollMIDI);
			DBG(F("Raw Pitch:"), pitch, F("\tMIDI Pitch:"), pitchMIDI);
			DBG(F("Relative Yaw:"), yaw, F("\tMIDI Yaw:"), yawMIDI);
			DBG(F("State Roll, Pitch, Yaw:"), rollTransmit, pitchTransmit, yawTransmit);
			// DBG(F("Raw X:"), xPosition, F("\tMIDI X:"), xMIDI);
			// DBG(F("Raw Y:"), yPosition, F("\tMIDI Y:"), yMIDI);
			// DBG(F("Raw Z:"), zPosition, F("\tMIDI Z:"), zMIDI);
			// DBG(F("State X, Y, Z:"), xTransmit, yTransmit, zTransmit);
			// DBG(F("Virtual Target:"), virtualTarget);
			// DBG(F("IP:"), WiFi.localIP());
			FSM_t0 = millis();
		}
	}
};

#endif