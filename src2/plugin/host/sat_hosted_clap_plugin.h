#ifndef sat_hosted_clap_plugin_included
#define sat_hosted_clap_plugin_included
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/lib/sat_clap.h"
#include "plugin/host/sat_host_window.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_HostedClapPlugin {

//------------------------------
private:
//------------------------------

  const clap_plugin_entry_t*      MClapEntry      = nullptr;
  const clap_plugin_factory_t*    MClapFactory    = nullptr;
  const clap_host_t*              MClapHost       = nullptr;
  const clap_plugin_descriptor_t* MClapDescriptor = nullptr;
  const clap_plugin_t*            MClapPlugin     = nullptr;
  const clap_plugin_gui_t*        MClapGui        = nullptr;

  SAT_HostWindow* MHostWindow = nullptr;

//------------------------------
public:
//------------------------------

  SAT_HostedClapPlugin() {
  }

  //----------

  virtual ~SAT_HostedClapPlugin() {
  }

//------------------------------
public:
//------------------------------

  virtual bool loadFromMemory(const clap_plugin_entry_t* AClapEntry, const char* APath="", uint32_t AIndex=0) {
    MClapEntry = AClapEntry;
    MClapEntry->init(APath);
    MClapFactory = (clap_plugin_factory_t*)MClapEntry->get_factory(CLAP_PLUGIN_FACTORY_ID);
    MClapDescriptor = MClapFactory->get_plugin_descriptor(MClapFactory,AIndex);
    return false;
  }

  //----------

  virtual bool loadFromFile(const char* APath, uint32_t AIndex) {
    return false;
  }

  //----------

  virtual void unload() {
  }

//------------------------------
public:
//------------------------------

  virtual bool createInstance(const clap_host_t* AHost) {
    MClapPlugin = MClapFactory->create_plugin(MClapFactory,AHost,MClapDescriptor->id);
    MClapPlugin->init(MClapPlugin);
    MClapGui = (clap_plugin_gui_t*)MClapPlugin->get_extension(MClapPlugin,CLAP_EXT_GUI);
    return false;
  }

  //----------

  virtual bool destroyInstance() {
    MClapPlugin->destroy(MClapPlugin);
  }

  //----------

  virtual bool initInstance() {
    return false;
  }

  //----------

  virtual bool openEditor(SAT_HostWindow* AHostWindow) {
    MHostEditor = AHostEditor;
    #ifdef SAT_LINUX
      MClapGui->create(MClapPlugin,CLAP_WINDOW_API_X11,false);
    #endif
    #ifdef SAT_WIN32
      MClapGui->create(plugin,CLAP_WINDOW_API_WIN32,false);
    #endif
    MClapGui->set_scale(MClapPlugin,1.0);
    uint32_t width,height;
    MClapGui->get_size(MClapPlugin,&width,&height);

    #ifdef SAT_LINUX
      xcb_window_t x11window = AHostWindow->getXcbWindow();
      const clap_window_t clapwindow = { .api = CLAP_WINDOW_API_X11, .x11 = x11window };
    #endif
    #ifdef SAT_WIN32
      HWND win32window = HostWindow->getWin32Window();
      const clap_window_t clapwindow = { .api = CLAP_WINDOW_API_WIN32, .win32 = win32window };
    #endif

    MClapGui->set_parent(MClapPlugin,&clapwindow);
    MClapGui->show(MClapPlugin);
    MClapGui->set_size(MClapPlugin,width,height); // is this needed?

    return false;
  }

  //----------

  virtual void closeEditor() {
    MHostEditor = nullptr;
    MClapGui->hide(MClapPlugin);
    MClapGui->destroy(MClapPlugin);
    //SAT_PRINT("editor closed\n");
  }

  //----------

  void getEditorSize(uint32_t* width, uint32_t* height) {
    MClapGui->get_size(MClapPlugin,width,height);
  }

  void mainloop(SAT_HostWindow* AHostWindow) {
    MHostWindow->eventLoop();
  }


};

//----------------------------------------------------------------------
#endif