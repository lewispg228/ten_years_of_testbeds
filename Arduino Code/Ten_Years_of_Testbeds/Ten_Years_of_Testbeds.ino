#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>

// The LuMini rings need two data pins connected
#define CLOCK_PIN1 13
#define DATA_PIN1 12
#define NUM_LEDS 349

// Define the array of leds
CRGB leds[NUM_LEDS];
uint8_t matrixWidth = 136;
uint8_t matrixHeight = 38;

const byte ledPin = 13;
const byte interruptPin = 2;
volatile byte state = LOW;
boolean buttonArmed = false; // used for debounce, requiring 100ms release of button
int releaseTime = 0;
long timeLastPressed = 0; // keep track of the last button press, for debouncing
byte mode = 1;
#define NUM_MODES 2
boolean modeResetRequired = false; // used to know if we've changed modes, and to reset variables

void setup() {
  randomSeed(1);
  LEDS.addLeds<APA102, DATA_PIN1, CLOCK_PIN1, RGB>(leds, NUM_LEDS);
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), buttonLow, FALLING);
  SerialUSB.begin(57600);
  delay(1000);
  SerialUSB.println("resetting");
}

void loop()
{
  switch (mode)
  {
    case 1:
      pete();
      SerialUSB.println("Pete");
      break;
    case 2:
      sparkleRandom();
      SerialUSB.println("sparkleRandom");
      break;
    default:
      break;
  }

  // Button debounce. Note, I kept this out of buttonLow(), to keep the ISR zippy
  // check if button is released (high)
  // count how long, and then arm for next press
  // this requires the user to release the button for it to be armed
  if (buttonArmed == false) // only check if it's NOT armed
  {
    if (digitalRead(interruptPin) == HIGH) releaseTime++;
    delay(1);
    if (releaseTime > 100) buttonArmed = true; // valid high time, let's arm!
  }

  if(modeResetRequired == true)
  {
    sparkleRandom_reset();
    pete_reset();
    modeResetRequired = false; // we just called all the reset functions, so we don'e need to again (until a new mode change)
  }
}

uint16_t xy (uint8_t x, uint8_t y)
{
  uint16_t ledNum = 350;
  if (y == 0)
  {
    ledNum = (matrixWidth + 1) - x;
  }
  else if (y == matrixHeight)
  {
    ledNum = matrixWidth + matrixHeight + x;
  }
  else if (x == 0)
  {
    ledNum = (matrixWidth + 1) + y;
  }
  else if (x == matrixWidth)
  {
    ledNum = (matrixWidth * 2) + matrixHeight + ((matrixHeight - 1) - y);
  }
  return ledNum;
}

void buttonLow()
{
  if (millis() > (timeLastPressed + 500)) // only increment MODE if we've seen a time interval since last press 
  {
    mode++;
    if (mode > NUM_MODES) mode = 1; // loop back to first mode
    modeResetRequired = true; // we just changed modes, so we need to reset variables
  }
  timeLastPressed = millis();
}
