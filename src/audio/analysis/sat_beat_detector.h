#ifndef sat_beat_detector_included
#define sat_beat_detector_included
//----------------------------------------------------------------------

#define FREQ_LP_BEAT  150.0                             /* Low Pass filter frequency */
#define T_FILTER      1.0/(2.0 * SAT_PI*FREQ_LP_BEAT)  /* Low Pass filter time constant */
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

template <typename T>
class SAT_BeatDetector {

  private:

    T     MBeatFilter;    // Filter coefficient
    T     MFilter1Out;
    T     MFilter2Out;
    T     MBeatRelease;    // Release time coefficient
    T     MPeakEnv;        // Peak enveloppe follower
    bool  MBeatTrigger;    // Schmitt trigger output
    bool  MPrevBeatPulse;  // Rising edge memory

  public:

    bool  MBeatPulse;      // Beat detector output

  public:

    SAT_BeatDetector() {
      MFilter1Out = 0.0;
      MFilter2Out = 0.0;
      MPeakEnv = 0.0;
      MBeatTrigger = false;
      MPrevBeatPulse = false;
      setSampleRate(44100.0f);
    }

    //----------

    ~SAT_BeatDetector() {
    }

    //----------

    void setSampleRate(T ASampleRate) {
      MBeatFilter = 1.0 / (ASampleRate*T_FILTER);
      MBeatRelease = exp( -1.0 / (ASampleRate*BEAT_RTIME) );
    }

    //----------

    bool process(T AInput) {
      T EnvIn;
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
      return MBeatPulse;
    }

};

#undef FREQ_LP_BEAT
#undef T_FILTER
#undef BEAT_RTIME

//----------------------------------------------------------------------
#endif

