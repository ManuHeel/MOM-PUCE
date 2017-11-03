// MIDI Organ Module
// Sending MIDI data input from a Siel hb720 Organ
// Author: Manuel & Frederic RUSSELLO
// Date : 2016

// Pin connections for Mega2560:
// Chip pin 1 (/PL)  goes to LATCH (D9)
// Chip pin 2 (CP)   goes to SCK   (D52)
// Chip pin 9 (Q7)   goes to MISO  (D50)

#include <SPI.h>

// Constants for the LATCH pin and the Debug LED
const byte LATCH = 9;
const int LED = 47;
const int SOUND = 39;
int playingNote = 0x00;

// Constants and variables for the analog inputs
const int VOLUME = A0;
int volumeValue = 0;
int lastVolumeValue = 0;
int volumeDebouncer = 0;
const int TEMPO = A1;
int tempoValue = 0;
int lastTempoValue = 0;
int tempoDebouncer = 0;
const int SUSTAIN = A2;
int sustainValue = 0;
int lastSustainValue = 0;
int sustainDebouncer = 0;
const int PIANO = A3;
int pianoValue = 0;
int lastPianoValue = 0;
int pianoDebouncer = 0;
const int LOWER_MANUAL = A4;
int lowerManualValue = 0;
int lastLowerManualValue = 0;
int lowerManualDebouncer = 0;
const int PEDAL = A5;
int pedalValue = 0;
int lastPedalValue = 0;
int pedalDebouncer = 0;
const int VIBRATO = A6;
int vibratoValue = 0;
int lastVibratoValue = 0;
int vibratoDebouncer = 0;
const int SWELL_PEDAL = A7;
int swellPedalValue = 0;
int lastSwellPedalValue = 0;
int swellPedalDebouncer = 0;

// Setup part
void setup () {
  SPI.begin();
  Serial.begin(115200);
  Serial.println("Begin switch test.");
  pinMode(LATCH, OUTPUT);
  digitalWrite(LATCH, HIGH);
  pinMode(LED, OUTPUT);
  delay(500);
  // Set MIDI baud rate
  Serial1.begin(31250);
}  // End of setup

// Variables for each switch bank (8 digital input) and their previous state
byte switchBank1;
byte oldSwitchBank1;
byte switchBank2;
byte oldSwitchBank2;
byte switchBank3;
byte oldSwitchBank3;
byte switchBank4;
byte oldSwitchBank4;
byte switchBank5;
byte oldSwitchBank5;
byte switchBank6;
byte oldSwitchBank6;
byte switchBank7;
byte oldSwitchBank7;
byte switchBank8;
byte oldSwitchBank8;
byte switchBank9;
byte oldSwitchBank9;
byte switchBank10;
byte oldSwitchBank10;
byte switchBank11;
byte oldSwitchBank11;
byte switchBank12;
byte oldSwitchBank12;
byte switchBank13;
byte oldSwitchBank13;
byte switchBank14;
byte oldSwitchBank14;
byte switchBank15;
byte oldSwitchBank15;
byte switchBank16;
byte oldSwitchBank16;
byte switchBank17;
byte oldSwitchBank17;
byte switchBank18;
byte oldSwitchBank18;
byte switchBank19;
byte oldSwitchBank19;
byte switchBank20;
byte oldSwitchBank20;
byte switchBank21;
byte oldSwitchBank21;

// Key structure declaration
struct Key {
  int note;
  int channel;
};

struct Key LM_Keys[44] = {
  {0x1D, 2}, // LM F2 - 1
  {0x1E, 2}, // LM Fs2 - 2
  {0x1F, 2}, // LM G2 - 3
  {0x20, 2}, // LM Gs2 - 4
  {0x21, 2}, // LM A2 - 5
  {0x22, 2}, // LM As2 - 6
  {0x23, 2}, // LM B2 - 7
  {0x24, 2}, // LM C3 - 8
  {0x25, 2}, // LM Cs3 - 9
  {0x26, 2}, // LM D3 - 10
  {0x27, 2}, // LM Ds3 - 11
  {0x28, 2}, // LM E3 - 12
  {0x29, 2}, // LM F3 - 13
  {0x2A, 2}, // LM Fs3 - 14
  {0x2B, 2}, // LM G3 - 15
  {0x2C, 2}, // LM Gs3 - 16
  {0x2D, 2}, // LM A3 - 17
  {0x2E, 2}, // LM As3 - 18
  {0x2F, 2}, // LM B3 - 19
  {0x30, 2}, // LM C4 - 20
  {0x31, 2}, // LM Cs4 - 21
  {0x32, 2}, // LM D4 - 22
  {0x33, 2}, // LM Ds4 - 23
  {0x34, 2}, // LM E4 - 24
  {0x35, 2}, // LM F4 - 25
  {0x36, 2}, // LM Fs4 - 26
  {0x37, 2}, // LM G4 - 27
  {0x38, 2}, // LM Gs4 - 28
  {0x39, 2}, // LM A4 - 29
  {0x3A, 2}, // LM As4 - 30
  {0x3B, 2}, // LM B4 - 31
  {0x3C, 2}, // LM C5 - 32
  {0x3D, 2}, // LM Cs5 - 33
  {0x3E, 2}, // LM D5 - 34
  {0x3F, 2}, // LM Ds5 - 35
  {0x40, 2}, // LM E5 - 36
  {0x41, 2}, // LM F5 - 37
  {0x42, 2}, // LM Fs5 - 38
  {0x43, 2}, // LM G5 - 39
  {0x44, 2}, // LM Gs5 - 40
  {0x45, 2}, // LM A5 - 41
  {0x46, 2}, // LM As5 - 42
  {0x47, 2}, // LM B5 - 43
  {0x48, 2} // LM C6 - 44
};

struct Key UM_Keys[44] = {
  {0x29, 1}, // UM F3 - 45
  {0x2A, 1}, // UM Fs3 - 46
  {0x2B, 1}, // UM G3 - 47
  {0x2C, 1}, // UM Gs3 - 48
  {0x2D, 1}, // UM A3 - 49
  {0x2E, 1}, // UM As3 - 50
  {0x2F, 1}, // UM B3 - 51
  {0x30, 1}, // UM C4 - 52
  {0x31, 1}, // UM Cs4 - 53
  {0x32, 1}, // UM D4 - 54
  {0x33, 1}, // UM Ds4 - 55
  {0x34, 1}, // UM E4 - 56
  {0x35, 1}, // UM F4 - 57
  {0x36, 1}, // UM Fs4 - 58
  {0x37, 1}, // UM G4 - 59
  {0x38, 1}, // UM Gs4 - 60
  {0x39, 1}, // UM A4 - 61
  {0x3A, 1}, // UM As4 - 62
  {0x3B, 1}, // UM B4 - 63
  {0x3C, 1}, // UM C5 - 64
  {0x3E, 1}, // UM Cs5 - 65 // ERROR HERE
  {0x3D, 1}, // UM D5 - 66 // ERROR HERE
  {0x3F, 1}, // UM Ds5 - 67
  {0x40, 1}, // UM E5 - 68
  {0x41, 1}, // UM F5 - 69
  {0x42, 1}, // UM Fs5 - 70
  {0x43, 1}, // UM G5 - 71
  {0x44, 1}, // UM Gs5 - 72
  {0x45, 1}, // UM A5 - 73
  {0x46, 1}, // UM As5 - 74
  {0x47, 1}, // UM B5 - 75
  {0x48, 1}, // UM C6 - 76
  {0x49, 1}, // UM Cs6 - 77
  {0x4A, 1}, // UM D6 - 78
  {0x4B, 1}, // UM Ds6 - 79
  {0x4C, 1}, // UM E6 - 80
  {0x4D, 1}, // UM F6 - 81
  {0x4E, 1}, // UM Fs6 - 82
  {0x4F, 1}, // UM G6 - 83
  {0x50, 1}, // UM Gs6 - 84
  {0x51, 1}, // UM A6 - 85
  {0x52, 1}, // UM As6 - 86
  {0x53, 1}, // UM B6 - 87
  {0x54, 1} // UM C7 - 88
};

struct Key PB_Keys[44] = {
  {0x18, 3}, // UM C1 - 45
  {0x19, 3}, // UM Cs1 - 46
  {0x1A, 3}, // UM D1 - 47
  {0x1B, 3}, // UM Ds1 - 48
  {0x1C, 3}, // UM E1 - 49
  {0x1D, 3}, // UM F1 - 50
  {0x1E, 3}, // UM Fs1 - 51
  {0x1F, 3}, // UM G1 - 52
  {0x20, 3}, // UM Gs1 - 53
  {0x21, 3}, // UM A1 - 54
  {0x22, 3}, // UM As1 - 55
  {0x23, 3}, // UM B1 - 56
  {0x24, 3} // UM C2 - 57
};

// Switch structure declaration
struct Switch {
  int control;
  int channel;
};

struct Switch switches[64] = {
  {0x10, 4}, //	Leslie Fast / Slow - 89
  {0x11, 4}, //	Vibrato - 90
  {0x12, 4}, //	Vibrato Delay - 91
  {0x13, 4}, //	Reverb - 92
  {0x14, 3}, //	8 / 16-8 - 93
  {0x15, 3}, //	Bass / Tuba - 94
  {0x16, 3}, //	Sustain - 95
  {0x14, 4}, //	Percussion 16 - 96
  {0x15, 4}, //	Percussion 8 - 97
  {0x16, 4}, //	Percussion 4 - 98
  {0x17, 4}, //	Percussion 2+(2/3) - 99
  {0x18, 4}, //	Percussion 2 - 100
  {0x19, 4}, //	Percussion Short / Long - 101
  {0x1A, 4}, //	Percussion Preset - 102
  {0x1B, 4}, //	Flutes Preset - 103
  {0x1C, 4}, //	Orchestra Preset - 104
  {0x1D, 4}, //	Piano - 105
  {0x1E, 4}, //	Harpsichord - 106
  {0x1F, 4}, //	Honky Tonky - 107
  {0x14, 1}, //	Flute 16 - 108
  {0x15, 1}, //	Flute 8 - 109
  {0x16, 1}, //	Flute 4 - 110
  {0x17, 1}, //	Flute 2+(2/3) - 111
  {0x18, 1}, //	Flute 2 - 112
  {0x19, 1}, //	Flutes Leslie Main - 113
  {0x1A, 1}, //	Sustain - 114
  {0x14, 2}, //	Flute 8 - 115
  {0x15, 2}, //	Flute 4 - 116
  {0x16, 2}, //	Viola 8 - 117
  {0x17, 2}, //	Reed 4 - 118
  {0x18, 2}, //	LM Leslie Main - 119
  {0x66, 1}, //	Trombone - 120
  {0x67, 1}, //	Trumpet - 121
  {0x68, 1}, //	Bass Clarinet - 122
  {0x69, 1}, //	Clarinet - 123
  {0x6A, 1}, //	Oboe - 124
  {0x6B, 1}, //	Accordion - 125
  {0x6C, 1}, //	Viola - 126
  {0x66, 4}, //	On - 127
  {0x67, 4}, //	Key Start - 128
  {0x68, 4}, //	Drum - 129
  {0x69, 4}, //	Bass - 130
  {0x6A, 4}, //	Chord - 131
  {0x6B, 4}, //	Special Sweet - 132
  {0x6C, 4}, //	Special Sharp - 133
  {0x6D, 4}, //	Special Module - 134
  {0x6E, 4}, //	Memory Leslie Main - 135
  {0x6F, 4}, //	Memory Chords - 136
  {0x66, 5}, //	Waltz - 137
  {0x67, 5}, //	Jazz waltz - 138
  {0x68, 5}, //	Tango - 139
  {0x69, 5}, //	Swing - 140
  {0x6A, 5}, //	Fox trot - 141
  {0x6B, 5}, //	Country Polka - 142
  {0x6C, 5}, //	Slow Rock - 143
  {0x6D, 5}, //	Slow Beat - 144
  {0x6E, 5}, //	Beat - 145
  {0x6F, 5}, //	Rock'n roll - 146
  {0x70, 5}, //	Bossa nova - 147
  {0x71, 5}, //	Samba - 148
  {0x72, 5}, //	Beguine - 149
  {0x73, 5}, //	Cha cha - 150
  {0x74, 5}, //	Mexico - 151
  {0x04, 4} // Pedal Switch - 165
};
void loop () {
  // Analog input update
  // Volume fader
  volumeValue = (constrain(analogRead(VOLUME), 0, 1023));
  volumeValue = fscale(10, 1010, 0, 127, volumeValue, 0); // Applying a scale curve
  if (((volumeDebouncer < 2) && volumeValue != lastVolumeValue) || ((volumeValue-7 > lastVolumeValue) || (volumeValue+7 < lastVolumeValue))) { // Update the input if it has been updated recently and is different, or is significantly different from the last value
    volumeDebouncer = 0; // Reset the debouncer
    lastVolumeValue = volumeValue; // The last swellPedalValue is the current value for the next cycle test
    updateAnalog(0x07, 4, volumeValue); // Send the new value over MIDI
  }
  if (volumeDebouncer < 2) {
    volumeDebouncer += 1;
  }
  // Tempo fader
  tempoValue = (constrain(analogRead(TEMPO), 0, 1023));
  tempoValue = fscale(10, 1010, 127, 0, tempoValue, 0); // Applying a scale curve
  if (((tempoDebouncer < 2) && tempoValue != lastTempoValue) || ((tempoValue-7 > lastTempoValue) || (tempoValue+7 < lastTempoValue))) { // Update the input if it has been updated recently and is different, or is significantly different from the last value
    tempoDebouncer = 0; // Reset the debouncer
    lastTempoValue = tempoValue; // The last swellPedalValue is the current value for the next cycle test
    updateAnalog(0x03, 4, tempoValue); // Send the new value over MIDI
  }
  if (tempoDebouncer < 2) {
    tempoDebouncer += 1;
  }
  // Sustain fader
  sustainValue = (constrain(analogRead(SUSTAIN), 0, 1023));
  sustainValue = fscale(10, 1010, 0, 127, sustainValue, 5.25); // Applying a scale curve
  if (((sustainDebouncer < 2) && sustainValue != lastSustainValue) || ((sustainValue-7 > lastSustainValue) || (sustainValue+7 < lastSustainValue))) { // Update the input if it has been updated recently and is different, or is significantly different from the last value
    sustainDebouncer = 0; // Reset the debouncer
    lastSustainValue = sustainValue; // The last swellPedalValue is the current value for the next cycle test
    updateAnalog(0x09, 4, sustainValue); // Send the new value over MIDI
  }
  if (sustainDebouncer < 2) {
    sustainDebouncer += 1;
  }
  // Piano fader
  pianoValue = (constrain(analogRead(PIANO), 0, 1023));
  pianoValue = fscale(10, 1010, 0, 127, pianoValue, 5.00); // Applying a scale curve
  if (((pianoDebouncer < 2) && pianoValue != lastPianoValue) || ((pianoValue-7 > lastPianoValue) || (pianoValue+7 < lastPianoValue))) { // Update the input if it has been updated recently and is different, or is significantly different from the last value
    pianoDebouncer = 0; // Reset the debouncer
    lastPianoValue = pianoValue; // The last swellPedalValue is the current value for the next cycle test
    updateAnalog(0x07, 1, pianoValue); // Send the new value over MIDI
  }
  if (pianoDebouncer < 2) {
    pianoDebouncer += 1;
  }
  // Lower Manual fader
  lowerManualValue = (constrain(analogRead(LOWER_MANUAL), 0, 1023));
  lowerManualValue = fscale(10, 1010, 0, 127, lowerManualValue, 5.25); // Applying a scale curve
  if (((lowerManualDebouncer < 2) && lowerManualValue != lastLowerManualValue) || ((lowerManualValue-7 > lastLowerManualValue) || (lowerManualValue+7 < lastLowerManualValue))) { // Update the input if it has been updated recently and is different, or is significantly different from the last value
    lowerManualDebouncer = 0; // Reset the debouncer
    lastLowerManualValue = lowerManualValue; // The last swellPedalValue is the current value for the next cycle test
    updateAnalog(0x07, 2, lowerManualValue); // Send the new value over MIDI
  }
  if (lowerManualDebouncer < 2) {
    lowerManualDebouncer += 1;
  }
  // Pedal fader
  pedalValue = (constrain(analogRead(PEDAL), 0, 1023));
  pedalValue = fscale(10, 1010, 0, 127, pedalValue, 5.25); // Applying a scale curve
  if (((pedalDebouncer < 2) && pedalValue != lastPedalValue) || ((pedalValue-7 > lastPedalValue) || (pedalValue+7 < lastPedalValue))) { // Update the input if it has been updated recently and is different, or is significantly different from the last value
    pedalDebouncer = 0; // Reset the debouncer
    lastPedalValue = pedalValue; // The last swellPedalValue is the current value for the next cycle test
    updateAnalog(0x07, 3, pedalValue); // Send the new value over MIDI
  }
  if (pedalDebouncer < 2) {
    pedalDebouncer += 1;
  }
  // Vibrato fader
  vibratoValue = (constrain(analogRead(VIBRATO), 0, 1023));
  vibratoValue = fscale(10, 1010, 127, 0, vibratoValue, 5.25); // Applying a scale curve
  if (((vibratoDebouncer < 2) && vibratoValue != lastVibratoValue) || ((vibratoValue-7 > lastVibratoValue) || (vibratoValue+7 < lastVibratoValue))) { // Update the input if it has been updated recently and is different, or is significantly different from the last value
    vibratoDebouncer = 0; // Reset the debouncer
    lastVibratoValue = vibratoValue; // The last swellPedalValue is the current value for the next cycle test
    updateAnalog(0x4C, 4, vibratoValue); // Send the new value over MIDI
  }
  if (vibratoDebouncer < 2) {
    vibratoDebouncer += 1;
  }
  // Swell pedal
  swellPedalValue = (constrain(analogRead(SWELL_PEDAL), 10, 895));
  swellPedalValue = fscale(10, 895, 127, 0, swellPedalValue, 3.5); // Applying a scale curve
  if (((swellPedalDebouncer < 2) && swellPedalValue != lastSwellPedalValue) || ((swellPedalValue-7 > lastSwellPedalValue) || (swellPedalValue+7 < lastSwellPedalValue))) { // Update the input if it has been updated recently and is different, or is significantly different from the last value
    swellPedalDebouncer = 0; // Reset the debouncer
    lastSwellPedalValue = swellPedalValue; // The last swellPedalValue is the current value for the next cycle test
    updateAnalog(0x0B, 4, swellPedalValue); // Send the new value over MIDI
  }
  if (swellPedalDebouncer < 2) {
    swellPedalDebouncer += 1;
  }

  digitalWrite(LATCH, LOW); // Pulse the parallel load latch
  digitalWrite(LATCH, HIGH);

  switchBank1 = SPI.transfer(0);
  switchBank2 = SPI.transfer(0);
  switchBank3 = SPI.transfer(0);
  switchBank4 = SPI.transfer(0);
  switchBank5 = SPI.transfer(0);
  switchBank6 = SPI.transfer(0);
  switchBank7 = SPI.transfer(0);
  switchBank8 = SPI.transfer(0);
  switchBank9 = SPI.transfer(0);
  switchBank10 = SPI.transfer(0);
  switchBank11 = SPI.transfer(0);
  switchBank12 = SPI.transfer(0);
  switchBank13 = SPI.transfer(0);
  switchBank14 = SPI.transfer(0);
  switchBank15 = SPI.transfer(0);
  switchBank16 = SPI.transfer(0);
  switchBank17 = SPI.transfer(0);
  switchBank18 = SPI.transfer(0);
  switchBank19 = SPI.transfer(0);
  switchBank20 = SPI.transfer(0);
  switchBank21 = SPI.transfer(0);

  byte mask1 = 1;
  for(int i = 1; i <= 8; i++) {
    if((switchBank1 & mask1) != (oldSwitchBank1 & mask1)) {
      Serial.print("Switch ");
      Serial.print(i);
      Serial.print(" now ");
      Serial.println((switchBank1 & mask1) ? "closed" : "open");
      (switchBank1 & mask1) ? noteOff(LM_Keys[i-1].note, LM_Keys[i-1].channel) : noteOn(LM_Keys[i-1].note, LM_Keys[i-1].channel);
    }  // end of bit has changed
    mask1 <<= 1;
  }  // end of for each bit
  byte mask2 = 1;
  for(int i = 9; i <= 16; i++) {
    if((switchBank2 & mask2) != (oldSwitchBank2 & mask2)) {
      Serial.print("Switch ");
      Serial.print(i);
      Serial.print(" now ");
      Serial.println((switchBank2 & mask2) ? "closed" : "open");
      (switchBank2 & mask2) ? noteOff(LM_Keys[i-1].note, LM_Keys[i-1].channel) : noteOn(LM_Keys[i-1].note, LM_Keys[i-1].channel);
    }  // end of bit has changed
    mask2 <<= 1;
  }  // end of for each bit
  byte mask3 = 1;
  for(int i = 17; i <= 24; i++) {
    if((switchBank3 & mask3) != (oldSwitchBank3 & mask3)) {
      Serial.print("Switch ");
      Serial.print(i);
      Serial.print(" now ");
      Serial.println((switchBank3 & mask3) ? "closed" : "open");
      (switchBank3 & mask3) ? noteOff(LM_Keys[i-1].note, LM_Keys[i-1].channel) : noteOn(LM_Keys[i-1].note, LM_Keys[i-1].channel);
    }  // end of bit has changed
    mask3 <<= 1;
  }  // end of for each bit
  byte mask4 = 1;
  for(int i = 25; i <= 32; i++) {
    if((switchBank4 & mask4) != (oldSwitchBank4 & mask4)) {
      Serial.print("Switch ");
      Serial.print(i);
      Serial.print(" now ");
      Serial.println((switchBank4 & mask4) ? "closed" : "open");
      (switchBank4 & mask4) ? noteOff(LM_Keys[i-1].note, LM_Keys[i-1].channel) : noteOn(LM_Keys[i-1].note, LM_Keys[i-1].channel);
    }  // end of bit has changed
    mask4 <<= 1;
  }  // end of for each bit
  byte mask5 = 1;
  for(int i = 33; i <= 40; i++) {
    if((switchBank5 & mask5) != (oldSwitchBank5 & mask5)) {
      Serial.print("Switch ");
      Serial.print(i);
      Serial.print(" now ");
      Serial.println((switchBank5 & mask5) ? "closed" : "open");
      (switchBank5 & mask5) ? noteOff(LM_Keys[i-1].note, LM_Keys[i-1].channel) : noteOn(LM_Keys[i-1].note, LM_Keys[i-1].channel);
    }  // end of bit has changed
    mask5 <<= 1;
  }  // end of for each bit
  byte mask6 = 1;
  for(int i = 41; i <= 48; i++) {
    if((switchBank6 & mask6) != (oldSwitchBank6 & mask6)) {
      Serial.print("Switch ");
      Serial.print(i);
      Serial.print(" now ");
      Serial.println((switchBank6 & mask6) ? "closed" : "open");
      if (i <= 44) {
        (switchBank6 & mask6) ? noteOff(LM_Keys[i-1].note, LM_Keys[i-1].channel) : noteOn(LM_Keys[i-1].note, LM_Keys[i-1].channel);
      } else {
        (switchBank6 & mask6) ? noteOff(UM_Keys[i-1-44].note, UM_Keys[i-1-44].channel) : noteOn(UM_Keys[i-1-44].note, UM_Keys[i-1-44].channel);

      }
    }  // end of bit has changed
    mask6 <<= 1;
  }  // end of for each bit
  byte mask7 = 1;
  for(int i = 49; i <= 56; i++) {
    if((switchBank7 & mask7) != (oldSwitchBank7 & mask7)) {
      Serial.print("Switch ");
      Serial.print(i);
      Serial.print(" now ");
      Serial.println((switchBank7 & mask7) ? "closed" : "open");
      (switchBank7 & mask7) ? noteOff(UM_Keys[i-1-44].note, UM_Keys[i-1-44].channel) : noteOn(UM_Keys[i-1-44].note, UM_Keys[i-1-44].channel);
    }  // end of bit has changed
    mask7 <<= 1;
  }  // end of for each bit
  byte mask8 = 1;
  for(int i = 57; i <= 64; i++) {
    if((switchBank8 & mask8) != (oldSwitchBank8 & mask8)) {
      Serial.print("Switch ");
      Serial.print(i);
      Serial.print(" now ");
      Serial.println((switchBank8 & mask8) ? "closed" : "open");
      (switchBank8 & mask8) ? noteOff(UM_Keys[i-1-44].note, UM_Keys[i-1-44].channel) : noteOn(UM_Keys[i-1-44].note, UM_Keys[i-1-44].channel);
    }  // end of bit has changed
    mask8 <<= 1;
  }  // end of for each bit
  byte mask9 = 1;
  for(int i = 65; i <= 72; i++) {
    if((switchBank9 & mask9) != (oldSwitchBank9 & mask9)) {
      Serial.print("Switch ");
      Serial.print(i);
      Serial.print(" now ");
      Serial.println((switchBank9 & mask9) ? "closed" : "open");
      (switchBank9 & mask9) ? noteOff(UM_Keys[i-1-44].note, UM_Keys[i-1-44].channel) : noteOn(UM_Keys[i-1-44].note, UM_Keys[i-1-44].channel);
    }  // end of bit has changed
    mask9 <<= 1;
  }  // end of for each bit
  byte mask10 = 1;
  for(int i = 73; i <= 80; i++) {
    if((switchBank10 & mask10) != (oldSwitchBank10 & mask10)) {
      Serial.print("Switch ");
      Serial.print(i);
      Serial.print(" now ");
      Serial.println((switchBank10 & mask10) ? "closed" : "open");
      (switchBank10 & mask10) ? noteOff(UM_Keys[i-1-44].note, UM_Keys[i-1-44].channel) : noteOn(UM_Keys[i-1-44].note, UM_Keys[i-1-44].channel);
    }  // end of bit has changed
    mask10 <<= 1;
  }  // end of for each bit
  byte mask11 = 1;
  for(int i = 81; i <= 88; i++) {
    if((switchBank11 & mask11) != (oldSwitchBank11 & mask11)) {
      Serial.print("Switch ");
      Serial.print(i);
      Serial.print(" now ");
      Serial.println((switchBank11 & mask11) ? "closed" : "open");
      (switchBank11 & mask11) ? noteOff(UM_Keys[i-1-44].note, UM_Keys[i-1-44].channel) : noteOn(UM_Keys[i-1-44].note, UM_Keys[i-1-44].channel);
    }  // end of bit has changed
    mask11 <<= 1;
  }  // end of for each bit
  byte mask12 = 1;
  for(int i = 89; i <= 96; i++) {
    if((switchBank12 & mask12) != (oldSwitchBank12 & mask12)) {
      Serial.print("Switch ");
      Serial.print(i);
      Serial.print(" now ");
      Serial.println((switchBank12 & mask12) ? "closed" : "open");
      (switchBank12 & mask12) ? updateDigital(switches[i-1-88].control, switches[i-1-88].channel, false) : updateDigital(switches[i-1-88].control, switches[i-1-88].channel, true);
    }  // end of bit has changed
    mask12 <<= 1;
  }  // end of for each bit
  byte mask13 = 1;
  for(int i = 97; i <= 104; i++) {
    if((switchBank13 & mask13) != (oldSwitchBank13 & mask13)) {
      Serial.print("Switch ");
      Serial.print(i);
      Serial.print(" now ");
      Serial.println((switchBank13 & mask13) ? "closed" : "open");
      (switchBank13 & mask13) ? updateDigital(switches[i-1-88].control, switches[i-1-88].channel, false) : updateDigital(switches[i-1-88].control, switches[i-1-88].channel, true);
    }  // end of bit has changed
    mask13 <<= 1;
  }  // end of for each bit
  byte mask14 = 1;
  for(int i = 105; i <= 112; i++) {
    if((switchBank14 & mask14) != (oldSwitchBank14 & mask14)) {
      Serial.print("Switch ");
      Serial.print(i);
      Serial.print(" now ");
      Serial.println((switchBank14 & mask14) ? "closed" : "open");
      (switchBank14 & mask14) ? updateDigital(switches[i-1-88].control, switches[i-1-88].channel, false) : updateDigital(switches[i-1-88].control, switches[i-1-88].channel, true);
    }  // end of bit has changed
    mask14 <<= 1;
  }  // end of for each bit
  byte mask15 = 1;
  for(int i = 113; i <= 120; i++) {
    if((switchBank15 & mask15) != (oldSwitchBank15 & mask15)) {
      Serial.print("Switch ");
      Serial.print(i);
      Serial.print(" now ");
      Serial.println((switchBank15 & mask15) ? "closed" : "open");
      (switchBank15 & mask15) ? updateDigital(switches[i-1-88].control, switches[i-1-88].channel, false) : updateDigital(switches[i-1-88].control, switches[i-1-88].channel, true);
    }  // end of bit has changed
    mask15 <<= 1;
  }  // end of for each bit
  byte mask16 = 1;
  for(int i = 121; i <= 128; i++) {
    if((switchBank16 & mask16) != (oldSwitchBank16 & mask16)) {
      Serial.print("Switch ");
      Serial.print(i);
      Serial.print(" now ");
      Serial.println((switchBank16 & mask16) ? "closed" : "open");
      (switchBank16 & mask16) ? updateDigital(switches[i-1-88].control, switches[i-1-88].channel, false) : updateDigital(switches[i-1-88].control, switches[i-1-88].channel, true);
    }  // end of bit has changed
    mask16 <<= 1;
  }  // end of for each bit
  byte mask17 = 1;
  for(int i = 129; i <= 136; i++) {
    if((switchBank17 & mask17) != (oldSwitchBank17 & mask17)) {
      Serial.print("Switch ");
      Serial.print(i);
      Serial.print(" now ");
      Serial.println((switchBank17 & mask17) ? "closed" : "open");
      (switchBank17 & mask17) ? updateDigital(switches[i-1-88].control, switches[i-1-88].channel, false) : updateDigital(switches[i-1-88].control, switches[i-1-88].channel, true);
    }  // end of bit has changed
    mask17 <<= 1;
  }  // end of for each bit
  byte mask18 = 1;
  for(int i = 137; i <= 144; i++) {
    if((switchBank18 & mask18) != (oldSwitchBank18 & mask18)) {
      Serial.print("Switch ");
      Serial.print(i);
      Serial.print(" now ");
      Serial.println((switchBank18 & mask18) ? "closed" : "open");
      (switchBank18 & mask18) ? updateDigital(switches[i-1-88].control, switches[i-1-88].channel, false) : updateDigital(switches[i-1-88].control, switches[i-1-88].channel, true);
    }  // end of bit has changed
    mask18 <<= 1;
  }  // end of for each bit
  byte mask19 = 1;
  for(int i = 145; i <= 152; i++) {
    if((switchBank19 & mask19) != (oldSwitchBank19 & mask19)) {
      Serial.print("Switch ");
      Serial.print(i);
      Serial.print(" now ");
      Serial.println((switchBank19 & mask19) ? "closed" : "open");
      (switchBank19 & mask19) ? digitalWrite(LED, LOW) : digitalWrite(LED, HIGH);
      if (i<=151) {
        (switchBank19 & mask19) ? updateDigital(switches[i-1-88].control, switches[i-1-88].channel, false) : updateDigital(switches[i-1-88].control, switches[i-1-88].channel, true);
      } else {
        (switchBank19 & mask19) ? noteOff(PB_Keys[i-1-151].note, PB_Keys[i-1-151].channel) : noteOn(PB_Keys[i-1-151].note, PB_Keys[i-1-151].channel);
      }
    }  // end of bit has changed
    mask19 <<= 1;
  }  // end of for each bit
  byte mask20 = 1;
  for(int i = 153; i <= 160; i++) {
    if((switchBank20 & mask20) != (oldSwitchBank20 & mask20)) {
      Serial.print("Switch ");
      Serial.print(i);
      Serial.print(" now ");
      Serial.println((switchBank20 & mask20) ? "closed" : "open");
      (switchBank20 & mask20) ? digitalWrite(LED, LOW) : digitalWrite(LED, HIGH);
      (switchBank20 & mask20) ? noteOff(PB_Keys[i-1-151].note, PB_Keys[i-1-151].channel) : noteOn(PB_Keys[i-1-151].note, PB_Keys[i-1-151].channel);
    }  // end of bit has changed
    mask20 <<= 1;
  }  // end of for each bit
  byte mask21 = 1;
  for(int i = 161; i <= 165; i++) {
    if((switchBank21 & mask21) != (oldSwitchBank21 & mask21)) {
      Serial.print("Switch ");
      Serial.print(i);
      Serial.print(" now ");
      Serial.println((switchBank21 & mask21) ? "closed" : "open");
      (switchBank21 & mask21) ? digitalWrite(LED, LOW) : digitalWrite(LED, HIGH);
      if (i<=164) {
        (switchBank21 & mask21) ? noteOff(PB_Keys[i-1-151].note, PB_Keys[i-1-151].channel) : noteOn(PB_Keys[i-1-151].note, PB_Keys[i-1-151].channel);
      } else {
        (switchBank21 & mask21) ? updateDigital(switches[63].control, switches[63].channel, false) : updateDigital(switches[63].control, switches[63].channel, true);
      }
    }  // end of bit has changed
    mask21 <<= 1;
  }  // end of for each bit
  oldSwitchBank1 = switchBank1;
  oldSwitchBank2 = switchBank2;
  oldSwitchBank3 = switchBank3;
  oldSwitchBank4 = switchBank4;
  oldSwitchBank5 = switchBank5;
  oldSwitchBank6 = switchBank6;
  oldSwitchBank7 = switchBank7;
  oldSwitchBank8 = switchBank8;
  oldSwitchBank9 = switchBank9;
  oldSwitchBank10 = switchBank10;
  oldSwitchBank11 = switchBank11;
  oldSwitchBank12 = switchBank12;
  oldSwitchBank13 = switchBank13;
  oldSwitchBank14 = switchBank14;
  oldSwitchBank15 = switchBank15;
  oldSwitchBank16 = switchBank16;
  oldSwitchBank17 = switchBank17;
  oldSwitchBank18 = switchBank18;
  oldSwitchBank19 = switchBank19;
  oldSwitchBank20 = switchBank20;
  oldSwitchBank21 = switchBank21;
  delay (50);   // debounce
}  // end of loop

// Plays a MIDI note
void noteOn(int pitch, int channel) {
  digitalWrite(LED, HIGH);
  int command;
  if (channel == 1) {
    command = 0x90;
  } else if (channel == 2) {
    command = 0x91;
  } else if (channel == 3) {
    command = 0x92;
  }
  int velocity = 0x64;
  Serial1.write(command);
  Serial1.write(pitch);
  Serial1.write(velocity);
  Serial.print("Sending NoteOn ");
  Serial.print(pitch);
  Serial.print(" with command ");
  Serial.print(command);
  Serial.print(" and velocity ");
  Serial.println(velocity);
}

// Stops a MIDI note
void noteOff(int pitch, int channel) {
  digitalWrite(LED, LOW);
  int command;
  if (channel == 1) {
    command = 0x80;
  } else if (channel == 2) {
    command = 0x81;
  } else if (channel == 3) {
    command = 0x82;
  }
  int velocity = 0x64;
  Serial1.write(command);
  Serial1.write(pitch);
  Serial1.write(velocity);
  Serial.print("Sending NoteOff ");
  Serial.print(pitch);
  Serial.print(" with command ");
  Serial.print(command);
  Serial.print(" and velocity ");
  Serial.println(velocity);
}

// Updates an analog control
void updateAnalog(int control, int channel, int value) {
  int command = 0xB0;
  if (channel == 2) {
    command = 0xB1;
  } else if (channel == 3) {
    command = 0xB2;
  } else if (channel == 4) {
    command = 0xB3;
  } else if (channel == 5) {
    command = 0xB4;
  }
  Serial1.write(command);
  Serial1.write(control);
  Serial1.write(value);
  Serial.print("Updating analog ");
  Serial.print(control);
  Serial.print(" on channel ");
  Serial.print(channel);
  Serial.print(" with value ");
  Serial.println(value);
}

// Updates an digital control
void updateDigital(int control, int channel, boolean value) {
  int command = 0xB0;
  if (channel == 2) {
    command = 0xB1;
  } else if (channel == 3) {
    command = 0xB2;
  } else if (channel == 4) {
    command = 0xB3;
  } else if (channel == 5) {
    command = 0xB4;
  }
  int MIDIValue = 0;
  if (value) {
    MIDIValue = 127;
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
  }
  Serial1.write(command);
  Serial1.write(control);
  Serial1.write(MIDIValue);
  Serial.print("Updating digital control ");
  Serial.print(control);
  Serial.print(" on channel ");
  Serial.print(channel);
  Serial.print(" with value ");
  Serial.println(MIDIValue);
}

// Scales an input accordingly to a curve
float fscale(float originalMin, float originalMax, float newBegin, float newEnd, float inputValue, float curve) {

  float OriginalRange = 0;
  float NewRange = 0;
  float zeroRefCurVal = 0;
  float normalizedCurVal = 0;
  float rangedValue = 0;
  boolean invFlag = 0;

  // condition curve parameter
  // limit range

  if (curve > 10) curve = 10;
  if (curve < -10) curve = -10;

  curve = (curve * -.1) ; // - invert and scale - this seems more intuitive - postive numbers give more weight to high end on output
  curve = pow(10, curve); // convert linear scale into lograthimic exponent for other pow function

  // Check for out of range inputValues
  if (inputValue < originalMin) {
    inputValue = originalMin;
  }
  if (inputValue > originalMax) {
    inputValue = originalMax;
  }

  // Zero Refference the values
  OriginalRange = originalMax - originalMin;

  if (newEnd > newBegin) {
    NewRange = newEnd - newBegin;
  } else {
    NewRange = newBegin - newEnd;
    invFlag = 1;
  }

  zeroRefCurVal = inputValue - originalMin;
  normalizedCurVal  =  zeroRefCurVal / OriginalRange;   // normalize to 0 - 1 float

  // Check for originalMin > originalMax  - the math for all other cases i.e. negative numbers seems to work out fine
  if (originalMin > originalMax ) {
    return 0;
  }

  if (invFlag == 0) {
    rangedValue =  (pow(normalizedCurVal, curve) * NewRange) + newBegin;
  } else {// invert the ranges
    rangedValue =  newBegin - (pow(normalizedCurVal, curve) * NewRange);
  }

  return rangedValue;
}
