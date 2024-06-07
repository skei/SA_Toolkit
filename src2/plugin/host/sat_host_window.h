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
//: public SAT_BasicWindow {
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
  //: SAT_BasicWindow(AWidth,AHeight,AParent) {
  : SAT_ImplementedWindow(AWidth,AHeight,AParent) {
    MClapPlugin = AClapPlugin;
    MClapGui = (const clap_plugin_gui_t*)MClapPlugin->get_extension(MClapPlugin,CLAP_EXT_GUI);;
  }

  //----------

  virtual ~SAT_HostWindow() {
  }

  // make it shut up about unimplemented virtual abstract methods..
  #ifdef SAT_PAINTER_NANOVG
    NVGcontext* on_surfaceOwner_getNanoVGContext()  override { return nullptr; }
  #endif


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

  //----------

  /*
  void on_timerListener_callback(SAT_Timer* ATimer, double ADelta) override {
    // SAT_TRACE;
    // uint32_t time = 0;
    // uint32_t block = 256;
    // MClapProcess.steady_time         = time;
    // MClapProcess.frames_count        = block;
    // MClapProcess.transport           = nullptr;
    // MClapProcess.audio_inputs        = nullptr;
    // MClapProcess.audio_outputs       = nullptr;
    // MClapProcess.audio_inputs_count  = 0;
    // MClapProcess.audio_outputs_count = 0;
    // MClapProcess.in_events           = nullptr;
    // MClapProcess.out_events          = nullptr;
    // MClapPlugin->process(MClapPlugin,&MClapProcess);
    // time += block;
  }
  */

};

//----------------------------------------------------------------------
#endif
