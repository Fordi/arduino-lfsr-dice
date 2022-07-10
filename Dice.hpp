#ifndef DICE_HPP
#define DICE_HPP

#include <stdint.h>
#include <Arduino.h>

// Spacing for noise reads
#define DT 32

class Dice {
  private:
    // Can be any non-zero state.
    uint16_t state = 0xACE1u;
    uint16_t entropyPin;
  public:
    Dice(uint8_t pin);
    void roll(uint8_t count, uint8_t size, uint8_t* result);
}

#endif