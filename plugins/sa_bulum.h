#ifndef sa_bulum_included
#define sa_bulum_included
//----------------------------------------------------------------------

//#define SAT_PLUGIN_GENERIC_EDITOR

#include "plugin/sat_plugin.h"
#include "gui/sat_widgets.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#ifdef SAT_DEBUG
  #define PLUGIN_NAME   "SA_bulum (debug)"
#else
  #define PLUGIN_NAME   "SA_bulum"
#endif

#define EDITOR_WIDTH    400
#define EDITOR_HEIGHT   (40 + 10 + (13 * (20 + 10)) + 20)

#define SAT_MAX_GRAINS  1024
#define SAT_BUFFERSIZE  (1024*1024)

//----------

// todo: not array of structs.. struct of/with array..
struct SAT_Grain {
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

//----------

const char* freeze_txt[2] = { "off", "on" };

//----------------------------------------------------------------------


//----------

const clap_plugin_descriptor_t sa_bulum_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = SAT_VENDOR "/" PLUGIN_NAME "/" SAT_VERSION,
  .name         = PLUGIN_NAME,
  .vendor       = SAT_VENDOR,
  .url          = SAT_URL,
  .manual_url   = "",
  .support_url  = "",
  .version      = SAT_VERSION,
  .description  = "",
  .features     = (const char*[]){ CLAP_PLUGIN_FEATURE_AUDIO_EFFECT, nullptr }
};

//----------

//#define

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_bulum_plugin
: public SAT_Plugin {

//------------------------------
private:
//------------------------------

  const clap_audio_port_info_t sa_bulum_audio_input_ports[1] = {
    { 0, "audio in 1", CLAP_AUDIO_PORT_IS_MAIN, 2, CLAP_PORT_STEREO, CLAP_INVALID_ID }
  };

  const clap_audio_port_info_t sa_bulum_audio_output_ports[1] = {
    { 0, "audio out 1", CLAP_AUDIO_PORT_IS_MAIN, 2, CLAP_PORT_STEREO, CLAP_INVALID_ID }
  };

  float     MSampleRate             = 0.0;

  SAT_Grain MGrains[SAT_MAX_GRAINS] = {0};
  float     MBuffer[SAT_BUFFERSIZE] = {0};

  bool      need_precalc            = true;
  bool      need_recalc             = false;

  float     master      = 0.0;
  int32_t   numgrains   = 0;
  int32_t   buffersize  = 0;
  bool      freeze      = false;
  float     graindist   = 0.0;
  float     grainsize   = 0.0;
  float     graindur    = 0.0;
  float     grainpitch  = 1.0;
  float     grainenv    = 0.0;
  float     startjit    = 0.0;
  float     pitchjit    = 0.0;
  float     sizejit     = 0.0;
  float     durjit      = 0.0;
  int32_t   index       = 0;
  float     countdown   = 0.0;


//------------------------------
public:
//------------------------------

  SAT_PLUGIN_DEFAULT_CONSTRUCTOR(sa_bulum_plugin);

//------------------------------
public: // plugin
//------------------------------

  bool init() final {
    setInitialEditorSize(EDITOR_WIDTH,EDITOR_HEIGHT);
    appendStereoInputPort();
    appendStereoOutputPort();
    registerDefaultExtensions();
    appendParameter(new SAT_Parameter(      "Master",            -6,   -60,  6    ));
    appendParameter(new SAT_IntParameter(   "Number of grains",   10,   1,   100  ));
    appendParameter(new SAT_Parameter(      "Buffer size",        1000, 1,   1000 ));
    appendParameter(new SAT_TextParameter(  "Freeze",             0,    0,   1,   freeze_txt ));
    appendParameter(new SAT_Parameter(      "Grain distance",     20,   0,   100  ));
    appendParameter(new SAT_Parameter(      "Grain size",         30,   1,   100  ));
    appendParameter(new SAT_Parameter(      "Grain duration",     300,  1,   1000 ));
    appendParameter(new SAT_Parameter(      "Grain pitch",        1,    0,   10   ));
    appendParameter(new SAT_Parameter(      "Grain envelope",     0,    0,   1    ));
    appendParameter(new SAT_Parameter(      "Distance jitter",    0.2,  0,   1    ));
    appendParameter(new SAT_Parameter(      "Pitch jitter",       0.2,  0,   1    ));
    appendParameter(new SAT_Parameter(      "Size jitter",        0.2,  0,   1    ));
    appendParameter(new SAT_Parameter(      "Duration jitter",    0.2,  0,   1    ));
    setAllParameterFlags(CLAP_PARAM_IS_MODULATABLE);
    bool result = SAT_Plugin::init();
    if (result) {
//      setDefaultParameterValues();
      need_precalc = true;
      return true;
    }
    return false;
  }

  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    MSampleRate = sample_rate;
    return SAT_Plugin::activate(sample_rate,min_frames_count,max_frames_count);
  }

//  bool initEditorWindow(SAT_Editor* AEditor, SAT_Window* AWindow) final {
//    // background panel
//    SAT_PanelWidget* panel = new SAT_PanelWidget(0);
//    AWindow->appendRootWidget(panel);
//    panel->setDrawBorder(false);
//    // header
//    const char* plugin_format = getPluginFormat();
//    SAT_PluginHeaderWidget* header = new SAT_PluginHeaderWidget(SAT_Rect(0,0,200,40),"bulum",plugin_format);
//    panel->appendChildWidget(header);
//    // footer
//    SAT_PluginFooterWidget* footer = new SAT_PluginFooterWidget(SAT_Rect(0,EDITOR_HEIGHT-20,200,20));
//    panel->appendChildWidget(footer);
//    // sliders
//    for (uint32_t i=0; i<getNumParameters(); i++) {
//      SAT_SliderWidget* slider = new SAT_SliderWidget(SAT_Rect(10, 50 + (i*30),380,20),"",0.0);
//      panel->appendChildWidget(slider);
//      AEditor->connect( slider,  getParameter(i) );
//    }
//    return true;
//  }


//------------------------------
private:
//------------------------------

  void precalc(float srate) {
    need_precalc = false;
    master = pow(2,(getParameterValue(0) / 6));
    numgrains = trunc(getParameterValue(1));
    buffersize = trunc( (getParameterValue(2) / 1000) * MSampleRate );
    if ( getParameterValue(3) > 0.5) freeze = true;
    else freeze = false;
    graindist = ( getParameterValue(4) / 1000) * MSampleRate;
    grainsize = ( getParameterValue(5) / 1000) * MSampleRate;
    graindur = ( getParameterValue(6) / 1000) * MSampleRate;
  }

  //----------

  void recalc(float srate) {
    need_recalc = false;
  }

//------------------------------
private:
//------------------------------

  bool handleParamValue(const clap_event_param_value_t* param_value) final {
    //SAT_Plugin::processParamValue(param_value);
    uint32_t index = param_value->param_id;
    float value = param_value->value;
    SAT_Print("%i = %f\n",index,value);
    switch (index) {
      case 0:  master      = pow(2,(value / 6)); break;
      case 1:  numgrains   = trunc( value ); break;
      case 2:  buffersize  = trunc( (value / 1000) * MSampleRate ); break;
      case 3:  value > 0.5 ? freeze=true : freeze=false; break;
      case 4:  graindist   = (value / 1000) * MSampleRate; break;
      case 5:  grainsize   = (value / 1000) * MSampleRate; break;
      case 6:  graindur    = (value / 1000) * MSampleRate; break;
      case 7:  grainpitch  = value; break;
      case 8:  grainenv    = value; break;
      case 9:  startjit    = value; break;
      case 10: pitchjit    = value; break;
      case 11: sizejit     = value; break;
      case 12: durjit      = value; break;
    }
    need_recalc = true;
    return true;
  }

  //----------
  
  void processAudio(SAT_ProcessContext* AContext) final {
    const clap_process_t* process = AContext->process;
    if (need_precalc) precalc(MSampleRate);
    if (need_recalc) recalc(MSampleRate);

    uint32_t len = process->frames_count;
    float* in0  = process->audio_inputs[0].data32[0];
    float* in1  = process->audio_inputs[0].data32[1];
    float* out0 = process->audio_outputs[0].data32[0];
    float* out1 = process->audio_outputs[0].data32[1];

    float spl0,spl1;
    for (uint32_t i=0; i<len; i++) {
      spl0 = *in0++;
      spl1 = *in1++;
      processSample(&spl0,&spl1);
      *out0++ = spl0;
      *out1++ = spl1;
    }
  }

//------------------------------
private:
//------------------------------

  void processSample(float* spl0, float* spl1) {
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

  //----------

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#ifndef SAT_NO_ENTRY
  #include "plugin/sat_entry.h"
  SAT_PLUGIN_ENTRY(sa_bulum_descriptor,sa_bulum_plugin);
#endif

//----------------------------------------------------------------------
#endif