
#include "base/sat.h"
#include "gui/sat_widgets.h"
#include "plugin/sat_plugin.h"
#include "audio/sat_audio_utils.h"

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

  SAT_PanelWidget*  MRootPanel  = nullptr;
  SAT_LogoWidget*   MLogo       = nullptr;
  SAT_TextWidget*   MText       = nullptr;
  SAT_ValueWidget*  MValue1     = nullptr;
  SAT_DragValueWidget*  MValue2     = nullptr;

//------------------------------
public:
//------------------------------

  SAT_PLUGIN_DEFAULT_CONSTRUCTOR(myPlugin);

  //----------

  bool init() final {
    registerDefaultExtensions();
    appendClapNoteInputPort();
    appendStereoInputPort();
    appendStereoOutputPort();
    appendParameter( new SAT_Parameter("Param1",0.0) );
    appendParameter( new SAT_Parameter("Param2",0.5) );
    appendParameter( new SAT_Parameter("Param3",1.5) );
    return SAT_Plugin::init();
  }

  //----------

  void destroy() final {
    // root widget is not automatically deleted
    if (MRootPanel) delete MRootPanel;
    SAT_Plugin::destroy();
  }

//------------------------------
public:
//------------------------------

  SAT_Editor* createEditor(SAT_EditorListener* AListener, uint32_t AWidth, uint32_t AHeight) final {
    SAT_PRINT;
    SAT_Editor* editor = new SAT_Editor(AListener,AWidth,AHeight);
    return editor;
  }

  //----------

  bool initEditorWindow(SAT_Editor* AEditor, SAT_Window* AWindow) final {
    SAT_PRINT;

    //MWidget = new myWidget( SAT_Rect(0,0,256,256) );

    MRootPanel = new SAT_PanelWidget( SAT_Rect(0,0,256,256) );
    AWindow->setRootWidget(MRootPanel);
    MRootPanel->setFillBackground(true);

    MLogo = new SAT_LogoWidget(SAT_Rect(50,50,200,200));
    MRootPanel->appendChildWidget(MLogo);
    MLogo->setLogoColor(SAT_White);

    MText = new SAT_TextWidget(SAT_Rect(50,270,200,20),"Hello world!");
    MRootPanel->appendChildWidget(MText);
    MText->setTextSize(12);

    MValue1 = new SAT_ValueWidget(SAT_Rect(50,300,200,20),"Param 1", 0.0);
    MRootPanel->appendChildWidget(MValue1);
    MValue1->setTextSize(12);

    MValue2 = new SAT_DragValueWidget(SAT_Rect(50,330,200,20),"Param 2", 0.0);
    MRootPanel->appendChildWidget(MValue2);
    MValue2->setTextSize(12);

    AEditor->connect(MValue1, getParameter(0));
    AEditor->connect(MValue2, getParameter(1));

    return true;
  }

//------------------------------
public:
//------------------------------

  bool handleNoteOn(const clap_event_note_t* event) final {
    //SAT_Print("\n");
    return false;
  }

  bool handleNoteOff(const clap_event_note_t* event) final {
    //SAT_Print("\n");
    return false;
  }

  bool handleNoteChoke(const clap_event_note_t* event) final {
    //SAT_Print("\n");
    return false;
  }

  bool handleNoteExpression(const clap_event_note_expression_t* event) final {
    //SAT_Print("\n");
    return false;
  }

  bool handleParamValue(const clap_event_param_value_t* event) final {
    //SAT_Print("\n");
    return false;
  }

  bool handleParamMod(const clap_event_param_mod_t* event) final {
    //SAT_Print("\n");
    return false;
  }

  bool handleTransport(const clap_event_transport_t* event) final {
    //SAT_Print("\n");
    return false;
  }

  bool handleMidi(const clap_event_midi_t* event) final {
    //SAT_Print("\n");
    return false;
  }

  bool handleMidiSysex(const clap_event_midi_sysex_t* event) final {
    //SAT_Print("\n");
    return false;
  }

  bool handleMidi2(const clap_event_midi2_t* event) final {
    //SAT_Print("\n");
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

