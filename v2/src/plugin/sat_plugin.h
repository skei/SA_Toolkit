#ifndef sat_plugin_included
#define sat_plugin_included
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/clap/sat_clap.h"
#include "plugin/clap/sat_clap_plugin.h"
#include "plugin/sat_audio_port.h"
#include "plugin/sat_note_port.h"
#include "plugin/sat_audio_processor.h"
// #include "plugin/sat_event_processor.h"
#include "plugin/sat_parameter_manager.h"
#include "plugin/sat_editor.h"
#include "plugin/sat_editor_listener.h"

//class SAT_ClapPlugin {};
// typedef SAT_Queue<uint32_t,SAT_PLUGIN_MAX_PARAM_EVENTS_PER_BLOCK> MHostParamQueue;
// typedef SAT_Queue<uint32_t,SAT_PLUGIN_MAX_GUI_EVENTS_PER_BLOCK>   MGuiParamQueue;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Plugin
: public SAT_ClapPlugin
, public SAT_EditorListener {

//------------------------------
private:
//------------------------------

  // MGuiParamQueue  MParamFromGuiToPluginQueue  = {};
  // MGuiParamQueue  MParamFromGuiToHostQueue    = {};
  // MHostParamQueue MParamFromHostToGui = {};
  // MHostParamQueue MModFromHostToGui = {};

//------------------------------
private:
//------------------------------

  const clap_plugin_descriptor_t* MDescriptor     = nullptr;
  SAT_AudioPorts                  MAudioPorts     = {};
  SAT_NotePorts                   MNotePorts      = {};
  SAT_ParameterManager            MParameters     = {};
  SAT_AudioProcessor              MAudioProcessor = {};
  SAT_Editor*                     MEditor         = nullptr;

//------------------------------
public:
//------------------------------

  SAT_Plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : SAT_ClapPlugin(ADescriptor,AHost) {
  }

  //----------

  virtual ~SAT_Plugin() {
  }

//------------------------------
protected:
//------------------------------

  bool init() override {
    return true;
  }

  void destroy() override {
  }

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) override {
    return true;
  }

  void deactivate() override {
  }

  bool start_processing() override {
    return true;
  }

  void stop_processing() override {
  }

  void reset() override {
  }

  clap_process_status process(const clap_process_t *process) override {
    return CLAP_PROCESS_CONTINUE;
  }

  const void* get_extension(const char *id) override {
    return nullptr;
  }

  void on_main_thread() override {
  }

//------------------------------
protected:
//------------------------------

  // void process(clap_process_t* process) {}

//------------------------------
protected:
//------------------------------

  // void queueParamFromGuiToPlugin() {}
  // void flushParamFromGuiToPlugin() {}

  // void queueParamFromGuiToHost() {}
  // void flushParamFromGuiToHost() {}

  // void queueParamFromHostToGui() {}
  // void flushParamFromHostToGui() {}

  // void queueModFromHostToGui() {}
  // void flushModFromHostToGui() {}

};

//----------------------------------------------------------------------
#endif

