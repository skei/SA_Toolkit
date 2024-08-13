#ifndef sa_sonic_maximizer_included
#define sa_sonic_maximizer_included
//----------------------------------------------------------------------

/*
  based on:
    BBE Sonic Maximizer modelisation, (C) 20011, Dominique Guichaoua.
    http://forum.cockos.com/showthread.php?t=91439
    license: ?
    from the forum:
    skei (me) "i converted both the sonic maximizer (just the first version,
               yet..) and the sonic enhancer to vst, available here [...] i
               haven't studied the licensing agreements, so if you don't want
               me to port your plugins, let me know, and i'll remove it, no
               problem.."
    domino49: "great!!"
    so i think it should be ok?
*/

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
// #include "plugin/sat_parameter.h"
// #include "plugin/sat_plugin.h"

//----------

//#define cDenorm   10e-30
#define c_ampdB   8.65617025
#define freqHP    2243.2
#define freqLP    224.32
#define freqHP_p2 (-SAT_PI2 * freqHP)
#define freqLP_p2 (-SAT_PI2 * freqLP)
//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t sa_sonic_maximizer_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = SAT_VENDOR "/sa_sonic_maximizer/v0",
  .name         = "sa_sonic_maximizer",
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
// processor
//
//----------------------------------------------------------------------

class sa_sonic_maximizer_processor
: public SAT_InterleavedProcessor {

//------------------------------
private:
//------------------------------

  bool  need_recalc = true;
  float samplerate = 0.0;

  float     param1      = 0.0f;
  float     param2      = 0.0f;
  float     param3      = 0.0f;

  float     band1       = 0.0f;
//float     band2       = 1.0f;
  float     band3       = 0.0f;
  float     tmplLP      = 0.0f;
  float     tmprLP      = 0.0f;
  float     tmplHP      = 0.0f;
  float     tmprHP      = 0.0f;
  float     amp         = 0.0f;
  float     xLP         = 0.0f;
  float     xHP         = 0.0f;
  float     a0LP        = 0.0f;
  float     a0HP        = 0.0f;
  float     b1LP        = 0.0f;
  float     b1HP        = 0.0f;
  
//------------------------------
public:
//------------------------------

  sa_sonic_maximizer_processor(SAT_ProcessorListener* AListener)
  : SAT_InterleavedProcessor(AListener) {
  }

  //----------

  virtual ~sa_sonic_maximizer_processor() {
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
    uint32_t index = event->param_id;
    float value = event->value;
    switch (index) {
      case 0:
        param1 = value;
        band1 = expf( (param1+3) / c_ampdB );
        break;
      case 1:
        param2 = value;
        band3 = expf( (param2+3) / c_ampdB );
        break;
      case 2:
        param3 = value;
        amp = expf( param3 / c_ampdB );
        break;
    }
    //band2 := 1; // exp(0/c_ampdB);
    need_recalc = true;
  }

  //----------

  void processAudio(SAT_ProcessContext* AContext, uint32_t AOffset, uint32_t ALength) override {
    const clap_process_t* process = AContext->process;
    if (need_recalc) recalc(samplerate);
    float* input0  = process->audio_inputs[0].data32[0]  + AOffset;
    float* input1  = process->audio_inputs[0].data32[1]  + AOffset;
    float* output0 = process->audio_outputs[0].data32[0] + AOffset;
    float* output1 = process->audio_outputs[0].data32[1] + AOffset;
    for (uint32_t i=0; i<ALength; i++) {

      float s0  = *input0++;
      float s1  = *input1++;
      tmplLP    = a0LP*s0 - b1LP*tmplLP;// + cDenorm;
      tmprLP    = a0LP*s1 - b1LP*tmprLP;// + cDenorm;
      float sp0 = tmplLP;
      float sp1 = tmprLP;
      tmplHP    = a0HP*s0 - b1HP*tmplHP;// + cDenorm;
      tmprHP    = a0HP*s1 - b1HP*tmprHP;// + cDenorm;
      float sp4 = s0 - tmplHP;
      float sp5 = s1 - tmprHP;
      float sp2 = s0 - sp0 - sp4;
      float sp3 = s1 - sp1 - sp5;
      *output0++ = (sp0 * band1 + sp2 /* * band2 */ + sp4 * band3) * amp;
      *output1++ = (sp1 * band1 + sp3 /* * band2 */ + sp5 * band3) * amp;

    }    
  }

//------------------------------
private:
//------------------------------

  void precalc() {
    param1 = getParameterValue(0);
    param2 = getParameterValue(1);
    param3 = getParameterValue(2);
    band1 = expf( (param1+3) / c_ampdB );
    band3 = expf( (param2+3) / c_ampdB );
    amp = expf( param3 / c_ampdB );
    //need_precalc = false;
  }

  void recalc(float srate) {
    xHP  = expf(freqHP_p2/srate);
    a0HP = 1.0 - xHP;
    b1HP = -xHP;
    xLP  = expf(freqLP_p2/srate);
    a0LP = 1.0 - xLP;
    b1LP = -xLP;
    need_recalc = false;
  }

};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class sa_sonic_maximizer_plugin
: public SAT_Plugin {
  
//------------------------------
private:
//------------------------------

  sa_sonic_maximizer_processor* MProcessor = nullptr;

//------------------------------
public:
//------------------------------

  sa_sonic_maximizer_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : SAT_Plugin(ADescriptor,AHost) {
  }

  //----------

  virtual ~sa_sonic_maximizer_plugin() {
  }

//------------------------------
public:
//------------------------------

  bool init() final {
    registerDefaultExtensions();    
    appendStereoAudioInputPort("In");
    appendStereoAudioOutputPort("Out");
    uint32_t flags = CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_MODULATABLE;
    appendParameter(new SAT_Parameter( "Low Cont", "",  1.0,   0.0, 10.0, flags ));
    appendParameter(new SAT_Parameter( "Process",  "",  1.0,   0.0, 10.0, flags ));
    appendParameter(new SAT_Parameter( "Output",   "", -3.0, -30.0,  0.0, flags ));
    MProcessor = new sa_sonic_maximizer_processor(this);
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
  SAT_PLUGIN_ENTRY(sa_sonic_maximizer_descriptor,sa_sonic_maximizer_plugin)
#endif

//#undef cDenorm
#undef c_ampdB
#undef freqHP
#undef freqLP
#undef freqHP_p2
#undef freqLP_p2

//----------------------------------------------------------------------
#endif








