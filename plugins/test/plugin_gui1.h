
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
    SAT_PRINT("%i = %.3f\n",id,value);
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
private:
//------------------------------

  const char* MButtonTexts[15] = {
    "1", "2", "3", "4", "5", 
    "6", "7", "8", "9", "10", 
    "11", "12", "13", "14", "15"
  };

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
    SAT_Parameter* p7 = new SAT_Parameter( "p7",     "", 0,    0, 1  );

    p2->setFlag(CLAP_PARAM_IS_STEPPED);
    p3->setFlag(CLAP_PARAM_IS_STEPPED);
    p4->setFlag(CLAP_PARAM_IS_STEPPED);
    p5->setFlag(CLAP_PARAM_IS_STEPPED);
    p7->setFlag(CLAP_PARAM_IS_ENUM);

    appendParameter(p1);
    appendParameter(p2);
    appendParameter(p3);
    appendParameter(p4);
    appendParameter(p5);
    appendParameter(p6);
    appendParameter(p7);

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

        SAT_SliderWidget* s1 = new SAT_SliderWidget(SAT_Rect(  10, 10, 480, 30 ));
        root->appendChild(s1);
        AEditor->connect(s1,getParameter(0));

        SAT_SliderWidget* s2 = new SAT_SliderWidget(SAT_Rect(  10, 50, 480, 30 ));
        root->appendChild(s2);
        AEditor->connect(s2,getParameter(1));
        s2->setDragSnap(true);
        s2->setDrawValueBar(true);

        SAT_SliderWidget* s3 = new SAT_SliderWidget(SAT_Rect(  10, 90, 480, 30 ));
        root->appendChild(s3);
        AEditor->connect(s3,getParameter(2));
        s3->setDragQuantize(true);
        s3->setDragQuantizeSteps(11);
        s3->setDrawQuantized(true);
        s3->setDrawValueBar(true);

        SAT_SliderWidget* s4 = new SAT_SliderWidget(SAT_Rect(  10,130, 480, 30 ));
        root->appendChild(s4);
        AEditor->connect(s4,getParameter(3));
        s4->setDragQuantize(true);
        s4->setDragQuantizeSteps(13);
        s4->setDrawQuantized(true);
        s4->setDrawValueBar(true);

        SAT_SliderWidget* s5 = new SAT_SliderWidget(SAT_Rect(  10,170, 480, 30 ));
        root->appendChild(s5);
        AEditor->connect(s5,getParameter(4));
        s5->setDrawDirection(SAT_DIRECTION_RIGHT);

        SAT_KnobWidget* k1 = new SAT_KnobWidget(  SAT_Rect(  10,210, 100,100));
        root->appendChild(k1);
        AEditor->connect(k1,getParameter(5));
        k1->setDrawKnobArcBackground(true);
        k1->setKnobArcThickness(15);
        k1->setTextSize(20);
        k1->setValueTextSize(25);

        SAT_ButtonWidget* b1 = new SAT_ButtonWidget(SAT_Rect( 120,210, 100, 30));
        root->appendChild(b1);
        AEditor->connect(b1,getParameter(6));
        b1->setTexts("Off","On");
        b1->setDrawParamText(false);

        SAT_GridWidget* g1 = new SAT_GridWidget(SAT_Rect( 120,250, 300,100),5,3);
        root->appendChild(g1);
        g1->setSelectCell(true);
        g1->setSelectMultipleCells(true);
        g1->setDrawSelectedCells(true);

        SAT_ButtonGridWidget* bg1 = new SAT_ButtonGridWidget(SAT_Rect(120,360,300,100),5,3);
        root->appendChild(bg1);
        bg1->setButtonTexts(MButtonTexts);

        SAT_SliderWidget* s6 = new SAT_SliderWidget(SAT_Rect(430,210,30,140));
        root->appendChild(s6);
        //AEditor->connect(s6,getParameter(4));
        s6->setDrawDirection(SAT_DIRECTION_UP);
        s6->setDrawText(false);
        s6->setValueTextAlignment(SAT_TEXT_ALIGN_CENTER);

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

