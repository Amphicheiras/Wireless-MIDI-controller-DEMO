/*
	*** Buttons, Triggers, LEDs, Viber & Power related operations ***

		  UF0_BUTTON class:


		  UF0_TRIGGER class:


		  UF0_LED class:


		  UF0_POWER class:


		  UF0_VIBER class:
*/

#ifndef UF0_HARDWARE_h
#define UF0_HARDWARE_h

// Preliminary
// LED
#include <Adafruit_NeoPixel.h>
// Joystick
#include <ezButton.h>
// BUTTON functions position
#define BUTTON_ON_OFF_POSITION 0
#define BUTTON_VIRTUAL_GRID_POSITION 1
#define BUTTON_SOLO_POSITION 2
#define BUTTON_YAW_RESET_POSITION 3
#define BUTTON_TOGGLE_MOUSE_POSITION 4
#define BUTTON_SCALE_POSITION 5
#define BUTTON_CHORD_POSITION 6
#define BUTTON_ROOT_POSITION 7
// BUTTON position pinZ
int BUTTON_POSITION_PIN[8] = {5, 17, 16, 4, 0, 2, 15, 13};
// BUTTON functions position pinZ
int BUTTON_ON_OFF_PIN = BUTTON_POSITION_PIN[BUTTON_ON_OFF_POSITION];
int BUTTON_VIRTUAL_GRID_PIN = BUTTON_POSITION_PIN[BUTTON_VIRTUAL_GRID_POSITION];
int BUTTON_SOLO_PIN = BUTTON_POSITION_PIN[BUTTON_SOLO_POSITION];
int BUTTON_YAW_RESET_PIN = BUTTON_POSITION_PIN[BUTTON_YAW_RESET_POSITION];
int BUTTON_TOGGLE_MOUSE_PIN = BUTTON_POSITION_PIN[BUTTON_TOGGLE_MOUSE_POSITION];
int BUTTON_SCALE_PIN = BUTTON_POSITION_PIN[BUTTON_SCALE_POSITION];
int BUTTON_CHORD_PIN = BUTTON_POSITION_PIN[BUTTON_CHORD_POSITION];
int BUTTON_ROOT_PIN = BUTTON_POSITION_PIN[BUTTON_ROOT_POSITION];
// BUTTON states & past states
bool BUTTON_ON_OFF_state, BUTTON_ON_OFF_state_past;
bool BUTTON_VIRTUAL_GRID_state, BUTTON_VIRTUAL_GRID_state_past;
bool BUTTON_SOLO_state, BUTTON_SOLO_state_past;
bool BUTTON_YAW_RESET_state, BUTTON_YAW_RESET_state_past;
bool BUTTON_TOGGLE_MOUSE_state, BUTTON_TOGGLE_MOUSE_state_past;
bool BUTTON_SCALE_state, BUTTON_SCALE_state_past;
bool BUTTON_CHORD_state, BUTTON_CHORD_state_past;
bool BUTTON_ROOT_state, BUTTON_ROOT_state_past;
bool toggle_T3 = false;
// BUTTON modeZ
#define NO_MODE -1
#define ON_OFF_MODE 0
#define VIRTUAL_GRID_MODE 1
#define SOLO_MODE 2
#define YAW_RESET_MODE 3
#define MOUSE_MODE 4
#define SCALE_MODE 5
#define CHORD_MODE 6
#define ROOT_MODE 7

// TRIGGERZ
#define TRIGGER_A 25 // R
#define TRIGGER_B 26 // G
#define TRIGGER_C 18 // B
bool TRIGGER_A_STATE, TRIGGER_B_STATE, TRIGGER_C_STATE,
	TRIGGER_A_STATE_past, TRIGGER_B_STATE_past, TRIGGER_C_STATE_past;

// JOYSTICK
#define JOYSTICK_Y 36	  // ESP32 pin GPIO36 (ADC0) connected to VRX pin
#define JOYSTICK_X 39	  // ESP32 pin GPIO39 (ADC0) connected to VRY pin
#define JOYSTICK_CLICK 16 // ESP32 pin GPIO17 connected to SW pin
ezButton button(JOYSTICK_CLICK);
// Joystick values
int joystickX, joystickY, joystickButton;

// PHOTONIC DISCHARGE DEVICE aka LED
#define LED_FPS 2
#define LED_STRIP_PIN 4
#define LED_NUM_PIXELS 8
Adafruit_NeoPixel LED_STRIP(LED_NUM_PIXELS, LED_STRIP_PIN, NEO_GRB + NEO_KHZ800);
bool toggleLED = false;
int debounceTime = 30;
#define BATTERY_FULL 3
#define BATTERY_HALF 2
#define BATTERY_LOW 1
#define BATTERY_CRITICAL 0
// Modes: -1 = No Mode, 0 = First Mode, 1 = Second Mode, ...
int btnMode = NO_MODE; // modes must go to UF0_FSM
#define ROOT_BUTTON_LED_POSITION BUTTON_ROOT_POSITION
#define SCALE_BUTTON_LED_POSITION BUTTON_SCALE_POSITION
#define CHORD_BUTTON_LED_POSITION BUTTON_CHORD_POSITION
#define ARP_BUTTON_LED_POSITION BUTTON_YAW_RESET_POSITION // ??
#define ON_OFF_BUTTON_LED_POSITION BUTTON_ON_OFF_POSITION
#define TOGGLE_MOUSE_BUTTON_LED_POSITION BUTTON_TOGGLE_MOUSE_POSITION
#define SOLO_BUTTON_LED_POSITION BUTTON_SOLO_POSITION
#define VG_BUTTON_LED_POSITION BUTTON_VIRTUAL_GRID_POSITION

// POWER SUPPLY
#define BATTERY_GAUGE_PIN 34
//

////////////// B U T T O N S //////////////
class UF0_BUTTON
{
private:
	void setup_buttons()
	{
		pinMode(BUTTON_ON_OFF_PIN, INPUT_PULLUP);
		pinMode(BUTTON_VIRTUAL_GRID_PIN, INPUT_PULLUP);
		pinMode(BUTTON_SOLO_PIN, INPUT_PULLUP);
		pinMode(BUTTON_YAW_RESET_PIN, INPUT_PULLUP);
		pinMode(BUTTON_TOGGLE_MOUSE_PIN, INPUT_PULLUP);
		pinMode(BUTTON_SCALE_PIN, INPUT_PULLUP);
		pinMode(BUTTON_CHORD_PIN, INPUT_PULLUP);
		pinMode(BUTTON_ROOT_PIN, INPUT_PULLUP);
	}

public:
	UF0_BUTTON() {}

	UF0_BUTTON(bool go)
	{
		setup_buttons();
	}
	void loop()
	{
		// Update btn states
		BUTTON_ON_OFF_state = !digitalRead(BUTTON_ON_OFF_PIN);
		BUTTON_VIRTUAL_GRID_state = !digitalRead(BUTTON_VIRTUAL_GRID_PIN);
		BUTTON_SOLO_state = !digitalRead(BUTTON_SOLO_PIN);
		BUTTON_YAW_RESET_state = !digitalRead(BUTTON_YAW_RESET_PIN);
		BUTTON_TOGGLE_MOUSE_state = !digitalRead(BUTTON_TOGGLE_MOUSE_PIN);
		BUTTON_SCALE_state = !digitalRead(BUTTON_SCALE_PIN);
		BUTTON_CHORD_state = !digitalRead(BUTTON_CHORD_PIN);
		BUTTON_ROOT_state = !digitalRead(BUTTON_ROOT_PIN);

		// Button do:
		// Hold BUTTON_ON_OFF:
		if (BUTTON_ON_OFF_state == HIGH && BUTTON_ON_OFF_state_past == LOW)
		{
			// startSleepTimer();
		}
		// Let BUTTON_ON_OFF
		if (BUTTON_ON_OFF_state == LOW && BUTTON_ON_OFF_state_past == HIGH)
		{
			// resetSleepTimer();
		}
		// Hold BUTTON_SOLO:
		if (BUTTON_SOLO_state == HIGH)
		{
			btnMode = SOLO_MODE;
		}
		// Bang BUTTON_YAW_RESET:
		if (BUTTON_YAW_RESET_state == HIGH && BUTTON_YAW_RESET_state_past == LOW)
		{
			// resetHeading();
		}

		// Update past btn states
		BUTTON_ON_OFF_state_past = BUTTON_ON_OFF_state;
		BUTTON_VIRTUAL_GRID_state_past = BUTTON_VIRTUAL_GRID_state;
		BUTTON_SOLO_state_past = BUTTON_SOLO_state;
		BUTTON_YAW_RESET_state_past = BUTTON_YAW_RESET_state;
		BUTTON_TOGGLE_MOUSE_state_past = BUTTON_TOGGLE_MOUSE_state;
		BUTTON_SCALE_state_past = BUTTON_SCALE_state;
		BUTTON_CHORD_state_past = BUTTON_CHORD_state;
		BUTTON_ROOT_state_past = BUTTON_ROOT_state;
	}
};

///////////// T R I G G E R S /////////////
class UF0_TRIGGER
{
private:
	UF0_MIDI *midii;

	void setup_triggers()
	{
		pinMode(TRIGGER_A, INPUT_PULLUP);
		pinMode(TRIGGER_B, INPUT_PULLUP);
		pinMode(TRIGGER_C, INPUT_PULLUP);
	}

public:
	UF0_TRIGGER() {}

	UF0_TRIGGER(bool cmon)
	{
		setup_triggers();
	}

	void loop()
	{
		// Update trigger states
		TRIGGER_A_STATE = !digitalRead(TRIGGER_A);
		TRIGGER_B_STATE = !digitalRead(TRIGGER_B);
		TRIGGER_C_STATE = !digitalRead(TRIGGER_C);
		// Check if any MOD buttons are pressed
		switch (btnMode)
		{
			// Trigger do:
			//////////////// N O    M O D E //////////////
		case NO_MODE:
			// Hold T1:
			if (TRIGGER_A_STATE == HIGH && TRIGGER_A_STATE_past == LOW)
			{
				// Turn off previously played note, if any
				// midii->sendMIDINoteOn(MIDINoteOn, 100, 5);
				midii->sendMIDIChordOn(MIDINoteOn, 70, 5);
			}
			// Let T1:
			if (TRIGGER_A_STATE == LOW && TRIGGER_A_STATE_past == HIGH)
			{
				// Turn off previously played note, if any
				// midii->sendMIDINoteOff(MIDINoteOn, 100, 5);
				midii->sendMIDIChordOff(MIDINoteOn, 70, 5);
			}
			// Hold T2:
			if (TRIGGER_B_STATE == HIGH && TRIGGER_B_STATE_past == LOW)
			{
				midii->enableControl();
			}
			// Let T2:
			if (TRIGGER_B_STATE == LOW && TRIGGER_B_STATE_past == HIGH)
			{
				midii->disableControl();
			}
			// Toggle TF:
			if (TRIGGER_C_STATE == HIGH && TRIGGER_C_STATE_past == LOW)
			{
				toggle_T3 = !toggle_T3;
				if (toggle_T3)
				{
					midii->toggle_T3_on();
				}
				else
				{
					midii->toggle_T3_off();
				}
			}
			// Toggle TF:
			if (TRIGGER_C_STATE == LOW && TRIGGER_C_STATE_past == HIGH)
			{
				// close function
				// midii->sendMIDINoteOff(MIDINoteOn, 100, 5);
			}
			break;
		//////////////// S O L O   M O D E ///////////////
		case SOLO_MODE:
			if (TRIGGER_A_STATE == HIGH && TRIGGER_A_STATE_past == LOW)
			{
			}
			if (TRIGGER_B_STATE == HIGH && TRIGGER_B_STATE_past == LOW)
			{
			}
			if (TRIGGER_C_STATE == HIGH && TRIGGER_C_STATE_past == LOW)
			{
			}
			break;
		/////////////// C H O R D   M O D E /////////////////
		case CHORD_MODE:
			// Hold T1:
			if (TRIGGER_A_STATE == HIGH && TRIGGER_A_STATE_past == LOW)
			{
				// Turn off previously played notes, if any

				// Update target MIDI Note On
				// MIDIChordOn[i] = rootNote + rootChord[i];
				// Play target note
			}
			// Let T1:
			if (TRIGGER_A_STATE == LOW && TRIGGER_A_STATE_past == HIGH)
			{
				// Turn off previously played notes, if any
			}
		default:
			DBG("WTF");
		}

		// Update old trigger states
		TRIGGER_A_STATE_past = TRIGGER_A_STATE;
		TRIGGER_B_STATE_past = TRIGGER_B_STATE;
		TRIGGER_C_STATE_past = TRIGGER_C_STATE;
	}
};

////////////// J O Y S T I C K ////////////
class UF0_JOYSTICK
{
private:
	// Timers
	unsigned long JOYSTICK_t0 = millis();

	void setup_joystick()
	{
		button.setDebounceTime(debounceTime);
	}

public:
	UF0_JOYSTICK() {}

	UF0_JOYSTICK(bool cmon)
	{
		setup_joystick();
	}

	void loop()
	{
		button.loop(); // MUST call the loop() function first
		// read X and Y analog values
		joystickX = analogRead(JOYSTICK_X);
		joystickY = analogRead(JOYSTICK_Y);
		// Read the button value
		joystickButton = button.getState();

		if (button.isPressed())
		{
			Serial.println("The button is pressed");
			// TODO do something here
		}

		if (button.isReleased())
		{
			Serial.println("The button is released");
			// TODO do something here
		}

		if ((millis() - JOYSTICK_t0) > monitorHz)
		{
			// DBG("x = ", joystickX,", y = ", joystickY, "button = ", joystickButton);
			JOYSTICK_t0 = millis();
		}
	}
};

////////////////// L E D //////////////////
class UF0_LED_element
{
private:
	int position;
	int color[3],
		brightness;

public:
	UF0_LED_element() {}

	UF0_LED_element(int pos, int R, int G, int B, int brightness)
	{
		position = pos;
		setColor(R, G, B);
		setBrightness(brightness);
	}

	int getPosition()
	{
		return position;
	}

	void setColor(int R, int G, int B)
	{
		color[0] = R;
		color[1] = G;
		color[2] = B;
	}

	int getColorRed()
	{
		return color[0];
	}

	int getColorGreen()
	{
		return color[1];
	}

	int getColorBlue()
	{
		return color[2];
	}

	void setBrightness(int bright)
	{
		brightness = bright;
	}

	int getBrightness()
	{
		return brightness;
	}

	void shine()
	{
		LED_STRIP.setPixelColor(position, LED_STRIP.Color(255, 255, 255));
		setBrightness(255);
	}

	void close()
	{
		LED_STRIP.setPixelColor(position, LED_STRIP.Color(0, 0, 0));
	}
};

class UF0_LED_handler
{
private:
	UF0_LED_element root_LED;
	UF0_LED_element scale_LED;
	UF0_LED_element chord_LED;
	UF0_LED_element arp_LED;
	UF0_LED_element on_off_LED;
	UF0_LED_element toggle_mouse_LED;
	UF0_LED_element solo_LED;
	UF0_LED_element VG_LED;

	void setup()
	{
		root_LED = UF0_LED_element(ROOT_BUTTON_LED_POSITION, 1.000 * 255, 0.431 * 255, 0.000 * 255, 15);
		scale_LED = UF0_LED_element(SCALE_BUTTON_LED_POSITION, 1.000 * 255, 0.863 * 255, 0.000 * 255, 15);
		chord_LED = UF0_LED_element(CHORD_BUTTON_LED_POSITION, 0.000 * 255, 1.000 * 255, 0.012 * 255, 15);
		arp_LED = UF0_LED_element(ARP_BUTTON_LED_POSITION, 0.000 * 255, 0.694 * 255, 1.000 * 255, 15);
		on_off_LED = UF0_LED_element(ON_OFF_BUTTON_LED_POSITION, 0.000 * 255, 0.012 * 255, 1.000 * 255, 15);
		toggle_mouse_LED = UF0_LED_element(TOGGLE_MOUSE_BUTTON_LED_POSITION, 0.310 * 255, 0.000 * 255, 1.000 * 255, 15);
		solo_LED = UF0_LED_element(SOLO_BUTTON_LED_POSITION, 0.635 * 255, 0.000 * 255, 1.000 * 255, 15);
		VG_LED = UF0_LED_element(VG_BUTTON_LED_POSITION, 0.992 * 255, 0.000 * 255, 1.000 * 255, 15);
	}
	// Timers
	unsigned long LED_FPS_timer = millis(), LED_t0 = millis();

public:
	UF0_LED_handler() : root_LED(ROOT_BUTTON_LED_POSITION, 1.000 * 255, 0.431 * 255, 0.000 * 255, 15),
						scale_LED(SCALE_BUTTON_LED_POSITION, 1.000 * 255, 0.863 * 255, 0.000 * 255, 15),
						chord_LED(CHORD_BUTTON_LED_POSITION, 0.000 * 255, 1.000 * 255, 0.012 * 255, 15),
						arp_LED(ARP_BUTTON_LED_POSITION, 0.000 * 255, 0.694 * 255, 1.000 * 255, 15),
						on_off_LED(ON_OFF_BUTTON_LED_POSITION, 0.000 * 255, 0.012 * 255, 1.000 * 255, 15),
						toggle_mouse_LED(TOGGLE_MOUSE_BUTTON_LED_POSITION, 0.310 * 255, 0.000 * 255, 1.000 * 255, 15),
						solo_LED(SOLO_BUTTON_LED_POSITION, 0.635 * 255, 0.000 * 255, 1.000 * 255, 15),
						VG_LED(VG_BUTTON_LED_POSITION, 0.992 * 255, 0.000 * 255, 1.000 * 255, 15) {}

	// BATTERY_LED(VG_BUTTON_LED_POSITION, 0.992*255, 0.000*255, 1.000*255, 15){}

	UF0_LED_handler(bool init)
	{
		setup();
	}

	void loop()
	{
		// Update Frame every FPS ms
		if ((millis() - LED_FPS_timer) > LED_FPS)
		{
			// Clear canvas
			LED_STRIP.clear();
			// Set individual LED colors
			LED_STRIP.setPixelColor(ROOT_BUTTON_LED_POSITION,
									LED_STRIP.Color(root_LED.getColorRed(),
													root_LED.getColorGreen(),
													root_LED.getColorBlue()));
			LED_STRIP.setPixelColor(SCALE_BUTTON_LED_POSITION,
									LED_STRIP.Color(scale_LED.getColorRed(),
													scale_LED.getColorGreen(),
													scale_LED.getColorBlue()));
			LED_STRIP.setPixelColor(CHORD_BUTTON_LED_POSITION,
									LED_STRIP.Color(chord_LED.getColorRed(),
													chord_LED.getColorGreen(),
													chord_LED.getColorBlue()));
			LED_STRIP.setPixelColor(ARP_BUTTON_LED_POSITION,
									LED_STRIP.Color(arp_LED.getColorRed(),
													arp_LED.getColorGreen(),
													arp_LED.getColorBlue()));
			LED_STRIP.setPixelColor(ON_OFF_BUTTON_LED_POSITION,
									LED_STRIP.Color(on_off_LED.getColorRed(),
													on_off_LED.getColorGreen(),
													on_off_LED.getColorBlue()));
			LED_STRIP.setPixelColor(TOGGLE_MOUSE_BUTTON_LED_POSITION,
									LED_STRIP.Color(toggle_mouse_LED.getColorRed(),
													toggle_mouse_LED.getColorGreen(),
													toggle_mouse_LED.getColorBlue()));
			LED_STRIP.setPixelColor(SOLO_BUTTON_LED_POSITION,
									LED_STRIP.Color(solo_LED.getColorRed(),
													solo_LED.getColorGreen(),
													solo_LED.getColorBlue()));
			LED_STRIP.setPixelColor(VG_BUTTON_LED_POSITION,
									LED_STRIP.Color(255,
													0,
													0));
			// Set (global?) brightness
			LED_STRIP.setBrightness(10);
			// Show LEDs
			LED_STRIP.show();
			// batteryMonitor(power.get_battery_level());
			LED_FPS_timer = millis();
		}
		// Close LEDs until time to open
		else
		{
			// LED_STRIP.clear();
			// LED_STRIP.show();
		}
	}

	void showTargetLED(int targetLED,
					   int targetLEDColorR,
					   int targetLEDColorG,
					   int targetLEDColorB,
					   int targetLEDBrightness)
	{
		LED_STRIP.setPixelColor(targetLED,
								LED_STRIP.Color(targetLEDColorR,
												targetLEDColorG,
												targetLEDColorB));
		LED_STRIP.setBrightness(targetLEDBrightness);
		LED_STRIP.show();
	}

	void shineAllLEDs()
	{
		for (int i = 0; i < 10; i++)
		{
			LED_STRIP.setPixelColor(i, LED_STRIP.Color(153, 0, 255)); // it only takes effect if pixels.show() is called
			LED_STRIP.setBrightness(10);
		}
		LED_STRIP.show();
	}

	void closeTargetLED(int targetLED)
	{
		LED_STRIP.setPixelColor(targetLED, 0, 0, 0, 0);
		LED_STRIP.show();
	}

	void closeAllLEDs()
	{
		LED_STRIP.clear();
		LED_STRIP.show();
	}

	void batteryMonitor(int batteryLevel)
	{
		switch (batteryLevel)
		{
		case BATTERY_FULL:
			showTargetLED(8, 0, 255, 0, 10);
			break;
		case BATTERY_HALF:
			showTargetLED(8, 0, 122, 122, 10);
			break;
		case BATTERY_LOW:
			showTargetLED(8, 85, 85, 85, 10);
			break;
		case BATTERY_CRITICAL:
			showTargetLED(8, 255, 0, 0, 10);
			break;
		default:
			closeTargetLED(8);
		}
	}

	void celebrate()
	{
		// Yoo!!
		shineAllLEDs();
		delay(100);
		closeAllLEDs();
		delay(100);
		shineAllLEDs();
		delay(100);
		closeAllLEDs();
		delay(100);
	}
};

////////////////// P O W E R //////////////////
class UF0_POWER
{
	// 0-100%: 4h45m 3200mAh
private:
	float raw_voltage;
	int supply_voltage, voltage_percent, voltageNormalized, voltageSampleBuffer[10];
	// Timers
	unsigned long POWER_t0 = millis();

	void setup_power()
	{
		measure_battery_level();
	}

	void measure_battery_level()
	{
		raw_voltage = (analogRead(BATTERY_GAUGE_PIN) * 3.3) / 4095;
		supply_voltage = 2 * 1000 * ((analogRead(BATTERY_GAUGE_PIN) * 3.3) / 4095) + 0.2;
		voltage_percent = map(supply_voltage, 2900, 4200, 0, 100);
	}

public:
	UF0_POWER() {}

	UF0_POWER(bool cmon)
	{
		setup_power();
	}

	int get_battery_percent()
	{
		measure_battery_level();
		return voltage_percent;
	}

	int get_battery_level()
	{
		// Average battery level values (sum(10*samples)/10)
		// for (t0 - millis() > 10){
		//   voltageSampleBuffer[i] = voltage_percent;
		//   i++;
		//   if (i > 9){
		//     voltageNormalized = voltageSampleBuffer.sum()/10;
		//     i=0;
		//   }
		//   t0 = millis();
		// }

		switch (get_battery_percent())
		{
		case 61 ... 100:
			return BATTERY_FULL;
		case 31 ... 60:
			return BATTERY_HALF;
		case 11 ... 30:
			return BATTERY_LOW;
		case 0 ... 10:
			return BATTERY_CRITICAL;
		default:
			return -1;
		}
	}
};

#endif