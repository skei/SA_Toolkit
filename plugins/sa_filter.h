#ifndef sa_filter_included
#define sa_filter_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "audio/sat_audio_utils.h"
#include "audio/old/filters/sat_cytomic_filter.h"
#include "plugin/clap/sat_clap.h"
#include "plugin/sat_parameter.h"
#include "plugin/sat_plugin.h"
#include "gui/sat_widgets.h"


//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#define PLUGIN_NAME "sa_filter"

//----------

const clap_plugin_descriptor_t sa_filter_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = SAT_VENDOR "/" PLUGIN_NAME,
  .name         = PLUGIN_NAME,
  .vendor       = SAT_VENDOR,
  .url          = SAT_URL,
  .manual_url   = "",
  .support_url  = "",
  .version      = SAT_VERSION,
  .description  = "a simple filter plugin",
  .features     = (const char* []) {
                    CLAP_PLUGIN_FEATURE_AUDIO_EFFECT,
                    nullptr
                  }
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

enum sa_filter_param_t {
  SA_FILTER_PAR_TYPE = 0,
  SA_FILTER_PAR_FREQ,
  SA_FILTER_PAR_Q,
  SA_FILTER_PAR_GAIN,
  SA_FILTER_NUM_PARAMETERS
};

#define SA_FILTER_NUM_TYPES 10
const char* sa_filter_type_text[SA_FILTER_NUM_TYPES] = {
  "off",
  "lowpass",
  "bandpass",
  "highpass",
  "notch",
  "peak",
  "allpass",
  "bell",
  "lowshelf",
  "highshelf"
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_filter_plugin
: public SAT_Plugin {
  
//------------------------------
public:
//------------------------------

  SAT_CytomicFilter MFilterLeft = {};
  SAT_CytomicFilter MFilterRight= {};
  
  uint32_t  par_type = _SVF_LOWPASS;
  double    par_freq = 1000;
  double    par_q    = 1.0;
  double    par_gain = 0.5;
  
  uint32_t  mod_type = 0;
  double    mod_freq = 0.0;
  double    mod_q    = 0.0;
  double    mod_gain = 0.0;
  
  bool need_update = true;

//------------------------------
public:
//------------------------------

  SAT_PLUGIN_DEFAULT_CONSTRUCTOR(sa_filter_plugin)

  //----------

  bool init() final {
    registerDefaultExtensions();    
    appendStereoAudioInputPort();
    appendStereoAudioOutputPort();
    
    setInitialEditorSize(300,160,3.0);
    
    SAT_TextParameter*  p_type  = new SAT_TextParameter("Type", 0,    0,  SA_FILTER_NUM_TYPES-1, sa_filter_type_text );
    SAT_Parameter*      p_freq  = new SAT_Parameter(    "Freq", 1000, 16, 22050 );
    SAT_Parameter*      p_q     = new SAT_Parameter(    "Q",    1,    1,  40 );
    SAT_Parameter*      p_gain  = new SAT_Parameter(    "Gain", 0.5 );

    //p_type->setFlag(CLAP_PARAM_IS_MODULATABLE);
    //p_freq->setFlag(CLAP_PARAM_IS_MODULATABLE);
    //p_q->setFlag(CLAP_PARAM_IS_MODULATABLE);
    //p_gain->setFlag(CLAP_PARAM_IS_MODULATABLE);

    appendParameter(p_type);
    appendParameter(p_freq);
    appendParameter(p_q);
    appendParameter(p_gain);
    
    return SAT_Plugin::init();
  }
  
  //----------

  bool initEditorWindow(SAT_Editor* AEditor, SAT_Window* AWindow) final {
    
    // filter type menu
    SAT_MenuWidget* flt_type_menu = new SAT_MenuWidget(SAT_Rect(75,10 * SA_FILTER_NUM_TYPES));
    for (uint32_t i=0; i<SA_FILTER_NUM_TYPES; i++) {
      SAT_MenuItemWidget* item = (SAT_MenuItemWidget*)flt_type_menu->appendChildWidget( new SAT_MenuItemWidget(10,sa_filter_type_text[i]));
      item->setTextSize(7);
    }
    
    //

    // background panel
    SAT_PanelWidget* panel = new SAT_PanelWidget(0);
    AWindow->appendRootWidget(panel);

    // header
    SAT_PluginHeaderWidget* header = new SAT_PluginHeaderWidget(SAT_Rect(0,0,200,40),"filter",getPluginFormat());
    panel->appendChildWidget(header);
    
    //

    // type
    SAT_SelectorWidget* w_type  = new SAT_SelectorWidget( SAT_Rect(10,10,280,20), "Type", flt_type_menu );
    SAT_SliderWidget*   w_freq  = new SAT_SliderWidget(   SAT_Rect(10,35,280,20), "Freq", 0.5 );
    SAT_SliderWidget*   w_q     = new SAT_SliderWidget(   SAT_Rect(10,60,280,20), "Q",    0.5 );
    SAT_SliderWidget*   w_gain  = new SAT_SliderWidget(   SAT_Rect(10,85,280,20), "Gain", 0.5 );

    panel->appendChildWidget(w_type);
    panel->appendChildWidget(w_freq);
    panel->appendChildWidget(w_q);
    panel->appendChildWidget(w_gain);
    
    AEditor->connect( w_type, getParameter(SA_FILTER_PAR_TYPE ));
    AEditor->connect( w_freq, getParameter(SA_FILTER_PAR_FREQ ));
    AEditor->connect( w_q,    getParameter(SA_FILTER_PAR_Q    ));
    AEditor->connect( w_gain, getParameter(SA_FILTER_PAR_GAIN ));
    
    //
    
    panel->appendChildWidget(flt_type_menu);
    
    return true;
  }

  //----------
  
  bool handleParamValue(const clap_event_param_value_t* event) final {
    double value = event->value;
    switch (event->param_id) {
      case SA_FILTER_PAR_TYPE:  par_type  = value;    need_update = true;   return true;
      case SA_FILTER_PAR_FREQ:  par_freq  = value;    need_update = true;   return true;
      case SA_FILTER_PAR_Q:     par_q     = value;    need_update = true;   return true;
      case SA_FILTER_PAR_GAIN:  par_gain  = value;  /*need_update = true;*/ return true;
    }
    return false;
  }
  
  //----------

  void processAudio(SAT_ProcessContext* AContext) final {
    
    float**   inputs  = AContext->process->audio_inputs[0].data32;
    float**   outputs = AContext->process->audio_outputs[0].data32;
    float*    input0  = inputs[0];
    float*    input1  = inputs[1];
    float*    output0 = outputs[0];
    float*    output1 = outputs[1];
    uint32_t  length  = AContext->process->frames_count;
    
    if (need_update) {
      MFilterLeft.resetState();
      MFilterRight.resetState();
      MFilterLeft.updateCoefficients( par_freq, par_q, par_type, AContext->samplerate);
      MFilterRight.updateCoefficients(par_freq, par_q, par_type, AContext->samplerate);
      need_update = false;
    }
    
    if (par_type == 0) {
      for (uint32_t i=0; i<length; i++) {
        *output0++ = *input0++;
        *output1++ = *input1++;
      }
    }
    else {
      for (uint32_t i=0; i<length; i++) {
        *output0++ = MFilterLeft.tick(*input0++);
        *output1++ = MFilterRight.tick(*input1++);
      }
    }
    
    SAT_ScaleStereoBuffer(outputs,par_gain,length);
    
  }
  
  //----------

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#ifndef SAT_NO_ENTRY

  #include "plugin/sat_entry.h"
  SAT_PLUGIN_ENTRY(sa_filter_descriptor,sa_filter_plugin)
  
#endif

//----------

#undef PLUGIN_NAME

//----------------------------------------------------------------------
#endif

