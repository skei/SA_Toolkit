#ifndef sa_small_room_reverb_included
#define sa_small_room_reverb_included
//----------------------------------------------------------------------


//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#include "sat.h"
#include "audio/sat_audio_utils.h"
#include "plugin/clap/sat_clap.h"
#include "plugin/sat_parameter.h"
#include "plugin/sat_plugin.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t sa_small_room_reverb_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = SAT_VENDOR "/sa_small_room_reverb",
  .name         = "sa_small_room_reverb",
  .vendor       = SAT_VENDOR,
  .url          = SAT_URL,
  .manual_url   = "",
  .support_url  = "",
  .version      = SAT_VERSION,
  .description  = "",
  .features     = (const char*[]) {
                    CLAP_PLUGIN_FEATURE_AUDIO_EFFECT,
                    CLAP_PLUGIN_FEATURE_REVERB,
                    nullptr
                  }
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_small_room_reverb_plugin
: public SAT_Plugin {
  
//------------------------------
private:
//------------------------------

  bool  need_recalc = true;
  float MSampleRate = 0.0;

  float BUFFER[1024*1024];

  //float slider1,slider2,slider3;

  uint32_t a0_pos, a1_pos, a2_pos, a3_pos, a4_pos;
  uint32_t a0r_pos, a1r_pos, a2r_pos, a3r_pos, a4r_pos;
  float tmp, tmpr, t, tr;

  float in, out, g, c;
  float dry, wet;

  float   a0_g,    a1_g,    a2_g,    a3_g,    a4_g;

  /*float*/ uint32_t   a0,    a1,    a2,    a3,    a4;
  /*float*/ uint32_t   a0r,   a1r,   a2r,   a3r,   a4r;

  uint32_t  a0_len,  a1_len,  a2_len,  a3_len,  a4_len;
  float   a0_in,   a1_in,   a2_in,   a3_in,   a4_in;
  float   a0_out,  a1_out,  a2_out,  a3_out,  a4_out;

  uint32_t  a0r_len, a1r_len, a2r_len, a3r_len, a4r_len;
  float   a0r_in,  a1r_in,  a2r_in,  a3r_in,  a4r_in;
  float   a0r_out, a1r_out, a2r_out, a3r_out, a4r_out;

//------------------------------
public:
//------------------------------

  SAT_DEFAULT_PLUGIN_CONSTRUCTOR(sa_small_room_reverb_plugin)

  //----------
  
  bool init() final {
    registerDefaultExtensions();    
    appendStereoAudioInputPort("In");
    appendStereoAudioOutputPort("Out");
    
    appendParameter(new SAT_Parameter( "Dry",      0,  -120, 0 ));
    appendParameter(new SAT_Parameter( "Wet",     -6,  -120, 0 ));
    appendParameter(new SAT_Parameter( "Damping",  50,   0,   100 ));
    
    setAllParameterFlags(CLAP_PARAM_IS_MODULATABLE);
    
      a0_pos = 0;
      a1_pos = 0;
      a2_pos = 0;
      a3_pos = 0;
      a4_pos = 0;
      a0r_pos = 0;
      a1r_pos = 0;
      a2r_pos = 0;
      a3r_pos = 0;
      a4r_pos = 0;
      tmp = 0;
      tmpr = 0;
      t = 0;
      tr = 0;
    
    
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
      
      // left channel

      in = spl0 + tmp*g;
      // ---- AP 1
      a0_in = in;
      a0_out = -a0_in*a0_g + a2_out;
      a1_in = a0_in + a0_out*a0_g;
      // -> nested
        a1_out = -a1_in*a1_g + BUFFER[a1+a1_pos];//a1[a1_pos];
        /*a1[a1_pos]*/ BUFFER[a1+a1_pos] = a1_in + a1_out*a1_g;
        a1_pos += 1;
        if (a1_pos >= a1_len) a1_pos = 0;
        a2_in = a1_out;
        a2_out = -a2_in*a2_g + BUFFER[a2+a2_pos]; //a2[a2_pos];
        /*a2[a2_pos]*/ BUFFER[a2+a2_pos] = a2_in + a2_out*a2_g;
        a2_pos+=1;
        if (a2_pos >= a2_len) a2_pos = 0;
        // delay
        /*a0[a0_pos]*/ BUFFER[a0+a0_pos] = a2_out;
        a0_pos+=1;
        if (a0_pos >= a0_len) a0_pos = 0;
        a2_out = BUFFER[a0+a0_pos]; //a0[a0_pos];
      // <-
      // ---- end AP 1
      // ---- AP 2
      a3_in = a0_out;
      a3_out = -a3_in*a3_g + a4_out;
      a4_in = a3_in + a3_out*a3_g;
      // -> nested
        a4_out = -a4_in*a4_g + BUFFER[a4+a4_pos]; //a4[a4_pos];
        /*a4[a4_pos]*/ BUFFER[a4+a4_pos] = a4_in + a4_out*a4_g;
        a4_pos+=1;
        if (a4_pos >= a4_len) a4_pos = 0;
        // delay
        /*a3[a3_pos]*/ BUFFER[a3+a3_pos] = a4_out;
        a3_pos+=1;
        if (a3_pos >= a3_len) a3_pos = 0;
        a4_out = BUFFER[a3+a3_pos]; //a3[a3_pos];
      // <-
      // ---- end AP 2
      tmp = a3_out;
      tmp = (t = tmp + c*(t-tmp));
      out = a0_out*0.5 + a3_out*0.5;
      spl0 = spl0*dry + out*wet;

      // right channel

      in = spl1 + tmpr*g;
      // ---- AP 1
      a0r_in = in;
      a0r_out = -a0r_in*a0_g + a2r_out;
      a1r_in = a0r_in + a0r_out*a0_g;
      // -> nested
        a1r_out = -a1r_in*a1_g + BUFFER[a1r+a1r_pos]; //a1r[a1r_pos];
        /*a1r[a1r_pos]*/ BUFFER[a1r+a1r_pos] = a1r_in + a1r_out*a1_g;
        a1r_pos+=1;
        if (a1r_pos >= a1r_len) a1r_pos = 0;
        a2r_in = a1r_out;
        a2r_out = -a2r_in*a2_g + BUFFER[a2r+a2r_pos]; //a2r[a2r_pos];
        /*a2r[a2r_pos]*/ BUFFER[a2r+a2r_pos] = a2r_in + a2r_out*a2_g;
        a2r_pos+=1;
        if (a2r_pos >= a2r_len) a2r_pos = 0;
        // delay
        /*a0r[a0r_pos]*/ BUFFER[a0r+a0r_pos] = a2r_out;
        a0r_pos+=1;
        if (a0r_pos >= a0r_len) a0r_pos = 0;
        a2r_out = BUFFER[a0r+a0r_pos]; //a0r[a0r_pos];
      // <-
      // AP 2
      a3r_in = a0r_out;
      a3r_out = -a3r_in*a3_g + a4r_out;
      a4r_in = a3r_in + a3r_out*a3_g;
      // -> nested
        a4r_out = -a4r_in*a4_g + BUFFER[a4r+a4r_pos]; //a4r[a4r_pos];
        /*a4r[a4r_pos]*/ BUFFER[a4r+a4r_pos] = a4r_in + a4r_out*a4_g;
        a4r_pos+=1;
        if (a4r_pos >= a4r_len) a4r_pos = 0;
        // delay
        /*a3r[a3r_pos]*/ BUFFER[a3r+a3r_pos] = a4r_out;
        a3r_pos+=1;
        if (a3r_pos >= a3r_len) a3r_pos = 0;
        a4r_out = BUFFER[a3r+a3r_pos]; //a3r[a3r_pos];
      // <-
      tmpr = a3r_out;
      tmpr = (tr = tmpr + c*(tr-tmpr));
      out = a0r_out*0.5 + a3r_out*0.5;
      spl1 = spl1*dry + out*wet;

      *out0++ = spl0;
      *out1++ = spl1;
    }
  }
  
//------------------------------
private:
//------------------------------

  void recalc(float srate) {
    need_recalc = false;
    dry = exp(getParameterValue(0)/8.65617);
    wet = exp(getParameterValue(1)/8.65617);
    g = 1-getParameterValue(2)/100;
    a0 = 0;
    a0_len = (srate*35/1000);
    a0_g = 0.3;
    a1 = a0+a0_len+1;
    a1_len = (srate*22/1000);
    a1_g = 0.4;
    a2 = a1+a1_len+1;
    a2_len = (srate*8.3/1000);
    a2_g = 0.6;
    a3 = a2+a2_len+1;
    a3_len = (srate*66/1000);
    a3_g = 0.1;
    a4 = a3+a3_len+1;
    a4_len = (srate*30/1000);
    a4_g = 0.4;
    // scatter second channels delay length's to give stereo spread
    uint32_t rndcoef = 50;
    a0r = a4+a4_len+1;
    a0r_len = ((srate*35/1000))+rndcoef;
    a1r = a0r+a0r_len+1;
    a1r_len = ((srate*22/1000))-rndcoef;
    a2r = a1r+a1r_len+1;
    a2r_len = ((srate*8.3/1000))+rndcoef;
    a3r = a2r+a2r_len+1;
    a3r_len = ((srate*66/1000))-rndcoef;
    a4r = a3r+a3r_len+1;
    a4r_len = ((srate*30/1000))+rndcoef;
    // the lowpass isn't specified any further so we use a simple RC filter
    c = exp(-2*3.14*4200/srate);
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#ifndef SAT_NO_ENTRY
  #include "plugin/sat_entry.h"
  SAT_PLUGIN_ENTRY(sa_small_room_reverb_descriptor,sa_small_room_reverb_plugin)
#endif

//----------


//----------------------------------------------------------------------
#endif
























#if 0

/* Small Room Reverberator
 * 
 * Implemented as proposed in "The Virtual Acoustic Room" by William Grant Gardner
 *                            (http://www.media.mit.edu/~billg/docs/ms-thesis.ps.gz)
 */

desc:Small Room Reverberator (Gardner)

slider1:0<-120,0,1>Dry (dB)
slider2:-6<-120,0,1>Wet (dB)
slider3:50<0,100,1>Damping (%)

@init
a0_pos = 0;
a1_pos = 0;
a2_pos = 0;
a3_pos = 0;
a4_pos = 0;
a0r_pos = 0;
a1r_pos = 0;
a2r_pos = 0;
a3r_pos = 0;
a4r_pos = 0;
tmp = 0;
tmpr = 0;
t = 0;
tr = 0;

@slider
wet = exp(slider2/8.65617);
dry = exp(slider1/8.65617);
g = 1-slider3/100;

a0 = 0;
a0_len = (srate*35/1000)|0;
a0_g = 0.3;

a1 = a0+a0_len+1;
a1_len = (srate*22/1000)|0;
a1_g = 0.4;

a2 = a1+a1_len+1;
a2_len = (srate*8.3/1000)|0;
a2_g = 0.6;

a3 = a2+a2_len+1;
a3_len = (srate*66/1000)|0;
a3_g = 0.1;

a4 = a3+a3_len+1;
a4_len = (srate*30/1000)|0;
a4_g = 0.4;

/* scatter second channels delay length's to give stereo spread */
rndcoef = 50;

a0r = a4+a4_len+1;
a0r_len = ((srate*35/1000)|0)+rndcoef;

a1r = a0r+a0r_len+1;
a1r_len = ((srate*22/1000)|0)-rndcoef;

a2r = a1r+a1r_len+1;
a2r_len = ((srate*8.3/1000)|0)+rndcoef;

a3r = a2r+a2r_len+1;
a3r_len = ((srate*66/1000)|0)-rndcoef;

a4r = a3r+a3r_len+1;
a4r_len = ((srate*30/1000)|0)+rndcoef;

/* the lowpass isn't sepcified any further so we use a simple RC filter */
c = exp(-2*3.14*4200/srate);

@sample

// left channel

in = spl0 + tmp*g;

// ---- AP 1
a0_in = in;
a0_out = -a0_in*a0_g + a2_out;
a1_in = a0_in + a0_out*a0_g;

// -> nested
  a1_out = -a1_in*a1_g + a1[a1_pos];
  a1[a1_pos] = a1_in + a1_out*a1_g;
  (a1_pos+=1) >= a1_len ? a1_pos = 0;

  a2_in = a1_out;
  a2_out = -a2_in*a2_g + a2[a2_pos];
  a2[a2_pos] = a2_in + a2_out*a2_g;
  (a2_pos+=1) >= a2_len ? a2_pos = 0;

  // delay
  a0[a0_pos] = a2_out;
  (a0_pos+=1) >= a0_len ? a0_pos = 0;
  a2_out = a0[a0_pos];
// <-

// ---- end AP 1

// ---- AP 2
a3_in = a0_out;
a3_out = -a3_in*a3_g + a4_out;
a4_in = a3_in + a3_out*a3_g;

// -> nested 
  a4_out = -a4_in*a4_g + a4[a4_pos];
  a4[a4_pos] = a4_in + a4_out*a4_g;
  (a4_pos+=1) >= a4_len ? a4_pos = 0;

  // delay
  a3[a3_pos] = a4_out;
  (a3_pos+=1) >= a3_len ? a3_pos = 0;
  a4_out = a3[a3_pos];
// <-

// ---- end AP 2

tmp = a3_out;
tmp = (t = tmp + c*(t-tmp));

out = a0_out*0.5 + a3_out*0.5;

spl0 = spl0*dry + out*wet;



// right channel

in = spl1 + tmpr*g;

// ---- AP 1
a0r_in = in;
a0r_out = -a0r_in*a0_g + a2r_out;
a1r_in = a0r_in + a0r_out*a0_g;

// -> nested
  a1r_out = -a1r_in*a1_g + a1r[a1r_pos];
  a1r[a1r_pos] = a1r_in + a1r_out*a1_g;
  (a1r_pos+=1) >= a1r_len ? a1r_pos = 0;

  a2r_in = a1r_out;
  a2r_out = -a2r_in*a2_g + a2r[a2r_pos];
  a2r[a2r_pos] = a2r_in + a2r_out*a2_g;
  (a2r_pos+=1) >= a2r_len ? a2r_pos = 0;

  // delay
  a0r[a0r_pos] = a2r_out;
  (a0r_pos+=1) >= a0r_len ? a0r_pos = 0;
  a2r_out = a0r[a0r_pos];
// <-

// AP 2
a3r_in = a0r_out;
a3r_out = -a3r_in*a3_g + a4r_out;
a4r_in = a3r_in + a3r_out*a3_g;

// -> nested 
  a4r_out = -a4r_in*a4_g + a4r[a4r_pos];
  a4r[a4r_pos] = a4r_in + a4r_out*a4_g;
  (a4r_pos+=1) >= a4r_len ? a4r_pos = 0;

  // delay
  a3r[a3r_pos] = a4r_out;
  (a3r_pos+=1) >= a3r_len ? a3r_pos = 0;
  a4r_out = a3r[a3r_pos];
// <-

tmpr = a3r_out;
tmpr = (tr = tmpr + c*(tr-tmpr));

out = a0r_out*0.5 + a3r_out*0.5;

spl1 = spl1*dry + out*wet;



#endif // 0


