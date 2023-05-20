#ifndef sa_iirfilter_included
#define sa_iirfilter_included
//----------------------------------------------------------------------


//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------


#include "base/sat.h"
#include "audio/sat_audio_math.h"
#include "audio/sat_audio_utils.h"
#include "plugin/clap/sat_clap.h"
#include "plugin/sat_parameter.h"
#include "plugin/sat_plugin.h"

const char* filter_types_txt[] = {
  "Low pass",
  "High pass",
  "Band pass 1",
  "Band pass 2",
  "Allpass",
  "Notch",
  "RIAA record/tape de-emphasis",
  "Peaking band EQ",
  "Bass boost",
  "Low shelf",
  "High shelf",
  "RIAA CD de-emphasis"
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#include <stddef.h>
//#ifdef __SSE2__
#include <emmintrin.h>
//#endif

#define sqr(a) ((a) * (a))

/* filter types */
enum {
  LPF,        /* low pass filter */
  HPF,        /* High pass filter */
  BPCSGF,     /* band pass filter 1 */
  BPZPGF,     /* band pass filter 2 */
  APF,        /* Allpass filter*/
  NOTCH,      /* Notch Filter */
  RIAA_phono, /* RIAA record/tape deemphasis */
  PEQ,        /* Peaking band EQ filter */
  BBOOST,     /* Bassboost filter */
  LSH,        /* Low shelf filter */
  HSH,        /* High shelf filter */
  RIAA_CD     /* CD de-emphasis */
};

//----------

class SAT_IIRFilter {

private:

  //#ifdef __SSE2__

  __SAT_ALIGNED(SAT_ALIGNMENT_SIMD)

  __m128 fir_coeff[2]     = {0};
  __m128 fir_buf[2]       = {0};
  __m128 iir_coeff        = {0};
  __m128 iir_buf          = {0};
  //#endif

  float p_freq            = 0.5;
  float p_qfact           = 0.5;
  float p_gain            = 0.5;
  int   p_mode            = 0;

//int   type              = 0;
//int   p_q_is_bandwidth  = 0;

  float xn1               = 0.0;
  float xn2               = 0.0;
  float yn1               = 0.0;
  float yn2               = 0.0;

  float omega             = 0.0;
  float cs                = 0.0;
  float a1pha             = 0.0;
  float beta              = 0.0;
  float b0                = 0.0;
  float b1                = 0.0;
  float b2                = 0.0;
  float a0                = 0.0;
  float a1                = 0.0;
  float a2                = 0.0;
  float A                 = 0.0;
  float sn                = 0.0;

public:

  SAT_IIRFilter() {
  }

  ~SAT_IIRFilter() {
  }

private:

  //lynched from SoX >w>
  void make_poly_from_roots(double const * roots, size_t num_roots, float * poly) {
    size_t i, j;
    poly[0] = 1;
    poly[1] = -roots[0];
    memset(poly + 2, 0, (num_roots + 1 - 2) * sizeof(*poly));
    for (i = 1; i < num_roots; ++i)
      for (j = num_roots; j > 0; --j)
        poly[j] -= poly[j - 1] * roots[i];
  }

//----------
public:
//----------

  void setMode(int val) {
    p_mode = val;
  }

  //----------

  void setFrequency(float val) {
    p_freq = val;
  }

  //----------

  void setQuality(float val) {
    p_qfact = val;
  }

  //----------

  void setGain(float val) {
    p_gain = val;
  }

  //----------

  //void init(int samplerate, int filter_type) {
  void init(int samplerate) {
    xn1 = 0.0;
    xn2 = 0.0;
    yn1 = 0.0;
    yn2 = 0.0;
    omega = 2.0 * SAT_PI * p_freq/samplerate;
    cs = cos(omega);
    sn = sin(omega);
    a1pha = sn / (2.0 * p_qfact);
    A = exp(log(10.0) * p_gain  / 40.0);
    beta = sqrt(A + A);
    //Set up filter coefficients according to type
    //switch (filter_type) {
    switch (p_mode) {
      case LPF:
        b0 =  (1.0 - cs) / 2.0 ;
        b1 =   1.0 - cs ;
        b2 =  (1.0 - cs) / 2.0 ;
        a0 =   1.0 + a1pha ;
        a1 =  -2.0 * cs ;
        a2 =   1.0 - a1pha ;
        break;
      case HPF:
        b0 =  (1.0 + cs) / 2.0 ;
        b1 = -(1.0 + cs) ;
        b2 =  (1.0 + cs) / 2.0 ;
        a0 =   1.0 + a1pha ;
        a1 =  -2.0 * cs ;
        a2 =   1.0 - a1pha ;
        break;
      case APF:
        b0 =  1.0 - a1pha;
        b1 = -2.0 * cs;
        b2 =  1.0 + a1pha;
        a0 =  1.0 + a1pha;
        a1 = -2.0 * cs;
        a2 =  1.0 - a1pha;
        break;
      case BPZPGF:
        b0 =   a1pha ;
        b1 =   0.0 ;
        b2 =  -a1pha ;
        a0 =   1.0 + a1pha ;
        a1 =  -2.0 * cs ;
        a2 =   1.0 - a1pha ;
        break;
      case BPCSGF:
        b0=sn/2.0;
        b1=0.0;
        b2=-sn/2;
        a0=1.0+a1pha;
        a1=-2.0*cs;
        a2=1.0-a1pha;
        break;
      case NOTCH:
        b0 = 1.0;
        b1 = -2.0 * cs;
        b2 = 1.0;
        a0 = 1.0 + a1pha;
        a1 = -2.0 * cs;
        a2 = 1.0 - a1pha;
        break;
      case RIAA_phono: /* http://www.dsprelated.com/showmessage/73300/3.php */
        if (samplerate == 44100) {
          static const double zeros[] = {-0.2014898, 0.9233820};
          static const double poles[] = {0.7083149, 0.9924091};
          make_poly_from_roots(zeros, (size_t)2, &b0);
          make_poly_from_roots(poles, (size_t)2, &a0);
        }
        else if (samplerate == 48000) {
          static const double zeros[] = {-0.1766069, 0.9321590};
          static const double poles[] = {0.7396325, 0.9931330};
          make_poly_from_roots(zeros, (size_t)2, &b0);
          make_poly_from_roots(poles, (size_t)2, &a0);
        }
        else if (samplerate == 88200) {
          static const double zeros[] = {-0.1168735, 0.9648312};
          static const double poles[] = {0.8590646, 0.9964002};
          make_poly_from_roots(zeros, (size_t)2, &b0);
          make_poly_from_roots(poles, (size_t)2, &a0);
        }
        else if (samplerate == 96000) {
          static const double zeros[] = {-0.1141486, 0.9676817};
          static const double poles[] = {0.8699137, 0.9966946};
          make_poly_from_roots(zeros, (size_t)2, &b0);
          make_poly_from_roots(poles, (size_t)2, &a0);
        }
        { /* Normalise to 0dB at 1kHz (Thanks to Glenn Davis) */
          double y = 2.0 * SAT_PI * 1000.0 / samplerate ;
          double b_re = b0 + b1 * cos(-y) +b2 * cos(-2.0 * y);
          double a_re = a0 + a1 * cos(-y) + a2 * cos(-2.0 * y);
          double b_im = b1 * sin(-y) + b2 * sin(-2.0 * y);
          double a_im = a1 * sin(-y) + a2 * sin(-2.0 * y);
          double g = 1.0 / sqrt((sqr(b_re) + sqr(b_im)) / (sqr(a_re) + sqr(a_im)));
          b0 *= g; b1 *= g; b2 *= g;
        }
        break;
      case PEQ:
        b0 =   1.0 + a1pha * A ;
        b1 =  -2.0 * cs ;
        b2 =   1.0 - a1pha * A ;
        a0 =   1.0 + a1pha / A ;
        a1 =  -2.0 * cs ;
        a2 =   1.0 - a1pha / A ;
        break;
      case BBOOST:
        beta = sqrt((A * A + 1) / 1.0 - (pow((A - 1), 2)));
        b0 = A * ((A + 1) - (A - 1) * cs + beta * sn);
        b1 = 2 * A * ((A - 1) - (A + 1) * cs);
        b2 = A * ((A + 1) - (A - 1) * cs - beta * sn);
        a0 = ((A + 1) + (A - 1) * cs + beta * sn);
        a1 = -2 * ((A - 1) + (A + 1) * cs);
        a2 = (A + 1) + (A - 1) * cs - beta * sn;
        break;
      case LSH:
        b0 = A * ((A + 1) - (A - 1) * cs + beta * sn);
        b1 = 2 * A * ((A - 1) - (A + 1) * cs);
        b2 = A * ((A + 1) - (A - 1) * cs - beta * sn);
        a0 = (A + 1) + (A - 1) * cs + beta * sn;
        a1 = -2 * ((A - 1) + (A + 1) * cs);
        a2 = (A + 1) + (A - 1) * cs - beta * sn;
        break;
      case RIAA_CD:
        omega = 2 * SAT_PI * 5283/samplerate;
        cs = cos(omega);
        sn = sin(omega);
        a1pha = sn / (2.0 * 0.4845);
        A = exp(log(10.0) * -9.477  / 40);
        beta = sqrt(A + A);
        // SKEI: no break?
      case HSH:
        b0 = A * ((A + 1) + (A - 1) * cs + beta * sn);
        b1 = -2 * A * ((A - 1) + (A + 1) * cs);
        b2 = A * ((A + 1) + (A - 1) * cs - beta * sn);
        a0 = (A + 1) - (A - 1) * cs + beta * sn;
        a1 = 2 * ((A - 1) - (A + 1) * cs);
        a2 = (A + 1) - (A - 1) * cs - beta * sn;
        break;
      default:
        break;
    }
    //#ifdef __SSE2__
    fir_coeff[0] = _mm_set_ps(b1 / a0, b1 / a0, b0 / a0, b0 / a0);
    fir_coeff[1] = _mm_set_ps(0.0f, 0.0f, b2 / a0, b2 / a0);
    iir_coeff = _mm_set_ps(-a2 / a0, -a2 / a0, -a1 / a0, -a1 / a0);
    //#endif
  }

  //----------

  float process(float samp) {
    float out, in = 0;
    in = samp;
    out = (b0 * in + b1 * xn1 + b2 * xn2 - a1 * yn1 - a2 * yn2) / a0;
    xn2 = xn1;
    xn1 = in;
    yn2 = yn1;
    yn1 = out;
    return out;
  }

  //----------

  //#ifdef __SSE2__

  void processBlock(float *out, const float *in, unsigned frames) {
    __m128 fir_coeff[2] = { this->fir_coeff[0], this->fir_coeff[1] };
    __m128 iir_coeff    = this->iir_coeff;
    __m128 fir_buf[2]   = { this->fir_buf[0],   this->fir_buf[1] };
    __m128 iir_buf      = this->iir_buf;
    for (unsigned i = 0; (i + 4) <= (2 * frames); in += 4, i += 4, out += 4) {
      __m128 input = _mm_loadu_ps(in);
      fir_buf[1] = _mm_shuffle_ps(fir_buf[0], fir_buf[1],  _MM_SHUFFLE(1, 0, 3, 2));
      fir_buf[0] = _mm_shuffle_ps(input, fir_buf[0], _MM_SHUFFLE(1, 0, 1, 0));
      __m128 res[3] = {
        _mm_mul_ps(fir_buf[0], fir_coeff[0]),
        _mm_mul_ps(fir_buf[1], fir_coeff[1]),
        _mm_mul_ps(iir_buf, iir_coeff),
      };
      __m128 result = _mm_add_ps(_mm_add_ps(res[0], res[1]), res[2]);
      result = _mm_add_ps(result, _mm_shuffle_ps(result, result, _MM_SHUFFLE(0, 0, 3, 2)));
      iir_buf = _mm_shuffle_ps(result, iir_buf, _MM_SHUFFLE(1, 0, 1, 0));
      fir_buf[1] = _mm_shuffle_ps(fir_buf[0], fir_buf[1],  _MM_SHUFFLE(1, 0, 3, 2));
      fir_buf[0] = _mm_shuffle_ps(input, fir_buf[0], _MM_SHUFFLE(1, 0, 3, 2));
      res[0] = _mm_mul_ps(fir_buf[0], fir_coeff[0]);
      res[1] = _mm_mul_ps(fir_buf[1], fir_coeff[1]);
      res[2] = _mm_mul_ps(iir_buf, iir_coeff);
      __m128 result2 = _mm_add_ps(_mm_add_ps(res[0], res[1]), res[2]);
      result2 = _mm_add_ps(result2, _mm_shuffle_ps(result2, result2, _MM_SHUFFLE(0, 0, 3, 2)));
      iir_buf = _mm_shuffle_ps(result2, iir_buf, _MM_SHUFFLE(1, 0, 1, 0));
      _mm_store_ps(out, _mm_shuffle_ps(result, result2, _MM_SHUFFLE(1, 0, 1, 0)));
    }
    this->fir_buf[0] = fir_buf[0];
    this->fir_buf[1] = fir_buf[1];
    this->iir_buf    = iir_buf;
  }

  //#endif // __SSE2__

};

//----------

#undef sqr

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t sa_iirfilter_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = SAT_VENDOR "/sa_iirfilter",
  .name         = "sa_iirfilter",
  .vendor       = SAT_VENDOR,
  .url          = SAT_URL,
  .manual_url   = "",
  .support_url  = "",
  .version      = SAT_VERSION,
  .description  = "",
  .features     = (const char*[]) {
                    CLAP_PLUGIN_FEATURE_AUDIO_EFFECT,
                    CLAP_PLUGIN_FEATURE_FILTER,
                    nullptr }
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_iirfilter_plugin
: public SAT_Plugin {
  
//------------------------------
private:
//------------------------------

  bool  need_recalc = true;
  float MSampleRate = 0.0;

  SAT_IIRFilter MLFilter    = {};
  SAT_IIRFilter MRFilter    = {};

//------------------------------
public:
//------------------------------

  SAT_PLUGIN_DEFAULT_CONSTRUCTOR(sa_iirfilter_plugin)

  //----------
  
  bool init() final {
    registerDefaultExtensions();    
    appendStereoInputPort();
    appendStereoOutputPort();
    
    appendParameter( new SAT_TextParameter( "Mode", 0,      0,   11, filter_types_txt ));
    appendParameter( new SAT_Parameter(     "Freq", 1024,   50,  16000 ));
    appendParameter( new SAT_Parameter(     "Q",    0.707,  0.1, 1 ));
    appendParameter( new SAT_Parameter(     "Gain", 0,      -50, 50 ));
    
    setAllParameterFlags(CLAP_PARAM_IS_MODULATABLE);
    //need_recalc = true;
    return SAT_Plugin::init();
  }
  
  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    MSampleRate = sample_rate;
    need_recalc = true;
    return SAT_Plugin::activate(sample_rate,min_frames_count,max_frames_count);
  }

  //----------

  bool handleParamValue(const clap_event_param_value_t* param_value) final {
    uint32_t index = param_value->param_id;
    double   value = param_value->value;
    switch (index) {
      case 0: MLFilter.setMode(value);      MRFilter.setMode(value);      break;
      case 1: MLFilter.setFrequency(value); MRFilter.setFrequency(value); break;
      case 2: MLFilter.setQuality(value);   MRFilter.setQuality(value);   break;
      case 3: MLFilter.setGain(value);      MRFilter.setGain(value);      break;
    }
    need_recalc = true;
    return true;
  }
  
  //----------

  void processAudio(SAT_ProcessContext* AContext) final {
    const clap_process_t* process = AContext->process;
    if (need_recalc) recalc(MSampleRate);
    uint32_t len = process->frames_count;
    float* in0  = process->audio_inputs[0].data32[0];
    float* in1  = process->audio_inputs[0].data32[1];
    float* out0 = process->audio_outputs[0].data32[0];
    float* out1 = process->audio_outputs[0].data32[1];
    for (uint32_t i=0; i<len; i++) {
      float spl0 = *in0++;
      float spl1 = *in1++;
      
      spl0 = MLFilter.process(spl0);
      spl1 = MRFilter.process(spl1);

      *out0++ = spl0;
      *out1++ = spl1;
    }
  }
  
//------------------------------
private:
//------------------------------

  void recalc(float srate) {
    MLFilter.init(srate);
    MRFilter.init(srate);
    need_recalc = false;
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#ifndef SAT_NO_ENTRY
  #include "plugin/sat_entry.h"
  SAT_PLUGIN_ENTRY(sa_iirfilter_descriptor,sa_iirfilter_plugin)
#endif

//----------


//----------------------------------------------------------------------
#endif

