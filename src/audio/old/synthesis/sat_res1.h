#ifndef mip_res1_included
#define mip_res1_included
//----------------------------------------------------------------------

#include "common/mip_interpolate.h"
#include "common/mip_math.h"
#include "common/mip_random.h"
#include "audio/_backup/synthesis/mip_waveguide.h"
//#include "audio/_backup/waveforms/mip_waveform_polyblep.h"
#include "audio/_backup/filters/mip_dc_filter.h"
#include "audio/_backup/filters/mip_rc_filter.h"


//----------

// resonator type

#define krt_off        0
#define krt_resonator  1
#define krt_pluck      2

//----------

#define CLAMP(x,a,b) x = KClamp(x,a,b)

//----------------------------------------------------------------------


class MIP_Res1 {

  private:

    MIP_RcFilter   nshape;
    MIP_DcFilter   dc;
    MIP_WaveGuide  wg1;
    //KWaveGuide      wg2;
    float           MSampleRate;
    float           MInvRate;

    float w1,w2;

    float         _hz1;
    uint32_t        _os;
    uint32_t        _type;      // krt_off, krt_resonator, krt_pluck
    float         _noise;     // 0..1
    float         _nshape;    // 0..1
    float         _fb;        // 0..1
    float         _damp;      // 0..1
    float         _tune;      // 0.01..2
    float         _mix;       // 0..1

    float         _tune_oct;
    float         _tune_semi;
    float         _tune_fine;

  public:

    MIP_Res1() {
      _hz1    = 0.0f;
      _os     = 1;
      _type   = krt_off;
      _noise  = 0.0f;
      _nshape = 0.0f;
      _fb     = 0.0f;
      _damp   = 0.0f;
      _tune   = 0.0f; // 1.0f
      _mix    = 0.0f;
    }

  public:


  public:

    void reset(void) {
    }

    void noteOn(void) {
      wg1.clear();
      //wg2.clear();
    }

    void setSampleRate(float srate) {
      MSampleRate = srate;
      MInvRate = 1.0f / MSampleRate;
      nshape.setSampleRate(srate);
      wg1.sampleRate(srate);
      //wg2.sampleRate(srate);
    }

    void setDamping(float d) {
      _damp = d;//KClamp(d,0,1);
      wg1.damp_lo(_damp);
      //wg2.damp_lo(_damp);
    }

    void setImpulse(bool i) {
      wg1.impulse(i);
      //wg2.impulse(i);
    }

    void setFreq(float hz)        { _hz1        = hz;   }
    void setOversample(float os)  { _os         = os;   }
    void setType(uint32_t type)     { _type       = type; }
    void setNoise(float n)        { _noise      = n;    } //KClamp(n,   0,    1); }
    void setNShape(float ns)      { _nshape     = ns;   } //KClamp(ns,  0,    1); }
    void setFeedback(float fb)    { _fb         = fb;   } //KClamp(fb,  0,    1); }
    void setTuneOct(float v)      { _tune_oct   = v;    }
    void setTuneSemi(float v)     { _tune_semi  = v;    }
    void setTuneFine(float v)     { _tune_fine  = v;    }
    void setTune(float tune)      { _tune       = tune; } //KClamp(tune,0.01, 2);
    void setMix(float m)          { _mix        = m;    } //KClamp(m,0,1);

    //----------

    float process(float AInput) {

      float v = AInput;
      if (_type != krt_off) {

      float len,fb;
      float w;
      //float v1,v2;
      float rnd = MIP_RandomSigned();
      nshape.setWeight( 1 - MIP_Curve(_nshape,0.9) );
      rnd = nshape.process(rnd);
      float nv = v + rnd * _noise;
      wg1.damp_lo(1-_damp);
      //wg2.damp_lo(1-_damp);

//      switch(_type) {
//        case krt_resonator:
//          len = MSampleRate / (_hz1*(KMax(0.01f,_tune)));
//          len *= _os;
//          fb = 1 - _fb;
//          fb = (fb*fb*fb);
//          fb = 1 - fb;
//          //w1 = wg1.feed(nv+w2,fb,len); // 1-sample delay
//          //w2 = wg2.feed(w1,fb,len);
//          //v1 = wg1.tap(len,0.1);
//          //v2 = wg2.tap(len,0.9);
//          //w = v1 + v2;
//          w = wg1.feed(nv,fb,len);
//          //v = KInterpolate_Linear( _mix, nv, w );
//          v = KInterpolate_Linear( _mix, v, w );
//          v = dc.process(v);  // ???
//          break;
//        case krt_pluck:

          //float note = (_tune_oct * 12) + _tune_semi + _tune_fine;// + _tune;
          //float hz = KNoteToHz(note);
          //len = MSampleRate / hz;

          //len = MSampleRate / (_hz1*(KMax(0.01f,_tune)));

          float h = MIP_Clamp(_hz1,20,MSampleRate*0.5);

          len = MSampleRate / h;//(_hz1*(KMax(0.01f,_tune)));

          len *= _os;
          fb = 1 - _fb;
          fb = (fb*fb*fb);
          fb = 1 - fb;
          w = wg1.feed(nv,fb,len);
          v = MIP_Interpolate_Linear( _mix, v, w );
          v = dc.process(v);  // ???
//          break;
//      }

      } // !off

      return v;
    }

};

//----------------------------------------------------------------------
#endif

