void rotate()
{
  static uint8_t hue = 0;
  // First slide the led in one direction
  for (int i = 0; i < NUM_LEDS; i++) {
    // Set the i'th led to red
    leds[i] = CHSV(hue++, 255, 255);
    // Show the leds
    FastLED.show();
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(5);
    if (modeResetRequired) break; // this is changes with an interrupt button
  }
}

void rotate_reset()
{
  for (int i = 0; i < NUM_LEDS; i++) {
    // Set the i'th led to 0
    leds[i] = CHSV(0, 0, 0);
  }
  // Show the leds
  FastLED.show();

#if DEBUG
  SerialUSB.println("rotate RESET");
#endif
}
