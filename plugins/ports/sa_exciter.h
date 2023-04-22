#ifndef sa_exciter_included
#define sa_exciter_included
//----------------------------------------------------------------------

/*

  based on the 'exciter' jesusonic plugin by 'loser'

  (C) 2007, Michael Gruhn.
  NO WARRANTY IS GRANTED. THIS PLUG-IN IS PROVIDED ON AN "AS IS" BASIS, WITHOUT
  WARRANTY OF ANY KIND. NO LIABILITY IS GRANTED, INCLUDING, BUT NOT LIMITED TO,
  ANY DIRECT OR INDIRECT,  SPECIAL,  INCIDENTAL OR CONSEQUENTIAL DAMAGE ARISING
  OUT OF  THE  USE  OR INABILITY  TO  USE  THIS PLUG-IN,  COMPUTER FAILTURE  OF
  MALFUNCTION INCLUDED.  THE USE OF THE SOURCE CODE,  EITHER  PARTIALLY  OR  IN
  TOTAL, IS ONLY GRANTED,  IF USED IN THE SENSE OF THE AUTHOR'S INTENTION,  AND
  USED WITH ACKNOWLEDGEMENT OF THE AUTHOR. FURTHERMORE IS THIS PLUG-IN A  THIRD
  PARTY CONTRIBUTION,  EVEN IF INCLUDED IN REAPER(TM),  COCKOS INCORPORATED  OR
  ITS AFFILIATES HAVE NOTHING TO DO WITH IT.  LAST BUT NOT LEAST, BY USING THIS
  PLUG-IN YOU RELINQUISH YOUR CLAIM TO SUE IT'S AUTHOR, AS WELL AS THE CLAIM TO
  ENTRUST SOMEBODY ELSE WITH DOING SO.

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

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t sa_exciter_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = SAT_VENDOR "/sa_exciter",
  .name         = "sa_exciter",
  .vendor       = SAT_VENDOR,
  .url          = SAT_URL,
  .manual_url   = "",
  .support_url  = "",
  .version      = SAT_VERSION,
  .description  = "",
  .features     = (const char*[]){ CLAP_PLUGIN_FEATURE_AUDIO_EFFECT, nullptr }
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_exciter_plugin
: public SAT_Plugin {
  
//------------------------------
private:
//------------------------------

  bool  need_recalc = true;
  float MSampleRate = 0.0;
  
  float tmplONE     = 0;
  float tmprONE     = 0;
  float tmplTWO     = 0;
  float tmprTWO     = 0;

  float c_ampDB     = 8.65617025;
  float cDenorm     = 10^-30;
  float clipBoost   = 0;
  float mixBack     = 0;
  float hdistr      = 0;
  float foo         = 0;
  float freq        = 0;
  float x           = 0;
  float a0          = 0;
  float b1          = 0;

//------------------------------
public:
//------------------------------

  SAT_PLUGIN_DEFAULT_CONSTRUCTOR(sa_exciter_plugin)

  //----------
  
  bool init() final {
    registerDefaultExtensions();    
    appendStereoInputPort();
    appendStereoOutputPort();
    appendParameter(new SAT_Parameter( "Freq",   2000,  100, 18000 ));
    appendParameter(new SAT_Parameter( "Boost",  0,     0,   6     ));
    appendParameter(new SAT_Parameter( "Harm",   0,     0,   100   ));
    appendParameter(new SAT_Parameter( "Mix",   -6,    -120, 0     ));
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
      
      float s0 = spl0;
      float s1 = spl1;

      tmplONE = a0*s0 - b1*tmplONE;// + cDenorm;
      tmprONE = a0*s1 - b1*tmprONE;// + cDenorm;

      //TODO: denorm

      s0 -= tmplONE;
      s1 -= tmprONE;;
      s0 = SAT_Min(SAT_Max(s0*clipBoost,-1),1);
      s1 = SAT_Min(SAT_Max(s1*clipBoost,-1),1);
      s0 = (1+foo) * s0 / (1 + foo * abs(spl0));
      s1 = (1+foo) * s1 / (1 + foo * abs(spl1));

      tmplTWO = a0*s0 - b1*tmplTWO;// + cDenorm
      tmprTWO = a0*s1 - b1*tmprTWO;// + cDenorm;

      //TODO: denorm

      s0 -= tmplTWO;
      s1 -= tmprTWO;

      spl0 += (s0 * mixBack);
      spl1 += (s1 * mixBack);

      *out0++ = spl0;
      *out1++ = spl1;
    }
  }
  
//------------------------------
private:
//------------------------------

  void recalc(float srate) {
    need_recalc = false;
    clipBoost = exp(getParameterValue(1) / c_ampDB);
    mixBack   = exp(getParameterValue(3) / c_ampDB);
    hdistr    = SAT_Min(getParameterValue(2) / 100.0f, 0.9f);
    foo       = 2.0f * hdistr / (1.0f - hdistr);
    freq      = SAT_Min(getParameterValue(0),srate);
    x         = exp(-2.0f * SAT_PI * freq / srate);
    a0        = 1.0f - x;
    b1        = -x;
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#ifndef SAT_NO_ENTRY
  #include "plugin/sat_entry.h"
  SAT_PLUGIN_ENTRY(sa_exciter_descriptor,sa_exciter_plugin)
#endif

//----------


//----------------------------------------------------------------------
#endif









