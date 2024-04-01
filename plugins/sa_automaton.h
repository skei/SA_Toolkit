#ifndef sa_automaton_included
#define sa_automaton_included
//----------------------------------------------------------------------

#include "plugin/sat_parameters.h"
#include "plugin/sat_plugin.h"

#if !defined (SAT_GUI_NOGUI)
  #include "gui/sat_widgets.h"
#endif
  
//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t sa_automaton_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = "sa_automaton",
  .name         = "sa_automaton",
  .vendor       = "skei.audio",
  .url          = "https://github.com/skei/SA_Toolkit",
  .manual_url   = "",
  .support_url  = "",
  .version      = "0.0.1",
  .description  = "game of life",
  .features     = (const char*[]){ CLAP_PLUGIN_FEATURE_NOTE_EFFECT, nullptr }
};

//----------------------------------------------------------------------
//
// root widget
//
//----------------------------------------------------------------------

class sa_automaton_root_widget
: public SAT_RootWidget {

  friend class sa_automaton_plugin;
  
private:

  SAT_DragValueWidget*  WGridWidth  = nullptr;
  SAT_DragValueWidget*  WGridHeight = nullptr;
  SAT_GridWidget*       WGrid       = nullptr;

public:

  sa_automaton_root_widget(SAT_WidgetListener* AListener=nullptr)
  : SAT_RootWidget(AListener) {
  }

  virtual ~sa_automaton_root_widget() {
  }

public:

  void do_widget_update(SAT_Widget* AWidget, uint32_t AIndex, uint32_t AMode) override {
    double value = 0.0;
    SAT_Parameter* param = (SAT_Parameter*)AWidget->getParameter();
    if (AWidget == WGridWidth) {
      double value = WGridWidth->getValue();
      if (param) value = param->denormalize(value);
      WGrid->setNumColumns(value);
      WGrid->do_widget_update(WGrid,0,0);
      WGrid->do_widget_redraw(WGrid,0,0);
    }
    else if (AWidget == WGridHeight) {
      double value = WGridHeight->getValue();
      if (param) value = param->denormalize(value);
      WGrid->setNumRows(value);
      WGrid->do_widget_update(WGrid,0,0);
      WGrid->do_widget_redraw(WGrid,0,0);
    }
    SAT_RootWidget::do_widget_update(AWidget,AIndex,AMode);
  }

};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class sa_automaton_plugin
: public SAT_Plugin {

  friend class sa_automaton_root_widget;
  
//------------------------------
private:
//------------------------------

  bool    MIsPlaying = false;
  double  MStartTime = 0.0;
  double  MTempo     = 0.0;
  double  MTempoInc  = 0.0;

//------------------------------
public:
//------------------------------

  SAT_DEFAULT_PLUGIN_CONSTRUCTOR(sa_automaton_plugin)
 
//------------------------------
public:
//------------------------------

  bool send_event(void* event) {
    SAT_ProcessContext* context = getProcessContext();
    const clap_output_events_t *out_events = context->process->out_events;
    out_events->try_push(out_events,(const clap_event_header_t*)event);
    return true;
  }

  void send_note() {
    //clap_event_note_t note_event;
    //note_event.header.size     = sizeof(clap_event_note_t);
    //note_event.header.time     = event.header.time;
    //note_event.header.space_id = CLAP_CORE_EVENT_SPACE_ID;
    //note_event.header.type     = CLAP_EVENT_NOTE_ON;
    //note_event.header.flags    = 0; // CLAP_EVENT_IS_LIVE, CLAP_EVENT_DONT_RECORD
    //note_event.note_id         = (chan * 128) + index;
    //note_event.port_index      = event->port_index;
    //note_event.channel         = chan;
    //note_event.key             = index;
    //note_event.velocity        = value * (double)SAT_INV127;
    //const clap_event_header_t* header = (const clap_event_header_t*)&note_event;
  }

  void send_cc() {
  }

//------------------------------
public:
//------------------------------

  bool init() final {

    registerDefaultExtensions();    
    appendNoteOutputPort("Out", CLAP_NOTE_DIALECT_CLAP | CLAP_NOTE_DIALECT_MIDI, CLAP_NOTE_DIALECT_CLAP);

    appendParameter( new SAT_IntParameter( "Width",    16,    4,   32,    CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED ));
    appendParameter( new SAT_IntParameter( "Height",   16,    4,   32,    CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED ));
    appendParameter( new SAT_IntParameter( "Tuning",   0,    -48,  48,    CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED ));
    appendParameter( new SAT_Parameter(    "Velocity", 0.0,   0.0, 1.0,   CLAP_PARAM_IS_AUTOMATABLE ));
    appendParameter( new SAT_IntParameter( "CC1",      64,    0,   127,   CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED ));
    appendParameter( new SAT_IntParameter( "CC2",      65,    0,   127,   CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED ));
    appendParameter( new SAT_IntParameter( "CC3",      66,    0,   127,   CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED ));

    #if !defined (SAT_GUI_NOGUI)
      setInitialEditorSize( 500, 560, 1.0 );
    #endif
    return SAT_Plugin::init();
  }
  
  //----------

  // bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
  //   return SAT_Plugin::activate(sample_rate,min_frames_count,max_frames_count);
  // }

  //----------

  #if !defined (SAT_GUI_NOGUI)
  
  void setupEditorWindow(SAT_Editor* AEditor, SAT_Window* AWindow) final {

    sa_automaton_root_widget* root = new sa_automaton_root_widget(AWindow);
    AWindow->setRootWidget(root);

    SAT_GridWidget*       w_grid        = new SAT_GridWidget(      SAT_Rect( 10, 50,400,400), 16, 16);

    SAT_DragValueWidget*  w_grid_width  = new SAT_DragValueWidget( SAT_Rect( 10,480, 80, 20),"Width");
    SAT_DragValueWidget*  w_grid_height = new SAT_DragValueWidget( SAT_Rect( 10,505, 80, 20),"Height");

    SAT_DragValueWidget*  w_tuning      = new SAT_DragValueWidget( SAT_Rect(100,480, 80, 20),"Tuning");
    SAT_DragValueWidget*  w_velocity    = new SAT_DragValueWidget( SAT_Rect(100,505, 80, 20),"Velocity");

    SAT_DragValueWidget*  w_cc1         = new SAT_DragValueWidget( SAT_Rect(190,480, 80, 20),"CC1");
    SAT_DragValueWidget*  w_cc2         = new SAT_DragValueWidget( SAT_Rect(190,505, 80, 20),"CC2");
    SAT_DragValueWidget*  w_cc3         = new SAT_DragValueWidget( SAT_Rect(190,530, 80, 20),"CC3");

    root->WGridWidth  = w_grid_width;
    root->WGridHeight = w_grid_height;
    root->WGrid       = w_grid;

    root->appendChildWidget(w_grid_width);
    root->appendChildWidget(w_grid_height);
    root->appendChildWidget(w_grid);
    root->appendChildWidget(w_tuning);
    root->appendChildWidget(w_velocity);
    root->appendChildWidget(w_cc1);
    root->appendChildWidget(w_cc2);
    root->appendChildWidget(w_cc3);

    AEditor->connect(w_grid_width,  getParameter(0));
    AEditor->connect(w_grid_height, getParameter(1));
    AEditor->connect(w_tuning,      getParameter(2));
    AEditor->connect(w_velocity,    getParameter(3));
    AEditor->connect(w_cc1,         getParameter(4));
    AEditor->connect(w_cc2,         getParameter(5));
    AEditor->connect(w_cc3,         getParameter(6));

  }

  #endif

//------------------------------
public:
//------------------------------

  //----------

  // bool on_plugin_paramValue(const clap_event_param_value_t* event) final {
  //   return true;
  // }

  //----------

  // bool on_plugin_paramMod(const clap_event_param_mod_t* event) final {
  //   return true;
  // }

  //----------

  // clap_process_status process(const clap_process_t *process) final {
  //   return SAT_Plugin::process(process);
  // }

  //----------

  bool on_plugin_transport(const clap_event_transport_t* event) final {

    SAT_ProcessContext* context = getProcessContext();
    SAT_Assert( context );

    double samplerate = context->samplerate;
    SAT_Assert( samplerate > 0 );

    uint64_t samplecount = context->sample_counter;

    if (event->flags & CLAP_TRANSPORT_HAS_TEMPO) {
      MTempo = event->tempo;
      MTempoInc = event->tempo_inc;
    }

    bool playing = event->flags & CLAP_TRANSPORT_IS_PLAYING;
    if (!MIsPlaying && playing) MIsPlaying = true;
    else if (MIsPlaying && !playing) MIsPlaying = false;

    //SAT_Print("samplerate %.2f samplecount %i tempo %.2f tempo_inc %.2f\n",samplerate,samplecount,tempo,tempo_inc);

    return true;
  }

  //----------

  virtual void processNoAudioSample() {
  }

  
};

//----------------------------------------------------------------------
//
// entry
//
//----------------------------------------------------------------------

#ifndef SAT_NO_ENTRY
  #include "plugin/sat_entry.h"
  SAT_PLUGIN_ENTRY(sa_automaton_descriptor,sa_automaton_plugin);
#endif

//----------------------------------------------------------------------
#endif