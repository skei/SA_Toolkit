#ifndef mip_comb_filter_included
#define mip_comb_filter_included
//----------------------------------------------------------------------

// https://github.com/rjeschke/cetonesynths/blob/master/cetoneorg/reverb.h

//----------

/*
    20hz @ 192k, srate / hz
    192k/20 = 9.6k
*/

#define MAX_BUFFER (1024*16) // 16k

//----------

class MIP_CombFilter {

  private:

    float   _Buffer[MAX_BUFFER]; // was 8192
    int32_t _Pos;
    float   _History;
    int32_t _Size;
    float   _Damp1;
    float   _Damp2;
    float   _Feedback;

  //------------------------------
  //
  //------------------------------

  public:

    MIP_CombFilter() {
      //_Size;
      //_Damp1;
      //_Damp2;
      //_Feedback;
      reset();
    }

    //----------

    ~MIP_CombFilter() {
    }

  //------------------------------
  //
  //------------------------------

  public:

    void reset(void) {
      _History = 0.0f;
      MIP_Memset(_Buffer, 0, MAX_BUFFER * sizeof(float));
    }

    //----------

    void size(int32_t ASize) {
      _Size = ASize;
      _Pos %= _Size;
      //_Pos = 0;
      //reset();
    }

    //----------

    void feedback(float AValue) {
      _Feedback = AValue;
    }

    //----------

    void damp(float AValue) {
      _Damp1 = AValue;
      _Damp2 = 1.0f - AValue;
    }

  //------------------------------
  //
  //------------------------------

  public:

    float process(float input) {
      //KDTrace("%i ",_Size);
      float output = _Buffer[_Pos];
      _History = (output * _Damp2) + (_History * _Damp1);
      //KKillDenorm(_History);
      _Buffer[_Pos] = input + (_History * _Feedback);
      //KKillDenorm(_Buffer[_Pos]);
      _Pos = (_Pos + 1) % _Size;
      return output;
    }

};

#undef MAX_BUFFER

//----------------------------------------------------------------------
#endif
