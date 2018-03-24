#include <OctoWS2811.h>
#include "TapBPM.h"

const int LEDS_IN_STRIP = 18;

const int NUM_COLOURS = 5;
const int g_colours[NUM_COLOURS] = { 0xFF0000, 0x00FF00, 0x0000FF, 0xFFFF00, 0xFFFFFF };

DMAMEM int g_display_memory[ LEDS_IN_STRIP * 6 ];
int g_drawing_memory[ LEDS_IN_STRIP * 6 ];

const int LED_CONFIG = WS2811_GRB | WS2811_800kHz;

OctoWS2811 g_led_strip( LEDS_IN_STRIP, g_display_memory, g_drawing_memory, LED_CONFIG );

TAP_BPM g_tap_bpm( 23 ); // pin 23

void setup()
{
  Serial.begin(9600);

#ifdef DEBUG_OUTPUT
  serial_port_initialised = true;
#endif // DEBUG_OUTPUT
  
  g_led_strip.begin();
  g_led_strip.show();

  g_tap_bpm.setup();

  DEBUG_TEXT("SETUP\n");
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

void clear_pixels()
{
  for( int i = 0; i < LEDS_IN_STRIP; ++i )
  {
    g_led_strip.setPixel( i, 0 );
  }
}

void loop()
{  
  static int pixel = 0;
  static int colour = 0;
  static int inc = 1;
  static bool reset = true;

  g_tap_bpm.update( millis() );

  if( g_tap_bpm.valid_bpm() )
  {
    const int beat_duration_us = g_tap_bpm.beat_duration_ms() * 1000;
    const int light_duration_us = beat_duration_us / LEDS_IN_STRIP;
    reset = false;
    pixel += inc;
    if( pixel == 0 || pixel == LEDS_IN_STRIP - 1 )
    {
      inc *= -1; // invert direction

      colour = (colour + 1) % NUM_COLOURS;
    }
    
    light_pixel( pixel, g_colours[colour] );

    g_led_strip.show();
    delayMicroseconds( light_duration_us );
  }
  else if( !reset )
  {
    // don't bother to clear pixels until the first beat (might conserve power this way)
    reset = true;
    
    clear_pixels();

    g_led_strip.show();
  }
}
