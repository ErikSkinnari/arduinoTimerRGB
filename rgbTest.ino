#include "Adafruit_WS2801.h"
#include "SPI.h" // Comment out this line if using Trinket or Gemma
#ifdef __AVR_ATtiny85__
#include <avr/power.h>
#endif

/*****************************************************************************
Example sketch for driving Adafruit WS2801 pixels!


  Designed specifically to work with the Adafruit RGB Pixels!
  12mm Bullet shape ----> https://www.adafruit.com/products/322
  12mm Flat shape   ----> https://www.adafruit.com/products/738
  36mm Square shape ----> https://www.adafruit.com/products/683

  These pixels use SPI to transmit the color data, and have built in
  high speed PWM drivers for 24 bit color per pixel
  2 pins are required to interface

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution

*****************************************************************************/

// Declarations
uint8_t dataPin = 5;  // Yellow wire on Adafruit Pixels
uint8_t clockPin = 6; // Green wire on Adafruit Pixels
long timeSinceReset = 0;
int timeSpan = 50;
int minute = 0;
long r = 0;
long g = 255;
long b = 0;
int pixelCount = 1;

Adafruit_WS2801 strip = Adafruit_WS2801(pixelCount, dataPin, clockPin);

void setup()
{
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
  clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
#endif

  strip.begin();

  strip.show();
}

void loop()
{

  if ((millis() % 500) == 0)
  {
    if (r < 255 - floorf(255 / timeSpan))
    {
      r += floorf(255 / timeSpan);
    }
    else
    {
      r = 255;
    }
    if (g > 255 / timeSpan)
    {
      g -= floorf(255 / timeSpan);
    }
    else
    {
      g = 0;
    }

    minute++;
    timeSpan--;
  }
  if (timeSpan <= 0) {
    colorWipe(Color(0,0,0), 0);
    delay(100);
    colorWipe(Color(255,0,0), 0);
    delay(100);
  }

  colorWipe(Color(r, g, b), 0);

}

void bug(){
  1 = 0;
}

void rainbow(uint8_t wait)
{
  int i, j;

  for (j = 0; j < 256; j++)
  { // 3 cycles of all 256 colors in the wheel
    for (i = 0; i < strip.numPixels(); i++)
    {
      strip.setPixelColor(i, Wheel((i + j) % 255));
    }
    strip.show(); // write all the pixels out
    delay(wait);
  }
}

// Slightly different, this one makes the rainbow wheel equally distributed
// along the chain
void rainbowCycle(uint8_t wait)
{
  int i, j;

  for (j = 0; j < 256 * 5; j++)
  { // 5 cycles of all 25 colors in the wheel
    for (i = 0; i < strip.numPixels(); i++)
    {
      // tricky math! we use each pixel as a fraction of the full 96-color wheel
      // (thats the i / strip.numPixels() part)
      // Then add in j which makes the colors go around per pixel
      // the % 96 is to make the wheel cycle around
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) % 256));
    }
    strip.show(); // write all the pixels out
    delay(wait);
  }
}

// fill the dots one after the other with said color
// good for testing purposes
void colorWipe(uint32_t c, uint8_t wait)
{
  int i;

  for (i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

/* Helper functions */

// Create a 24 bit color value from R,G,B
uint32_t Color(byte r, byte g, byte b)
{
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}

//Input a value 0 to 255 to get a color value.
//The colours are a transition r - g -b - back to r
uint32_t Wheel(byte WheelPos)
{
  if (WheelPos < 85)
  {
    return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
  else if (WheelPos < 170)
  {
    WheelPos -= 85;
    return Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  else
  {
    WheelPos -= 170;
    return Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
