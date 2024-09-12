#ifndef sa_pitch_included
#define sa_pitch_included
//----------------------------------------------------------------------

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#include "plugin/sat_plugin.h"
#include "plugin/processor/sat_interleaved_processor.h"
//#include "audio/sat_audio_utils.h"

#include "sat.h"
#include "audio/sat_audio_utils.h"
#include "plugin/sat_parameter.h"
#include "plugin/sat_plugin.h"

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t sa_pitch_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = SAT_VENDOR "/sa_pitch/v0",
  .name         = "sa_pitch",
  .vendor       = SAT_VENDOR,
  .url          = SAT_URL,
  .manual_url   = "",
  .support_url  = "",
  .version      = SAT_VERSION,
  .description  = "",
  .features     = (const char*[]){ CLAP_PLUGIN_FEATURE_AUDIO_EFFECT, CLAP_PLUGIN_FEATURE_PITCH_SHIFTER, nullptr }
};

//----------------------------------------------------------------------
//
// processor
//
//----------------------------------------------------------------------

class sa_pitch_processor
: public SAT_InterleavedProcessor {

//------------------------------
private:
//------------------------------

  bool  need_recalc = true;
  float samplerate = 0.0;

  float buffer[1024*1024*8] = {0};
  float in_pos              = 0;
  float out_pos             = 0;
  float base                = 0;
  float old_base            = 0;
  float fade_base           = 0;
  float inc_                = 0;
  float len_                = 0;
  float fade_               = 0;
  
//------------------------------
public:
//------------------------------

  sa_pitch_processor(SAT_ProcessorListener* AListener)
  : SAT_InterleavedProcessor(AListener) {
  }

  //----------

  virtual ~sa_pitch_processor() {
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

      uint32_t p = (uint32_t)floorf(in_pos);
      buffer[ p*2] = spl0;
      buffer[ p*2+1] = spl1;
      in_pos = (uint32_t)floorf(in_pos+1) % (uint32_t)floorf(len_);
      float gain = SAT_Min(out_pos/fade_,1);
      uint32_t p0 = (uint32_t)floorf(base+out_pos) % (uint32_t)floorf(len_);
      uint32_t p1 = (uint32_t)floorf(fade_base+out_pos) % (uint32_t)floorf(len_);
      spl0 = buffer[ p0*2] * gain
           + buffer[ p1*2] * (1-gain);
      spl1 = buffer[ p0*2+1] * gain
           + buffer[ p1*2+1] * (1-gain);
      out_pos += inc_;
      if (out_pos >= (len_-1-fade_)) {
        fade_base = base + len_ - 1 - fade_;
        out_pos = 0;
        base = in_pos;
      }

      *output0++ = spl0;
      *output1++ = spl1;
    }    
  }

//------------------------------
private:
//------------------------------

  void recalc(float srate) {
    need_recalc = false;
    //inc_  = AValue/100;
    if (getParameterValue(0) >= 0) inc_ = 1 + getParameterValue(0);
    else if (getParameterValue(0) < 0) inc_ = 1 + (getParameterValue(0) * 0.5); // -1 -> 1+(-1*0.5) = 1 + -0.5 = 0.5
    //else inc_ = 1;
    len_  = floorf(getParameterValue(1)/1000*samplerate/2)*2;
    fade_ = getParameterValue(2)/100 * len_/2;
  }

};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class sa_pitch_plugin
: public SAT_Plugin {
  
//------------------------------
private:
//------------------------------

  sa_pitch_processor* MProcessor = nullptr;

//------------------------------
public:
//------------------------------

  sa_pitch_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : SAT_Plugin(ADescriptor,AHost) {
  }

  //----------

  virtual ~sa_pitch_plugin() {
  }

//------------------------------
public:
//------------------------------

  bool init() final {
    registerDefaultExtensions();    
    appendStereoAudioInputPort("In");
    appendStereoAudioOutputPort("Out");
    uint32_t flags = CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_MODULATABLE;
    appendParameter(new SAT_Parameter( "Pitch",     "", 0,    -1, 1, flags   ));
    appendParameter(new SAT_Parameter( "Window",    "", 100,   1, 200, flags ));
    appendParameter(new SAT_Parameter( "CrossFade", "", 100,   0, 100, flags ));
    MProcessor = new sa_pitch_processor(this);
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
  SAT_PLUGIN_ENTRY(sa_pitch_descriptor,sa_pitch_plugin)
#endif

//----------------------------------------------------------------------
#endif






















#if 0

/* Pitch shifting via overlap and add
 * Copyright (c) 2012
 * All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 * NOTE: This is not Pitch Synchronous Overlap and Add but just Overlap
 *       and Add!
 */

desc:Pitch (OLA)
slider1:100<50,200,1>Pitch (%)
slider2:100<0,200,1>Window (ms)
slider3:100<0,100,1>Cross-Fade (%)

@init
in_pos = 0;
out_pos = 0;
base = 0;
old_base = 0;

@slider
inc = slider1/100;

/* len must be multiple of 2 otherwise there will be drift due to floating
 * point math in 1sdft93hgosdh
 */
len = floor(slider2/1000*srate/2)*2;

fade = slider3/100 * len/2;

@sample
in = spl0;

/* fill buffer */
0[in_pos] = in;

/* increment in_pos */
in_pos = (in_pos+1)%len;

/* generate the output; first calculate the xfade gain; then mix the to
 * from both base and old_base and xfade
 */
gain = min(out_pos/fade, 1);
out = 0[(base+out_pos)%len] * gain + 0[(fade_base+out_pos)%len] * (1-gain);

/* increment the out_pos by inc (pitch!) ref 1sdft93hgosdh and check whether
 * we need to initialize a fade
 */
(out_pos+=inc) >= len-1-fade ? (
	fade_base = base+len-1-fade;
	out_pos = 0; base = in_pos;
);

spl0 = spl1 = out;


#endif // 0