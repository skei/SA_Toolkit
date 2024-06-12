
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

      SAT_VisualWidget* bottom_panel = new SAT_VisualWidget(60);
      bottom_panel->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_BOTTOM_LEFT;
      bottom_panel->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
      bottom_panel->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_BOTTOM;
      bottom_panel->setDrawBorder(false);
      bottom_panel->Layout.innerBorder = SAT_Rect(5,5,5,0);
      bottom_panel->Layout.spacing = SAT_Point(5,5);

      //SAT_VisualWidget* left_panel = new SAT_VisualWidget(200);
      SAT_ScrollBoxWidget* left_panel = new SAT_ScrollBoxWidget(200);
      left_panel->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      left_panel->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_VERT;
      left_panel->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_LEFT;
      left_panel->setDrawBorder(false);
      left_panel->getContentWidget()->Layout.innerBorder = SAT_Rect(0,0,5,5);
      left_panel->getContentWidget()->Layout.spacing = SAT_Point(5,5);

      SAT_VisualWidget* center_panel = new SAT_VisualWidget(0);
      center_panel->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      center_panel->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_ALL;
      center_panel->setDrawBorder(false);
      center_panel->Layout.innerBorder = SAT_Rect(5,0,0,5);
      center_panel->Layout.spacing = SAT_Point(5,5);

      SAT_SizerWidget* left_sizer = new SAT_SizerWidget(5,SAT_DIRECTION_LEFT,left_panel);
      SAT_SizerWidget* bottom_sizer = new SAT_SizerWidget(5,SAT_DIRECTION_DOWN,bottom_panel);

      root->appendChild(bottom_panel);
      root->appendChild(bottom_sizer);
      root->appendChild(left_panel);
      root->appendChild(left_sizer);
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

      SAT_TextWidget* text1 = new SAT_TextWidget(20,"Text");
      left_panel->appendChild(text1);
      text1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      text1->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
      text1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;
      text1->setFillBackground(true);
      text1->setBackgroundColor(0.45);
      text1->setDrawBorder(false);

      SAT_ValueWidget* value1 = new SAT_ValueWidget(20);
      left_panel->appendChild(value1);
      value1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      value1->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
      value1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;
      value1->setText("Value");
      value1->setDrawBorder(false);
      value1->setBackgroundColor(0.45);
      value1->setDrawBorder(false);

      SAT_ButtonWidget* button1 = new SAT_ButtonWidget(20);
      left_panel->appendChild(button1);
      button1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      button1->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
      button1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;

      SAT_ButtonWidget* button2 = new SAT_ButtonWidget(20,true);
      left_panel->appendChild(button2);
      button2->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      button2->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
      button2->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;

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

      SAT_ScrollBarWidget* scrollbar1 = new SAT_ScrollBarWidget(20);
      left_panel->appendChild(scrollbar1);
      scrollbar1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      scrollbar1->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
      scrollbar1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;

      SAT_GroupBoxWidget* groupbox1 = new SAT_GroupBoxWidget(80,20,true);
      left_panel->appendChild(groupbox1);
      groupbox1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      groupbox1->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
      groupbox1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;
        groupbox1->appendChild( new SAT_KnobWidget(SAT_Rect( 5,5,40,40)));
        groupbox1->appendChild( new SAT_KnobWidget(SAT_Rect(50,5,40,40)));
        groupbox1->appendChild( new SAT_KnobWidget(SAT_Rect(95,5,40,40)));

      SAT_TabsWidget* tabs1 = new SAT_TabsWidget(80,20);
      left_panel->appendChild(tabs1);
      tabs1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      tabs1->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
      tabs1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;
        SAT_VisualWidget* p1 = new SAT_VisualWidget(15);
        SAT_VisualWidget* p2 = new SAT_VisualWidget(15);
        SAT_VisualWidget* p3 = new SAT_VisualWidget(15);
        p1->setBackgroundColor(SAT_Yellow);
        p1->appendChild( new SAT_KnobWidget(SAT_Rect( 5,5,40,40)));
        p1->appendChild( new SAT_KnobWidget(SAT_Rect(50,5,40,40)));
        p1->appendChild( new SAT_KnobWidget(SAT_Rect(95,5,40,40)));
        p2->setBackgroundColor(SAT_Orange);
        p2->appendChild( new SAT_SliderWidget(SAT_Rect(5, 5,150,20)));
        p2->appendChild( new SAT_SliderWidget(SAT_Rect(5,30,150,20)));
        p3->setBackgroundColor(SAT_Green2);
        p3->appendChild( new SAT_ButtonWidget(SAT_Rect( 5, 5,40,20)));
        p3->appendChild( new SAT_ButtonWidget(SAT_Rect(50, 5,40,20)));
        p3->appendChild( new SAT_ButtonWidget(SAT_Rect( 5,30,40,20)));
        p3->appendChild( new SAT_ButtonWidget(SAT_Rect(50,30,40,20)));
        tabs1->appendPage( "Page1", p1);
        tabs1->appendPage( "Page2", p2);
        tabs1->appendPage( "Page3", p3);
      tabs1->selectPage(0);

      SAT_GridWidget* grid1 = new SAT_GridWidget(60,4,4);
      left_panel->appendChild(grid1);
      grid1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      grid1->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
      grid1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;
      grid1->setCanSelectCell(true);
      grid1->setCanSelectMultipleCells(true);
      grid1->setDrawSelectedCells(true);

      SAT_ButtonGridWidget* buttongrid1 = new SAT_ButtonGridWidget(60,4,4,MButtonTexts);
      left_panel->appendChild(buttongrid1);
      buttongrid1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      buttongrid1->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
      buttongrid1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;
      buttongrid1->setSingle(false,true);

      SAT_KeyboardWidget* keyboard = new SAT_KeyboardWidget(60);
      left_panel->appendChild(keyboard);
      keyboard->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      keyboard->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
      keyboard->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;

      //--------------------
      // bottom panel
      //--------------------

      SAT_KnobWidget* knob1 = new SAT_KnobWidget(50);
      bottom_panel->appendChild(knob1);
      knob1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      knob1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_LEFT;

      SAT_CurveWidget* curve1 = new SAT_CurveWidget(50,false);
      bottom_panel->appendChild(curve1);
      curve1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      curve1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_LEFT;

      SAT_CurveWidget* curve2 = new SAT_CurveWidget(50,true);
      bottom_panel->appendChild(curve2);
      curve2->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      curve2->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_LEFT;

      //--------------------
      // center panel
      //--------------------

      // SAT_ScrollBoxWidget* scrollbox1 = new SAT_ScrollBoxWidget(100);
      // center_panel->appendChild(scrollbox1);
      // scrollbox1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_RIGHT;
      // scrollbox1->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_VERT;
      // scrollbox1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_RIGHT;

      //   for (uint32_t i=0; i< 30; i++) {
      //     scrollbox1->appendChild( new SAT_SliderWidget(SAT_Rect(5,5+i*25,80,20)));
      //   }

      SAT_GraphWidget* graph1 = new SAT_GraphWidget(0);
      center_panel->appendChild(graph1);
      graph1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      graph1->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_ALL;
      //graph1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;

        graph1->setFillBackground(true);
        //graph->setAlignment(SAT_EDGE_BOTTOM);
        //graph->setStretching(SAT_EDGE_RIGHT | SAT_EDGE_BOTTOM);
        for (uint32_t i=0; i<10; i++) {
          SAT_GraphModule* module = new SAT_GraphModule();
          module->numInputs = 2;
          module->inputs[0] = SAT_PIN_SIGNAL;
          module->outputs[0] = SAT_PIN_SIGNAL;
          module->numOutputs = 2;
          graph1->addModule(module,i*15,i*15,"module");
        }


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

