
#include "base/sat.h"
#include "gui/sat_widgets.h"
#include "plugin/sat_plugin.h"
#include "audio/sat_audio_utils.h"

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

#define EDITOR_WIDTH  (50 + 200 + 50)
#define EDITOR_HEIGHT (50 + 200 + (4 * (10 + 20)) + 50)

//----------

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
    SAT_Parameter* par1 = appendParameter( new SAT_Parameter("Param1",0.0) );
    SAT_Parameter* par2 = appendParameter( new SAT_Parameter("Param2",0.5) );
    SAT_Parameter* par3 = appendParameter( new SAT_Parameter("Param3",1.5) );
    par3->setFlag(CLAP_PARAM_IS_MODULATABLE);
    setInitialEditorSize(EDITOR_WIDTH,EDITOR_HEIGHT);
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
    //SAT_PRINT;
    SAT_Editor* editor = new SAT_Editor(AListener,AWidth,AHeight);
    return editor;
  }

  //----------

  bool initEditorWindow(SAT_Editor* AEditor, SAT_Window* AWindow) final {
    //SAT_PRINT;

    //MWidget = new myWidget( SAT_Rect(0,0,256,256) );

    MRootPanel = new SAT_PanelWidget( SAT_Rect(0,0,EDITOR_WIDTH,EDITOR_HEIGHT) );
    AWindow->setRootWidget(MRootPanel);
    MRootPanel->setFillBackground(true);

    SAT_LogoWidget* logo = new SAT_LogoWidget(SAT_Rect(50,50,200,200));
    MRootPanel->appendChildWidget(logo);
    logo->setLogoColor(SAT_White);

    SAT_TextWidget* text = new SAT_TextWidget(SAT_Rect(50,270,200,20),"Hello world!");
    MRootPanel->appendChildWidget(text);
    text->setTextSize(12);

    SAT_ValueWidget* value = new SAT_ValueWidget(SAT_Rect(50,300,200,20),"Param 1", 0.0);
    MRootPanel->appendChildWidget(value);
    value->setTextSize(12);

    SAT_DragValueWidget* dragvalue = new SAT_DragValueWidget(SAT_Rect(50,330,200,20),"Param 2", 0.0);
    MRootPanel->appendChildWidget(dragvalue);
    dragvalue->setTextSize(12);
    dragvalue->setSnap(true);
    dragvalue->setSnapPos(0.75);
    dragvalue->setAutoHideCursor(false);
    dragvalue->setAutoLockCursor(true);

    SAT_SliderWidget* slider = new SAT_SliderWidget(SAT_Rect(50,360,200,20),"Param 3", 0.0);
    MRootPanel->appendChildWidget(slider);
    slider->setTextSize(12);
    slider->setBipolar(true);
    slider->setBipolarCenter(0.3);
    slider->setSnap(true);
    slider->setSnapPos(0.3);
    slider->setAutoHideCursor(true);
    slider->setAutoLockCursor(true);
    slider->setModulation(0.25);
    slider->setDrawModulation(true);
    slider->setModulationColor( SAT_Color(1,1,1,0.25) );

    AEditor->connect(value,     getParameter(0));
    AEditor->connect(dragvalue, getParameter(1));
    AEditor->connect(slider,    getParameter(2));

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
    sat_param_t scale = getParameterValue(2);
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

