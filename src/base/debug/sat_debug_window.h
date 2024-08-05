#ifndef sat_debug_window_included
#define sat_debug_window_included
//----------------------------------------------------------------------

/*
  todo:
    - conditions for each observed varianle
      for example, draw text differently if certain conditions are true
      (null ptr, value < 0.0, ..)
*/

//#ifdef SAT_DEBUG_WINDOW

#define SAT_DEBUG_WINDOW_WIDTH  640
#define SAT_DEBUG_WINDOW_HEIGHT 480

#include "sat.h"
#include "gui/sat_window.h"
#include "gui/sat_widgets.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_DebugWindow
: public SAT_Window {
  
//------------------------------
private:
//------------------------------

//SAT_Print*                MPrint    = nullptr;
//SAT_PanelWidget*          panel     = nullptr;

SAT_TextBoxWidget*          MTextBox   = nullptr;

#ifdef SAT_DEBUG_OBSERVER
  SAT_DebugObserverWidget*  MObserver  = nullptr;
#endif

//------------------------------
public:
//------------------------------

  SAT_DebugWindow()
  : SAT_Window(SAT_DEBUG_WINDOW_WIDTH,SAT_DEBUG_WINDOW_HEIGHT) {
    SAT_TRACE;

    setTitle("SAT_DebugWindow");

    SAT_RootWidget* root = new SAT_RootWidget(this);
    setRootWidget(root);
    root->Layout.innerBorder = {10,10,10,10};

    MTextBox = new SAT_TextBoxWidget(200,true,true);
    MTextBox->Layout.flags = SAT_WIDGET_LAYOUT_ANCHOR_TOP_RIGHT | SAT_WIDGET_LAYOUT_STRETCH_VERT | SAT_WIDGET_LAYOUT_FILL_RIGHT;
    root->appendChild(MTextBox);

    #ifdef SAT_DEBUG_OBSERVER
      MObserver = new SAT_DebugObserverWidget(200);
      root->appendChild(MObserver);
      MObserver->Layout.flags = SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT | SAT_WIDGET_LAYOUT_STRETCH_VERT | SAT_WIDGET_LAYOUT_FILL_LEFT;
      MObserver->setFillBackground(true);
      MObserver->setBackgroundColor(SAT_Grey);
    #endif


    show();
    startEventThread();




  }

  //----------

  virtual ~SAT_DebugWindow() {
    SAT_TRACE;
    stopEventThread();
    hide();
  }

//------------------------------
public:
//------------------------------

  // bool initialize(SAT_Print* APrint) {
  //   MPrint = APrint;
  //   return true;
  // }

  //----------

  // void cleanup() {
  // }

//------------------------------
public:
//------------------------------

  void on_TimerListener_callback(double ADelta) override {
    //SAT_TRACE;
    SAT_Window::on_TimerListener_callback(ADelta);

    #ifdef SAT_DEBG_OBSERVER
      uint32_t num = SAT_GLOBAL.DEBUG.OBSERVER.getNumObservers();
      for (uint32_t i=0; i<num; i++) {
        SAT_Observable* obs = SAT_GLOBAL.DEBUG.OBSERVER.getObserver(i);
      }
    #endif

    //MTextBox->appendLine("text...");
    MTextBox->do_widget_realign(MTextBox,SAT_WIDGET_REALIGN_SELF);
//    MTextBox->do_widget_redraw(MTextBox,0,SAT_WIDGET_REDRAW_SELF);
  }

  // void on_timerListener_callback(SAT_Timer* ATimer) override {
  //   //SAT_PRINT;
  //   // uint32_t num = SAT_GLOBAL.DEBUG.getNumObservers();
  //   // for (uint32_t i=0; i<num; i++) {
  //   //   SAT_Observable* obs = SAT_GLOBAL.DEBUG.getObserver(i);
  //   // }
  //   SAT_Window::on_timerListener_callback(ATimer);
  // }
  
};

//#endif // SAT_DEBUG_WINDOW

//----------------------------------------------------------------------
#endif