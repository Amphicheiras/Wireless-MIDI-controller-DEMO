#ifndef UF0_GYRO_ICM_20948_h
#define UF0_GYRO_ICM_20948_h

/*

	Get orientation with quaternia?

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

ICM_20948_I2C myICM;
// ICM_20948_SPI myICM;

// Constants
const double GRAVITY = 9.81;	   // Acceleration due to gravity (m/s^2)
const double GRAVITY_LSB = 1000.0; // Conversion factor for raw data to g
// Acceleration calibration offsets
double accelX_offset = 0.0;
double accelY_offset = 0.0;
double accelZ_offset = 0.0;
// Gravity calibration offsets
double gravityX_offset = 0.0;
double gravityY_offset = 0.0;
double gravityZ_offset = 0.0;
double axFilter = 0.0;
double ayFilter = 0.0;
double azFilter = 0.0;
double prevOutputX = 0.0;
double prevOutputY = 0.0;
double prevOutputZ = 0.0;
// Calibration parameters
const int CALIBRATION_SAMPLES = 100;
// YPR raw values
float pitch, roll, yaw;
// Quaternia values
double q0, q1, q2, q3;
// XYZ raw acceleration values
double axRaw, ayRaw, azRaw;
// XYZ calibrated acceleration values
double axCal, ayCal, azCal;
// XYZ g compensated acceleration values
double axComp, ayComp, azComp;
// XYZ position raw values
double xPosition, yPosition, zPosition;
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
double dT;
double xVelocity = 0.0;
double yVelocity = 0.0;
double zVelocity = 0.0;
double displacement[3] = {0.0, 0.0, 0.0};
float orientation;
float positionCurrent[3];
float positionPrevious[3];
int accelReadTime = 20;

icm_20948_DMP_data_t data;

/*/////////// 3D Position /////////

	fix 15 sec of 0.07 on x axis?

*/
/////////////////////////////////////

void getAccelSensorReadings()
{
	myICM.readDMPdataFromFIFO(&data);

	if ((myICM.status == ICM_20948_Stat_Ok) || (myICM.status == ICM_20948_Stat_FIFOMoreDataAvail)) // Was valid data available?
	{
		if ((data.header & DMP_header_bitmap_Quat6) > 0) // We have asked for GRV data so we should receive Quat6
		{
			// Update AGMT values
			if (myICM.dataReady())
			{
				// Q0 value is computed from this equation: Q0^2 + Q1^2 + Q2^2 + Q3^2 = 1.
				// In case of drift, the sum will not add to 1, therefore, quaternion data need to be corrected with right bias values.
				// The quaternion data is scaled by 2^30.

				// Scale to +/- 1
				q1 = ((double)data.Quat6.Data.Q1) / 1073741824.0; // Convert to double. Divide by 2^30
				q2 = ((double)data.Quat6.Data.Q2) / 1073741824.0; // Convert to double. Divide by 2^30
				q3 = ((double)data.Quat6.Data.Q3) / 1073741824.0; // Convert to double. Divide by 2^30
				q0 = sqrt(1.0 - ((q1 * q1) + (q2 * q2) + (q3 * q3)));

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
				myICM.getAGMT();
				// Raw accelerometer readings in m/s^2
				axRaw = myICM.accX() / GRAVITY_LSB;
				ayRaw = myICM.accY() / GRAVITY_LSB;
				azRaw = myICM.accZ() / GRAVITY_LSB;

				// DBG("       Raw", axRaw, ayRaw, azRaw);
			}
		}
	}
}

void calibrateAccelerometer()
{
	Serial.println("Calibrating accelerometer...");

	double accelX_sum = 0.0;
	double accelY_sum = 0.0;
	double accelZ_sum = 0.0;

	// Must be while because ICM data is not always ready
	int i = 0;
	while (i < CALIBRATION_SAMPLES)
	{
		if (myICM.dataReady())
		{
			// Fetch accelerometer data
			myICM.getAGMT();
			// Sum accelerometer readings (in g)
			accelX_sum += myICM.accX() / GRAVITY_LSB;
			accelY_sum += myICM.accY() / GRAVITY_LSB;
			accelZ_sum += myICM.accZ() / GRAVITY_LSB;
			i++;
		}
		delay(10); // Short delay between samples
	}

	// Assume that when flat, X and Y should be 0g and Z should be approximately 1g
	accelX_offset = accelX_sum / CALIBRATION_SAMPLES;
	accelY_offset = accelY_sum / CALIBRATION_SAMPLES;
	accelZ_offset = (accelZ_sum / CALIBRATION_SAMPLES) - 1.0;

	Serial.println("Calibration completed. Offsets: X: ");
	Serial.print(accelX_offset);
	Serial.print(" g, Y: ");
	Serial.print(accelY_offset);
	Serial.print(" g, Z: ");
	Serial.print(accelZ_offset);
	Serial.println(" g");
}

void calibrateGravityVector()
{
	Serial.println("Calibrating accelerometer...");

	double gravityX_sum = 0.0;
	double gravityY_sum = 0.0;
	double gravityZ_sum = 0.0;

	// Calculate bias by comparing readings to gravity vector
	for (int i = 0; i < CALIBRATION_SAMPLES; i++)
	{
		// Calculate gravity vector components based on quaternion
		getAccelSensorReadings();
		gravityX_sum += 2 * (q1 * q3 - q0 * q2);
		gravityY_sum += 2 * (q0 * q1 + q2 * q3);
		gravityZ_sum += (q0 * q0 - q1 * q1 - q2 * q2 + q3 * q3) - 1;
		DBG(gravityX_sum, gravityY_sum, gravityZ_sum);
		delay(10);
	}

	// Average the biases
	gravityX_offset = gravityX_sum / CALIBRATION_SAMPLES;
	gravityY_offset = gravityY_sum / CALIBRATION_SAMPLES;
	gravityZ_offset = gravityZ_sum / CALIBRATION_SAMPLES;

	DBG("Calibration completed. Offsets: X: ", gravityX_offset, ", Y: ", gravityY_offset, ", Z: ", gravityZ_offset);
}

// ax, ay, az: accelerometer readings in m/s², dT: time difference between readings (ms), velocity: variable, displacement: variable
void getDisplacement()
{
	// Time step (in seconds)
	unsigned long currentTime = millis();
	dT = (currentTime - lastTime) / 1000.0;
	lastTime = currentTime;
	// Update values if quaternia are ready
	if (!std::isnan(q0))
	{
		// Update velocity by integrating acceleration (m/s)
		xVelocity += axFilter * dT;
		yVelocity += ayFilter * dT;
		zVelocity += azFilter * dT;
		// Update displacement by integrating velocity (m)
		displacement[0] += xVelocity * dT;
		displacement[1] += yVelocity * dT;
		displacement[2] += zVelocity * dT;
	}

	DBG("ax:", axComp, "ay:", ayComp, "az:", azComp, "velocity X:", xVelocity, ", velocity Y:", yVelocity, ", velocity Z:", zVelocity, ", displacement X:", displacement[0], ", displacement Y:", displacement[1], ", displacement Z:", displacement[2]);
}

void updateRelevantPosition(float *positionCurrent, float *positionPrevious, float *displacement)
{
	getDisplacement();
	positionCurrent[0] = positionPrevious[0] + displacement[0];
	positionCurrent[1] = positionPrevious[1] + displacement[1];
	positionCurrent[2] = positionPrevious[2] + displacement[2];
}

float get3DPosition(float *positionCurrent, float *positionPrevious, float *displacement)
{
	updateRelevantPosition(positionCurrent, positionPrevious, displacement);
	return *positionCurrent;
}

void getFilteredAccel()
{
	double alpha = 0.1;

	// Update values if quaternia are ready
	if (!std::isnan(axComp) && !std::isnan(ayComp) && !std::isnan(azComp))
	{
		axFilter = alpha * axComp + (1.0 - alpha) * prevOutputX;
		ayFilter = alpha * ayComp + (1.0 - alpha) * prevOutputY;
		azFilter = alpha * azComp + (1.0 - alpha) * prevOutputZ;
		prevOutputX = axFilter;
		prevOutputY = ayFilter;
		prevOutputZ = azFilter;
	}

	// DBG("Filtered", axFilter, ayFilter, azFilter, "Comp", axComp, ayComp, azComp);
}

// Function to compensate for gravity in all axes
void getCompensatedGravityAccel()
{
	// Gravity vector components (normalized)
	double gravityX = 2 * (q1 * q3 - q0 * q2);
	double gravityY = 2 * (q0 * q1 + q2 * q3);
	double gravityZ = (q0 * q0 - q1 * q1 - q2 * q2 + q3 * q3);

	// Subtract the gravity components from accelerometer readings
	// axComp = axCal - gravityX;
	// ayComp = ayCal - gravityY;
	// azComp = azCal - gravityZ;
	axComp = axRaw - gravityX;
	ayComp = ayRaw - gravityY;
	azComp = azRaw - gravityZ;

	// DBG("Compensated", axComp, ayComp, azComp);
}

void getCalibratedSensorReadings()
{
	axCal = axRaw - accelX_offset;
	ayCal = ayRaw - accelY_offset;
	azCal = azRaw - accelZ_offset;

	// DBG(" Calibrated", axCal, ayCal, azCal);
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

		// Accelerometer settings
		ICM_20948_fss_t fss;
		fss.a = gpm2;	// Set accelerometer range to ±2g
		fss.g = dps250; // Set gyroscope range to ±250 degrees per second

		// Set the full scale range for both accelerometer and gyroscope
		myICM.setFullScale(ICM_20948_Internal_Acc | ICM_20948_Internal_Gyr, fss);

		// Configure DLPF (Digital Low Pass Filter) for noise reduction
		ICM_20948_dlpcfg_t dlpfCfg;
		dlpfCfg.a = acc_d473bw_n499bw;	 // Set accelerometer bandwidth
		dlpfCfg.g = gyr_d361bw4_n376bw5; // Set gyroscope bandwidth

		myICM.setDLPFcfg(ICM_20948_Internal_Acc | ICM_20948_Internal_Gyr, dlpfCfg);

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
		calibrateAccelerometer();
		// calibrateGravityVector();
	}

public:
	UF0_GYRO() {}

	UF0_GYRO(bool cmon)
	{
		setup();
	}

	void loop()
	{

		getAccelSensorReadings();
		getCalibratedSensorReadings();
		getCompensatedGravityAccel();
		getFilteredAccel();
		getDisplacement();
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