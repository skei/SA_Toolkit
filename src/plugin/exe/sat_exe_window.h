#ifndef sat_exe_window_included
#define sat_exe_window_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "gui/sat_window.h"
#include "plugin/clap/sat_clap.h"

//----------------------------------------------------------------------
//
// exe window
//
//----------------------------------------------------------------------

class SAT_ExeWindow
: public SAT_ImplementedWindow {

//------------------------------
private:
//------------------------------

  const clap_plugin_t*      MPlugin = nullptr;
  const clap_plugin_gui_t*  MGui    = nullptr;

//------------------------------
public:
//------------------------------

  SAT_ExeWindow(uint32_t AWidth, uint32_t AHeight, intptr_t AParent, const clap_plugin_t* APlugin, const clap_plugin_gui_t* AGui)
  : SAT_ImplementedWindow(AWidth,AHeight,AParent) {
    MPlugin = APlugin;
    MGui = AGui;
    setTitle("SA_Toolkit / SAT_ExeWindow");
  }
  
//------------------------------
public:
//------------------------------

  void on_window_resize(int32_t AWidth, int32_t AHeight) final {
    MGui->set_size(MPlugin,AWidth,AHeight);
  }

};

//----------------------------------------------------------------------
#endif
