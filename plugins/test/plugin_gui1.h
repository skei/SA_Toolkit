
#include "plugin/sat_plugin.h"
#include "plugin/processor/sat_interleaved_processor.h"

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

clap_plugin_descriptor_t myDescriptor = {
  .clap_version = CLAP_VERSION,
  .id           = "myPlugin-v0",
  .name         = "myPlugin",
  .vendor       = SAT_VENDOR,
  .url          = SAT_URL,
  .manual_url   = "",
  .support_url  = "",
  .version      = SAT_VERSION,
  .description  = "my plugin..",
  .features     = (const char*[]){ CLAP_PLUGIN_FEATURE_AUDIO_EFFECT, nullptr }
};

//----------------------------------------------------------------------
//
// processor
//
//----------------------------------------------------------------------

class myProcessor
: public SAT_InterleavedProcessor {

//------------------------------
public:
//------------------------------

  myProcessor(SAT_ProcessorOwner* AOwner)
  : SAT_InterleavedProcessor(AOwner) {
  }

  //----------

  virtual ~myProcessor() {
  }

//------------------------------
public:
//------------------------------

  void paramValueEvent(const clap_event_param_value_t* event) final {
    uint32_t id = event->param_id;
    sat_param_t value = event->value;
    //SAT_PRINT("%i = %.3f\n",id,value);
  }

  //----------

  // void processStereoSample(sat_sample_t* spl0, sat_sample_t* spl1) final {
  // }

};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class myPlugin
: public SAT_Plugin {

//------------------------------
public:
//------------------------------

  myPlugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : SAT_Plugin(ADescriptor,AHost) {
  }

  //----------

  virtual ~myPlugin() {
  }

//------------------------------
public:
//------------------------------

  bool init() final {
    registerDefaultExtensions();
    appendStereoAudioInputPort();
    appendStereoAudioOutputPort();

    SAT_Parameter* p1 = new SAT_Parameter( "param1", "", 0.0, -2, 10 );
    SAT_Parameter* p2 = new SAT_Parameter( "param2", "", 1.0,  0, 10 );
    SAT_Parameter* p3 = new SAT_Parameter( "param3", "", 2.0,  0, 10 );
    SAT_Parameter* p4 = new SAT_Parameter( "param4", "", 3.0, -2, 10 );
    SAT_Parameter* p5 = new SAT_Parameter( "param5", "", 4.0, -2, 10 );
    SAT_Parameter* p6 = new SAT_Parameter( "p6",     "", 5.0, -2, 10 );

    p2->setFlag(CLAP_PARAM_IS_STEPPED);
    p3->setFlag(CLAP_PARAM_IS_STEPPED);
    p4->setFlag(CLAP_PARAM_IS_STEPPED);
    p5->setFlag(CLAP_PARAM_IS_STEPPED);

    appendParameter(p1);
    appendParameter(p2);
    appendParameter(p3);
    appendParameter(p4);
    appendParameter(p5);
    appendParameter(p6);

    setProcessor( new myProcessor(this) );
    setInitialEditorSize(640,480,1.0,false);
    return SAT_Plugin::init();
  }

//------------------------------
public:
//------------------------------

  #ifndef SAT_NO_GUI

    bool setupEditor(SAT_Editor* AEditor) final {
      //SAT_TRACE;

      SAT_Window* window = AEditor->getWindow();
      SAT_RootWidget* root = new SAT_RootWidget( window, SAT_Rect() );
      window->setRootWidget(root);

        root->Layout.innerBorder = SAT_Rect(10,10,10,10);

        SAT_SliderWidget* s1 = new SAT_SliderWidget(SAT_Rect(  10, 10, 620, 30 ));
        SAT_SliderWidget* s2 = new SAT_SliderWidget(SAT_Rect(  10, 50, 620, 30 ));
        SAT_SliderWidget* s3 = new SAT_SliderWidget(SAT_Rect(  10, 90, 620, 30 ));
        SAT_SliderWidget* s4 = new SAT_SliderWidget(SAT_Rect(  10,130, 620, 30 ));
        SAT_SliderWidget* s5 = new SAT_SliderWidget(SAT_Rect(  10,170, 620, 30 ));

        SAT_KnobWidget*   k1 = new SAT_KnobWidget(  SAT_Rect(  10,210, 100,100));

        s2->setDragSnap(true);
        s2->setDrawValueBar(true);

        s3->setDragQuantize(true);
        s3->setDragQuantizeSteps(11);
        s3->setDrawQuantized(true);
        s3->setDrawValueBar(true);

        s4->setDragQuantize(true);
        s4->setDragQuantizeSteps(13);
        s4->setDrawQuantized(true);
        s4->setDrawValueBar(true);

        k1->setDrawKnobArcBackground(true);
        k1->setKnobArcThickness(15);
        k1->setTextSize(20);
        k1->setValueTextSize(25);

        root->appendChild(s1);
        root->appendChild(s2);
        root->appendChild(s3);
        root->appendChild(s4);
        root->appendChild(s5);
        root->appendChild(k1);

        AEditor->connect(s1,getParameter(0));
        AEditor->connect(s2,getParameter(1));
        AEditor->connect(s3,getParameter(2));
        AEditor->connect(s4,getParameter(3));
        AEditor->connect(s5,getParameter(4));
        AEditor->connect(k1,getParameter(5));

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

#include "plugin/sat_entry.h"
SAT_PLUGIN_ENTRY(myDescriptor,myPlugin);

