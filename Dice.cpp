#include <Arduino.h>
#include <stdint.h>

/*
  taps.h contains all the LFSR tap masks that have a period of 65535
    (which is the 16 bit maximum period)

  To reduce PROGMEM footprint, you can shorten the array.
  It's recommended to pick a prime number for this array, to avoid 
    syncing up any looped periods.  The footprint in PROGMEM will be
    (2 * TAP_COUNT) bytes.

  Primes to 1031:
      2      3      5      7     11     13     17     19     23     29 
     31     37     41     43     47     53     59     61     67     71 
     73     79     83     89     97    101    103    107    109    113 
    127    131    137    139    149    151    157    163    167    173 
    179    181    191    193    197    199    211    223    227    229 
    233    239    241    251    257    263    269    271    277    281 
    283    293    307    311    313    317    331    337    347    349 
    353    359    367    373    379    383    389    397    401    409 
    419    421    431    433    439    443    449    457    461    463 
    467    479    487    491    499    503    509    521    523    541 
    547    557    563    569    571    577    587    593    599    601 
    607    613    617    619    631    641    643    647    653    659 
    661    673    677    683    691    701    709    719    727    733 
    739    743    751    757    761    769    773    787    797    809 
    811    821    823    827    829    839    853    857    859    863 
    877    881    883    887    907    911    919    929    937    941 
    947    953    967    971    977    983    991    997   1009   1013 
   1019   1021   1031
 */

#include "./taps.h"

#include "./Dice.hpp"

using namespace std;

// Spacing for noise reads (must be a power of 2)
#define DT 64

uint8_t parity(uint16_t n) {
  uint8_t result = 0;
  while (n != 0) {
    result = result ^ (n & 1);
    n = n >> 1;
  }
  return result;
};

void Dice::roll(uint8_t count, uint8_t size, uint8_t* result) {
  // For DT == 64, this is 32 + the lower 5 bits of state
  // Point is to randomize the read interval, so a radio signal
  // can't be tuned consistently to interfere in a predictable manner.
  uint16_t dt = (DT >> 1) + (state & ((DT >> 1) - 1));
  // Read and XOR the low bits of a pair of analogReads, spaced apart by DT.
  uint16_t ent = analogRead(entropyPin) & 1;
  delay(dt);
  ent = ent ^ (analogRead(entropyPin) & 1);
  delay(dt);
  for (uint8_t d = 0; d < count; d++) {
    uint16_t out = 0;
    for (uint8_t i = 0; i < 16; i++) {
      // Tap the existing state, xor'd with read-in entropy bit
      uint8_t bit = parity(state & taps[tapNum]) ^ ent;
      // Add to feedback
      state = (state >> 1) | (bit << 15);
      // pack bit onto output and shift
      out = ((out << 1) | (state & 1));
      // 1-in-4 chance to move to the next tap, to avoid predictable sequencing
      if ((bit ^ ent) == 0 && (state & 1) == 1) {
        tapNum = (tapNum + 1) % TAP_COUNT;
      }
    };
    // Store the result
    result[d] = (out % size) + 1;
  }
}

Dice::Dice(uint8_t pin) {
  entropyPin = pin;
  uint8_t dice[10];
  // Roll a fresh seed as part of construction.
  roll(10, 8, &dice[0]);
  // Grab the ninth roll, and re-roll that many times to shuffle the initial seed.
  uint8_t result = dice[9];
  for (int i = 0; i < dice[9]; i++) {
    roll(10, 100, &dice[0]);
  }
  // Mean total init time should be (5 * DT) ms, or about 1/3 second.
}
