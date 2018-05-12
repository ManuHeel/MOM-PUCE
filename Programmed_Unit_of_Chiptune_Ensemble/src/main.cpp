#include <Arduino.h>
#include <MIDI.h>
#include "pitches.h"

// IntervalTimers for audio processing and oscillators 
IntervalTimer AUDIO_OUTPUT_TIMER;
IntervalTimer OSC1_TIMER;
IntervalTimer OSC2_TIMER;
IntervalTimer OSC3_TIMER;

// Output analog pin
const int AUDIO_OUTPUT_PIN = A14;

// MIDI settings
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

// Waveform arrays
int SQUARE_WAVE [16] = { 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255 };

// Oscillators
volatile unsigned short OSC1_PHASE = 0;
volatile unsigned short OSC1_VALUE = 127;
volatile unsigned long OSC1_GAIN = 0;

volatile unsigned short OSC2_PHASE = 0;
volatile unsigned short OSC2_VALUE = 127;
volatile unsigned long OSC2_GAIN = 0;

volatile unsigned short OSC3_PHASE = 0;
volatile unsigned short OSC3_VALUE = 127;
volatile unsigned long OSC3_GAIN = 0;

// Timer based functions
void processAudio() {
  uint8_t divider = OSC1_GAIN + OSC2_GAIN + OSC3_GAIN; // Divide the master gain by the sum of each gain
  uint16_t outputValue = (OSC1_VALUE*OSC1_GAIN + OSC2_VALUE*OSC2_GAIN + OSC3_VALUE*OSC3_GAIN) / divider; // Compute the current audio signal to output
  analogWrite(AUDIO_OUTPUT_PIN, outputValue);
}
void updateOsc1() {
  OSC1_VALUE = SQUARE_WAVE[OSC1_PHASE];
  if (OSC1_PHASE < 15) OSC1_PHASE++; else OSC1_PHASE = 0;
}
void updateOsc2() {
  OSC2_VALUE = SQUARE_WAVE[OSC2_PHASE];
  if (OSC2_PHASE < 15) OSC2_PHASE++; else OSC2_PHASE = 0;
}
void updateOsc3() {
  OSC3_VALUE = SQUARE_WAVE[OSC3_PHASE];
  if (OSC3_PHASE < 15) OSC3_PHASE++; else OSC3_PHASE = 0;
}

void playNote(uint8_t midiNote, uint8_t midiChannel) {
  if (midiChannel == 1) {
    OSC1_GAIN = 1;
    OSC1_TIMER.begin(updateOsc1, pitchToPeriod(midiNote));
  } else if (midiChannel == 2) {
    OSC2_GAIN = 1;
    OSC2_TIMER.begin(updateOsc2, pitchToPeriod(midiNote));
  } 
}

void stopNote(uint8_t midiNote, uint8_t midiChannel) {
  if (midiChannel == 1) {
    OSC1_GAIN = 0;
  } else if (midiChannel == 2) {
    OSC2_GAIN = 0;
  } 
}

void setup() {
  pinMode(AUDIO_OUTPUT_PIN, OUTPUT);
  MIDI.begin(MIDI_CHANNEL_OMNI);
  AUDIO_OUTPUT_TIMER.begin(processAudio, 20);  // Audio output processing at roughly 48kHz 440Hz
  OSC1_TIMER.begin(updateOsc1, 0);  // Set frequency of the oscillator to roughly 440Hz
  OSC2_TIMER.begin(updateOsc2, 0);  // Set frequency of the oscillator to roughly 220Hz
  OSC3_TIMER.begin(updateOsc3, 0);  // Set frequency of the oscillator to roughly 330Hz
  playNote(0x39, 1);
}

// The main program
void loop() {
  // Update MIDI
  int midiType, midiNote, midiVelocity, midiChannel, midiD1, midiD2;
  if (MIDI.read()) { // If there is a MIDI message incoming
    midiType = MIDI.getType();
    switch (midiType) {
      case midi::NoteOn:
        midiNote = MIDI.getData1();
        midiVelocity = MIDI.getData2();
        midiChannel = MIDI.getChannel();
        if (midiVelocity > 0) {
          playNote(midiNote, midiChannel);
        } else {
          stopNote(midiNote, midiChannel);
        }
        break;
      case midi::NoteOff:
        midiNote = MIDI.getData1();
        midiVelocity = MIDI.getData2();
        midiChannel = MIDI.getChannel();
        stopNote(midiNote, midiChannel);
        break;
      default:
        midiD1 = MIDI.getData1();
        midiD2 = MIDI.getData2();
        midiChannel = MIDI.getChannel();
        // TODO : Do something with the other info
    }
  }
}