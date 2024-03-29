#ifndef sa_saturation_included
#define sa_saturation_included
//----------------------------------------------------------------------

/*
  based on the 'saturation' jesusonic effect by tale/loser
  http://forum.cockos.com/showpost.php?p=1537127&postcount=3
  see bottom of this file for original
*/

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#include "sat.h"
#include "audio/sat_audio_utils.h"
#include "plugin/clap/sat_clap.h"
#include "plugin/sat_parameters.h"
#include "plugin/sat_plugin.h"

#define scale_ ( 1.1 / 3.0 )

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t sa_saturation_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = SAT_VENDOR "/sa_saturation",
  .name         = "sa_port_saturation",
  .vendor       = SAT_VENDOR,
  .url          = SAT_URL,
  .manual_url   = "",
  .support_url  = "",
  .version      = SAT_VERSION,
  .description  = "",
  .features     = (const char*[]){ CLAP_PLUGIN_FEATURE_AUDIO_EFFECT, nullptr }
};


const char* txt_func[2] = { "sine", "cubic" };

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_saturation_plugin
: public SAT_Plugin {
  
//------------------------------
private:
//------------------------------

  bool  need_recalc = true;
  float MSampleRate = 0.0;

//  float     p_saturation;
//  uint32_t  p_stages;
//  uint32_t  p_function;
  float     a, div, gain;

//------------------------------
public:
//------------------------------

  SAT_DEFAULT_PLUGIN_CONSTRUCTOR(sa_saturation_plugin)

  //----------
  
  bool init() final {
    registerDefaultExtensions();    
    appendStereoAudioInputPort("In");
    appendStereoAudioOutputPort("Out");
    
    appendParameter(new SAT_Parameter(     "Saturation", 0,  0,   1 ));
    appendParameter(new SAT_IntParameter(  "Stages",     1,  1,   10 ));
    appendParameter(new SAT_TextParameter( "Function",   0,  0,   1, txt_func ));
    
    setAllParameterFlags(CLAP_PARAM_IS_MODULATABLE);
    //need_recalc = true;
    return SAT_Plugin::init();
  }
  
  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    MSampleRate = sample_rate;
    return SAT_Plugin::activate(sample_rate,min_frames_count,max_frames_count);
  }

  //----------

  bool on_plugin_paramValue(const clap_event_param_value_t* event) final {
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
      
      //----------
      if (a>0) {
        float x0 = SAT_Max(-1, SAT_Min(1, spl0));
        float x1 = SAT_Max(-1, SAT_Min(1, spl1));
        switch( (int)getParameterValue(2) ) {
          case 0:
            for (uint32_t j=0; j<getParameterValue(1); j++) {
              x0 = sin(x0 * a) * div;
              x1 = sin(x1 * a) * div;
            }
            break;
          case 1:
            for (uint32_t j=0; j<getParameterValue(1); j++) {
              x0 *= a; x0 = (x0 - x0*x0*x0 * scale_) * div;
              x1 *= a; x1 = (x1 - x1*x1*x1 * scale_) * div;
            }
            break;
        }
        spl0 = x0 * gain;
        spl1 = x1 * gain;
      } // a>0
      //----------

      *out0++ = spl0;
      *out1++ = spl1;
    }
  }
  
//------------------------------
private:
//------------------------------

  void recalc(float srate) {
    //a = SAT_Max(0, SAT_Min(1, p_saturation));
    a = SAT_Max(0, SAT_Min(1, getParameterValue(0)));
    if (a > 0) {
      gain = 1 - (1 - 1/(float)getParameterValue(1)) * a;
      float f = 1.0f;
      switch( (int)getParameterValue(2) ) {
        case 0:
          a *= SAT_PI05;
          f = sin(a);
          break;
        case 1:
          f = a - a*a*a * scale_;
          break;
      }
      div = 1 / f;
    }
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
  SAT_PLUGIN_ENTRY(sa_saturation_descriptor,sa_saturation_plugin)
#endif

//----------

#undef scale_

//----------------------------------------------------------------------
#endif





//  desc:Another loser/Saturation rewrite, thanks to SaulT!
//  slider1:0<0,1,0.01>Saturation
//  slider2:1<1,10,1>Stages
//  slider3:1<0,1,1{Sine,Cubic}>Function
//
//  // Sine:  f(x) = sin(x * a * 0.5 * pi)/sin(a * 0.5 * pi)
//  // Cubic: f(x) = ((x*a)-((x*a)^3)/3)/(a-(a^3)/3)
//
//  @init
//
//  scale = 1.1 / 3;
//
//  @slider
//
//  a = max(0, min(1, slider1));
//  a > 0 ? (
//    n = max(1, floor(slider2 + 0.5));
//    f = slider3 < 0.5;
//
//    gain = 1 - (1 - 1/n) * a;
//    div = 1/(f ? sin(a *= 0.5*$pi) : a - a*a*a * scale);
//  );
//
//  @sample
//
//  a > 0 ? (
//    x0 = max(-1, min(1, spl0));
//    x1 = max(-1, min(1, spl1));
//
//    f ? loop(n,
//      x0 = sin(x0 * a) * div;
//      x1 = sin(x1 * a) * div;
//    ) : loop(n,
//      x0 *= a; x0 = (x0 - x0*x0*x0 * scale) * div;
//      x1 *= a; x1 = (x1 - x1*x1*x1 * scale) * div;
//    );
//
//    spl0 = x0 * gain;
//    spl1 = x1 * gain;
//  );
