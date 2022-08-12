// See https://www.arduino.cc/reference/en/libraries/ezbutton/
#include <ezButton.h>
#include "./Dice.hpp"
#include "./AnalogEntropySource.hpp"

ezButton rollBtn(7);

// Spacing for debounce time
#define DT 64

// This is where you pick the sacrificial analog pin for extra entropy
// Comment out to not use extra entropy
#define ENTROPY_PIN A3

#ifdef ENTROPY_PIN
  AnalogEntropySource source(A3);
  Dice dice(&source);
#else
  Dice dice();
#endif

void setup() {
  // Other init
  Serial.begin(9600);
  rollBtn.setDebounceTime(64);
}

void loop() {
  rollBtn.loop();
  if (rollBtn.isPressed()) {
    uint8_t rolls[10];
    // Roll 4d6
    dice.roll(4, 6, &rolls[0]);
    uint8_t sum = 0;
    for (int i = 0; i < 4; i++) {
      if (i != 0) {
        Serial.print(", ");
      }
      sum += rolls[i];
      Serial.print(rolls[i]);
    }
    Serial.println(sum);
  }
}
