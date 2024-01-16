#ifndef sat_rms_included
#define sat_rms_included
//----------------------------------------------------------------------

// original author: lubomir i ivanov (for axonlib)
// used with permission

//----------------------------------------------------------------------

/*
  one pole filter for RMS approximation
  usage example:
    dspRMS rmsfilter;
    rmsfilter.setup(300, 44100);              // 300ms, samplerate (can be updateSampleRate())
    float rmsout = rmsfilter.process(input);  // process input sample
    rmsfilter.reset();                        // to reset the rms value
*/

template <typename T>
class SAT_Rms {

  private:

    T win,fout,a0,b1;

  public:

    SAT_Rms() {
      reset();
      //setup(300,44100);
    }

    //----------

    SAT_Rms(T AWindowLength, T ASampleRate) {
      reset();
      setup(AWindowLength,ASampleRate);
    }

    //----------

    /*
      set filter coeff
      winlen  : window length (ms)
      srate   : sample rate
    */

    void setup(T AWindowLength, T ASampleRate) {
      b1 = exp(-1.0 / (AWindowLength * ASampleRate * 0.001));
      a0 = 1.0 - b1;
    }

    //----------

    void reset() {
      win = 0;
      fout = 0;
      a0 = 0;
      b1 = 0;
    }

    //----------

    T process(T input) {
      fout = a0 * input * input + b1 * fout;// + MIP_FLOAT_DENORM;
      return sqrt(fout);
    }

};

//----------------------------------------------------------------------
#endif
