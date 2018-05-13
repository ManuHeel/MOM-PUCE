// Libraries and sub-files
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

// Wavetables
uint8_t SQUARE_1_WAVETABLE [16] = { 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255 };
uint8_t SQUARE_2_WAVETABLE [16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255 };
uint8_t SQUARE_3_WAVETABLE [16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255 };
uint8_t TRIANGLE_WAVETABLE [16] = { 127, 159, 191, 223, 255, 223, 191, 159, 127, 95, 63, 31, 0, 31, 63, 95 };
uint8_t SAWTOOTH_WAVETABLE [16] = { 127, 143, 159, 175, 191, 207, 223, 239, 0, 15, 31, 47, 63, 79, 95, 111 };
uint8_t SINE_WAVETABLE [16] = { 128, 176, 218, 245, 255, 245, 218, 176, 128, 79, 37, 10, 0, 10, 37, 79 };

// Enums
enum Waveform { SQUARE_1, SQUARE_2, SQUARE_3, TRIANGLE, SAWTOOTH, SINE };
enum PolyphonyMode { MONO, POLY, ARP, SLIDE };

// Oscillators
struct Oscillator {
  uint8_t index; // Main identifier
  volatile unsigned short phase ;// Current phase (updates on every period defined by the note chosen)
  volatile unsigned short value; // Current value to output (waveform shifted by phase)
  volatile float gain; // Gain value (0 to 1)
  bool active; // Is the oscillator currently used?
  Waveform waveform; // Waveform identifier
  uint8_t* wavetable; // Pointer to wavetable
} osc1, osc2, osc3;

// Keyboards
struct Keyboard{
  Waveform waveform; // Waveform identifier
  PolyphonyMode polyphonyMode; // Polyphony mode identifier
  uint8_t activeKeys[10] = {0,0,0,0,0,0,0,0,0,0}; // Current active keys (i.e. pressed keys)
  uint8_t nextKeyIndex = 0; // Index to next free active key. Back to 0 if full.
  uint8_t oscillatorIndex; // Index identifying the reserved oscillator (in MONO)
} upperKeyboard, lowerKeyboard, pedalBoard;

// Timer based functions
void processAudio() {
  //double divider = osc1.gain + osc2.gain + osc3.gain; // Divide the master gain by the sum of each gain // MEH
  double divider = 3;
  uint16_t outputValue = (double) (osc1.value*osc1.gain + osc2.value*osc2.gain + osc3.value*osc3.gain) / divider; // Compute the current audio signal to output
  analogWrite(AUDIO_OUTPUT_PIN, outputValue);
}
void processOsc1() {
  osc1.value = osc1.wavetable[osc1.phase];
  if (osc1.phase < 15) osc1.phase++; else osc1.phase = 0;
}
void processOsc2() {
  osc2.value = osc2.wavetable[osc2.phase];
  if (osc2.phase < 15) osc2.phase++; else osc2.phase = 0;
}
void processOsc3() {
  osc3.value = osc3.wavetable[osc3.phase];
  if (osc3.phase < 15) osc3.phase++; else osc3.phase = 0;
}

void updateOscillator(Oscillator& oscillator, float period, Waveform waveform, uint8_t gain) {
  if(gain > 0) {
    if (waveform != oscillator.waveform) {
      oscillator.waveform = waveform;
      switch (waveform) {
        case SQUARE_1:
          oscillator.wavetable = SQUARE_1_WAVETABLE;
          break;
        case SQUARE_2:
          oscillator.wavetable = SQUARE_2_WAVETABLE;
          break;
        case SQUARE_3:
          oscillator.wavetable = SQUARE_3_WAVETABLE;
          break;
        case TRIANGLE:
          oscillator.wavetable = TRIANGLE_WAVETABLE;
          break;
        case SAWTOOTH:
          oscillator.wavetable = SAWTOOTH_WAVETABLE;
          break;
        case SINE:
          oscillator.wavetable = SINE_WAVETABLE;
          break;
      }
    }
    switch (oscillator.index) {
      case 1:
        OSC1_TIMER.begin(processOsc1, period); // Set the frequency (1/period)
        break;
      case 2:
        OSC2_TIMER.begin(processOsc2, period); // Set the frequency (1/period)
        break;
      case 3:
        OSC3_TIMER.begin(processOsc1, period); // Set the frequency (1/period)
        break;
    }
    oscillator.active = true;
  } else {
    oscillator.active = false;
  }
  oscillator.gain = gain;
}

void addNote(uint8_t midiNote, Keyboard& keyboard) {
  switch (keyboard.polyphonyMode) {
    case MONO:
      if (keyboard.nextKeyIndex <= 9) { // If there is still room for a new key
        keyboard.activeKeys[keyboard.nextKeyIndex] = midiNote; // Add the new note at the end of the active keys
        keyboard.nextKeyIndex++; // Update the index of the end of the active keys
        switch (keyboard.oscillatorIndex) {
          case 1:
            updateOscillator(osc1, keyToPeriod(midiNote), keyboard.waveform, 1); // Ask the oscillator to play the very new note
            break;
          case 2:
            updateOscillator(osc2, keyToPeriod(midiNote), keyboard.waveform, 1); // Ask the oscillator to play the very new note
            break;
          case 3:
            updateOscillator(osc3, keyToPeriod(midiNote), keyboard.waveform, 1); // Ask the oscillator to play the very new note
            break;
        }
      }
      break;
  }
}

void removeNote(uint8_t midiNote, Keyboard& keyboard) {
  switch (keyboard.polyphonyMode) {
    case MONO:
      /* Remove the element from the array */
      uint8_t noteToRemoveIndex = 255; // Note to remove (255 while it has not been found)
      int i = 0; // Main iterator
      while (i < 10) { // As long as we haven't reached the end nor found the note to remove...
        if (keyboard.activeKeys[i] == midiNote) {
          noteToRemoveIndex = i; // The note to remove has been found
          break;
        }
        i++;
      }
      bool isTheCurrentNote = false;
      if (noteToRemoveIndex == keyboard.nextKeyIndex-1) isTheCurrentNote = true; // If the note to remove was the last, we'll have to play the previous note, if it was not, we won't change the current note played
      for (i = noteToRemoveIndex; i < keyboard.nextKeyIndex; i++) { // From the note to remove to the end of the array...
        keyboard.activeKeys[i] = keyboard.activeKeys[i+1]; // Shift each element 1 row left
      }
      for (i = keyboard.nextKeyIndex; i < 10; i++) { // From the note to remove to the end of the array...
        keyboard.activeKeys[i] = 0; // Shift each element 1 row left
      }
      keyboard.nextKeyIndex--; // Update the index of the end of the active keys

      if (keyboard.nextKeyIndex != 0 && isTheCurrentNote) { // If there still is an active note to play and we did remove the very current playing one
        switch (keyboard.oscillatorIndex) {
          case 1:
            updateOscillator(osc1, keyToPeriod(keyboard.activeKeys[keyboard.nextKeyIndex-1]), keyboard.waveform, 1); // Ask the oscillator to play the last played note still active
            break;
          case 2:
            updateOscillator(osc2, keyToPeriod(keyboard.activeKeys[keyboard.nextKeyIndex-1]), keyboard.waveform, 1); // Ask the oscillator to play the last played note still active
            break;
          case 3:
            updateOscillator(osc3, keyToPeriod(keyboard.activeKeys[keyboard.nextKeyIndex-1]), keyboard.waveform, 1); // Ask the oscillator to play the last played note still active
            break;
        }
      } else if (keyboard.nextKeyIndex == 0) { // Else if it was the last one possible to play 
        switch (keyboard.oscillatorIndex) {
          case 1:
            updateOscillator(osc1, 0, keyboard.waveform, 0); // Ask the oscillator to stop
            break;
          case 2:
            updateOscillator(osc2, 0, keyboard.waveform, 0); // Ask the oscillator to stop
            break;
          case 3:
            updateOscillator(osc3, 0, keyboard.waveform, 0); // Ask the oscillator to stop
            break;
        }
      }
      break;
  }
}

void setup() {
  // Main setup
  Serial.begin(9600); // USB is always 12 Mbit/sec
  pinMode(AUDIO_OUTPUT_PIN, OUTPUT); // Initialize the DAC
  AUDIO_OUTPUT_TIMER.begin(processAudio, 20);  // Audio output processing at roughly 48kHz
  // Initialize the oscillators
  osc1.index = 1;
  osc1.active = false;
  osc1.phase = 0;
  osc1.gain = 0;
  osc1.value = 127;
  osc1.waveform = SQUARE_1;
  osc1.wavetable = SQUARE_1_WAVETABLE;
  osc2.index = 2;
  osc2.active = false;
  osc2.phase = 0;
  osc2.gain = 0;
  osc2.value = 127;
  osc2.waveform = SQUARE_1;
  osc2.wavetable = SQUARE_1_WAVETABLE;
  osc3.index = 3;
  osc3.active = false;
  osc3.phase = 0;
  osc3.gain = 0;
  osc3.value = 127;
  osc3.waveform = SQUARE_1;
  osc3.wavetable = SQUARE_1_WAVETABLE;
  // Initialize the keyboards
  upperKeyboard.waveform = SQUARE_2;
  upperKeyboard.polyphonyMode = MONO;
  upperKeyboard.oscillatorIndex = 1;
  lowerKeyboard.waveform = SAWTOOTH;
  lowerKeyboard.polyphonyMode = MONO;
  lowerKeyboard.oscillatorIndex = 2;
  pedalBoard.waveform = TRIANGLE;
  pedalBoard.polyphonyMode = MONO;
  pedalBoard.oscillatorIndex = 3;
  // Wait a bit to ignore strange initialization messages from the MIDI module, then start the MIDI communication
  delay(3000);
  MIDI.begin(MIDI_CHANNEL_OMNI); // Initialize MIDI communications
}

// The main program
void loop() {
  // Update MIDI
  int midiType, midiNote, midiVelocity, midiChannel, midiD1, midiD2;
  if (MIDI.read()) { // If there is a MIDI message incoming
    midiType = MIDI.getType();
    Keyboard* keyboard;
    switch (midiType) {
      case midi::NoteOn:
        midiNote = MIDI.getData1();
        midiVelocity = MIDI.getData2();
        midiChannel = MIDI.getChannel();
        switch (midiChannel) {
          case 1:
            keyboard = &upperKeyboard;
            break;
          case 2:
            keyboard = &lowerKeyboard;
            break;
          case 3:
            keyboard = &pedalBoard;
            break;
        }
        if (midiVelocity > 0) {
          addNote(midiNote, *keyboard);
        } else {
          removeNote(midiNote, *keyboard);
        }
        break;
      case midi::NoteOff:
        midiNote = MIDI.getData1();
        midiVelocity = MIDI.getData2();
        midiChannel = MIDI.getChannel();
        switch (midiChannel) {
          case 1:
            keyboard = &upperKeyboard;
            break;
          case 2:
            keyboard = &lowerKeyboard;
            break;
          case 3:
            keyboard = &pedalBoard;
            break;
        }
        removeNote(midiNote, *keyboard);
        break;
      default:
        midiD1 = MIDI.getData1();
        midiD2 = MIDI.getData2();
        midiChannel = MIDI.getChannel();
        // TODO : Do something with the other info
    }
  }
}