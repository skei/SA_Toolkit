
#include "base/sat.h"
#include "plugin/sat_plugin.h"
#include "audio/sat_audio_utils.h"

#define NANOSVG_IMPLEMENTATION
//#define NANOSVG_IMPLEMENTATION
#include "extern/nanosvg/nanosvgrast.h"

//----------------------------------------------------------------------
//
// widget
//
//----------------------------------------------------------------------

class myWidget
: public SAT_Widget {

//------------------------------
private:
//------------------------------

  NSVGimage*  MLogo          = nullptr;
  //SAT_Window* MOwnerWindow   = nullptr;
  bool        MFillBackground   = true;
  SAT_Color   MBackgroundColor  = SAT_Grey;

//------------------------------
public:
//------------------------------

  myWidget(SAT_Rect ARect)
  : SAT_Widget(ARect) {
    MLogo = nsvgParseFromFile("/DISKS/sda2/skei.audio/logo/export/SVG/SA_black sircle.svg", "px", 96);
    SAT_Assert(MLogo);
  }

  //----------

  virtual ~myWidget() {
    SAT_Assert(MLogo);
    nsvgDelete(MLogo);
  }

//------------------------------
public:
//------------------------------

  void setFillBackground(bool AFill=true)   { MFillBackground = AFill; }
  void setBackgroundColor(SAT_Color AColor) { MBackgroundColor = AColor; }

//------------------------------
public:
//------------------------------

  //void prepare(SAT_WidgetListener* AWindow, bool ARecursive=true) final {
  //  MOwnerWindow = (SAT_Window*)AWindow;
  //  SAT_Widget::prepare(AWindow,ARecursive);
  //}

  //----------

  void on_widget_paint(SAT_PaintContext* AContext) final {

    SAT_Assert(AContext);
    SAT_Assert(MLogo);

    SAT_Painter* painter = AContext->painter;
    SAT_Assert(painter);
    SAT_Rect mrect = getRect();

    if (MFillBackground) {
      painter->setFillColor(MBackgroundColor);
      painter->fillRect(mrect.x,mrect.y,mrect.w,mrect.h);
    }


    // border

    painter->setDrawColor(SAT_White);
    painter->setLineWidth(1);
    painter->drawRect(mrect.x,mrect.y,mrect.w,mrect.h);

    // logo

    painter->setDrawColor(SAT_White);
    painter->setLineWidth(6);

    double offset = 50.0;
    double scale  = (100.0 / MLogo->width);

    for (NSVGshape* shape = MLogo->shapes; shape != NULL; shape = shape->next) {
      for (NSVGpath* path = shape->paths; path != NULL; path = path->next) {
        for (int i = 0; i < path->npts-1; i += 3) {
          float* p = &path->pts[i*2];
          double x1 = offset + (p[0] * scale);
          double y1 = offset + (p[1] * scale);
          double x2 = offset + (p[2] * scale);
          double y2 = offset + (p[3] * scale);
          double x3 = offset + (p[4] * scale);
          double y3 = offset + (p[5] * scale);
          double x4 = offset + (p[6] * scale);
          double y4 = offset + (p[7] * scale);
          painter->drawCurveBezier(x1,y1, x4,y4, x2,y2, x3,y3);
        }
      }
    }

    // test

    int32_t font = painter->getHeaderFont();
    painter->selectFont(font);
    painter->setTextColor(SAT_BrightYellow);
    painter->setTextSize(64);
    painter->drawTextBox( mrect, "SA_Toolkit",SAT_TEXT_ALIGN_CENTER);

  }

  //----------

};

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

  myWidget* MWidget = nullptr;

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
    if (MWidget) delete MWidget;
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

  bool initEditorWindow(SAT_Window* AWindow) final {
    SAT_PRINT;
    MWidget = new myWidget( SAT_Rect(0,0,256,256) );
    MWidget->setFillBackground(true);
    MWidget->setBackgroundColor(SAT_DarkerGrey);
    AWindow->setRootWidget(MWidget);
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

