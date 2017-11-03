# MOM-PUCE
Midi Organ Module &amp; Programmed Unit of Chiptune Ensemble.
Project of an organ hacked into a chiptune music workstation/synthesizer.

Made up of two sub-projects:
- MOM: MIDI management and I/O
- PUCE: Sound generation (digital synth).

MOM is integrated to the organ electronics through an Arduino Mega.
PUCE is accessed by MIDI and outputs to a Jack socket through the DAC of the board (Teensy 3.2 with native C code).
