/*
  in '/bin' directory:
    ../build/compile -i ../build/build.cpp -o build.exe -f exe -g x11 -d  
  in separate console:
    nc -U -l -k /tmp/sat.socket
*/
//----------------------------------------------------------------------

//#define SAT_DEBUG_PRINT_SOCKET
//#define SAT_DEBUG_MEMTRACE

#include "sat.h"
#include "plugin/sat_plugin.h"
#include "gui/sat_window.h"
#include "gui/sat_widgets.h"

#include "base/utils/sat_easing.h"
#include "base/utils/sat_windowing.h"
#include "base/utils/sat_tween_manager.h"

#define EDITOR_WIDTH  500
#define EDITOR_HEIGHT 500
#define EDITOR_SCALE  1.0

//----------------------------------------------------------------------
//
// tests
//
//----------------------------------------------------------------------

#if defined(SAT_TESTS)
  bool build_test1() { return true; }
  bool build_test2() { return true; }
  SAT_TEST("BUILD: test1",build_test1)
  SAT_TEST("BUILD: test2 yo, man!",build_test2)
#endif

//----------------------------------------------------------------------
//
// widget
//
//----------------------------------------------------------------------

class myWidget
: public SAT_Widget {

private:

  SAT_Color MColor1 = SAT_Yellow;
  SAT_Color MColor2 = SAT_Red;

public:

  myWidget(SAT_Rect ARect)
  : SAT_Widget(ARect) {
    setName("myWidget");

    //void* memory_leak = malloc(666);

  }

  //----------
  
  void on_widget_paint(SAT_PaintContext* AContext) final {
    //SAT_Print("%s\n",getName());
    SAT_BasePainter* painter = AContext->painter;
    //double scale = AContext->scale;
    SAT_Rect rect = getRect();
    painter->setFillColor(MColor1);
    painter->fillRect(rect.x,rect.y,rect.w,rect.h);
    painter->setFillColor(MColor2);
    double centerx = rect.x + (rect.w * 0.5);
    double centery = rect.y + (rect.h * 0.5);
    double radiusx = rect.w * 0.4;
    double radiusy = rect.h * 0.4;
    painter->fillEllipse(centerx,centery,radiusx,radiusy);
    paintChildWidgets(AContext);

    //SAT_Assert(1==0);

    //SAT_GLOBAL.DEBUG.printCallStack();

    //int* ptr = nullptr;
    //int i = *ptr;
    //SAT_Print("%i\n",i);

  }

  //----------
  
  void on_widget_mouseClick(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp) final {
    SAT_PRINT;
    SAT_Color temp = MColor1;
    MColor1 = MColor2;
    MColor2 = temp;
    do_widget_redraw(this);
  }

  //void on_widget_timer(SAT_Timer* ATimer, double AElapsed) final {
  //  SAT_Print("elapsed %f\n",AElapsed);
  //}

};

//----------------------------------------------------------------------
//
// main
//
//----------------------------------------------------------------------

int main() {

  //SAT_Print("Hello world!\n");
  //SAT_PRINT;
  //SAT_DPrint("hello #2\n");
  SAT_LOG("test %i\n",1);
  SAT_RUN_TESTS

  //-----

  SAT_Window* window = new SAT_Window(EDITOR_WIDTH,EDITOR_HEIGHT);

  SAT_RootWidget* root = new SAT_RootWidget(0,window);
  window->setRootWidget(root);
  root->setText("Hello, world!");

  myWidget* widget = new myWidget(SAT_Rect(10,10,100,100));
  root->appendChildWidget(widget);

  SAT_DragValueWidget* dragvalue = new SAT_DragValueWidget(SAT_Rect(10,120,200,20),"Value",0.5);
  root->appendChildWidget(dragvalue);
  
  //window->registerTimerWidget(widget);

  window->open();
  window->eventLoop();
  window->close();

  delete window;

  return 0;
}

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t myDescriptor = {
  .clap_version = CLAP_VERSION,
  .id           = "me/test1",
  .name         = "test1",
  .vendor       = "me",
  .url          = "website.com",
  .manual_url   = "website.com/manual",
  .support_url  = "website.com/support",
  .version      = "0.0.1",
  .description  = "my plugin",
  .features     = (const char*[]){ CLAP_PLUGIN_FEATURE_AUDIO_EFFECT, nullptr }
};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class myPlugin
: public SAT_Plugin {

//------------------------------
public:
//------------------------------

  myPlugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : SAT_Plugin(ADescriptor,AHost) {
    //SAT_Print("hello world!\n");
  }

  //----------

  virtual ~myPlugin() {
    //SAT_Print("goodbye world!\n");
  }

//------------------------------
public:
//------------------------------

  bool init() override {
    //SAT_PRINT;
    registerDefaultExtension();
    //appendClapNoteInputPort("input");
    appendStereoAudioInputPort("input");
    appendStereoAudioOutputPort("output");
    appendParameter( new SAT_Parameter("param1",0.5));
    //setInitialEditorSize();
    return SAT_Plugin::init();
  }

  //----------

  void setupEditorWindow(SAT_Editor* AEditor, SAT_Window* AWindow) override {
    //SAT_PRINT;
    //AWindow->setInitialSize(EDITOR_WIDTH,EDITOR_HEIGHT,EDITOR_SCALE);
    SAT_RootWidget* root = new SAT_RootWidget(SAT_Rect(100,100),AWindow);
    AWindow->setRootWidget(root);

    myWidget* widget = new myWidget(SAT_Rect(10,10,100,100));
    root->appendChildWidget(widget);

    SAT_DragValueWidget* dragvalue = new SAT_DragValueWidget(SAT_Rect(10,120,200,20),"Value",0.5);
    root->appendChildWidget(dragvalue);
  
    //AWindow->registerTimerWidget(widget);

  }

};

//----------------------------------------------------------------------
//
// entry
//
//----------------------------------------------------------------------

#include "plugin/sat_entry.h"
SAT_PLUGIN_ENTRY(myDescriptor,myPlugin);

