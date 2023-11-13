#ifndef mip_gordon_smith_included
#define mip_gordon_smith_included
//----------------------------------------------------------------------

// unfinished
// untested

// https://github.com/abaga129/lib_dsp/blob/master/oscillators/Gordon-SmithOscillator.h

//----------
/*
  http://www.kvraudio.com/forum/viewtopic.php?p=5859488#p5859488

  That particular algorithm is known by many names, but I don't think
  "Gordon-Smith" is really that popular. Most of the time (in audio anyway)
  it's just called "modified coupled form." There might have also been some
  other common name that I can't recall right now. In graphics it's also known
  as the magic circle. Other names might be found in other engineering fields.

*/

//----------------------------------------------------------------------

//#include<cmath>
//#include "../core/core.h"

/*
  Class for creating pure sine wave oscillators or LFOs based on the Gordon
  Smith design. The frequency can change smoothly with no audible pops or
  clicks.
*/

class MIP_GordonSmith {

private:

    float yq;
    float yq1;
    float yn;
    float yn1;
    float epsilon;
    float theta;
    float fo;
    float fs;
    bool  init;

  public:

    MIP_GordonSmith() {
      init = false;
    }

    //----------

    /*
      Must be called at the beginning of processBlock() in the Audio Processor
      This method initializes all the values neccesary to begin oscillation.
    */

    void frequency(float frequency, float sampleRate) {
      fo = frequency;
      fs = sampleRate;
      theta = 2 * MIP_PI * fo / fs;
      epsilon = 2 * sinf(theta / 2);
      if (!init) {
        yn1 = sinf(-1 * theta);
        yq1 = cosf(-1 * theta);
        init = true;
      }
    }

    //----------

    // Returns the next sample and increments the delaySamples

    float process(/*float input*/void) {
      yq = yq1 - epsilon * yn1;
      yn = epsilon * yq + yn1;
      yq1 = yq;
      yn1 = yn;
      return yn;
    }

    //----------

    void flush(void) {
    }

};

//----------------------------------------------------------------------
#endif
