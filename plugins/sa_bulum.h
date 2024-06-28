#ifndef sa_bulum_included
#define sa_bulum_included
//----------------------------------------------------------------------

/*
  TODO:
  
  - replace primitive buffer with proper interpolated delay line
  - display w/ delay buffer & grains
*/

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#include "plugin/sat_plugin.h"
#include "plugin/processor/sat_interleaved_processor.h"
//#include "audio/sat_audio_utils.h"

#include "plugin/sat_plugin.h"
#include "plugin/sat_parameters.h"
#include "gui/sat_widgets.h"

#define MAX_GRAINS  1024
#define BUFFERSIZE  (1024*1024)

struct sa_bulum_grain {
  bool  on;
  float pos;
  float sta;
  float end;
  float dur;
  float spd;
  float ph;
  float pa;
  float dh;
  float da;
};

const char* freeze_txt[2] = { "off", "on" };

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t sa_bulum_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = SAT_VENDOR "/sa_bulum/v0",
  .name         = "sa_bulum",
  .vendor       = SAT_VENDOR,
  .url          = SAT_URL,
  .manual_url   = "",
  .support_url  = "",
  .version      = SAT_VERSION,
  .description  = "",
  .features     = (const char*[]){ CLAP_PLUGIN_FEATURE_AUDIO_EFFECT, CLAP_PLUGIN_FEATURE_GRANULAR, nullptr }
};

//----------------------------------------------------------------------
//
// processor
//
//----------------------------------------------------------------------

class sa_bulum_processor
: public SAT_InterleavedProcessor {

//------------------------------
private:
//------------------------------

  //bool      need_recalc         = true;
  float     samplerate          = 0.0;

  sa_bulum_grain  MGrains[MAX_GRAINS] = {0};
  float           MBuffer[BUFFERSIZE] = {0};

  bool      need_precalc        = true;
  bool      need_recalc         = false;

  float     master              = 0.0;
  int32_t   numgrains           = 0;
  int32_t   buffersize          = 0;
  bool      freeze              = false;
  float     graindist           = 0.0;
  float     grainsize           = 0.0;
  float     graindur            = 0.0;
  float     grainpitch          = 1.0;
  float     grainenv            = 0.0;
  float     startjit            = 0.0;
  float     pitchjit            = 0.0;
  float     sizejit             = 0.0;
  float     durjit              = 0.0;
  int32_t   index               = 0;
  float     countdown           = 0.0;

  
//------------------------------
public:
//------------------------------

  sa_bulum_processor(SAT_ProcessorOwner* AOwner)
  : SAT_InterleavedProcessor(AOwner) {
  }

  //----------

  virtual ~sa_bulum_processor() {
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
      case 0:  master      = pow(2,(value / 6)); break;
      case 1:  numgrains   = trunc( value ); break;
      case 2:  buffersize  = trunc( (value / 1000) * samplerate ); break;
      case 3:  value > 0.5 ? freeze=true : freeze=false; break;
      case 4:  graindist   = (value / 1000) * samplerate; break;
      case 5:  grainsize   = (value / 1000) * samplerate; break;
      case 6:  graindur    = (value / 1000) * samplerate; break;
      case 7:  grainpitch  = value; break;
      case 8:  grainenv    = value; break;
      case 9:  startjit    = value; break;
      case 10: pitchjit    = value; break;
      case 11: sizejit     = value; break;
      case 12: durjit      = value; break;
    }
    need_recalc = true;
  }

  //----------

  void processAudio(SAT_ProcessContext* AContext, uint32_t AOffset, uint32_t ALength) override {
    const clap_process_t* process = AContext->process;

    if (need_precalc) precalc(samplerate);
    if (need_recalc) recalc(samplerate);

    float* input0  = process->audio_inputs[0].data32[0]  + AOffset;
    float* input1  = process->audio_inputs[0].data32[1]  + AOffset;
    float* output0 = process->audio_outputs[0].data32[0] + AOffset;
    float* output1 = process->audio_outputs[0].data32[1] + AOffset;
    for (uint32_t i=0; i<ALength; i++) {
      float spl0 = *input0++;
      float spl1 = *input1++;

      process_sample(&spl0,&spl1);

      *output0++ = spl0;
      *output1++ = spl1;
    }    
  }

//------------------------------
private:
//------------------------------

  void process_sample(float* spl0, float* spl1) {
    float in0 = *spl0;
    float in1 = *spl1;
    float out0 = 0;
    float out1 = 0;
    int32_t newgrain = -1;
    
    //if (numgrains > 0) {
      for (int32_t i=0; i<numgrains; i++) {
        if (MGrains[i].on) {
          MGrains[i].pos += MGrains[i].spd;
          if (MGrains[i].pos >= MGrains[i].end) MGrains[i].pos = MGrains[i].sta;
          if (MGrains[i].pos >= buffersize) MGrains[i].pos -= buffersize;
          MGrains[i].ph += (MGrains[i].pa*2);
          if (MGrains[i].ph >= 2) MGrains[i].ph -= 2;
          MGrains[i].dh += (MGrains[i].da*2);
          if (MGrains[i].dh >=2) MGrains[i].dh -= 2;
          MGrains[i].dur -= 1;
          if (MGrains[i].dur <= 0) MGrains[i].on = false;
          float gvol = MGrains[i].ph * (2-abs(MGrains[i].ph)); // abs-neg ?
          float dvol = MGrains[i].dh * (2-abs(MGrains[i].dh));
          int32_t temp = trunc( MGrains[i].pos );
          temp *= 2;
          out0 += MBuffer[ temp ] * dvol * gvol;
          out1 += MBuffer[ temp+1 ] * dvol * gvol;
        }
        else newgrain = i;
      }
    //}
    
    if (countdown <= 0) {
      countdown = graindist;
      if (newgrain > 0) {
        float startrnd = 1 + (startjit * SAT_RandomSigned());
        float pitchrnd = 1 + (pitchjit * SAT_RandomSigned());
        float sizernd  = 1 + (sizejit  * SAT_RandomSigned());
        float durrnd   = 1 + (durjit   * SAT_RandomSigned());
        float siz = (grainsize*sizernd);
        float st = index * startrnd;
        if (st >= buffersize) st -= buffersize;
        if (st <  0) st += buffersize;
        float en = st + siz;
        if (en >= buffersize) en = buffersize; // clamp
        if (en <  0) en = 0;
        float du = graindur*durrnd;
        MGrains[newgrain].on   = true;
        MGrains[newgrain].pos  = st;
        MGrains[newgrain].sta  = st;
        MGrains[newgrain].end  = en;
        MGrains[newgrain].dur  = du;
        MGrains[newgrain].spd  = grainpitch * pitchrnd;
        MGrains[newgrain].ph   = 0;
        MGrains[newgrain].pa   = 1 / siz;
        MGrains[newgrain].dh   = 0;
        MGrains[newgrain].da   = 1 / du;
      }
    }
    countdown -= 1;
    
    if (!freeze) {
      MBuffer[index*2] = in0;
      MBuffer[index*2+1] = in1;
    }
    
    index += 1;
    if (index >= buffersize) index -= buffersize;
    
    *spl0 = out0 * master;
    *spl1 = out1 * master;
  }


//------------------------------
private:
//------------------------------

  void precalc(float srate) {
    need_precalc = false;
    master = pow(2,(getParameterValue(0) / 6));
    numgrains = trunc(getParameterValue(1));
    buffersize = trunc( (getParameterValue(2) / 1000) * samplerate );
    if ( getParameterValue(3) > 0.5) freeze = true;
    else freeze = false;
    graindist = ( getParameterValue(4) / 1000) * samplerate;
    grainsize = ( getParameterValue(5) / 1000) * samplerate;
    graindur  = ( getParameterValue(6) / 1000) * samplerate;
  }

  //----------

  void recalc(float srate) {
    need_recalc = false;
  }

};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class sa_bulum_plugin
: public SAT_Plugin {
  
//------------------------------
private:
//------------------------------

  sa_bulum_processor* MProcessor = nullptr;

//------------------------------
public:
//------------------------------

  sa_bulum_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : SAT_Plugin(ADescriptor,AHost) {
  }

  //----------

  virtual ~sa_bulum_plugin() {
  }

//------------------------------
public:
//------------------------------

  bool init() final {
    registerDefaultExtensions();    
    appendStereoAudioInputPort("In");
    appendStereoAudioOutputPort("Out");
    uint32_t flags = CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_MODULATABLE;
    appendParameter(new SAT_Parameter(      "Master",            "", -6,   -60,  6, flags                  ));
    appendParameter(new SAT_IntParameter(   "Number of grains",  "",  10,   1,   MAX_GRAINS, flags         ));
    appendParameter(new SAT_Parameter(      "Buffer size",       "",  1000, 1,   1000, flags               ));
    appendParameter(new SAT_TextParameter(  "Freeze",            "",  0,    0,   1, flags,     freeze_txt  ));
    appendParameter(new SAT_Parameter(      "Grain distance",    "",  20,   0,   100, flags                ));
    appendParameter(new SAT_Parameter(      "Grain size",        "",  30,   1,   100, flags                ));
    appendParameter(new SAT_Parameter(      "Grain duration",    "",  300,  1,   1000, flags               ));
    appendParameter(new SAT_Parameter(      "Grain pitch",       "",  1,    0,   4, flags                  ));
    appendParameter(new SAT_Parameter(      "Grain envelope",    "",  0,    0,   1, flags                  ));
    appendParameter(new SAT_Parameter(      "Distance jitter",   "",  0.2,  0,   1, flags                  ));
    appendParameter(new SAT_Parameter(      "Pitch jitter",      "",  0.2,  0,   1, flags                  ));
    appendParameter(new SAT_Parameter(      "Size jitter",       "",  0.2,  0,   1, flags                  ));
    appendParameter(new SAT_Parameter(      "Duration jitter",   "",  0.2,  0,   1 , flags                 ));
    MProcessor = new sa_bulum_processor(this);
    setProcessor(MProcessor);
    return SAT_Plugin::init();
  }
  
  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    MProcessor->setSampleRate(sample_rate);
    return SAT_Plugin::activate(sample_rate,min_frames_count,max_frames_count);
  }

};

#undef MAX_GRAINS
#undef BUFFERSIZE

//----------------------------------------------------------------------
//
// entry point
//
//----------------------------------------------------------------------

#ifndef SAT_NO_ENTRY
  #include "plugin/sat_entry.h"
  SAT_PLUGIN_ENTRY(sa_bulum_descriptor,sa_bulum_plugin)
#endif

//----------------------------------------------------------------------
#endif

