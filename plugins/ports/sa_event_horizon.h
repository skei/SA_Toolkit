#ifndef sa_event_horizon_included
#define sa_event_horizon_included
//----------------------------------------------------------------------

/*
  based on:
    event horizon (peak-eating limiter) jesusonic plugin
    Copyright 2006, Thomas Scott Stillwell
    see bottom of this file for original and license info
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

#define _log2db (20.0f / logf(10.0f))
#define _db2log (logf(10.0f) / 20.0f)


//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t sa_event_horizon_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = SAT_VENDOR "/sa_event_horizon",
  .name         = "sa_event_horizon",
  .vendor       = SAT_VENDOR,
  .url          = SAT_URL,
  .manual_url   = "",
  .support_url  = "",
  .version      = SAT_VERSION,
  .description  = "",
  .features     = (const char*[]) {
                    CLAP_PLUGIN_FEATURE_AUDIO_EFFECT,
                    CLAP_PLUGIN_FEATURE_LIMITER,
                    nullptr
                  }
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_event_horizon_plugin
: public SAT_Plugin {
  
//------------------------------
private:
//------------------------------

  bool  need_recalc = true;
  float MSampleRate = 0.0;
  
  float thresh      = 0.0;
  float threshdb    = 0.0;
  float ceiling     = 0.0;
  float ceildb      = 0.0;
  float makeupdb    = 0.0;
  float makeup      = 0.0;
  float sc          = 0.0;
  float scv         = 0.0;
  float sccomp      = 0.0;
  float peakdb      = 0.0;
  float peaklvl     = 0.0;
  float scratio     = 0.0;
  float scmult      = 0.0;

//------------------------------
public:
//------------------------------

  SAT_PLUGIN_DEFAULT_CONSTRUCTOR(sa_event_horizon_plugin)

  //----------
  
  bool init() final {
    registerDefaultExtensions();    
    appendStereoInputPort();
    appendStereoAudioOutputPort();
    appendParameter(new SAT_Parameter( "Threshold", 0,   -30, 0 ));
    appendParameter(new SAT_Parameter( "Ceiling",  -0.1, -20, 0 ));
    appendParameter(new SAT_Parameter( "Soft Clip", 2,     0, 6 ));
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
      
      //float peak = SMax(abs(spl0),abs(spl1));
      spl0 *= makeup;
      spl1 *= makeup;

      float sign0 = SAT_Sign(spl0);
      float sign1 = SAT_Sign(spl1);
      float abs0 = fabs(spl0);
      float abs1 = fabs(spl1);

      float overdb0 = 2.08136898 * logf(abs0) * _log2db - ceildb; // c++ Log = pascal Ln ?????
      float overdb1 = 2.08136898 * logf(abs1) * _log2db - ceildb;

      if (abs0 > scv) spl0 = sign0 * (scv + expf(overdb0*scmult)*_db2log);
      if (abs1 > scv) spl1 = sign1 * (scv + expf(overdb1*scmult)*_db2log);

      spl0 = SAT_Min(ceiling,fabs(spl0)) * SAT_Sign(spl0);
      spl1 = SAT_Min(ceiling,fabs(spl1)) * SAT_Sign(spl1);

      *out0++ = spl0;
      *out1++ = spl1;
    }
  }
  
//------------------------------
private:
//------------------------------

  void recalc(float srate) {
    need_recalc = false;
    thresh      = expf(getParameterValue(0) * _db2log);
    threshdb    = getParameterValue(0);
    ceiling     = expf(getParameterValue(1) * _db2log);
    ceildb      = getParameterValue(1);
    makeup      = expf((ceildb-threshdb) * _db2log);
    makeupdb    = ceildb - threshdb;
    sc          = -getParameterValue(2);
    scv         = expf( sc * _db2log);
    sccomp      = expf(-sc * _db2log);
    peakdb      = ceildb + 25.0f;
    peaklvl     = expf(peakdb * _db2log);
    scratio     = getParameterValue(2);   // 3??
    scmult      = fabs((ceildb - sc) / (peakdb - sc));
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#ifndef SAT_NO_ENTRY
  #include "plugin/sat_entry.h"
  SAT_PLUGIN_ENTRY(sa_event_horizon_descriptor,sa_event_horizon_plugin)
#endif



  //  inc = slider1/100;
  //  // len must be multiple of 2 otherwise there will be drift due to floating point math in 1sdft93hgosdh
  //  len = floor(slider2/1000*srate/2)*2;
  //  fade = slider3/100 * len/2;

//----------

#undef _log2db
#undef _db2log

//----------------------------------------------------------------------
#endif





/*

// Copyright 2006, Thomas Scott Stillwell
// All rights reserved.
//
//Redistribution and use in source and binary forms, with or without modification, are permitted
//provided that the following conditions are met:
//
//Redistributions of source code must retain the above copyright notice, this list of conditions
//and the following disclaimer.
//
//Redistributions in binary form must reproduce the above copyright notice, this list of conditions
//and the following disclaimer in the documentation and/or other materials provided with the distribution.
//
//The name of Thomas Scott Stillwell may not be used to endorse or
//promote products derived from this software without specific prior written permission.
//
//THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
//IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
//FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
//BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
//PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
//STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
//THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
desc:peak-eating limiter

slider1:0.0<-30.0,0.0,0.1>Threshold
slider2:-0.1<-20.0,0.0,0.1>Ceiling
slider3:2.0<0,6.0,0.01>Soft clip (dB)
//slider4:10<3,20,0.1>Soft clip ratio

@init
  pi = 3.1415926535;
  log2db = 8.6858896380650365530225783783321; // 20 / ln(10)
  db2log = 0.11512925464970228420089957273422; // ln(10) / 20

@slider
  thresh=exp(slider1 * db2log);
  threshdb=slider1;
  ceiling=exp(slider2 * db2log);
  ceildb=slider2;
  makeup=exp((ceildb-threshdb) * db2log);
  makeupdb=ceildb-threshdb;
  sc = -slider3;
  scv = exp(sc * db2log);
  sccomp = exp(-sc * db2log);
  peakdb = ceildb+25;
  peaklvl = exp(peakdb * db2log);
  scratio = slider4;
  scmult = abs((ceildb - sc) / (peakdb - sc));

@sample
  peak=max(abs(spl0),abs(spl1));
  spl0=spl0*makeup;
  spl1=spl1*makeup;
  sign0 = sign(spl0);
  sign1 = sign(spl1);
  abs0=abs(spl0);
  abs1=abs(spl1);
  overdb0 = 2.08136898 * log(abs0) * log2db - ceildb;
  overdb1 = 2.08136898 * log(abs1) * log2db - ceildb;
  abs0 > scv ? (
    spl0=sign0*(scv + exp(overdb0*scmult)*db2log);
  );
  abs1 > scv ? (
    spl1=sign1*(scv + exp(overdb1*scmult)*db2log);
  );
  spl0=min(ceiling,abs(spl0))*sign(spl0);
  spl1=min(ceiling,abs(spl1))*sign(spl1);

*/