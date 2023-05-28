#ifndef sa_hrtf_included
#define sa_hrtf_included
//----------------------------------------------------------------------


/*
  Copyright (c) 2017 Benno Straub
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
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

const clap_plugin_descriptor_t sa_hrtf_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = SAT_VENDOR "/sa_hrtf",
  .name         = "sa_hrtf",
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

class sa_hrtf_plugin
: public SAT_Plugin {
  
//------------------------------
private:
//------------------------------

  bool  need_recalc = true;
  float MSampleRate = 0.0;

//------------------------------
public:
//------------------------------

  SAT_PLUGIN_DEFAULT_CONSTRUCTOR(sa_hrtf_plugin)

  //----------
  
  bool init() final {
    registerDefaultExtensions();    
    appendStereoInputPort();
    appendStereoAudioOutputPort();
    
    appendParameter(new SAT_Parameter( "Rotate",   0, -180,  180 ));
    appendParameter(new SAT_Parameter( "Slope",    0, -90,   90  ));
    appendParameter(new SAT_Parameter( "Distance", 1,  0.01, 2   ));
    
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
    
    //for (uint32_t i=0; i<len; i++) {
    //  float spl0 = *in0++;
    //  float spl1 = *in1++;
    //  *out0++ = spl0;
    //  *out1++ = spl1;
    //}
    
    float left, right;
    update(&left,&right);
    for (uint32_t i=0; i<len; i++) {
      *out0++ = *in0++ * left;
      *out1++ = *in1++ * right;
    }
    
  }
  
//------------------------------
private:
//------------------------------

  void recalc(float srate) {
    need_recalc = false;
  }

  void update(float* left, float* right) {

    float MRotate = getParameterValue(0);
    //float MSlope = getParameterValue(1);
    float MDistance = getParameterValue(2) * getParameterValue(2);

    float panl = (cosf(((MRotate + 90.0f) * SAT_PI) / 180.0f) + 1.0f) * 0.5f;
    float panr = 1.0f - panl;
    float pan_intensity = 1.0f / MDistance;
    *left  = powf(panl, pan_intensity);
    *right = powf(panr, pan_intensity);
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#ifndef SAT_NO_ENTRY
  #include "plugin/sat_entry.h"
  SAT_PLUGIN_ENTRY(sa_hrtf_descriptor,sa_hrtf_plugin)
#endif

//----------


//----------------------------------------------------------------------
#endif
