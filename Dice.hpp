#ifndef DICE_HPP
#define DICE_HPP

#include <stdint.h>
#include "./EntropySource.hpp"
#include "./taps.h"

using namespace std;

#define INIT_ROLLS 10

class Dice {
  private:
    // Can be any non-zero state.  Using the tap set itself to pick the init state.
    uint16_t state = taps[taps[0] % TAP_COUNT];
    EntropySource* entropySource = nullptr;
    uint16_t tapNum = 0;
    void init();
  public:
    Dice();
    Dice(EntropySource* entSource);
    void roll(uint16_t count, uint8_t size, uint8_t* result);
};

#endif // DICE_HPP
