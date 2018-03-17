#pragma once

#include <ADC.h>
#include <Bounce.h>


//////////////////////////////////////

class BUTTON
{
  int16_t       m_data_pin;
  int16_t       m_is_toggle : 1;
  int16_t       m_prev_is_active : 1;
  int16_t       m_is_active : 1;
  int16_t       m_down_time_valid : 1;
  uint32_t       m_down_time_stamp;
  uint32_t       m_down_time_curr;

  Bounce        m_bounce;

public:

  BUTTON( int data_pin, bool is_toggle );

  bool          active() const;
  bool          single_click() const;

  int32_t       down_time_ms() const;

  void          setup();
  void          update( uint32_t time_ms );
};


