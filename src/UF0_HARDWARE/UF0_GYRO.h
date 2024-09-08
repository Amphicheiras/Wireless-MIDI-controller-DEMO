#ifndef UF0_GYRO_ICM_20948_h
#define UF0_GYRO_ICM_20948_h

/*
	Get orientation with quaternia

	Change connection to SPI

	Fix 3D space

*/



// Preliminary
#include <SPI.h>
#include "ICM_20948.h"

#define WIRE_PORT Wire

// Create an instance of SPIClass with VSPI or HSPI
// SPIClass mySPI(VSPI); // Use VSPI or HSPI depending on your preference

// // SPI setup
// // #define SPI_PORT mySPI // Your desired SPI port.       Used only when "USE_SPI" is defined
// #define SPI_PORT SPI // Your desired SPI port.       Used only when "USE_SPI" is defined
// #define SPI_SCL 22
// #define SPI_MOSI 23
// #define SPI_MISO 19
// #define SPI_CS 5

ICM_20948_I2C myICM; // If using SPI create an ICM_20948_SPI object
// ICM_20948_SPI myICM; // If using SPI create an ICM_20948_SPI object

// YPR raw values
float pitch, roll, yaw;
// XYZ acceleration values
float ax, ay, az;
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
unsigned long GYRO_t0 = millis(), GYRO_t1 = millis(), GYRO_t_accel = millis(), lastTime = 0;
// 3D position
float velocity[3];
float displacement[3];
float orientation;
float positionCurrent[3];
float positionPrevious[3];
int accelReadTime = 20;

icm_20948_DMP_data_t data;

/*/////////// 3D Position /////////

- compensate gravity on all axes

- figure out quaternion math and variables

- figure out linear algebra for orientation


*/
/////////////////////////////////////

// ax, ay, az: accelerometer readings in m/s², dT: time difference between readings (ms), velocity: variable, displacement: variable
void getAbsoluteDisplacement(float ax, float ay, float az, float dT, float *velocity, float *displacement)
{		
	// Time step (in seconds)
	unsigned long currentTime = millis();
	dT = (currentTime - lastTime) / 1000.0;
	lastTime = currentTime;
	// Update velocity by integrating acceleration (m/s)
	velocity[0] += ax * dT;
	velocity[1] += ay * dT;
	velocity[2] += az * dT;
	// Update displacement by integrating velocity (m)
	displacement[0] += velocity[0] * dT;
	displacement[1] += velocity[1] * dT;
	displacement[2] += velocity[2] * dT;

	DBG("ax:", ax, "ay:", ay, "az:", az, "velocity X:", velocity[0], ", velocity Y:", velocity[1], ", velocity Z:", velocity[2], ", displacement X:", displacement[0], ", displacement Y:", displacement[1], ", displacement Z:", displacement[2]);
}

void getDisplacementOrientation(float orientation)
{
	// quaternion N stuff
}

void getRelevantDisplacement(float *displacement)
{
	getAbsoluteDisplacement(ax, ay, az, 0.01, velocity, displacement);
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

void getSensorReadings()
{
	myICM.readDMPdataFromFIFO(&data);

	if ((myICM.status == ICM_20948_Stat_Ok) || (myICM.status == ICM_20948_Stat_FIFOMoreDataAvail)) // Was valid data available?
	{
		if ((data.header & DMP_header_bitmap_Quat6) > 0) // We have asked for GRV data so we should receive Quat6
		{
			// Q0 value is computed from this equation: Q0^2 + Q1^2 + Q2^2 + Q3^2 = 1.
			// In case of drift, the sum will not add to 1, therefore, quaternion data need to be corrected with right bias values.
			// The quaternion data is scaled by 2^30.

			// Scale to +/- 1
			double q1 = ((double)data.Quat6.Data.Q1) / 1073741824.0; // Convert to double. Divide by 2^30
			double q2 = ((double)data.Quat6.Data.Q2) / 1073741824.0; // Convert to double. Divide by 2^30
			double q3 = ((double)data.Quat6.Data.Q3) / 1073741824.0; // Convert to double. Divide by 2^30

			// Convert the quaternions to Euler angles (roll, pitch, yaw)
			// https://en.wikipedia.org/w/index.php?title=Conversion_between_quaternions_and_Euler_angles&section=8#Source_code_2

			double q0 = sqrt(1.0 - ((q1 * q1) + (q2 * q2) + (q3 * q3)));
			double q2sqr = q2 * q2;

			// roll (x-axis rotation)
			double t0 = +2.0 * (q0 * q1 + q2 * q3);
			double t1 = +1.0 - 2.0 * (q1 * q1 + q2sqr);
			roll = -atan2(t0, t1) * 180.0 / PI;

			double sin_pitch = 2.0 * (q0 * q2 - q3 * q1);
			double cos_pitch = 1.0 - 2.0 * (q1 * q1 + q2 * q2);
			pitch = atan2(sin_pitch, cos_pitch) * 180.0 / PI;

			// yaw (z-axis rotation)
			double t3 = +2.0 * (q0 * q3 + q1 * q2);
			double t4 = +1.0 - 2.0 * (q2sqr + q3 * q3);
			yaw = -atan2(t3, t4) * 180.0 / PI;

			// Update AGMT values
			if (myICM.dataReady())
			{
				myICM.getAGMT();
				// Raw accelerometer readings in m/s^2
				ax = myICM.accX() / 1000;
				ay = myICM.accY() / 1000;
				az = myICM.accZ() / 1000;
			}
		}
	}
}

class UF0_GYRO
{
private:
	void setup()
	{
		WIRE_PORT.begin(23, 22);
		WIRE_PORT.setClock(400000);
		// Initialize SPI
		// SPI_PORT.begin();
		// mySPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI, SPI_CS);
		delay(100);

		// Enable helpful debug messages
		myICM.enableDebugging();

		bool initialized = false;
		while (!initialized)
		{
			// Initialize the ICM-20948
			// myICM.begin(SPI_CS, mySPI);
			myICM.begin(WIRE_PORT, 1);

			DBG(F("Initialization of the sensor returned: "), myICM.statusString());
			if (myICM.status != ICM_20948_Stat_Ok)
			{
				DBG(F("Trying again..."));
				delay(500);
			}
			else
			{
				initialized = true;
			}
		}
		DBG(F("IMU connected!"));

		// Use success to show if the DMP configuration was successful
		bool success = true;

		// Initialize the DMP. initializeDMP is a weak function. You can overwrite it if you want to e.g. to change the sample rate
		success &= (myICM.initializeDMP() == ICM_20948_Stat_Ok);
		// Enable the DMP Game Rotation Vector sensor
		success &= (myICM.enableDMPSensor(INV_ICM20948_SENSOR_GAME_ROTATION_VECTOR) == ICM_20948_Stat_Ok);
		// Enable any additional sensors / features
		success &= (myICM.enableDMPSensor(INV_ICM20948_SENSOR_RAW_ACCELEROMETER) == ICM_20948_Stat_Ok);
		// Configuring DMP to output data at multiple ODRs:
		// DMP is capable of outputting multiple sensor data at different rates to FIFO.
		// Setting value can be calculated as follows:
		// Value = (DMP running rate / ODR ) - 1
		// E.g. For a 5Hz ODR rate when DMP is running at 55Hz, value = (55/5) - 1 = 10.
		success &= (myICM.setDMPODRrate(DMP_ODR_Reg_Quat6, 0) == ICM_20948_Stat_Ok); // Set to the maximum
		// Enable the FIFO
		success &= (myICM.enableFIFO() == ICM_20948_Stat_Ok);
		// Enable the DMP
		success &= (myICM.enableDMP() == ICM_20948_Stat_Ok);
		// Reset DMP
		success &= (myICM.resetDMP() == ICM_20948_Stat_Ok);
		// Reset FIFO
		success &= (myICM.resetFIFO() == ICM_20948_Stat_Ok);

		// Check success
		if (success)
		{
			DBG(F("DMP enabled!"));
		}
		else
		{
			DBG(F("Enable DMP failed!"));
			DBG(F("Please check that you have uncommented line 29 (#define ICM_20948_USE_DMP) in ICM_20948_C.h..."));
			while (1);
		}
	}

public:
	UF0_GYRO() {}

	UF0_GYRO(bool cmon)
	{
		setup();
	}

	void loop()
	{
		getSensorReadings();
		// get displacement
		getAbsoluteDisplacement(ax, ay, az, 0.01, velocity, displacement);
	}
};

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