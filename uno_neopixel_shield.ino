#include <FastLED.h>

#define LED_PIN 6
#define LED_TYPE WS2812
#define COLOR_ORDER GRB
#define BRIGHTNESS 30
#define NUM_LEDS 40

CRGB leds[NUM_LEDS];

#define UPDATE_MS 10
uint32_t last_update_ms = 0;

// I'm gonna use breathe value as a brightness...cycle it from 0 to 255 and back.
int breathe_value = 0;
int breathe_direction = 1;

void init_breathe()
{
  int i;

  for (i=0;i<NUM_LEDS;i++)
  {
    leds[i] = CRGB::Green;
  }

  breathe_value = 0;
  
  FastLED.setBrightness(breathe_value);
  FastLED.show();
}

void process_breathe()
{
  breathe_value = breathe_value + breathe_direction;

  /* I need to pause a little on "black".  To do this, I'm gonna actually 
   *  let breathe value go down to -50, but keep the brightness at 0 there.
   *  Then, once we hit -50, we start walking back up...but still use 
   *  brightness of 0.
   */
  if (breathe_value > 255)
  {
    breathe_value = 255;
    breathe_direction = -1;
    FastLED.setBrightness(breathe_value);
  }
  else if (breathe_value < -50)
  {
    breathe_direction = 1;
    FastLED.setBrightness(0);
  }
  else if (breathe_value < 0)
  {
    FastLED.setBrightness(0);
  }
  else
  {
    FastLED.setBrightness(breathe_value);
  }

  FastLED.show();
}

void setup()
{
  int i;
  
  Serial.begin(9600);

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );

  Serial.println("Initialized...starting power-up-test");
  
  // Power up test...cycle all LEDs blue.
  for (i=0;i<NUM_LEDS;i++)
  {
    leds[i] = CRGB::Blue;
    FastLED.show();
    FastLED.delay(100);
  }

  Serial.println("power up test complete");

  init_breathe();
}

void loop(void)
{
  uint32_t current_ms;

  current_ms = millis();

  if (current_ms > last_update_ms + UPDATE_MS)
  {
    last_update_ms = current_ms;
    process_breathe();
  }
  
}
