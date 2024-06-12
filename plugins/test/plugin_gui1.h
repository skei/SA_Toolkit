
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

  const char* MButtonTexts[10*6] = {
    "0",   "1",  "2",  "3",  "4",  "5",  "6",  "7",  "8",  "9",
    "10", "11", "12", "13", "14", "15", "16", "17", "18", "19",
    "20", "21", "22", "23", "24", "25", "26", "27", "28", "29",
    "30", "31", "32", "33", "34", "35", "36", "37", "38", "39",
    "40", "41", "42", "43", "44", "45", "46", "47", "48", "49",
    "50", "51", "52", "53", "54", "55", "56", "57", "58", "59"
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
    SAT_Parameter* p8 = new SAT_Parameter( "p7",     "", 0,    0, 1  );

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
    appendParameter(p8);

    setProcessor( new myProcessor(this) );
    setInitialEditorSize(640,480+100,1.0,true);
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

        SAT_CurveWidget* c1 = new SAT_CurveWidget(SAT_Rect(10,320,45,45),false);
        root->appendChild(c1);
        //c1->setDecay(false);

        SAT_CurveWidget* c2 = new SAT_CurveWidget(SAT_Rect(65,320,45,45),true);
        root->appendChild(c2);
        //c2->setDecay(true);

        SAT_KeyboardWidget* kb1 = new SAT_KeyboardWidget(SAT_Rect(10,375,100,30));
        root->appendChild(kb1);

        SAT_ButtonWidget* b1 = new SAT_ButtonWidget(SAT_Rect(120,210,100,30),false);
        root->appendChild(b1);
        AEditor->connect(b1,getParameter(6));
        b1->setTexts("Off","On");
        b1->setDrawParamText(false);

        SAT_ButtonWidget* b2 = new SAT_ButtonWidget(SAT_Rect(230,210,100,30),true);
        root->appendChild(b2);

        SAT_GridWidget* g1 = new SAT_GridWidget(SAT_Rect( 120,250, 300,100),10,6);
        root->appendChild(g1);
        g1->setSelectCell(true);
        g1->setSelectMultipleCells(true);
        g1->setDrawSelectedCells(true);

        SAT_ButtonGridWidget* bg1 = new SAT_ButtonGridWidget(SAT_Rect(120,360,300,100),10,6,MButtonTexts);
        root->appendChild(bg1);
        //bg1->setButtonTexts(MButtonTexts);
        bg1->setSingle(true,false);

        SAT_SliderWidget* s6 = new SAT_SliderWidget(SAT_Rect(430,210,30,140));
        root->appendChild(s6);
        //AEditor->connect(s6,getParameter(4));
        s6->setDrawDirection(SAT_DIRECTION_UP);
        s6->setDrawText(false);
        s6->setValueTextAlignment(SAT_TEXT_ALIGN_CENTER);

        SAT_ScrollBarWidget* sb1 = new SAT_ScrollBarWidget(SAT_Rect(470,210,20,140));
        root->appendChild(sb1);
        sb1->setDirection(SAT_DIRECTION_VERT);
        sb1->setThumbSize(0.3);

        SAT_GroupBoxWidget* gb1 = new SAT_GroupBoxWidget(SAT_Rect(430,360,200,100),20,true);
        root->appendChild(gb1);

          SAT_KnobWidget* gb1_k1 = new SAT_KnobWidget(SAT_Rect(10,10,50,50));
          gb1->appendChild(gb1_k1);
          AEditor->connect(gb1_k1,getParameter(7));
          gb1_k1->setFillBackground(false);
          gb1_k1->setKnobArcBackgroundColor(SAT_DarkerGrey);

        SAT_TabsWidget* t1 = new SAT_TabsWidget(SAT_Rect(430,470,200,100),20);
        root->appendChild(t1);

          SAT_VisualWidget* p1 = new SAT_VisualWidget(15);
          SAT_VisualWidget* p2 = new SAT_VisualWidget(15);
          SAT_VisualWidget* p3 = new SAT_VisualWidget(15);
          p1->setBackgroundColor(SAT_Yellow);
          p2->setBackgroundColor(SAT_Orange);
          p3->setBackgroundColor(SAT_Green2);
          t1->appendPage( "Page1", p1);
          t1->appendPage( "Page2", p2);
          t1->appendPage( "Page3", p3);

        t1->selectPage(0);

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

