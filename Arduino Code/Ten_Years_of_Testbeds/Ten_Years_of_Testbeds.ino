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

uint8_t gradientPosition = 0;
uint8_t frameDelay = 0;
uint8_t randomDelay = 0;
uint8_t colorDelay = 0;

uint8_t randomHue = 0;
uint8_t randomSat = 255;
uint8_t randomVal = 255;

uint16_t rotation = 0;

uint8_t colorArray[256][3];//An array of random colors for a random gradient.

/*This function fades our entire array, this causes each random spark to slowly die out.*/
void fadeAll(uint8_t scale = 240) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].nscale8(scale);
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

void colorRandomize(uint8_t &gradientPosition, uint8_t &sat, uint8_t &val)  //This function picks a color with random saturation and value, but somewhere within the same range of hue as our previous color
{
  int gradientPositionOffset = random(127);
  sat = 100 + random(155);
  val = 127 + random(128);
  randomDelay = 25 + random(100); //Decreasing this number will increase the speed at which the gradient is changed.
  gradientPosition = gradientPosition + (gradientPositionOffset - 64);
}

void sparkleRandom ()
{
  for (int row = 0; row <= matrixHeight; row++)
  {
    for (int column = 0; column <= matrixWidth + 1; column++) //Loop through every LED in the array each frame, and decide whether or not to run it on.
    {
      uint8_t chance = 18; //The chance that any LED gets turned on during a given frame is 1/chance. Lower numbers mean more sparkles. Use in conjunction with fadeAll to get the desired sparkliness
      uint8_t randomNum = random(chance);
      if (randomNum == 1)
      {
        uint8_t gradientPositionOffset = gradientPosition + (column * 1.75) ; //Spread our gradient across the x axis of our LED array
        leds[ xy(column, row) ] = CHSV(colorArray[gradientPositionOffset][0], colorArray[gradientPositionOffset][1], colorArray[gradientPositionOffset][2]);
      }
    }
  }
  colorDelay++;
  frameDelay++;
  if (colorDelay >= randomDelay)//Only want to randomize at random intervals for more randomness 
  {
    colorRandomize(randomHue, randomSat, randomVal);
    colorDelay = 0;
  }
  if (frameDelay % 5 == 0)
  {
    colorArray[gradientPosition][0] = randomHue;
    colorArray[gradientPosition][1] = randomSat;
    colorArray[gradientPosition][2] = randomVal;
    gradientPosition++;
  }
  FastLED.show();
  fadeAll(200);//Passing a lower number into fadeAll will make each sparkle fade more quickly. Use in conjucntion with the setting of the random number
}

void setup() {
  randomSeed(1);
  LEDS.addLeds<APA102, DATA_PIN1, CLOCK_PIN1, RGB>(leds, NUM_LEDS);
}

void loop() {
  sparkleRandom();
}
