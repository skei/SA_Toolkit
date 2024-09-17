#ifndef sa_synth_flt_included
#define sa_synth_flt_included
//----------------------------------------------------------------------

#include "sat.h"
#include "audio/sat_audio_math.h"
#include "audio/filters/sat_svf_filter.h"

typedef SAT_SVFFilter<sat_sample_t> sa_synth_svf;

  /*
    gain:   Gain in dB to boost or cut the cutoff point of the Low shelf filter
    cutoff  cutoff frequency in Hz that should be clamped into the range [16hz, NYQUIST]
    Q       Q factor that should be clamped into the range [0.025f, 40.f]. Default value is 0.5    
  */

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_synth_flt {

//------------------------------
private:
//------------------------------

  sa_synth_svf  MFilter     = {};
  double        MSampleRate = 0.0;

  sat_param_t   par_type  = 0.0;
  sat_param_t   par_freq  = 0.0;  // 16..nyquist
  sat_param_t   par_q     = 0.0;  // 0.025..40 (default 0.5)
  sat_param_t   par_gain  = 0.0;  // db

//------------------------------
public:
//------------------------------

  void setSampleRate(double ASampleRate) {
    MFilter.resetState();
    MSampleRate = ASampleRate;
  }

  //----------

  void setType(sat_param_t AType) {
    par_type = AType;
  }

  void setFreq(sat_param_t AFreq) {
    par_freq = AFreq;
  }

  void setQ(sat_param_t AQ) {
    par_q = AQ;
  }

  void setGain(sat_param_t AGain) {
    par_gain = AGain;
  }

  void set(sat_param_t AType, sat_param_t AFreq, sat_param_t AQ, sat_param_t AGain) {
    par_type = AType;
    par_freq = AFreq;
    par_q = AQ;
    par_gain = AGain;
  }

  //----------

  void update_coeffs() {
    SAT_Assert(MSampleRate > 0.0);
    double nyquist = MSampleRate * 0.5;

    uint32_t type = par_type;

    // Gain in dB to boost or cut the cutoff point of the Low shelf filter
    sat_sample_t gain = SAT_VolumeToDb(par_gain);  

    // cutoff frequency in Hz that should be clamped into the range [16hz, NYQUIST]
    sat_sample_t freq = par_freq * nyquist;
    freq = SAT_Clamp(freq, 16.0, nyquist);

    // Q factor that should be clamped into the range [0.025f, 40.f]. Default value is 0.5    
    sat_sample_t q = par_q * 40.0;                 
    q = SAT_Clamp(q, 0.025, 40.0);

    //SAT_PRINT("gain %.2f freq %.2f q %.2f type %i srate %.3f\n",gain,freq,q,type,MSampleRate);
    MFilter.setGain(gain);
    MFilter.updateCoefficients(freq,q,type,MSampleRate);
  }

  //----------

  void reset(void) {
    MFilter.resetState();
  }

//------------------------------
public:
//------------------------------

  void noteOn(uint32_t AIndex, double AValue) {
    update_coeffs();
  }

  //----------

  void noteOff(uint32_t AIndex, double AValue) {
  }

  //----------

  void noteExpression(uint32_t AIndex, sat_param_t AValue) {
  }

  //----------

  sat_sample_t process(sat_sample_t AInput) {
    sat_sample_t out = MFilter.tick(AInput);
    return out;
  }

};

//----------------------------------------------------------------------
#endif
