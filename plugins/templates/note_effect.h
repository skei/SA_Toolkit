
#include "sat.h"
#include "audio/sat_audio_utils.h"
#include "plugin/sat_entry.h"
#include "plugin/sat_parameters.h"
#include "plugin/sat_plugin.h"
#include "gui/sat_widgets.h"

//----------------------------------------------------------------------

#define PLUGIN_NAME "note_effect"
#define PLUGIN_AUTHOR "me"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t my_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = PLUGIN_AUTHOR "/" PLUGIN_NAME,
  .name         = PLUGIN_NAME,
  .vendor       = PLUGIN_AUTHOR,
  .url          = "",
  .manual_url   = "",
  .support_url  = "",
  .version      = "0.0.1",
  .description  = "",
  .features     = (const char*[]){ CLAP_PLUGIN_FEATURE_NOTE_EFFECT, nullptr }
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class note_effect
: public SAT_Plugin {
  
//------------------------------
private:
//------------------------------

//------------------------------
public:
//------------------------------

  SAT_DEFAULT_PLUGIN_CONSTRUCTOR(note_effect)
 
  //----------

  bool init() final {
    registerDefaultExtensions();    
    appendClapNoteInputPort("In");
    appendClapNoteOutputPort("Out");
    setAllParameterFlags(CLAP_PARAM_IS_MODULATABLE);
    return SAT_Plugin::init();
  }
  
  //----------

  bool on_plugin_noteOn(const clap_event_note_t* event) final {
    return true;
  }

  //----------

  bool on_plugin_noteOff(const clap_event_note_t* event) final {
    return true;
  }

  //----------

  bool on_plugin_noteChoke(const clap_event_note_t* event) final {
    return true;
  }

  //----------

  bool on_plugin_noteExpression(const clap_event_note_expression_t* event) final {
    return true;
  }

  //----------

  bool on_plugin_paramValue(const clap_event_param_value_t* event) final {
    return true;
  }

  //----------

  bool on_plugin_paramMod(const clap_event_param_mod_t* event) final {
    return true;
  }

  //----------

  bool on_plugin_transport(const clap_event_transport_t* event) final {
    return true;
  }
  
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#ifndef SAT_NO_ENTRY
  #include "plugin/sat_entry.h"
  SAT_PLUGIN_ENTRY(my_descriptor,note_effect);
#endif

//----------

#undef PLUGIN_NAME
#undef PLUGIN_AUTHOR
//#undef EDITOR_WIDTH
//#undef EDITOR_HEIGHT

