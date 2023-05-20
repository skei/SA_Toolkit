#ifndef sa_compciter_included
#define sa_compciter_included
//----------------------------------------------------------------------

// based on the 'compciter' jesusonic effect by 'loser'

/*
  (C) 2007, Michael Gruhn.

  NO WARRANTY IS GRANTED.  THIS PLUG-IN  IS PROVIDED FOR FREE  ON  AN  "AS  IS"
  BASIS, WITHOUT WARRANTY OF ANY KIND.  NO LIABILITY IS GRANTED, INCLUDING, BUT
  NOT LIMITED TO, ANY DIRECT OR INDIRECT,  SPECIAL, INCIDENTAL OR CONSEQUENTIAL
  DAMAGE ARISING OUT OF THE USE  OR INABILITY  TO  USE  THIS PLUG-IN,  COMPUTER
  FAILTURE  OF  MALFUNCTION  INCLUDED.  THE  USE  OF THE  SOURCE  CODE,  EITHER
  PARTIALLY OR IN TOTAL, IS ONLY GRANTED,  IF USED IN THE SENSE OF THE AUTHOR'S
  INTENTION,  AND USED WITH ACKNOWLEDGEMENT OF THE AUTHOR.  LAST BUT NOT LEAST,
  BY USING THIS  PLUG-IN YOU RELINQUISH YOUR CLAIM TO SUE IT'S AUTHOR,  AS WELL
  AS YOUR CLAIM TO ENTRUST SOMEBODY ELSE WITH DOING SO.
*/

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#include "base/sat.h"
#include "audio/sat_audio_utils.h"
#include "plugin/clap/sat_clap.h"
#include "plugin/sat_parameter.h"
#include "plugin/sat_plugin.h"

//----------

//#define cDenorm   10e-30
#define c_ampdB     8.65617025
#define freqHP      2243.2
#define freqLP      224.32
#define freqHP_p2   (-SAT_PI2 * freqHP)
#define freqLP_p2   ( SAT_PI2 * freqLP)


//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t sa_compciter_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = SAT_VENDOR "/sa_compciter",
  .name         = "sa_compciter",
  .vendor       = SAT_VENDOR,
  .url          = SAT_URL,
  .manual_url   = "",
  .support_url  = "",
  .version      = SAT_VERSION,
  .description  = "",
  .features     = (const char*[]) {
                    CLAP_PLUGIN_FEATURE_AUDIO_EFFECT,
                    nullptr
                  }
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_compciter_plugin
: public SAT_Plugin {
  
//------------------------------
private:
//------------------------------

  bool  need_recalc           = true;
  float MSampleRate           = 0.0;
  
  float threshDB  = 0;
  float thresh    = 0;
  float ratio     = 0;
  float release   = 0;
  float t0        = 0;
  float t1        = 0;
  float blp       = 0;
  float alp       = 0;
  float wet       = 0;
  float dry       = 0;

  float t00       = 0;
  float t01       = 0;
  float t10       = 0;
  float t11       = 0;
  float t20       = 0;
  float t21       = 0;

  // @init

  float gain      = 1.0f;
  float seekgain  = 1.0f;
  float c         = 8.65617025;
  float dc        = 10^-30;

//------------------------------
public:
//------------------------------

  SAT_PLUGIN_DEFAULT_CONSTRUCTOR(sa_compciter_plugin)

  //----------
  
  bool init() final {
    registerDefaultExtensions();    
    appendStereoInputPort();
    appendStereoOutputPort();
    appendParameter( new SAT_Parameter( "Drive", 0,    0,   60    ));
    appendParameter( new SAT_Parameter( "Dist",  25,   0,   100   ));
    appendParameter( new SAT_Parameter( "HP",    5000, 800, 12000 ));
    appendParameter( new SAT_Parameter( "Wet",   -6,  -60,  24    ));
    appendParameter( new SAT_Parameter( "Dry",   0,   -120, 0     ));
    setAllParameterFlags(CLAP_PARAM_IS_MODULATABLE);
    need_recalc = true;
    return SAT_Plugin::init();
  }
  
  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    MSampleRate = sample_rate;
    return SAT_Plugin::activate(sample_rate,min_frames_count,max_frames_count);
  }

  //----------

  bool handleParamValue(const clap_event_param_value_t* param_value) final {
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
      
      float s0,s1;
      t00 = alp * spl0 - blp * t00;
      t01 = alp * spl1 - blp * t01;
      s0 = t00;
      s1 = t01;
      t10 = alp * s0 - blp * t10;
      t11 = alp * s1 - blp * t11;
      s0 = t10;
      s1 = t11;
      t20 = alp * s0 - blp * t20;
      t21 = alp * s1 - blp * t21;
      s0 = spl0 - t20;
      s1 = spl1 - t21;
      float rms = fmax( abs(spl0) , abs(spl1) );
      //seekgain = ((rms > thresh) ? exp((threshDB + (log(rms)*c-threshDB)*ratio) /c) / rms : 1);
      if (rms > thresh) seekgain = exp((threshDB + (log(rms)*c-threshDB)*ratio) /c) / rms;
      else seekgain = 1;
      //gain = ((gain > seekGain) ? seekGain : min( gain/release , seekGain ));
      if (gain > seekgain) {
        gain = seekgain;
      }
      else {
        gain = fmin( gain/release , seekgain );
      }
      spl0 = spl0*dry + s0 *gain * wet;
      spl1 = spl1*dry + s1 *gain * wet;

      *out0++ = spl0;
      *out1++ = spl1;
    }
  }
  
//------------------------------
private:
//------------------------------

  void recalc(float srate) {
    need_recalc = false;
    threshDB    = - getParameterValue(0);
    thresh      = exp(threshDB / c);
    ratio       = 1 / 20;
    release     = exp(-60 / (pow(1 - getParameterValue(1) / 100, 3) * 500 * srate / 1000) / c);
    t0          = 0;
    t1          = 0;
    blp         = -exp(-2 * SAT_PI * getParameterValue(2) * 3 / srate);
    alp         = 1.0 + blp;
    wet         = exp( getParameterValue(3) / c) / exp((threshDB - threshDB * ratio ) / c);
    dry         = exp( getParameterValue(4) / c);
    seekgain    = 1;
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#ifndef SAT_NO_ENTRY
  #include "plugin/sat_entry.h"
  SAT_PLUGIN_ENTRY(sa_compciter_descriptor,sa_compciter_plugin)
#endif



  //  inc = slider1/100;
  //  // len must be multiple of 2 otherwise there will be drift due to floating point math in 1sdft93hgosdh
  //  len = floor(slider2/1000*srate/2)*2;
  //  fade = slider3/100 * len/2;

//----------

//#undef cDenorm
#undef c_ampdB
#undef freqHP
#undef freqLP
#undef freqHP_p2
#undef freqLP_p2

//----------------------------------------------------------------------
#endif

