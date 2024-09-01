#ifndef sat_plugin_base_included
#define sat_plugin_base_included
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/clap/sat_clap.h"

#include "plugin/sat_audio_port.h"
#include "plugin/sat_note_port.h"
#include "plugin/sat_parameter.h"

class SAT_Timer;
class SAT_Window;
class SAT_WindowListener;

//----------------------------------------------------------------------
//
// editor
//
//----------------------------------------------------------------------

class SAT_EditorListener {
public:
  virtual void          on_EditorListener_update(uint32_t AId, double AValue) {}
  virtual void          on_EditorListener_timer(double ADelta) {}

  #ifdef SAT_EDITOR_EMBEDDED
    virtual SAT_Window* on_EditorListener_createWindow(uint32_t AWidth, uint32_t AHeight, SAT_WindowListener* AListener) { return nullptr; }
    virtual void        on_EditorListener_deleteWindow(SAT_Window* AWindow) {}
  #endif

  #ifdef SAT_EDITOR_REMOTE
  #endif

};

//----------

class SAT_BaseEditor {
public:
  SAT_BaseEditor(SAT_EditorListener* AListener, uint32_t AWidth, uint32_t AHeight, double AScale=1.0, bool AProportional=false) {}
  virtual ~SAT_BaseEditor() {}
public:
  virtual void initParameterValue(SAT_Parameter* AParameter) {}
  virtual void updateParameterFromHost(SAT_Parameter* AParameter, double AValue) {}
  virtual void updateModulationFromHost(SAT_Parameter* AParameter, double AValue) {}
public:
  virtual bool isApiSupported(const char *api, bool is_floating) { return false; }
  virtual bool getPreferredApi(const char **api, bool *is_floating) { return false; }
  virtual bool create(const char *api, bool is_floating) { return false; }
  virtual void destroy() {}
  virtual bool setScale(double scale) { return false; }
  virtual bool getSize(uint32_t *width, uint32_t *height) { return false; }
  virtual bool canResize() { return false; }
  virtual bool getResizeHints(clap_gui_resize_hints_t *hints) { return false; }
  virtual bool adjustSize(uint32_t *width, uint32_t *height) { return false; }
  virtual bool setSize(uint32_t width, uint32_t height) { return false; }
  virtual bool setParent(const clap_window_t *window) { return false; }
  virtual bool setTransient(const clap_window_t *window) { return false; }
  virtual void suggestTitle(const char *title) {}
  virtual bool show() { return false; }
  virtual bool hide() { return false; }
};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

// SAT_BasePlugin = SAT_ClapPlugin

//----------------------------------------------------------------------
//
// processor
//
//----------------------------------------------------------------------

struct SAT_ProcessContext {
  const clap_process_t* process         = nullptr;
  SAT_ParameterArray*   parameters      = nullptr;
  double                samplerate      = 0.0;
  uint32_t              minframescount  = 0;
  uint32_t              maxframescount  = 0;
  uint32_t              process_counter = 0;
  uint32_t              sample_counter  = 0;
  
};

//----------

class SAT_ProcessorListener {
public:
  virtual SAT_AudioPortArray* on_ProcessorListener_getAudioInputPorts()  { return nullptr; }
  virtual SAT_AudioPortArray* on_ProcessorListener_getAudioOutputPorts() { return nullptr; }
  virtual SAT_NotePortArray*  on_ProcessorListener_getNoteInputPorts()   { return nullptr; }
  virtual SAT_NotePortArray*  on_ProcessorListener_getNoteOutputPorts()  { return nullptr; }
  virtual SAT_ParameterArray* on_ProcessorListener_getParameters()       { return nullptr; }
  virtual void                on_ProcessorListener_updateParamFromHostToGui(uint32_t AIndex, sat_param_t AValue)  {}
  virtual void                on_ProcessorListener_updateModFromHostToGui(uint32_t AIndex, sat_param_t AValue)  {}
  virtual void                on_ProcessorListener_outputEvent()  {}
//virtual void                on_ProcessorListener_queueParamFromHostToGui(uint32_t AIndex, sat_param_t AValue)  {}
//virtual void                on_ProcessorListener_queueModFromHostToGui(uint32_t AIndex, sat_param_t AValue)    {}
//virtual void                on_ProcessorListener_queueNoteEndFromAudioToHost(SAT_Note ANote) {}
};

//----------------------------------------------------------------------
//
// voice
//
//----------------------------------------------------------------------

struct SAT_VoiceContext {
  SAT_ProcessContext* process_context   = nullptr;  // only valid during process()
  double              sample_rate       = 0.0;
  uint32_t            block_length      = 0.0;
  sat_sample_t*       voice_buffer      = nullptr;
//sat_sample_t*       block_buffer      = nullptr;
//uint32_t            min_frames_count  = 0;
//uint32_t            max_frames_count  = 0;
//SAT_Plugin*         plugin            = nullptr;
};

//----------

// class SAT_VoiceListener {
// public:
// };

//----------------------------------------------------------------------
#endif

