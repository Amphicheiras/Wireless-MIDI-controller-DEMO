#ifndef UF0_BLACKMAGIC_h
#define UF0_BLACKMAGIC_h

// Preliminary
// Disk access
#include <Preferences.h>
Preferences preferences;
// Virtual Grid current target
int virtualTarget;
//

// pop_XY = population XY, Mmin_XY = XY min movement, Mmax_XY = XY max movement
int virtualGrid(int pop_X, int pop_Y, float Mmin_X, float Mmax_X, float Mmin_Y, float Mmax_Y)
{
	// DoF = Degrees of Freedom (Movement)
	float DoF_X = Mmax_X - Mmin_X;
	float DoF_Y = Mmax_Y - Mmin_Y;
	// 1 tick is 1 cell step
	float tick_X = DoF_X / pop_X;
	float tick_Y = DoF_Y / pop_Y;
	for (int i = 0; i < pop_X; i++)
	{
		// DBG("  YAW:", yaw , ">=", (i * tick_X), "&&", yaw, "<", ((i+1) * tick_X));
		if ((yaw >= (i * tick_X + Mmin_X)) && (yaw < ((i + 1) * tick_X + Mmin_X)))
		{
			for (int j = 0; j < pop_Y; j++)
			{
				// DBG("PITCH:", pitch , ">=", (j * tick_Y), "&&", pitch, "<", ((j+1) * tick_Y));
				if ((pitch >= (j * tick_Y + Mmin_Y)) && (pitch < ((j + 1) * tick_Y + Mmin_Y)))
				{
					return (j * pop_X + i); // Or smth
				}
			}
		}
	}
	return -1;
}

class UF0_BLACKMAGIC
{
private:
	void setup()
	{
		loadPreset();
	}

public:
	UF0_BLACKMAGIC() {}

	UF0_BLACKMAGIC(bool cmon)
	{
		setup();
	}

	void loop()
	{
		virtualTarget = virtualGrid(4, 3, -90, +90, -90, +90);
	}

	// void save(auto saveWhat, char* saveAs, char* saveAt){
	//   preferences.begin(saveAt, false);
	//   preferences.put(saveAs, saveWhat);
	//   preferences.end();
	// }

	// auto load(auto loadWhat, char* loadFrom){
	//   auto toReturn;
	//   preferences.begin(loadFrom, true);
	//   toReturn = get(loadWhat, 0);
	//   preferences.end();
	//   return toReturn;
	// }

	void pushCopyBuffer(int *x) {

	};

	void popCopyBuffer(int *x) {

	};

	void copy(int *copyObject)
	{
		pushCopyBuffer(copyObject);
	}

	void paste(int *pasteDestination)
	{
		popCopyBuffer(pasteDestination);
	}

	void remove()
	{
		// preferences.remove()?
	}

	// Write to disk
	void savePreset()
	{
		// Begin: True -> Read only | False -> Read/Write
		preferences.begin("gyroStates", false);
		preferences.putBool("activeX", pitch);
		preferences.putBool("activeY", roll);
		preferences.putBool("activeZ", yaw);
		// preferences.("wifi un & password", activeZ);
		preferences.end();
	}

	// Read from disk
	void loadPreset()
	{
		preferences.begin("gyroStates", false);
		pitchTransmit = preferences.getBool("activeX", "");
		rollTransmit = preferences.getBool("activeY", "");
		yawTransmit = preferences.getBool("activeZ", "");
		preferences.end();
	}
};

#endif