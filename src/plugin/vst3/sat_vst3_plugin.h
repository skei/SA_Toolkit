#ifndef sat_vst3_plugin_included
#define sat_vst3_plugin_included
//----------------------------------------------------------------------

// clap-as-vst3
// this is a mess.. work in progress..

//----------------------------------------------------------------------

#include "base/sat.h"
#include "plugin/clap/sat_clap.h"
#include "plugin/vst3/sat_vst3.h"
#include "plugin/vst3/sat_vst3_utils.h"

//----------------------------------------------------------------------

struct SAT_Vst3NoteId {
  int32_t note_id   = -1;
  int16_t port      = -1;
  int16_t channel   = -1;
  int16_t key       = -1;
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

//class SAT_Vst3Plugin
//: public SAT_Plugin {

class SAT_Vst3Plugin
: public IComponent
, public IAudioProcessor
, public IUnitInfo
, public Vst::IConnectionPoint
, public IMidiMapping
, public IKeyswitchController
, public INoteExpressionController
, public IEditController
, public IEditController2
, public IPlugView
#ifndef SAT_WIN32
, public ITimerHandler
#endif
/*, public SAT_WindowListener*/ {

//------------------------------
private:
//------------------------------

  SAT_ClapPlugin*                 MPlugin             = nullptr;
  const clap_plugin_descriptor_t* MDescriptor         = nullptr;
  uint32_t                        MRefCount           = 1;
  IComponentHandler*              MComponentHandler   = nullptr;
  IComponentHandler2*             MComponentHandler2  = nullptr;
  IPlugFrame*                     MPlugFrame          = nullptr;
  IHostApplication*               MHostApp            = nullptr;
  ParameterInfo*                  MParamInfos         = nullptr;
  #ifdef SAT_LINUX
    IRunLoop*                     MRunLoop            = nullptr;
  #endif
  uint32_t                        MIoMode             = 0;
  char                            MHostName[129]      = {0};
  bool                            MIsProcessing       = false;
  uint32_t                        MProcessMode        = 0;
  uint32_t                        MSampleSize         = 0;
  uint32_t                        MBlockSize          = 0;
  float                           MSampleRate         = 0.0;
  char                            MEditorId[16]       = {0};
  int64_t                         MSteadyTime         = 0;
  clap_process_t                  MClapProcess        = {};
  clap_event_transport_t          MTransport          = {};
  clap_audio_buffer_t             MAudioInputs        = {};
  clap_audio_buffer_t             MAudioOutputs       = {};
  uint32_t                        MNumEvents          = 0;
  char                            MEvents[SAT_PLUGIN_VST3_MAX_EVENTS_PER_BLOCK * SAT_PLUGIN_VST3_MAX_EVENT_SIZE]  = {0};
  uint32_t                        MLastNoteId         = 0;
  SAT_Vst3NoteId                  MNoteIds[SAT_PLUGIN_VST3_MAX_NOTE_IDS] = {};
  SAT_LockFreeQueue<uint32_t,SAT_PLUGIN_VST3_MAX_GUI_EVENTS> MHostParamQueue = {}; // gui -> host
  double  MQueuedHostParamValues[SAT_PLUGIN_VST3_MAX_EVENTS_PER_BLOCK] = {0};

//------------------------------
public:
//------------------------------

  SAT_Vst3Plugin(SAT_ClapPlugin* APlugin) {
    MPlugin     = APlugin;
    MDescriptor = MPlugin->getDescriptor();
    MRefCount   = 1;
    setupParameterInfo();
  }
  
  //----------

  virtual ~SAT_Vst3Plugin() {
    if (MPlugin) delete MPlugin;
    if (MParamInfos) free(MParamInfos);
  }
  
//------------------------------
private: // in_events
//------------------------------

  uint32_t vst3_input_events_size() {
    return MNumEvents;
  }

  //----------

  const clap_event_header_t* vst3_input_events_get(uint32_t index) {
    uint32_t pos = SAT_PLUGIN_VST3_MAX_EVENT_SIZE * index;
    return (const clap_event_header_t*)&MEvents[pos];
  }

  //----------

  clap_input_events_t MVst3InputEvents = {
    this,
    vst3_input_events_size_callback,
    vst3_input_events_get_callback
  };

  //----------

  static uint32_t vst3_input_events_size_callback(const struct clap_input_events *list) {
    SAT_Vst3Plugin* plugin = (SAT_Vst3Plugin*)list->ctx;
    return plugin->vst3_input_events_size();
  }

  //----------

  static const clap_event_header_t* vst3_input_events_get_callback(const struct clap_input_events *list, uint32_t index) {
    SAT_Vst3Plugin* plugin = (SAT_Vst3Plugin*)list->ctx;
    return plugin->vst3_input_events_get(index);
  }

//------------------------------
private: // out_events
//------------------------------

  bool vst3_output_events_try_push(const clap_event_header_t *event) {
    if (event->space_id == CLAP_CORE_EVENT_SPACE_ID) {
      switch (event->type) {
        case CLAP_EVENT_NOTE_ON:
          SAT_Print("TODO: send NOTE_ON to host\n");
          return true;
        case CLAP_EVENT_NOTE_OFF:
          SAT_Print("TODO: send NOTE_OFF to host\n");
          return true;
        case CLAP_EVENT_NOTE_CHOKE:
          SAT_Print("TODO: send NOTE_CHOKE to host\n");
          return true;
        case CLAP_EVENT_NOTE_END:
          SAT_Print("TODO: send NOTE_END to host\n");
          return true;
        case CLAP_EVENT_NOTE_EXPRESSION:
          SAT_Print("TODO: send NOTE_EXPRESSION to host\n");
          return true;
        case CLAP_EVENT_PARAM_GESTURE_BEGIN:
          SAT_Print("TODO: send PARAM_GESTURE_BEGIN to host\n");
          return true;
        case CLAP_EVENT_PARAM_GESTURE_END:
          SAT_Print("TODO: send PARAM_GESTURE_END to host\n");
          return true;

        case CLAP_EVENT_PARAM_VALUE: {
          SAT_Print("queueing PARAM_VALUE (to host)\n");
          clap_event_param_value_t* param_value = (clap_event_param_value_t*)event;
          uint32_t index = param_value->param_id;
          double value = param_value->value;

          clap_param_info_t info;

          SAT_Print("\n");
          SAT_Print("index %i\n",index);
          SAT_Print("&info %p\n",&info);
          SAT_Print("MPlugin %p\n",MPlugin);
          SAT_Print("\n");

          MPlugin->params_get_info(index,&info); // crash? (win32)
          double range = info.max_value - info.min_value;
          if (range > 0) {
            value -= info.min_value;
            value /= range;
          }
          queueHostParam(index,value);
          return true;
        }

        case CLAP_EVENT_PARAM_MOD:
          SAT_Print("TODO: send PARAM_MOD to host\n");
          return true;
        case CLAP_EVENT_TRANSPORT:
          SAT_Print("TODO: send TRANSPORT to host\n");
          return true;
        case CLAP_EVENT_MIDI:
          SAT_Print("TODO: send MIDI to host\n");
          return true;
        case CLAP_EVENT_MIDI_SYSEX:
          SAT_Print("TODO: send MIDI_SYSEX to host\n");
          return true;
        case CLAP_EVENT_MIDI2:
          SAT_Print("TODO: send MIDI2 to host\n");
          return true;
      } // switch
    } // clap_space
    return false;
  }

  //----------

  clap_output_events_t MVst3OutputEvents = {
    this,
    vst3_output_events_try_push_callback
  };

  //----------

  static bool vst3_output_events_try_push_callback(const struct clap_output_events *list, const clap_event_header_t *event) {
    SAT_Vst3Plugin* plugin = (SAT_Vst3Plugin*)list->ctx;
    return plugin->vst3_output_events_try_push(event);
  }

//------------------------------
private:
//------------------------------

  void queueHostParam(uint32_t AIndex, double AValue) {
    MQueuedHostParamValues[AIndex] = AValue;
    MHostParamQueue.write(AIndex);
  }

  //----------

  void flushHostParams() {
    if (MComponentHandler) {
      uint32_t index;
      while (MHostParamQueue.read(&index)) {
        double value = MQueuedHostParamValues[index];
        //SAT_Print("flush! %i = %.3f\n",index,value);

        //if (MComponentHandler2) MComponentHandler2->startGroupEdit();
        MComponentHandler->beginEdit(index);          // click
        MComponentHandler->performEdit(index,value);  // drag
        MComponentHandler->endEdit(index);            // release
        //if (MComponentHandler2) MComponentHandler2->finishGroupEdit();
      }
    }
  }

  //----------

  void prepareParameters(ProcessData& data) {
    IParameterChanges* paramChanges = data.inputParameterChanges;
    if (paramChanges) {
      int32_t num_param_changes = paramChanges->getParameterCount();
      if (num_param_changes > 0) {
        for (int32_t i=0; i<num_param_changes; i++) {
          IParamValueQueue* paramQueue = paramChanges->getParameterData(i);
          if (paramQueue) {
            uint32_t id = paramQueue->getParameterId();
            if (id < MPlugin->params_count()) {

              clap_param_info_t info;
              MPlugin->params_get_info(id,&info);

              int32_t pointcount = paramQueue->getPointCount();
              for (int32_t j=0; j<pointcount; j++) {
                int32_t offset = 0;
                double value = 0;
                paramQueue->getPoint(j,offset,value);
                value = info.min_value + ((info.max_value - info.min_value) * value);
                uint32_t pos = SAT_PLUGIN_VST3_MAX_EVENT_SIZE * MNumEvents++;
                clap_event_param_value_t* param_value_event = (clap_event_param_value_t*)&MEvents[pos];
                memset(param_value_event,0,sizeof(clap_event_param_value_t));
                param_value_event->header.size     = sizeof(clap_event_param_value_t);
                param_value_event->header.time     = offset;
                param_value_event->header.space_id = CLAP_CORE_EVENT_SPACE_ID;
                param_value_event->header.type     = CLAP_EVENT_PARAM_VALUE;
                param_value_event->header.flags    = 0;
                param_value_event->param_id        = id;//paramQueue->getParameterId();
                param_value_event->note_id         = -1;
                param_value_event->port_index      = 0;
                param_value_event->channel         = 0;
                param_value_event->key             = 0;
                param_value_event->value           = value;
                //SAT_Print("param_value: %i = %.3f\n",paramQueue->getParameterId(),value);
              }
            } // id < param
          } // paramqueue
        } // for all params
      } // numparams > 0
    } // paramchanges
  }

  //----------

  void prepareEvents(ProcessData& data) {
    IEventList* inputEvents = data.inputEvents;
    if (inputEvents) {
      int32_t num = inputEvents->getEventCount();
      for (int32_t i=0; i<num; i++) {
        Event event;
        inputEvents->getEvent(i,event);
        switch (event.type) {

          case Event::kNoteOnEvent: {
            uint32_t pos = SAT_PLUGIN_VST3_MAX_EVENT_SIZE * MNumEvents++;
            clap_event_note_t* note_event = (clap_event_note_t*)&MEvents[pos];
            memset(note_event,0,sizeof(clap_event_note_t));
            note_event->header.size     = sizeof(clap_event_note_t);
            note_event->header.time     = event.sampleOffset;
            note_event->header.space_id = CLAP_CORE_EVENT_SPACE_ID;
            note_event->header.type     = CLAP_EVENT_NOTE_ON;
            note_event->header.flags    = 0;
            note_event->note_id         = event.noteOn.noteId;
            note_event->port_index      = 0; // event.busIndex;
            note_event->channel         = event.noteOn.channel;
            note_event->key             = event.noteOn.pitch;
            note_event->velocity        = event.noteOn.velocity;
            //event.noteOn.tuning
            //event.noteOn.length
            MNoteIds[MLastNoteId].note_id = event.noteOn.noteId;
            MNoteIds[MLastNoteId].port    = 0; // event.busIndex;
            MNoteIds[MLastNoteId].channel = event.noteOn.channel;
            MNoteIds[MLastNoteId].key     = event.noteOn.pitch;
            MLastNoteId += 1;
            MLastNoteId %= SAT_PLUGIN_VST3_MAX_NOTE_IDS;
            break;
          }

          case Event::kNoteOffEvent: {
            uint32_t pos = SAT_PLUGIN_VST3_MAX_EVENT_SIZE * MNumEvents++;
            clap_event_note_t* note_event = (clap_event_note_t*)&MEvents[pos];
            memset(note_event,0,sizeof(clap_event_note_t));
            note_event->header.size     = sizeof(clap_event_note_t);
            note_event->header.time     = event.sampleOffset;
            note_event->header.space_id = CLAP_CORE_EVENT_SPACE_ID;
            note_event->header.type     = CLAP_EVENT_NOTE_OFF;
            note_event->header.flags    = 0;
            note_event->note_id         = event.noteOff.noteId;
            note_event->port_index      = 0; // event.busIndex;
            note_event->channel         = event.noteOff.channel;
            note_event->key             = event.noteOff.pitch;
            note_event->velocity        = event.noteOff.velocity;
            //event.noteOff.tuning
            for (uint32_t i=0; i<SAT_PLUGIN_VST3_MAX_NOTE_IDS; i++) {
              if (MNoteIds[i].note_id == event.noteOff.noteId) {
                MNoteIds[i].note_id = -1;
                MNoteIds[i].port    = -1;
                MNoteIds[i].channel = -1;
                MNoteIds[i].key     = -1;
              }
            }
            break;
          }

          case Event::kPolyPressureEvent: {
            uint32_t pos = SAT_PLUGIN_VST3_MAX_EVENT_SIZE * MNumEvents++;
            clap_event_note_expression_t* note_expression_event = (clap_event_note_expression_t*)&MEvents[pos];
            memset(note_expression_event,0,sizeof(clap_event_note_expression_t));
            note_expression_event->header.size     = sizeof(clap_event_note_expression_t);
            note_expression_event->header.time     = event.sampleOffset;
            note_expression_event->header.space_id = CLAP_CORE_EVENT_SPACE_ID;
            note_expression_event->header.type     = CLAP_EVENT_NOTE_EXPRESSION;
            note_expression_event->header.flags    = 0;
            note_expression_event->note_id         = event.polyPressure.noteId;
            note_expression_event->port_index      = 0; // event.busIndex;
            note_expression_event->channel         = event.polyPressure.channel;
            note_expression_event->key             = event.polyPressure.pitch;
            note_expression_event->expression_id   = CLAP_NOTE_EXPRESSION_PRESSURE;
            note_expression_event->value           = event.polyPressure.pressure;
            break;
          }

          case Event::kNoteExpressionValueEvent: {
            uint32_t pos = SAT_PLUGIN_VST3_MAX_EVENT_SIZE * MNumEvents++;
            clap_event_note_expression_t* note_expression_event = (clap_event_note_expression_t*)&MEvents[pos];
            memset(note_expression_event,0,sizeof(clap_event_note_expression_t));
            //uint32_t chan = (event.noteExpressionValue.noteId >> 8);
            //uint32_t key = (event.noteExpressionValue.noteId & 255);
            int32_t port = -1;
            int32_t chan = -1;
            int32_t key = -1;
            for (uint32_t i=0; i<SAT_PLUGIN_VST3_MAX_NOTE_IDS; i++) {
              if (MNoteIds[i].note_id == event.noteExpressionValue.noteId) {
                port    = MNoteIds[i].port;
                chan    = MNoteIds[i].channel;
                key     = MNoteIds[i].key;
              }
            }
            SAT_Assert(chan >= 0);
            SAT_Assert(key >= 0);
            note_expression_event->header.size     = sizeof(clap_event_note_expression_t);
            note_expression_event->header.time     = event.sampleOffset;
            note_expression_event->header.space_id = CLAP_CORE_EVENT_SPACE_ID;
            note_expression_event->header.type     = CLAP_EVENT_NOTE_EXPRESSION;
            note_expression_event->header.flags    = 0;
            note_expression_event->note_id         = event.noteExpressionValue.noteId;
            note_expression_event->port_index      = port; //event.busIndex;
            note_expression_event->channel         = chan; //event.noteExpressionValue.channel;
            note_expression_event->key             = key;  //event.noteExpressionValue.noteId;
            switch (event.noteExpressionValue.typeId) {
              case kVolumeTypeID:
                // Volume, plain range [0 = -oo , 0.25 = 0dB, 0.5 = +6dB, 1 = +12dB]:
                // plain = 20 * log (4 * norm)
                note_expression_event->expression_id  = CLAP_NOTE_EXPRESSION_VOLUME;
                note_expression_event->value          = 20.0 * log( 4.0 * event.noteExpressionValue.value );
                break;
              case kPanTypeID:
                  // Panning (L-R), plain range [0 = left, 0.5 = center, 1 = right]
                note_expression_event->expression_id  = CLAP_NOTE_EXPRESSION_PAN;
                note_expression_event->value          = (event.noteExpressionValue.value - 0.5) - 2.0;
                break;
              case kTuningTypeID:
                  // Tuning, plain range [0 = -120.0 (ten octaves down), 0.5 none, 1 = +120.0 (ten octaves up)]
                  // plain = 240 * (norm - 0.5) and norm = plain / 240 + 0.5
                  // oneOctave is 12.0 / 240.0; oneHalfTune = 1.0 / 240.0;
                note_expression_event->expression_id  = CLAP_NOTE_EXPRESSION_TUNING;
                note_expression_event->value          = (event.noteExpressionValue.value - 0.5) * 240.0;
                break;
              case kVibratoTypeID:
                note_expression_event->expression_id  = CLAP_NOTE_EXPRESSION_VIBRATO;
                note_expression_event->value          = event.noteExpressionValue.value;
                break;
              case kExpressionTypeID:
                note_expression_event->expression_id  = CLAP_NOTE_EXPRESSION_EXPRESSION;
                note_expression_event->value          = event.noteExpressionValue.value;
                break;
              case kBrightnessTypeID:
                note_expression_event->expression_id  = CLAP_NOTE_EXPRESSION_BRIGHTNESS;
                note_expression_event->value          = event.noteExpressionValue.value;
                break;
              //case kTextTypeID:
              //  break;
              //case kPhonemeTypeID:
              //  break;
            }
            break;
          }
          //case Event::kDataEvent:
          //case Event::kNoteExpressionTextEvent:
          //case Event::kChordEvent:
          //case Event::kScaleEvent:
        } // event.type
      } // for all events
    } // if input events
  }

  //----------

  static
  int compare_events(const void* a, const void* b) {
    clap_event_header_t* ev1 = (clap_event_header_t*)a;
    clap_event_header_t* ev2 = (clap_event_header_t*)b;
    return (ev1->time - ev2->time);
  }

  //----------

  void sortEvents() {
    qsort(MEvents,MNumEvents,SAT_PLUGIN_VST3_MAX_EVENT_SIZE, compare_events);
  }

//------------------------------
private:
//------------------------------

  double normalizeParameter(uint32_t index, double value) {
    clap_param_info_t info;
    MPlugin->params_get_info(index,&info);
    double range = info.max_value - info.min_value;
    if (range == 0) return 0.0;
    return (value - info.min_value) / range;
  }

  //----------

  double denormalizeParameter(uint32_t index, double value) {
    clap_param_info_t info;
    MPlugin->params_get_info(index,&info);
    double range = info.max_value - info.min_value;
    return info.min_value + (value * range);
  }

//------------------------------
public: // editor listener
//------------------------------

  //#ifndef SAT_NO_GUI
  //
  ///*
  //  we run our gui in its own thread, but need to communicate with the
  //  host on its gui thread.. so, when we tweak a parameter, we queue the
  //  parameter (index), and send all updated ones to the host during onTimer
  //  (see bottom)
  //  the host should communicate the parameter change (audio thread) during
  //  next process()
  //*/
  //
  //// AValue is 0..1 (widget-value)
  //// TODO: convert via parameter if necessary..
  //
  //void on_updateParameterFromEditor(uint32_t AIndex, float AValue) override {
  //  //MEditorParameterValues[AIndex] = AValue;
  //  //SAT_Parameter* parameter = MDescriptor->parameters[AIndex];
  //  //float value = parameter->from01(AValue);
  //  float value = AValue;
  //  MPlugin->setParameterValue(AIndex,value);
  //  MVst2Host->updateParameter(AIndex,value);
  //}
  //
  ////----------
  //
  //void on_resizeFromEditor(uint32_t AWidth, uint32_t AHeight) override {
  //}
  //
  //#endif

//------------------------------
public:
//------------------------------

  //void notifyHostUpdateParameter(uint32_t AIndex, float AValue) override {
  //  if (MComponentHandler) {
  //    //if (MComponentHandler2) MComponentHandler2->startGroupEdit();
  //    MComponentHandler->beginEdit(AIndex);          // click
  //    MComponentHandler->performEdit(AIndex,AValue);  // drag
  //    MComponentHandler->endEdit(AIndex);            // release
  //    //if (MComponentHandler2) MComponentHandler2->finishGroupEdit();
  //  }
  //}

  //----------

  //void notifyHostResizeWindow(uint32_t AWidth, uint32_t AHeight) override {
  //}

  //----------

  //void notifyHostMidiOutput(uint32_t AOffset, uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3) override {
  //}

//------------------------------
public: // editor listener
//------------------------------

  /*
    aka on_updateHostParameter
    we must call the MComponentHandler from the same thread as we received it,
    so we write to a queue, and read them all in updateParametersInTimer(),
    which is called via IRunLoop (same thread as setComponentHandler)
    see updateParametersInTimer below
  */

  //void updateParameterFromEditor(uint32_t AIndex, float AValue) override {
  //  MHostValues[AIndex] = AValue;
  //  MUpdatedHostParameters.write(AIndex);
  //  SAT_BaseInstance::updateParameterFromEditor(AIndex,AValue);
  //}

  //----------

  //void resizeWindowFromEditor(uint32_t AWidth, uint32_t AHeight) override {
  //  SAT_BaseInstance::resizeWindowFromEditor(AWidth,AHeight) ;
  //}

//------------------------------
private:
//------------------------------

  /*
    struct ParameterInfo {
      ParamID id;                         // unique identifier of this parameter (named tag too)
      String128 title;                    // parameter title (e.g. "Volume")
      String128 shortTitle;               // parameter shortTitle (e.g. "Vol")
      String128 units;                    // parameter unit (e.g. "dB")
      int32 stepCount;                    // number of discrete steps (0: continuous, 1: toggle, discrete value otherwise
                                          // (corresponding to max - min, for example: 127 for a min = 0 and a max = 127) - see \ref vst3parameterIntro)
      ParamValue defaultNormalizedValue;  // default normalized value [0,1] (in case of discrete value: defaultNormalizedValue = defDiscreteValue / stepCount)
      UnitID unitId;                      // id of unit this parameter belongs to (see vst3UnitsIntro)
      int32 flags;                        // ParameterFlags (see below)
      enum ParameterFlags {
        kCanAutomate      = 1 << 0,   // parameter can be automated
        kIsReadOnly       = 1 << 1,   // parameter cannot be changed from outside (implies that kCanAutomate is false)
        kIsWrapAround     = 1 << 2,   // attempts to set the parameter value out of the limits will result in a wrap around [SDK 3.0.2]
        kIsList           = 1 << 3,   // parameter should be displayed as list in generic editor or automation editing [SDK 3.1.0]
        kIsProgramChange  = 1 << 15,  // parameter is a program change (unitId gives info about associated unit
                                      // - see \ref vst3UnitPrograms)
        kIsBypass         = 1 << 16   // special bypass parameter (only one allowed): Plug-in can handle bypass
                                      // (highly recommended to export a bypass parameter for effect Plug-in)
      };
    };
  */

  /*
    called from
      SAT_Vst3Instance constructor
  */

  void setupParameterInfo() {
    if (!MParamInfos) {
      uint32_t num = MPlugin->params_count();
      MParamInfos = (ParameterInfo*)malloc( num * sizeof(ParameterInfo) );
      for (uint32_t i=0; i<num; i++) {
        clap_param_info_t info;// = {0};
        //memset(info,0,sizeof(clap_param_info_t));
        MPlugin->params_get_info(i,&info);
        MParamInfos[i].id = i;
        VST3_CharToUtf16(info.name,MParamInfos[i].title);
        VST3_CharToUtf16(info.name,MParamInfos[i].shortTitle);
        VST3_CharToUtf16("",MParamInfos[i].units);
        if (info.flags & CLAP_PARAM_IS_STEPPED) {
          MParamInfos[i].stepCount = info.max_value - info.min_value + 1;
        }
        MParamInfos[i].defaultNormalizedValue = 0; // info.default_value; // TODO: normalize!
        MParamInfos[i].unitId = kRootUnitId; //-1;
        int32_t flags = 0;
        if (info.flags & CLAP_PARAM_IS_AUTOMATABLE) flags += ParameterInfo::kCanAutomate;
        else flags += ParameterInfo::kIsReadOnly; // ??
        MParamInfos[i].flags = flags;
      }
    }
  }

  //----------

  /*
    enum RestartFlags {
      kReloadComponent            // The Component should be reloaded
      kIoChanged                  // Input and/or Output Bus configuration has changed
      kParamValuesChanged         // Multiple parameter values have changed (as result of a program change for example)
      kLatencyChanged             // Latency has changed (IAudioProcessor.getLatencySamples)
      kParamTitlesChanged         // Parameter titles or default values or flags have changed
      kMidiCCAssignmentChanged    // MIDI Controller Assignments have changed
      kNoteExpressionChanged      // Note Expression has changed (info, count...)
      kIoTitlesChanged            // Input and/or Output bus titles have changed
      kPrefetchableSupportChanged // Prefetch support has changed (\see IPrefetchableSupport)
      kRoutingInfoChanged         // RoutingInfo has changed (\see IComponent)
    };
    MComponentHandler->restartComponent(int32 flags)
  */

  /*
    https://github.com/soundradix/JUCE/commit/2e9e66cbc8c65e889be5232ffae83c0ca78f9c7e
    performEdit ((Vst::ParamID) index, (double) newValue);
    // call setParamNormalized too, as without it value will reset at endEdit in Cubase.
    // setParamNormalized does not replace performEdit as it does not record automation.
    setParamNormalized ((Vst::ParamID) index, (double) newValue);

    https://sdk.steinberg.net/viewtopic.php?t=693
    "Remember that everything in the edit controller domain must happen on the
    main thread also calls to the IComponentHandler instance of the host. So
    don't call beginEdit, endEdit or performEdit on a secondary thread."

    NB: Cubase has problems if performEdit is called without setParamNormalized
    EditController::setParamNormalized(AIndex,AValue);

    MComponentHandler->performEdit(AIndex,AValue); // drag

  */

  /*
    called from
      SAT_Vst3Instance.onTimer
  */

  //void notifyHostAboutUpdatedParameters() {
  //  uint32_t index;
  //  while (MUpdatedHostParameters.read(&index)) {
  //    float value = MHostValues[index];
  //
  //    SAT_Trace("%i %.2f\n",index,value);
  //
  //    if (MComponentHandler) {
  //      //if (MComponentHandler2) MComponentHandler2->startGroupEdit();
  //      MComponentHandler->beginEdit(index);          // click
  //      MComponentHandler->performEdit(index,value);  // drag
  //      MComponentHandler->endEdit(index);            // release
  //      //if (MComponentHandler2) MComponentHandler2->finishGroupEdit();
  //    }
  //  }
  //}

//------------------------------
public: // FUnknown
//------------------------------

  /*
    Adds a reference and return the new reference count.
    Remarks: The initial reference count after creating an object is 1.
  */

  uint32 PLUGIN_API addRef() override {
    MRefCount++;
    return MRefCount;
  }

  //----------

  /*
    Releases a reference and return the new reference count.
    If the reference count reaches zero, the object will be destroyed in
    memory.
  */

  uint32 PLUGIN_API release() override {
    uint32_t r = --MRefCount;
    if (r == 0) {
      SAT_Print("deleting\n");
      delete this;
    };
    return r;
  }

  //----------

  /*
    Query for a pointer to the specified interface.
    Returns kResultOk on success or kNoInterface if the object does not
    implement the interface.
    The object has to call addRef when returning an interface.
    _iid: (in)  16 Byte interface identifier (-> FUID)
    obj:  (out) On return, *obj points to the requested interface

    file:///WORK/code/_backup/vst3/VST3SDK/index.html

    Although it is not recommended, it is possible to implement both, the
    processing part and the controller part in one component class. The host
    tries to query the IEditController interface after creating an
    IAudioProcessor and on success uses it as controller.
  */

  tresult PLUGIN_API queryInterface(const TUID _iid, void** obj) override {
    *obj = nullptr;
    if ( FUnknownPrivate::iidEqual(IAudioProcessor_iid,_iid) ) {
      *obj = (IAudioProcessor*)this;
      addRef();
      return kResultOk;
    }
    if ( FUnknownPrivate::iidEqual(IEditController_iid,_iid) ) {
      *obj = (IEditController*)this;
      addRef();
      return kResultOk;
    }
    if ( FUnknownPrivate::iidEqual(IMidiMapping_iid,_iid) ) {
      *obj = (IMidiMapping*)this;
      addRef();
      return kResultOk;
    }
    if ( FUnknownPrivate::iidEqual(IUnitInfo_iid,_iid) ) {
      *obj = (IUnitInfo*)this;
      addRef();
      return kResultOk;
    }
    if ( FUnknownPrivate::iidEqual(INoteExpressionController_iid,_iid) ) {
      *obj = (INoteExpressionController*)this;
      addRef();
      return kResultOk;
      return kNoInterface;
    }
    if ( FUnknownPrivate::iidEqual(IKeyswitchController_iid,_iid) ) {
      //*obj = (IKeyswitchController*)this;
      //addRef();
      //return kResultOk;
      return kNoInterface;
    }
    if ( FUnknownPrivate::iidEqual(IConnectionPoint_iid,_iid) ) {
      *obj = (IConnectionPoint*)this;
      addRef();
      return kResultOk;
    }
    //if ( FUnknownPrivate::iidEqual(ITimerHandler_iid,_iid) ) {
    //  *obj = (ITimerHandler*)this;
    //  addRef();
    //  return kResultOk;
    //}
    return kNoInterface;
  }

//------------------------------
public: // IPluginBase
//------------------------------

  /*
    file:///WORK/code/_backup/vst3/VST3SDK/index.html

    The context parameter passed to IPluginBase::initialize is
    Vst::IHostApplication. Hosts should not call others functions before
    initialize is called!, except IComponent::setIoMode which need to be
    called before or IComponent::getControllerClassId which could be called
    before.

    note: Extensive memory allocations etc. should be performed in this
    method rather than in the class' constructor!
    If the method does NOT return kResultOk, the object is released
    immediately. In this case terminate is not called!
  */

  /*
    class IHostApplication: public FUnknown {
      // Gets host application name.
      virtual tresult PLUGIN_API getName (String128 name) = 0;
      // Creates host object (e.g. Vst::IMessage).
      virtual tresult PLUGIN_API createInstance (TUID cid, TUID _iid, void** obj) = 0;
  */

  /*
    3.6.12
    Allow a Plug-in to ask the host if a given Plug-in interface is
    supported/used by the host. It is implemented by the hostContext given
    when the component is initialized.
    tresult PLUGIN_API MyPluginController::initialize (FUnknown* context) {
      // ...
      FUnknownPtr<IPlugInterfaceSupport> plugInterfaceSupport (context);
      if (plugInterfaceSupport) {
        if (plugInterfaceSupport->isPlugInterfaceSupported (IMidiMapping::iid) == kResultTrue)
          // IMidiMapping is used by the host
      }
      // ...
    }
  */

  /*
    IHostApplication: passed as 'context' in to IPluginBase::initialize()
  */

  tresult PLUGIN_API initialize(FUnknown* context) override {
    MHostApp = (IHostApplication*)context;
    //context->queryInterface(IHostApplication_iid, (void**)&MHostApp);
    if (MHostApp) {
      String128 u;
      MHostApp->getName(u);
      VST3_Utf16ToChar(u,MHostName);
    }
    else {
    }
    //MPlugin->on_plugin_init();
    return kResultOk;
  }

  //----------

  /*
    This function is called before the Plug-in is unloaded and can be used
    for cleanups. You have to release all references to any host application
    interfaces.
  */

  tresult PLUGIN_API terminate() override {
    //MPlugin->on_plugin_deinit();
    return kResultOk;
  }

//------------------------------
public: // IComponent
//------------------------------

  /*
    Called before initializing the component to get information about the
    controller class.

    file:///WORK/code/_backup/vst3/VST3SDK/index.html

    In order to enable the host to create a corresponding edit controller the
    processing component has to provide the matching class-ID. The host uses
    the module's class factory to create the controller component.
    (IComponent::getControllerClassId)
  */

  tresult PLUGIN_API getControllerClassId(TUID classId) override {
    getEditorId(MDescriptor);
    memcpy(classId,MEditorId,16);
    return kResultOk;
  }

  //----------

  /*
    Called before 'initialize' to set the component usage (optional).
    See IoModes
  */

  tresult PLUGIN_API setIoMode(IoMode mode) override {
    //switch (mode) {
    //  case kSimple:             VST3_Trace("(kSimple)\n"); break;
    //  case kAdvanced:           VST3_Trace("(kAdvanced)\n"); break;
    //  case kOfflineProcessing:  VST3_Trace("(kOfflineProcessing)\n"); break;
    //  default:                  VST3_Trace("(unknown)\n"); break;
    //}
    MIoMode = mode;
    return kResultOk;
  }

  //----------

  /*
    Called after the Plug-in is initialized.
    See MediaTypes, BusDirections

    enum MediaTypes {
      kAudio = 0,   // audio
      kEvent,       // events
      kNumMediaTypes
    };
    enum BusDirections {
      kInput = 0,   //input bus
      kOutput       // output bus
    };
  */

  int32 PLUGIN_API getBusCount(MediaType type, BusDirection dir) override {
    if (type == kAudio) {
      //if (dir == kInput) return MDescriptor->getNumAudioInputs();
      //else if (dir == kOutput) return MDescriptor->getNumAudioOutputs();
      return 1;
    }
    if (type == kEvent) {
      //if (dir==kOutput) return 1;
      //else
      //if (MDescriptor->canReceiveMidi()) {
      //  if (dir == kInput) return 1;
      //}
      return 1;
    }
    return 0;
  }

  //----------

  /*
    Called after the Plug-in is initialized.
    See MediaTypes, BusDirections

    struct BusInfo {
      MediaType mediaType;    // Media type - has to be a value of \ref MediaTypes
      BusDirection direction; // input or output \ref BusDirections
      int32 channelCount;     // number of channels (if used then need to be recheck after \ref IAudioProcessor::setBusArrangements is called)
      String128 name;         // name of the bus
      BusType busType;        // main or aux - has to be a value of \ref BusTypes
      uint32 flags;           // flags - a combination of \ref BusFlags
      enum BusFlags {
        kDefaultActive = 1 << 0	///< bus active per default
      };
    };
  */

  tresult PLUGIN_API getBusInfo(MediaType type, BusDirection dir, int32 index, BusInfo& bus) override {
    if (type == kAudio) {
      bus.mediaType = kAudio;
      if (dir == kInput) {
        //const clap_plugin_audio_ports_t* audio_ports = (const clap_plugin_audio_ports_t*)MPlugin->get_extension(MPlugin,CLAP_EXT_AUDIO_PORTS);
        //if (audio_ports) {
          // TODO
          bus.direction = kInput;
          bus.channelCount = 2;
          VST3_CharToUtf16("input",bus.name);
        //}
      }
      else if (dir == kOutput) {
        //SAT_AudioPort* port = MDescriptor->getAudioOutput(index);
        //bus.direction = kOutput;
        //bus.channelCount = port->num_channels;
        //VST3_CharToUtf16(port->name,bus.name);
        // TODO
        bus.direction = kOutput;
        bus.channelCount = 2;
        VST3_CharToUtf16("output",bus.name);
      }
      bus.flags = 0;//kDefaultActive;
      return kResultOk;
    }
    else if (type == kEvent) {
      bus.mediaType = kEvent;
      if (dir == kInput) {
        //bus.direction = kInput;
        //bus.channelCount = 1; // 16?
        //VST3_CharToUtf16("Midi In",bus.name);
        // debug:
        bus.direction = kInput;
        bus.channelCount = 1; // 16?
        VST3_CharToUtf16("midi",bus.name);
      }
      bus.flags = 0;//kDefaultActive;
      return kResultOk;
    }
    return kResultFalse;
  }

  //----------

  /*
    Retrieves routing information (to be implemented when more than one
    regular input or output bus exists).
    The inInfo always refers to an input bus while the returned outInfo must
    refer to an output bus!

    struct RoutingInfo
    {
      MediaType mediaType;  // media type see \ref MediaTypes
      int32 busIndex;       // bus index
      int32 channel;        // channel (-1 for all channels)
    };
  */

  tresult PLUGIN_API getRoutingInfo(RoutingInfo& inInfo, RoutingInfo& outInfo) override {
    outInfo.mediaType = inInfo.mediaType; // MediaTypes::kAudio;
    outInfo.busIndex  = inInfo.busIndex;  // 0;
    outInfo.channel   = -1;
    return kResultOk;
  }

  //----------

  /*
    Called upon (de-)activating a bus in the host application. The Plug-in
    should only processed an activated bus, the host could provide less
    AudioBusBuffers in the process call (see IAudioProcessor::process) if
    last buses are not activated
  */

  tresult PLUGIN_API activateBus(MediaType type, BusDirection dir, int32 index, TBool state) override {
    return kResultOk;
  }

  //----------

  /*
    Activates / deactivates the component.
  */

  tresult PLUGIN_API setActive(TBool state) override {
    if (state) MPlugin->activate(MSampleRate,0,MBlockSize);
    else MPlugin->deactivate();
    return kResultOk;
  }

  //----------

  /*

    How does persistence work?

    An instantiated Plug-in often has state information that must be saved in
    order to properly re-instantiate that Plug-in at a later time. A VST 3
    Plug-in has two states which are saved and reloaded: its component state
    and its controller state.
    The sequence of actions for saving is:
      component->getState (compState)
      controller->getState (ctrlState)
    The sequence of actions for loading is:
      component->setState (compState)
      controller->setComponentState (compState)
      controller->setState (ctrlState)
    In the latter sequence you can see that the controller part will receive
    the component state. This allows the 2 parts to synchronize their states.

    What's the difference between IEditController::setComponentState and
    IEditController::setState?

    After a preset is loaded, the host calls IEditController::setComponentState
    and IComponent::setState. Both delivering the same information.
    IEditController::setState is called by the host so that the Plug-in is able
    to update its controller dependent parameters, eg. position of scroll bars.
    Prior to this, there should have been a call by the host to
    IEditController::getState, where the Plug-in writes these various
    parameters into the stream.

    Can IComponent::getState()/setState() could be called during processing?

    Yes, setState and getState are called normally from the UI Thread when the
    Plug-in is used in a realtime context, in an offline context set/getState
    could be called in the same thread than the process call. Check the
    workflow diagram Audio Processor Call Sequence for more info about in which
    state which interfaces are called.
  */

  //----------

  /*
    Restore: When the states are restored, the host passes the processor state
    to both the processor and the controller (IEditController::setComponentState).
    A host must always pass that state to the processor first. The controller
    then has to synchronize its parameters to this state (but must not perform
    any IComponentHandler callbacks).
    After restoring a state, the host will rescan the parameters (asking the
    controller) in order to update its intern representation.
  */

  /*
    IBStream->read(
      void*   buffer,             // destination buffer
      int32   numBytes,           // amount of bytes to be read
      int32*  numBytesRead=0)     // how many bytes have been read from stream (set to 0 if this is of no interest)
    )

    IBStream->write(
      void*   buffer,             // destination buffer
      int32   numBytes,           // amount of bytes to be read
      int32*  numBytesWritten=0)  // how many bytes have been written to stream (set to 0 if this is of no interest)
    )

  */

  /*
    Sets complete state of component.
    //state->read(MParamValues,MNumParameters*sizeof(float));
  */

  tresult PLUGIN_API setState(IBStream* state) override {
    //uint32_t version = 0;
    //uint32_t mode = 0;
    //int32_t size = 0;
    //uint32_t num_params = 0;
    //void*    ptr = nullptr;
    //int32_t num_read = 0;
    //state->read(&version,sizeof(uint32_t),&num_read); //  SAT_Assert( num_read == sizeof(uint32_t) );
    //state->read(&mode,sizeof(uint32_t),&num_read);    //  SAT_Assert( num_read == sizeof(uint32_t) );
    //state->read(&size,sizeof(int32_t),&num_read);     //  SAT_Assert( num_read == sizeof(int32_t) );
    //switch (mode) {
    //  case 0:
    //    // is it safe to malloc/free here?
    //    ptr = malloc(size);
    //    state->read(&ptr,size,&num_read);
    //    //MPlugin->on_plugin_restoreState(size,ptr,0);
    //    free(ptr);
    //    break;
    //  case 1:
    //    num_params = MDescriptor->getNumParameters();
    //    SAT_Assert( (uint32_t)size == (num_params * sizeof(float)) );
    //    for (uint32_t i=0; i<num_params; i++) {
    //      float v = 0.f;
    //      state->read(&v,sizeof(float),&num_read);
    //      //MPlugin->setParamValue(i,v);
    //    }
    //    //MPlugin->updateAllParameters();
    //    break;
    //}
    int num_read  = 0;
    uint32_t i = 666;
    state->read(&i,sizeof(uint32_t),&num_read);
    return kResultOk;
  }

  //----------

  /*
    Retrieves complete state of component.
    //state->write(MParamValues,MNumParameters*sizeof(float));

    mode
    0 = user
    1 = simple (all params)
  */

  tresult PLUGIN_API getState(IBStream* state) override {
    //uint32_t  version = MDescriptor->getVersion();
    //uint32_t  mode    = 0;
    //void*     ptr     = nullptr;
    //uint32_t  size    = MPlugin->on_plugin_saveState(&ptr,0);
    //if ((size == 0) && (ptr == nullptr)) {
    //  ptr = MPlugin->getParamValues();
    //  size = MDescriptor->getNumParameters() * sizeof(float);
    //  mode = 1;
    //}
    //int num_written  = 0;
    //state->write(&version,sizeof(uint32_t),&num_written); //  SAT_Assert( num_written == sizeof(uint32_t) );
    //state->write(&mode,sizeof(uint32_t),&num_written);    //  SAT_Assert( num_written == sizeof(uint32_t) );
    //state->write(&size,sizeof(uint32_t),&num_written);    //  SAT_Assert( num_written == sizeof(uint32_t) );
    //state->write(ptr,size,&num_written);                  //  SAT_Assert( num_written == size );
    int num_written  = 0;
    uint32_t i = 666;
    state->write(&i,sizeof(uint32_t),&num_written);
    return kResultOk;
  }

//------------------------------
public: // IAudioProcessor
//------------------------------

  /*
    Try to set (from host) a predefined arrangement for inputs and outputs.
    The host should always deliver the same number of input and output buses
    than the Plug-in needs (see IComponent::getBusCount).
    The Plug-in returns kResultFalse if wanted arrangements are not supported.
    If the Plug-in accepts these arrangements, it should modify its buses to
    match the new arrangements
    (asked by the host with IComponent::getInfo() or
    IAudioProcessor::getBusArrangement ()) and then return kResultTrue.
    If the Plug-in does not accept these arrangements, but can adapt its
    current arrangements (according to the wanted ones), it should modify its
    buses arrangements and return kResultFalse.
  */

  //Steinberg::Vst::
  //const SpeakerArrangement kStereo = kSpeakerL | kSpeakerR;

  tresult PLUGIN_API setBusArrangements(SpeakerArrangement* inputs, int32 numIns, SpeakerArrangement* outputs, int32 numOuts) override {
    //if ((numIns == 2) && (*inputs == kStereo)) {}
    if ((numIns == 2) && (numOuts == 2)) return kResultTrue;
    return kResultFalse;
  }

  //----------

  /*
    Gets the bus arrangement for a given direction (input/output) and index.
    Note: IComponent::getInfo() and IAudioProcessor::getBusArrangement()
    should be always return the same information about the buses arrangements.
  */

  tresult PLUGIN_API getBusArrangement(BusDirection dir, int32 index, SpeakerArrangement& arr) override {
    //if ((dir==kOutput) && (index==0)) {
    //  arr = Steinberg::Vst::kSpeakerL | Steinberg::Vst::kSpeakerR;
    //  return kResultOk;
    //}
    //if (dir == kInput) {
    //  SAT_AudioPort* port = MDescriptor->getAudioInput(index);
    //  if (port) {
    //    arr = Steinberg::Vst::kSpeakerL | Steinberg::Vst::kSpeakerR;
    //    return kResultOk;
    //  }
    //}
    //else if (dir == kOutput) {
    //  SAT_AudioPort* port = MDescriptor->getAudioInput(index);
    //  if (port) {
    //    arr = Steinberg::Vst::kSpeakerL | Steinberg::Vst::kSpeakerR;
    //    return kResultOk;
    //  }
    //}
    arr = Steinberg::Vst::kSpeakerL | Steinberg::Vst::kSpeakerR;
    return kResultOk;
  }

  //----------

  /*
    Asks if a given sample size is supported.
    see SymbolicSampleSizes.

    enum SymbolicSampleSizes {
      kSample32,    // 32-bit precision
      kSample64     // 64-bit precision
    };
  */

  tresult PLUGIN_API canProcessSampleSize(int32 symbolicSampleSize) override {
    if (symbolicSampleSize==kSample32) {
      return kResultTrue;
    }
    return kResultFalse;// kNotImplemented;
  }

  //----------

  /*
    Gets the current Latency in samples.
    The returned value defines the group delay or the latency of the Plug-in.
    For example, if the Plug-in internally needs to look in advance (like
    compressors) 512 samples then this Plug-in should report 512 as latency.
    If during the use of the Plug-in this latency change, the Plug-in has to
    inform the host by using IComponentHandler::restartComponent(kLatencyChanged),
    this could lead to audio playback interruption because the host has to
    recompute its internal mixer delay compensation.
    Note that for player live recording this latency should be zero or small.
  */

  uint32 PLUGIN_API getLatencySamples() override {
    return MPlugin->tail_get();
    //return 0;
  }

  //----------

  /*
    Called in disable state (not active) before processing will begin.
  */

  /*
    enum ProcessModes {
      kRealtime,  // realtime processing
      kPrefetch,  // prefetch processing
      kOffline    // offline processing
    };
    enum SymbolicSampleSizes {
      kSample32,  // 32-bit precision
      kSample64   // 64-bit precision
    };
    struct ProcessSetup {
      int32 processMode;        // ProcessModes
      int32 symbolicSampleSize; // \ref SymbolicSampleSizes
      int32 maxSamplesPerBlock; // maximum number of samples per audio block
      SampleRate sampleRate;    // sample rate (double)
    };
  */

  tresult PLUGIN_API setupProcessing(ProcessSetup& setup) override {
    MProcessMode  = setup.sampleRate;         // kRealtime, kPrefetch, kOffline
    MSampleSize   = setup.symbolicSampleSize; // kSample32, kSample64
    MBlockSize    = setup.maxSamplesPerBlock;
    MSampleRate   = setup.sampleRate;
    //MPlugin->on_sampleRate(MSampleRate);
    return kResultOk;
  }

  //----------

  /*
    Informs the Plug-in about the processing state. This will be called
    before any process calls start with true and after with false.
    Note that setProcessing (false) may be called after setProcessing (true)
    without any process calls.
    In this call the Plug-in should do only light operation (no memory
    allocation or big setup reconfiguration), this could be used to reset
    some buffers (like Delay line or Reverb).
  */

  tresult PLUGIN_API setProcessing(TBool state) override {
    MIsProcessing = state;
    return kResultOk;
  }

  //----------

  /*
    For any audio bus defined by the Plug-in the host must provide buffer data
    - even for inactive buses. Buses are addressed by index, so leaving out
    inactive buses will mix-up these indices. The actual data buffer can be
    null though. (see AudioBusBuffers).
    Note that channelBuffers32 (or channelBuffers64) buffers pointers could be
    the same or different for input and output: this has to be take into
    account in the process function (for example not reseting the output before
    processing if input and output buffers are the same!). It could be the same
    for multiple inputs or multiple outputs (case of instrument Plug-ins) all
    outputs (or inputs) could share the same buffer!
    Important: the host could call IAudioProcessor::process without buffers
    (numInputs and numOutputs of AudioBusBuffers are zeroed, numSamples too),
    in order to flush parameters (from host to Plug-in). Parameters flush could
    happen only when the host needs to send parameter changes and no processing
    is called.
  */

  /*
    The Process call, where all information (parameter changes, event,
    audio buffer) are passed.

    struct AudioBusBuffers {
      int32   numChannels;            // number of audio channels in bus
      uint64  silenceFlags;           // Bitset of silence state per channel
      union {
        Sample32**  channelBuffers32; // sample buffers to process with 32-bit precision
        Sample64**  channelBuffers64; // sample buffers to process with 64-bit precision
      };
    };

    class IParamValueQueue {
      virtual ParamID PLUGIN_API getParameterId();
      virtual int32   PLUGIN_API getPointCount();
      virtual tresult PLUGIN_API getPoint (int32 index, int32& sampleOffset, ParamValue& value);
      virtual tresult PLUGIN_API addPoint (int32 sampleOffset, ParamValue value, int32& index);
    };

    class IParameterChanges: public FUnknown {
      virtual int32             PLUGIN_API getParameterCount();
      virtual IParamValueQueue* PLUGIN_API getParameterData(int32 index);
      virtual IParamValueQueue* PLUGIN_API addParameterData(const Vst::ParamID& id, int32& index);
    };

    class IEventList: public FUnknown {
      virtual int32   PLUGIN_API getEventCount();
      virtual tresult PLUGIN_API getEvent(int32 index, Event& e);
      virtual tresult PLUGIN_API addEvent(Event& e);
    };

    struct ProcessContext {
      // Transport state & other flags
      enum StatesAndFlags {
        kPlaying                = 1 << 1,   // currently playing
        kCycleActive            = 1 << 2,   // cycle is active
        kRecording              = 1 << 3,   // currently recording
        kSystemTimeValid        = 1 << 8,   // systemTime contains valid information
        kContTimeValid          = 1 << 17,  // continousTimeSamples contains valid information
        kProjectTimeMusicValid  = 1 << 9,   // projectTimeMusic contains valid information
        kBarPositionValid       = 1 << 11,  // barPositionMusic contains valid information
        kCycleValid             = 1 << 12,  // cycleStartMusic and barPositionMusic contain valid information
        kTempoValid             = 1 << 10,  // tempo contains valid information
        kTimeSigValid           = 1 << 13,  // timeSigNumerator and timeSigDenominator contain valid information
        kChordValid             = 1 << 18,  // chord contains valid information
        kSmpteValid             = 1 << 14,  // smpteOffset and frameRate contain valid information
        kClockValid             = 1 << 15   // samplesToNextClock valid
      };
      uint32        state;                // a combination of the values from \ref StatesAndFlags
      double        sampleRate;           // current sample rate (always valid)
      TSamples      projectTimeSamples;   // project time in samples (always valid)
      int64         systemTime;           // system time in nanoseconds (optional)
      TSamples      continousTimeSamples; // project time, without loop (optional)
      TQuarterNotes projectTimeMusic;     // musical position in quarter notes (1.0 equals 1 quarter note)
      TQuarterNotes barPositionMusic;     // last bar start position, in quarter notes
      TQuarterNotes cycleStartMusic;      // cycle start in quarter notes
      TQuarterNotes cycleEndMusic;        // cycle end in quarter notes
      double        tempo;                // tempo in BPM (Beats Per Minute)
      int32         timeSigNumerator;     // time signature numerator (e.g. 3 for 3/4)
      int32         timeSigDenominator;   // time signature denominator (e.g. 4 for 3/4)
      Chord         chord;                // musical info
      int32         smpteOffsetSubframes; // SMPTE (sync) offset in subframes (1/80 of frame)
      FrameRate     frameRate;            // frame rate
      int32         samplesToNextClock;   // MIDI Clock Resolution (24 Per Quarter Note), can be negative (nearest)
    };

    struct ProcessData {
      int32               processMode;            // processing mode - value of \ref ProcessModes
      int32               symbolicSampleSize;     // sample size - value of \ref SymbolicSampleSizes
      int32               numSamples;             // number of samples to process
      int32               numInputs;              // number of audio input buses
      int32               numOutputs;             // number of audio output buses
      AudioBusBuffers*    inputs;                 // buffers of input buses
      AudioBusBuffers*    outputs;                // buffers of output buses
      IParameterChanges*  inputParameterChanges;	// incoming parameter changes for this block
      IParameterChanges*  outputParameterChanges; // outgoing parameter changes for this block (optional)
      IEventList*         inputEvents;            // incoming events for this block (optional)
      IEventList*         outputEvents;           // outgoing events for this block (optional)
      ProcessContext*     processContext;         // processing context (optional, but most welcome)
    };

  */

  /*
    VST3_SDK/doc/vstsdk/faq.html#faqCommunication6
    Q: How my parameter changes (from UI interaction) are send to the
    processor if the host does not process?
    When a parameter is changed in the Plug-in UI by user action, the plug
    sends this change to the host with performEdit (do not forget to call
    beginEdit and endEdit), then the host has the responsibility to transfer
    this parameter change to the processor part: => if the audio engine is
    running (playing) this will be done in the next available process call =>
    if the audio engine is not running, the host has to flush parameter
    changes from time to time by sending them to processor by calling process
    (with audio buffer set to null), in this case the Plug-in should only
    update the parameters changes without processing any audio. This is very
    important that the host supports this flush mechanism else by saving
    Plug-ins state (project/preset) the host will not get the correct updated
    one.

    VST3_SDK/doc/vstinterfaces/index.html#vst3Editing
    Any change to a parameter that is caused by user interaction in the
    Plug-in GUI must be properly reported to the Steinberg::Vst::
    IComponentHandler. The host is responsible for transmitting the change to
    the processor. In order to make recording of automation work accordingly
    it is necessary to call beginEdit, performEdit and endEdit in the
    expected order!
    When the controller transmits a parameter change to the host, the host
    synchronizes the processor by passing the new values as Steinberg::Vst::
    IParameterChanges to the process call.
  */

  /*
    http://www.noelborthwick.com/2014/01/22/developer-notes-sonar-x3-vst3-internals/
    The host may also send parameters to the plugin when no processing is
    active via a special “flush parameters” process call. This happens when
    the audio engine has been stopped in SONAR and the user manipulates an
    envelope or the user interface widget to change a plugin parameter.
    In this call numInputs and numOutputs of Steinberg::Vst::AudioBusBuffers
    are zeroed as well as numSamples. Plugin’s should expect and properly
    handle this call.
  */

  // assume only 1 input/output bus..

  tresult PLUGIN_API process(ProcessData& data) override {
    MNumEvents = 0;
    prepareEvents(data);
    prepareParameters(data);
    sortEvents();
    bool _flush = ( (data.numInputs == 0) && (data.numOutputs == 0) && (data.numSamples == 0) );
    if (!_flush) {

      MAudioInputs.data32               = data.inputs[0].channelBuffers32;//MAudioInputBuffers;
      MAudioInputs.data64               = data.inputs[0].channelBuffers64;//MAudioInputBuffers_64;
      MAudioInputs.channel_count        = 2;
      MAudioInputs.latency              = 0;
      MAudioInputs.constant_mask        = 0;

      MAudioOutputs.data32              = data.outputs[0].channelBuffers32;//MAudioOutputBuffers;
      MAudioOutputs.data64              = data.outputs[0].channelBuffers64;//MAudioOutputBuffers_64;
      MAudioOutputs.channel_count       = 2;
      MAudioOutputs.latency             = 0;
      MAudioOutputs.constant_mask       = 0;

      MTransport.header.type            = CLAP_EVENT_TRANSPORT;
      MTransport.header.size            = sizeof(clap_event_transport_t);
      MTransport.header.space_id        = CLAP_CORE_EVENT_SPACE_ID;
      MTransport.header.time            = 0;
      MTransport.header.flags           = 0;

      MTransport.flags = 0;
      if (data.processContext->state & ProcessContext::StatesAndFlags::kTempoValid)             MTransport.flags |= CLAP_TRANSPORT_HAS_TEMPO;
      if (data.processContext->state & ProcessContext::StatesAndFlags::kBarPositionValid)       MTransport.flags |= CLAP_TRANSPORT_HAS_BEATS_TIMELINE;
      if (data.processContext->state & ProcessContext::StatesAndFlags::kProjectTimeMusicValid)  MTransport.flags |= CLAP_TRANSPORT_HAS_SECONDS_TIMELINE;
      if (data.processContext->state & ProcessContext::StatesAndFlags::kTimeSigValid)           MTransport.flags |= CLAP_TRANSPORT_HAS_TIME_SIGNATURE;
      if (data.processContext->state & ProcessContext::StatesAndFlags::kPlaying)                MTransport.flags |= CLAP_TRANSPORT_IS_PLAYING;
      if (data.processContext->state & ProcessContext::StatesAndFlags::kRecording)              MTransport.flags |= CLAP_TRANSPORT_IS_RECORDING;
      if (data.processContext->state & ProcessContext::StatesAndFlags::kCycleActive)            MTransport.flags |= CLAP_TRANSPORT_IS_LOOP_ACTIVE;
      //if (data.processContext->state & ProcessContext::StatesAndFlags::)                        MTransport.flags |= CLAP_TRANSPORT_IS_WITHIN_PRE_ROLL;

      MTransport.song_pos_beats         = data.processContext->projectTimeMusic;
      MTransport.song_pos_seconds       = data.processContext->continousTimeSamples;
      MTransport.tempo                  = data.processContext->tempo;
      MTransport.tempo_inc              = 0.0;  // hz/srate
      MTransport.loop_start_beats       = data.processContext->cycleStartMusic;
      MTransport.loop_end_beats         = data.processContext->cycleEndMusic;
      MTransport.loop_start_seconds     = 0;
      MTransport.loop_end_seconds       = 0;
      MTransport.bar_start              = 0;
      MTransport.bar_number             = 0;//data.processContext->barPositionMusic;
      MTransport.tsig_num               = data.processContext->timeSigNumerator;
      MTransport.tsig_denom             = data.processContext->timeSigDenominator;

      MClapProcess.steady_time          = MSteadyTime;
      MClapProcess.frames_count         = data.numSamples;
      MClapProcess.transport            = &MTransport;
      MClapProcess.audio_inputs         = &MAudioInputs;
      MClapProcess.audio_outputs        = &MAudioOutputs;
      MClapProcess.audio_inputs_count   = 2;
      MClapProcess.audio_outputs_count  = 2;
      MClapProcess.in_events            = &MVst3InputEvents;
      MClapProcess.out_events           = &MVst3OutputEvents;

      MPlugin->process(&MClapProcess);
      MSteadyTime += data.numSamples;

    }

    //else {
    //  // flush
    //  SAT_Print("flusah\n");
    //}

    // https://forum.juce.com/t/vst3-plugin-wrapper/12323/5
    // I recall the Steinberg Validator complaining that process() should just
    // return kResultTrue...
    return kResultOk; // = kResulttrue (0)
  }

  //----------

  /*
    Gets tail size in samples. For example, if the Plug-in is a Reverb
    Plug-in and it knows that the maximum length of the Reverb is 2sec, then
    it has to return in getTailSamples()
    (in VST2 it was getGetTailSize()) : 2*sampleRate.
    This information could be used by host for offline processing, process
    optimization and downmix (avoiding signal cut (clicks)).
    It should return:
    - kNoTail when no tail
    - x * sampleRate when x Sec tail.
    - kInfiniteTail when infinite tail.
  */

  uint32 PLUGIN_API getTailSamples() override {
    return kNoTail;
  }

//------------------------------
public: // IMidiMapping
//------------------------------

  /*
    busIndex              index of Input Event Bus
    channel               channel of the bus
    midiControllerNumber  see \ref ControllerNumbers for expected values (could be bigger than 127)
    id                    return the associated ParamID to the given midiControllerNumber

    see ivstmidicontrollers.h

    kCtrlModWheel
    kCtrlFilterResonance (cc74) // Filter Resonance (Brightness)
    kAfterTouch
    kPitchBend

  */

  /*
    todo:
    ParamId, bitmask..
    0x0000.0x0000 'normal' parameter
    0x0001.0x0000 midi
    etc..
  */

  tresult PLUGIN_API getMidiControllerAssignment(int32 busIndex, int16 channel, CtrlNumber midiControllerNumber, ParamID& id) override {
    //if (busIndex == 0) {
    //  switch (midiControllerNumber) {
    //    case kAfterTouch: // 128
    //      id = SAT_PLUGIN_VST3_PARAM_AFTERTOUCH + channel;
    //      return kResultOk;
    //    case kPitchBend: // 129
    //      id = SAT_PLUGIN_VST3_PARAM_PITCHBEND + channel;
    //      return kResultOk;
    //    case kCtrlFilterResonance: // cc 74 (slide)
    //      id = SAT_PLUGIN_VST3_PARAM_BRIGHTNESS + channel;
    //      return kResultOk;
    //  }
    //}
    return kResultFalse;
  }

//------------------------------
public: // INoteExpressionController
//------------------------------

  // Returns number of supported note change types for event bus index and channel.

  int32 PLUGIN_API getNoteExpressionCount(int32 busIndex, int16 channel) override {
    //if (busIndex==0) return 1;
    return 2;
  }

  //----------

  //flags: NoteExpressionTypeInfo::kIsBipolar, kIsOneShot, kIsAbsolute, kAssociatedParameterIDValid

  tresult PLUGIN_API getNoteExpressionInfo(int32 busIndex, int16 channel, int32 noteExpressionIndex, NoteExpressionTypeInfo& info) override {
    //if (busIndex==0) {
      switch(noteExpressionIndex) {
        case 0:
          info.typeId                 = 0; // ??
          VST3_CharToUtf16("Tuning",info.title);
          VST3_CharToUtf16("Tun",info.shortTitle);
          VST3_CharToUtf16("",info.units);
          info.unitId                 = 0;
          info.valueDesc.defaultValue = 0.0;
          info.valueDesc.minimum      = 0;
          info.valueDesc.maximum      = 1;
          info.associatedParameterId  = -1;
          info.flags                  = NoteExpressionTypeInfo::kIsBipolar;
          return kResultOk;
        case 1:
          info.typeId                 = 1; // ??
          VST3_CharToUtf16("Brightness",info.title);
          VST3_CharToUtf16("Bri",info.shortTitle);
          VST3_CharToUtf16("",info.units);
          info.unitId                 = 0;
          info.valueDesc.defaultValue = 0.0;
          info.valueDesc.minimum      = 0;
          info.valueDesc.maximum      = 1;
          info.associatedParameterId  = -1;
          info.flags                  = 0;
          return kResultOk;
      }
    //}
    return kResultFalse;
  }

  //----------

  // Gets a user readable representation of the normalized note change value.

  tresult PLUGIN_API getNoteExpressionStringByValue(int32 busIndex, int16 channel, NoteExpressionTypeID id, NoteExpressionValue valueNormalized, String128 string) override {
    char temp[100];
    //SAT_FloatToString(temp,valueNormalized);
    sprintf(temp,"%.3f",valueNormalized);
    VST3_CharToUtf16(temp,string);
    return kResultOk;
  }

  //----------

  // Converts the user readable representation to the normalized note change value.

  tresult PLUGIN_API getNoteExpressionValueByString(int32 busIndex, int16 channel, NoteExpressionTypeID id, const TChar* string, NoteExpressionValue& valueNormalized) override{
    char temp[129];
    VST3_Utf16ToChar(string,temp);
    //float res = SAT_StringToFloat(temp);
    float res = atof(temp);
    valueNormalized = res;
    return kResultOk;
  }

//------------------------------
public: // IKeyswitchController
//------------------------------

  // file:///WORK/code/VST3_SDK/doc/vstinterfaces/keyswitch.html

  /*
    When a (instrument) Plugin supports such interface, the host could get
    from the Plug-in the current set of used key switches (megatrig/
    articulation) for a given channel of a event bus and then automatically use
    them (like in Cubase 6) to create VST Expression Map (allowing to
    associated symbol to a given articulation / key switch).
  */

  //----------

  // Returns number of supported key switches for event bus index and channel.

  int32 PLUGIN_API getKeyswitchCount(int32 busIndex, int16 channel) override {
    return 0;
  }

  //----------

  /*
  struct KeyswitchInfo {
    KeyswitchTypeID typeId; // see KeyswitchTypeID
    String128 title;        // name of key switch (e.g. "Accentuation")
    String128 shortTitle;   // short title (e.g. "Acc")
    int32 keyswitchMin;     // associated main key switch min (value between [0, 127])
    int32 keyswitchMax;     // associated main key switch max (value between [0, 127])
    int32 keyRemapped;      // optional remapped key switch (default -1), the Plug-in could provide one remapped key for a key switch (allowing better location on the keyboard of the key switches)
    int32 unitId;           // id of unit this key switch belongs to (see \ref vst3UnitsIntro), -1 means no unit used.
    int32 flags;            // not yet used (set to 0)
  };
  */

  // Returns key switch info.

  tresult PLUGIN_API getKeyswitchInfo(int32 busIndex, int16 channel, int32 keySwitchIndex, KeyswitchInfo& info) override {
    return kResultFalse;
  }

//------------------------------
public: // IConnectionPoint
//------------------------------

  // file:///WORK/code/VST3_SDK/doc/vstinterfaces/index.html#vst3Communication

  /*
    This interface is used for the communication of separate components.
    Note that some hosts will place a proxy object between the components so
    that they are not directly connected.
  */

  //----------

  // Connects this instance with another connection point.

  /*
    class IConnectionPoint: public FUnknown {
    public:
      virtual tresult PLUGIN_API connect (IConnectionPoint* other) = 0;
      virtual tresult PLUGIN_API disconnect (IConnectionPoint* other) = 0;
      virtual tresult PLUGIN_API notify (IMessage* message) = 0;
      static const FUID iid;
    };
  */

  tresult PLUGIN_API connect(IConnectionPoint* other) override {
    //IMessage* msg;
    //msg->setMessageID("test");
    //other->notify(msg);
    return kResultFalse;
  }

  //----------

  //Disconnects a given connection point from this.

  tresult PLUGIN_API disconnect(IConnectionPoint* other)  override {
    return kResultFalse;
  }

  //----------

  // Called when a message has been sent from the connection point to this.

  /*
  class IMessage: public FUnknown {
  public:
    virtual FIDString PLUGIN_API getMessageID () = 0;
    virtual void PLUGIN_API setMessageID (FIDString id) = 0;
    virtual IAttributeList* PLUGIN_API getAttributes () = 0;
    static const FUID iid;
  };
  */

  tresult PLUGIN_API notify(IMessage* message) override {
    return kResultFalse;
  }

//------------------------------
public: // IUnitInfo
//------------------------------

  // file:///WORK/code/VST3_SDK/doc/vstinterfaces/vst3Units.html

  /*
    IUnitInfo describes the internal structure of the Plug-in.
    - The root unit is the component itself, so getUnitCount must return 1 at least.
    - The root unit id has to be 0 (kRootUnitId).
    - Each unit can reference one program list - this reference must not change.
    - Each unit using a program list, references one program of the list.
  */

  //----------

  // Returns the flat count of units.

  int32 PLUGIN_API getUnitCount() override {
    return 1;
  }

  //----------

  // Gets UnitInfo for a given index in the flat list of unit.

  /*
  struct UnitInfo {
    UnitID id;                    // unit identifier
    UnitID parentUnitId;          // identifier of parent unit (kNoParentUnitId: does not apply, this unit is the root)
    String128 name;               // name, optional for the root component, required otherwise
    ProgramListID programListId;  // id of program list used in unit (kNoProgramListId = no programs used in this unit)
  };
  */

  tresult PLUGIN_API getUnitInfo(int32 unitIndex, UnitInfo& info) override {
    if (unitIndex==0) {
      info.id = kRootUnitId; //0;
      info.parentUnitId = kNoParentUnitId;
      VST3_CharToUtf16("root",info.name);
      info.programListId = kNoProgramListId;
      return kResultOk;
    }
    return kResultFalse;
  }

  //----------

  // Gets the count of Program List.

  int32 PLUGIN_API getProgramListCount() override {
    return 0; // 1
  }

  //----------

  // Gets for a given index the Program List Info.

  /*
  struct ProgramListInfo {
    ProgramListID id;     // program list identifier
    String128 name;       // name of program list
    int32 programCount;   // number of programs in this list
  };
  */

  tresult PLUGIN_API getProgramListInfo(int32 listIndex, ProgramListInfo& info) override {
    if (listIndex == 0) {
      info.id = 0;
      VST3_CharToUtf16("program",info.name);
      info.programCount = 1;
      return kResultOk;
    }
    return kResultFalse;
  }

  //----------

  // Gets for a given program list ID and program index its program name.

  tresult PLUGIN_API getProgramName(ProgramListID listId, int32 programIndex, String128 name) override {
    if ((listId == 0) && (programIndex == 0)) {
      VST3_CharToUtf16("program",name);
      return kResultOk;
    }
    return kResultFalse;
  }

  //----------

  /*
    Gets for a given program list ID, program index and attributeId the
    associated attribute value.
  */

  //CString attributeId, String128 attributeValue) {
  tresult PLUGIN_API getProgramInfo(ProgramListID listId, int32 programIndex, Steinberg::Vst::CString attributeId, String128 attributeValue) override {
    ////attributeId = "";
    //if ((listId == 0) && (programIndex == 0) /* attributeId */) {
    //  char_to_utf16("",attributeValue);
    //  return kResultOk;
    //}
    return kResultFalse;
  }

  //----------

  // Returns kResultTrue if the given program index of a given program list ID supports PitchNames.

  tresult PLUGIN_API hasProgramPitchNames(ProgramListID listId, int32 programIndex) override {
    return kResultFalse;
  }

  //----------

  /*
    Gets the PitchName for a given program list ID, program index and pitch.
    If PitchNames are changed the Plug-in should inform the host with
    IUnitHandler::notifyProgramListChange.
  */

  tresult PLUGIN_API getProgramPitchName(ProgramListID listId, int32 programIndex, int16 midiPitch, String128 name) override {
    //char_to_utf16("pitch",name);
    return kResultFalse;
  }

  //----------

  // Gets the current selected unit.

  UnitID PLUGIN_API getSelectedUnit() override {
    return 0;
  }

  //----------

  // Sets a new selected unit.

  tresult PLUGIN_API selectUnit(UnitID unitId) override {
    return kResultOk;
  }

  //----------

  /*
    Gets the according unit if there is an unambiguous relation between a
    channel or a bus and a unit.
    This method mainly is intended to find out which unit is related to a
    given MIDI input channel.
  */

  tresult PLUGIN_API getUnitByBus(MediaType type, BusDirection dir, int32 busIndex, int32 channel, UnitID& unitId) override {
    unitId = 0;
    return kResultOk;//False;
  }

  //----------

  /*
    Receives a preset data stream.
    - If the component supports program list data (IProgramListData), the destination of the data
      stream is the program specified by list-Id and program index (first and second parameter)
    - If the component supports unit data (IUnitData), the destination is the unit specified by the first
      parameter - in this case parameter programIndex is < 0).
  */

  tresult PLUGIN_API setUnitProgramData(int32 listOrUnitId, int32 programIndex, IBStream* data) override {
    return kResultFalse;
  }

//------------------------------
public: // IEditController
//------------------------------

  // file:///WORK/code/VST3_SDK/doc/vstinterfaces/vst3Automation.html

  //----------

  /*
    // https://forum.juce.com/t/automation-issue-in-cubase-nuendo/14566/5
    // Cubase and Nuendo need to inform the host of the current parameter values
    if (AudioProcessor* const pluginInstance = getPluginInstance()) {
      for (int i = 0; i < pluginInstance->getNumParameters(); ++i)
        setParamNormalized((Vst::ParamID) i, (double) pluginInstance->getParameter (i));
    }
    return kResultFalse; // false?
  */

  tresult PLUGIN_API setComponentState(IBStream* state) override {
    /*
    // we receive the current state of the component (processor part)
    if (state == nullptr) return kResultFalse;
    // using helper to read the stream
    IBStreamer streamer(state, kLittleEndian);
    // ABSwitchParamID::kAudioSwitch
    float savedParam1 = 0.f;
    if (!streamer.readFloat(savedParam1)) return kResultFalse;
    setParamNormalized(ABSwitchParamID::kAudioSwitch, savedParam1);
    return kResultOk;
    */
    //return kResultOk;//False;
    /*
      https://forum.juce.com/t/possible-demo-plugin-bug/19131/9
      "..which in turn returns kNotImplemented, should you not be returning
      kResultOk here?"
      "That is intended. I can’t remember why but some DAWs needed that."

    */
    /*
    int num_read = 0;
    uint32_t test_int = 0;
    state->read(&test_int,4,&num_read);
    */
    // MEditor->updateParameterFromHost(param,index,value);
    return kResultOk;
  }

  //----------

  /*
    stores and restores any GUI settings that are not related to the processor
    (like scroll positions etc)

    When the states are restored, the host passes the processor state to both
    the processor and the controller (IEditController::setComponentState).
    A host must always pass that state to the processor first. The controller
    then has to synchronize its parameters to this state (but must not perform
    any IComponentHandler callbacks).
    After restoring a state, the host will rescan the parameters (asking the
    controller) in order to update its intern representation.
  */

  //tresult PLUGIN_API setState(IBStream* state) {
  tresult PLUGIN_API setEditorState(IBStream* state) override {
    return kResultOk;
  }

  //----------

  //tresult PLUGIN_API getState(IBStream* state) {
  tresult PLUGIN_API getEditorState(IBStream* state) override {
    return kResultOk;
  }

  //----------

  /*
    called repeatedly by bitwig, even if plugin is deactivated
  */

  int32 PLUGIN_API getParameterCount() override {
    return MPlugin->params_count();
    //return 0;
  }

  //----------

  tresult PLUGIN_API getParameterInfo(int32 paramIndex, ParameterInfo& info) override {
    if (paramIndex < 0) return kResultFalse;

    int32_t num_params = MPlugin->params_count();
    if (paramIndex < num_params) {
      //SAT_Parameter* param = MDescriptor->getParameter(paramIndex);
      //if (param) {
        memcpy(&info,&MParamInfos[paramIndex],sizeof(ParameterInfo));
        return kResultOk;
      //}
    } // index < numparams
    else { // > # params
      //SAT_Trace("paramIndex (%08x) >= num.params\n",paramIndex);
      switch (paramIndex) {
        case kAfterTouch: // 128
          break;
        case kPitchBend: // 129
          break;
        case kCtrlFilterResonance: // cc 74 (slide)
          break;
      }
      switch (paramIndex & 0xffff0000) {
        case SAT_PLUGIN_VST3_PARAM_AFTERTOUCH:
          break;
        case SAT_PLUGIN_VST3_PARAM_PITCHBEND:
          break;
        case SAT_PLUGIN_VST3_PARAM_BRIGHTNESS:
          break;
      }
      //return kResultOk;
      return kResultFalse;
    } // > #params
    return kResultFalse;
  }

  //----------

  tresult PLUGIN_API getParamStringByValue(ParamID id, ParamValue valueNormalized, String128 string) override {
    if (id < MPlugin->params_count()) {
      //SAT_Parameter* param = MDescriptor->getParameter(id);
      char buffer[32]; // ???
      //param->getDisplayText(valueNormalized,buffer);
      //param->displayText(buffer,valueNormalized);
      double v = denormalizeParameter(id,valueNormalized);
      sprintf(buffer,"%.3f",v);
      VST3_CharToUtf16(buffer,string);
      return kResultOk;
    }
    else {
      return kResultFalse;
    }
  }

  //----------

  tresult PLUGIN_API getParamValueByString(ParamID id, TChar* string, ParamValue& valueNormalized) override {
    if (id < MPlugin->params_count()) {
      //SAT_Parameter* param = MDescriptor->getParameter(id);
      char temp[129];
      //VST3_CharToUtf16(string,temp);
      VST3_Utf16ToChar(string,temp);
      double v = atoi(temp);
      //float v2 = param->to01(v);
      valueNormalized = denormalizeParameter(id,v);
      return kResultOk;
    }
    else {
      return kResultFalse;
    }
  }

  //----------

  ParamValue PLUGIN_API normalizedParamToPlain(ParamID id, ParamValue valueNormalized) override {
    if (id < MPlugin->params_count()) {
      //SAT_Parameter* param = MDescriptor->getParameter(id);
      //float v = param->from01(valueNormalized);
      //return v;
      return denormalizeParameter(id,valueNormalized);
    }
    else {
      return valueNormalized;
    }
  }

  //----------

  ParamValue PLUGIN_API plainParamToNormalized(ParamID id, ParamValue plainValue) override {
    if (id < MPlugin->params_count()) {
      //SAT_Parameter* param = MDescriptor->getParameter(id);
      //float v = param->to01(plainValue);
      //return v;
      return normalizeParameter(id,plainValue);
    }
    else {
      return plainValue;
    }
  }

  //----------

  ParamValue PLUGIN_API getParamNormalized(ParamID id) override {
    if (id < MPlugin->params_count()) {
      //float v = MPlugin->getParamValue(id);
      SAT_Plugin* plugin = (SAT_Plugin*)MPlugin;
      SAT_Parameter* param = plugin->getParameter(id);
      double v = param->getValue();

      //float v = 0;
      return param->normalizeValue(v);
    }
    else {
      return 0;
    }
  }

  //----------

  /*
    The host is responsible for updating the Plug-in GUI when automation data
    is transmitted to the processor. This is realized by frequent calls of
    Steinberg::Vst::IEditController::setParamNormalized.
  */

  /*
    Sets the normalized value to the parameter associated to the paramID.
    The controller must never pass this value-change back to the host via the
    IComponentHandler. It should update the according GUI element(s) only!
  */

  // bitwig sends a ParamID = 0x3f800000

  tresult PLUGIN_API setParamNormalized(ParamID id, ParamValue value) override {
    SAT_Print("%i = %.3f\n",id,value);
    //if (id >= MDescriptor->getNumParameters()) {
    //  return kResultFalse; // ???
    //}
    ////MPlugin->setParamValue(id,value);
    ////MEditorValues[id] = value;
    //#ifndef SAT_NO_GUI
    //if (MEditor) {
    //  MEditor->setParameterValueAndRedraw(id,value);
    //}
    //#endif // SAT_NO_GUI
    //SAT_Parameter* parameter = MDescriptor->getParameter(id);
    //float v = parameter->from01(value);
    //on_updateParameter(id,v);
    return kResultOk;
  }

  //----------

  /*
    Gets from host a handler.
    aka host.. beginEdit, performEdit, endEdit, restartComponent
  */

  /*
  class IComponentHandler: public FUnknown {
  public:
    virtual tresult PLUGIN_API beginEdit (ParamID id) = 0;
    virtual tresult PLUGIN_API performEdit (ParamID id, ParamValue valueNormalized) = 0;
    virtual tresult PLUGIN_API endEdit (ParamID id) = 0;
    virtual tresult PLUGIN_API restartComponent (int32 flags) = 0;
    static const FUID iid;
  };

  notifyHostParameterChanged
    MComponentHandler->begin/perform/endEdit

  */

  tresult PLUGIN_API setComponentHandler(IComponentHandler* handler) override {
    MComponentHandler = handler;
    //handler->queryInterface(IComponentHandler2::iid,(void**)&MComponentHandler2);
    //SAT_Vst3Host* host = (SAT_Vst3Host*)MPlugin->getHost();
    //if (host) host->setComponentHandler(MComponentHandler,MComponentHandler2);
    return kResultOk;
  }

  //----------

  /*
    Creates the editor view of the Plug-in, currently only "editor" is
    supported, see ViewType.
    The life time of the editor view will never exceed the life time of
    this controller instance.
  */

  IPlugView* PLUGIN_API createView(FIDString name) override {
    //if (MDescriptor->hasEditor()) {
      if (name && (strcmp(name,ViewType::kEditor) == 0)) {
        addRef();
        return (IPlugView*)this;
      }
    //}
    return nullptr;
  }

//------------------------------
public: // IEditController2
//------------------------------

  tresult PLUGIN_API setKnobMode(KnobMode mode) override {
    return kResultFalse;
  }

  //----------

  tresult PLUGIN_API openHelp(TBool onlyCheck) override {
    return kResultFalse;
  }

  //----------

  tresult PLUGIN_API openAboutBox(TBool onlyCheck) override {
    return kResultFalse;
  }

//------------------------------
public: // IPlugView
//------------------------------

  const char* getEditorId(const clap_plugin_descriptor_t* descriptor) {
    uint32_t* id = (uint32_t*)MEditorId;
    id[0] = SAT_MAGIC_EDITOR;
    id[1] = SAT_HashString(descriptor->name);
    id[2] = SAT_HashString(descriptor->vendor);
    id[3] = SAT_HashString(descriptor->version);
    return (const char*)id;
  }

  //--------------------

  tresult PLUGIN_API isPlatformTypeSupported(FIDString type) override {
    // "X11EmbedWindowID"

    #ifdef SAT_LINUX
      if (type && strcmp(type,kPlatformTypeX11EmbedWindowID) == 0) return kResultOk;
    #endif

    #ifdef SAT_WIN32
      if (type && strcmp(type,kPlatformTypeHWND) == 0) return kResultOk;
    #endif

    return kResultFalse;
  }

  //----------

  /*
    The parent window of the view has been created, the (platform)
    representation of the view should now be created as well.

    Note that the parent is owned by the caller and you are not allowed
    to alter it in any way other than adding your own views. Note that
    in this call the Plug-in could call a IPlugFrame::resizeView ()!

    Parameters
      parent  : platform handle of the parent window or view
      type    : Platform UI Types which should be created
  */

  tresult PLUGIN_API attached(void* parent, FIDString type) override {

    SAT_PRINT;

    const clap_plugin_gui_t* gui = (const clap_plugin_gui_t*)MPlugin->get_extension(CLAP_EXT_GUI);
    const clap_plugin_t* plugin = MPlugin->getPlugin();

    SAT_PRINT;

    #ifdef SAT_LINUX
    if (gui && gui->is_api_supported(plugin,CLAP_WINDOW_API_X11,false)) {
      if (gui->create(plugin,CLAP_WINDOW_API_X11,false)) {
    #endif

    SAT_PRINT;

    #ifdef SAT_WIN32
    if (gui && gui->is_api_supported(plugin,CLAP_WINDOW_API_WIN32,false)) {
      if (gui->create(plugin,CLAP_WINDOW_API_WIN32,false)) {
    #endif

    SAT_PRINT;

        gui->set_scale(plugin,1.0);
        uint32_t width = 0;
        uint32_t height = 0;
        gui->get_size(plugin,&width,&height);
        if (MPlugFrame) {
          ViewRect r;
          r.left    = 0;
          r.top     = 0;
          r.right   = width;
          r.bottom  = height;
          MPlugFrame->resizeView(this,&r);
        }
        clap_window_t clap_window = {};

        #ifdef SAT_LINUX
          clap_window.api = CLAP_WINDOW_API_X11;
          clap_window.x11 = (clap_xwnd)parent;
        #endif

        #ifdef SAT_WIN32
          clap_window.api = CLAP_WINDOW_API_WIN32;
          clap_window.win32 = (clap_hwnd)parent;
        #endif

        gui->set_size(plugin,width,height);
        gui->set_parent(plugin,&clap_window);
        gui->show(plugin);

        #ifdef SAT_LINUX
          MRunLoop->registerTimer(this,SAT_PLUGIN_VST3_TIMER_MS);
        #endif
        // why not windows ???
      }
    }
    return kResultOk;
    //return kResultFalse;
  }

  //----------

  /*
    The parent window of the view is about to be destroyed.
    You have to remove all your own views from the parent window or view.
  */

  tresult PLUGIN_API removed() override {
    const clap_plugin_gui_t* gui = (const clap_plugin_gui_t*)MPlugin->get_extension(CLAP_EXT_GUI);
    const clap_plugin_t* plugin = MPlugin->getPlugin();
    if (gui) {
      gui->hide(plugin);
      gui->destroy(plugin);

      #ifdef SAT_LINUX
        MRunLoop->unregisterTimer(this);
      #endif
      // why not windows ???
    }
    return kResultOk;
  }

  //----------

  tresult PLUGIN_API onWheel(float distance) override {
    return kResultOk;
  }

  //----------

  tresult PLUGIN_API onKeyDown(char16 key, int16 keyCode, int16 modifiers) override {
    return kResultOk;
  }

  //----------

  tresult PLUGIN_API onKeyUp(char16 key, int16 keyCode, int16 modifiers) override {
    return kResultOk;
  }

  //----------

  /*
    Returns the size of the platform representation of the view.
  */

  tresult PLUGIN_API getSize(ViewRect* size) override {
    uint32_t width = 0;
    uint32_t height = 0;
    const clap_plugin_gui_t* gui = (const clap_plugin_gui_t*)MPlugin->get_extension(CLAP_EXT_GUI);
    const clap_plugin_t* plugin = MPlugin->getPlugin();
    //SAT_Editor* editor = MPlugin->getEditor();
    gui->get_size(plugin,&width,&height);
    size->left    = 0;
    size->top     = 0;
    size->right   = width;//MDescriptor->getEditorWidth();
    size->bottom  = height;//MDescriptor->getEditorHeight();
    return kResultOk;
  }

  //----------

  /*
    Resizes the platform representation of the view to the given rect.
    Note that if the Plug-in requests a resize (IPlugFrame::resizeView())
    onSize has to be called afterward.
  */

  tresult PLUGIN_API onSize(ViewRect* newSize) override {
    //MEditorWidth = newSize->getWidth();
    //MEditorHeight = newSize->getHeight();
    //TODO: resize/redraw editor
    uint32_t w = newSize->getWidth();
    uint32_t h = newSize->getHeight();
    if (MPlugin) MPlugin->gui_set_size(w,h);
    return kResultOk;
  }

  //----------

  /*
    Focus changed message.
  */

  tresult PLUGIN_API onFocus(TBool state) override {
    return kResultOk;
  }

  //----------

  /*
    Sets IPlugFrame object to allow the Plug-in to inform the host
    about resizing.
  */

  tresult PLUGIN_API setFrame(IPlugFrame* frame) override {
    MPlugFrame = frame;
    #ifdef SAT_LINUX
      //tresult res =
      MPlugFrame->queryInterface(IRunLoop_iid, (void**)&MRunLoop);
    #endif
    return kResultOk;
  }

  //----------

  /*
    Is view sizable by user?
  */

  tresult PLUGIN_API canResize() override {
    if (MPlugin->get_extension(CLAP_EXT_GUI)) return kResultTrue;
    else return kResultFalse;
  }

  //----------

  /*
    On live resize this is called to check if the view can be resized to the
    given rect, if not adjust the rect to the allowed size.
  */

  tresult PLUGIN_API checkSizeConstraint(ViewRect* rect) override {
    return kResultOk;//False;
  }

//------------------------------
public: // ITimerHandler
//------------------------------

  // tid = same as setComponentHandler..

  /*
    will be called afte we regisdter the timer:
      MRunLoop->registerTimer(this,SAT_PLUGIN_VST3_TIMER_MS);
    (in IEditView.attached)
  */

  #ifdef SAT_LINUX
    void onTimer() override {
      //MPlugin->on_updateEditor(MEditor);
      //MPlugin->flushParamsToHost();
      //SAT_PRINT;
      flushHostParams();
    }
  #endif

  #ifdef SAT_WIN32
    //void onTimer() override {
    //}
  #endif

//

};

//----------------------------------------------------------------------
#endif


