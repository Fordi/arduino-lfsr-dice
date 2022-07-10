// See https://www.arduino.cc/reference/en/libraries/ezbutton/
#include <ezButton.h>
#include "./Dice.hpp"

ezButton rollBtn(7);
Dice dice(A3);

void setup() {
  dice.init();
  // Other init
  Serial.begin(9600);
  rollBtn.setDebounceTime(64);
}

void loop() {
  rollBtn.loop();
  if (rollBtn.isPressed()) {
    uint8_t dice[10];
    // Roll 4d6
    roll(4, 6, &dice);
    uint8_t sum = 0;
    for (int i = 0; i < 4; i++) {
      if (i !== 0) {
        Serial.print(", ");
      }
      sum += dice[i];
      Serial.print(dice[i]);
    }
    Serial.println(sum);
  }
}