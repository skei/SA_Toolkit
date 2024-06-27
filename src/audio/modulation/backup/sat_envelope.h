#ifndef mip_envelope_included
#define mip_envelope_included
//----------------------------------------------------------------------

// exponential decay
// https://en.wikipedia.org/wiki/Exponential_decay

//#define env_numstages 5

#include <math.h>

#define mip_env_rate_scale     10.0f // 30.0f
#define mip_env_threshold      0.001 //SAT_TINY

#define SAT_ENVELOPE_OFF       0
#define SAT_ENVELOPE_ATTACK    1
#define SAT_ENVELOPE_DECAY     2
#define SAT_ENVELOPE_SUSTAIN   3
#define SAT_ENVELOPE_RELEASE   4
#define SAT_ENVELOPE_FINISHED  5

//----------------------------------------------------------------------

template <class T>
struct SAT_EnvelopeStage {
  T target;
  T rate;
};

//----------------------------------------------------------------------

// We calculate the coefficients using the time-constant equation:
// g = e ^ ( -1 / (time * sample rate) ),
// out = in + g * (out  in),

//----------------------------------------------------------------------

template <class T>
class SAT_Envelope {

  private:

    T                     MSampleRate = 0;// = 44100;
    T                     MValue = 0;
    int32_t               MStage = 0;
    SAT_EnvelopeStage<T>  MStages[5]; // -,a,d,s,r

  public:

    SAT_Envelope() {
      //MScale = 50.0;//6.0;
      MStage = SAT_ENVELOPE_OFF;
      MValue = 0.0;
    }

    //----------

    void    setSampleRate(T ARate)  { MSampleRate = ARate; }
    T       getValue(void)          { return MValue; }
    int32_t getStage(void)          { return MStage; }

  public:

    void reset() {
      MStage = SAT_ENVELOPE_OFF;
      MValue = 0.0;
    }

    T calcRate(T ms) {
      SAT_Assert(MSampleRate > 0);
      T a = ms * mip_env_rate_scale; // 0..1 -> 0..25
      a = (a*a*a);
      T scale = MSampleRate / 44100.0;
      a *= scale;
      a += 1.0;
      return 1.0 / a;
    }

    //----------

    void setAttack(T AValue) {
      MStages[SAT_ENVELOPE_ATTACK].target = 1.0;
      MStages[SAT_ENVELOPE_ATTACK].rate   = calcRate(AValue);
    }

    void setDecay(T AValue) {
      MStages[SAT_ENVELOPE_DECAY].rate = calcRate(AValue);
    }

    void setSustain(T AValue) {
      MStages[SAT_ENVELOPE_DECAY].target = AValue;
      MStages[SAT_ENVELOPE_SUSTAIN].target = AValue;
      MStages[SAT_ENVELOPE_SUSTAIN].rate = 1.0;
    }

    void setRelease(T AValue) {
      MStages[SAT_ENVELOPE_RELEASE].target = 0.0;
      MStages[SAT_ENVELOPE_RELEASE].rate = calcRate(AValue);
    }

    //----------

    void setADSR(T a, T d, T s, T r) {
      setAttack(a);
      setDecay(d);
      setSustain(s);
      setRelease(r);
    }

    //----------

    void noteOn(void) {
      MStage = SAT_ENVELOPE_ATTACK;
      MValue = 0.0;

    }

    //----------

    void noteOff(void) {
      MStage = SAT_ENVELOPE_RELEASE;
    }

    //----------

    T process(void) {
      if (MStage == SAT_ENVELOPE_OFF) return 0.0;
      if (MStage == SAT_ENVELOPE_FINISHED) return 0.0;
      if (MStage == SAT_ENVELOPE_SUSTAIN) return MValue;
      T target = MStages[MStage].target;
      T rate   = MStages[MStage].rate;
      MValue += ( (target - MValue) * rate );
      if (fabs(target - MValue) <= mip_env_threshold) {
        MStage += 1;
      }
      return MValue;
    }

    //----------

    // http://www.kvraudio.com/forum/viewtopic.php?p=6515525#p6515525

    T processSteps(int32_t ASteps) {
      if (ASteps==0) return MValue;
      T result = process();
      T target = MStages[MStage].target;
      T rate   = MStages[MStage].rate;

      MValue += (target - MValue) * (1 - pow(1 - rate, ASteps));
      //MValue += (target - MValue) * (1 - SAT_PowF2(1 - rate, ASteps));

      return result;
    }

    //----------

};


//----------------------------------------------------------------------
#endif

/*

coeff = exp(-2 * pi * fc/fs)
coeff = exp(-1 / (tau * fs))
..where "tau" is the decay length in seconds

----------

delta = input - buffer
buffer = buffer + scale * delta

expr(n,r) = exp(ln(ln(r))-ln(ln(n)))
nroot(n,r) = n^(1/r)
pos(spd,smp) = (1-spd)^smp
spd(pos,smp) = 1-nroot(pos,smp)
smp(spd,pos) = expr(1-spd,pos)

expr()  = anti-power
nroot() = nth root
pos()   = position arrived at speed in samples
spd()   = speed required to arrive at position in samples
smp()   = samples required to arrive at position with speed

->

delta         = input - buffer
buffer        = buffer + scale * delta
pos(spd,smp)  = (1-spd)^smp
pos()         = position arrived at speed in samples

*/
