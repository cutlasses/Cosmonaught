#include "TapBPM.h"

const float MAX_DURATION = 5000;  // 5s between beats

TAP_BPM::TAP_BPM( int button_pin ) :
  m_tap_button( button_pin, false ),
  m_average_times(),
  m_prev_tap_time_ms(-1.0f),
  m_next_beat_time_ms(0.0f),
  m_current_beat( NO_BEAT )
{
  
}

bool TAP_BPM::valid_bpm() const
{
 #ifdef FIXED_TEMPO
  return true;
 #else
  return m_average_times.size() >= 2; // 3 taps
#endif  
}

float TAP_BPM::bpm() const
{
  //static const float mult = 60 / 1000.0f; 
  const float dur_ms = beat_duration_ms();
  const float dur_s = dur_ms / 1000.0f;

  const float bps = 1.0f / dur_s;
  const float bpm = bps / 60.0f;
  
  return bpm;
}

float TAP_BPM::beat_duration_ms() const
{
#ifdef FIXED_TEMPO
  return 500;
#endif

  ASSERT_MSG( valid_bpm(), "beat_duration_ms() invalid_bpm!" );
  return m_average_times.average();
}

void TAP_BPM::setup()
{
  m_tap_button.setup();
}

void TAP_BPM::update( float time_ms )
{  
  m_current_beat = NO_BEAT;
  
  m_tap_button.update( time_ms );

  // button pressed
  if( m_tap_button.down_time_ms() > 2000 )
  {
    // reset on long hold (2s)
    m_average_times.reset();
  }
  else if( m_tap_button.single_click() )
  {
#ifdef DEBUG_OUTPUT
    Serial.print("Button down\n");
#endif // DEBUG_OUTPUT
    
    if( m_prev_tap_time_ms > 0.0f )
    {
      float duration = time_ms - m_prev_tap_time_ms;

      if( duration < MAX_DURATION )
      {
        m_average_times.add( duration );
      }
    }

    m_prev_tap_time_ms = time_ms;

    if( valid_bpm() )
    {
      m_next_beat_time_ms = time_ms + beat_duration_ms();
    }

    m_current_beat = TAP_BEAT;
  }
  // no button press - wait for next beat
  else
  {    
    if( valid_bpm() )
    {
      if( time_ms > m_next_beat_time_ms )
      {
        m_next_beat_time_ms = m_next_beat_time_ms + beat_duration_ms();
  
        m_current_beat = AUTO_BEAT;
      }
    }
  }
}

TAP_BPM::BEAT_TYPE TAP_BPM::beat_type() const
{
  return m_current_beat;
}

