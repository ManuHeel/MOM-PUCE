#include <tables/sin512_int8.h> // sine table
#include <tables/saw512_int8.h> // saw table
#include <tables/square_no_alias512_int8.h> // square table
#include <tables/triangle512_int8.h> // triangle table
#include "waveform.h" // Waveforms enumeration
#include "pitches.h" // Personal pitch system

class Instrument {

  // Attributes
  int currentNotes[3];
  bool isPolyphonic;
  Oscil <512, AUDIO_RATE> oscil1;
  Oscil <512, AUDIO_RATE> oscil2;
  Oscil <512, AUDIO_RATE> oscil3;

  // Functions
  Instrument(Waveform waveform);
    
};

