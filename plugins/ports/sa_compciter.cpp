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
  .id           = SAT_VENDOR "/sa_compciter/v0",
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

  sa_compciter_processor(SAT_ProcessorListener* AListener)
  : SAT_InterleavedProcessor(AListener) {
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
    MProcessor = new sa_compciter_processor(this);
    setProcessor(MProcessor);
    #ifndef SAT_NO_GUI
      setInitialEditorSize(510,340,1.0,true);
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

//  /*

  #ifndef SAT_NO_GUI
  
    bool setupEditor(SAT_Editor* AEditor) final {
      //SAT_TRACE;
      SAT_Window* window = AEditor->getWindow();
      SAT_RootWidget* root = new SAT_RootWidget(window);
      window->setRootWidget(root);
  
      //
  
      SAT_PluginHeaderWidget* header = new SAT_PluginHeaderWidget(40,"compciter");
      root->appendChild(header);
      header->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      header->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
      header->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;
  
      SAT_PluginFooterWidget* footer = new SAT_PluginFooterWidget(20,"  ...");
      root->appendChild(footer);
      footer->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_BOTTOM_LEFT;
      footer->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
      footer->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_BOTTOM;
  
      SAT_VisualWidget* center = new SAT_VisualWidget(0);
      root->appendChild(center);
      center->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      center->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_ALL;
  
      //
  
      SAT_KnobWidget* knob0 = new SAT_KnobWidget(SAT_Rect(30,30,200,200));
      center->appendChild(knob0);
      AEditor->connect(knob0,getParameter(0));
      knob0->setTextSize(20);
      knob0->setValueTextSize(30);
      knob0->setKnobArcColor(SAT_Red);
      knob0->setKnobArcThickness(25);
      knob0->setDrawKnobNeedle(true);
      knob0->setKnobNeedleStart(0.57);
      knob0->setKnobNeedleLength(0.65);
      knob0->setKnobNeedleThickness(10);
  
      SAT_KnobWidget* knob1 = new SAT_KnobWidget(SAT_Rect(280,30,90,90));
      center->appendChild(knob1);
      AEditor->connect(knob1,getParameter(1));
      knob1->setTextSize(10);
      knob1->setValueTextSize(10);
      knob1->setKnobArcColor(SAT_Blue1);
      knob1->setKnobArcThickness(15);
      knob1->setDrawKnobNeedle(true);
      knob1->setKnobNeedleStart(0.66);
      knob1->setKnobNeedleLength(1.0);
      knob1->setKnobNeedleThickness(2);
  
      SAT_KnobWidget* knob2 = new SAT_KnobWidget(SAT_Rect(390,30,90,90));
      center->appendChild(knob2);
      AEditor->connect(knob2,getParameter(2));
      knob2->setTextSize(10);
      knob2->setValueTextSize(10);
      knob2->setKnobArcColor(SAT_Blue1);
      knob2->setKnobArcThickness(15);
      knob2->setDrawKnobNeedle(true);
      knob2->setKnobNeedleStart(0.66);
      knob2->setKnobNeedleLength(1.0);
      knob2->setKnobNeedleThickness(2);
  
      SAT_KnobWidget* knob3 = new SAT_KnobWidget(SAT_Rect(290,150,70,70));
      center->appendChild(knob3);
      AEditor->connect(knob3,getParameter(3));
      knob3->setTextSize(10);
      knob3->setValueTextSize(10);
      knob3->setKnobArcColor(SAT_DarkerGrey);
      knob3->setKnobArcThickness(13);
      knob3->setDrawKnobNeedle(true);
      knob3->setKnobNeedleStart(0.66);
      knob3->setKnobNeedleLength(1.0);
      knob3->setKnobNeedleThickness(2);
  
      SAT_KnobWidget* knob4 = new SAT_KnobWidget(SAT_Rect(400,150,70,70));
      center->appendChild(knob4);
      AEditor->connect(knob4,getParameter(4));
      knob4->setTextSize(10);
      knob4->setValueTextSize(10);
      knob4->setKnobArcColor(SAT_DarkerGrey);
      knob4->setKnobArcThickness(13);
      knob4->setDrawKnobNeedle(true);
      knob4->setKnobNeedleStart(0.66);
      knob4->setKnobNeedleLength(1.0);
      knob4->setKnobNeedleThickness(2);
  
      return true;
    }
  
  #endif // nogui

//  */

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
