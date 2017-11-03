// Converts a MIDI pitch to a A440-tuned frequency
float pitchToFrequency(int pitch) {
  float frequency = 440.000;
  if (pitch == 0x18) {
    frequency = 65.406;
  } else if (pitch == 0x19) {
    frequency = 69.296;
  } else if (pitch == 0x1A) {
    frequency = 73.416;
  } else if (pitch == 0x1B) {
    frequency = 77.782;
  } else if (pitch == 0x1C) {
    frequency = 82.407;
  } else if (pitch == 0x1D) {
    frequency = 87.307;
  } else if (pitch == 0x1E) {
    frequency = 92.499;
  } else if (pitch == 0x1F) {
    frequency = 97.999;
  } else if (pitch == 0x20) {
    frequency = 103.83;
  } else if (pitch == 0x21) {
    frequency = 110.00;
  } else if (pitch == 0x22) {
    frequency = 116.54;
  } else if (pitch == 0x23) {
    frequency = 123.47;
  } else if (pitch == 0x24) {
    frequency = 130.81;
  } else if (pitch == 0x25) {
    frequency = 138.59;
  } else if (pitch == 0x26) {
    frequency = 146.83;
  } else if (pitch == 0x27) {
    frequency = 155.56;
  } else if (pitch == 0x28) {
    frequency = 164.81;
  } else if (pitch == 0x29) {
    frequency = 174.61;
  } else if (pitch == 0x2A) {
    frequency = 185.00;
  } else if (pitch == 0x2B) {
    frequency = 196.00;
  } else if (pitch == 0x2C) {
    frequency = 207.65;
  } else if (pitch == 0x2D) {
    frequency = 220.00;
  } else if (pitch == 0x2E) {
    frequency = 233.08;
  } else if (pitch == 0x2F) {
    frequency = 246.94;
  } else if (pitch == 0x30) {
    frequency = 261.63;
  } else if (pitch == 0x31) {
    frequency = 277.18;
  } else if (pitch == 0x32) {
    frequency = 293.67;
  } else if (pitch == 0x33) {
    frequency = 311.13;
  } else if (pitch == 0x34) {
    frequency = 329.63;
  } else if (pitch == 0x35) {
    frequency = 349.23;
  } else if (pitch == 0x36) {
    frequency = 369.99;
  } else if (pitch == 0x37) {
    frequency = 392.00;
  } else if (pitch == 0x38) {
    frequency = 415.30;
  } else if (pitch == 0x39) {
    frequency = 440.00;
  } else if (pitch == 0x3A) {
    frequency = 466.16;
  } else if (pitch == 0x3B) {
    frequency = 493.88;
  } else if (pitch == 0x3C) {
    frequency = 523.25;
  } else if (pitch == 0x3D) {
    frequency = 554.37;
  } else if (pitch == 0x3E) {
    frequency = 587.33;
  } else if (pitch == 0x3F) {
    frequency = 622.25;
  } else if (pitch == 0x40) {
    frequency = 659.26;
  } else if (pitch == 0x41) {
    frequency = 698.46;
  } else if (pitch == 0x42) {
    frequency = 739.99;
  } else if (pitch == 0x43) {
    frequency = 783.99;
  } else if (pitch == 0x44) {
    frequency = 830.61;
  } else if (pitch == 0x45) {
    frequency = 880.00;
  } else if (pitch == 0x46) {
    frequency = 932.33;
  } else if (pitch == 0x47) {
    frequency = 987.77;
  } else if (pitch == 0x48) {
    frequency = 1046.5;
  } else if (pitch == 0x49) {
    frequency = 1108.7;
  } else if (pitch == 0x4A) {
    frequency = 1174.7;
  } else if (pitch == 0x4B) {
    frequency = 1244.5;
  } else if (pitch == 0x4C) {
    frequency = 1318.5;
  } else if (pitch == 0x4D) {
    frequency = 1396.9;
  } else if (pitch == 0x4E) {
    frequency = 1480.0;
  } else if (pitch == 0x4F) {
    frequency = 1568.0;
  } else if (pitch == 0x50) {
    frequency = 1661.2;
  } else if (pitch == 0x51) {
    frequency = 1760.0;
  } else if (pitch == 0x52) {
    frequency = 1864.7;
  } else if (pitch == 0x53) {
    frequency = 1975.5;
  } else if (pitch == 0x54) {
    frequency = 2093.0;
  }
  return frequency;
}

