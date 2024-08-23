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
unsigned long GYRO_t0 = millis(), GYRO_t1 = millis(), GYRO_t_accel = millis();
// 3D position
float velocity[3];
float displacement[3];
float orientation;
float positionCurrent[3];
float positionPrevious[3];
int accelReadTime = 20;

icm_20948_DMP_data_t data;

/*/////////// 3D Position /////////

- deltaTime?

- figure out quaternion math and variables

- figure out linear algebra for orientation


*/
/////////////////////////////////////

// ax, ay, az: accelerometer readings in m/s², deltaTime: time difference between readings (ms), velocity: variable, displacement: variable
void getAbsoluteDisplacement(float ax, float ay, float az, float deltaTime, float *velocity, float *displacement)
{
	// Scale dT up to seconds
	deltaTime /= 1000;
	// Update velocity by integrating acceleration (m/s)
	velocity[0] += ax * deltaTime;
	velocity[1] += ay * deltaTime;
	velocity[2] += az * deltaTime;
	// Update displacement by integrating velocity (m)
	displacement[0] += velocity[0] * deltaTime;
	displacement[1] += velocity[1] * deltaTime;
	displacement[2] += velocity[2] * deltaTime;

	// DBG("velocity X:", velocity[0], ", velocity Y:", velocity[1], ", velocity Z:", velocity[2], ", displacement X:", displacement[0], ", displacement Y:", displacement[1], ", displacement Z:", displacement[2]);
}

void getDisplacementOrientation(float orientation)
{
	// quaternion N stuff
}

void getRelevantDisplacement(float *displacement)
{
	getAbsoluteDisplacement(xAcceleration, yAcceleration, zAcceleration, 0.01, velocity, displacement);
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
			// If the DMP is enabled, .begin performs a minimal startup. We need to configure the sample mode etc. manually.
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
			while (1)
				;
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
				roll = atan2(t0, t1) * 180.0 / PI;

				// pitch (y-axis rotation)
				double t2 = +2.0 * (q0 * q2 - q3 * q1);
				t2 = t2 > 1.0 ? 1.0 : t2;
				t2 = t2 < -1.0 ? -1.0 : t2;
				pitch = asin(t2) * 180.0 / PI;

				// yaw (z-axis rotation)
				double t3 = +2.0 * (q0 * q3 + q1 * q2);
				double t4 = +1.0 - 2.0 * (q2sqr + q3 * q3);
				yaw = atan2(t3, t4) * 180.0 / PI;

				// Update AGMT values
				if (myICM.dataReady())
				{
					myICM.getAGMT();
				}
				// Compensate gravity acceleration
				if ((millis() - GYRO_t_accel) > accelReadTime){
					const float g = 9.80665;
					// // Acceleration (m/s^2)
					// xAcceleration = myICM.accX() * 9.80665 / 1000;
					// yAcceleration = myICM.accY() * 9.80665 / 1000;
					// zAcceleration = myICM.accZ() * 9.80665 / 1000;

					// Raw accelerometer readings in m/s^2
					float ax = myICM.accX();
					float ay = myICM.accY();
					float az = myICM.accZ();

					// Calculate pitch and roll in radians
					float pitch = atan2(ay, sqrt(ax * ax + az * az));
					float roll = atan2(-ax, az);

					// Calculate gravity components along each axis
					float gravityX = sin(pitch);
					float gravityY = -sin(roll) * cos(pitch);
					float gravityZ = cos(roll) * cos(pitch);

					// Compensate the raw accelerometer readings by removing the gravity contribution
					float compensatedX = ax - gravityX;
					float compensatedY = ay - gravityY;
					float compensatedZ = az - gravityZ;

					// DBG(compensatedX, compensatedY, compensatedZ);

					getAbsoluteDisplacement(xAcceleration, yAcceleration, zAcceleration, accelReadTime, velocity, displacement);
					GYRO_t_accel = millis();
				}

#ifndef QUAT_ANIMATION
				// DBG("Roll:", roll, " Pitch:", pitch, " Yaw:", yaw);
#else
				// Output the Quaternion data in the format expected by ZaneL's Node.js Quaternion animation tool
				SERIAL_PORT.print(F("{\"quat_w\":"));
				SERIAL_PORT.print(q0, 3);
				SERIAL_PORT.print(F(", \"quat_x\":"));
				SERIAL_PORT.print(q1, 3);
				SERIAL_PORT.print(F(", \"quat_y\":"));
				SERIAL_PORT.print(q2, 3);
				SERIAL_PORT.print(F(", \"quat_z\":"));
				SERIAL_PORT.print(q3, 3);
				SERIAL_PORT.println(F("}"));
#endif
			}
		}

		if (myICM.status != ICM_20948_Stat_FIFOMoreDataAvail) // If more data is available then we should read it right away - and not delay
		{
			delay(10);
		}

		newAccelZ = (float)data.Raw_Accel.Data.Z;
		;
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