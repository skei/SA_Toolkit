#ifndef sa_limiter_included
#define sa_limiter_included
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

const clap_plugin_descriptor_t sa_limiter_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = SAT_VENDOR "/sa_limiter/v0",
  .name         = "sa_limiter",
  .vendor       = SAT_VENDOR,
  .url          = SAT_URL,
  .manual_url   = "",
  .support_url  = "",
  .version      = SAT_VERSION,
  .description  = "",
  .features     = (const char*[]){ CLAP_PLUGIN_FEATURE_AUDIO_EFFECT, CLAP_PLUGIN_FEATURE_LIMITER, nullptr }
};

//----------------------------------------------------------------------
//
// processor
//
//----------------------------------------------------------------------

class sa_limiter_processor
: public SAT_InterleavedProcessor {

//------------------------------
private:
//------------------------------

  bool  need_recalc = true;
  float samplerate = 0.0;

  float     C1 = 0.0f;
  float     C2 = 0.0f;
  float     C3 = 0.0f;
  uint32_t  len = 0;
  uint32_t  pos = 0;
  float     thresh = 0.0f;
  float     make_gain = 0.0f;
  float     ceiling = 0.0f;

//  uint32_t  bufl = 0;
//  uint32_t  bufr = 0;
//  float     BUFFER[1024*1024] = {0};

  uint32_t  buf = 0;
  float     LBUFFER[1024*1024] = {0};
  float     RBUFFER[1024*1024] = {0};

  //float     slider1 = 0.0f;
  //float     slider2 = 0.0f;

  float out1 = 0.0f;
  float out2 = 0.0f;
  float out3 = 0.0f;

  
//------------------------------
public:
//------------------------------

  sa_limiter_processor(SAT_ProcessorListener* AListener)
  : SAT_InterleavedProcessor(AListener) {
  }

  //----------

  virtual ~sa_limiter_processor() {
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

      float in = SAT_Max(fabs(spl0),fabs(spl1));
      // smooth via a rc low pass
      out1 = (in   > out1) ? in   : in   + C1 * (out1-in);
      out2 = (out1 > out2) ? out1 : out1 + C2 * (out2-out1);
      out3 = out2 + C3 * (out3-out2);
      float out = out3;
      //BUFFER[bufl+pos] = spl0;
      //BUFFER[bufr+pos] = spl1;
      LBUFFER[buf+pos] = spl0;
      RBUFFER[buf+pos] = spl1;
      pos = (pos + 1) % len;
      float gain = ((out > thresh) ? (thresh / out) : 1.0) * make_gain;
      //spl0 = BUFFER[bufl+pos] * gain;
      spl0 = LBUFFER[buf+pos] * gain;
      spl0 = SAT_Max(SAT_Min(spl0,ceiling),-ceiling);
      //spl1 = BUFFER[bufr+pos] * gain;
      spl1 = RBUFFER[buf+pos] * gain;
      spl1 = SAT_Max(SAT_Min(spl1,ceiling),-ceiling);

      *output0++ = spl0;
      *output1++ = spl1;
    }    
  }

//------------------------------
private:
//------------------------------

  void recalc(float srate) {
    C1    = exp(-1.0 / (25.0 / 1000.0 * srate));
    C2    = exp(-1.0 / (25.0 / 1000.0 * srate));
    C3    = exp(-1.0 / (5.0 / 1000.0 * srate));
    len   = (srate * 10.0) / 1000.0; //10/1000*srate;
    //bufl  = 0;
    //bufr  = len;
    buf   = 0;
    pos   = 0;
    thresh    = powf(10.0,(  getParameterValue(0) / 20.0));
    make_gain = powf(10.0,((-getParameterValue(0) + getParameterValue(1)) / 20.0));
    ceiling   = powf(10.0,(  getParameterValue(1) / 20.0));
    need_recalc = false;
  }

};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class sa_limiter_plugin
: public SAT_Plugin {
  
//------------------------------
private:
//------------------------------

  sa_limiter_processor* MProcessor = nullptr;

//------------------------------
public:
//------------------------------

  sa_limiter_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : SAT_Plugin(ADescriptor,AHost) {
  }

  //----------

  virtual ~sa_limiter_plugin() {
  }

//------------------------------
public:
//------------------------------

  bool init() final {
    registerDefaultExtensions();    
    appendStereoAudioInputPort("In");
    appendStereoAudioOutputPort("Out");
    uint32_t flags = CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_MODULATABLE;
    appendParameter(new SAT_Parameter( "Threshold", "", 0,  -30, 0, flags ));
    appendParameter(new SAT_Parameter( "Ceiling",   "", 0,  -30, 0, flags ));
    MProcessor = new sa_limiter_processor(this);
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
  SAT_PLUGIN_ENTRY(sa_limiter_descriptor,sa_limiter_plugin)
#endif

//----------------------------------------------------------------------
#endif







/* Copyright (c) 2012, Michael Gruhn
 * All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF
 * THIS SOFTWARE.
 */

//  desc:Limiter
//  slider1:0<-30,0,1>Threshold (dB)
//  slider2:0<-30,0,1>Ceiling (dB)
//
//  @init
//  /* rc smoothing filters */
//  C1 = exp(-1/(25/1000*srate));
//  C2 = exp(-1/(25/1000*srate));
//  C3 = exp(-1/(5/1000*srate));
//
//  len = 10/1000*srate;
//  bufl = 0;
//  bufr = len;
//  pos = 0;
//
//  pdc_delay = len;
//  pdc_bot_ch = 0;
//  pdc_top_ch = 2;
//
//  @slider
//  thresh = 10^(slider1/20);
//  make_gain = 10^((-slider1+slider2)/20);
//  ceiling = 10^(slider2/20);
//
//  @sample
//  in = max(abs(spl0),abs(spl1));
//
//  /* smooth via a rc low pass */
//  out1 = in  >out1 ? in   : in   + C1 * (out1-in);
//  out2 = out1>out2 ? out1 : out1 + C2 * (out2-out1);
//  out3 = out2 + C3 * (out3-out2);
//
//  out = out3;
//
//  bufl[pos] = spl0;
//  bufr[pos] = spl1;
//
//  pos = (pos+1)%len;
//
//  gain = (out > thresh ? thresh/out : 1)*make_gain;
//
//  spl0 = bufl[pos] * gain;
//  spl0 = max(min(spl0,ceiling),-ceiling);
//  spl1 = bufr[pos] * gain;
//  spl1 = max(min(spl1,ceiling),-ceiling);
