
#include "base/sat.h"
#include "plugin/sat_plugin.h"
#include "audio/sat_audio_utils.h"

#define NANOSVG_IMPLEMENTATION
//#define NANOSVG_IMPLEMENTATION
#include "extern/nanosvg/nanosvgrast.h"

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t myDescriptor = {
  .clap_version = CLAP_VERSION,
  .id           = "me/myPlugin/0",
  .name         = "myPlugin",
  .vendor       = "me",
  .url          = "",
  .manual_url   = "",
  .support_url  = "",
  .version      = "0",
  .description  = "...",
  .features     = (const char*[]){ CLAP_PLUGIN_FEATURE_AUDIO_EFFECT, nullptr }
};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class myPlugin
: public SAT_Plugin {

//------------------------------
private:
//------------------------------

  NSVGimage* image = nullptr;;

//------------------------------
public:
//------------------------------

  myPlugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : SAT_Plugin(ADescriptor,AHost) {
    SAT_Print("Hello world!\n");

    //registerAllExtensions();
    registerDefaultExtensions();

    appendClapNoteInputPort();
    appendStereoInputPort();
    appendStereoOutputPort();
    appendParameter( new SAT_Parameter("Param1",0.0) );
    appendParameter( new SAT_Parameter("Param2",0.5) );
    appendParameter( new SAT_Parameter("Param3",1.5) );
  }

  //----------

  virtual ~myPlugin() {
  }

//------------------------------
public:
//------------------------------

  bool init() final {
    bool result = SAT_Plugin::init();
    if (result) {

      //struct NSVGimage* image;
      image = nsvgParseFromFile("/DISKS/sda2/skei.audio/logo/export/SVG/SA_black sircle.svg", "px", 96);
      SAT_Print("size: %f x %f\n", image->width, image->height);

      //// Use...
      //for (NSVGshape* shape = image->shapes; shape != NULL; shape = shape->next) {
      //  for (NSVGpath* path = shape->paths; path != NULL; path = path->next) {
      //    for (int i = 0; i < path->npts-1; i += 3) {
      //      float* p = &path->pts[i*2];
      //      drawCubicBez(p[0],p[1], p[2],p[3], p[4],p[5], p[6],p[7]);
      //    }
      //  }
      //}

    }
    return result;
  }

  //----------

  void destroy() final {
    nsvgDelete(image);
    SAT_Plugin::destroy();
  }

//------------------------------
public:
//------------------------------

  SAT_Editor* createEditor(SAT_EditorListener* AListener, uint32_t AWidth, uint32_t AHeight) final {
    return new SAT_Editor(AListener,AWidth,AHeight);
  }

//------------------------------
public:
//------------------------------

  bool handleNoteOn(const clap_event_note_t* event) final {
    SAT_Print("\n");
    return false;
  }

  bool handleNoteOff(const clap_event_note_t* event) final {
    SAT_Print("\n");
    return false;
  }

  bool handleNoteChoke(const clap_event_note_t* event) final {
    SAT_Print("\n");
    return false;
  }

  bool handleNoteExpression(const clap_event_note_expression_t* event) final {
    SAT_Print("\n");
    return false;
  }

  bool handleParamValue(const clap_event_param_value_t* event) final {
    SAT_Print("\n");
    return false;
  }

  bool handleParamMod(const clap_event_param_mod_t* event) final {
    SAT_Print("\n");
    return false;
  }

  bool handleTransport(const clap_event_transport_t* event) final {
    //SAT_Print("\n");
    return false;
  }

  bool handleMidi(const clap_event_midi_t* event) final {
    SAT_Print("\n");
    return false;
  }

  bool handleMidiSysex(const clap_event_midi_sysex_t* event) final {
    SAT_Print("\n");
    return false;
  }

  bool handleMidi2(const clap_event_midi2_t* event) final {
    SAT_Print("\n");
    return false;
  }

//------------------------------
public:
//------------------------------

  void processAudio(SAT_ProcessContext* AContext) final {
    const clap_process_t* process = AContext->process;
    uint32_t length = process->frames_count;
    float** inputs = process->audio_inputs[0].data32;
    float** outputs = process->audio_outputs[0].data32;
    SAT_CopyStereoBuffer(outputs,inputs,length);
    sat_param_t scale = getParameterValue(0);
    SAT_ScaleStereoBuffer(outputs,scale,length);
  }

};

//----------------------------------------------------------------------
//
// register
//
//----------------------------------------------------------------------

#include "plugin/sat_entry.h"
SAT_PLUGIN_ENTRY(myDescriptor,myPlugin);

