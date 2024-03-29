#ifndef demo_included
#define demo_included
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
  "parameters",
  "host",
  "voices",
  "events"
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

    //char* memleak_ptr = nullptr;

//------------------------------
public:
//------------------------------

  //SAT_DEFAULT_PLUGIN_CONSTRUCTOR(sat_demo_plugin)
  
  sat_demo_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : SAT_Plugin(ADescriptor,AHost) {

    //SAT_PRINT;
    //SAT_Print("some text.. %i %p\n",123,nullptr);
    //SAT_DPrint("..or colored " SAT_TERM_FG_LIGHT_GREEN "text..\n" SAT_TERM_RESET );

    //SAT_LOG("hello world!\n");

    //int* ptr = nullptr;
    //int val = *ptr;
    //SAT_Print("%p = %i\n",ptr,val);

    //memleak_ptr = (char*)malloc(666);
    //memleak_ptr = new char[666];

  }

  //----------

  virtual ~sat_demo_plugin() {
    //SAT_PRINT;
    
    //free(memleak_ptr);
    //delete memleak_ptr;
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

    appendParameter( new SAT_Parameter("p1",0.1) );
    appendParameter( new SAT_Parameter("p2",0.2) );
    appendParameter( new SAT_Parameter("p3",0.3) );
    appendParameter( new SAT_Parameter("p4",0.4) );
    appendParameter( new SAT_Parameter("p5",0.5) );
    appendParameter( new SAT_Parameter("p6",0.6) );
    appendParameter( new SAT_Parameter("p7",0.7) );
    appendParameter( new SAT_Parameter("p8",0.8) );
    appendParameter( new SAT_Parameter("p9",0.9) );

    setAllParameterFlags(CLAP_PARAM_IS_AUTOMATABLE);
    setAllParameterFlags(CLAP_PARAM_IS_MODULATABLE);

    setInitialEditorSize(EDITOR_WIDTH,EDITOR_HEIGHT,EDITOR_SCALE);
    for (uint32_t i=0; i<2048; i++) MBuffer[i] = SAT_RandomRange( -0.8, 0.8 );
    return SAT_Plugin::init();
  }

  //----------

  void setupEditorWindow(SAT_Editor* AEditor, SAT_Window* AWindow) final {
    SAT_RootWidget* root = new SAT_RootWidget(AWindow);
    AWindow->setRootWidget(root);
    //root->setLayoutInnerBorder(SAT_Rect(10,10,10,10));
    init_editor(AEditor,AWindow,root);
  }
  
//------------------------------
private: // editor
//------------------------------

  void init_editor(SAT_Editor* AEditor, SAT_Window* AWindow, SAT_RootWidget* root) {

    MSelectorMenu = new SAT_MenuWidget(SAT_Rect(0,0,200,15*5+2+2));
      MSelectorMenu->setLayoutInnerBorder(SAT_Rect(2,2,2,2));
      //MSelectorMenu->setLayoutSpacing(SAT_Point(1,1));
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
    tabs->appendPage("voices",    create_voices_page(AEditor,AWindow)  );
    tabs->appendPage("events",    create_events_page(AEditor,AWindow)  );
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

        SAT_PanelWidget* column1 = new SAT_PanelWidget(210);
        scrollbox->appendChildWidget(column1);
        column1->addLayoutFlag(SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT);
        column1->addLayoutFlag(SAT_WIDGET_LAYOUT_STRETCH_VERT);
        column1->addLayoutFlag(SAT_WIDGET_LAYOUT_CROP_TOP);
        column1->setLayoutInnerBorder(SAT_Rect(10,10,10,10));
        column1->setLayoutSpacing(SAT_Point(5,5));

        SAT_PanelWidget* panel;

        // panel : text

        panel = new SAT_PanelWidget(20);
        column1->appendChildWidget(panel);
          panel->addLayoutFlag(SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT);
          panel->addLayoutFlag(SAT_WIDGET_LAYOUT_STRETCH_HORIZ);
          panel->addLayoutFlag(SAT_WIDGET_LAYOUT_CROP_TOP);
          panel->setDrawText("Text");
          panel->setText("Text");


        // panel : text + value

        panel = new SAT_PanelWidget(20);
        column1->appendChildWidget(panel);
          panel->addLayoutFlag(SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT);
          panel->addLayoutFlag(SAT_WIDGET_LAYOUT_STRETCH_HORIZ);
          panel->addLayoutFlag(SAT_WIDGET_LAYOUT_CROP_TOP);
          panel->setDrawText(true);
          panel->setText("Value");
          panel->setTextAlignment(SAT_TEXT_ALIGN_LEFT);
          panel->setTextOffset(SAT_Rect(5,0,0,0));
          panel->setDrawValue(true);
          panel->setValue(0.5);
          panel->setValueAlignment(SAT_TEXT_ALIGN_RIGHT);
          panel->setValueOffset(SAT_Rect(0,0,5,0));
          panel->setDrawBorder(false);//true);
          //panel->setBorderColor(SAT_Grey);
          //panel->setBorderWidth(1);
          panel->setRoundedCorners(true);
          panel->setCornerSize(6);
          panel->setDrawBevel(true);
          panel->setBevelInset(true);

        // panel : color

        panel = new SAT_PanelWidget(20);
        column1->appendChildWidget(panel);
          panel->addLayoutFlag(SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT);
          panel->addLayoutFlag(SAT_WIDGET_LAYOUT_STRETCH_HORIZ);
          panel->addLayoutFlag(SAT_WIDGET_LAYOUT_CROP_TOP);
          panel->setFillBackground(true);
          panel->setBackgroundColor(SAT_Cyan);
          panel->setDrawBorder(true);
          panel->setBorderColor(SAT_Black);
          panel->setBorderWidth(1);

        // panel : rounded corners

        panel = new SAT_PanelWidget(20);
        column1->appendChildWidget(panel);
          panel->addLayoutFlag(SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT);
          panel->addLayoutFlag(SAT_WIDGET_LAYOUT_STRETCH_HORIZ);
          panel->addLayoutFlag(SAT_WIDGET_LAYOUT_CROP_TOP);
          panel->setDrawBorder(true);
          panel->setBorderColor(SAT_LightGrey);
          panel->setBorderWidth(2);
          panel->setRoundedCorners(true);
          //panel->setCornerSize(8);
          panel->setCornerSizes(8,8,0,8);
          //panel->setBorderCorners(SAT_CORNER_UPPER_RIGHT + SAT_CORNER_LOWER_RIGHT + SAT_CORNER_LOWER_LEFT);

        // panel : gradient

        panel = new SAT_PanelWidget(20);
        column1->appendChildWidget(panel);
          panel->addLayoutFlag(SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT);
          panel->addLayoutFlag(SAT_WIDGET_LAYOUT_STRETCH_HORIZ);
          panel->addLayoutFlag(SAT_WIDGET_LAYOUT_CROP_TOP);
          panel->setFillLinearGradient(true);
          panel->setGradientColors(SAT_Grey,0.45,0.55);
          panel->setRoundedCorners(true);
          panel->setCornerSize(8);

        // panel : drop shadow

        panel = new SAT_PanelWidget(30);
        column1->appendChildWidget(panel);
          panel->addLayoutFlag(SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT);
          panel->addLayoutFlag(SAT_WIDGET_LAYOUT_STRETCH_HORIZ);
          panel->addLayoutFlag(SAT_WIDGET_LAYOUT_CROP_TOP);
          panel->setFillBackground(true);
          panel->setBackgroundColor(SAT_Grey);
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

        panel = new SAT_PanelWidget(30);
        column1->appendChildWidget(panel);
          panel->addLayoutFlag(SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT);
          panel->addLayoutFlag(SAT_WIDGET_LAYOUT_STRETCH_HORIZ);
          panel->addLayoutFlag(SAT_WIDGET_LAYOUT_CROP_TOP);
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

        // panel : font

        panel = new SAT_PanelWidget(30);
        column1->appendChildWidget(panel);
          panel->addLayoutFlag(SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT);
          panel->addLayoutFlag(SAT_WIDGET_LAYOUT_STRETCH_HORIZ);
          panel->addLayoutFlag(SAT_WIDGET_LAYOUT_CROP_TOP);
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

        panel = new SAT_PanelWidget(30);
        column1->appendChildWidget(panel);
          panel->addLayoutFlag(SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT);
          panel->addLayoutFlag(SAT_WIDGET_LAYOUT_STRETCH_HORIZ);
          panel->addLayoutFlag(SAT_WIDGET_LAYOUT_CROP_TOP);
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

        SAT_LogoWidget* logo = new SAT_LogoWidget(SAT_Rect(60,60));
        column1->appendChildWidget(logo);
          logo->addLayoutFlag(SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT);
          //logo->addLayoutFlag(SAT_WIDGET_LAYOUT_STRETCH_HORIZ);
          //logo->addLayoutFlag(SAT_WIDGET_LAYOUT_CROP_TOP);

        // image

        SAT_ImageWidget* image = new SAT_ImageWidget(SAT_Rect(65,0,60,60),(void*)SA,SA_size);
        column1->appendChildWidget(image);
          image->setScale(0.6);
          image->addLayoutFlag(SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT);
          //image->addLayoutFlag(SAT_WIDGET_LAYOUT_STRETCH_HORIZ);
          //image->addLayoutFlag(SAT_WIDGET_LAYOUT_CROP_TOP);

        // circular waveform

        SAT_CircularWaveformWidget* circular_waveform = new SAT_CircularWaveformWidget(SAT_Rect(130,0,60,60), 128 );
        column1->appendChildWidget(circular_waveform);
          circular_waveform->addLayoutFlag(SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT);
          //circular_waveform->addLayoutFlag(SAT_WIDGET_LAYOUT_STRETCH_HORIZ);
          circular_waveform->addLayoutFlag(SAT_WIDGET_LAYOUT_CROP_TOP);
          circular_waveform->setBackgroundColor(0.375);
          circular_waveform->setWaveformBackgroundColor(0.35);
          circular_waveform->setZeroLineColor(0.2);
          circular_waveform->setAudioBuffer(2048,MBuffer,false);
          circular_waveform->setWaveformWidth(1);

        // symbols

        for (uint32_t i=0; i<9; i++) {
          double x = i * 20;
          double y = 0;
          SAT_SymbolWidget* widget = new SAT_SymbolWidget(SAT_Rect(x,y,16,16),i);
          widget->addLayoutFlag(SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT);
          column1->appendChildWidget(widget);
        }

        for (uint32_t i=0; i<9; i++) {
          double x = i * 20;
          double y = 20;
          SAT_SymbolWidget* widget = new SAT_SymbolWidget(SAT_Rect(x,y,16,16), i + 9);
          widget->addLayoutFlag(SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT);
          column1->appendChildWidget(widget);
        }

        // waveform

        SAT_WaveformWidget* waveform = new SAT_WaveformWidget(SAT_Rect(0,45,200,50));
        column1->appendChildWidget(waveform);
          waveform->setNumGrid(4);
          waveform->setNumSubGrid(2);
          waveform->setBuffer(MBuffer,2048);
          waveform->addLayoutFlag(SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT);
          waveform->addLayoutFlag(SAT_WIDGET_LAYOUT_STRETCH_HORIZ);
          waveform->addLayoutFlag(SAT_WIDGET_LAYOUT_CROP_TOP);

        //--------------------
        // column 2
        //--------------------

        // drag value

        SAT_DragValueWidget* dragvalue1 = new SAT_DragValueWidget(SAT_Rect(220,10,200,20),"DragValue",0.1);
        scrollbox->appendChildWidget(dragvalue1);
          dragvalue1->setTextOffset(SAT_Rect(5,0,0,0));
          dragvalue1->setValueOffset(SAT_Rect(0,0,5,0));
          //dragvalue1->setAutoLockCursor(false);
          //dragvalue1->setAutoHideCursor(false);

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

        // frequency response

        SAT_FrequencyResponseWidget<SAT_FRTestFilter>* freqresp = new SAT_FrequencyResponseWidget<SAT_FRTestFilter>(SAT_Rect(300,280,30,30));
        scrollbox->appendChildWidget(freqresp);
        freqresp->do_fft();

        // movable

        SAT_MovableWidget* movable = new SAT_MovableWidget(SAT_Rect(340,280,80,30));
        scrollbox->appendChildWidget(movable);
        movable->setIsMovable(true);
        movable->setIsSizable(true);

        // button

        SAT_ButtonWidget* button1 = new SAT_ButtonWidget(SAT_Rect(220,320,95,20),true);
        scrollbox->appendChildWidget(button1);
        button1->setTexts("Switch ON","Switch OFF");

        SAT_ButtonWidget* button2 = new SAT_ButtonWidget(SAT_Rect(325,320,95,20),false);
        scrollbox->appendChildWidget(button2);
        button2->setTexts("ON","OFF");

        // buttonrow

        SAT_ButtonRowWidget* buttonrow1 = new SAT_ButtonRowWidget(SAT_Rect(220,350,200,20),5,buttonrow_txt,SAT_BUTTON_ROW_SINGLE,false);
        scrollbox->appendChildWidget(buttonrow1);
        buttonrow1->setFillLinearGradient(true);
        buttonrow1->setGradientColors(SAT_Grey,0.2,0.2);


        SAT_ButtonRowWidget* buttonrow2 = new SAT_ButtonRowWidget(SAT_Rect(220,380,200,20),5,buttonrow_txt,SAT_BUTTON_ROW_MULTI,false);
        scrollbox->appendChildWidget(buttonrow2);
        buttonrow2->setRoundedCorners(true);
        buttonrow2->setCornerSize(7);

        // selector

        SAT_SelectorWidget* selector = new SAT_SelectorWidget(SAT_Rect(220,410,200,20),"Selector",MSelectorMenu);
        scrollbox->appendChildWidget(selector);

        SAT_TextEditWidget* textedit = new SAT_TextEditWidget(SAT_Rect(220,440,200,20),"edit me..");
        scrollbox->appendChildWidget(textedit);

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

        SAT_GridWidget* grid = new SAT_GridWidget(SAT_Rect(430,280,200,50),10,5);
        scrollbox->appendChildWidget(grid);

        SAT_TextBoxWidget* textbox = new SAT_TextBoxWidget(SAT_Rect(430,340,200,100));
        scrollbox->appendChildWidget(textbox);
        textbox->appendLine("Hello world!");
        textbox->appendLine("The quick brown fox jumps over the lazy dog");

        SAT_GroupBoxWidget* groupbox = new SAT_GroupBoxWidget(SAT_Rect(430,450,200,100),20,false);
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

//        SAT_Widget* dummy1 = new SAT_Widget(SAT_Rect(1150,490,10,10));
//        scrollbox->appendChildWidget(dummy1);

        //--------------------
        // column 5
        //--------------------

        SAT_PanelWidget* p5 = new SAT_GraphWidget(SAT_Rect(950,10,200,200));
        scrollbox->appendChildWidget(p5);


    return page;
  }

//------------------------------
private: // params page
//------------------------------

  SAT_PanelWidget* create_params_page(SAT_Editor* AEditor, SAT_Window* AWindow) {
    SAT_PanelWidget* page = new SAT_PanelWidget(0);
    //page->setBackgroundColor(SAT_Grey);

      SAT_KnobWidget* knob;

      knob = new SAT_KnobWidget(SAT_Rect(10,10,60,60),"p 1",0);
      page->appendChildWidget(knob);
      AEditor->connect(knob,getParameter(0));

      knob = new SAT_KnobWidget(SAT_Rect(80,10,60,60),"p 2",0);
      page->appendChildWidget(knob);
      AEditor->connect(knob,getParameter(1));

      knob = new SAT_KnobWidget(SAT_Rect(150,10,60,60),"p 3",0);
      page->appendChildWidget(knob);
      AEditor->connect(knob,getParameter(2));

      knob = new SAT_KnobWidget(SAT_Rect(220,10,60,60),"p 4",0);
      page->appendChildWidget(knob);
      AEditor->connect(knob,getParameter(3));


      knob = new SAT_KnobWidget(SAT_Rect(10,80,60,60),"p 5",0);
      page->appendChildWidget(knob);
      AEditor->connect(knob,getParameter(4));

      knob = new SAT_KnobWidget(SAT_Rect(80,80,60,60),"p 6",0);
      page->appendChildWidget(knob);
      AEditor->connect(knob,getParameter(5));

      knob = new SAT_KnobWidget(SAT_Rect(150,80,60,60),"p 7",0);
      page->appendChildWidget(knob);
      AEditor->connect(knob,getParameter(6));

      knob = new SAT_KnobWidget(SAT_Rect(220,80,60,60),"p 8",0);
      page->appendChildWidget(knob);
      AEditor->connect(knob,getParameter(7));


      knob = new SAT_KnobWidget(SAT_Rect(300,10,60,60),"p 9",0);
      page->appendChildWidget(knob);
      AEditor->connect(knob,getParameter(8));
      knob->setTextColor(SAT_DarkerGrey);
      knob->setTextSize(12);

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

//------------------------------
private: // voices page
//------------------------------

  SAT_PanelWidget* create_voices_page(SAT_Editor* AEditor, SAT_Window* AWindow) {
    SAT_PanelWidget* page = new SAT_PanelWidget(0);
    //page->setBackgroundColor(SAT_Grey);
    return page;
  }

//------------------------------
private: // events page
//------------------------------

  SAT_PanelWidget* create_events_page(SAT_Editor* AEditor, SAT_Window* AWindow) {
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

#ifndef SAT_NO_ENTRY
  #include "plugin/sat_entry.h"
  SAT_PLUGIN_ENTRY(sat_demo_descriptor,sat_demo_plugin);
#endif


//----------------------------------------------------------------------
#endif
