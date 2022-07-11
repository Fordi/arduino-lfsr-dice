#ifndef DICE_HPP
#define DICE_HPP

#include <stdint.h>
#include "./taps.h"

using namespace std;

class Dice {
  private:
    // Can be any non-zero state.
    uint16_t state = 0xACE1u;
    uint16_t entropyPin;
    // Select a tap based on how long the µC has been up
    uint16_t tapNum = micros() % TAP_COUNT;
  public:
    Dice(uint8_t pin);
    void roll(uint8_t count, uint8_t size, uint8_t* result);
};

#endif
