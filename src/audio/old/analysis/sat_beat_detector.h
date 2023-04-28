#ifndef mip_beat_detector_included
#define mip_beat_detector_included
//----------------------------------------------------------------------

#define FREQ_LP_BEAT  150.0                             /* Low Pass filter frequency */
#define T_FILTER      1.0/(2.0 * MIP_PI*FREQ_LP_BEAT)  /* Low Pass filter time constant */
#define BEAT_RTIME    0.02                              /* Release time of enveloppe detector in second */

/*
function fabs(value:single):Single;
asm
  fld value
  fabs
  fwait
end;
*/

#include <math.h>

// http://www.musicdsp.org/showArchiveComment.php?ArchiveID=200

class MIP_BeatDetector {

  private:

    float MBeatFilter;    // Filter coefficient
    float MFilter1Out;
    float MFilter2Out;
    float MBeatRelease;    // Release time coefficient
    float MPeakEnv;        // Peak enveloppe follower
    bool  MBeatTrigger;    // Schmitt trigger output
    bool  MPrevBeatPulse;  // Rising edge memory

  public:

    bool  MBeatPulse;      // Beat detector output

  public:

    MIP_BeatDetector() {
      MFilter1Out = 0.0;
      MFilter2Out = 0.0;
      MPeakEnv = 0.0;
      MBeatTrigger = false;
      MPrevBeatPulse = false;
      setSampleRate(44100.0f);
    }

    //----------

    ~MIP_BeatDetector() {
    }

    //----------

    void setSampleRate(float ASampleRate) {
      MBeatFilter = 1.0 / (ASampleRate*T_FILTER);
      MBeatRelease = exp( -1.0 / (ASampleRate*BEAT_RTIME) );
    }

    //----------

    void process(float AInput) {
      float EnvIn;
      // Step 1 : 2nd order low pass filter (made of two 1st order RC filter)
      MFilter1Out = MFilter1Out + ( MBeatFilter * (AInput-MFilter1Out) );
      MFilter2Out = MFilter2Out + ( MBeatFilter * (MFilter1Out-MFilter2Out) );
      // Step 2 : peak detector
      EnvIn = /*fabs*/abs(MFilter2Out);
      if (EnvIn > MPeakEnv) {
        MPeakEnv = EnvIn;  // Attack time = 0
      }
      else {
        MPeakEnv = MPeakEnv * MBeatRelease;
        MPeakEnv = MPeakEnv + (1.0-MBeatRelease) * EnvIn;
      }
      // Step 3 : Schmitt trigger
      if (!MBeatTrigger) {
        if (MPeakEnv > 0.3) MBeatTrigger = true;
      }
      else {
        if (MPeakEnv < 0.15) MBeatTrigger = false;
      }
      // Step 4 : rising edge detector
      MBeatPulse = false;
      if (MBeatTrigger && !MPrevBeatPulse) MBeatPulse = true;
      MPrevBeatPulse = MBeatTrigger;
    }

};

#undef FREQ_LP_BEAT
#undef T_FILTER
#undef BEAT_RTIME

//----------------------------------------------------------------------
#endif

