#ifndef sa_tilteq_included
#define sa_tilteq_included
//----------------------------------------------------------------------

// (C) lubomir i. ivanov
// used with permission

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#include "plugin/sat_plugin.h"
#include "plugin/processor/sat_interleaved_processor.h"
//#include "audio/sat_audio_utils.h"

// #include "sat.h"
// #include "audio/sat_audio_utils.h"
#include "plugin/sat_parameters.h"
// #include "plugin/sat_plugin.h"

const char* sa_tilteq_process_mode_txt[2] = {
  "Stereo", "Mono"
};

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t sa_tilteq_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = SAT_VENDOR "/sa_tilteq/v0",
  .name         = "sa_tilteq",
  .vendor       = SAT_VENDOR,
  .url          = SAT_URL,
  .manual_url   = "",
  .support_url  = "",
  .version      = SAT_VERSION,
  .description  = "",
  .features     = (const char*[]){ CLAP_PLUGIN_FEATURE_AUDIO_EFFECT, CLAP_PLUGIN_FEATURE_FILTER, nullptr }
};

//----------------------------------------------------------------------
//
// processor
//
//----------------------------------------------------------------------

class sa_tilteq_processor
: public SAT_InterleavedProcessor {

//------------------------------
private:
//------------------------------

  bool  need_recalc = true;
  float samplerate = 0.0;

  float amp = 6/log(2); //Ln
  float lp_out    = 0;
  float lp_out_r  = 0;
  float a0        = 0;
  float b1        = 0;

  float outgain   = 0;
  float lgain     = 0;
  float hgain     = 0;
  uint32_t mono   = 0;

  float sr3       = 0;
  
//------------------------------
public:
//------------------------------

  sa_tilteq_processor(SAT_ProcessorOwner* AOwner)
  : SAT_InterleavedProcessor(AOwner) {
  }

  //----------

  virtual ~sa_tilteq_processor() {
  }

//------------------------------
public:
//------------------------------

  void setSampleRate(double ASampleRate) {
    samplerate = ASampleRate;
  }

//------------------------------
public:
//------------------------------

  void paramValueEvent(const clap_event_param_value_t* event) final {
    need_recalc = true;
  }

  //----------

  void processAudio(SAT_ProcessContext* AContext, uint32_t AOffset, uint32_t ALength) override {
    const clap_process_t* process = AContext->process;
    if (need_recalc) recalc(samplerate);

    sr3 = 3 * samplerate;

    float* input0  = process->audio_inputs[0].data32[0]  + AOffset;
    float* input1  = process->audio_inputs[0].data32[1]  + AOffset;
    float* output0 = process->audio_outputs[0].data32[0] + AOffset;
    float* output1 = process->audio_outputs[0].data32[1] + AOffset;
    for (uint32_t i=0; i<ALength; i++) {
      float spl0 = *input0++;
      float spl1 = *input1++;

      float inp,outp, inp_r,outp_r;
      
      if (mono==1) {
        //process mono
        inp = (spl0+spl1)*0.5;
        lp_out = a0*inp + b1*lp_out;
        outp = inp + lgain*lp_out + hgain*(inp - lp_out);
        spl0 = outp*outgain;
        spl1 = outp*outgain;
      }
      else {
        //process stereo
        inp = spl0;
        lp_out = a0*inp + b1*lp_out;
        outp = inp + lgain*lp_out + hgain*(inp - lp_out);
        spl0 = outp*outgain;

        inp_r = spl1;
        lp_out_r = a0*inp_r + b1*lp_out_r;
        outp_r = inp_r + lgain*lp_out_r + hgain*(inp_r - lp_out_r);
        spl1 = outp_r*outgain;
      }

      *output0++ = spl0;
      *output1++ = spl1;
    }    
  }

//------------------------------
private:
//------------------------------

  void recalc(float srate) {
    float sx,f0,omega,n,gain,gfactor,g1,g2;

    need_recalc = false;

    //switch(index) {
    //  case 0: // slider1
        mono = SAT_Trunc(getParameterValue(0));
    //    break;
    //  case 1: // slider2
        sx = 16 + getParameterValue(1) * 1.20103;
        f0 = floor(exp(sx*log(1.059))*8.17742);
        //f0 := trunc(Exp(sx*ln(1.059))*8.17742); // log(x), log2? orig had Logf
        omega = 2*SAT_PI*f0;
        n = 1/(sr3 + omega); // sr3 = 0 in constructor...
        a0 = 2*omega*n;
        b1 = (sr3 - omega)*n;
    //    break;
    //  case 2: // slider3
        gain = getParameterValue(2);
        gfactor = 4;
        if (gain>0) {
          g1 = -gfactor*gain;
          g2 = gain;
        }
        else {
          g1 = -gain;
          g2 = gfactor*gain;
        }
        lgain = exp(g1/amp)-1;
        hgain = exp(g2/amp)-1;
    //    break;
    //  case 3: // slider4
        outgain = exp(getParameterValue(3)/amp);
    //    break;
    //}
  }

};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class sa_tilteq_plugin
: public SAT_Plugin {
  
//------------------------------
private:
//------------------------------

  sa_tilteq_processor* MProcessor = nullptr;

//------------------------------
public:
//------------------------------

  sa_tilteq_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : SAT_Plugin(ADescriptor,AHost) {
  }

  //----------

  virtual ~sa_tilteq_plugin() {
  }

//------------------------------
public:
//------------------------------

  bool init() final {
    registerDefaultExtensions();    
    appendStereoAudioInputPort("In");
    appendStereoAudioOutputPort("Out");
    uint32_t flags = CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_MODULATABLE;
    appendParameter(new SAT_TextParameter( "Processing",  "", 0,  0,   1, flags, sa_tilteq_process_mode_txt ));
    appendParameter(new SAT_Parameter(     "Center Freq", "", 50, 0,   100, flags ));
    appendParameter(new SAT_Parameter(     "Tilt",        "", 0, -6,   6, flags ));
    appendParameter(new SAT_Parameter(     "Gain",        "", 0, -25,  25, flags ));
    MProcessor = new sa_tilteq_processor(this);
    setProcessor(MProcessor);
    return SAT_Plugin::init();
  }
  
  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    MProcessor->setSampleRate(sample_rate);
    return SAT_Plugin::activate(sample_rate,min_frames_count,max_frames_count);
  }

};

//----------------------------------------------------------------------
//
// entry point
//
//----------------------------------------------------------------------

#ifndef SAT_NO_ENTRY
  #include "plugin/sat_entry.h"
  SAT_PLUGIN_ENTRY(sa_tilteq_descriptor,sa_tilteq_plugin)
#endif

//----------------------------------------------------------------------
#endif


