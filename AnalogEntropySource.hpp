#ifndef ANALOG_ENTROPY_SOURCE_HPP
#define ANALOG_ENTROPY_SOURCE_HPP

#include <Arduino.h>
#include <stdint.h>
#include "./EntropySource.hpp"

// Spacing for noise reads (must be a power of 2)
#define NOISE_DT 64

class AnalogEntropySource : public EntropySource {
  private:
    uint8_t entropyPin;
  public:
    AnalogEntropySource(uint8_t pin);
    uint16_t operator()(uint16_t state);
};

#endif // ANALOG_ENTROPY_SOURCE_HPP