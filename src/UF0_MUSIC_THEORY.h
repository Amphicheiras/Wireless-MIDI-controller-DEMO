#ifndef UF0_MUSIC_THEORY_h
#define UF0_MUSIC_THEORY_h

class Note{
  // Is Note a musical element with numOfNotes = 1?
  private:
    char* name;
    int frequency;
    enum accidental{
      NOTE_ACCIDENTAL_NATURAL,
      NOTE_ACCIDENTAL_FLAT,
      NOTE_ACCIDENTAL_SHARP
    };

  public:
    Note(){}
    Note(char* name) : name(name){}
    Note(char* name, int frequency) : name(name), frequency(frequency){}
};

class MusicalElement{
  protected:
    char* name;
    int numOfNotes{0};
    Note* notesInElement; // rootNote intervals or absolute Notes?

  public:
    MusicalElement() : name(nullptr), numOfNotes(0), notesInElement(nullptr){}
    MusicalElement(char* n) : name(n), numOfNotes(0), notesInElement(nullptr){}
    MusicalElement(char* n, int numOfNotes, Note* notesInElement) : name(n), numOfNotes(numOfNotes){
      this->notesInElement = new Note[numOfNotes];
      for (int i = 0; i < numOfNotes; ++i) {
          this->notesInElement[i] = notesInElement[i];
      }
    }
    virtual ~MusicalElement(){
      delete[] name;
      delete[] notesInElement;
    }
    virtual char* getName() const {return name;}
    virtual int getNumOfNotes() const {return numOfNotes;}
    virtual Note* getNotes() {return notesInElement;}
    virtual void setName(char* name) {this->name = name;}
    virtual void setNumOfNotes(const int numOfNotes) {this->numOfNotes = numOfNotes;}
    virtual void setNotes(Note* notesInElement) {this->notesInElement = notesInElement;}
    virtual void play() const = 0;
};

class Scale : public MusicalElement{
  private:
    enum Type{
      SCALE_TYPE_MAJOR,
      SCALE_TYPE_NATURAL_MINOR,
      SCALE_TYPE_MELODIC_MINOR,
      SCALE_TYPE_HARMONIC_MINOR
    };
    enum Mode{
      SCALE_MODE_IONIAN,
      SCALE_MODE_DORIAN,
      SCALE_MODE_PHRYGIAN,
      SCALE_MODE_LYDIAN,
      SCALE_MODE_MIXOLYDIAN,
      SCALE_MODE_AEOLIAN,
      SCALE_MODE_LOCRIAN
    };
    Type type;
    Mode mode;

  public:
    Scale(char* name) : MusicalElement(name){}
    Scale(char* name, int numOfNotes, Note* notes) : MusicalElement(name, numOfNotes, notes){}
    ~Scale(){}
    void setType(Type type){this->type = type;}
    void setMode(Mode mode){this->mode = mode;}
};

class Chord : public MusicalElement{
  private:
    enum Accidental{
      CHORD_ADDICENTAL_NATURAL,
      CHORD_ADDICENTAL_FLAT,
      CHORD_ADDICENTAL_SHARP
    };
    enum Quality{
      CHORD_QUALITY_MAJOR,
      CHORD_QUALITY_MINOR,
      CHORD_QUALITY_DIMINISHED,
      CHORD_QUALITY_AUGMENTED,
      CHORD_QUALITY_SUSPENDED
    };
    enum Addition{
      CHORD_ADDITION_7,
      CHORD_ADDITION_6
    };
    Accidental accidental;
    Quality quality;
    Addition addition;

  public:
    Chord(char* name) : MusicalElement(name){}
    Chord(char* name, int numOfNotes, Note* notes) : MusicalElement(name, numOfNotes, notes){}
    ~Chord(){}
    void setAccidental(Accidental accidental){this->accidental = accidental;}
    void setQuality(Quality quality){this->quality = quality;}
    void setAddition(Addition addition){this->addition = addition;}
};

class Arpeggiator : public MusicalElement{
  private:
    int arpSpeed{0};
    Scale* scale;

  public:
    Arpeggiator(char* name) : MusicalElement(name){}
    Arpeggiator(char* name, int numOfNotes, Note* notes) : MusicalElement(name, numOfNotes, notes){}
    ~Arpeggiator(){}
};

class Riff : public MusicalElement{
  private:
	
  public:
};

#endif