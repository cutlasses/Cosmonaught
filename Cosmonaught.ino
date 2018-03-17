#include <OctoWS2811.h>
#include "TapBPM.h"

const int LEDS_IN_STRIP = 18;

DMAMEM int g_display_memory[ LEDS_IN_STRIP * 6 ];
int g_drawing_memory[ LEDS_IN_STRIP * 6 ];

const int LED_CONFIG = WS2811_GRB | WS2811_800kHz;

OctoWS2811 g_led_strip( LEDS_IN_STRIP, g_display_memory, g_drawing_memory, LED_CONFIG );

TAP_BPM g_tap_bpm( 23 ); // pin 23

void setup()
{
  Serial.begin(9600);
  
  g_led_strip.begin();
  g_led_strip.show();
}

void light_pixel( int pixel_index, int colour )
{
  for( int i = 0; i < LEDS_IN_STRIP; ++i )
  {
    if( i == pixel_index )
    {
      g_led_strip.setPixel( i, colour );
    }
    else
    {
      g_led_strip.setPixel( i, 0 );
    }
  }
}

void loop()
{
  constexpr int beat_duration = ( 120 / 60 ) * 1000 * 1000;
  constexpr int light_duration = beat_duration / LEDS_IN_STRIP;
  
  static int pixel = 0;
  static int inc = 1;

  pixel += inc;
  if( pixel == 0 || pixel == LEDS_IN_STRIP - 1 )
  {
    inc *= -1; // invert direction
  }
  
  light_pixel( pixel, 0xFFFFFF );
  g_led_strip.show();
  delayMicroseconds( light_duration );
}
