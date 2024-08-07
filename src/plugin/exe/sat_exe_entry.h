#ifndef sat_exe_entry_included
#define sat_exe_entry_included
//----------------------------------------------------------------------

// todo: move most of this to dedicated hosting class

#include "sat.h"
#include "plugin/clap/sat_clap.h"
#include "plugin/exe/sat_exe.h"
#include "plugin/sat_host_implementation.h"

#ifndef SAT_NO_GUI
  #include "plugin/exe/sat_host_window.h"
  #include "gui/sat_window.h"
#endif

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

  const clap_plugin_entry_t*      MClapEntry      = nullptr;
  const clap_plugin_factory_t*    MClapFactory    = nullptr;
  const clap_host_t*              MClapHost       = nullptr;
  const clap_plugin_descriptor_t* MClapDescriptor = nullptr;
  const clap_plugin_t*            MClapPlugin     = nullptr;
  const clap_plugin_gui_t*        MClapGui        = nullptr;
  
  #ifndef SAT_NO_GUI
  SAT_HostWindow*                 MHostWindow     = nullptr;
  #endif

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

  bool initFromMemory(const clap_plugin_entry_t* AClapEntry, const char *APath) {
    MClapEntry = AClapEntry;
    MClapEntry->init(APath);
    MClapFactory = (clap_plugin_factory_t*)MClapEntry->get_factory(CLAP_PLUGIN_FACTORY_ID);
    return true;
  }

  //----------

  bool initFromFile(const char* APath) {
    return false;
  }

  //----------

  void deinit() {
  }

  //----------

  bool loadPlugin(const clap_host_t* host, uint32_t index) {
    MClapDescriptor = MClapFactory->get_plugin_descriptor(MClapFactory,index);
    MClapPlugin = MClapFactory->create_plugin(MClapFactory,host,MClapDescriptor->id);
    MClapPlugin->init(MClapPlugin);
    MClapGui = (clap_plugin_gui_t*)MClapPlugin->get_extension(MClapPlugin,CLAP_EXT_GUI);
    return true;
  }

  //----------

  void unloadPlugin() {
    MClapPlugin->destroy(MClapPlugin);
  }

//------------------------------
//
//------------------------------

  #ifndef SAT_NO_GUI

  SAT_HostWindow* openEditor(SAT_HostWindow* AHostWindow) {
    //MHostWindow = AHostWindow;
    #ifdef SAT_LINUX
      MClapGui->create(MClapPlugin,CLAP_WINDOW_API_X11,false);
    #endif
    #ifdef SAT_WIN32
      MClapGui->create(MClapPlugin,CLAP_WINDOW_API_WIN32,false);
    #endif
    MClapGui->set_scale(MClapPlugin,1.0);
    uint32_t width,height;
    MClapGui->get_size(MClapPlugin,&width,&height);

    #ifdef SAT_LINUX
      xcb_window_t x11window = AHostWindow->getXcbWindow();
      const clap_window_t clapwindow = { .api = CLAP_WINDOW_API_X11, .x11 = x11window };
    #endif
    #ifdef SAT_WIN32
      HWND win32window = AHostWindow->getWin32Window();
      const clap_window_t clapwindow = { .api = CLAP_WINDOW_API_WIN32, .win32 = win32window };
    #endif

    MClapGui->set_parent(MClapPlugin,&clapwindow);
    MClapGui->show(MClapPlugin);
    MClapGui->set_size(MClapPlugin,width,height); // is this needed?

    return AHostWindow;
  }

  //----------

  void closeEditor() {
    MClapGui->hide(MClapPlugin);
    MClapGui->destroy(MClapPlugin);
    //SAT_PRINT("editor closed\n");
  }

  //----------

  void getEditorSize(uint32_t* width, uint32_t* height) {
    MClapGui->get_size(MClapPlugin,width,height);
  }

  //----------

  void mainloop(SAT_HostWindow* AHostWindow) {
    //AHostWindow->startTimer(1000);
    AHostWindow->eventLoop();
    //AHostWindow->stopTimer();
  }

  #endif // gui

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#ifndef SAT_PLUGIN_EXE_NO_MAIN

int main(int argc, char** argv) {
  SAT_HostImplementation* host = new SAT_HostImplementation();
  initFromMemory(&clap_entry,"");
  loadPlugin(host->getClapHost(),0);

  // todo: start audio/midi

  #ifndef SAT_NO_GUI
    if (MClapGui) {
      uint32_t width,height;
      getEditorSize(&width,&height);
      SAT_HostWindow* window = new SAT_HostWindow(width,height,0,MClapPlugin);
      window->setTitle("SAT_ExeEntry");
      window->show();
      SAT_HostWindow* hostwindow = openEditor(window);
      //hostwindow->on_window_show();
      mainloop(hostwindow);
      //hostwindow->on_window_hide();
      closeEditor();
      window->hide();
      delete window;
    }
  #endif

  // todo: stop audio/midi

  unloadPlugin();
  deinit();
  delete host;
  return 0;
}

#endif

//----------------------------------------------------------------------
#endif
