#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

uint16_t parity(uint16_t n) {
  uint16_t result = 0;
  while (n != 0) {
    result = result ^ (n & 1);
    n = n >> 1;
  }
  return result;
};

uint16_t lfsr_fib(uint16_t tap) {
  uint16_t start_state = 0xACE1u;
  uint16_t lfsr = start_state;
  uint16_t last = lfsr;
  uint16_t bit;
  uint32_t period = 0;

  do {
    bit = parity(lfsr & tap);
    lfsr = (lfsr >> 1) | (bit << 15);
    if (last == lfsr) return 0;
    last = lfsr;
    ++period;
  }
  while (lfsr != start_state && period <= 65535);
  return period;
};

uint16_t taps[65535] = {0};

int main(int argc, char** argv) {
  uint16_t maxTaps = 0;
  if (argc == 2) {
    maxTaps = atoi(argv[1]) & 0xFFFF;
  }
  uint8_t first = 1;
  uint16_t next = 0;
  for (uint32_t i = 1; i <= 0x7FFF; i+=1) {
    uint16_t pd = lfsr_fib(i);
    if (pd == 65535) {
      taps[next++] = i;
    }
    if (maxTaps != 0 && next >= maxTaps) break;
  }
  // Randomize the taps
  srand(time(NULL));
  for (int i = next - 1; i > 0; i--) {
    int j = rand() % (i + 1);
    uint16_t tmp = taps[i];
    taps[i] = taps[j];
    taps[j] = tmp;
  }
  fprintf(stderr, "#ifndef TAPS_H\n#define TAPS_H\n\n");
  fprintf(stderr, "#include <stdint.h>\n\n");
  fprintf(stderr, "#ifdef ARDUINO\n  #include <Arduino.h>\n  #define tap_t const PROGMEM uint16_t\n#else\n  #define tap_t const uint16_t\n#endif\n\n");
  fprintf(stderr, "#define TAP_COUNT %d\n\n", next);
  fprintf(stderr, "tap_t taps[TAP_COUNT] = {\n  ");
  for (uint16_t i = 0; i < next; i++) {
    if (first == 1) {
      first = 0;
    } else {
      if ((i % 10) == 0) {
        fprintf(stderr, ",\n  ");
      } else {
        fprintf(stderr, ", ");
      }
    }
    fprintf(stderr, "0x%04x", taps[i]);
  }
  fprintf(stderr, "\n};\n\n#endif\n");
  return 0;
};
