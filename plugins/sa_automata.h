#ifndef sa_automata_included
#define sa_automata_included
//----------------------------------------------------------------------

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#include "plugin/sat_parameters.h"
#include "plugin/sat_plugin.h"
#include "plugin/processor/sat_interleaved_processor.h"
//#include "audio/sat_audio_utils.h"

#if !defined (SAT_NO_GUI)
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
  .id           = SAT_VENDOR "/sa_automata/v0",
  .name         = "sa_automata",
  .vendor       = SAT_VENDOR,
  .url          = SAT_URL,
  .manual_url   = "",
  .support_url  = "",
  .version      = SAT_VERSION,
  .description  = "",
  .features     = (const char*[]){ CLAP_PLUGIN_FEATURE_NOTE_EFFECT, nullptr }
};

//----------------------------------------------------------------------
//
// processor
//
//----------------------------------------------------------------------

class sa_automata_processor
: public SAT_InterleavedProcessor {

//------------------------------
private:
//------------------------------

  bool  need_recalc = true;
  float samplerate = 0.0;

  bool                  MIsPlaying              = false;
  double                MStartTime              = 0.0;
  double                MTempo                  = 0.0;
//double                MTempoInc               = 0.0;
  double                MSampleRate             = 0.0;

  double                MSpeed                  = 1.0;
  double                MTuning                 = 0.0;
  double                MTiming                 = 1.0;

  bool                  MWraparound             = false;
  double                MTickSize               = 0.0;
  double                MTickRemaining          = 0.0;

  uint8_t               MStates[256*256]        = {0};
  uint8_t               MStatesBackup[256*256]  = {0};
  uint8_t               MTempBuffer[256*256]    = {0};

  uint32_t              p_width                 = 0;
  uint32_t              p_height                = 0;
  uint32_t              p_timing_num            = 0;
  uint32_t              p_timing_denom          = 0;
  int32_t               p_tuning                = 0;
  double                p_velocity              = 0.0;
  uint32_t              p_cc1                   = 0;
  uint32_t              p_cc2                   = 0;
  uint32_t              p_cc1_val               = 0;
  uint32_t              p_cc2_val               = 0;

  sa_automata_grid*     MGrid                   = nullptr;
  SAT_ProcessContext*   MProcessContext         = nullptr;
  
//------------------------------
public:
//------------------------------

  sa_automata_processor(SAT_ProcessorOwner* AOwner)
  : SAT_InterleavedProcessor(AOwner) {
  }

  //----------

  virtual ~sa_automata_processor() {
  }

//------------------------------
public:
//------------------------------

  void setSampleRate(double ASampleRate) {
    samplerate = ASampleRate;
  }

  void setGrid(sa_automata_grid* AGrid) {
    MGrid = AGrid;
  }

  uint8_t* getStates() {
    return MStates;
  }

//------------------------------
public:
//------------------------------

  void paramValueEvent(const clap_event_param_value_t* event) final {
    //need_recalc = true;
    //SAT_Print("index %i value %.3f\n",event->param_id,event->value);
    uint32_t index = event->param_id;
    double value = event->value;
    switch (index) {
      case 0: p_width         = value;  break;
      case 1: p_height        = value;  break;
      case 2: p_timing_num    = value;  break;
      case 3: p_timing_denom  = value;  break;
      case 4: p_tuning        = value;  break;
      case 5: p_velocity      = value;  break;
      case 6: p_cc1           = value;  break;
      case 7: p_cc2           = value;  break;
      case 8: p_cc1_val       = value;  break;
      case 9: p_cc2_val       = value;  break;
    }
  }

  //----------

  // bool on_plugin_transport(const clap_event_transport_t* event) final {
  //   SAT_ProcessContext* context = getProcessContext();
  //   SAT_Assert(context);
  //   double samplerate = context->samplerate;
  //   SAT_Assert(samplerate > 0);
  //   uint64_t samplecount = context->sample_counter;
  //   if (event->flags & CLAP_TRANSPORT_HAS_TEMPO) {
  //     MTempo = event->tempo;
  //     //MTempoInc = event->tempo_inc;
  //   }
  //   bool playing = event->flags & CLAP_TRANSPORT_IS_PLAYING;
  //   if (!MIsPlaying && playing) {
  //     MIsPlaying = true;
  //     handleStartPlaying();
  //   }
  //   else if (MIsPlaying && !playing) {
  //     MIsPlaying = false;
  //     handleStopPlaying();
  //   }
  //   calcTickSize();
  //   return true;
  // }

  //----------

  // void processStereoSample(sat_sample_t* spl0, sat_sample_t* spl1) final {
  //   if ((p_timing_num > 0) && (p_timing_denom > 0)) {
  //     MTiming = (double)p_timing_num / (double)p_timing_denom;
  //   }
  //   if (MIsPlaying) {
  //     MTickRemaining -= 1.0;
  //     if (MTickRemaining <= 0.0) {
  //       //MTickRemaining = SAT_Fract(MTickRemaining);
  //       MTickRemaining += MTickSize * MTiming;
  //       handleTick();
  //     }
  //   }
  //   //MTempo += MTempoInc;
  // }

  void processAudio(SAT_ProcessContext* AContext, uint32_t AOffset, uint32_t ALength) override {
    MProcessContext = AContext;
    const clap_process_t* process = AContext->process;
    //if (need_recalc) recalc(samplerate);
    for (uint32_t i=0; i<ALength; i++) {
      if ((p_timing_num > 0) && (p_timing_denom > 0)) {
        MTiming = (double)p_timing_num / (double)p_timing_denom;
      }
      if (MIsPlaying) {
        MTickRemaining -= 1.0;
        if (MTickRemaining <= 0.0) {
          //MTickRemaining = SAT_Fract(MTickRemaining);
          MTickRemaining += MTickSize * MTiming;
          handleTick();
        }
      }
      //MTempo += MTempoInc;
    }    
  }

//------------------------------
private:
//------------------------------

  // void recalc(float srate) {
  //   need_recalc = false;
  // }

//------------------------------
public: // ticks
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
    //SAT_PRINT;
    MTickRemaining = 0.0;
    //MTickRemaining = MTickSize;
    MGrid->copyCellsTo(MStatesBackup);
  }

  //----------

  void handleStopPlaying() {
    //SAT_PRINT;
    killAllNotes();
    MGrid->copyCellsFrom(MStatesBackup);
    //memcpy(MStates,MStatesBackup,(sizeof(uint8_t)*(256*256)));
//    if (isEditorOpen()) {
      //w_grid->do_widget_update(w_grid,0,0);
      MGrid->do_widget_redraw(MGrid,0,0);
//    }
  }

  //----------

  void handleTick() {
    updateCells();
//    if (isEditorOpen()) {
      //w_grid->do_widget_update(w_grid,0,0);
      MGrid->do_widget_redraw(MGrid,0,0);
//    }
  }

//------------------------------
public: // automata
//------------------------------

  uint8_t getCell(uint8_t* AStates, int32_t AX, int32_t AY) {
    uint32_t index = (AY * 256) + AX;
    return AStates[index];
  }

  //----------

  void setCell(uint8_t* AStates, int32_t AX, int32_t AY, uint8_t AState) {
    uint32_t index = (AY * 256) + AX;
    AStates[index] = AState;
  }

  //----------

  bool isCellAlive(uint8_t* AStates, int32_t AX, int32_t AY) {
    if (MWraparound) {
      if (AX < 0) AX += p_width;
      if (AX >= (int32_t)p_width) AX -= p_width;;
      if (AY < 0) AY += p_height;
      if (AY >= (int32_t)p_height) AY -= p_height;
    }
    else {
      if (AX < 0) return false;
      if (AX >= (int32_t)p_width) return false;
      if (AY < 0) return false;
      if (AY >= (int32_t)p_height) return false;
    }
    uint32_t index = (AY * 256) + AX;
    return (AStates[index] & 1);
  }

  //----------

  void setCellAlive(uint8_t* AStates, int32_t AX, int32_t AY, bool ATrigger) {
    if (AX < 0) return;
    if (AX >= (int32_t)p_width) return;
    if (AY < 0) return;
    if (AY >= (int32_t)p_height) return;
    uint32_t index = (AY * 256) + AX;
    AStates[index] |= 1;
    uint8_t cell = getCell(MStates,AX,AY);
    bool r = (cell & 2);
    bool g = (cell & 4);
    bool b = (cell & 8);
    if (ATrigger) {
      if (r) sendNoteOn(AX,AY,0);
      if (g) sendCC1On(AX,AY,0);
      if (b) sendCC2On(AX,AY,0);
    }
  }

  //----------

  void setCellDead(uint8_t* AStates, int32_t AX, int32_t AY, bool ATrigger) {
    if (AX < 0) return;
    if (AX >= (int32_t)p_width) return;
    if (AY < 0) return;
    if (AY >= (int32_t)p_height) return;
    uint32_t index = (AY * 256) + AX;
    AStates[index] &= (255- 1);
    uint8_t cell = getCell(MStates,AX,AY);
    bool r = (cell & 2);
    bool g = (cell & 4);
    bool b = (cell & 8);
    if (ATrigger) {
      if (r) sendNoteOff(AX,AY,0);
      if (g) sendCC1Off(AX,AY,0);
      if (b) sendCC2Off(AX,AY,0);
    }
  }
  
  //----------

  // conway's game of life

  /*
    Any live cell with fewer than two live neighbors dies, as if by underpopulation.
    Any live cell with two or three live neighbors lives on to the next generation.
    Any live cell with more than three live neighbors dies, as if by overpopulation.
    Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.
  */

  // last arg = trigger (new note/cc)

  void updateCell(int32_t AX, int32_t AY, bool AAlive, int32_t ANeighbours) {
    if (AAlive) {
      if (ANeighbours < 2)      setCellDead( MTempBuffer,AX,AY,true);   // die
      else if (ANeighbours > 3) setCellDead( MTempBuffer,AX,AY,true);   // die
      else                      setCellAlive(MTempBuffer,AX,AY,false);  // stay alive
    }
    else {
      if (ANeighbours == 3)     setCellAlive(MTempBuffer,AX,AY,true);   // birth
      else                      setCellDead( MTempBuffer,AX,AY,false);  // stay dead
    }
  }

  //----------

  void updateCells() {
    MGrid->copyCellsTo(MTempBuffer);
    for (uint32_t y=0; y<p_height; y++) {
      for (uint32_t x=0; x<p_width; x++) {

        bool alive = false;
        uint32_t neighbours = 0;
        if      (isCellAlive( MStates, x-1, y-1)) neighbours += 1;
        if      (isCellAlive( MStates, x,   y-1)) neighbours += 1;
        if      (isCellAlive( MStates, x+1, y-1)) neighbours += 1;
        if      (isCellAlive( MStates, x-1, y  )) neighbours += 1;
        alive = (isCellAlive( MStates, x,   y  ));
        if      (isCellAlive( MStates, x+1, y  )) neighbours += 1;
        if      (isCellAlive( MStates, x-1, y+1)) neighbours += 1;
        if      (isCellAlive( MStates, x,   y+1)) neighbours += 1;
        if      (isCellAlive( MStates, x+1, y+1)) neighbours += 1;

        updateCell(x,y,alive,neighbours);
      }
    }
    
    MGrid->copyCellsFrom(MTempBuffer);
  }


//------------------------------
public: // send events
//------------------------------

  void sendEvent(clap_event_header_t* event) {
    const clap_output_events_t* out_events = MProcessContext->process->out_events;
    out_events->try_push(out_events,event);
  }

  //----------

  void sendNoteOnEvent(int32_t AKey, double AVelocity, uint32_t AOffset) {
    clap_event_note_t event;
    event.header.size     = sizeof(clap_event_note_t);
    event.header.time     = AOffset;
    event.header.space_id = CLAP_CORE_EVENT_SPACE_ID;
    event.header.type     = CLAP_EVENT_NOTE_ON;
    event.header.flags    = 0;
    event.note_id         = -1;
    event.port_index      = 0;
    event.channel         = 0;
    event.key             = AKey + p_tuning;
    event.velocity        = AVelocity;
    sendEvent((clap_event_header_t*)&event);
    // const clap_output_events_t* out_events = getProcessContext()->process->out_events;
    // out_events->try_push(out_events,(clap_event_header_t*)&event);
  }

  //----------

  void sendNoteOffEvent(int32_t AKey, uint32_t AOffset) {
    clap_event_note_t event;
    event.header.size     = sizeof(clap_event_note_t);
    event.header.time     = AOffset;
    event.header.space_id = CLAP_CORE_EVENT_SPACE_ID;
    event.header.type     = CLAP_EVENT_NOTE_OFF;
    event.header.flags    = 0;
    event.note_id         = -1;
    event.port_index      = 0;
    event.channel         = 0;
    event.key             = AKey + p_tuning;
    event.velocity        = 0.0;
    sendEvent((clap_event_header_t*)&event);
    // const clap_output_events_t* out_events = getProcessContext()->process->out_events;
    // out_events->try_push(out_events,(clap_event_header_t*)&event);
  }

  //----------

  void sendMidiCCEvent(int32_t ACC, double AValue, uint32_t AOffset) {
    clap_event_midi_t midi_event;
    midi_event.header.size     = sizeof(clap_event_midi_t);
    midi_event.header.time     = AOffset;
    midi_event.header.space_id = CLAP_CORE_EVENT_SPACE_ID;
    midi_event.header.type     = CLAP_EVENT_MIDI;
    midi_event.header.flags    = 0;
    midi_event.port_index      = 0;
    midi_event.data[0]         = SAT_MIDI_CONTROL_CHANGE + 0x00;
    midi_event.data[1]         = ACC;
    midi_event.data[2]         = AValue;
    sendEvent((clap_event_header_t*)&midi_event);
  }

//------------------------------
public:
//------------------------------

  void killAllNotes() {
    for (uint32_t y=0; y<127; y++) sendNoteOffEvent(y,0);
    sendMidiCCEvent(p_cc1,0,0);
    sendMidiCCEvent(p_cc2,0,0);
  }

  //----------

  void sendNoteOn(int32_t AX, int32_t AY, uint32_t AOffset) {
    sendNoteOnEvent( 60 + AY + p_tuning, p_velocity, AOffset );
  }

  //----------

  void sendNoteOff(int32_t AX, int32_t AY, uint32_t AOffset) {
    sendNoteOffEvent( 60 + AY + p_tuning, AOffset );
  }

  //----------

  void sendCC1On(int32_t AX, int32_t AY, uint32_t AOffset) {
    sendMidiCCEvent(p_cc1,p_cc1_val,AOffset);
  }

  //----------

  void sendCC1Off(int32_t AX, int32_t AY, uint32_t AOffset) {
    sendMidiCCEvent(p_cc1,0,AOffset);
  }

  //----------

  void sendCC2On(int32_t AX, int32_t AY, uint32_t AOffset) {
    sendMidiCCEvent(p_cc2,p_cc2_val,AOffset);
  }

  //----------

  void sendCC2Off(int32_t AX, int32_t AY, uint32_t AOffset) {
    sendMidiCCEvent(p_cc2,0,AOffset);
  }

};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class sa_automata_plugin
: public SAT_Plugin {
  
//------------------------------
private:
//------------------------------

  sa_automata_processor* MProcessor = nullptr;

//------------------------------
public:
//------------------------------

  sa_automata_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : SAT_Plugin(ADescriptor,AHost) {
  }

  //----------

  virtual ~sa_automata_plugin() {
  }

//------------------------------
public:
//------------------------------

  bool init() final {
    registerDefaultExtensions();    
    registerExtension(CLAP_EXT_NOTE_PORTS);
    appendClapNoteOutputPort("Out");

    appendParameter( new SAT_IntParameter( "Width",        "", 16,   4,   32,    CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED ));
    appendParameter( new SAT_IntParameter( "Height",       "", 16,   4,   32,    CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED ));
    appendParameter( new SAT_IntParameter( "Timing Num",   "", 1,    1,   8,     CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED ));
    appendParameter( new SAT_IntParameter( "Timing Denom", "", 1,    1,   8,     CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED ));
    appendParameter( new SAT_IntParameter( "Tuning",       "", 0,   -48,  48,    CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED ));
    appendParameter( new SAT_Parameter(    "Velocity",     "", 0.0,  0.0, 1.0,   CLAP_PARAM_IS_AUTOMATABLE ));
    appendParameter( new SAT_IntParameter( "CC1",          "", 64,   0,   127,   CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED ));
    appendParameter( new SAT_IntParameter( "CC2",          "", 65,   0,   127,   CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED ));
    appendParameter( new SAT_IntParameter( "CC1 Val",      "", 127,  0,   127,   CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED ));
    appendParameter( new SAT_IntParameter( "CC2 Val",      "", 127,  0,   127,   CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED ));

    MProcessor = new sa_automata_processor(this);
    setProcessor(MProcessor);

    #if !defined (SAT_NO_GUI)
      setInitialEditorSize( 420, 560, 1.0, true );
    #endif


    return SAT_Plugin::init();
  }
  
  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    MProcessor->setSampleRate(sample_rate);
    return SAT_Plugin::activate(sample_rate,min_frames_count,max_frames_count);
  }

//------------------------------
private:
//------------------------------

  sa_automata_grid*     w_grid                  = nullptr;
  SAT_DragValueWidget*  w_grid_width            = nullptr;
  SAT_DragValueWidget*  w_grid_height           = nullptr;
  SAT_TextWidget*       w_timing_text           = nullptr;
  SAT_DragValueWidget*  w_timing_num            = nullptr;
  SAT_TextWidget*       w_timing_sep            = nullptr;
  SAT_DragValueWidget*  w_timing_denom          = nullptr;
  SAT_DragValueWidget*  w_tuning                = nullptr;
  SAT_DragValueWidget*  w_velocity              = nullptr;
  SAT_DragValueWidget*  w_cc1                   = nullptr;
  SAT_DragValueWidget*  w_cc2                   = nullptr;
  SAT_DragValueWidget*  w_cc1_val               = nullptr;
  SAT_DragValueWidget*  w_cc2_val               = nullptr;

//------------------------------
public: // gui
//------------------------------

  #ifndef SAT_NO_GUI

    bool setupEditor(SAT_Editor* AEditor) final {
      //SAT_TRACE;
      SAT_Window* window = AEditor->getWindow();
      SAT_RootWidget* root = new SAT_RootWidget(window);
      window->setRootWidget(root);

      w_grid          = new sa_automata_grid(     SAT_Rect( 10, 10, 400,400), 16, 16, MProcessor->getStates() );
      w_grid_width    = new SAT_DragValueWidget(  SAT_Rect( 10,440, 80, 20 ), "Width");
      w_grid_height   = new SAT_DragValueWidget(  SAT_Rect( 10,465, 80, 20 ), "Height");

      w_timing_text   = new SAT_TextWidget(       SAT_Rect( 10,500, 35, 20 ), "Timing");
      w_timing_num    = new SAT_DragValueWidget(  SAT_Rect( 55,500, 20, 20 ), "");
      w_timing_sep    = new SAT_TextWidget(       SAT_Rect( 78,500, 5, 20 ), "/");
      w_timing_denom  = new SAT_DragValueWidget(  SAT_Rect( 85,500, 20, 20 ), "");

      w_tuning        = new SAT_DragValueWidget(  SAT_Rect(100,440, 80, 20 ), "Tuning");
      w_velocity      = new SAT_DragValueWidget(  SAT_Rect(100,465, 80, 20 ), "Velocity");
      w_cc1           = new SAT_DragValueWidget(  SAT_Rect(190,440, 80, 20 ), "CC1");
      w_cc2           = new SAT_DragValueWidget(  SAT_Rect(190,465, 80, 20 ), "CC2");
      w_cc1_val       = new SAT_DragValueWidget(  SAT_Rect(280,440, 80, 20 ), "CC1 Val");
      w_cc2_val       = new SAT_DragValueWidget(  SAT_Rect(280,465, 80, 20 ), "CC2 Val");

      w_tuning->setBackgroundColor( SAT_Red );
      w_velocity->setBackgroundColor( SAT_Red );

      w_cc1->setBackgroundColor( SAT_Green );
      w_cc1_val->setBackgroundColor( SAT_Green );
      w_cc2->setBackgroundColor( SAT_Blue );
      w_cc2_val->setBackgroundColor( SAT_Blue );

      w_timing_text->setDrawBorder(false);
      w_timing_num->setDrawText(false);
      w_timing_sep->setDrawBorder(false);
      w_timing_denom->setDrawText(false);

      // root->WGridWidth  = w_grid_width;
      // root->WGridHeight = w_grid_height;
      // root->WGrid       = w_grid;

      root->appendChild(w_grid_width);
      root->appendChild(w_grid_height);
      root->appendChild(w_timing_text);
      root->appendChild(w_timing_num);
      root->appendChild(w_timing_sep);
      root->appendChild(w_timing_denom);
      root->appendChild(w_grid);
      root->appendChild(w_tuning);
      root->appendChild(w_velocity);
      root->appendChild(w_cc1);
      root->appendChild(w_cc2);
      root->appendChild(w_cc1_val);
      root->appendChild(w_cc2_val);

      AEditor->connect(w_grid_width,  getParameter(0));
      AEditor->connect(w_grid_height, getParameter(1));
      AEditor->connect(w_timing_num,  getParameter(2));
      AEditor->connect(w_timing_denom,getParameter(3));
      AEditor->connect(w_tuning,      getParameter(4));
      AEditor->connect(w_velocity,    getParameter(5));
      AEditor->connect(w_cc1,         getParameter(6));
      AEditor->connect(w_cc2,         getParameter(7));
      AEditor->connect(w_cc1_val,     getParameter(8));
      AEditor->connect(w_cc2_val,     getParameter(9));

      MProcessor->setGrid(w_grid);

      return true;

    }
  #endif


};

//----------------------------------------------------------------------
//
// entry point
//
//----------------------------------------------------------------------

#ifndef SAT_NO_ENTRY
  #include "plugin/sat_entry.h"
  SAT_PLUGIN_ENTRY(sa_automata_descriptor,sa_automata_plugin)
#endif

//----------------------------------------------------------------------
#endif


