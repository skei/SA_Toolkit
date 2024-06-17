
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

//------------------------------
public:
//------------------------------

  void paramValueEvent(const clap_event_param_value_t* event) final {
    uint32_t id = event->param_id;
    sat_param_t value = event->value;
    SAT_PRINT("%i = %.3f\n",id,value);
  }

  //----------

  void processStereoSample(sat_sample_t* spl0, sat_sample_t* spl1) final {
  }

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

  #include "../../data/img/SA.h"

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
    appendParameter(p1);

    SAT_Parameter* p2 = new SAT_Parameter( "param2", "", 1.0,  0, 10 );
    appendParameter(p2);
    p2->setFlag(CLAP_PARAM_IS_STEPPED);

    SAT_Parameter* p3 = new SAT_Parameter( "param3", "", 2.0,  0, 10 );
    appendParameter(p3);
    p3->setFlag(CLAP_PARAM_IS_STEPPED);

    SAT_Parameter* p4 = new SAT_Parameter( "param4", "", 3.0, -2, 10 );
    appendParameter(p4);
    p4->setFlag(CLAP_PARAM_IS_STEPPED);

    SAT_Parameter* p5 = new SAT_Parameter( "param5", "", 4.0, -2, 10 );
    appendParameter(p5);
    p5->setFlag(CLAP_PARAM_IS_STEPPED);

    SAT_Parameter* p6 = new SAT_Parameter( "p6",     "", 5.0, -2, 10 );
    appendParameter(p6);

    SAT_Parameter* p7 = new SAT_Parameter( "p7",     "", 0,    0, 1  );
    appendParameter(p7);
    p7->setFlag(CLAP_PARAM_IS_ENUM);

    SAT_Parameter* p8 = new SAT_Parameter( "p7",     "", 0,    0, 1  );
    appendParameter(p8);

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
      root->setDrawBorder(true);
      root->setBorderColor(0.4);

      SAT_PluginHeaderWidget* header1 = new SAT_PluginHeaderWidget(40,"plugin_gui2");
      root->appendChild(header1);

      SAT_PluginFooterWidget* footer1 = new SAT_PluginFooterWidget(20,"...");
      root->appendChild(footer1);

      SAT_VisualWidget* middle1 = new SAT_VisualWidget(0);
      root->appendChild(middle1);
      middle1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_BOTTOM_LEFT; // not TOP?
      middle1->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_ALL;
      middle1->Layout.innerBorder  = SAT_Rect(5,5,5,5);
      middle1->Layout.spacing      = SAT_Point(0,0);

        SAT_ScrollBoxWidget* bottom_panel = new SAT_ScrollBoxWidget(150,true,true);
        middle1->appendChild(bottom_panel);
        bottom_panel->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_BOTTOM_LEFT;
        bottom_panel->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
        bottom_panel->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_BOTTOM;
        bottom_panel->Layout.innerBorder = SAT_Rect(0,0,0,0);
        bottom_panel->Layout.spacing = SAT_Point(0,0);
        bottom_panel->getContentWidget()->Layout.innerBorder = SAT_Rect(0,5,5,5);
        bottom_panel->getContentWidget()->Layout.spacing = SAT_Point(5,5);
        bottom_panel->setDrawBorder(false);

        SAT_SizerWidget* bottom_sizer = new SAT_SizerWidget(5,SAT_DIRECTION_DOWN,bottom_panel);
        middle1->appendChild(bottom_sizer);

        SAT_ScrollBoxWidget* left_panel = new SAT_ScrollBoxWidget(200,true,false);
        middle1->appendChild(left_panel);
        left_panel->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
        left_panel->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_VERT;
        left_panel->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_LEFT;
        left_panel->Layout.innerBorder = SAT_Rect(0,0,0,0);
        left_panel->Layout.spacing = SAT_Point(0,0);
        left_panel->getContentWidget()->Layout.innerBorder = SAT_Rect(0,0,5,5);
        left_panel->getContentWidget()->Layout.spacing = SAT_Point(0,5);
        left_panel->setDrawBorder(false);

        SAT_SizerWidget* left_sizer = new SAT_SizerWidget(5,SAT_DIRECTION_LEFT,left_panel);
        middle1->appendChild(left_sizer);

        SAT_VisualWidget* center_panel = new SAT_VisualWidget(0);
        middle1->appendChild(center_panel);
        center_panel->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
        center_panel->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_ALL;
        center_panel->Layout.innerBorder = SAT_Rect(5,0,0,5);
        center_panel->Layout.spacing = SAT_Point(5,0);
        center_panel->setDrawBorder(false);

      //--------------------
      // menu
      //--------------------

      SAT_MenuWidget* menu1 = new SAT_MenuWidget(SAT_Rect(100,20*5));
        menu1->appendItem("Item1");
        menu1->appendItem("Item2");
        menu1->appendItem("Item3");
        menu1->appendItem("Item4");
        menu1->appendItem("Item5");

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
      //AEditor->connect(value1,getParameter(0));

      SAT_ButtonWidget* button1 = new SAT_ButtonWidget(20);
      left_panel->appendChild(button1);
      button1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      button1->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
      button1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;
      //button1->setDrawParamText(false);
      //AEditor->connect(button1,getParameter(1));

      SAT_ButtonWidget* button2 = new SAT_ButtonWidget(20,true);
      left_panel->appendChild(button2);
      button2->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      button2->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
      button2->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;
      //button2->setDrawParamText(false);
      //AEditor->connect(button2,getParameter(2));

      SAT_SelectorWidget* selector1 = new SAT_SelectorWidget(20,"Select..",menu1);
      left_panel->appendChild(selector1);
      menu1->setListener(selector1);
      selector1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      selector1->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
      selector1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;
      selector1->setDrawParamText(false);
      //AEditor->connect(selector1,getParameter(3));

      SAT_TextEditWidget* textedit1 = new SAT_TextEditWidget(20,"Edit..");
      left_panel->appendChild(textedit1);
      textedit1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      textedit1->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
      textedit1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;

      SAT_DragValueWidget* dragvalue1 = new SAT_DragValueWidget(20);
      left_panel->appendChild(dragvalue1);
      dragvalue1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      dragvalue1->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
      dragvalue1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;
      dragvalue1->setText("DragValue");
      //AEditor->connect(dragvalue1,getParameter(4));

      SAT_SliderWidget* slider1 = new SAT_SliderWidget(20);
      left_panel->appendChild(slider1);
      slider1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      slider1->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
      slider1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;
      slider1->setText("Slider");
      //AEditor->connect(slider1,getParameter(5));

      SAT_ScrollBarWidget* scrollbar1 = new SAT_ScrollBarWidget(20);
      left_panel->appendChild(scrollbar1);
      scrollbar1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      scrollbar1->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
      scrollbar1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;
      //AEditor->connect(scrollbar1,getParameter(6));

      // SAT_RangeBarWidget* rangebar1 = new SAT_RangeBarWidget(20);
      // left_panel->appendChild(rangebar1);
      // rangebar1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      // rangebar1->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
      // rangebar1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;

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
        //p1->setBackgroundColor(SAT_Yellow);
        p1->appendChild( new SAT_KnobWidget(SAT_Rect( 5,5,40,40)));
        p1->appendChild( new SAT_KnobWidget(SAT_Rect(50,5,40,40)));
        p1->appendChild( new SAT_KnobWidget(SAT_Rect(95,5,40,40)));
        //p2->setBackgroundColor(SAT_Orange);
        p2->appendChild( new SAT_SliderWidget(SAT_Rect(5, 5,150,20)));
        p2->appendChild( new SAT_SliderWidget(SAT_Rect(5,30,150,20)));
        //p3->setBackgroundColor(SAT_Green2);
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

      SAT_KeyboardWidget* keyboard1 = new SAT_KeyboardWidget(60);
      left_panel->appendChild(keyboard1);
      keyboard1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      keyboard1->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
      keyboard1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;

      SAT_SliderBankWidget* sliderbank1 = new SAT_SliderBankWidget(60,16);
      left_panel->appendChild(sliderbank1);
      sliderbank1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      sliderbank1->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
      sliderbank1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;

      SAT_ValueGraphWidget* valuegraph1 = new SAT_ValueGraphWidget(60,16);
      left_panel->appendChild(valuegraph1);
      valuegraph1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      valuegraph1->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
      valuegraph1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;

      SAT_VisualWidget* symbol_container = new SAT_VisualWidget(20);
      left_panel->appendChild(symbol_container);
      symbol_container->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      symbol_container->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
      symbol_container->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;
      symbol_container->setDrawBorder(false);
      
        for (uint32_t i=0; i<17; i++) {
          SAT_SymbolWidget* symbol = new SAT_SymbolWidget(SAT_Rect(i*20,0,18,18),i);
          symbol->setColor(SAT_DarkerGrey);
          symbol->setColor(SAT_DarkerGrey);
          symbol->setPenWidth(2);
          symbol_container->appendChild(symbol);
        }

      //--------------------
      // bottom panel
      //--------------------

      SAT_KnobWidget* knob1 = new SAT_KnobWidget(100);
      bottom_panel->appendChild(knob1);
      knob1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      knob1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_LEFT;
      //AEditor->connect(knob1,getParameter(7));

      SAT_CurveWidget* curve1 = new SAT_CurveWidget(30,false);
      bottom_panel->appendChild(curve1);
      curve1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      curve1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_LEFT;
      curve1->setDrawParamText(false);
      //AEditor->connect(curve1,getParameter(1));


      SAT_CurveWidget* curve2 = new SAT_CurveWidget(30,true);
      bottom_panel->appendChild(curve2);
      curve2->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      curve2->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_LEFT;
      curve2->setDrawParamText(false);
      //AEditor->connect(curve2,getParameter(2));

      SAT_SliderWidget* slider2 = new SAT_SliderWidget(SAT_Rect(25,100));
      bottom_panel->appendChild(slider2);
      slider2->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      slider2->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_LEFT;
      slider2->setDrawDirection(SAT_DIRECTION_UP);
      slider2->setDrawText(false);
      slider2->setValueTextAlignment(SAT_TEXT_ALIGN_CENTER);
      slider2->setDrawParamText(false);
      //AEditor->connect(slider2,getParameter(6));

      SAT_ScrollBarWidget* scrollbar2 = new SAT_ScrollBarWidget(SAT_Rect(25,100));
      bottom_panel->appendChild(scrollbar2);
      scrollbar2->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      scrollbar2->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_LEFT;
      scrollbar2->setDirection(SAT_DIRECTION_VERT);

      SAT_ImageWidget* image1 = new SAT_ImageWidget(100,(void*)SA_png,SA_png_size);
      bottom_panel->appendChild(image1);
      image1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      image1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_LEFT;
      image1->setDrawBorder(false);

      SAT_LogoWidget* logo1 = new SAT_LogoWidget(SAT_Rect(5,0,100,100));
      bottom_panel->appendChild(logo1);
      logo1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      logo1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_LEFT;
      logo1->setLogoColor(0.25);
      logo1->setLogoWidth(3);
      logo1->setFillBackground(true);
      logo1->setBackgroundColor(0.85);
      logo1->setLogoOffset(SAT_Rect(12,12,12,12));

      SAT_TextBoxWidget* textbox1 = new SAT_TextBoxWidget(SAT_Rect(150,100));
      bottom_panel->appendChild(textbox1);
      textbox1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      textbox1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_LEFT;
        textbox1->appendLine("Hello world!");
        textbox1->appendLine("another line");
        textbox1->appendLine("let's try a much, much longer line, and see if the scrollbars are updated");

      SAT_WaveformWidget* waveform1 = new SAT_WaveformWidget(SAT_Rect(150,100));
      bottom_panel->appendChild(waveform1);
      waveform1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      waveform1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_LEFT;
      //waveform1->setAudioBuffer(size,buffer,stereo);

      SAT_CircularWaveformWidget* waveform2 = new SAT_CircularWaveformWidget(100,360);
      bottom_panel->appendChild(waveform2);
      waveform2->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      waveform2->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_LEFT;
      //waveform2->setAudioBuffer(size,buffer,stereo);


      //--------------------
      // center panel
      //--------------------

      SAT_TabsWidget* center_tabs1 = new SAT_TabsWidget(0,15);
      center_panel->appendChild(center_tabs1);
      center_tabs1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      center_tabs1->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_ALL;
      center_tabs1->getPages()->setDrawBorder(false);

        SAT_VisualWidget* modular_tab1 = new SAT_VisualWidget(15);
        center_tabs1->appendPage( "Modular", modular_tab1);
        //modular_tab1->Layout.innerBorder = SAT_Rect(5,5,5,5);
        modular_tab1->Layout.spacing = SAT_Point(0,5);
        modular_tab1->setDrawBorder(false);
        {
          SAT_TimelineWidget* timeline1 = new SAT_TimelineWidget(100);
          modular_tab1->appendChild(timeline1);
          timeline1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_BOTTOM_LEFT;
          timeline1->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
          timeline1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_BOTTOM;

            SAT_TimelineTrack* track1 = new SAT_TimelineTrack("Track1");
            timeline1->addTrack(track1);
            SAT_TimelineSegment* segment1 = new SAT_TimelineSegment("1",1,10);
            SAT_TimelineSegment* segment2 = new SAT_TimelineSegment("2",3,5);
            SAT_TimelineSegment* segment3 = new SAT_TimelineSegment("3",6,10);
            SAT_TimelineSegment* segment4 = new SAT_TimelineSegment("4",2,7);
            track1->addSegment(segment1);

            SAT_TimelineTrack* track2 = new SAT_TimelineTrack("T2");
            timeline1->addTrack(track2);
            track2->addSegment(segment2);
            track2->addSegment(segment3);

            SAT_TimelineTrack* track3 = new SAT_TimelineTrack("Automation");
            timeline1->addTrack(track3);
            track3->addSegment(segment4);

          SAT_GraphWidget* graph1 = new SAT_GraphWidget(0);
          modular_tab1->appendChild(graph1);
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
        } // modular_tab1

        //center_tabs1->appendPage( "t2", new SAT_VisualWidget(0));

        SAT_VisualWidget* t2_tab1 = new SAT_VisualWidget(15);
        center_tabs1->appendPage( "t2", t2_tab1);
        //t2_tab1->Layout.innerBorder = SAT_Rect(5,5,5,5);
        t2_tab1->Layout.spacing = SAT_Point(0,5);
        t2_tab1->setDrawBorder(false);
        {

          SAT_VisualWidget* ds1 = new SAT_VisualWidget(SAT_Rect(20,20,100,100));
          t2_tab1->appendChild(ds1);
          ds1->setDrawDropShadow(true);
          //ds1->setDropShadowCorner(5);
          ds1->setFillBackground(false);
          ds1->setDrawBorder(false);
          SAT_VisualWidget* ds2 = new SAT_VisualWidget(SAT_Rect(25,25,90,90));
          t2_tab1->appendChild(ds2);

          SAT_VisualWidget* ds3 = new SAT_VisualWidget(SAT_Rect(130,20,100,100));
          t2_tab1->appendChild(ds3);
          ds3->setDrawDropShadow(true);
          ds3->setDropShadowInner(true);
          ds3->setDropShadowCorner(5);
          ds3->setFillBackground(false);
          ds3->setDrawBorder(true);
          // SAT_VisualWidget* ds4 = new SAT_VisualWidget(SAT_Rect(135,25,90,90));
          // t2_tab1->appendChild(ds4);
          // ds4->setFillBackground(false);
          // ds4->setDrawBorder(true);

          SAT_VisualWidget* ds5 = new SAT_VisualWidget(SAT_Rect(240,20,100,100));
          t2_tab1->appendChild(ds5);
          ds5->setFillBackground(true);
          ds5->setFillGradient(true);
          //ds5->setDrawBorder(true);

          SAT_VisualWidget* ds6 = new SAT_VisualWidget(SAT_Rect(350,20,100,100));
          t2_tab1->appendChild(ds6);
          ds6->setFillBackground(true);
          ds6->setBackgroundColor(SAT_Yellow);
          ds6->setDrawRoundedCorners(true);
          ds6->setRoundedCornerSize(20);
          ds6->setRoundedCorners(SAT_CORNER_UPPER_LEFT | SAT_CORNER_LOWER_RIGHT);

        } // t2_tab


        center_tabs1->appendPage( "t3", new SAT_VisualWidget(0));

      center_tabs1->selectPage(0);

      //--------------------
      // menu
      //--------------------

      root->appendChild(menu1);

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

