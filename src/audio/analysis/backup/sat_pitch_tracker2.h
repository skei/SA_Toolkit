#ifndef mip_pitchtracker2_included
#define mip_pitchtracker2_included
//----------------------------------------------------------------------

#include "common/mip_math.h"

class MIP_PitchTracker2 {

  private:
    float b0 = 0;
    float a1 = 0;
    float x0 = 0;
    float in_x0 = 0;
    float hp_x0 = 0;
    float in_power = 0;
    float hp_power = 0;
    float f = 0;

  public:

    MIP_PitchTracker2() {
    }

    ~MIP_PitchTracker2() {
    }

  public:

    void calc_coefs(float sample_rate,float milliseconds) {
      // Filter coefs for power averaging
      // Trapezoidal filter gave me more accurate power at
      // high frequencies than standard EWMA
      float tc = tan( 1000.0 / (sample_rate * milliseconds) * MIP_PI );
      b0 = tc / ( 1.0 + tc );
      a1 = ( tc - 1 ) / ( tc + 1 );
    }

    float process(float in) {
      // Split the signal into two: raw and a simple differentiator
      float x1 = x0;
      x0 = in;
      float hp = ( x0 - x1 ) * 0.5; // Simple differentiator. equivalent to BLT high pass at PI / 2
      // Calculate the average power of each
      float in_x1 = in_x0;
      float hp_x1 = hp_x0;
      in_x0 = in * in;
      hp_x0 = hp * hp;
      in_power =  ( in_x0 + in_x1 ) * b0 - in_power * a1;
      hp_power =  ( hp_x0 + hp_x1 ) * b0 - hp_power * a1;
      // Normalize power of hp to the total power
      f = hp_power / in_power;
      // Calculate inverse of transfer function
      f = sqrtf( -f / (f  - 1) ); // Function is positive for valid input values. 0 <= f < 1
      return f;
    }

};

//----------------------------------------------------------------------
#endif

/*

  https://www.reddit.com/r/DSP/comments/57hgdp/a_crude_frequency_estimation_thing/

  I came up with this simple frequency estimation tool. As far as I can tell,
  it estimates the frequency center in terms of power. It works perfectly for
  sine waves or finding the cutoff of a filter when applied to white noise.
  I don't know if this is something that already exists.

    // Filter coefs for power averaging
    // Trapezoidal filter gave me more accurate power at
    // high frequencies than standard EWMA
    tc = tan( 1000.0 / (sample_rate * miliseconds) * PI );
    b0 = tc / ( 1.0 + tc );
    a1 = ( tc - 1 ) / ( tc + 1 );

    // Split the signal into two: raw and a simple differentiator
    x1 = x0;
    x0 = in;
    hp = ( x0 - x1 ) * 0.5; // Simple differentiator. equivalent to BLT high pass at PI / 2

    // Calculate the average power of each
    in_x1 = in_x0;
    hp_x1 = hp_x0;
    in_x0 = in * in;
    hp_x0 = hp * hp;
    in_power =  ( in_x0 + in_x1 ) * b0 - in_power * a1;
    hp_power =  ( hp_x0 + hp_x1 ) * b0 - hp_power * a1;

    // Normalize power of hp to the total power
    f = hp_power / in_power;

    // Calculate inverse of transfer function
    f = sqrt( -f / (f  - 1) ); // Function is positive for valid input values. 0 <= f < 1

  The output is a normalized digital (warped) frequency. You can linearize it and convert to Hz with

    f_hz = sample_rate * atan(f) / PI;

  Let me know if it's useful for anything.

*/
