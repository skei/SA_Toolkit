#ifndef sa_large_room_reverb_included
#define sa_large_room_reverb_included
//----------------------------------------------------------------------

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

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t sa_large_room_reverb_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = SAT_VENDOR "/sa_large_room_reverb/v0",
  .name         = "sa_large_room_reverb",
  .vendor       = SAT_VENDOR,
  .url          = SAT_URL,
  .manual_url   = "",
  .support_url  = "",
  .version      = SAT_VERSION,
  .description  = "",
  .features     = (const char*[]){ CLAP_PLUGIN_FEATURE_AUDIO_EFFECT, CLAP_PLUGIN_FEATURE_REVERB, nullptr }
};

//----------------------------------------------------------------------
//
// processor
//
//----------------------------------------------------------------------

class sa_large_room_reverb_processor
: public SAT_InterleavedProcessor {

//------------------------------
private:
//------------------------------

  bool  need_recalc = true;
  float samplerate = 0.0;

  float BUFFER[1024*1024] = {0};
  
  uint32_t  a0, a0_pos, a0_len, a0r, a0r_pos, a0r_len;
  uint32_t  a1, a1_pos, a1_len, a1r, a1r_pos, a1r_len;
  uint32_t  a2, a2_pos, a2_len, a2r, a2r_pos, a2r_len;
  uint32_t  a3, a3_pos, a3_len, a3r, a3r_pos, a3r_len;
  uint32_t  a4, a4_pos, a4_len, a4r, a4r_pos, a4r_len;
  uint32_t  a5, a5_pos, a5_len, a5r, a5r_pos, a5r_len;
  uint32_t  a6, a6_pos, a6_len, a6r, a6r_pos, a6r_len;

  float     a0_g,    a1_g,    a2_g,    a3_g,    a4_g,    a5_g,    a6_g;
  float     a0_in,   a1_in,   a2_in,   a3_in,   a4_in,   a5_in,   a6_in;
  float     a0_out,  a1_out,  a2_out,  a3_out,  a4_out,  a5_out,  a6_out;
  float     a0r_in,  a1r_in,  a2r_in,  a3r_in,  a4r_in,  a5r_in,  a6r_in;
  float     a0r_out, a1r_out, a2r_out, a3r_out, a4r_out, a5r_out, a6r_out;

  float     tmp, tmpr;
  float     t,   tr;

  float     wet, dry;
  float     c, g;
  
//------------------------------
public:
//------------------------------

  sa_large_room_reverb_processor(SAT_ProcessorListener* AListener)
  : SAT_InterleavedProcessor(AListener) {
  }

  //----------

  virtual ~sa_large_room_reverb_processor() {
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
    float* input0  = process->audio_inputs[0].data32[0]  + AOffset;
    float* input1  = process->audio_inputs[0].data32[1]  + AOffset;
    float* output0 = process->audio_outputs[0].data32[0] + AOffset;
    float* output1 = process->audio_outputs[0].data32[1] + AOffset;
    for (uint32_t i=0; i<ALength; i++) {
      float spl0 = *input0++;
      float spl1 = *input1++;

      float s_in = spl0;
      float in = spl0 + tmp * g;
      
      // ---- AP 1
      a0_in = in;
      a0_out = -a0_in*a0_g + BUFFER[a0+a0_pos]; //a0[a0_pos];
      /*a0[a0_pos]*/ BUFFER[a0+a0_pos] = a0_in + a0_out*a0_g;
      a0_pos+=1;
      if (a0_pos >= a0_len) a0_pos = 0;
      // ---- AP 2
      a1_in = a0_out;
      a1_out = -a1_in*a1_g + BUFFER[a1+a1_pos]; //a1[a1_pos];
      /*a1[a1_pos]*/ BUFFER[a1+a1_pos] = a1_in + a1_out*a1_g;
      a1_pos+=1;
      if (a1_pos >= a1_len) a1_pos = 0;
      // ---- AP 3
      a2_in = a1_out;
      a2_out = -a2_in*a2_g + a3_out;
      a3_in = a2_in + a2_out*a2_g;
      // --> nested
        a3_out = -a3_in*a3_g + BUFFER[a3+a3_pos];//a3[a3_pos];
        /*a3[a3_pos]*/ BUFFER[a3+a3_pos] = a3_in + a3_out*a3_g;
        a3_pos+=1;
        if (a3_pos >= a3_len) a3_pos = 0;
        /*a2[a2_pos]*/ BUFFER[a2+a2_pos] = a3_out;
        a2_pos+=1;
        if (a2_pos >= a2_len) a2_pos = 0;
        a3_out = BUFFER[a2+a2_pos]; //a2[a2_pos];
      // <-- nested
      // ---- AP 5
      a4_in = a2_out;
      a4_out = -a4_in*a4_g + a6_out;
      a5_in = a4_in + a4_out*a4_g;
      // --> nested
        a5_out = -a5_in*a5_g + BUFFER[a5+a5_pos];//a5[a5_pos];
        /*a5[a5_pos]*/ BUFFER[a5+a5_pos] = a5_in + a5_out*a5_g;
        a5_pos+=1;
        if (a5_pos >= a5_len) a5_pos = 0;
        a6_in = a5_out;
        a6_out = -a6_in*a6_g + BUFFER[a6+a6_pos];//a6[a6_pos];
        /*a6[a6_pos]*/ BUFFER[a6+a6_pos] = a6_in + a6_out*a5_g;
        a6_pos+=1;
        if (a6_pos >= a6_len) a6_pos = 0;
        /*a4[a4_pos]*/ BUFFER[a4+a4_pos] = a6_out;
        a4_pos+=1;
        if (a4_pos >= a4_len) a4_pos = 0;
        a6_out = BUFFER[a4+a4_pos];//a4[a4_pos];
      // <-- nested
      tmp = a4_out;
      tmp = (t = tmp + c*(t-tmp));
      float out = a1_out*0.34 + a2_out*0.14 + a4_out*0.15;
      
      spl0 = s_in * dry + out * wet;
      
      s_in = spl1;
      in = spl1 + tmpr*g;
      
      // ---- AP 1
      a0r_in = in;
      a0r_out = -a0r_in*a0_g + BUFFER[a0r+a0r_pos];//a0r[a0r_pos];
      /*a0r[a0r_pos]*/ BUFFER[a0r+a0r_pos] = a0r_in + a0r_out*a0_g;
      a0r_pos+=1;
      if (a0r_pos >= a0r_len) a0r_pos = 0;
      // ---- AP 2
      a1r_in = a0r_out;
      a1r_out = -a1r_in*a1_g + BUFFER[a1r+a1r_pos];//a1r[a1r_pos];
      /*a1r[a1r_pos]*/ BUFFER[a1r+a1r_pos] = a1r_in + a1r_out*a1_g;
      a1r_pos+=1;
      if (a1r_pos >= a1r_len) a1r_pos = 0;
      // ---- AP 3
      a2r_in = a1r_out;
      a2r_out = -a2r_in*a2_g + a3r_out;
      a3r_in = a2r_in + a2r_out*a2_g;
      // --> nested
        a3r_out = -a3r_in*a3_g + BUFFER[a3r+a3r_pos];//a3r[a3r_pos];
        /*a3r[a3r_pos]*/ BUFFER[a3r+a3r_pos] = a3r_in + a3r_out*a3_g;
        a3r_pos+=1;
        if (a3r_pos >= a3r_len) a3r_pos = 0;
        /*a2r[a2r_pos]*/ BUFFER[a2r+a2r_pos] = a3r_out;
        a2r_pos+=1;
        if (a2r_pos >= a2r_len) a2r_pos = 0;
        a3r_out = BUFFER[a2r+a2r_pos];//a2r[a2r_pos];
      // <-- nested
      // ---- AP 5
      a4r_in = a2r_out;
      a4r_out = -a4r_in*a4_g + a6r_out;
      a5r_in = a4r_in + a4r_out*a4_g;
      // --> nested
        a5r_out = -a5r_in*a5_g + BUFFER[a5r+a5r_pos];//a5r[a5r_pos];
        /*a5r[a5r_pos]*/ BUFFER[a5r+a5r_pos] = a5r_in + a5r_out*a5_g;
        a5r_pos+=1;
        if (a5r_pos >= a5r_len) a5r_pos = 0;
        a6r_in = a5r_out;
        a6r_out = -a6r_in*a6_g + BUFFER[a6r+a6r_pos];//a6r[a6r_pos];
        /*a6r[a6r_pos]*/ BUFFER[a6r+a6r_pos] = a6r_in + a6r_out*a5_g;
        a6r_pos+=1;
        if (a6r_pos >= a6r_len) a6r_pos = 0;
        /*a4r[a4r_pos]*/ BUFFER[a4r+a4r_pos] = a6r_out;
        a4r_pos+=1;
        if (a4r_pos >= a4r_len) a4r_pos = 0;
        a6r_out = BUFFER[a4r+a4r_pos];//a4r[a4r_pos];
      // <-- nested
      tmpr = a4r_out;
      tmpr = (tr = tmpr + c*(tr-tmpr));
      out = a1r_out*0.34 + a2r_out*0.14 + a4r_out*0.15;
      
      spl1 = s_in * dry + out * wet;

      *output0++ = spl0;
      *output1++ = spl1;
    }    
  }

//------------------------------
private:
//------------------------------

  void recalc(float srate) {
    dry = exp(getParameterValue(0) / 8.65617);
    wet = exp(getParameterValue(1) / 8.65617);
    g = 1.0 - getParameterValue(2) / 100.0;
    a0 = 0;
    a0_len = (srate * 8 / 1000);
    a0_g = 0.3;
    a1 = a0 + a0_len + 1;
    a1_len = (srate * 12 / 1000);
    a1_g = 0.3;
    a2 = a1 + a1_len + 1;
    a2_len = (srate * 87 / 1000);
    a2_g = 0.5;
    a3 = a2 + a2_len + 1;
    a3_len = (srate * 62 / 1000);
    a3_g = 0.25;
    a4 = a3 + a3_len + 1;
    a4_len = (srate * 120 / 1000);
    a4_g = 0.5;
    a5 = a4 + a4_len + 1;
    a5_len = (srate * 76 / 1000);
    a5_g = 0.25;
    a6 = a5 + a5_len + 1;
    a6_len = (srate * 30 / 1000);
    a6_g = 0.25;
    uint32_t rndcoef = 50;
    a0r = a6 + a6_len + 1;
    a0r_len = ((srate * 8 / 1000)) + rndcoef;
    a1r = a0r + a0r_len + 1;
    a1r_len = ((srate * 12 / 1000)) - rndcoef;
    a2r = a1r + a1r_len + 1;
    a2r_len = ((srate * 87 / 1000)) + rndcoef;
    a3r = a2r + a2r_len + 1;
    a3r_len = ((srate * 62 / 1000)) - rndcoef;
    a4r = a3r + a3r_len + 1;
    a4r_len = ((srate * 120 / 1000)) + rndcoef;
    a5r = a4r + a4r_len + 1;
    a5r_len = ((srate * 76 / 1000)) - rndcoef;
    a6r = a5r + a5r_len + 1;
    a6r_len = ((srate * 30 / 1000)) + rndcoef;
    c = exp(-2 * 3.14 * 2600 / srate);
    need_recalc = false;

  }

};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class sa_large_room_reverb_plugin
: public SAT_Plugin {
  
//------------------------------
private:
//------------------------------

  sa_large_room_reverb_processor* MProcessor = nullptr;

//------------------------------
public:
//------------------------------

  sa_large_room_reverb_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : SAT_Plugin(ADescriptor,AHost) {
  }

  //----------

  virtual ~sa_large_room_reverb_plugin() {
  }

//------------------------------
public:
//------------------------------

  bool init() final {
    registerDefaultExtensions();    
    appendStereoAudioInputPort("In");
    appendStereoAudioOutputPort("Out");
    uint32_t flags = CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_MODULATABLE;
    appendParameter(new SAT_Parameter( "Dry",     "",  0,  -120, 0,   flags ));
    appendParameter(new SAT_Parameter( "Wet",     "", -6,  -120, 0,   flags ));
    appendParameter(new SAT_Parameter( "Damping", "",  50,  0,   100, flags ));
    MProcessor = new sa_large_room_reverb_processor(this);
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
  SAT_PLUGIN_ENTRY(sa_large_room_reverb_descriptor,sa_large_room_reverb_plugin)
#endif

//----------------------------------------------------------------------
#endif























  
  





#if 0

/* Small Room Reverberator
 * 
 * Implemented as proposed in "The Virtual Acoustic Room" by William Grant Gardner
 *                            (http://www.media.mit.edu/~billg/docs/ms-thesis.ps.gz)
 */

desc:Large Room Reverberator

slider1:0<-120,0,1>Dry
slider2:-6<-120,0,1>Wet
slider3:50<0,100,1>Damping (%)

@init
a0_pos = 0;
a1_pos = 0;
a2_pos = 0;
a3_pos = 0;
a4_pos = 0;
a5_pos = 0;
a6_pos = 0;
tmp = 0;
t = 0;

a0r_pos = 0;
a1r_pos = 0;
a2r_pos = 0;
a3r_pos = 0;
a4r_pos = 0;
a5r_pos = 0;
a6r_pos = 0;
tmpr = 0;
tr = 0;

@slider
wet = exp(slider2/8.65617);
dry = exp(slider1/8.65617);
g = 1-slider3/100;

a0 = 0;
a0_len = (srate*8/1000)|0;
a0_g = 0.3;

a1 = a0+a0_len+1;
a1_len = (srate*12/1000)|0;
a1_g = 0.3;

a2 = a1+a1_len+1;
a2_len = (srate*87/1000)|0;
a2_g = 0.5;

a3 = a2+a2_len+1;
a3_len = (srate*62/1000)|0;
a3_g = 0.25;

a4 = a3+a3_len+1;
a4_len = (srate*120/1000)|0;
a4_g = 0.5;

a5 = a4+a4_len+1;
a5_len = (srate*76/1000)|0;
a5_g = 0.25;

a6 = a5+a5_len+1;
a6_len = (srate*30/1000)|0;
a6_g = 0.25;


rndcoef = 50;

a0r = a6+a6_len+1;
a0r_len = ((srate*8/1000)|0)+rndcoef;

a1r = a0r+a0r_len+1;
a1r_len = ((srate*12/1000)|0)-rndcoef;

a2r = a1r+a1r_len+1;
a2r_len = ((srate*87/1000)|0)+rndcoef;

a3r = a2r+a2r_len+1;
a3r_len = ((srate*62/1000)|0)-rndcoef;

a4r = a3r+a3r_len+1;
a4r_len = ((srate*120/1000)|0)+rndcoef;

a5r = a4r+a4r_len+1;
a5r_len = ((srate*76/1000)|0)-rndcoef;

a6r = a5r+a5r_len+1;
a6r_len = ((srate*30/1000)|0)+rndcoef;

c = exp(-2*3.14*2600/srate);

@sample

s_in = spl0;

in = spl0 + tmp*g;

// ---- AP 1
a0_in = in;
a0_out = -a0_in*a0_g + a0[a0_pos];
a0[a0_pos] = a0_in + a0_out*a0_g;
(a0_pos+=1) >= a0_len ? a0_pos = 0;

// ---- AP 2
a1_in = a0_out;
a1_out = -a1_in*a1_g + a1[a1_pos];
a1[a1_pos] = a1_in + a1_out*a1_g;
(a1_pos+=1) >= a1_len ? a1_pos = 0;

// ---- AP 3
a2_in = a1_out;
a2_out = -a2_in*a2_g + a3_out;
a3_in = a2_in + a2_out*a2_g;

// --> nested
  a3_out = -a3_in*a3_g + a3[a3_pos];
  a3[a3_pos] = a3_in + a3_out*a3_g;
  (a3_pos+=1) >= a3_len ? a3_pos = 0;
  
  a2[a2_pos] = a3_out;
  (a2_pos+=1) >= a2_len ? a2_pos = 0;
  a3_out = a2[a2_pos];

// <-- nested


// ---- AP 5
a4_in = a2_out;
a4_out = -a4_in*a4_g + a6_out;
a5_in = a4_in + a4_out*a4_g;

// --> nested
  a5_out = -a5_in*a5_g + a5[a5_pos];
  a5[a5_pos] = a5_in + a5_out*a5_g;
  (a5_pos+=1) >= a5_len ? a5_pos = 0;
  
  a6_in = a5_out;
  a6_out = -a6_in*a6_g + a6[a6_pos];
  a6[a6_pos] = a6_in + a6_out*a5_g;
  (a6_pos+=1) >= a6_len ? a6_pos = 0;

  a4[a4_pos] = a6_out;
  (a4_pos+=1) >= a4_len ? a4_pos = 0;
  a6_out = a4[a4_pos];

// <-- nested


tmp = a4_out;
tmp = (t = tmp + c*(t-tmp));

out = a1_out*0.34 + a2_out*0.14 + a4_out*0.15;

spl0 = s_in*dry + out*wet;


s_in = spl1;

in = spl1 + tmpr*g;

// ---- AP 1
a0r_in = in;
a0r_out = -a0r_in*a0_g + a0r[a0r_pos];
a0r[a0r_pos] = a0r_in + a0r_out*a0_g;
(a0r_pos+=1) >= a0r_len ? a0r_pos = 0;

// ---- AP 2
a1r_in = a0r_out;
a1r_out = -a1r_in*a1_g + a1r[a1r_pos];
a1r[a1r_pos] = a1r_in + a1r_out*a1_g;
(a1r_pos+=1) >= a1r_len ? a1r_pos = 0;

// ---- AP 3
a2r_in = a1r_out;
a2r_out = -a2r_in*a2_g + a3r_out;
a3r_in = a2r_in + a2r_out*a2_g;

// --> nested
  a3r_out = -a3r_in*a3_g + a3r[a3r_pos];
  a3r[a3r_pos] = a3r_in + a3r_out*a3_g;
  (a3r_pos+=1) >= a3r_len ? a3r_pos = 0;
  
  a2r[a2r_pos] = a3r_out;
  (a2r_pos+=1) >= a2r_len ? a2r_pos = 0;
  a3r_out = a2r[a2r_pos];

// <-- nested


// ---- AP 5
a4r_in = a2r_out;
a4r_out = -a4r_in*a4_g + a6r_out;
a5r_in = a4r_in + a4r_out*a4_g;

// --> nested
  a5r_out = -a5r_in*a5_g + a5r[a5r_pos];
  a5r[a5r_pos] = a5r_in + a5r_out*a5_g;
  (a5r_pos+=1) >= a5r_len ? a5r_pos = 0;
  
  a6r_in = a5r_out;
  a6r_out = -a6r_in*a6_g + a6r[a6r_pos];
  a6r[a6r_pos] = a6r_in + a6r_out*a5_g;
  (a6r_pos+=1) >= a6r_len ? a6r_pos = 0;

  a4r[a4r_pos] = a6r_out;
  (a4r_pos+=1) >= a4r_len ? a4r_pos = 0;
  a6r_out = a4r[a4r_pos];

// <-- nested


tmpr = a4r_out;
tmpr = (tr = tmpr + c*(tr-tmpr));

out = a1r_out*0.34 + a2r_out*0.14 + a4r_out*0.15;

spl1 = s_in*dry + out*wet;


#endif // 0
