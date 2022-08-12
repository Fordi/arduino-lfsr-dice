#ifndef ENTROPY_SOURCE_HPP
#define ENTROPY_SOURCE_HPP

#include <stdint.h>

// Defines the interface for getting entropy from the spigot.  Basically, you call it like a
// function with the last state, which, in `AnalogEntropySource`, is used to vary the interval
// at which the analog signal is polled. 
class EntropySource {
  public:
    virtual uint16_t operator()(uint16_t state) = 0;
};

#endif // ENTROPY_SOURCE_HPP
