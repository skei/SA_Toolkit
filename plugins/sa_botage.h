#ifndef sa_botage_included
#define sa_botage_included
//----------------------------------------------------------------------


#define SA_BOTAGE_NAME          "sa_botage"
#define SA_BOTAGE_EDITOR_WIDTH  490 + 132
#define SA_BOTAGE_EDITOR_HEIGHT 392

//----------------------------------------------------------------------

#include "plugin/sat_plugin.h"
#include "audio/sat_audio_utils.h"

#include "sa_botage/sa_botage_parameters.h"
#include "sa_botage/sa_botage_processor.h"
#include "sa_botage/sa_botage_editor.h"

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t sa_botage_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = SAT_VENDOR "/" SA_BOTAGE_NAME,
  .name         = SA_BOTAGE_NAME,
  .vendor       = SAT_VENDOR,
  .url          = SAT_URL,
  .manual_url   = "",
  .support_url  = "",
  .version      = SAT_VERSION,
  .description  = "sabotages your audio",
  .features     = (const char*[]){ CLAP_PLUGIN_FEATURE_AUDIO_EFFECT, nullptr }
};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class sa_botage_plugin
: public SAT_Plugin {

//------------------------------
private:
//------------------------------

  sa_botage_processor MProcessor;

//------------------------------
public:
//------------------------------

  sa_botage_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : SAT_Plugin(ADescriptor,AHost) {
    setInitialEditorSize(SA_BOTAGE_EDITOR_WIDTH,SA_BOTAGE_EDITOR_HEIGHT);
    //SAT_PRINT;
  }

//------------------------------
public:
//------------------------------

  bool init() final {
    bool result = SAT_Plugin::init();
    appendStereoInput();
    appendStereoOutput();
    sa_botage_init_parameters(this);
    return result;
  }

  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    SAT_Plugin::activate(sample_rate,min_frames_count,max_frames_count);
    MProcessor.activate(sample_rate);
    return true;
  }

  //----------

  bool gui_create(const char* api, bool is_floating) final {
    setEditorIsOpen(false);
    uint32_t width = MInitialEditorWidth;
    uint32_t height = MInitialEditorHeight;
    const clap_plugin_descriptor_t* descriptor = getClapDescriptor();
    SAT_Editor* editor = new sa_botage_editor(this,width,height,&MParameters,descriptor);
    setEditor(editor);
    return (editor);
  }

  //----------

  void processParamValue(const clap_event_param_value_t* event) final {
    MProcessor.setParamValue(event->param_id,event->value);
  }

  //----------

  void processTransport(const clap_event_transport_t* transport) final {
    SAT_Plugin::processTransport(transport);
    MProcessor.transport(transport->flags);
  }

  //----------

  void processAudioBlock(SAT_ProcessContext* AContext) final {
    MProcessor.processAudioBlock(AContext);
  }

//------------------------------
public:
//------------------------------

  void on_editor_timer() override {
    sa_botage_editor* editor = (sa_botage_editor*)getEditor();
    if (editor && editor->isEditorOpen()) {
      editor->updateWaveformWidget(&MProcessor);
      editor->updateProbIndicators(&MProcessor);
    }
    SAT_Plugin::on_editor_timer();
  }

};

//----------------------------------------------------------------------
//
// entry
//
//----------------------------------------------------------------------

#ifndef SAT_NO_ENTRY

  #include "plugin/sat_entry.h"
  SAT_DEFAULT_ENTRY(sa_botage_descriptor,sa_botage_plugin);

#endif


//----------------------------------------------------------------------
#endif