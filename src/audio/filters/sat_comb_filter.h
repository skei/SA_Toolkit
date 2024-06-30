#ifndef sat_comb_filter_included
#define sat_comb_filter_included
//----------------------------------------------------------------------

// https://github.com/rjeschke/cetonesynths/blob/master/cetoneorg/reverb.h

//----------

/*
    20hz @ 192k, srate / hz
    192k/20 = 9.6k
*/

#define MAX_BUFFER (1024*16) // 16k

//----------

template <typename T>
class SAT_CombFilter {

  private:

    T       _Buffer[MAX_BUFFER]; // was 8192
    int32_t _Pos;
    T       _History;
    int32_t _Size;
    T       _Damp1;
    T       _Damp2;
    T       _Feedback;

  //------------------------------
  //
  //------------------------------

  public:

    SAT_CombFilter() {
      //_Size;
      //_Damp1;
      //_Damp2;
      //_Feedback;
      reset();
    }

    //----------

    ~SAT_CombFilter() {
    }

  //------------------------------
  //
  //------------------------------

  public:

    void reset(void) {
      _History = 0.0f;
      memset(_Buffer, 0, MAX_BUFFER * sizeof(T));
    }

    //----------

    void size(int32_t ASize) {
      _Size = ASize;
      _Pos %= _Size;
      //_Pos = 0;
      //reset();
    }

    //----------

    void feedback(T AValue) {
      _Feedback = AValue;
    }

    //----------

    void damp(T AValue) {
      _Damp1 = AValue;
      _Damp2 = 1.0f - AValue;
    }

  //------------------------------
  //
  //------------------------------

  public:

    T process(T input) {
      //KDTrace("%i ",_Size);
      T output = _Buffer[_Pos];
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
