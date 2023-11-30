
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/sat_plugin.h"
#include "plugin/sat_editor.h"
#include "gui/sat_widgets.h"

#include "../data/img/SA.h"

//----------

#define EDITOR_WIDTH  1000
#define EDITOR_HEIGHT 600
#define EDITOR_SCALE  1.333

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t sat_demo_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = SAT_VENDOR "/sat_demo",
  .name         = "sat_demo",
  .vendor       = SAT_VENDOR,
  .url          = SAT_URL,
  .manual_url   = SAT_URL,
  .support_url  = SAT_URL,
  .version      = SAT_VERSION,
  .description  = "...",
  .features     = (const char*[]){ CLAP_PLUGIN_FEATURE_INSTRUMENT, nullptr }
};

const char* tabs_txt[5] = {
  "widgets",
  "2",
  "3",
  "4",
  "5"
};

const char* buttonrow_txt[5] = {
  "One",
  "Two",
  "3",
  "IV",
  "V"
};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class sat_demo_plugin
: public SAT_Plugin {

//------------------------------
public:
//------------------------------

    SAT_MenuWidget* MSelectorMenu = nullptr;
    float           MBuffer[2048];

//------------------------------
public:
//------------------------------

  sat_demo_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : SAT_Plugin(ADescriptor,AHost) {
    //SAT_PRINT;

    //int* ptr = nullptr;
    //int val = *ptr;
    //SAT_Print("%p = %i\n",ptr,val);

  }

  //----------

  virtual ~sat_demo_plugin() {
    //SAT_PRINT;
  }

//------------------------------
public:
//------------------------------

  bool init() final {
    //registerDefaultSynthExtensions();
    registerAllExtension();
    registerDraftExtension();
    appendClapNoteInputPort("input");
    appendStereoAudioInputPort("input");
    appendStereoAudioOutputPort("output");

    appendParameter( new SAT_Parameter("param1",0.1) );
    appendParameter( new SAT_Parameter("param2",0.2) );
    appendParameter( new SAT_Parameter("param3",0.3) );
    appendParameter( new SAT_Parameter("param4",0.4) );
    appendParameter( new SAT_Parameter("param5",0.5) );
    appendParameter( new SAT_Parameter("param6",0.6) );
    appendParameter( new SAT_Parameter("param7",0.7) );
    appendParameter( new SAT_Parameter("param8",0.8) );
    appendParameter( new SAT_Parameter("param9",0.9) );

    setInitialEditorSize(EDITOR_WIDTH,EDITOR_HEIGHT,EDITOR_SCALE);
    for (uint32_t i=0; i<2048; i++) MBuffer[i] = SAT_RandomRange( -0.8, 0.8 );
    return SAT_Plugin::init();
  }

  //----------

  void setupEditorWindow(SAT_Editor* AEditor, SAT_Window* AWindow) final {
    SAT_RootWidget* root = new SAT_RootWidget(0,AWindow);
    AWindow->setRootWidget(root);
    //root->setLayoutInnerBorder(SAT_Rect(10,10,10,10));
    init_editor(AEditor,AWindow,root);
  }
  
//------------------------------
private: // editor
//------------------------------

  void init_editor(SAT_Editor* AEditor, SAT_Window* AWindow, SAT_RootWidget* root) {

    MSelectorMenu = new SAT_MenuWidget(SAT_Rect(0,0,200,15*5));
      MSelectorMenu->appendChildWidget( new SAT_MenuItemWidget(15,"Item 1") );
      MSelectorMenu->appendChildWidget( new SAT_MenuItemWidget(15,"Item 2") );
      MSelectorMenu->appendChildWidget( new SAT_MenuItemWidget(15,"Item 3") );
      MSelectorMenu->appendChildWidget( new SAT_MenuItemWidget(15,"Item 4") );
      MSelectorMenu->appendChildWidget( new SAT_MenuItemWidget(15,"Item 5") );

    SAT_PluginHeaderWidget* header = new SAT_PluginHeaderWidget(30,"demo");
    root->appendChildWidget(header);
    
    SAT_PluginFooterWidget* footer = new SAT_PluginFooterWidget(20,"...");
    root->appendChildWidget(footer);

    SAT_Widget* middle = new SAT_Widget(0);
    root->appendChildWidget(middle);
    middle->addLayoutFlag(SAT_WIDGET_LAYOUT_STRETCH_ALL);
    middle->setLayoutInnerBorder(SAT_Rect(10,10,10,10));
    
    SAT_TabsWidget* tabs = new SAT_TabsWidget(SAT_Rect(10,10,100,100),32,20);
    middle->appendChildWidget(tabs);
    tabs->addLayoutFlag(SAT_WIDGET_LAYOUT_STRETCH_ALL);
    tabs->appendPage("widgets",   create_widgets_page(AEditor,AWindow) );
    tabs->appendPage("parameters",create_params_page(AEditor,AWindow)  );
    tabs->appendPage("host",      create_host_page(AEditor,AWindow)  );
    tabs->selectPage(0);

    root->appendChildWidget(MSelectorMenu);

  }

//------------------------------
private: // widget page
//------------------------------

  SAT_PanelWidget* create_widgets_page(SAT_Editor* AEditor, SAT_Window* AWindow) {
    SAT_PanelWidget* page = new SAT_PanelWidget(0);
    //page->setBackgroundColor(SAT_Grey);
    page->setFillBackground(false);

      SAT_ScrollBoxWidget* scrollbox = new SAT_ScrollBoxWidget(SAT_Rect(100),true,true);
      page->appendChildWidget(scrollbox);
      scrollbox->addLayoutFlag(SAT_WIDGET_LAYOUT_STRETCH_ALL);
      scrollbox->setFillBackground(true);
      //scrollbox->setBackgroundColor(SAT_Grey);


        //--------------------
        // column 1
        //--------------------


        SAT_PanelWidget* panel;

        // panel : text

        panel = new SAT_PanelWidget(SAT_Rect(10,10,200,20));
        scrollbox->appendChildWidget(panel);
          panel->setDrawText("Text");
          panel->setText("Text");

        // panel : text + value

        panel = new SAT_PanelWidget(SAT_Rect(10,40,200,20));
        scrollbox->appendChildWidget(panel);
          panel->setDrawText(true);
          panel->setText("Value");
          panel->setTextAlignment(SAT_TEXT_ALIGN_LEFT);
          panel->setTextOffset(SAT_Rect(5,0,0,0));
          panel->setDrawValue(true);
          panel->setValue(0.5);
          panel->setValueAlignment(SAT_TEXT_ALIGN_RIGHT);
          panel->setValueOffset(SAT_Rect(0,0,5,0));

        // panel : color

        panel = new SAT_PanelWidget(SAT_Rect(10,70,200,20));
        scrollbox->appendChildWidget(panel);
          panel->setFillBackground(true);
          panel->setBackgroundColor(SAT_Cyan);
          panel->setDrawBorder(true);
          panel->setBorderColor(SAT_Black);
          panel->setBorderWidth(1);

        // panel : rounded corners

        panel = new SAT_PanelWidget(SAT_Rect(10,100,200,20));
        scrollbox->appendChildWidget(panel);
          panel->setDrawBorder(true);
          panel->setBorderColor(SAT_LightGrey);
          panel->setBorderWidth(2);
          panel->setRoundedCorners(true);
          //panel->setCornerSize(8);
          panel->setCornerSizes(8,8,0,8);
          //panel->setBorderCorners(/*SAT_CORNER_UPPER_LEFT +*/ SAT_CORNER_UPPER_RIGHT + SAT_CORNER_LOWER_RIGHT + SAT_CORNER_LOWER_LEFT);

        // panel : gradient

        panel = new SAT_PanelWidget(SAT_Rect(10,130,200,20));
        scrollbox->appendChildWidget(panel);
          panel->setFillLinearGradient(true);
          panel->setGradientColors(SAT_Grey,0.45,0.55);
          panel->setRoundedCorners(true);
          panel->setCornerSize(8);

        // panel : drop shadow

        panel = new SAT_PanelWidget(SAT_Rect(10,160,200,30));
        scrollbox->appendChildWidget(panel);
          panel->setFillBackground(true);
          //panel->setBackgroundColor(SAT_Grey);
          panel->setFillLinearGradient(true);
          panel->setGradientColors(SAT_Grey,0.2,0.2);
          panel->setDrawBorder(true);
          panel->setBorderColor(SAT_Black);
          panel->setRoundedCorners(true);
          panel->setCornerSize(6);
          panel->setBorderWidth(0.5);
          panel->setDrawDropShadow(true);
          panel->setDropShadowFeather(5);
          panel->setDropShadowOffset(3,2);

        // panel : inner drop shadow

        panel = new SAT_PanelWidget(SAT_Rect(10,200,200,30));
        scrollbox->appendChildWidget(panel);
          panel->setFillBackground(false);
          panel->setBackgroundColor(SAT_Grey);
          panel->setDrawBorder(true);
          panel->setBorderColor(SAT_Black);
          panel->setBorderWidth(0.5);
          panel->setRoundedCorners(true);
          panel->setCornerSize(6);
          panel->setDrawDropShadow(true);
          panel->setDropShadowInner(true);
          panel->setDropShadowFeather(5);
          panel->setDropShadowOffset(3,2);
          panel->setDropShadowColors( SAT_Color(0.0, 0.0, 0.0, 0.5 ), SAT_Color(0.33, 0.33, 0.33, 0.5 ));

        // panel : font

        panel = new SAT_PanelWidget(SAT_Rect(10,240,200,30));
        scrollbox->appendChildWidget(panel);
          SAT_Painter* painter = AWindow->getPainter();
          int32_t font = painter->getHeaderFont();
          panel->setDrawBorder(false);
          panel->setFillBackground(true);
          panel->setBackgroundColor(SAT_DarkerGrey);
          panel->setText("header");
          panel->setTextFont(font);
          panel->setTextColor(SAT_LighterGrey);
          panel->setTextSize(24);
          panel->setTextOffset(SAT_Rect(0,10,0,0));



        // panel: text + text shadow

        panel = new SAT_PanelWidget(SAT_Rect(10,280,200,30));
        scrollbox->appendChildWidget(panel);
          panel->setDrawBorder(false);
          panel->setDrawText(true);
          panel->setText("Text Shadow");
          panel->setTextColor(SAT_LightGrey);
          panel->setTextSize(20);
          panel->setDrawTextDropShadow(true);
          panel->setTextDropShadowSize(3);
          panel->setTextDropShadowOffset(SAT_Point(2,2));
          panel->setTextDropShadowColor(SAT_Black);

        // logo

        SAT_LogoWidget* logo = new SAT_LogoWidget(SAT_Rect(10,320,60,60));
        scrollbox->appendChildWidget(logo);

        // image

        //SAT_ImageWidget* image = new SAT_ImageWidget(SAT_Rect(80,320,60,60),"../data/img/sa.png");
        SAT_ImageWidget* image = new SAT_ImageWidget(SAT_Rect(80,320,60,60),(void*)SA,SA_size);
        scrollbox->appendChildWidget(image);
        image->setScale(0.6);

        // circular waveform

        SAT_CircularWaveformWidget* circular_waveform = new SAT_CircularWaveformWidget(SAT_Rect(150,320,60,60), 128 );
        scrollbox->appendChildWidget(circular_waveform);
          circular_waveform->setBackgroundColor(0.375);
          circular_waveform->setWaveformBackgroundColor(0.35);
          circular_waveform->setZeroLineColor(0.2);
          circular_waveform->setAudioBuffer(2048,MBuffer,false);
          circular_waveform->setWaveformWidth(1);

        // symbols

        for (uint32_t i=0; i<8; i++) scrollbox->appendChildWidget( new SAT_SymbolWidget(SAT_Rect(10 + (i*(16+7)),390,16,16), i    ));
        for (uint32_t i=0; i<8; i++) scrollbox->appendChildWidget( new SAT_SymbolWidget(SAT_Rect(10 + (i*(16+7)),410,16,16), i + 9));

        // waveform

        SAT_WaveformWidget* waveform = new SAT_WaveformWidget(SAT_Rect(10,440,200,50));
        scrollbox->appendChildWidget(waveform);
          waveform->setNumGrid(4);
          waveform->setNumSubGrid(2);
          waveform->setBuffer(MBuffer,2048);

        //--------------------
        // column 2
        //--------------------


        // drag value

        SAT_DragValueWidget* dragvalue1 = new SAT_DragValueWidget(SAT_Rect(220,10,200,20),"DragValue",0.1);
        scrollbox->appendChildWidget(dragvalue1);
          dragvalue1->setTextOffset(SAT_Rect(5,0,0,0));
          dragvalue1->setValueOffset(SAT_Rect(0,0,5,0));

        // drag value : snap

        SAT_DragValueWidget* dragvalue2 = new SAT_DragValueWidget(SAT_Rect(220,40,200,20),"Snap",0.1);
        scrollbox->appendChildWidget(dragvalue2);
          dragvalue2->setTextOffset(SAT_Rect(5,0,0,0));
          dragvalue2->setValueOffset(SAT_Rect(0,0,5,0));
          dragvalue2->setSnap(true);
          dragvalue2->setSnapPos(0.5);

        // drag value : quantize

        SAT_DragValueWidget* dragvalue3 = new SAT_DragValueWidget(SAT_Rect(220,70,200,20),"Quantize",0.0);
        scrollbox->appendChildWidget(dragvalue3);
          dragvalue3->setTextOffset(SAT_Rect(5,0,0,0));
          dragvalue3->setValueOffset(SAT_Rect(0,0,5,0));
          dragvalue3->setQuantize(true);
          dragvalue3->setQuantizeSteps(6);

        // slider

        SAT_SliderWidget* slider1 = new SAT_SliderWidget(SAT_Rect(220,110,200,20),"Slider",0.2);
        scrollbox->appendChildWidget(slider1);
          slider1->setTextOffset(SAT_Rect(5,0,0,0));
          slider1->setValueOffset(SAT_Rect(0,0,5,0));

        // slider : bipolar + snap

        SAT_SliderWidget* slider2 = new SAT_SliderWidget(SAT_Rect(220,140,200,20),"Bipolar + Snap",0.3);
        scrollbox->appendChildWidget(slider2);
          slider2->setTextOffset(SAT_Rect(5,0,0,0));
          slider2->setValueOffset(SAT_Rect(0,0,5,0));
          slider2->setBipolar(true);
          slider2->setSnap(true);
          slider2->setSnapPos(0.5);

        // slider : quantize

        SAT_SliderWidget* slider3 = new SAT_SliderWidget(SAT_Rect(220,170,200,20),"Quantize",0.2);
        scrollbox->appendChildWidget(slider3);
          slider3->setTextOffset(SAT_Rect(5,0,0,0));
          slider3->setValueOffset(SAT_Rect(0,0,5,0));
          slider3->setQuantize(true);
          slider3->setQuantizeSteps(6);

        // knob

        SAT_KnobWidget* knob1 = new SAT_KnobWidget(SAT_Rect(220,210,60,60),"Vol",0.7);
        scrollbox->appendChildWidget(knob1);

        // knob : bipolar + snap

        SAT_KnobWidget* knob2 = new SAT_KnobWidget(SAT_Rect(290,210,60,60),"Pan",0.5);
        scrollbox->appendChildWidget(knob2);
          knob2->setBipolar(true);
          knob2->setSnap(true);
          knob2->setSnapPos(0.5);

        // knob: quantize

        SAT_KnobWidget* knob3 = new SAT_KnobWidget(SAT_Rect(360,210,60,60),"%",0.4);
        scrollbox->appendChildWidget(knob3);
          knob3->setQuantize(true);
          knob3->setQuantizeSteps(11);

        // curve

        SAT_CurveWidget* curve1 = new SAT_CurveWidget(SAT_Rect(220,280,30,30),false);
        scrollbox->appendChildWidget(curve1);
          curve1->setSnap(true);
          curve1->setSnapPos(0.5);

        SAT_CurveWidget* curve2 = new SAT_CurveWidget(SAT_Rect(260,280,30,30),true);
        scrollbox->appendChildWidget(curve2);

        // button

        SAT_ButtonWidget* button1 = new SAT_ButtonWidget(SAT_Rect(220,320,95,20),true);
        scrollbox->appendChildWidget(button1);
        button1->setTexts("Switch ON","Switch OFF");

        SAT_ButtonWidget* button2 = new SAT_ButtonWidget(SAT_Rect(325,320,95,20),false);
        scrollbox->appendChildWidget(button2);
        button2->setTexts("Toggle ON","Toggle OFF");

        // buttonrow

        SAT_ButtonRowWidget* buttonrow1 = new SAT_ButtonRowWidget(SAT_Rect(220,350,200,20),5,buttonrow_txt,SAT_BUTTON_ROW_SINGLE,false);
        scrollbox->appendChildWidget(buttonrow1);

        SAT_ButtonRowWidget* buttonrow2 = new SAT_ButtonRowWidget(SAT_Rect(220,380,200,20),5,buttonrow_txt,SAT_BUTTON_ROW_MULTI,false);
        scrollbox->appendChildWidget(buttonrow2);

        // selector

        SAT_SelectorWidget* selector = new SAT_SelectorWidget(SAT_Rect(220,410,200,20),"Selector",MSelectorMenu);
        scrollbox->appendChildWidget(selector);

        //--------------------
        // column 3
        //--------------------

        // dual slider

        SAT_DualSliderWidget* dual_slider = new SAT_DualSliderWidget(SAT_Rect(430,10,200,20),"Dual",0.2,0.7);
        scrollbox->appendChildWidget(dual_slider);

        // dual value

        SAT_DualValueWidget* dual_value = new SAT_DualValueWidget(SAT_Rect(430,40,200,20),0.2,0.7);
        scrollbox->appendChildWidget(dual_value);

        // range bar

        SAT_RangeBarWidget* range_bar = new SAT_RangeBarWidget(SAT_Rect(430,70,200,20));
        scrollbox->appendChildWidget(range_bar);





        SAT_KeyboardWidget* keyboard = new SAT_KeyboardWidget(SAT_Rect(430,130,200,40));
        scrollbox->appendChildWidget(keyboard);

        SAT_SliderBankWidget* sliderbank = new SAT_SliderBankWidget(SAT_Rect(430,180,200,40),10);
        scrollbox->appendChildWidget(sliderbank);

        SAT_ValueGraphWidget* valuegraph = new SAT_ValueGraphWidget(SAT_Rect(430,230,200,40),10);
        scrollbox->appendChildWidget(valuegraph);

//        SAT_MovableWidget* movable = new SAT_MovableWidget(SAT_Rect(430,190,95,50));
//        scrollbox->appendChildWidget(movable);

        SAT_GridWidget* grid = new SAT_GridWidget(SAT_Rect(430,280,200,50),10,5);
        scrollbox->appendChildWidget(grid);

        SAT_GroupBoxWidget* groupbox = new SAT_GroupBoxWidget(SAT_Rect(430,340,200,100),20,false);
        scrollbox->appendChildWidget(groupbox);
          groupbox->getContainer()->setFillBackground(true);
          groupbox->getContainer()->setBackgroundColor(SAT_DarkGreen);


        //--------------------
        // column 4
        //--------------------

        SAT_GraphWidget* graph = new SAT_GraphWidget(SAT_Rect(640,10,300,300));
        scrollbox->appendChildWidget(graph);
          graph->setFillBackground(true);
          //graph->setAlignment(SAT_EDGE_BOTTOM);
          //graph->setStretching(SAT_EDGE_RIGHT | SAT_EDGE_BOTTOM);
          for (uint32_t i=0; i<6; i++) {
            SAT_GraphModule* module = new SAT_GraphModule();
            module->numInputs = 2;
            module->inputs[0] = SAT_PIN_SIGNAL;
            module->outputs[0] = SAT_PIN_SIGNAL;
            module->numOutputs = 2;
            graph->addModule(module,i*15,i*15,"module");
          }

        SAT_TimelineWidget* timeline = new SAT_TimelineWidget(SAT_Rect(640,320,300,100));
        scrollbox->appendChildWidget(timeline);
          timeline->setDrawBorder(true);
          timeline->setBorderColor(SAT_LightGrey);
          timeline->setBorderWidth(0.5);
          SAT_TimelineTrack* track1 = new SAT_TimelineTrack("Track 1");
          timeline->addTrack(track1);
            SAT_TimelineSegment* segment1 = new SAT_TimelineSegment("Clip1",0,10);
            track1->addSegment(segment1);
            SAT_TimelineSegment* segment2 = new SAT_TimelineSegment("C2",11,15);
            track1->addSegment(segment2);
          SAT_TimelineTrack* track2 = new SAT_TimelineTrack("Track 2");
          timeline->addTrack(track2);
            SAT_TimelineSegment* segment3 = new SAT_TimelineSegment("3",5.2,17);
            track2->addSegment(segment3);
          SAT_TimelineTrack* track3 = new SAT_TimelineTrack("Track 3");
          timeline->addTrack(track3);

        // dummy

        SAT_Widget* dummy1 = new SAT_Widget(SAT_Rect(940,490,10,10));
        scrollbox->appendChildWidget(dummy1);

    return page;
  }

//------------------------------
private: // params page
//------------------------------

  SAT_PanelWidget* create_params_page(SAT_Editor* AEditor, SAT_Window* AWindow) {
    SAT_PanelWidget* page = new SAT_PanelWidget(0);
    //page->setBackgroundColor(SAT_Grey);

      SAT_SliderWidget* slider;
      slider = new SAT_SliderWidget(SAT_Rect(10,10,200,20),"param1",0);
      page->appendChildWidget(slider);
      AEditor->connect(slider,getParameter(0));

      slider = new SAT_SliderWidget(SAT_Rect(10,40,200,20),"param2",0);
      page->appendChildWidget(slider);
      AEditor->connect(slider,getParameter(1));

      slider = new SAT_SliderWidget(SAT_Rect(10,70,200,20),"param3",0);
      page->appendChildWidget(slider);
      AEditor->connect(slider,getParameter(2));

      slider = new SAT_SliderWidget(SAT_Rect(10,100,200,20),"param4",0);
      page->appendChildWidget(slider);
      AEditor->connect(slider,getParameter(3));

      slider = new SAT_SliderWidget(SAT_Rect(10,130,200,20),"param5",0);
      page->appendChildWidget(slider);
      AEditor->connect(slider,getParameter(4));

      slider = new SAT_SliderWidget(SAT_Rect(10,160,200,20),"param6",0);
      page->appendChildWidget(slider);
      AEditor->connect(slider,getParameter(5));

      slider = new SAT_SliderWidget(SAT_Rect(10,190,200,20),"param7",0);
      page->appendChildWidget(slider);
      AEditor->connect(slider,getParameter(6));

      slider = new SAT_SliderWidget(SAT_Rect(10,220,200,20),"param8",0);
      page->appendChildWidget(slider);
      AEditor->connect(slider,getParameter(7));

      slider = new SAT_SliderWidget(SAT_Rect(10,250,200,20),"param9",0);
      page->appendChildWidget(slider);
      AEditor->connect(slider,getParameter(8));

    return page;
  }

//------------------------------
private: // host page
//------------------------------

  SAT_PanelWidget* create_host_page(SAT_Editor* AEditor, SAT_Window* AWindow) {
    SAT_PanelWidget* page = new SAT_PanelWidget(0);
    //page->setBackgroundColor(SAT_Grey);

    return page;
  }

};

//----------------------------------------------------------------------
//
// entry
//
//----------------------------------------------------------------------

#include "plugin/sat_entry.h"
SAT_PLUGIN_ENTRY(sat_demo_descriptor,sat_demo_plugin);

