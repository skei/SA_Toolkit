#ifndef sat_karlsen_filter_included
#define sat_karlsen_filter_included
//----------------------------------------------------------------------

//#define sDenorm (1.0e-24);

/*
  http://musicdsp.org/showArchiveComment.php?ArchiveID=141
  ---
  Nice to see a pascal version too, Christian!
  Although I really recommend one set a lower denormal threshold, maybe a 1/100, it really affects the sound of the filter. The best is probably tweaking that value in realtime to see what sounds best.
  Also, doubles for the buffers.. :)
  ---
  Christian, shouldn't your code end:
  L:=fPole[4];
  B:=fPole[4]-fPole[1];
  //CWB posted
  //N:=I-fPole[1];
  //B:=I-fPole[4]-fPole[1];
  //DSP posted
  H:=I-fPole[4]; //Surely pole 4 would give a 24dB/Oct HP, rather than the 6dB version posted
  N:=I-fPole[4]-fPole[1]; //Inverse of BP
  Any thoughts, anyone?
  DSP

*/

//----------------------------------------------------------------------

template <typename T>
class SAT_KarlsenFilter {

//------------------------------
private:
//------------------------------

  T   fQ;
  T   fF1,fF;
  T   fFS;
  T  fTmp;
  uint8_t   fOS;
  T   fPole[4];

  T L;
  T B;
  T N;
  T H;

//------------------------------
public:
//------------------------------

  //property    Frequency   : Single read fF write SetFrequency;
  //property    SampleRate  : Single read fFS write fFS;
  //property    Q           : Single read fQ write SetQ;
  //property    OverSample  : Byte read fOS write fOS;

  T getFrequency(void)  { return fF; }
  T getSampleRate(void) { return fFS; }
  T getQ(void)          { return fQ; }
  uint8_t getOverSample(void) { return fOS; }

//------------------------------
private:
//------------------------------

  void setFrequency(T v) {
    if (fFS<=0) return; //raise exception.create('Sample Rate Error!');
    if (v!=fF) {
      fF=v;
      fF1 = fF/fFS; // fF1 range from 0..1
    }
  }

  //----------

  void setQ(T v) {
    if (v!=fQ) {
      if (v<0) fQ = 0;
      else if (v>50) fQ = 50;
      else fQ = v;
    }
  }

//------------------------------
public:
//------------------------------

  SAT_KarlsenFilter() {
    fFS = 44100;
    setFrequency(1000);
    fOS = 2;
    setQ(1);
  }

  //----------

  ~SAT_KarlsenFilter() {
  }

  //----------

  void process(T AValue/*, T* L, T* B, T* N, T* H*/) {
    for (int32_t o=0; o<fOS; o++) {
      T prevfp = fTmp;
      if (prevfp > 1) prevfp = 1; // Q-limiter
      fTmp = (fTmp*0.418)+((fQ*fPole[4])*0.582); // dynamic feedback
      T intfp = (fTmp*0.36)+(prevfp*0.64); // feedback phase

      fPole[1] = (((AValue /*+ sDenorm*/ /* + SAT_FLOAT_DENORM*/ )-intfp) * fF1) + (fPole[1] * (1 - fF1));

      if (fPole[1] > 1) fPole[1] = 1;
      else if (fPole[1] < -1) fPole[1] = -1;
      fPole[2] = (fPole[1]*fF1)+(fPole[2]*(1-fF1)); // pole 2
      fPole[3] = (fPole[2]*fF1)+(fPole[3]*(1-fF1)); // pole 3
      fPole[4] = (fPole[3]*fF1)+(fPole[4]*(1-fF1)); // pole 4
    }
    L = fPole[4];
    B = fPole[4]-fPole[1];
    N = AValue-fPole[1];
    H = AValue-fPole[4]-fPole[1];
  }

};

//#undef sDenorm

//----------------------------------------------------------------------
#endif




