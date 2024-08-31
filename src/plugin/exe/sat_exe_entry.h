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

const clap_plugin_entry_t*      sat_exe_ClapEntry      = nullptr;
const clap_plugin_factory_t*    sat_exe_ClapFactory    = nullptr;
const clap_host_t*              sat_exe_ClapHost       = nullptr;
const clap_plugin_descriptor_t* sat_exe_ClapDescriptor = nullptr;
const clap_plugin_t*            sat_exe_ClapPlugin     = nullptr;
const clap_plugin_gui_t*        sat_exe_ClapGui        = nullptr;

// #ifndef SAT_NO_GUI
//   SAT_HostWindow*               sat_exe_HostWindow     = nullptr;
// #endif

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

bool sat_exe_initFromMemory(const clap_plugin_entry_t* AClapEntry, const char *APath) {
  sat_exe_ClapEntry = AClapEntry;
  sat_exe_ClapEntry->init(APath);
  sat_exe_ClapFactory = (clap_plugin_factory_t*)sat_exe_ClapEntry->get_factory(CLAP_PLUGIN_FACTORY_ID);
  return true;
}

//----------

bool sat_exe_initFromFile(const char* APath) {
  return false;
}

//----------

void sat_exe_deinit() {
}

//----------

bool sat_exe_loadPlugin(const clap_host_t* host, uint32_t index) {
  sat_exe_ClapDescriptor = sat_exe_ClapFactory->get_plugin_descriptor(sat_exe_ClapFactory,index);
  sat_exe_ClapPlugin = sat_exe_ClapFactory->create_plugin(sat_exe_ClapFactory,host,sat_exe_ClapDescriptor->id);
  sat_exe_ClapPlugin->init(sat_exe_ClapPlugin);
  sat_exe_ClapGui = (clap_plugin_gui_t*)sat_exe_ClapPlugin->get_extension(sat_exe_ClapPlugin,CLAP_EXT_GUI);
  return true;
}

//----------

void sat_exe_unloadPlugin() {
  sat_exe_ClapPlugin->destroy(sat_exe_ClapPlugin);
}

//------------------------------
//
//------------------------------

#ifndef SAT_NO_GUI

SAT_HostWindow* sat_exe_openEditor(SAT_HostWindow* AHostWindow) {
  //MHostWindow = AHostWindow;
  #ifdef SAT_LINUX
    sat_exe_ClapGui->create(sat_exe_ClapPlugin,CLAP_WINDOW_API_X11,false);
  #endif
  #ifdef SAT_WIN32
    MClapGui->create(MClapPlugin,CLAP_WINDOW_API_WIN32,false);
  #endif
  sat_exe_ClapGui->set_scale(sat_exe_ClapPlugin,1.0);
  uint32_t width,height;
  sat_exe_ClapGui->get_size(sat_exe_ClapPlugin,&width,&height);

  #ifdef SAT_LINUX
    xcb_window_t x11window = AHostWindow->getXcbWindow();
    const clap_window_t clapwindow = { .api = CLAP_WINDOW_API_X11, .x11 = x11window };
  #endif
  #ifdef SAT_WIN32
    HWND win32window = AHostWindow->getWin32Window();
    const clap_window_t clapwindow = { .api = CLAP_WINDOW_API_WIN32, .win32 = win32window };
  #endif

  sat_exe_ClapGui->set_parent(sat_exe_ClapPlugin,&clapwindow);
  sat_exe_ClapGui->show(sat_exe_ClapPlugin);
  sat_exe_ClapGui->set_size(sat_exe_ClapPlugin,width,height); // is this needed?

  return AHostWindow;
}

//----------

void sat_exe_closeEditor() {
  sat_exe_ClapGui->hide(sat_exe_ClapPlugin);
  sat_exe_ClapGui->destroy(sat_exe_ClapPlugin);
  //SAT_PRINT("editor closed\n");
}

//----------

void sat_exe_getEditorSize(uint32_t* width, uint32_t* height) {
  sat_exe_ClapGui->get_size(sat_exe_ClapPlugin,width,height);
}

//----------

void sat_exe_mainloop(SAT_HostWindow* AHostWindow) {
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
  sat_exe_initFromMemory(&clap_entry,"");
  sat_exe_loadPlugin(host->getClapHost(),0);

  // todo: start audio/midi

  #ifndef SAT_NO_GUI
    if (sat_exe_ClapGui) {
      uint32_t width,height;
      sat_exe_getEditorSize(&width,&height);
      SAT_HostWindow* window = new SAT_HostWindow(width,height,0,sat_exe_ClapPlugin);
      window->setTitle("SAT_ExeEntry");
      window->show();
      SAT_HostWindow* hostwindow = sat_exe_openEditor(window);
      //hostwindow->on_Window_show();
      sat_exe_mainloop(hostwindow);
      //hostwindow->on_Window_hide();
      sat_exe_closeEditor();
      window->hide();
      delete window;
    }
  #endif

  // todo: stop audio/midi

  sat_exe_unloadPlugin();
  sat_exe_deinit();
  delete host;
  return 0;
}

#endif

//----------------------------------------------------------------------
#endif
