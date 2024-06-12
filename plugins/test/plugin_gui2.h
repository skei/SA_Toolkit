
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

    // SAT_Parameter* p1 = new SAT_Parameter( "param1", "", 0.0, -2, 10 );
    // appendParameter(p1);

    // SAT_Parameter* p2 = new SAT_Parameter( "param2", "", 1.0,  0, 10 );
    // appendParameter(p2);
    // p2->setFlag(CLAP_PARAM_IS_STEPPED);

    // SAT_Parameter* p3 = new SAT_Parameter( "param3", "", 2.0,  0, 10 );
    // appendParameter(p3);
    // p3->setFlag(CLAP_PARAM_IS_STEPPED);

    // SAT_Parameter* p4 = new SAT_Parameter( "param4", "", 3.0, -2, 10 );
    // appendParameter(p4);
    // p4->setFlag(CLAP_PARAM_IS_STEPPED);

    // SAT_Parameter* p5 = new SAT_Parameter( "param5", "", 4.0, -2, 10 );
    // appendParameter(p5);
    // p5->setFlag(CLAP_PARAM_IS_STEPPED);

    // SAT_Parameter* p6 = new SAT_Parameter( "p6",     "", 5.0, -2, 10 );
    // appendParameter(p6);

    // SAT_Parameter* p7 = new SAT_Parameter( "p7",     "", 0,    0, 1  );
    // appendParameter(p7);
    // p7->setFlag(CLAP_PARAM_IS_ENUM);

    // SAT_Parameter* p8 = new SAT_Parameter( "p7",     "", 0,    0, 1  );
    // appendParameter(p8);

    setProcessor( new myProcessor(this) );
    setInitialEditorSize(800,600,1.0,true);
    return SAT_Plugin::init();
  }

//------------------------------
public:
//------------------------------

  #ifndef SAT_NO_GUI

    bool setupEditor(SAT_Editor* AEditor) final {
      //SAT_TRACE;

      //--------------------
      // main layout
      //--------------------

      SAT_Window* window = AEditor->getWindow();
      SAT_RootWidget* root = new SAT_RootWidget( window, SAT_Rect() );
      window->setRootWidget(root);
      root->Layout.innerBorder = SAT_Rect(10,10,10,10);

      SAT_VisualWidget* left_panel = new SAT_VisualWidget(200);
      left_panel->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      left_panel->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_VERT;
      left_panel->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_LEFT;
      left_panel->setDrawBorder(false);
      left_panel->Layout.innerBorder = SAT_Rect(0,0,5,5);
      left_panel->Layout.spacing = SAT_Point(5,5);

      SAT_VisualWidget* bottom_panel = new SAT_VisualWidget(100);
      bottom_panel->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_BOTTOM_LEFT;
      bottom_panel->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
      bottom_panel->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_BOTTOM;
      bottom_panel->setDrawBorder(false);

      SAT_VisualWidget* center_panel = new SAT_VisualWidget(0);
      center_panel->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      center_panel->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_ALL;
      center_panel->setDrawBorder(false);

      SAT_SizerWidget* left_sizer = new SAT_SizerWidget(5,SAT_DIRECTION_LEFT,left_panel);
      SAT_SizerWidget* bottom_sizer = new SAT_SizerWidget(5,SAT_DIRECTION_DOWN,bottom_panel);

      root->appendChild(left_panel);
      root->appendChild(left_sizer);
      root->appendChild(bottom_panel);
      root->appendChild(bottom_sizer);
      root->appendChild(center_panel);

      //--------------------
      // left panel
      //--------------------

      SAT_VisualWidget* widget1 = new SAT_VisualWidget(20);
      left_panel->appendChild(widget1);
      widget1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      widget1->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
      widget1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;
      widget1->setFillBackground(true);
      widget1->setBackgroundColor(SAT_Red);
      widget1->setDrawBorder(false);

      SAT_ValueWidget* value1 = new SAT_ValueWidget(20);
      left_panel->appendChild(value1);
      value1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      value1->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
      value1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;
      value1->setText("Value");

      SAT_DragValueWidget* dragvalue1 = new SAT_DragValueWidget(20);
      left_panel->appendChild(dragvalue1);
      dragvalue1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      dragvalue1->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
      dragvalue1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;
      dragvalue1->setText("DragValue");

      SAT_SliderWidget* slider1 = new SAT_SliderWidget(20);
      left_panel->appendChild(slider1);
      slider1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      slider1->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
      slider1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;
      slider1->setText("Slider");

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

