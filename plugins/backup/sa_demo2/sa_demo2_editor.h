#ifndef sa_demo2_editor_included
#define sa_demo2_editor_included
//----------------------------------------------------------------------

//#include "sa_demo2_page_host.h"
//#include "sa_demo2_page_plugin.h"
//#include "sa_demo2_page_parameters.h"
//#include "sa_demo2_page_audio.h"
//#include "sa_demo2_page_synth.h"
//#include "sa_demo2_page_widgets.h"
//#include "sa_demo2_page_layout.h"
//#include "sa_demo2_page_painting.h"
//#include "sa_demo2_page_animation.h"

//----------------------------------------------------------------------

#define SA_DEMO2_NUM_DEMO_BUTTONS 7

const char* sa_demo2_demo_buttons_txt[SA_DEMO2_NUM_DEMO_BUTTONS] = {
  "start",
  "host",
  "audio",
  "parameters",
  "voices",
  "widgets",
  "layout"
};

//----------------------------------------------------------------------

class sa_demo2_editor
: public SAT_Editor
/*, public SAT_WidgetListener*/ {
  
//------------------------------
private:
//------------------------------

  SAT_Host*             MHost         = nullptr;
  SAT_Plugin*           MPlugin       = nullptr;
  
  SAT_PanelWidget*      MRoot             = nullptr;
  SAT_PanelWidget*      MRootPanel        = nullptr;
  SAT_PanelWidget*      MMainPanel        = nullptr;
  SAT_PanelWidget*      MLeftPanel        = nullptr;
  SAT_PanelWidget*      MRightTopPanel    = nullptr;
  SAT_PanelWidget*      MRightBottomPanel = nullptr;
  
  SAT_TextBoxWidget*    MTextOutput       = nullptr;
  SAT_ButtonRowWidget*  MLeftMenu         = nullptr;

//  SAT_PluginHeaderWidget* MPluginHeader = nullptr;
//  SAT_PluginFooterWidget* MPluginFooter = nullptr;
//  SAT_PanelWidget*        MLeftColumn   = nullptr;
//  SAT_PagesWidget*        MDemoPages    = nullptr;
//  SAT_ButtonRowWidget*    MDemoButtons  = nullptr;
  
//------------------------------
public:
//------------------------------

  sa_demo2_editor(SAT_EditorListener* AListener, uint32_t AWidth, uint32_t AHeight, SAT_Host* AHost, SAT_Plugin* APlugin)
  : SAT_Editor(AListener,AWidth,AHeight) {
    MHost = AHost;
    MPlugin = APlugin;
  }

  //----------

  virtual ~sa_demo2_editor() {
  }
  
//------------------------------
public:
//------------------------------

  void set_track_name() {
  }

//------------------------------
public:
//------------------------------

//  void do_widgetListener_select(SAT_Widget* ASender, int32_t AIndex, int32_t ASubIndex) {
//  }

  //----------

//  void do_windowListener_update_widget(SAT_Widget* ASender, uint32_t AMode, uint32_t AIndex) override {
//    SAT_Editor::do_windowListener_update_widget(ASender,AMode,AIndex);
//  }
  
  //----------
  
//  void do_windowListener_timer(SAT_Window* AWindow, double AElapsed) override {
//    SAT_Editor::do_windowListener_timer(AWindow,AElapsed);
//  }

  //----------
  
//  void do_windowListener_timer(SAT_Window* AWindow, double AElapsed) {
//    SAT_Editor::do_windowListener_timer(AWindow,AElapsed);
//  }

//------------------------------
public:
//------------------------------

  bool initialize(SAT_Window* AWindow, const char* AFormat) {
    //SAT_PRINT;
    
    // root

    MRoot = new SAT_PanelWidget( SAT_Rect(0,0,EDITOR_WIDTH,EDITOR_HEIGHT) );
    AWindow->appendRootWidget(MRoot);
    
    //MRoot->setAlignment(SAT_WIDGET_ALIGN_FILL);
    MRoot->setFillBackground(true);
    MRoot->setBackgroundColor(SAT_DarkGrey);
    MRoot->setDrawBorder(false);
    MRoot->setInnerBorder(SAT_Rect(5,50,5,20));
    
      SAT_Widget* aspect_rect = new SAT_Widget(SAT_Rect(0,0,EDITOR_WIDTH-10,EDITOR_HEIGHT-70)); // -header/footer/border
      MRoot->appendChildWidget(aspect_rect);

        MMainPanel = new SAT_PanelWidget(0);
        aspect_rect->appendChildWidget(MMainPanel);
        //MMainPanel->setLayout(SAT_WIDGET_ALIGN_FILL,SAT_WIDGET_STRETCH_ALL);
        MMainPanel->setAlignment(SAT_WIDGET_ALIGN_FILL);
        MMainPanel->setInnerBorder(SAT_Rect(10,10,5,5));
        MMainPanel->setSpacing(SAT_Point(5,5));
        MMainPanel->setFillBackground(false);
        MMainPanel->setDrawBorder(false);
        MMainPanel->setRoundedCorners(true);
        MMainPanel->setCornerSize(5);
        MMainPanel->setDrawDropShadow(true);
        MMainPanel->setDropShadowInner(true);
        MMainPanel->setDropShadowColors(SAT_Grey,SAT_Color(0,0,0,0.5));
        MMainPanel->setDropShadowCorner(5);
        MMainPanel->setDropShadowFeather(5);
    
          MLeftPanel = new SAT_PanelWidget(100);
          MMainPanel->appendChildWidget(MLeftPanel);
          MLeftPanel->setLayout(SAT_WIDGET_ALIGN_LEFT,SAT_WIDGET_STRETCH_VERTICAL);
          MLeftPanel->setInnerBorder(SAT_Rect(5,5,10,5));
          MLeftPanel->setFillBackground(true);
          MLeftPanel->setBackgroundColor(SAT_DarkGrey);
          MLeftPanel->setDrawBorder(true);
          MLeftPanel->setRoundedCorners(true);
          MLeftPanel->setCornerSize(5);
          MLeftPanel->setDrawDropShadow(true);
          MLeftPanel->setDropShadowColors(SAT_Color(0,0,0,0.5),SAT_Grey);
          MLeftPanel->setDropShadowCorner(5);
          MLeftPanel->setDropShadowFeather(5);
          MLeftPanel->setDropShadowOffset(3,3);
          
            MLeftMenu = new SAT_ButtonRowWidget(SAT_Rect(15*SA_DEMO2_NUM_DEMO_BUTTONS),SA_DEMO2_NUM_DEMO_BUTTONS,sa_demo2_demo_buttons_txt,SAT_BUTTON_ROW_SINGLE,SAT_BUTTON_ROW_VERT);
            MLeftPanel->appendChildWidget(MLeftMenu);
            MLeftMenu->setLayout(SAT_WIDGET_ALIGN_TOP_LEFT,SAT_WIDGET_STRETCH_HORIZONTAL);
            MLeftMenu->setCellFillBackground(true);
            MLeftMenu->setCellBackgroundColor(SAT_DarkGrey);
            MLeftMenu->setCellDrawBorder(false);
            MLeftMenu->selectButton(0);

          MRightBottomPanel = new SAT_PanelWidget(100);
          MMainPanel->appendChildWidget(MRightBottomPanel);
          MRightBottomPanel->setLayout(SAT_WIDGET_ALIGN_BOTTOM,SAT_WIDGET_STRETCH_HORIZONTAL);
          MRightBottomPanel->setInnerBorder(SAT_Rect(5,5,10,10));
          MRightBottomPanel->setFillBackground(true);
          MRightBottomPanel->setBackgroundColor(SAT_DarkGrey);
          MRightBottomPanel->setDrawBorder(true);
          MRightBottomPanel->setRoundedCorners(true);
          MRightBottomPanel->setCornerSize(5);
          MRightBottomPanel->setDrawDropShadow(true);
          MRightBottomPanel->setDropShadowColors(SAT_Color(0,0,0,0.5),SAT_Grey);
          MRightBottomPanel->setDropShadowCorner(5);
          MRightBottomPanel->setDropShadowFeather(5);
          MRightBottomPanel->setDropShadowOffset(3,3);
            
            MTextOutput = new SAT_TextBoxWidget(0);
            MRightBottomPanel->appendChildWidget(MTextOutput);
            MTextOutput->setLayout(SAT_WIDGET_ALIGN_FILL,SAT_WIDGET_STRETCH_ALL);
            //MTextOutput->setOuterBorder(SAT_Rect(0,0,5,5));    
            MTextOutput->setDrawBorder(false);
            MTextOutput->getContentWidget()->setFillBackground(true);
            MTextOutput->getContentWidget()->setBackgroundColor(SAT_DarkGrey);
            MTextOutput->getContentWidget()->setInnerBorder(SAT_Rect(5,5,5,5));
            MTextOutput->appendLine("OK");

          MRightTopPanel = new SAT_PanelWidget(150);
          MMainPanel->appendChildWidget(MRightTopPanel);
          MRightTopPanel->setLayout(SAT_WIDGET_ALIGN_FILL,SAT_WIDGET_STRETCH_ALL);
          MRightTopPanel->setInnerBorder(SAT_Rect(5,5,10,10));
          MRightTopPanel->setFillBackground(true);
          MRightTopPanel->setBackgroundColor(SAT_DarkGrey);
          MRightTopPanel->setDrawBorder(true);
          MRightTopPanel->setRoundedCorners(true);
          MRightTopPanel->setCornerSize(5);
          MRightTopPanel->setDrawDropShadow(true);
          MRightTopPanel->setDropShadowColors(SAT_Color(0,0,0,0.5),SAT_Grey);
          MRightTopPanel->setDropShadowCorner(5);
          MRightTopPanel->setDropShadowFeather(5);
          MRightTopPanel->setDropShadowOffset(3,3);
    
    return true;
  }
  
//------------------------------
public:
//------------------------------

//  void setProcessCounter(uint32_t process_counter) {
//  }

//  void setProcessSteadyTime(uint32_t steady_time) {
//  }
  
};

//----------

#undef SA_DEMO2_NUM_DEMO_BUTTONS

//----------------------------------------------------------------------
#endif

