#ifndef UF0_PRESET_SYSTEM_h
#define UF0_PRESET_SYSTEM_h

/*
	classes:
		Intrument
		Control
		Scale_Control
		Chord_Control
		Arp_Control

	Preset_Handler:
		8 x Bank	(Gig Level)
			8 x Set 	(Song Level)
				8 x Preset  (Instrument Level)

	invent new notes (give name, freq/pitch)
	new names for: 'preset' -> 'instrument', 'set' -> 'song', 'bank' -> 'gig'
*/

#include "UF0_MUSIC_THEORY.h"

enum PlayHow{
  NOTE_PLAY_ON_TRIGGER,
  NOTE_PLAY_ON_DRUMHIT
};

enum PlayMode{
  NOTE_PLAY_MODE_HOLD,
  NOTE_PLAY_MODE_TOGGLE,
  NOTE_PLAY_MODE_BANG
};

enum Motion{
	MOTION_BOB,
	MOTION_ROLL,
	MOTION_YAW,
	MOTION_JOYSTICK_HORIZONTAL,
	MOTION_JOYSTICK_VERTICAL,
	MOTION_JOYSTICK_UP,
	MOTION_JOYSTICK_DOWN,
	MOTION_JOYSTICK_LEFT,
	MOTION_JOYSTICK_RIGHT
};

enum Controller{
	CONTROLLER_LEFT,
	CONTROLLER_RIGHT,
	CONTROLLER_BOTH
};

class Instrument{
private:
	char* name;
	PlayHow playHow{NOTE_PLAY_ON_TRIGGER};
	PlayMode playMode{NOTE_PLAY_MODE_HOLD};
	int bangDuration{10}; // 10 ms
	int MIDIChannel{1};
	bool noteActive{0};
	bool scaleActive{0};
	bool chordActive{0};
	bool octaveActive{0};
	bool arpActive{0};
	Note* note;
	Scale* scale;
	Chord* chord;
	Arpeggiator* arp;
	Riff* riff;

public:
	Instrument(){};
	void rename(char* newName){
		this->name = newName;
	}
	void init(){
		// this->scale->init();
		// this->chord->init();
		// this->arpeggiator->init();
		// this->riff->init();
	}
};

class ControlElement{
protected:
	char* name{nullptr};
  // int handID;
	Controller controller;
	Motion motion;
	int motionLimitsMin{-90};
	int motionLimitsMax{+90};
	int motionLimitsFlip{0};
	int gridLength{2};
	int gridHeight{2};
	int VG_X{0};
	int VG_Y{0};

public:
    ControlElement(){}
    ControlElement(char* n) : name(n) {}
    virtual ~ControlElement(){}
    virtual char* getName() const {return name;}
};

class Control : public ControlElement{
private:
	int Channel{1};
	int CC{176};

public:
	Control(char* name) : ControlElement(name){};
	void abolish(){
		// ~Control();
	}
	~Control(){};
};

class Chord_Control : public ControlElement{
private:
	Chord* chord;

public:
	Chord_Control(char* name) : ControlElement(name){
		// this.chord = new Chord;
	}
};

class Scale_Control : public ControlElement{
private:
	Scale* scale;

public:
	Scale_Control(char* name) : ControlElement(name){}
};

class Arpeggiator_Control : public ControlElement{
private:
	Arpeggiator* arpeggiator;

public:
	Arpeggiator_Control(char* name) : ControlElement(name){}
};

class Riff_Control : public ControlElement{
private:
	
public:
	Riff_Control(char* name) : ControlElement(name){}
};

class Preset{
private:
	Instrument* instrument;
	int numOfControls{0};
	Control** controlList;

public:
	int id;

	Preset(int id){
		this->id = id;
	};
	void initPreset(){
		instrument->init();
		for(int i=0; i<numOfControls; i++){
			controlList[i]->abolish();	
		}
	}
};

class Set{
private:

public:
	int id{0};
	Preset* presetList[8];

	Set(int id){
		this->id = id;
	};
	void initSet(){
		for(int i=0; i<8; i++){
			presetList[i]->initPreset();	
		}
	}
};

class Bank{
private:

public:
	int id{0};
	Set* setList[8];

	Bank(int id){
		this->id = id;
	};
	void initBank(){
		for(int i=0; i<8; i++){
			setList[i]->initSet();
		}
	}
};

class UF0_PRESET_HANDLER{
private:
	int* copyBuffer;	// has max 1 item?

	// pushCopyBuffer(Object* copyObject){
	// 	copyBuffer = copyObject;
	// }
	// //	does it ever pop or just refresh?
	// popCopyBuffer(Destination* pasteDestination){
	// 	pasteDestination = copyBuffer;
	// }

public:
	Bank* bankList[8];

	UF0_PRESET_HANDLER(){}

	UF0_PRESET_HANDLER(bool cmon){
		for (int i=0; i<8; ++i){
			bankList[i] = new Bank(i);
			DBG(bankList[i]->id);
			for (int j=0; j<8; ++j){
				bankList[i]->setList[j] = new Set(i*8+j);
				DBG(bankList[i]->setList[j]->id);
				for (int k=0; k<8; ++k){
					bankList[i]->setList[j]->presetList[k] = new Preset(i*8+j*8+k);
					DBG(bankList[i]->setList[j]->presetList[k]->id);
				}
			}
		}
	}

	void loop(){

	}

	void loadCollection(){
		for (int i=0; i<8; ++i){
			// bankList[i] = load(bankList[i]->id, "bankList");
			for (int j=0; j<8; ++j){
				// bankList[i]->setList[j] = load(bankList[i]->setList[j]->id, "setList");
				for (int k=0; k<8; ++k){
					// bankList[i]->setList[j]->presetList[k] = load(bankList[i]->setList[j]->presetList[k]->id, "presetList");
				}
			}
		}
	}
};

#endif