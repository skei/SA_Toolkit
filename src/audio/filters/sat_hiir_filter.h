#ifndef mip_hiir_filter_included
#define mip_hiir_filter_included
//----------------------------------------------------------------------

/*
  polyphase halfband
  dave / muon software
  http://www.musicdsp.org/archive.php?classid=3#39
*/



/*
  It is worth noting that if these filters are being used for upsampling/
  downsampling, the "noble identity" can be used to reduce the CPU cost.
  The basic idea is that operations that can be expressed in the form:
  filter that uses z^-N for its states -> downsample by N can be rearranged
  to use the form downsample by N -> filter that uses z^-1 for its states.
  The same property holds true for upsampling.
  See http://mue.music.miami.edu/thesis/jvandekieft/jvchapter3.htm for more details.
  For the above code, this would entail creating an alternative allpass process
  function, that uses the z^-1 for its states, and then rearranging some of the
  operations.
*/

#include "audio/filters/sat_allpass_filter.h"

//----------------------------------------------------------------------

//rejection=104dB, transition band=0.01

template <typename T>
class MIP_Hiir12SteepFilter {
  private:
    T FPrevious;
    SAT_AllpassFilter<T> a0,a1,a2,a3,a4,a5;
    SAT_AllpassFilter<T> b0,b1,b2,b3,b4,b5;
  public:
    MIP_Hiir12SteepFilter() {
      FPrevious = 0;
      a0.setCoefficient(0.036681502163648017);
      a1.setCoefficient(0.2746317593794541);
      a2.setCoefficient(0.56109896978791948);
      a3.setCoefficient(0.769741833862266);
      a4.setCoefficient(0.8922608180038789);
      a5.setCoefficient(0.962094548378084);
      b0.setCoefficient(0.13654762463195771);
      b1.setCoefficient(0.42313861743656667);
      b2.setCoefficient(0.6775400499741616);
      b3.setCoefficient(0.839889624849638);
      b4.setCoefficient(0.9315419599631839);
      b5.setCoefficient(0.9878163707328971);
    }
    T process(T AInput) {
      T a = AInput;
      a = a0.process(a);
      a = a1.process(a);
      a = a2.process(a);
      a = a3.process(a);
      a = a4.process(a);
      a = a5.process(a);
//      a = MIP_KillDenormal(a);
      T b = AInput;
      b = b0.process(b);
      b = b1.process(b);
      b = b2.process(b);
      b = b3.process(b);
      b = b4.process(b);
      b = b5.process(b);
//      b = SKillDenorm(b);
      T result = ( a + FPrevious ) * 0.5;
      FPrevious = b;
      return result;
    }
    void upsample(T AInput, T* AOutput1, T* AOutput2) {
      *AOutput1 = process(AInput);
      *AOutput2 = process(0.0f);
    }
    T downsample(T AInput1, T AInput2) {
      T out0 = process(AInput1);
      /*T out1 =*/ process(AInput2);
      return out0;
    }
};

//----------

//rejection=86dB, transition band=0.01

template <typename T>
class MIP_Hiir10SteepFilter {
  private:
    T FPrevious;
    SAT_AllpassFilter<T> a0,a1,a2,a3,a4;
    SAT_AllpassFilter<T> b0,b1,b2,b3,b4;
  public:
    MIP_Hiir10SteepFilter() {
      FPrevious = 0;
      a0.setCoefficient(0.051457617441190984);
      a1.setCoefficient(0.35978656070567017);
      a2.setCoefficient(0.6725475931034693);
      a3.setCoefficient(0.8590884928249939);
      a4.setCoefficient(0.9540209867860787);
      b0.setCoefficient(0.18621906251989334);
      b1.setCoefficient(0.529951372847964);
      b2.setCoefficient(0.7810257527489514);
      b3.setCoefficient(0.9141815687605308);
      b4.setCoefficient(0.985475023014907);
    }
    T process(T AInput) {
      T a = AInput;
      a = a0.process(a);
      a = a1.process(a);
      a = a2.process(a);
      a = a3.process(a);
      a = a4.process(a);
//      a = SKillDenorm(a);
      T b = AInput;
      b = b0.process(b);
      b = b1.process(b);
      b = b2.process(b);
      b = b3.process(b);
      b = b4.process(b);
//      b = SKillDenorm(b);
      T result = ( a + FPrevious ) * 0.5;
      FPrevious = b;
      return result;
    }
    void upsample(T AInput, T* AOutput1, T* AOutput2) {
      *AOutput1 = process(AInput);
      *AOutput2 = process(0.0f);
    }
    T downsample(T AInput1, T AInput2) {
      T out0 = process(AInput1);
      /*T out1 =*/ process(AInput2);
      return out0;
    }
};

//----------

//rejection=69dB, transition band=0.01

template <typename T>
class MIP_Hiir8SteepFilter {
  private:
    T FPrevious;
    SAT_AllpassFilter<T> a0,a1,a2,a3;
    SAT_AllpassFilter<T> b0,b1,b2,b3;
  public:
    MIP_Hiir8SteepFilter() {
      FPrevious = 0;
      a0.setCoefficient(0.07711507983241622);
      a1.setCoefficient(0.4820706250610472);
      a2.setCoefficient(0.7968204713315797);
      a3.setCoefficient(0.9412514277740471);
      b0.setCoefficient(0.2659685265210946);
      b1.setCoefficient(0.6651041532634957);
      b2.setCoefficient(0.8841015085506159);
      b3.setCoefficient(0.9820054141886075);
    }
    T process(T AInput) {
      T a = AInput;
      a = a0.process(a);
      a = a1.process(a);
      a = a2.process(a);
      a = a3.process(a);
//      a = SKillDenorm(a);
      T b = AInput;
      b = b0.process(b);
      b = b1.process(b);
      b = b2.process(b);
      b = b3.process(b);
//      b = SKillDenorm(b);
      T result = ( a + FPrevious ) * 0.5;
      FPrevious = b;
      return result;
    }
    void upsample(T AInput, T* AOutput1, T* AOutput2) {
      *AOutput1 = process(AInput);
      *AOutput2 = process(0.0f);
    }
    T downsample(T AInput1, T AInput2) {
      T out0 = process(AInput1);
      /*T out1 =*/ process(AInput2);
      return out0;
    }
};

//----------

//rejection=51dB, transition band=0.01

template <typename T>
class MIP_Hiir6SteepFilter {
  private:
    T FPrevious;
    SAT_AllpassFilter<T> a0,a1,a2;
    SAT_AllpassFilter<T> b0,b1,b2;
  public:
    MIP_Hiir6SteepFilter() {
      FPrevious = 0;
      a0.setCoefficient(0.1271414136264853);
      a1.setCoefficient(0.6528245886369117);
      a2.setCoefficient(0.9176942834328115);
      b0.setCoefficient(0.40056789819445626);
      b1.setCoefficient(0.8204163891923343);
      b2.setCoefficient(0.9763114515836773);
    }
    T process(T AInput) {
      T a = AInput;
      a = a0.process(a);
      a = a1.process(a);
      a = a2.process(a);
//      a = SKillDenorm(a);
      T b = AInput;
      b = b0.process(b);
      b = b1.process(b);
      b = b2.process(b);
//      b = SKillDenorm(b);
      T result = ( a + FPrevious ) * 0.5;
      FPrevious = b;
      return result;
    }
    void upsample(T AInput, T* AOutput1, T* AOutput2) {
      *AOutput1 = process(AInput);
      *AOutput2 = process(0.0f);
    }
    T downsample(T AInput1, T AInput2) {
      T out0 = process(AInput1);
      /*T out1 =*/ process(AInput2);
      return out0;
    }
};

//----------

//rejection=53dB, transition band=0.05

template <typename T>
class MIP_Hiir4SteepFilter {
  private:
    T FPrevious;
    SAT_AllpassFilter<T> a0,a1;
    SAT_AllpassFilter<T> b0,b1;
  public:
    MIP_Hiir4SteepFilter() {
      FPrevious = 0;
      a0.setCoefficient(0.12073211751675449);
      a1.setCoefficient(0.6632020224193995);
      b0.setCoefficient(0.3903621872345006);
      b1.setCoefficient(0.890786832653497);
    }
    T process(T AInput) {
      T a = AInput;
      a = a0.process(a);
      a = a1.process(a);
//      a = SKillDenorm(a);
      T b = AInput;
      b = b0.process(b);
      b = b1.process(b);
//      b = SKillDenorm(b);
      T result = ( a + FPrevious ) * 0.5;
      FPrevious = b;
      return result;
    }
    void upsample(T AInput, T* AOutput1, T* AOutput2) {
      *AOutput1 = process(AInput);
      *AOutput2 = process(0.0f);
    }
    T downsample(T AInput1, T AInput2) {
      T out0 = process(AInput1);
      /*T out1 =*/ process(AInput2);
      return out0;
    }
};

//----------

//rejection=36dB, transition band=0.1

template <typename T>
class MIP_Hiir2SteepFilter {
  private:
    T FPrevious;
    SAT_AllpassFilter<T> a0;
    SAT_AllpassFilter<T> b0;
  public:
    MIP_Hiir2SteepFilter() {
      FPrevious = 0;
      a0.setCoefficient(0.23647102099689224);
      b0.setCoefficient(0.7145421497126001);
    }
    T process(T AInput) {
      T a = AInput;
      a = a0.process(a);
//      a = SKillDenorm(a);
      T b = AInput;
      b = b0.process(b);
//      b = SKillDenorm(b);
      T result = ( a + FPrevious ) * 0.5;
      FPrevious = b;
      return result;
    }
    void upsample(T AInput, T* AOutput1, T* AOutput2) {
      *AOutput1 = process(AInput);
      *AOutput2 = process(0.0f);
    }
    T downsample(T AInput1, T AInput2) {
      T out0 = process(AInput1);
      /*T out1 =*/ process(AInput2);
      return out0;
    }
};


//----------------------------------------------------------------------
//softer slopes, more attenuation and less stopband ripple
//----------------------------------------------------------------------

//rejection=150dB, transition band=0.05

template <typename T>
class MIP_Hiir12Filter {
  private:
    T FPrevious;
    SAT_AllpassFilter<T> a0,a1,a2,a3,a4,a5;
    SAT_AllpassFilter<T> b0,b1,b2,b3,b4,b5;
  public:
    MIP_Hiir12Filter() {
      FPrevious = 0;
      a0.setCoefficient(0.01677466677723562);
      a1.setCoefficient(0.13902148819717805);
      a2.setCoefficient(0.3325011117394731);
      a3.setCoefficient(0.53766105314488);
      a4.setCoefficient(0.7214184024215805);
      a5.setCoefficient(0.8821858402078155);
      b0.setCoefficient(0.06501319274445962);
      b1.setCoefficient(0.23094129990840923);
      b2.setCoefficient(0.4364942348420355);
      b3.setCoefficient(0.6329609551399348); // (0.06329609551399348); bug fix, rob.belcham@zen.co.uk
      b4.setCoefficient(0.80378086794111226);
      b5.setCoefficient(0.9599687404800694);
    }
    T process(T AInput) {
      T a = AInput;
      a = a0.process(a);
      a = a1.process(a);
      a = a2.process(a);
      a = a3.process(a);
      a = a4.process(a);
      a = a5.process(a);
      a = MIP_KillDenormal(a);
      T b = AInput;
      b = b0.process(b);
      b = b1.process(b);
      b = b2.process(b);
      b = b3.process(b);
      b = b4.process(b);
      b = b5.process(b);
      b = MIP_KillDenormal(b);
      T result = ( a + FPrevious ) * 0.5;
      FPrevious = b;
      return result;
    }
    void upsample(T AInput, T* AOutput1, T* AOutput2) {
      *AOutput1 = process(AInput);
      *AOutput2 = process(0.0f);
    }
    T downsample(T AInput1, T AInput2) {
      T out0 = process(AInput1);
      /*T out1 =*/ process(AInput2);
      return out0;
    }
};

//rejection=133dB, transition band=0.05

template <typename T>
class MIP_Hiir10Filter {
  private:
    T FPrevious;
    SAT_AllpassFilter<T> a0,a1,a2,a3,a4;
    SAT_AllpassFilter<T> b0,b1,b2,b3,b4;
  public:
    MIP_Hiir10Filter() {
      FPrevious = 0;
      a0.setCoefficient(0.02366831419883467);
      a1.setCoefficient(0.18989476227180174);
      a2.setCoefficient(0.43157318062118555);
      a3.setCoefficient(0.6632020224193995);
      a4.setCoefficient(0.860015542499582);
      b0.setCoefficient(0.09056555904993387);
      b1.setCoefficient(0.3078575723749043);
      b2.setCoefficient(0.5516782402507934);
      b3.setCoefficient(0.7652146863779808);
      b4.setCoefficient(0.95247728378667541);
    }
    T process(T AInput) {
      T a = AInput;
      a = a0.process(a);
      a = a1.process(a);
      a = a2.process(a);
      a = a3.process(a);
      a = a4.process(a);
      a = MIP_KillDenormal(a);
      T b = AInput;
      b = b0.process(b);
      b = b1.process(b);
      b = b2.process(b);
      b = b3.process(b);
      b = b4.process(b);
      b = MIP_KillDenormal(b);
      T result = ( a + FPrevious ) * 0.5;
      FPrevious = b;
      return result;
    }
    void upsample(T AInput, T* AOutput1, T* AOutput2) {
      *AOutput1 = process(AInput);
      *AOutput2 = process(0.0f);
    }
    T downsample(T AInput1, T AInput2) {
      T out0 = process(AInput1);
      /*T out1 =*/ process(AInput2);
      return out0;
    }
};

//rejection=106dB, transition band=0.05

template <typename T>
class MIP_Hiir8Filter {
  private:
    T FPrevious;
    SAT_AllpassFilter<T> a0,a1,a2,a3;
    SAT_AllpassFilter<T> b0,b1,b2,b3;
  public:
    MIP_Hiir8Filter() {
      FPrevious = 0;
      a0.setCoefficient(0.03583278843106211);
      a1.setCoefficient(0.2720401433964576);
      a2.setCoefficient(0.5720571972357003);
      a3.setCoefficient(0.827124761997324);
      b0.setCoefficient(0.1340901419430669);
      b1.setCoefficient(0.4243248712718685);
      b2.setCoefficient(0.7062921421386394);
      b3.setCoefficient(0.9415030941737551);
    }
    T process(T AInput) {
      T a = AInput;
      a = a0.process(a);
      a = a1.process(a);
      a = a2.process(a);
      a = a3.process(a);
      a = MIP_KillDenormal(a);
      T b = AInput;
      b = b0.process(b);
      b = b1.process(b);
      b = b2.process(b);
      b = b3.process(b);
      b = MIP_KillDenormal(b);
      T result = ( a + FPrevious ) * 0.5;
      FPrevious = b;
      return result;
    }
    void upsample(T AInput, T* AOutput1, T* AOutput2) {
      *AOutput1 = process(AInput);
      *AOutput2 = process(0.0f);
    }
    T downsample(T AInput1, T AInput2) {
      T out0 = process(AInput1);
      /*T out1 =*/ process(AInput2);
      return out0;
    }
};

//rejection=80dB, transition band=0.05

template <typename T>
class MIP_Hiir6Filter {
  private:
    T FPrevious;
    SAT_AllpassFilter<T> a0,a1,a2;
    SAT_AllpassFilter<T> b0,b1,b2;
  public:
    MIP_Hiir6Filter() {
      FPrevious = 0;
      a0.setCoefficient(0.06029739095712437);
      a1.setCoefficient(0.4125907203610563);
      a2.setCoefficient(0.7727156537429234);
      b0.setCoefficient(0.21597144456092948);
      b1.setCoefficient(0.6043586264658363);
      b2.setCoefficient(0.9238861386532906);
    }
    T process(T AInput) {
      T a = AInput;
      a = a0.process(a);
      a = a1.process(a);
      a = a2.process(a);
      a = MIP_KillDenormal(a);
      T b = AInput;
      b = b0.process(b);
      b = b1.process(b);
      b = b2.process(b);
      b = MIP_KillDenormal(b);
      T result = ( a + FPrevious ) * 0.5;
      FPrevious = b;
      return result;
    }
    void upsample(T AInput, T* AOutput1, T* AOutput2) {
      *AOutput1 = process(AInput);
      *AOutput2 = process(0.0f);
    }
    T downsample(T AInput1, T AInput2) {
      T out0 = process(AInput1);
      /*T out1 =*/ process(AInput2);
      return out0;
    }
};

//rejection=70dB, transition band=0.1

template <typename T>
class MIP_Hiir4Filter {
  private:
    T FPrevious;
    SAT_AllpassFilter<T> a0,a1;
    SAT_AllpassFilter<T> b0,b1;
  public:
    MIP_Hiir4Filter() {
      FPrevious = 0;
      a0.setCoefficient(0.07986642623635751);
      a1.setCoefficient(0.5453536510711322);
      b0.setCoefficient(0.28382934487410993);
      b1.setCoefficient(0.8344118914807379);
    }
    T process(T AInput) {
      T a = AInput;
      a = a0.process(a);
      a = a1.process(a);
      a = MIP_KillDenormal(a);
      T b = AInput;
      b = b0.process(b);
      b = b1.process(b);
      b = MIP_KillDenormal(b);
      T result = ( a + FPrevious ) * 0.5;
      FPrevious = b;
      return result;
    }
    void upsample(T AInput, T* AOutput1, T* AOutput2) {
      *AOutput1 = process(AInput);
      *AOutput2 = process(0.0f);
    }
    T downsample(T AInput1, T AInput2) {
      T out0 = process(AInput1);
      /*T out1 =*/ process(AInput2);
      return out0;
    }
};

//rejection=36dB, transition band=0.1

template <typename T>
class MIP_Hiir2Filter {
  private:
    T FPrevious;
    SAT_AllpassFilter<T> a0;//,a1,a2;
    SAT_AllpassFilter<T> b0;//,b1,b2;
  public:
    MIP_Hiir2Filter() {
      FPrevious = 0;
      a0.setCoefficient(0.23647102099689224);
      b0.setCoefficient(0.7145421497126001);
    }
    T process(T AInput) {
      T a = AInput;
      a = a0.process(a);
      a = MIP_KillDenormal(a);
      T b = AInput;
      b = b0.process(b);
      b = MIP_KillDenormal(b);
      T result = ( a + FPrevious ) * 0.5;
      FPrevious = b;
      return result;
    }
    //T process0(void) {
    //  return 0;
    //}
    //void processDrop(void) {
    //}
    void upsample(T AInput, T* AOutput1, T* AOutput2) {
      *AOutput1 = process(AInput);
      *AOutput2 = process(0.0f);
    }
    T downsample(T AInput1, T AInput2) {
      T out0 = process(AInput1);
      /*T out1 =*/ process(AInput2);
      return out0;
    }
};

//----------------------------------------------------------------------
#endif

