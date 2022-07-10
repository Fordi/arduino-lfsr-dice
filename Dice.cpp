#include "./Dice.hpp"

void Dice::roll(uint8_t count, uint8_t size, uint8_t* result) {
  // Read and XOR the low bits of a pair of analogReads, spaced apart by DT.
  uint16_t ent = analogRead(entropyPin) & 1;
  delay(DT >> 1);
  ent = ent ^ (analogRead(entropyPin) & 1);
  delay(DT >> 1);
  for (uint8_t d = 0; d < count; d++) {
    uint16_t out = 0;
    for (uint8_t i = 0; i < 16; i++) {
      // Grab the LSFR taps
      bit = ((state >> 0) ^ (state >> 2) ^ (state >> 3) ^ (state >> 5)) & 1u;
      // Add to feedback, xor'd with read-in entropy bit
      lfsr = (state >> 1) | ((bit ^ ent) << 15);
      // pack bit onto next shift
      out = ((out << 1) | (state & 1));
    };
    // Store the result
    result[d] = (out % size) + 1;
  }
}

Dice::Dice(uint8_t pin) {
  entropyPin = pin;
  uint8_t dice[10];
  // Roll a fresh seed as part of construction.
  roll(10, 4, &dice);
  // Grab the ninth roll, and re-roll that many times to shuffle the initial seed.
  uint8_t result = dice[9];
  for (int i = 0; i < dice[9]; i++) {
    roll(10, 100, &dice);
  }
  // Mean total init time should be (2.5 * DT) ms
}
