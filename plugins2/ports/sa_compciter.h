#ifndef sa_compciter_included
#define sa_compciter_included
//----------------------------------------------------------------------

// based on the 'compciter' jesusonic effect by 'loser'

/*
  (C) 2007, Michael Gruhn.

  NO WARRANTY IS GRANTED.  THIS PLUG-IN  IS PROVIDED FOR FREE  ON  AN  "AS  IS"
  BASIS, WITHOUT WARRANTY OF ANY KIND.  NO LIABILITY IS GRANTED, INCLUDING, BUT
  NOT LIMITED TO, ANY DIRECT OR INDIRECT,  SPECIAL, INCIDENTAL OR CONSEQUENTIAL
  DAMAGE ARISING OUT OF THE USE  OR INABILITY  TO  USE  THIS PLUG-IN,  COMPUTER
  FAILTURE  OF  MALFUNCTION  INCLUDED.  THE  USE  OF THE  SOURCE  CODE,  EITHER
  PARTIALLY OR IN TOTAL, IS ONLY GRANTED,  IF USED IN THE SENSE OF THE AUTHOR'S
  INTENTION,  AND USED WITH ACKNOWLEDGEMENT OF THE AUTHOR.  LAST BUT NOT LEAST,
  BY USING THIS  PLUG-IN YOU RELINQUISH YOUR CLAIM TO SUE IT'S AUTHOR,  AS WELL
  AS YOUR CLAIM TO ENTRUST SOMEBODY ELSE WITH DOING SO.
*/

//----------------------------------------------------------------------

//#define SAT_NO_GUI

//#define cDenorm   10e-30
#define c_ampdB     8.65617025
#define freqHP      2243.2
#define freqLP      224.32
#define freqHP_p2   (-SAT_PI2 * freqHP)
#define freqLP_p2   ( SAT_PI2 * freqLP)

#include "plugin/sat_plugin.h"
#include "plugin/processor/sat_interleaved_processor.h"

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

clap_plugin_descriptor_t sa_compciter_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = "sa_compciter-v0",
  .name         = "sa_compciter",
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

class sa_compciter_processor
: public SAT_InterleavedProcessor {

//------------------------------
private:
//------------------------------

  bool  need_recalc = true;
  float samplerate  = 0.0;
  
  float threshDB    = 0.0;
  float thresh      = 0.0;
  float ratio       = 0.0;
  float release     = 0.0;
  float t0          = 0.0;
  float t1          = 0.0;
  float blp         = 0.0;
  float alp         = 0.0;
  float wet         = 0.0;
  float dry         = 0.0;

  float t00         = 0.0;
  float t01         = 0.0;
  float t10         = 0.0;
  float t11         = 0.0;
  float t20         = 0.0;
  float t21         = 0.0;

  // @init

  float gain        = 1.0;
  float seekgain    = 1.0;
  float c           = 8.65617025;
  float dc          = 10^-30;

//------------------------------
public:
//------------------------------

  sa_compciter_processor(SAT_ProcessorOwner* AOwner)
  : SAT_InterleavedProcessor(AOwner) {
  }

  //----------

  virtual ~sa_compciter_processor() {
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

  void process(SAT_ProcessContext* AContext) final {
    if (need_recalc) recalc(samplerate);
    SAT_InterleavedProcessor::process(AContext);
  }

  //----------

  void processStereoSample(sat_sample_t* spl0, sat_sample_t* spl1) final {
    float left  = *spl0;
    float right = *spl1;

    float s0,s1;
    t00 = alp * left  - blp * t00;
    t01 = alp * right - blp * t01;
    s0  = t00;
    s1  = t01;
    t10 = alp * s0 - blp * t10;
    t11 = alp * s1 - blp * t11;
    s0  = t10;
    s1  = t11;
    t20 = alp * s0 - blp * t20;
    t21 = alp * s1 - blp * t21;
    s0  = left  - t20;
    s1  = right - t21;
    float rms = fmax( fabs(left) , fabs(right) );
    //seekgain = ((rms > thresh) ? exp((threshDB + (log(rms)*c-threshDB)*ratio) /c) / rms : 1);
    if (rms > thresh) seekgain = exp((threshDB + (log(rms) * c - threshDB) * ratio) / c) / rms;
    else seekgain = 1;
    //gain = ((gain > seekGain) ? seekGain : min( gain/release , seekGain ));
    if (gain > seekgain) {
      gain = seekgain;
    }
    else {
      gain = fmin( gain / release , seekgain );
    }
    *spl0 = (left  * dry) + (s0 * gain * wet);
    *spl1 = (right * dry) + (s1 * gain * wet);
  }

//------------------------------
private:
//------------------------------

  void recalc(float srate) {
    need_recalc = false;
    threshDB    = - getParameterValue(0);
    thresh      = exp(threshDB / c);
    ratio       = 1 / 20;
    release     = exp(-60 / (pow(1 - getParameterValue(1) / 100, 3) * 500 * srate / 1000) / c);
    t0          = 0;
    t1          = 0;
    blp         = -exp(-2 * SAT_PI * getParameterValue(2) * 3 / srate);
    alp         = 1.0 + blp;
    wet         = exp( getParameterValue(3) / c) / exp((threshDB - threshDB * ratio ) / c);
    dry         = exp( getParameterValue(4) / c);
    seekgain    = 1;
  }

};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class sa_compciter_plugin
: public SAT_Plugin {

//------------------------------
private:
//------------------------------

  sa_compciter_processor* MProcessor = nullptr;

//------------------------------
public:
//------------------------------

  sa_compciter_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : SAT_Plugin(ADescriptor,AHost) {
  }

  //----------

  virtual ~sa_compciter_plugin() {
  }

//------------------------------
public:
//------------------------------


  bool init() final {

    registerDefaultExtensions();
    appendStereoAudioInputPort();
    appendStereoAudioOutputPort();

    appendParameter( new SAT_Parameter( "Drive", "", 0,    0,   60    ));
    appendParameter( new SAT_Parameter( "Dist",  "", 25,   0,   100   ));
    appendParameter( new SAT_Parameter( "HP",    "", 5000, 800, 12000 ));
    appendParameter( new SAT_Parameter( "Wet",   "", -6,  -60,  24    ));
    appendParameter( new SAT_Parameter( "Dry",   "", 0,   -12,  0     ));
  //appendParameter( new SAT_Parameter( "Dry",   "", 0,   -120, 0     ));

    MProcessor = new sa_compciter_processor(this);
    setProcessor(MProcessor);

    #ifndef SAT_NO_GUI
      setInitialEditorSize(640,480,2.0,false);
    #endif

    return SAT_Plugin::init();
  }

  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    MProcessor->setSampleRate(sample_rate);
    return SAT_Plugin::activate(sample_rate,min_frames_count,max_frames_count);
  }

//------------------------------
public:
//------------------------------

  #ifndef SAT_NO_GUI

    bool setupEditor(SAT_Editor* AEditor) final {
      SAT_TRACE;

      SAT_Window* window = AEditor->getWindow();
      SAT_RootWidget* root = new SAT_RootWidget( window, SAT_Rect() );
      window->setRootWidget(root);

      SAT_SliderWidget* s0 = new SAT_SliderWidget(SAT_Rect(10,10,300,25));
      root->appendChild(s0);
      AEditor->connect(s0,getParameter(0));

      SAT_SliderWidget* s1 = new SAT_SliderWidget(SAT_Rect(10,40,300,25));
      root->appendChild(s1);
      AEditor->connect(s1,getParameter(1));

      SAT_SliderWidget* s2 = new SAT_SliderWidget(SAT_Rect(10,70,300,25));
      root->appendChild(s2);
      AEditor->connect(s2,getParameter(2));

      SAT_SliderWidget* s3 = new SAT_SliderWidget(SAT_Rect(10,100,300,25));
      root->appendChild(s3);
      AEditor->connect(s3,getParameter(3));

      // SAT_SliderWidget* s4 = new SAT_SliderWidget(SAT_Rect(10,130,300,25));
      // root->appendChild(s4);
      // AEditor->connect(s4,getParameter(4));


      SAT_KnobWidget* k4 = new SAT_KnobWidget(SAT_Rect(10,130,50,50));
      root->appendChild(k4);
      AEditor->connect(k4,getParameter(4));


      return true;
    }

  #endif // gui

  //----------

  // void on_editorListener_timer(SAT_Timer* ATimer, double ADelta) final {
  //   SAT_Plugin::on_editorListener_timer(ATimer,ADelta);
  //   SAT_PRINT("delta %.3f\n",ADelta);
  // }

};

//----------------------------------------------------------------------
//
// entry
//
//----------------------------------------------------------------------

#ifndef SAT_NO_ENTRY
  #include "plugin/sat_entry.h"
  SAT_PLUGIN_ENTRY(sa_compciter_descriptor,sa_compciter_plugin);
#endif

//----------------------------------------------------------------------

//#undef cDenorm
#undef c_ampdB
#undef freqHP
#undef freqLP
#undef freqHP_p2
#undef freqLP_p2

//----------------------------------------------------------------------
#endif
