#include "./AnalogEntropySource.hpp"

AnalogEntropySource::AnalogEntropySource(uint8_t pin) {
  entropyPin = pin;
}

uint16_t AnalogEntropySource::operator()(uint16_t state) {
  // For DT == 64, this is 32 + the lower 5 bits of state
  // Point is to randomize the read interval, so a radio signal
  // can't be tuned consistently to interfere in a predictable manner.
  uint16_t dt = (DT >> 1) + (state & ((DT >> 1) - 1));
  // Read and XOR the low bits of a pair of analogReads, spaced apart by DT.
  uint16_t ent = analogRead(entropyPin) & 1;
  delay(dt);
  ent = ent ^ (analogRead(entropyPin) & 1);
  delay(dt);
  return ent;
}
