#include <MozziGuts.h>
#include <Oscil.h> // oscillator template
#include <tables/sin512_int8.h> // sine table
#include <tables/saw_analogue512_int8.h> // saw table
#include <tables/smoothsquare8192_int8.h> // square table
#include <MIDI.h> // Teensy native MIDI I/O library
#include "manual.h" // Manual system for the organ

// use: Oscil <table_size, update_rate> oscilName (wavetable), look in .h file of table #included above
// Upper Manual Oscillators
Oscil <SMOOTHSQUARE8192_NUM_CELLS, AUDIO_RATE> UM_aSin1(SMOOTHSQUARE8192_DATA);
byte UM_aOsc1_Gain = 127;
Oscil <SMOOTHSQUARE8192_NUM_CELLS, AUDIO_RATE> UM_aSin2(SMOOTHSQUARE8192_DATA);
byte UM_aOsc2_Gain = 127;
Oscil <SMOOTHSQUARE8192_NUM_CELLS, AUDIO_RATE> UM_aSin3(SMOOTHSQUARE8192_DATA);
byte UM_aOsc3_Gain = 127;
// Lower Manual Oscillators
Oscil <SAW_ANALOGUE512_NUM_CELLS, AUDIO_RATE> LM_aSin1(SAW_ANALOGUE512_DATA);
byte LM_aOsc1_Gain = 127;
Oscil <SAW_ANALOGUE512_NUM_CELLS, AUDIO_RATE> LM_aSin2(SAW_ANALOGUE512_DATA);
byte LM_aOsc2_Gain = 127;
Oscil <SAW_ANALOGUE512_NUM_CELLS, AUDIO_RATE> LM_aSin3(SAW_ANALOGUE512_DATA);
byte LM_aOsc3_Gain = 127;
// Pedal Board Oscillators
Oscil <SIN512_NUM_CELLS, AUDIO_RATE> PB_aSin(SIN512_DATA);
byte PB_aSin_Gain = 127;

// Use #define for CONTROL_RATE, not a constant
#define CONTROL_RATE 256 // powers of 2 please

// Debug LED
int ledPin = 13;

// Playing notes
int UM_PlayingNotes[3] = {0x00, 0x00, 0x00};
int LM_PlayingNotes[3] = {0x00, 0x00, 0x00};
int PB_PlayingNote = 0x00;

// Modes
bool UM_Polyphonic = false;
bool LM_Polyphonic = false;

void setup(){
  // Blink LED to start
  pinMode(ledPin, OUTPUT);
  analogWriteResolution(12);
  digitalWrite(ledPin, HIGH);
  delay(300);
  digitalWrite(ledPin, LOW);
  // Setup MIDI
  MIDI.begin(MIDI_CHANNEL_OMNI);
  Serial.begin(57600);
  Serial.println("MIDI Input Test");
  // Start Mozzi
  startMozzi(CONTROL_RATE); // set a control rate of 64 (powers of 2 please)
}


void updateControl(){
  // Update MIDI
  int midiType, midiNote, midiVelocity, midiChannel, midiD1, midiD2;
  if (MIDI.read()) { // If there is a MIDI message incoming
    byte midiType = MIDI.getType();
    switch (midiType) {
      case NoteOn:
        midiNote = MIDI.getData1();
        midiVelocity = MIDI.getData2();
        midiChannel = MIDI.getChannel();
        if (midiVelocity > 0) {
          playNote(midiNote, midiChannel);
        } else {
          stopNote(midiNote, midiChannel);
        }
        break;
      case NoteOff:
        midiNote = MIDI.getData1();
        midiVelocity = MIDI.getData2();
        midiChannel = MIDI.getChannel();
        stopNote(midiNote, midiChannel);
        break;
      default:
        midiD1 = MIDI.getData1();
        midiD2 = MIDI.getData2();
        midiChannel = MIDI.getChannel();
        Serial.println(String("Message, type=") + midiType + ", data = " + midiD1 + " " + midiD2);
        if (midiD1 == 0x19 && midiChannel == 1) { // Flutes Leslie / Main -> Monophonic / Polyphonic mode switch for Upper Manual
          UM_Polyphonic = !UM_Polyphonic;
        } else if (midiD1 == 0x18 && midiChannel == 2) { // Lower Manual Leslie / Main -> Monophonic / Polyphonic mode switch for Lower Manual
          LM_Polyphonic = !LM_Polyphonic;
        }
    }
  }
}


int updateAudio(){
  int out =
    (UM_aSin1.next() * UM_aOsc1_Gain) +
    (UM_aSin2.next() * UM_aOsc2_Gain) +
    (UM_aSin3.next() * UM_aOsc3_Gain) +
    (LM_aSin1.next() * LM_aOsc1_Gain) +
    (LM_aSin2.next() * LM_aOsc2_Gain) +
    (LM_aSin3.next() * LM_aOsc3_Gain) +
    (PB_aSin.next() * PB_aSin_Gain);
  
  return out>>4 ; // return an int signal centred around 0 and shift to 8
}


void loop(){
  audioHook(); // required here
}

// Play a note on a manual or change it if it's in a monophonic mode (or has exceeded the limit of notes)
void playNote(int note, int channel) {
  if (channel == 1) { // Upper Manual note
    if (UM_Polyphonic) { // If the Upper Manual is in Polyphonic Mode
      addPolyphonicNote(note, channel);
    } else {
      addMonophonicNote(note, channel);
    }
  } else if (channel == 2) { // Lower Manual note
    if (LM_Polyphonic) { // If the Lower Manual is in Polyphonic Mode
      addPolyphonicNote(note, channel);
    } else {
      addMonophonicNote(note, channel);
    }
  } else if (channel == 3) { // Pedal Board note
    PB_PlayingNote = note;
    PB_aSin.setFreq(pitchToFrequency(note));
    PB_aSin_Gain = 127;
  }
}

// Stop playing a note if it was the last key of a manual to be pressed
void stopNote(int note, int channel) {
  if (channel == 1) { // Upper Manual note
    if (UM_Polyphonic) { // If the Upper Manual is in Polyphonic Mode
      removePolyphonicNote(note, channel);
    } else if (note == UM_PlayingNotes[0]) { // If it's the currently played note (so the last key was released)
      removeMonophonicNote(note, channel);
    }
  } else if (channel == 2) { // Lower Manual note
    if (LM_Polyphonic) { // If the Upper Manual is in Polyphonic Mode
      removePolyphonicNote(note, channel);
    } else if (note == LM_PlayingNotes[0]) { // If it's the currently played note (so the last key was released)
      removeMonophonicNote(note, channel);
    }
  } else if (channel == 3) { // Pedal Board note
    if (note == PB_PlayingNote) { // If it's the currently played note (so the last key was released)
      PB_aSin_Gain = 0;
    }
  }
}

void addPolyphonicNote(int note, int channel) {
  if (channel == 1) { // The note is on the Upper Manual
    if (UM_PlayingNotes[0] == 0x00) { // If the first slot is available
      UM_PlayingNotes[0] = note;
      UM_aSin1.setFreq(pitchToFrequency(note));
      UM_aOsc1_Gain = 127;
    } else if (UM_PlayingNotes[1] == 0x00) { // If the second slot is available
      UM_PlayingNotes[1] = note;
      UM_aSin2.setFreq(pitchToFrequency(note));
      UM_aOsc2_Gain = 127;
    } else if (UM_PlayingNotes[2] == 0x00) { // If the third slot is available
      UM_PlayingNotes[2] = note;
      UM_aSin3.setFreq(pitchToFrequency(note));
      UM_aOsc3_Gain = 127;
    } else { // If no slot is available, remove the first, shift everything of 1 slot and add the new note at the end
      UM_PlayingNotes[0] = UM_PlayingNotes[1];
      UM_aSin1.setFreq(pitchToFrequency(UM_PlayingNotes[0]));
      UM_aOsc1_Gain = 127;
      UM_PlayingNotes[1] = UM_PlayingNotes[2];
      UM_aSin2.setFreq(pitchToFrequency(UM_PlayingNotes[1]));
      UM_aOsc2_Gain = 127;
      UM_PlayingNotes[2] = note;
      UM_aSin3.setFreq(pitchToFrequency(UM_PlayingNotes[2]));
      UM_aOsc3_Gain = 127;
    }
  } else if (channel == 2) { // The note is on the Upper Manual
    if (LM_PlayingNotes[0] == 0x00) { // If the first slot is available
      LM_PlayingNotes[0] = note;
      LM_aSin1.setFreq(pitchToFrequency(note));
      LM_aOsc1_Gain = 127;
    } else if (LM_PlayingNotes[1] == 0x00) { // If the second slot is available
      LM_PlayingNotes[1] = note;
      LM_aSin2.setFreq(pitchToFrequency(note));
      LM_aOsc2_Gain = 127;
    } else if (LM_PlayingNotes[2] == 0x00) { // If the third slot is available
      LM_PlayingNotes[2] = note;
      LM_aSin3.setFreq(pitchToFrequency(note));
      LM_aOsc3_Gain = 127;
    } else { // If no slot is available, remove the first, shift everything of 1 slot and add the new note at the end
      LM_PlayingNotes[0] = LM_PlayingNotes[1];
      LM_aSin1.setFreq(pitchToFrequency(LM_PlayingNotes[0]));
      LM_aOsc1_Gain = 127;
      LM_PlayingNotes[1] = LM_PlayingNotes[2];
      LM_aSin2.setFreq(pitchToFrequency(LM_PlayingNotes[1]));
      LM_aOsc2_Gain = 127;
      LM_PlayingNotes[2] = note;
      LM_aSin3.setFreq(pitchToFrequency(LM_PlayingNotes[2]));
      LM_aOsc3_Gain = 127;
    }
  }
}

void removePolyphonicNote(int note, int channel) {
  if (channel == 1) { // The note is on the Upper Manual
    if (UM_PlayingNotes[0] == note) { // If the note to remove is on the first slot
      UM_PlayingNotes[0] = UM_PlayingNotes[1];
      UM_aSin1.setFreq(pitchToFrequency(UM_PlayingNotes[0]));
      UM_aOsc1_Gain = 127;
      UM_PlayingNotes[1] = UM_PlayingNotes[2];
      UM_aSin2.setFreq(pitchToFrequency(UM_PlayingNotes[1]));
      UM_aOsc2_Gain = 127;
      UM_PlayingNotes[2] = 0x00;
    } else if (UM_PlayingNotes[1] == note) { // If the note to remove is on the second slot
      UM_PlayingNotes[1] = UM_PlayingNotes[2];
      UM_aSin2.setFreq(pitchToFrequency(UM_PlayingNotes[1]));
      UM_aOsc2_Gain = 127;
      UM_PlayingNotes[2] = 0x00;
    } else if (UM_PlayingNotes[2] == note) { // If the note to remove is on the third slot
      UM_PlayingNotes[2] = 0x00;
    }
    if (UM_PlayingNotes[0] == 0x00) {
      UM_aOsc1_Gain = 0;
    }
    if (UM_PlayingNotes[1] == 0x00) {
      UM_aOsc2_Gain = 0;
    }
    if (UM_PlayingNotes[2] == 0x00) {
      UM_aOsc3_Gain = 0;
    }
  } else if (channel == 2) { // The note is on the Upper Manual
    if (LM_PlayingNotes[0] == note) { // If the note to remove is on the first slot
      LM_PlayingNotes[0] = LM_PlayingNotes[1];
      LM_aSin1.setFreq(pitchToFrequency(LM_PlayingNotes[0]));
      LM_aOsc1_Gain = 127;
      LM_PlayingNotes[1] = LM_PlayingNotes[2];
      LM_aSin2.setFreq(pitchToFrequency(LM_PlayingNotes[1]));
      LM_aOsc2_Gain = 127;
      LM_PlayingNotes[2] = 0x00;
      LM_aOsc3_Gain = 0;
    } else if (LM_PlayingNotes[1] == note) { // If the note to remove is on the second slot
      LM_PlayingNotes[1] = LM_PlayingNotes[2];
      LM_aSin2.setFreq(pitchToFrequency(LM_PlayingNotes[1]));
      LM_aOsc2_Gain = 127;
      LM_PlayingNotes[2] = 0x00;
      LM_aOsc3_Gain = 0;
    } else if (LM_PlayingNotes[2] == note) { // If the note to remove is on the third slot
      LM_PlayingNotes[2] = 0x00;
      LM_aOsc3_Gain = 0;
    }
    if (LM_PlayingNotes[0] == 0x00) {
      LM_aOsc1_Gain = 0;
    }
    if (LM_PlayingNotes[1] == 0x00) {
      LM_aOsc2_Gain = 0;
    }
    if (LM_PlayingNotes[2] == 0x00) {
      LM_aOsc3_Gain = 0;
    }
  }
}

void addMonophonicNote(int note, int channel) {
  if (channel == 1) { // The note is on the Upper Manual
    if (UM_PlayingNotes[0] == 0x00) { // If the first slot is available
      UM_PlayingNotes[0] = note;
      UM_aSin1.setFreq(pitchToFrequency(note));
      UM_aOsc1_Gain = 127;
    } else if (UM_PlayingNotes[1] == 0x00) { // If the second slot is available
      UM_PlayingNotes[1] = note;
      UM_aSin2.setFreq(pitchToFrequency(note));
      UM_aOsc1_Gain = 0;
      UM_aOsc2_Gain = 127;
    } else { // If the third slot is available or else
      UM_PlayingNotes[2] = note;
      UM_aSin3.setFreq(pitchToFrequency(note));
      UM_aOsc1_Gain = 0;
      UM_aOsc2_Gain = 0;
      UM_aOsc3_Gain = 127;
    }
  } else if (channel == 2) { // The note is on the Lower Manual
    if (LM_PlayingNotes[0] == 0x00) { // If the first slot is available
      LM_PlayingNotes[0] = note;
      LM_aSin1.setFreq(pitchToFrequency(note));
      LM_aOsc1_Gain = 127;
    } else if (LM_PlayingNotes[1] == 0x00) { // If the second slot is available
      LM_PlayingNotes[1] = note;
      LM_aSin2.setFreq(pitchToFrequency(note));
      LM_aOsc1_Gain = 0;
      LM_aOsc2_Gain = 127;
    } else { // If the third slot is available or else
      LM_PlayingNotes[2] = note;
      LM_aSin3.setFreq(pitchToFrequency(note));
      LM_aOsc1_Gain = 0;
      LM_aOsc2_Gain = 0;
      LM_aOsc3_Gain = 127;
    }
  }
}

void removeMonophonicNote(int note, int channel) {
  if (channel == 1) { // The note is on the Upper Manual
    if (UM_PlayingNotes[0] == note) { // If the note to remove is on the first slot
      UM_PlayingNotes[0] = UM_PlayingNotes[1];
      UM_aSin1.setFreq(pitchToFrequency(UM_PlayingNotes[0]));
      UM_aOsc1_Gain = 0;
      UM_PlayingNotes[1] = UM_PlayingNotes[2];
      UM_aSin2.setFreq(pitchToFrequency(UM_PlayingNotes[1]));
      UM_aOsc2_Gain = 127;
      UM_PlayingNotes[2] = 0x00;
    } else if (UM_PlayingNotes[1] == note) { // If the note to remove is on the second slot
      UM_PlayingNotes[1] = UM_PlayingNotes[2];
      UM_aSin2.setFreq(pitchToFrequency(UM_PlayingNotes[1]));
      UM_aOsc2_Gain = 127;
      UM_PlayingNotes[2] = 0x00;
    } else if (UM_PlayingNotes[2] == note) { // If the note to remove is on the third slot
      UM_PlayingNotes[2] = 0x00;
    }
    if (UM_PlayingNotes[0] == 0x00) {
      UM_aOsc1_Gain = 0;
    }
    if (UM_PlayingNotes[1] == 0x00) {
      UM_aOsc2_Gain = 0;
    }
    if (UM_PlayingNotes[2] == 0x00) {
      UM_aOsc3_Gain = 0;
    }
  } else if (channel == 2) { // The note is on the Upper Manual
    if (LM_PlayingNotes[0] == note) { // If the note to remove is on the first slot
      LM_PlayingNotes[0] = LM_PlayingNotes[1];
      LM_aSin1.setFreq(pitchToFrequency(LM_PlayingNotes[0]));
      LM_aOsc1_Gain = 0;
      LM_PlayingNotes[1] = LM_PlayingNotes[2];
      LM_aSin2.setFreq(pitchToFrequency(LM_PlayingNotes[1]));
      LM_aOsc2_Gain = 127;
      LM_PlayingNotes[2] = 0x00;
    } else if (LM_PlayingNotes[1] == note) { // If the note to remove is on the second slot
      LM_PlayingNotes[1] = LM_PlayingNotes[2];
      LM_aSin2.setFreq(pitchToFrequency(LM_PlayingNotes[1]));
      LM_aOsc2_Gain = 127;
      LM_PlayingNotes[2] = 0x00;
    } else if (LM_PlayingNotes[2] == note) { // If the note to remove is on the third slot
      LM_PlayingNotes[2] = 0x00;
    }
    if (LM_PlayingNotes[0] == 0x00) {
      LM_aOsc1_Gain = 0;
    }
    if (LM_PlayingNotes[1] == 0x00) {
      LM_aOsc2_Gain = 0;
    }
    if (LM_PlayingNotes[2] == 0x00) {
      LM_aOsc3_Gain = 0;
    }
  }
}



