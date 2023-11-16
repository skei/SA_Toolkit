
//----------------------------------------------------------------------

#include "plugin/sat_plugin.h"
#include "plugin/sat_editor.h"
#include "gui/sat_widgets.h"

//----------

#define EDITOR_WIDTH  800
#define EDITOR_HEIGHT 600
#define EDITOR_SCALE  1.0

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

  sat_demo_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : SAT_Plugin(ADescriptor,AHost) {
  }

  //----------

  virtual ~sat_demo_plugin() {
  }

//------------------------------
public:
//------------------------------

  bool init() final {
    registerDefaultSynthExtensions();
    appendClapNoteInputPort("input");
    appendStereoAudioInputPort("input");
    appendStereoAudioOutputPort("output");
    appendParameter( new SAT_Parameter("param1",0.5));
    setInitialEditorSize(EDITOR_WIDTH,EDITOR_HEIGHT,EDITOR_SCALE);

    return SAT_Plugin::init();
  }

  //----------

  void setupEditorWindow(SAT_Editor* AEditor, SAT_Window* AWindow) final {
    SAT_RootWidget* root = new SAT_RootWidget(0,AWindow);
    AWindow->setRootWidget(root);
    root->setLayoutInnerBorder(SAT_Rect(10,10,10,10));
    init_editor(AEditor,AWindow,root);
  }
  
//------------------------------
private: // editor
//------------------------------

  void init_editor(SAT_Editor* AEditor, SAT_Window* AWindow, SAT_RootWidget* root) {
    SAT_TabsWidget* tabs = new SAT_TabsWidget(SAT_Rect(10,10,100,100),32,20);
    root->appendChildWidget(tabs);
    tabs->addLayoutFlag(SAT_WIDGET_LAYOUT_STRETCH_ALL);
    tabs->appendPage("widgets",   create_widgets_page(AEditor,AWindow) );
    tabs->appendPage("parameters",create_params_page(AEditor,AWindow)  );
    tabs->appendPage("host",      create_host_page(AEditor,AWindow)  );
    tabs->selectPage(0);
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
          panel->setFillGradient(true);
          panel->setGradientColors(SAT_Grey,0.45,0.55);
          panel->setRoundedCorners(true);
          panel->setCornerSize(8);

        // panel : drop shadow

        panel = new SAT_PanelWidget(SAT_Rect(10,160,200,30));
        scrollbox->appendChildWidget(panel);
          panel->setFillBackground(true);
          //panel->setBackgroundColor(SAT_Grey);
          panel->setFillGradient(true);
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

        // knpb: quantize

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


        //--------------------
        // column 1
        //--------------------

        SAT_KeyboardWidget* keyboard = new SAT_KeyboardWidget(SAT_Rect(430,10,170,40));
        scrollbox->appendChildWidget(keyboard);

        SAT_SliderBankWidget* sliderbank = new SAT_SliderBankWidget(SAT_Rect(430,70,170,40),10);
        scrollbox->appendChildWidget(sliderbank);

        SAT_ValueGraphWidget* valuegraph = new SAT_ValueGraphWidget(SAT_Rect(430,120,170,40),10);
        scrollbox->appendChildWidget(valuegraph);

        //SAT_TextEditWidget* textedit = new SAT_TextEditWidget(SAT_Rect(430,170,170,20),"TextEdit");
        //scrollbox->appendChildWidget(textedit);




    return page;
  }

//------------------------------
private: // params page
//------------------------------

  SAT_PanelWidget* create_params_page(SAT_Editor* AEditor, SAT_Window* AWindow) {
    SAT_PanelWidget* page = new SAT_PanelWidget(0);
    //page->setBackgroundColor(SAT_Grey);

      SAT_SliderWidget* slider1 = new SAT_SliderWidget(SAT_Rect(10,10,200,20),"p value",0.5);
      page->appendChildWidget(slider1);
      AEditor->connect(slider1,getParameter(0));

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

