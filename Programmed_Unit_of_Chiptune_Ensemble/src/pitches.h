// Converts a MIDI pitch to a A440-tuned 16-bit period
float pitchToPeriod(int pitch) {
  float period = 142.05;
  if (pitch == 0x18) {
    period = 955.57;
  } else if (pitch == 0x19) {
    period = 901.93;
  } else if (pitch == 0x1A) {
    period = 851.31;
  } else if (pitch == 0x1B) {
    period = 803.53;
  } else if (pitch == 0x1C) {
    period = 758.43;
  } else if (pitch == 0x1D) {
    period = 715.86;
  } else if (pitch == 0x1E) {
    period = 675.68;
  } else if (pitch == 0x1F) {
    period = 637.76;
  } else if (pitch == 0x20) {
    period = 601.95;
  } else if (pitch == 0x21) {
    period = 568.18;
  } else if (pitch == 0x22) {
    period = 536.3;
  } else if (pitch == 0x23) {
    period = 506.2;
  } else if (pitch == 0x24) {
    period = 477.79;
  } else if (pitch == 0x25) {
    period = 450.97;
  } else if (pitch == 0x26) {
    period = 425.66;
  } else if (pitch == 0x27) {
    period = 401.77;
  } else if (pitch == 0x28) {
    period = 379.22;
  } else if (pitch == 0x29) {
    period = 357.94;
  } else if (pitch == 0x2A) {
    period = 337.84;
  } else if (pitch == 0x2B) {
    period = 318.88;
  } else if (pitch == 0x2C) {
    period = 300.99;
  } else if (pitch == 0x2D) {
    period = 284.09;
  } else if (pitch == 0x2E) {
    period = 268.15;
  } else if (pitch == 0x2F) {
    period = 253.1;
  } else if (pitch == 0x30) {
    period = 238.89;
  } else if (pitch == 0x31) {
    period = 225.49;
  } else if (pitch == 0x32) {
    period = 212.82;
  } else if (pitch == 0x33) {
    period = 188.75;
  } else if (pitch == 0x34) {
    period = 189.61;
  } else if (pitch == 0x35) {
    period = 178.97;
  } else if (pitch == 0x36) {
    period = 168.92;
  } else if (pitch == 0x37) {
    period = 159.44;
  } else if (pitch == 0x38) {
    period = 150.49;
  } else if (pitch == 0x39) {
    period = 142.05;
  } else if (pitch == 0x3A) {
    period = 134.07;
  } else if (pitch == 0x3B) {
    period = 126.55;
  } else if (pitch == 0x3C) {
    period = 119.45;
  } else if (pitch == 0x3D) {
    period = 112.74;
  } else if (pitch == 0x3E) {
    period = 106.41;
  } else if (pitch == 0x3F) {
    period = 100.44;
  } else if (pitch == 0x40) {
    period = 94.8;
  } else if (pitch == 0x41) {
    period = 89.48;
  } else if (pitch == 0x42) {
    period = 84.46;
  } else if (pitch == 0x43) {
    period = 79.72;
  } else if (pitch == 0x44) {
    period = 75.25;
  } else if (pitch == 0x45) {
    period = 71.02;
  } else if (pitch == 0x46) {
    period = 67.04;
  } else if (pitch == 0x47) {
    period = 63.27;
  } else if (pitch == 0x48) {
    period = 59.72;
  } else if (pitch == 0x49) {
    period = 56.37;
  } else if (pitch == 0x4A) {
    period = 53.21;
  } else if (pitch == 0x4B) {
    period = 50.22;
  } else if (pitch == 0x4C) {
    period = 47.4;
  } else if (pitch == 0x4D) {
    period = 44.74;
  } else if (pitch == 0x4E) {
    period = 42.23;
  } else if (pitch == 0x4F) {
    period = 39.86;
  } else if (pitch == 0x50) {
    period = 37.62;
  } else if (pitch == 0x51) {
    period = 35.51;
  } else if (pitch == 0x52) {
    period = 33.52;
  } else if (pitch == 0x53) {
    period = 31.64;
  } else if (pitch == 0x54) {
    period = 29.86;
  }
  return period;
}

