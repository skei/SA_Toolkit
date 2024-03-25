
#include "plugin/sat_parameters.h"
#include "plugin/sat_plugin.h"
#include "gui/sat_widgets.h"

//----------------------------------------------------------------------

const clap_plugin_descriptor_t myDescriptor = {
  .clap_version = CLAP_VERSION,
  .id           = "me/test_params",
  .name         = "test_params",
  .vendor       = "me",
  .url          = "https://website.com/myPlugin",
  .manual_url   = "https://website.com/myPlugin/manual.pdf",
  .support_url  = "https://website.com/support",
  .version      = "0.0.0",
  .description  = "my precious plugin",
  .features     = (const char*[]) { CLAP_PLUGIN_FEATURE_AUDIO_EFFECT, nullptr }
};

//----------------------------------------------------------------------

class myPlugin
: public SAT_Plugin {

//------------------------------
public:
//------------------------------

  SAT_DEFAULT_PLUGIN_CONSTRUCTOR(myPlugin);

//------------------------------
public:
//------------------------------

  bool init() final {

    registerDefaultExtensions();
    appendStereoAudioInputPort("Input");
    appendStereoAudioOutputPort("Output");

    appendParameter( new SAT_Parameter(     "p0 0..5",              0, 0, 5, CLAP_PARAM_IS_AUTOMATABLE ));
    appendParameter( new SAT_Parameter(     "p1 (stepped) 0..5",    0, 0, 5, CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED ));
    appendParameter( new SAT_Parameter(     "p2 (enum) 0..5",       0, 0, 5, CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED | CLAP_PARAM_IS_ENUM ));

    appendParameter( new SAT_Parameter(     "p3 -5..5",             0, -5, 5, CLAP_PARAM_IS_AUTOMATABLE ));
    appendParameter( new SAT_Parameter(     "p4 (stepped) -5..5",   0, -5, 5, CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED ));
    appendParameter( new SAT_Parameter(     "p5 (enum) -5..5",      0, -5, 5, CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED | CLAP_PARAM_IS_ENUM ));

    appendParameter( new SAT_IntParameter(  "ip6 0..5",             0, 0, 5, CLAP_PARAM_IS_AUTOMATABLE ));
    appendParameter( new SAT_IntParameter(  "ip7 (stepped) 0..5",   0, 0, 5, CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED ));
    appendParameter( new SAT_IntParameter(  "ip8 (enum) 0..5",      0, 0, 5, CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED | CLAP_PARAM_IS_ENUM ));

    appendParameter( new SAT_IntParameter(  "ip9 -5..5",            0, -5, 5, CLAP_PARAM_IS_AUTOMATABLE ));
    appendParameter( new SAT_IntParameter(  "ip10 (stepped) -5..5", 0, -5, 5, CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED ));
    appendParameter( new SAT_IntParameter(  "ip11 enum -5..5",      0, -5, 5, CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED | CLAP_PARAM_IS_ENUM ));

    appendParameter( new SAT_Parameter(     "pm0 0..5",              0, 0, 5, CLAP_PARAM_IS_AUTOMATABLE ));
    appendParameter( new SAT_Parameter(     "pm1 (stepped) 0..5",    0, 0, 5, CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED ));

    setInitialEditorSize( 430, 270, 3.0 );
    return SAT_Plugin::init();
  }

  //----------

  // void processStereoSample(sat_sample_t* spl0, sat_sample_t* spl1) final {
  //   sat_param_t gain = getModulatedParameterValue(0);
  //   *spl0 *= gain;
  //   *spl1 *= gain;
  // }

  //----------

  bool on_plugin_paramValue(const clap_event_param_value_t* event) final {
    SAT_Print("index %i value %.3f\n",event->param_id,event->value);
    return true;
  }


  //----------

  void setupEditorWindow(SAT_Editor* AEditor, SAT_Window* AWindow) final {
    SAT_RootWidget* root = new SAT_RootWidget(AWindow);
    AWindow->setRootWidget(root);

    SAT_SliderWidget* s0 = new SAT_SliderWidget( SAT_Rect(10,10,180,15), "", 0 );
    root->appendChildWidget(s0);
    AEditor->connect( s0, getParameter(0) );

    SAT_SliderWidget* s1 = new SAT_SliderWidget( SAT_Rect(10,30,180,15), "", 0 );
    root->appendChildWidget(s1);
    AEditor->connect( s1, getParameter(1) );

    SAT_SliderWidget* s2 = new SAT_SliderWidget( SAT_Rect(10,50,180,15), "", 0 );
    root->appendChildWidget(s2);
    AEditor->connect( s2, getParameter(2) );

    SAT_SliderWidget* s3 = new SAT_SliderWidget( SAT_Rect(10,70,180,15), "", 0 );
    root->appendChildWidget(s3);
    AEditor->connect( s3, getParameter(3) );

    SAT_SliderWidget* s4 = new SAT_SliderWidget( SAT_Rect(10,90,180,15), "", 0 );
    root->appendChildWidget(s4);
    AEditor->connect( s4, getParameter(4) );

    SAT_SliderWidget* s5 = new SAT_SliderWidget( SAT_Rect(10,110,180,15), "", 0 );
    root->appendChildWidget(s5);
    AEditor->connect( s5, getParameter(5) );

    SAT_SliderWidget* s6 = new SAT_SliderWidget( SAT_Rect(10,130,180,15), "", 0 );
    root->appendChildWidget(s6);
    AEditor->connect( s6, getParameter(6) );

    SAT_SliderWidget* s7 = new SAT_SliderWidget( SAT_Rect(10,150,180,15), "", 0 );
    root->appendChildWidget(s7);
    AEditor->connect( s7, getParameter(7) );

    SAT_SliderWidget* s8 = new SAT_SliderWidget( SAT_Rect(10,170,180,15), "", 0 );
    root->appendChildWidget(s8);
    AEditor->connect( s8, getParameter(8) );

    SAT_SliderWidget* s9 = new SAT_SliderWidget( SAT_Rect(10,190,180,15), "", 0 );
    root->appendChildWidget(s9);
    AEditor->connect( s9, getParameter(9) );

    SAT_SliderWidget* s10 = new SAT_SliderWidget( SAT_Rect(10,210,180,15), "", 0 );
    root->appendChildWidget(s10);
    AEditor->connect( s10, getParameter(10) );

    SAT_SliderWidget* s11 = new SAT_SliderWidget( SAT_Rect(10,230,180,15), "", 0 );
    root->appendChildWidget(s11);
    AEditor->connect( s11, getParameter(11) );

    // knob->setArcThickness(25);
    // knob->setTextSize(30);
    // knob->setValueSize(45);
    // AEditor->connect( knob, getParameter(0) );

    //

    SAT_SliderWidget* sm0 = new SAT_SliderWidget( SAT_Rect(200,10,180,15), "", 0 );
    root->appendChildWidget(sm0);
    sm0->setNumValues(2);
    sm0->setValue(0.75,1);
    sm0->setCursor(SAT_CURSOR_ARROW_LEFT_RIGHT);    
    sm0->setDragDirection(SAT_DIRECTION_RIGHT);    
    //AEditor->connect( sm0, getParameter(12)   );
    //AEditor->connect( sm0, getParameter(13),1 );

  }

};

//----------------------------------------------------------------------

#include "plugin/sat_entry.h"
SAT_PLUGIN_ENTRY(myDescriptor,myPlugin);
