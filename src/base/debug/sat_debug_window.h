#ifndef sat_debug_window_included
#define sat_debug_window_included
//----------------------------------------------------------------------

#ifdef SAT_DEBUG_WINDOW

#include "sat.h"
//#include "gui/sat_window.h"
//#include "gui/sat_widgets.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_DebugWindow
/*: public SAT_Window*/ {
  
//------------------------------
private:
//------------------------------

  SAT_Print* MPrint      = nullptr;

  // SAT_PanelWidget*          panel     = nullptr;
  // SAT_TextBoxWidget*        textbox   = nullptr;
  //  
  // #ifdef SAT_DEBUG_OBSERVER
  // SAT_DebugObserverWidget*  observer = nullptr;
  // #endif

//------------------------------
public:
//------------------------------

  bool initialize(SAT_Print* APrint) {
    MPrint = APrint;
    return true;
  }

  //----------

  void cleanup() {
  }

//------------------------------
public:
//------------------------------
  
  SAT_DebugWindow(/*uint32_t AWidth, uint32_t AHeight, intptr_t AParent=0, SAT_WindowListener* AListener=nullptr*/)
  /*: SAT_Window(AWidth,AHeight,AParent,AListener)*/ {
    // SAT_TRACE;
    // setTitle("SAT_DebugWindow");
    // panel = new SAT_PanelWidget(200);
    // appendRootWidget(panel);
    // //panel->setFillBackground(false);
    // //anel->setDrawBorder(false);
    // //panel->setInnerBorder(10);
    // //panel->setSpacing(10);
    // #ifdef SAT_DEBUG_OBSERVER
    //   observer = new SAT_DebugObserverWidget(SAT_Rect(0,0,300,480));
    //   panel->appendChildWidget(observer);
    //   //observer->setLayout(SAT_WIDGET_ALIGN_TOP_LEFT, SAT_WIDGET_STRETCH_VERTICAL);
    //   observer->setFillBackground(true);
    //   observer->setBackgroundColor(SAT_Grey);
    // #endif
    // textbox = new SAT_TextBoxWidget(SAT_Rect(300,0,340,480));
    // panel->appendChildWidget(textbox);
    // //textbox->setLayout(SAT_WIDGET_ALIGN_TOP_LEFT, SAT_WIDGET_STRETCH_VERTICAL);
  }
  
  //----------
  
  virtual ~SAT_DebugWindow() {
    // SAT_PRINT;
  }

//------------------------------
public:
//------------------------------

  // void on_timerListener_callback(SAT_Timer* ATimer) override {
  //   //SAT_PRINT;
  //   // uint32_t num = SAT_GLOBAL.DEBUG.getNumObservers();
  //   // for (uint32_t i=0; i<num; i++) {
  //   //   SAT_Observable* obs = SAT_GLOBAL.DEBUG.getObserver(i);
  //   // }
  //   SAT_Window::on_timerListener_callback(ATimer);
  // }
  
};

#endif // SAT_DEBUG_WINDOW

//----------------------------------------------------------------------
#endif