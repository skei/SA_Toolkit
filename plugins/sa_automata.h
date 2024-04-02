#ifndef sa_automata_included
#define sa_automata_included
//----------------------------------------------------------------------

#include "plugin/sat_parameters.h"
#include "plugin/sat_plugin.h"

#if !defined (SAT_GUI_NOGUI)
  //#include "gui/sat_widgets.h"
  #include "sa_automata/sa_automata_widgets.h"
#endif
  
//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t sa_automata_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = "sa_automata",
  .name         = "sa_automata",
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
// plugin
//
//----------------------------------------------------------------------

class sa_automata_plugin
: public SAT_Plugin {

  friend class sa_automata_root_widget;
  
//------------------------------
private:
//------------------------------

  bool      MIsPlaying        = false;
  double    MStartTime        = 0.0;
  double    MTempo            = 0.0;
//double    MTempoInc         = 0.0;
  double    MSampleRate       = 0.0;

  double    MSpeed            = 1.0;
  double    MTuning           = 0.0;

  double    MTickSize         = 0.0;
  double    MTickRemaining    = 0.0;

  uint8_t   MStates[256*256]  = {0};

  uint32_t              p_width         = 0.0;
  uint32_t              p_height        = 0.0;
  int32_t               p_tuning        = 0.0;
  double                p_velocity      = 0.0;
  uint32_t              p_cc1           = 0.0;
  uint32_t              p_cc2           = 0.0;
  uint32_t              p_cc3           = 0.0;

  sa_automata_grid*     w_grid        = nullptr;
  SAT_DragValueWidget*  w_grid_width  = nullptr;
  SAT_DragValueWidget*  w_grid_height = nullptr;
  SAT_DragValueWidget*  w_tuning      = nullptr;
  SAT_DragValueWidget*  w_velocity    = nullptr;
  SAT_DragValueWidget*  w_cc1         = nullptr;
  SAT_DragValueWidget*  w_cc2         = nullptr;
  SAT_DragValueWidget*  w_cc3         = nullptr;



//------------------------------
public:
//------------------------------

  SAT_DEFAULT_PLUGIN_CONSTRUCTOR(sa_automata_plugin)
 
//------------------------------
public:
//------------------------------

  bool send_event(void* event) {
    SAT_ProcessContext* context = getProcessContext();
    const clap_output_events_t *out_events = context->process->out_events;
    out_events->try_push(out_events,(const clap_event_header_t*)event);
    return true;
  }

  //----------

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

  //----------

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
      setInitialEditorSize( 420, 560, 1.0 );
    #endif
    return SAT_Plugin::init();
  }
  
  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    MSampleRate = sample_rate;
    return SAT_Plugin::activate(sample_rate,min_frames_count,max_frames_count);
  }

//------------------------------
public:
//------------------------------

  #if !defined (SAT_GUI_NOGUI)
    void setupEditorWindow(SAT_Editor* AEditor, SAT_Window* AWindow) final {
      sa_automata_root_widget* root = new sa_automata_root_widget(AWindow);
      AWindow->setRootWidget(root);
      w_grid        = new sa_automata_grid(    SAT_Rect( 10, 50, 400,400), 16, 16, MStates);
      w_grid_width  = new SAT_DragValueWidget( SAT_Rect( 10,480, 80, 20),"Width");
      w_grid_height = new SAT_DragValueWidget( SAT_Rect( 10,505, 80, 20),"Height");
      w_tuning      = new SAT_DragValueWidget( SAT_Rect(100,480, 80, 20),"Tuning");
      w_velocity    = new SAT_DragValueWidget( SAT_Rect(100,505, 80, 20),"Velocity");
      w_cc1         = new SAT_DragValueWidget( SAT_Rect(190,480, 80, 20),"CC1");
      w_cc2         = new SAT_DragValueWidget( SAT_Rect(190,505, 80, 20),"CC2");
      w_cc3         = new SAT_DragValueWidget( SAT_Rect(190,530, 80, 20),"CC3");
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

  bool on_plugin_paramValue(const clap_event_param_value_t* event) final {
    SAT_Print("index %i value %.3f\n",event->param_id,event->value);
    uint32_t index = event->param_id;
    double value = event->value;
    switch (index) {
      case 0: p_width     = value;  break;
      case 1: p_height    = value;  break;
      case 2: p_tuning    = value;  break;
      case 3: p_velocity  = value;  break;
      case 4: p_cc1       = value;  break;
      case 5: p_cc2       = value;  break;
      case 6: p_cc3       = value;  break;
    }
    return true;
  }

  //----------

  // bool on_plugin_paramMod(const clap_event_param_mod_t* event) final {
  //   return true;
  // }

  //----------

  bool on_plugin_transport(const clap_event_transport_t* event) final {
    SAT_ProcessContext* context = getProcessContext();
    SAT_Assert(context);
    double samplerate = context->samplerate;
    SAT_Assert(samplerate > 0);
    uint64_t samplecount = context->sample_counter;
    if (event->flags & CLAP_TRANSPORT_HAS_TEMPO) {
      MTempo = event->tempo;
      //MTempoInc = event->tempo_inc;
    }
    bool playing = event->flags & CLAP_TRANSPORT_IS_PLAYING;
    if (!MIsPlaying && playing) {
      MIsPlaying = true;
      handleStartPlaying();
    }
    else if (MIsPlaying && !playing) {
      MIsPlaying = false;
      handleStopPlaying();
    }
    calcTickSize();
    return true;
  }

  //----------

  void processStereoSample(sat_sample_t* spl0, sat_sample_t* spl1) final {
    if (MIsPlaying) {
      MTickRemaining -= 1.0;
      if (MTickRemaining <= 0.0) {
        MTickRemaining += MTickSize; // = MTickSize - MTickRemaining;
        handleTick();
      }
    }
    //MTempo += MTempoInc;
  }
  
//------------------------------
public:
//------------------------------

  void calcTickSize() {
    SAT_Assert(MTempo > 0.0);
    double beats_per_second = MTempo / 60.0;
    double seconds_per_beat = 60.0 / MTempo;
    double samples_per_beat = MSampleRate * seconds_per_beat;
    MTickSize = samples_per_beat;
    //SAT_Print("MTickSize %.3f\n",MTickSize);
  }

  //----------

  void handleStartPlaying() {
    SAT_PRINT;
    MTickRemaining = 0.0;
    //MTickRemaining = MTickSize;
  }

  //----------

  void handleStopPlaying() {
    SAT_PRINT;
  }

  //----------

  // we got a crash when closing editor..

  void handleTick() {

    SAT_PRINT;

    //for (uint32_t y=0; y<p_height; y++) {
    //  for (uint32_t x=0; x<p_width; x++) {
    //    uint32_t cell = w_grid->getCellState(x,y);
    //  }
    //}

    uint32_t x = SAT_RandomRangeInt(0,p_width-1);
    uint32_t y = SAT_RandomRangeInt(0,p_height-1);
    uint32_t v = SAT_RandomRangeInt(0,15);
    MStates[(y*256)+x] = v;

    // crashes when playing & closing editor..
    if (isEditorOpen()) {
      // w_grid->do_widget_update(w_grid,0,0);
      w_grid->do_widget_redraw(w_grid,0,0);
    }

  }

};

//----------------------------------------------------------------------
//
// entry
//
//----------------------------------------------------------------------

#ifndef SAT_NO_ENTRY
  #include "plugin/sat_entry.h"
  SAT_PLUGIN_ENTRY(sa_automata_descriptor,sa_automata_plugin);
#endif

//----------------------------------------------------------------------
#endif