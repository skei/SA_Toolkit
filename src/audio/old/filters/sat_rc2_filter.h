#ifndef mip_rc2_filter_included
#define mip_rc2_filter_included
//----------------------------------------------------------------------

// http://musicdsp.org/showArchiveComment.php?ArchiveID=185


class MIP_Rc2Filter {

  private:

    float c;      // cutoff
    float r;      // resonance
    float z0,z1;

  public:

    MIP_Rc2Filter() {
      z0 = 0.0f;
      z1 = 0.0f;
    }

    //----------

    ~MIP_Rc2Filter() {
    }

    //----------

     // 0..1

    void setCutoff(float ACutoff) {
      float cutoff = ACutoff * 127.0f;
      c = powf(0.5f, (128.0f-cutoff) / 16.0f);
    }

    //----------

    void setFreq(float AFreq, float ARate) {
      c = 2.0f * sinf( MIP_PI * AFreq / ARate );
      //You can approximate this (tuning error towards nyquist):
      //c := MIP_PI2*freq/samplerate;
    }

    //----------

     // 0..4

    void setResonance(float AResonance) {
      float resonance = AResonance * 127.0f;
      r = powf(0.5f, (resonance+24.0f) / 16.0f);
    }

    //----------

    void setQ(float AQ) {
      r = 1.0f / AQ;
    }

    //----------

    float process(float AInput) {
      /*
      z0 = (1-r*c)*z0 - c*z1 + c*AInput;
      z1 = (1-r*c)*z1 + c*z0;
      result = z1;
      */
      // This filter has stability issues for high r values. State variable filter
      // stability limits seem to work fine here. It can also be oversampled for
      // better stability and wider frequency range (use 0.5*original frequency):
      z0 = (1.0f - r*c) * z0 - c*z1 + c*AInput;
      z1 = (1.0f - r*c) * z1 + c*z0;
      float tmp = z1;
      z0 = (1.0f - r*c) * z0 - c*z1 + c*AInput;
      z1 = (1.0f - r*c) * z1 + c*z0;
      return (tmp + z1) * 0.5f;
    }

};


//----------------------------------------------------------------------
#endif
