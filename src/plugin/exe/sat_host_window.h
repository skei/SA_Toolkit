#ifndef sat_host_window_inluded
#define sat_host_window_inluded
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/clap/sat_clap.h"
#include "gui/sat_window.h"

#ifdef SAT_PAINTER_NANOVG
  #include "gui/nanovg/sat_nanovg.h"
#endif

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_HostWindow
: public SAT_ImplementedWindow {

//------------------------------
public:
//------------------------------

  const clap_plugin_t*      MClapPlugin   = nullptr;
  const clap_plugin_gui_t*  MClapGui      = nullptr;
  clap_process_t            MClapProcess  = {0};

//------------------------------
public:
//------------------------------

  SAT_HostWindow(uint32_t AWidth, uint32_t AHeight, intptr_t AParent,const clap_plugin_t* AClapPlugin)
  : SAT_ImplementedWindow(AWidth,AHeight,AParent) {
    MClapPlugin = AClapPlugin;
    MClapGui = (const clap_plugin_gui_t*)MClapPlugin->get_extension(MClapPlugin,CLAP_EXT_GUI);;
  }

  //----------

  virtual ~SAT_HostWindow() {
  }

//------------------------------
public:
//------------------------------

  /*
    embedded windows don't receive a window resized event,
    so we call set_size manually..
    we could try to get the editor from the plugin (only if we're using an embedded editor),
    then the window from that, and finally call on_window_resize

    todo: proportional scaling..
  */

  void on_window_resize(uint32_t AWidth, uint32_t AHeight) override {
    if (MClapGui) MClapGui->set_size(MClapPlugin,AWidth,AHeight);
  }

};

//----------------------------------------------------------------------
#endif
