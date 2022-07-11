#ifndef DICE_HPP
#define DICE_HPP

#include <stdint.h>
#include "./taps.h"

using namespace std;

class EntropySource {
  public:
    virtual uint16_t operator()(uint16_t state) = 0;
};

class Dice {
  private:
    // Can be any non-zero state.
    uint16_t state = 0xACE1u;
    EntropySource* entropySource = nullptr;
    uint16_t tapNum = 0;
  public:
    Dice();
    Dice(EntropySource* entSource);
    void roll(uint16_t count, uint8_t size, uint8_t* result);
};

#endif
