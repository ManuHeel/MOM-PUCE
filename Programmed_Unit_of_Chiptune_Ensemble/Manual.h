#include "Instrument.h"
#include "waveform.h"

class Manual {

  // Attributes
  int pressedKeys[6];
  bool isPolyphonic;
  Instrument instrument;

  // Functions
  Manual();
  void setInstrument(Waveform waveform);
  void addKey(int key);
  void removeKey(int key):
    
};

