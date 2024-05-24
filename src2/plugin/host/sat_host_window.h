#ifndef sat_host_window_inluded
#define sat_host_window_inluded
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/lib/sat_clap.h"
#include "gui/sat_window.h"

#ifdef SAT_PAINTER_NANOVG
  #include "gui/lib/sat_nanovg.h"
#endif

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_HostWindow
: public SAT_BasicWindow {

//------------------------------
public:
//------------------------------

  const clap_plugin_t*      MClapPlugin = nullptr;
  const clap_plugin_gui_t*  MClapGui    = nullptr;

//------------------------------
public:
//------------------------------

  SAT_HostWindow(uint32_t AWidth, uint32_t AHeight, intptr_t AParent,const clap_plugin_t* AClapPlugin)
  : SAT_BasicWindow(AWidth,AHeight,AParent) {
    MClapPlugin = AClapPlugin;
    MClapGui = (const clap_plugin_gui_t*)MClapPlugin->get_extension(MClapPlugin,CLAP_EXT_GUI);;
  }

  //----------

  virtual ~SAT_HostWindow() {
  }

//------------------------------
public:
//------------------------------

  void on_window_resize(uint32_t AWidth, uint32_t AHeight) override {
    //SAT_PRINT("w %i h %i\n",AWidth,AHeight);
    if (MClapGui) MClapGui->set_size(MClapPlugin,AWidth,AHeight);
  }

  //----------

  // void on_window_show() override {
  //   SAT_TRACE;
  //   SAT_BasicWindow::on_window_show();
  // }

  //----------

  // void on_window_hide() override {
  //   SAT_TRACE;
  //   SAT_BasicWindow::on_window_show();
  // }

};

//----------------------------------------------------------------------
#endif
