#ifndef sat_debug_window_included
#define sat_debug_window_included
//----------------------------------------------------------------------

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

    #ifdef SAT_DEBUG_OBSERVER
      MObserver = new SAT_DebugObserverWidget(SAT_Rect(10,10,300,480));
      root->appendChild(MObserver);
      //MObserver->setLayout(SAT_WIDGET_ALIGN_TOP_LEFT, SAT_WIDGET_STRETCH_VERTICAL);
      MObserver->setFillBackground(true);
      MObserver->setBackgroundColor(SAT_Grey);
    #endif

    MTextBox = new SAT_TextBoxWidget(SAT_Rect(0,0,320,480),true,false);
    MTextBox->Layout.flags = SAT_WIDGET_LAYOUT_ANCHOR_TOP_RIGHT | SAT_WIDGET_LAYOUT_STRETCH_VERT;
    root->appendChild(MTextBox);

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

  void on_timerListener_callback(SAT_Timer* ATimer, double ADelta) override {
    //SAT_TRACE;
    SAT_Window::on_timerListener_callback(ATimer,ADelta);

    #ifdef SAT_DEBG_OBSERVER
      uint32_t num = SAT_GLOBAL.DEBUG.OBSERVER.getNumObservers();
      for (uint32_t i=0; i<num; i++) {
        SAT_Observable* obs = SAT_GLOBAL.DEBUG.OBSERVER.getObserver(i);
      }
    #endif

    //MTextBox->appendLine("text...");
    MTextBox->do_widget_realign(MTextBox);
    MTextBox->do_widget_redraw(MTextBox);
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