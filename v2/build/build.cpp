/*

  ./compile -i build.cpp -o build.exe -f exe -g x11 -d
  nc -U -l -k /tmp/sat.socket

*/
//----------------------------------------------------------------------

//#define SAT_NO_TESTS
//#define SAT_DEBUG_PRINT_SOCKET

#include "sat.h"
#include "gui/sat_window.h"
//#include "gui/sat_widget.h"
#include "plugin/sat_plugin.h"

#define WINDOW_WIDTH  500
#define WINDOW_HEIGHT 500

const clap_plugin_descriptor_t myDescriptor = {
  .clap_version = CLAP_VERSION,
  .id           = "me/myPlugin",
  .name         = "myPlugin",
  .vendor       = "me",
  .url          = "website.com",
  .manual_url   = "website.com/manual",
  .support_url  = "website.com/support",
  .version      = "0.0.2",
  .description  = "my plugin",
  .features     = (const char*[]){ CLAP_PLUGIN_FEATURE_AUDIO_EFFECT, nullptr }
};

//----------------------------------------------------------------------
//
// window
//
//----------------------------------------------------------------------

// class myWindow
// : public SAT_Window {
// public:
//   myWindow(uint32_t AWidth, uint32_t AHeight, intptr_t AParent=0)
//   : SAT_Window(AWidth,AHeight,AParent) {
//   }
// public:
// };

//----------------------------------------------------------------------
//
// root widget
//
//----------------------------------------------------------------------

class myRootWidget
: public SAT_RootWidget {

private:

  SAT_Color color1 = SAT_Yellow;
  SAT_Color color2 = SAT_Red;

public:

  myRootWidget(SAT_Rect ARect,SAT_WidgetListener* AListener)
  : SAT_RootWidget(ARect,AListener) {
  }
  
  void on_widget_paint(SAT_PaintContext* AContext) final {
    SAT_Print("\n");
    SAT_BasePainter* painter = AContext->painter;
    SAT_Rect rect = getRect();
    painter->setFillColor(color1);
    painter->fillRect(rect.x,rect.y,rect.w,rect.h);
    painter->setFillColor(color2);
    double centerx = rect.x + (rect.w * 0.5);
    double centery = rect.y + (rect.h * 0.5);
    double radiusx = rect.w * 0.4;
    double radiusy = rect.h * 0.4;
    painter->fillEllipse(centerx,centery,radiusx,radiusy);
  }

};

//----------------------------------------------------------------------
//
// tests
//
//----------------------------------------------------------------------

bool build_test1() {
  //SAT_Print("Hello world!\n");
  return false;
}

//----------

SAT_TEST("BUILD: test1",build_test1)

//----------------------------------------------------------------------
//
// main
//
//----------------------------------------------------------------------

int main() {
  SAT_LOG("test %i\n",1);
  SAT_Print("Hello world!\n");
  SAT_RUN_TESTS
  SAT_Window* window = new SAT_Window(WINDOW_WIDTH,WINDOW_HEIGHT);
  myRootWidget* root = new myRootWidget(0,window);
  window->setRootWidget(root);
  window->open();
  window->eventLoop();
  window->close();
  delete root;
  delete window;
  return 0;
}

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
  }

  //----------

  virtual ~myPlugin() {
  }


};

//----------------------------------------------------------------------
//
// entry
//
//----------------------------------------------------------------------

SAT_PLUGIN_ENTRY(myDescriptor,myPlugin);

/*

void SAT_Register(SAT_Registry* ARegistry) {
  SAT_Print("Hello world!\n");
}

const clap_plugin_t* SAT_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
  SAT_Print("Hello world!\n");
  switch (AIndex) {
    case 0:
      myPlugin* plugin = new myPlugin(ADescriptor,AHost);
      return plugin->getClapPlugin();
  }
  SAT_Print("ERROR! No plugin at index %i\n",AIndex);
  return nullptr;
}

*/
