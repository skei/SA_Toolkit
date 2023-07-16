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

#define SA_DEMO2_NUM_DEMO_BUTTONS 2

const char* sa_demo2_demo_buttons_txt[SA_DEMO2_NUM_DEMO_BUTTONS] = {
  "home",
  "host"
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
  
  SAT_PanelWidget*      MRoot         = nullptr;
  SAT_PanelWidget*      MRootPanel    = nullptr;
  SAT_PanelWidget*      MCenterPanel  = nullptr;
  SAT_PanelWidget*      MInnerPanel   = nullptr;
  
  SAT_TextBoxWidget*    MTextOutput   = nullptr;
  SAT_ButtonRowWidget*  MDemoButtons  = nullptr;

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

        MCenterPanel = new SAT_PanelWidget(0);
        aspect_rect->appendChildWidget(MCenterPanel);
        //MCenterPanel->setLayout(SAT_WIDGET_ALIGN_FILL,SAT_WIDGET_STRETCH_ALL);
        MCenterPanel->setAlignment(SAT_WIDGET_ALIGN_FILL);
        MCenterPanel->setInnerBorder(SAT_Rect(10,10,5,5));
        MCenterPanel->setSpacing(SAT_Point(5,5));
        MCenterPanel->setFillBackground(false);
        MCenterPanel->setDrawBorder(false);
        MCenterPanel->setRoundedCorners(true);
        MCenterPanel->setCornerSize(5);
        MCenterPanel->setDrawDropShadow(true);
        MCenterPanel->setDropShadowInner(true);
        MCenterPanel->setDropShadowColors(SAT_Grey,SAT_Color(0,0,0,0.5));
        MCenterPanel->setDropShadowCorner(5);
        MCenterPanel->setDropShadowFeather(5);
    
          MInnerPanel = new SAT_PanelWidget(150);
          MCenterPanel->appendChildWidget(MInnerPanel);
          MInnerPanel->setLayout(SAT_WIDGET_ALIGN_LEFT_TOP,SAT_WIDGET_STRETCH_VERTICAL);
          MInnerPanel->setInnerBorder(SAT_Rect(5,5,10,5));
          MInnerPanel->setFillBackground(true);
          MInnerPanel->setBackgroundColor(SAT_DarkGrey);
          MInnerPanel->setDrawBorder(true);
          MInnerPanel->setRoundedCorners(true);
          MInnerPanel->setCornerSize(5);
          MInnerPanel->setDrawDropShadow(true);
          MInnerPanel->setDropShadowColors(SAT_Color(0,0,0,0.5),SAT_Grey);
          MInnerPanel->setDropShadowCorner(5);
          MInnerPanel->setDropShadowFeather(5);
          MInnerPanel->setDropShadowOffset(3,3);
          
            MDemoButtons = new SAT_ButtonRowWidget(SAT_Rect(15*SA_DEMO2_NUM_DEMO_BUTTONS),SA_DEMO2_NUM_DEMO_BUTTONS,sa_demo2_demo_buttons_txt,SAT_BUTTON_ROW_SINGLE,SAT_BUTTON_ROW_VERT);
            MInnerPanel->appendChildWidget(MDemoButtons);
            MDemoButtons->setLayout(SAT_WIDGET_ALIGN_TOP_LEFT,SAT_WIDGET_STRETCH_HORIZONTAL);
            MDemoButtons->setCellFillBackground(true);
            MDemoButtons->setCellBackgroundColor(SAT_DarkGrey);
            MDemoButtons->setCellDrawBorder(false);
            MDemoButtons->selectButton(0);
            
          MTextOutput = new SAT_TextBoxWidget(150);
          MCenterPanel->appendChildWidget(MTextOutput);
          MTextOutput->setLayout(SAT_WIDGET_ALIGN_BOTTOM_LEFT,SAT_WIDGET_STRETCH_HORIZONTAL);
          MTextOutput->setOuterBorder(SAT_Rect(0,0,5,5));    
          MTextOutput->getContentWidget()->setFillBackground(true);
          MTextOutput->getContentWidget()->setBackgroundColor(SAT_Grey);
          MTextOutput->getContentWidget()->setInnerBorder(SAT_Rect(5,5,5,5));
          MTextOutput->appendLine("OK");
    
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

