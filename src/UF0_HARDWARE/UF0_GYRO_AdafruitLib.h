#ifndef UF0_GYRO_ICM_20948_ADAFRUIT_h
#define UF0_GYRO_ICM_20948_ADAFRUIT_h

#include <Adafruit_Sensor.h>
#include <Adafruit_ICM20X.h>
#include <Adafruit_ICM20948.h>

// SPI setup
#define ICM_SCK 14
#define ICM_MOSI 13
#define ICM_MISO 12
#define ICM_CS 2

// ICM setup
Adafruit_ICM20948 icm;
Adafruit_Sensor *accel;

// PRY raw values
float pitch, roll, yaw; 
// XYZ acceleration values
float xAcceleration, yAcceleration, zAcceleration;
// XYZ position raw values
float xPosition, yPosition, zPosition;
// Heading deviation from Svalbard
double initHeading;
// Acceleration MATH for drum hit
double oldAccelZ{0}, newAccelZ{0}, highestAccelZ{0}, highestDiff{0}, accelDiff{0};
// For drum hit
int hit{-1};
int drumHitDebounce{100} /*(ms)*/;
int drumHitThreshold{650};
// Timers
unsigned long GYRO_t0 = millis(), GYRO_t1 = millis();
// 3D position
float velocity[3];
float displacement[3];
float orientation;
float positionCurrent[3];
float positionPrevious[3];

class UF0_GYRO
{
private:
	void setup()
	{
		if (!icm.begin_SPI(ICM_CS, ICM_SCK, ICM_MISO,  ICM_MOSI)) {
			Serial.println("Failed to find ICM20948 chip");
			while (1) {
				delay(10);
			}
		}

		icm.enableAccelDLPF(true, ICM20X_ACCEL_FREQ_5_7_HZ);

		// Get an Adafruit_Sensor compatible object for the ICM20948's accelerometer
		accel = icm.getAccelerometerSensor();
	}

public:
	UF0_GYRO() {}

	UF0_GYRO(bool cmon)
	{
		setup();
	}

	void loop()
	{
		// Get a new normalized sensor event
		sensors_event_t a;

		// fill the event with the most recent data
		accel->getEvent(&a);

		Serial.print(a.acceleration.x);
		Serial.print(","); Serial.print(a.acceleration.y);
		Serial.print(","); Serial.print(a.acceleration.z);

		Serial.println();

		delayMicroseconds((float)10/1000);
	}
};

/*/////////// 3D Position /////////

- deltaTime?

- figure out quaternion math and variables

- figure out linear algebra for orientation


*/
/////////////////////////////////////

void getDisplacement(float ax, float ay, float az, float deltaTime, float *velocity, float *displacement)
{
  // Assuming ax, ay, az are accelerometer readings in m/s²
  // deltaTime is the time difference between readings in seconds (??)

  // Update velocity by integrating acceleration
	velocity[0] += ax * deltaTime; // Velocity in x-direction
	velocity[1] += ay * deltaTime; // Velocity in y-direction
	velocity[2] += az * deltaTime; // Velocity in z-direction

  // Update position by integrating velocity
	displacement[0] = velocity[0] * deltaTime; // Position in x-direction
	displacement[1] = velocity[1] * deltaTime; // Position in y-direction
	displacement[2] = velocity[2] * deltaTime; // Position in z-direction

	// DBG("velocity X:", velocity[0], ", velocity Y:", velocity[1], ", velocity Z:", velocity[2], ", displacement X:", displacement[0], ", displacement Y:", displacement[1], ", displacement Z:", displacement[2]);
}

void getDisplacementOrientation(float orientation)
{
	// quaternion N stuff
}

void getRelevantDisplacement(float *displacement)
{
	getDisplacement(xAcceleration, yAcceleration, zAcceleration, 0.01, velocity, displacement);
	getDisplacementOrientation(orientation);

	// rotate movement vectors using ~LINEAR ALBEGRA~
	// displacement[0] += orientation[0]; // + LINEAR ALBEGRA
	// displacement[1] += orientation[1]; // + LINEAR ALBEGRA
	// displacement[2] += orientation[2]; // + LINEAR ALBEGRA
}

void updateRelevantPosition(float *positionCurrent, float *positionPrevious, float *displacement)
{
	getRelevantDisplacement(displacement);
	positionCurrent[0] = positionPrevious[0] + displacement[0];
	positionCurrent[1] = positionPrevious[1] + displacement[1];
	positionCurrent[2] = positionPrevious[2] + displacement[2];
}

float get3DPosition(float *positionCurrent, float *positionPrevious, float *displacement)
{
	updateRelevantPosition(positionCurrent, positionPrevious, displacement);
	return *positionCurrent;
}

// Drum hit gesture
int drumHit()
{
	if ((millis() - GYRO_t1) > drumHitDebounce)
	{
		accelDiff = newAccelZ - oldAccelZ;
		oldAccelZ = newAccelZ;
		if (accelDiff > drumHitThreshold)
		{
			// If hit, return velocity!
			return map(accelDiff - drumHitThreshold, 0, 2000, 0, 127);
		}
		else
		{
			return -1;
		}
		GYRO_t1 = millis();
	}

	// // Leave for drum hit optimization
	// if (newAccelZ > highestAccelZ)
	//   highestAccelZ = newAccelZ;
	// if (newAccelZ - oldAccelZ > highestDiff)
	//   highestDiff = newAccelZ - oldAccelZ;
	// if ((millis() - GYRO_t0) > 1000){
	//     DBG("");
	//     DBG("New:", newAccelZ);
	//     DBG("Old:", oldAccelZ);
	//     DBG("New - old:", newAccelZ - oldAccelZ);
	//     DBG("High:", highestAccelZ);
	//     DBG("Highest diff:", highestDiff);
	//     GYRO_t0 = millis();
	// }
	return -1;
}

#endif