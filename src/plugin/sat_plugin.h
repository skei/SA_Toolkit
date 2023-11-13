#ifndef sat_plugin_included
#define sat_plugin_included
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/clap/sat_clap.h"
#include "plugin/clap/sat_clap_plugin.h"
#include "plugin/sat_audio_port.h"
#include "plugin/sat_note_port.h"
#include "plugin/sat_parameter.h"
#include "plugin/sat_process_context.h"

#if !defined (SAT_GUI_NOGUI)
  #include "plugin/sat_editor.h"
  #include "plugin/sat_editor_listener.h"
#endif

//#include "plugin/sat_audio_processor.h"
//#include "plugin/sat_event_processor.h"
//#include "plugin/sat_parameter_manager.h"

typedef SAT_LockFreeQueue<uint32_t,SAT_PLUGIN_MAX_PARAM_EVENTS_PER_BLOCK> SAT_FromHostQueue;
typedef SAT_LockFreeQueue<uint32_t,SAT_PLUGIN_MAX_GUI_EVENTS_PER_BLOCK>   SAT_FromGuiQueue;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Plugin
: public SAT_ClapPlugin

#if !defined (SAT_GUI_NOGUI)
, public SAT_EditorListener
#endif

  {

//------------------------------
private:
//------------------------------

  const clap_plugin_descriptor_t* MDescriptor           = nullptr;

  SAT_ParameterArray              MParameters           = {};
  SAT_AudioPortArray              MAudioInputPorts      = {};
  SAT_NotePortArray               MNoteInputPorts       = {};
  SAT_AudioPortArray              MAudioOutputPorts     = {};
  SAT_NotePortArray               MNoteOutputPorts      = {};

  SAT_ProcessContext              MProcessContext       = {};
  SAT_Dictionary<const void*>     MExtensions           = {};

  uint32_t                        MEventMode            = SAT_PLUGIN_EVENT_MODE_BLOCK;

  bool                            MIsInitialized        = false;
  bool                            MIsActivated          = false;
  bool                            MIsProcessing         = false;
  double                          MSampleRate           = 0.0;
  uint32_t                        MMinBufferSize        = 0;
  uint32_t                        MMaxBufferSize        = 0;
  int32_t                         MRenderMode           = CLAP_RENDER_REALTIME;

#if !defined (SAT_GUI_NOGUI)
  SAT_Editor*                     MEditor               = nullptr;
  uint32_t                        MEditorWidth          = 256;
  uint32_t                        MEditorHeight         = 256;
  SAT_FromHostQueue               MParamFromHostToGui   = {};
  SAT_FromHostQueue               MModFromHostToGui     = {};
  SAT_FromGuiQueue                MParamFromGuiToAudio  = {};
  SAT_FromGuiQueue                MParamFromGuiToHost   = {};
#endif


//------------------------------
public:
//------------------------------

  SAT_Plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : SAT_ClapPlugin(ADescriptor,AHost) {
  }

  //----------

  virtual ~SAT_Plugin() {
    SAT_PRINT;
    #ifndef SAT_NO_AUTODELETE
      deleteAudioPorts();
      deleteNotePorts();
      deleteParameters();
    #endif
  }

//------------------------------
public: // presets
//------------------------------

  virtual bool loadPreset(const char* ALocation, const char* AKey) {
    //return false;
    char line_buffer[256] = {0};
    SAT_File file = {};
    if (!file.exists(ALocation)) {
      SAT_Print("Error! '%s' does not exist\n",ALocation);
      return false;
    }
    
    if (file.open(ALocation,SAT_FILE_READ_TEXT)) {
      for (uint32_t i=0; i<5; i++) file.readLine(line_buffer,256); // skip metadata
      
      // hex

      //sat_param_t param_buffer[SAT_PLUGIN_MAX_PARAMETERS] = {0};
      //void* ptr = param_buffer;
      //while (file.readLine(line_buffer,256)) {
      //  if (line_buffer[strlen(line_buffer)-1] == '\n') line_buffer[strlen(line_buffer)-1] = 0;
      //  if (line_buffer[0] != 0) {
      //    SAT_Print("%s\n",line_buffer);
      //    ptr = SAT_HexDecode(ptr,line_buffer,32); // num bytes
      //  }
      //}
      //sat_param_t* param_ptr = (sat_param_t*)ptr;
      //uint32_t num_params = getNumParameters();
      //for (uint32_t i=0; i<num_params; i++) {
      //  sat_param_t value = *param_ptr++;
      //  SAT_Print("%i : %f\n",i,value);
      //}
      //setAllParameters(param_buffer);
      
      // ascii
      
      uint32_t i = 0;
      while (file.readLine(line_buffer,256)) {
        if (line_buffer[strlen(line_buffer)-1] == '\n') line_buffer[strlen(line_buffer)-1] = 0;
        if (line_buffer[0] != 0) {
          
          SAT_Print("line %i: '%s'\n",i,line_buffer);
          
          //ptr = SAT_HexDecode(ptr,line_buffer,32); // num bytes
          //setParameterValue(i,v);

          //double v = atof(line_buffer);
          //SAT_Print("%i = %f\n",i,v);
          
          i += 1;
        }
      }
      file.close();
    }
    else {
      SAT_Print("Error opening file '%s'\n",ALocation);
      return false;
    }
    
    return true;

  }

//------------------------------
public: // extensions
//------------------------------

  virtual void registerExtension(const char* AId, const void* APtr) {
    MExtensions.addItem(AId,APtr);
  }

  //----------

  virtual void registerDefaultExtension() {
    MExtensions.addItem(CLAP_EXT_AUDIO_PORTS,&MExtAudioPorts);
    MExtensions.addItem(CLAP_EXT_NOTE_PORTS,&MExtNotePorts);
    MExtensions.addItem(CLAP_EXT_PARAMS,&MExtParams);
    #if !defined (SAT_GUI_NOGUI)
      MExtensions.addItem(CLAP_EXT_GUI,&MExtGui);
    #endif
  }

  //----------

  virtual void registerDefaultSynthExtension() {
  }

  //----------

  virtual void registerAllExtension() {
  }

//------------------------------
public: // parameters
//------------------------------

  virtual SAT_Parameter* appendParameter(SAT_Parameter* AParameter) {
    int32_t index = MParameters.size();
    AParameter->setIndex(index);
    MParameters.append(AParameter);
    return AParameter;
  }

  //----------

  virtual SAT_Parameter* appendParameter(const clap_param_info_t* AInfo) {
    SAT_Parameter* parameter = new SAT_Parameter(AInfo);
    return appendParameter(parameter);
  }

  //----------

  virtual void deleteParameters() {
    uint32_t num = MParameters.size();
    for (uint32_t i=0; i<num; i++) {
      if (MParameters[i]) {
        delete MParameters[i];
        MParameters[i] = nullptr;
      }
    }
    MParameters.clear(true);
  }

  //----------

  virtual uint32_t getNumParameters() {
    return MParameters.size();
  }

  //----------

  virtual SAT_Parameter* getParameter(uint32_t AIndex) {
    return MParameters[AIndex];
  }

  //----------

  virtual void updateParameterFromGui(SAT_Parameter* AParameter) {
    uint32_t index = AParameter->getIndex();
    sat_param_t value = AParameter->getValue();
    SAT_Print("index %i value %f\n",index,value);
  }

  //----------
  //----------
  //----------

  // void setParameterValue(uint32_t AIndex, sat_param_t AValue) {
  //   //MParameterValues[AIndex] = AValue;
  //   MParameters[AIndex]->setValue(AValue);
  // }

  //----------

  // void setDefaultParameterValues() {
  //   uint32_t num = MParameters.size();
  //   for (uint32_t i=0; i<num; i++) {
  //     double value = MParameters[i]->getDefaultValue();
  //     MParameters[i]->setValue(value);
  //   }
  // }
  
  //----------
  
  // void initEditorParameterValues() {
  //   uint32_t num = MParameters.size();
  //   for (uint32_t i=0; i<num; i++) {
  //     SAT_Parameter* param = MParameters[i];
  //     double value = MParameters[i]->getValue();//getDefaultValue();
  //     // parameters are in clap-space
  //     // widgets are 0..1
  //     uint32_t sub = param->getWidgetIndex();
  //     //SAT_Print("sub %i\n",sub);
  //      
  //     MEditor->initParameterValue(param,i,sub,value); // (arg value  = clap space)
  //   }
  // }

//------------------------------
public: // modulation
//------------------------------

  // void resetAllModulations() {
  //   uint32_t num_params = getNumParameters();
  //   for (uint32_t i=0; i<num_params; i++) {
  //     SAT_Print("%i : %f\n",i);
  //     //setModulation(0);
  //   }
  // }

//------------------------------
public: // audio ports
//------------------------------

  virtual int32_t appendAudioInputPort(SAT_AudioPort* APort) {
    int32_t index = MAudioInputPorts.size();
    APort->setIndex(index);
    MAudioInputPorts.append(APort);
    return index;
  }

  //----------

  virtual int32_t appendAudioInputPort(const clap_audio_port_info_t* AInfo) {
    SAT_AudioPort* port = new SAT_AudioPort(AInfo);
    return appendAudioInputPort(port);
  }

  //----------

  virtual int32_t appendStereoAudioInputPort(const char* name, uint32_t flags=CLAP_AUDIO_PORT_IS_MAIN) {
    SAT_AudioPort* port = new SAT_AudioPort(name,flags,2,CLAP_PORT_STEREO);
    return appendAudioInputPort(port);
  }

  //----------

  virtual int32_t appendAudioOutputPort(SAT_AudioPort* APort) {
    int32_t index = MAudioOutputPorts.size();
    APort->setIndex(index);
    MAudioOutputPorts.append(APort);
    return index;
  }

  //----------

  virtual int32_t appendAudioOutputPort(const clap_audio_port_info_t* AInfo) {
    SAT_AudioPort* port = new SAT_AudioPort(AInfo);
    return appendAudioOutputPort(port);
  }

  //----------

  virtual int32_t appendStereoAudioOutputPort(const char* name, uint32_t flags=CLAP_AUDIO_PORT_IS_MAIN) {
    SAT_AudioPort* port = new SAT_AudioPort(name,flags,2,CLAP_PORT_STEREO);
    return appendAudioOutputPort(port);
  }

  //----------

  virtual void deleteAudioPorts() {
    for (uint32_t i=0; i<MAudioInputPorts.size(); i++) {
      if (MAudioInputPorts[i]) delete MAudioInputPorts[i];
    }
    for (uint32_t i=0; i<MAudioOutputPorts.size(); i++) {
      if (MAudioOutputPorts[i]) delete MAudioOutputPorts[i];
    }
    MAudioInputPorts.clear(true);
    MAudioOutputPorts.clear(true);
  }

//------------------------------
public: // note ports
//------------------------------

  virtual int32_t appendNoteInputPort(SAT_NotePort* APort) {
    int32_t index = MNoteInputPorts.size();
    APort->setIndex(index);
    MNoteInputPorts.append(APort);
    return index;
  }

  //----------

  virtual int32_t appendNoteInputPort(const clap_note_port_info_t* AInfo) {
    SAT_NotePort* port = new SAT_NotePort(AInfo);
    return appendNoteInputPort(port);
  }

  //----------

  virtual int32_t appendClapNoteInputPort(const char* name) {
    SAT_NotePort* port = new SAT_NotePort(name,CLAP_NOTE_DIALECT_CLAP,CLAP_NOTE_DIALECT_CLAP);
    return appendNoteInputPort(port);
  }

  //----------

  virtual int32_t appendMidiNoteInputPort(const char* name) {
    SAT_NotePort* port = new SAT_NotePort(name,CLAP_NOTE_DIALECT_MIDI,CLAP_NOTE_DIALECT_MIDI);
    return appendNoteInputPort(port);
  }

  //----------

  virtual int32_t appendNoteOutputPort(SAT_NotePort* APort) {
    int32_t index = MNoteOutputPorts.size();
    APort->setIndex(index);
    MNoteOutputPorts.append(APort);
    return index;
  }

  //----------

  virtual int32_t appendNoteOutputPort(const clap_note_port_info_t* AInfo) {
    SAT_NotePort* port = new SAT_NotePort(AInfo);
    return appendNoteOutputPort(port);
  }

  //----------

  virtual int32_t appendClapNoteOutputPort(const char* name) {
    SAT_NotePort* port = new SAT_NotePort(name,CLAP_NOTE_DIALECT_CLAP,CLAP_NOTE_DIALECT_CLAP);
    return appendNoteOutputPort(port);
  }

  //----------

  virtual int32_t appendMidiNoteOutputPort(const char* name) {
    SAT_NotePort* port = new SAT_NotePort(name,CLAP_NOTE_DIALECT_MIDI,CLAP_NOTE_DIALECT_MIDI);
    return appendNoteOutputPort(port);
  }

  //----------

  virtual void deleteNotePorts() {
    for (uint32_t i=0; i<MNoteInputPorts.size(); i++) {
      if (MNoteInputPorts[i]) delete MNoteInputPorts[i];
    }
    for (uint32_t i=0; i<MNoteOutputPorts.size(); i++) {
      if (MNoteOutputPorts[i]) delete MNoteOutputPorts[i];
    }
    MNoteInputPorts.clear(true);
    MNoteOutputPorts.clear(true);
  }

//------------------------------
public: // editor
//------------------------------

#if !defined (SAT_GUI_NOGUI)

  virtual SAT_Editor* createEditor(SAT_EditorListener* AListener) {
    return new SAT_Editor(AListener,MEditorWidth,MEditorHeight);
    //return nullptr;
  }

  virtual void deleteEditor(SAT_Editor* AEditor) {
    delete AEditor;
  }

  //----------

  virtual void setupEditorWindow(SAT_Editor* AEditor, SAT_Window* AWindow) {
    SAT_PRINT;
  }

  //----------

  //virtual void cleanupEditor(SAT_Editor* AEditor) {
  //}

#endif // nogui

//------------------------------
public: // events
//------------------------------

  virtual void handleNoteOn(clap_event_note_t* event) {}
  virtual void handleNoteOff(clap_event_note_t* event) {}
  virtual void handleNoteChoke(clap_event_note_t* event) {}
  virtual void handleNoteExpression(clap_event_note_expression_t* headeventer) {}
  virtual void handleParamValue(clap_event_param_value_t* event) {}
  virtual void handleParamMod(clap_event_param_mod_t* event) {}
  virtual void handleTransport(clap_event_transport_t* event) {}
  virtual void handleMidi(clap_event_midi_t* event) {}
  virtual void handleMidiSysex(clap_event_midi_sysex_t* event) {}
  virtual void handleMidi2(clap_event_midi2_t* event) {}

  //----------

  virtual void handleEvents(const clap_input_events_t *in_events, const clap_output_events_t *out_events) {
    uint32_t num = in_events->size(in_events);
    switch (MEventMode) {
      case SAT_PLUGIN_EVENT_MODE_BLOCK: {
        for (uint32_t i=0; i<num; i++) {
          const clap_event_header_t* header;
          header = in_events->get(in_events,i);
          handleEvent(header);
        }
        break;
      }
      case SAT_PLUGIN_EVENT_MODE_INTERLEAVED: {
        break;
      }
      case SAT_PLUGIN_EVENT_MODE_QUANTIZED: {
        break;
      }
    }
  }

  //----------

  // virtual
  void handleEvent(const clap_event_header_t* header) {
    if (header->space_id == CLAP_CORE_EVENT_SPACE_ID) {
      switch (header->type) {
        case CLAP_EVENT_NOTE_ON:          handleNoteOn((clap_event_note_t*)header);
        case CLAP_EVENT_NOTE_OFF:         handleNoteOff((clap_event_note_t*)header);
        case CLAP_EVENT_NOTE_CHOKE:       handleNoteChoke((clap_event_note_t*)header);
        case CLAP_EVENT_NOTE_EXPRESSION:  handleNoteExpression((clap_event_note_expression_t*)header);
        case CLAP_EVENT_PARAM_VALUE:      handleParamValue((clap_event_param_value_t*)header);
        case CLAP_EVENT_PARAM_MOD:        handleParamMod((clap_event_param_mod_t*)header);
        case CLAP_EVENT_TRANSPORT:        handleTransport((clap_event_transport_t*)header);
        case CLAP_EVENT_MIDI:             handleMidi((clap_event_midi_t*)header);
        case CLAP_EVENT_MIDI_SYSEX:       handleMidiSysex((clap_event_midi_sysex_t*)header);
        case CLAP_EVENT_MIDI2:            handleMidi2((clap_event_midi2_t*)header);
      }
    }
  }

  //----------

  void handleNoteOnEvent(clap_event_note_t* event) {
    //int32_t id   = event->note_id;
    //int16_t port = event->port_index;
    //int16_t chan = event->key;
    //int16_t key  = event->key;
    //double  vel  = event->velocity;
    handleNoteOn(event);
  }

  //----------

  void handleNoteOffEvent(clap_event_note_t* event) {
    handleNoteOff(event);
  }

  //----------

  void handleNoteChokeEvent(clap_event_note_t* event) {
    handleNoteChoke(event);
  }

  //----------

  void handleNoteExpressionEvent(clap_event_note_expression_t* event) {
    handleNoteExpression(event);
  }

  //----------

  void handleParamValueEvent(clap_event_param_value_t* event) {
    uint32_t index = event->param_id; // !!!
    double value = event->value;
    SAT_Print("index %i value %.3f\n",index,value);
    MParameters[index]->setValue(value);
    handleParamValue(event);
    // host_to_gui
  }

  //----------

  void handleParamModEvent(clap_event_param_mod_t* event) {
    uint32_t index = event->param_id; // !!!
    double amount = event->amount;
    SAT_Print("index %i amount %.3f\n",index,amount);
    MParameters[index]->setModulation(amount);
    handleParamMod(event);
    // host_to_gui
  }

  //----------

  void handleTransportEvent(clap_event_transport_t* event) {
    handleTransport(event);
  }

  //----------

  void handleMidiEvent(clap_event_midi_t* event) {
    handleMidi(event); // data1,data2,data3
    uint8_t data1 = event->data[0];
    uint8_t data2 = event->data[1];
    uint8_t data3 = event->data[2];
    #ifdef SAT_PLUGIN_CONVERT_MIDI
      uint8_t msg   = (data1 & 0xf0);
      uint8_t chan  = (data1 & 0x0f);
      switch (msg) {
        case SAT_MIDI_NOTE_OFF: break;
        case SAT_MIDI_NOTE_ON: break;
        case SAT_MIDI_POLY_AFTERTOUCH: break;
        case SAT_MIDI_CONTROL_CHANGE: break;
        case SAT_MIDI_PROGRAM_CHANGE: break;
        case SAT_MIDI_CHANNEL_AFTERTOUCH: break;
        case SAT_MIDI_PITCHBEND: break;
        case SAT_MIDI_SYS: break;
      }
    #endif
  }

  //----------

  void handleMidiSysexEvent(clap_event_midi_sysex_t* event) {
    handleMidiSysex(event);
  }

  //----------

  void handleMidi2Event(clap_event_midi2_t* event) {
    handleMidi2(event);
  }

//------------------------------
public: // process events
//------------------------------

  /*
    don't send ALL param value/mods to gui.. only last one in block
    set flag, and check at end of process
  */

  // call before processing all events
  
  // void clearAutomationToGui() {
  //   uint32_t num = MParameters.size();
  //   for (uint32_t i=0; i<num; i++) {
  //     MParameters[i]->setGuiAutomationDirty(false);
  //   }
  // }
  
  // void clearModulationToGui() {
  //   uint32_t num = MParameters.size();
  //   for (uint32_t i=0; i<num; i++) {
  //     MParameters[i]->setGuiModulationDirty(false);
  //   }
  // }
  
  //----------
    
  // call after processing all events
  
  // void queueAutomationToGui() {
  //   uint32_t num = MParameters.size();
  //   for (uint32_t i=0; i<num; i++) {
  //     if (MParameters[i]->isGuiAutomationDirty()) {
  //       double value = MParameters[i]->getLastAutomatedValue();
  //       queueParamFromHostToGui(i,value);
  //     }
  //   }
  // }

  // void queueModulationToGui() {
  //   uint32_t num = MParameters.size();
  //   for (uint32_t i=0; i<num; i++) {
  //     if (MParameters[i]->isGuiModulationDirty()) {
  //       double value = MParameters[i]->getLastModulatedValue();
  //       queueModFromHostToGui(i,value);
  //     }
  //   }
  // }

  //----------

  // virtual void preProcessEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) {
  // }

  //----------

  // virtual void postProcessEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) {
  // }

  //----------

  // called from SAT_Plugin.process(), just before processAudio()

  // virtual void processBlockEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) {
  //   //SAT_PRINT;
  //   if (!in_events) return;
  //   //if (!out_events) return;
  //   clearAutomationToGui();
  //   clearModulationToGui();
  //   uint32_t prev_time = 0;
  //   uint32_t size = in_events->size(in_events);
  //   for (uint32_t i=0; i<size; i++) {
  //     const clap_event_header_t* header = in_events->get(in_events,i);
  //     if (header->space_id == CLAP_CORE_EVENT_SPACE_ID) {
  //       if (header->time < prev_time) {
  //         SAT_Print("huh? not sorted? prev_time %i header->time %i header->type %i\n",prev_time,header->time,header->type);
  //       }
  //       handleEvent(header);
  //       prev_time = header->time;
  //     }
  //   }
  //   queueAutomationToGui();
  //   queueModulationToGui();
  // }

  //----------
  
  /*
    todo: check what would happen if we get events that aren't sorted..
    ignored?
  */

  // processes events at their sample accurate place, and audio inbetween

  // virtual void processInterleavedEvents(SAT_ProcessContext* AContext) {
  //   const clap_input_events_t* in_events = AContext->process->in_events;
  //   uint32_t remaining = AContext->process->frames_count;
  //   uint32_t num_events = in_events->size(in_events);
  //   uint32_t current_time = 0;
  //   uint32_t current_event = 0;
  //   clearAutomationToGui();
  //   clearModulationToGui();
  //   while (remaining > 0) {
  //     if (current_event < num_events) {
  //       const clap_event_header_t* header = in_events->get(in_events,current_event);
  //       current_event += 1;
  //       int32_t length = header->time - current_time;
  //       // if length > remaining ...
  //       //while (length > 0) {
  //       if (length > 0) {
  //         processAudio(AContext,current_time,length);
  //         remaining -= length;    // -= 32;
  //         current_time += length; // -= 32;
  //       }
  //       //processEventInterleaved(header);
  //       handleEvent(header);
  //     }
  //     else { // no more events
  //       int32_t length = remaining;
  //       processAudio(AContext,current_time,length);
  //       remaining -= length;
  //       current_time += length;
  //     }
  //   }
  //   //SAT_Assert( events.read(&event) == false );
  //   queueAutomationToGui();
  //   queueModulationToGui();
  // }

  //----------
  
  // split audio block in smaller, regular sizes, and quantize events
  // (process all events 'belonging' to the slice, at the atart ot the slice,
  // and then the audio)..
  // events could be processed up to (slicesize - 1) samples 'early'..

  // virtual void processQuantizedEvents(SAT_ProcessContext* AContext) {
  //   uint32_t buffer_length = AContext->process->frames_count;
  //   uint32_t remaining = buffer_length;
  //   uint32_t current_time = 0;
  //   uint32_t current_event = 0;
  //   uint32_t next_event_time = 0;
  //   clearAutomationToGui();
  //   clearModulationToGui();
  //   const clap_input_events_t* in_events = AContext->process->in_events;
  //   uint32_t num_events = in_events->size(in_events);
  //   if (num_events > 0) {
  //     const clap_event_header_t* header = in_events->get(in_events,current_event);
  //     current_event += 1;
  //     next_event_time = header->time;
  //     do {
  //       // process events for next slice
  //       while (next_event_time < (current_time + SAT_AUDIO_QUANTIZED_SIZE)) {
  //         handleEvent(header);
  //         if (current_event < num_events) {
  //           header = in_events->get(in_events,current_event);
  //           // if (header)
  //           current_event += 1;
  //           next_event_time = header->time;
  //         }
  //         else {
  //           next_event_time = buffer_length; // ???
  //         }
  //       }
  //       // process next slice
  //       if (remaining < SAT_AUDIO_QUANTIZED_SIZE) {
  //         processAudio(AContext,current_time,remaining);
  //         current_time += remaining;
  //         remaining = 0;
  //       }
  //       else {
  //         processAudio(AContext,current_time);
  //         current_time += SAT_AUDIO_QUANTIZED_SIZE;
  //         remaining -= SAT_AUDIO_QUANTIZED_SIZE;
  //       }
  //     } while (remaining > 0);
  //   }
  //   else { // no events..
  //     do {
  //       if (remaining < SAT_AUDIO_QUANTIZED_SIZE) processAudio(AContext,current_time,remaining);
  //       else processAudio(AContext,current_time);
  //       current_time += SAT_AUDIO_QUANTIZED_SIZE;
  //       remaining -= SAT_AUDIO_QUANTIZED_SIZE;
  //     } while (remaining > 0);
  //   }
  //   queueAutomationToGui();
  //   queueModulationToGui();
  // }

//------------------------------
public: // audio
//------------------------------

  virtual void processAudio(SAT_ProcessContext* AContext) {
    switch (MEventMode) {
      case SAT_PLUGIN_EVENT_MODE_BLOCK: {
        processAudioBlock(AContext);
        break;
      }
      case SAT_PLUGIN_EVENT_MODE_INTERLEAVED: {
        break;
      }
      case SAT_PLUGIN_EVENT_MODE_QUANTIZED: {
        break;
      }
    }
  }

  //----------

  void processAudioBlock(SAT_ProcessContext* AContext) {
    const clap_process_t* process = AContext->process;
    uint32_t length = process->frames_count;
    if (MAudioInputPorts.size() < 1) return;
    if (MAudioOutputPorts.size() < 1) return;
    if (MAudioInputPorts[0]->getInfo()->channel_count != 2) return;
    if (MAudioOutputPorts[0]->getInfo()->channel_count != 2) return;
    float* input0 = process->audio_inputs[0].data32[0];
    float* input1 = process->audio_inputs[0].data32[1];
    float* output0 = process->audio_outputs[0].data32[0];
    float* output1 = process->audio_outputs[0].data32[1];
    for (uint32_t i=0; i<length; i++) {
      *output0++ = *input0++;
      *output1++ = *input1++;
    }
  }

  //----------

  //void processAudioInterleaved(SAT_ProcessContext* AContext, uint32_t AOffset) {
  //}

  //----------

  //void processAudioQuantized(SAT_ProcessContext* AContext) {
  //}

//------------------------------
public: // queues
//------------------------------

#if !defined (SAT_GUI_NOGUI)

  bool queueParamFromHostToGui(uint32_t AIndex) {
    return MParamFromHostToGui.write(AIndex);
  }

  bool queueModFromHostToGui(uint32_t AIndex) {
    return MModFromHostToGui.write(AIndex);
  }

  bool queueParamFromGuiToHost(uint32_t AIndex) {
    return MParamFromGuiToHost.write(AIndex);
  }

  bool queueParamFromGuiToAudio(uint32_t AIndex) {
    return MParamFromGuiToAudio.write(AIndex);
  }

  //----------

  void flushParamFromHostToGui() {
    uint32_t index;
    while (MParamFromHostToGui.read(&index)) {
      SAT_Print("%i\n",index);
      // todo
    }
  }

  void flushModFromHostToGui() {
    uint32_t index;
    while (MParamFromHostToGui.read(&index)) {
      SAT_Print("%i\n",index);
      // todo
    }
  }

  void flushParamFromGuiToHost() {
    uint32_t index;
    while (MParamFromHostToGui.read(&index)) {
      SAT_Print("%i\n",index);
      // todo
    }
  }

  void flushParamFromGuiToAudio() {
    uint32_t index;
    while (MParamFromHostToGui.read(&index)) {
      SAT_Print("%i\n",index);
      // todo
    }
  }

#endif // nogui

//------------------------------
protected: // SAT_EditorListener
//------------------------------

#if !defined (SAT_GUI_NOGUI)

  void on_editorListener_timer() override {
    // flushParamFromHostToGui();
    // flushModFromHostToGui();
  }

  void on_editorListener_update(SAT_Parameter* AParameter, sat_param_t AValue) override {
    uint32_t index = AParameter->getIndex();
    SAT_Print("parameter %p index %i value %f\n",AParameter,index,AValue);
    MParameters[index]->setValue(AValue);
//    queueParamFromGuiToAudio(AIndex);
//    queueParamFromGuiToHost(AIndex);

  }

#endif // nogui

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------



//------------------------------
protected: // clap_plugin
//------------------------------

  bool init() override {
    MIsInitialized = true;
    return true;
  }

  //----------

  void destroy() override {
    SAT_PRINT;
    MIsInitialized = false;

    delete this;
    
  }

  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) override {
    MIsActivated = true;
    MSampleRate     = sample_rate;
    MMinBufferSize  = min_frames_count;
    MMaxBufferSize  = max_frames_count;
    return true;
  }

  //----------

  void deactivate() override {
    MIsActivated = false;
  }

  //----------

  bool start_processing() override {
    MIsProcessing = true;
    return true;
  }

  //----------

  void stop_processing() override {
    MIsProcessing = false;
  }

  //----------

  void reset() override {
    MProcessContext.process_counter = 0;
    MProcessContext.sample_counter = 0;
  }

  //----------

  clap_process_status process(const clap_process_t *process) override {
    MProcessContext.process = process;
    MProcessContext.samplerate = MSampleRate;
    MProcessContext.process_counter += 1;

    #if !defined (SAT_GUI_NOGUI)
      flushParamFromGuiToAudio();
    #endif

    handleEvents(process->in_events,process->out_events);
    processAudio(&MProcessContext);

    #if !defined (SAT_GUI_NOGUI)
      flushParamFromGuiToHost();
    #endif

    MProcessContext.sample_counter += process->frames_count;
    return CLAP_PROCESS_CONTINUE;
  }

  //----------

  const void* get_extension(const char *id) override {
    if (MExtensions.hasItem(id)) return MExtensions.getItem(id);
    return nullptr;
  }

  //----------

  void on_main_thread() override {
  }

//------------------------------
protected: // audio_ports
//------------------------------

  uint32_t audio_ports_count(bool is_input) override {
    if (is_input) {
      return MAudioInputPorts.size();
    }
    else {
      return MAudioOutputPorts.size();
    }
  }

  //----------

  bool audio_ports_get(uint32_t index, bool is_input, clap_audio_port_info_t *info) override {
    if (is_input) {
      memcpy(info,MAudioInputPorts[index]->getInfo(),sizeof(clap_audio_port_info_t));
    }
    else {
      memcpy(info,MAudioOutputPorts[index]->getInfo(),sizeof(clap_audio_port_info_t));
    }
      return true;
  }

//------------------------------
protected: // audio_ports_config
//------------------------------

  uint32_t audio_ports_config_count() override {
    return 1;
  }

  //----------

  //TODO: fix this

  bool audio_ports_config_get(uint32_t index, clap_audio_ports_config_t *config) override {
    switch (index) {
      case 0: {
        config->id                        = 0; // index;
        SAT_Strlcpy(config->name,"",CLAP_NAME_SIZE);
        config->input_port_count          = 1;
        config->output_port_count         = 1;
        // main input info
        config->has_main_input            = true;
        config->main_input_channel_count  = 2;
        config->main_input_port_type      = CLAP_PORT_STEREO;
        // main output info
        config->has_main_output           = true;
        config->main_output_channel_count = 2;
        config->main_output_port_type     = CLAP_PORT_STEREO;
        return true;
      }
    }
    return false;
  }

  //----------

  bool audio_ports_config_select(clap_id config_id) override {
    return true;
  }

//------------------------------
protected: // gui
//------------------------------

#if !defined (SAT_GUI_NOGUI)

  bool gui_is_api_supported(const char *api, bool is_floating) override {
    SAT_Print("api %s is_floating %i",api,is_floating);
    #if defined(SAT_GUI_WAYLAND)
      if ((strcmp(api,CLAP_WINDOW_API_WAYLAND) == 0) && (is_floating)) {
        SAT_DPrint(" -> true\n");
        return true;
      }
    #elif defined(SAT_GUI_WIN32)
      if ((strcmp(api,CLAP_WINDOW_API_WIN32) == 0) && (!is_floating)) {
        SAT_DPrint(" -> true\n");
        return true;
      }
    #elif defined(SAT_GUI_X11)
      if ((strcmp(api,CLAP_WINDOW_API_X11) == 0) && (!is_floating)) {
        SAT_DPrint(" -> true\n");
        return true;
      }
    #endif
    SAT_DPrint(" -> false\n");
    return false;
  }

  //----------

  bool gui_get_preferred_api(const char **api, bool *is_floating) override {
    SAT_Print("");
    #if defined(SAT_GUI_WAYLAND)
      *api = CLAP_WINDOW_API_WAYLAND;
      *is_floating = true;
      SAT_DPrint(" -> true (*api %s is_floating %i)\n",*api,*is_floating);
      return true;
    #elif defined(SAT_GUI_WIN32)
      *api = CLAP_WINDOW_API_WIN32;
      *is_floating = false;
      SAT_DPrint(" -> true (*api %s is_floating %i)\n",*api,*is_floating);
      return true;
    #elif defined(SAT_GUI_X11)
      *api = CLAP_WINDOW_API_X11;
      *is_floating = false;
      SAT_DPrint(" -> true (*api %s is_floating %i)\n",*api,*is_floating);
      return true;
    #endif
    SAT_DPrint(" -> false\n");
    return false;
  }

  //----------

  bool gui_create(const char *api, bool is_floating) override {
    SAT_Print("api %s is_floating %i",api,is_floating);
    MEditor = createEditor(this);
    if (MEditor) {
      //setupEditor(MEditor);
      SAT_DPrint(" -> true\n");
      return true;
    }
    SAT_DPrint(" -> false\n");
    SAT_Print("MEditor = null\n");
    return false;
  }

  //----------

  void gui_destroy() override {
    SAT_Print("\n");
    if (MEditor) {
      //cleanupEditor(MEditor);
      deleteEditor(MEditor);
      MEditor = nullptr;
    }
    else { SAT_Print("MEditor = null\n"); }
  }

  //----------

  bool gui_set_scale(double scale) override {
    SAT_Print("scale %f\n",scale);
    if (MEditor) return MEditor->set_scale(scale);
    SAT_Print("MEditor = null\n");
    return false;
  }

  //----------

  bool gui_get_size(uint32_t *width, uint32_t *height) override {
    bool result = MEditor->get_size(width,height);
    SAT_Print("(*width %i *height %i)\n",*width,*height);
    return result;
  }

  //----------

  bool gui_can_resize() override {
    SAT_Print("\n");
    if (MEditor) return MEditor->can_resize();
    SAT_Print("MEditor = null\n");
    return false;
  }

  //----------

  bool gui_get_resize_hints(clap_gui_resize_hints_t *hints) override {
    SAT_Print("\n");
    if (MEditor) return MEditor->get_resize_hints(hints);
    SAT_Print("MEditor = null\n");
    return false;
  }

  //----------

  bool gui_adjust_size(uint32_t *width, uint32_t *height) override {
    SAT_Print("(*width %i *height %i)\n",*width,*height);
    if (MEditor) return MEditor->adjust_size(width,height);
    SAT_Print("MEditor = null\n");
    return false;
  }

  //----------

  bool gui_set_size(uint32_t width, uint32_t height) override {
    SAT_Print("width %i height %i\n",width,height);
    if (MEditor) return MEditor->set_size(width,height);
    SAT_Print("MEditor = null\n");
    return false;
  }

  //----------

  bool gui_set_parent(const clap_window_t *window) override {
    SAT_Print("\n");
    if (MEditor) {
      bool result = MEditor->set_parent(window);
      SAT_Window* window = MEditor->getWindow();
      if (window) setupEditorWindow(MEditor,window);
      return result;
    }
    SAT_Print("MEditor = null\n");
    return false;
  }

  //----------

  bool gui_set_transient(const clap_window_t *window) override {
    SAT_Print("\n");
    if (MEditor) return MEditor->set_transient(window);
    SAT_Print("MEditor = null\n");
    return false;
  }

  //----------

  void gui_suggest_title(const char *title) override {
    SAT_Print("title %s\n",title);
    if (MEditor) MEditor->suggest_title(title);
    else { SAT_Print("MEditor = null\n"); }
  }

  //----------

  bool gui_show() override {
    SAT_Print("\n");
    if (MEditor) return MEditor->show();
    SAT_Print("MEditor = null\n");
    return false;
  }

  //----------

  bool gui_hide() override {
    SAT_Print("\n");
    if (MEditor) return MEditor->hide();
    SAT_Print("MEditor = null\n");
    return false;
  }

#endif // nogui

//------------------------------
protected: // latency
//------------------------------

  uint32_t latency_get() override {
    return 0;
  }

//------------------------------
protected: // note_name
//------------------------------

  uint32_t note_name_count() override {
    return 0;
  }

  //----------

  bool note_name_get(uint32_t index, clap_note_name_t *note_name) override {
    //note_name->port = -1;
    //note_name->channel = -1;
    //note_name->key = -1;
    //SAT_Strlcpy(note_name->name,"",CLAP_NAME_SIZE);
    return false;
  }

//------------------------------
protected: // note_ports
//------------------------------

  uint32_t note_ports_count(bool is_input) override {
    return 1;
  }

  //----------

  bool note_ports_get(uint32_t index, bool is_input, clap_note_port_info_t *info) override {
    switch (index) {
      case 0: {
        info->id                  = 0; // index;
        SAT_Strlcpy(info->name,"",CLAP_NAME_SIZE);
        info->preferred_dialect   = CLAP_NOTE_DIALECT_CLAP;
        info->supported_dialects  = CLAP_NOTE_DIALECT_CLAP;// | CLAP_NOTE_DIALECT_MIDI | CLAP_NOTE_DIALECT_MIDI_MPE | CLAP_NOTE_DIALECT_MIDI2;
        return true;
      }
    }
    return false;
  }

//------------------------------
protected: // params
//------------------------------

  uint32_t params_count() override {
    return MParameters.size();
  }

  //----------

  bool params_get_info(uint32_t param_index, clap_param_info_t *param_info) override {
    memcpy(param_info,MParameters[param_index]->getInfo(),sizeof(clap_param_info_t));
    return true;
  }
  
  //----------

  bool params_get_value(clap_id param_id, double *out_value) override {
    *out_value = MParameters[param_id]->getValue();
    return true;
  }
  
  //----------

  bool params_value_to_text(clap_id param_id, double value, char *out_buffer, uint32_t out_buffer_capacity) override {
    SAT_Strlcpy( out_buffer, MParameters[param_id]->valueToText(value), out_buffer_capacity );
    return true;
  }
  
  //----------

  bool params_text_to_value(clap_id param_id, const char *param_value_text, double *out_value) override {
    *out_value = MParameters[param_id]->textToValue(param_value_text);
    return true;
  }
  
  //----------

  // on main/gui thread?

  void params_flush(const clap_input_events_t *in, const clap_output_events_t *out) override {
    //handleEvents(in,out);
    for (uint32_t i=0; i<in->size(in); i++) {
      const clap_event_header_t* header;
      header = in->get(in,i);
      handleEvent(header);
    }
  }

//------------------------------
protected: // posix_fd_support
//------------------------------

  void posix_fd_support_on_fd(int fd, clap_posix_fd_flags_t flags) override {
  }

//------------------------------
protected: // render
//------------------------------

  bool render_has_hard_realtime_requirement() override {
    return false;
  }

  //----------

  bool render_set(clap_plugin_render_mode mode) override {
    MRenderMode = mode;
    return true;
  }

//------------------------------
protected: // state
//------------------------------

  bool state_save(const clap_ostream_t *stream) override {
    return true;
  }

  //----------

  bool state_load(const clap_istream_t *stream) override {
    return true;
  }

//------------------------------
protected: // tail
//------------------------------

  uint32_t tail_get() override {
    return 0;
  }

//------------------------------
protected: // thread_pool
//------------------------------

  void thread_pool_exec(uint32_t task_index) override {
  }

//------------------------------
protected: // timer
//------------------------------

  void timer_support_on_timer(clap_id timer_id) override {
  }

//------------------------------
protected: // voice_info
//------------------------------

  bool voice_info_get(clap_voice_info_t *info) override {
    //info->voice_count     = 16;
    //info->voice_capacity  = 16;
    //info->flags           = CLAP_VOICE_INFO_SUPPORTS_OVERLAPPING_NOTES;
    //return true;
    return false;
  }

//------------------------------
protected: // draft: ambisonic
//------------------------------

  bool ambisonic_is_config_supported(const clap_ambisonic_config_t *config) override {
    return false; 
  }

  //----------

  bool ambisonic_get_config(bool is_input, uint32_t port_index, clap_ambisonic_config_t *config) override {
    return false; 
  }

//------------------------------
protected: // draft: audio_ports_activation
//------------------------------

  bool audio_ports_activation_can_activate_while_processing() override {
    return false; 
  }

  //----------

  bool audio_ports_activation_set_active(bool is_input, uint32_t port_index, bool is_active, uint32_t sample_size) override {
    return false; 
  }

//------------------------------
protected: // draft: check_for_update
//------------------------------

  void check_for_update_check(bool include_preview) override {
  }

//------------------------------
protected: // draft: configurable_audio_ports
//------------------------------

  bool configurable_audio_ports_can_apply_configuration(const struct clap_audio_port_configuration_request *requests, uint32_t request_count) override {
    return false; 
  }

  //----------

  bool configurable_audio_ports_apply_configuration(const struct clap_audio_port_configuration_request *requests, uint32_t request_count) override {
    return false; 
  }

//------------------------------
protected: // draft: context_menu
//------------------------------

  bool context_menu_populate(const clap_context_menu_target_t  *target, const clap_context_menu_builder_t *builder) override {
    return false; 
  }

  //----------

  bool context_menu_perform(const clap_context_menu_target_t *target, clap_id action_id) override {
    return false; 
  }

//------------------------------
protected: // draft: cv
//------------------------------

  bool cv_get_channel_type(bool is_input, uint32_t port_index, uint32_t channel_index, uint32_t *channel_type) override {
    return false; 
  }

//------------------------------
protected: //
//------------------------------

  bool extensible_audio_ports_add_port(bool is_input, uint32_t channel_count, const char *port_type, const void *port_details) override {
    return false; 
  }

  //----------

  bool extensible_audio_ports_remove_port(bool is_input, uint32_t index) override {
    return false; 
  }

//------------------------------
protected: // draft: midi_mappings
//------------------------------

  uint32_t midi_mappings_count() override {
    return 0;
  }

  //----------

  bool midi_mappings_get(uint32_t index, clap_midi_mapping_t *mapping) override {
    //mapping->channel = 0;
    //mapping->number = 0;
    //apping->param_id = 0;
    //return true;
    return false;
  }
  
//------------------------------
protected: // draft: param_indication
//------------------------------

  void param_indication_set_mapping(clap_id param_id, bool has_mapping, const clap_color_t *color, const char *label, const char *description) override {
    MParameters[param_id]->setIsMapped(has_mapping);
    MParameters[param_id]->setMappedColor(SAT_Color(color->red,color->green,color->blue,color->alpha));
  }
  
  //----------



  void param_indication_set_automation(clap_id param_id, uint32_t automation_state, const clap_color_t *color) override {
    //MParameters[param_id]->setAutomationIndication(automation_state,color);
    MParameters[param_id]->setAutomationState(automation_state);
    MParameters[param_id]->setAutomationColor(SAT_Color(color->red,color->green,color->blue,color->alpha));
  }
  
//------------------------------
protected: // draft: preset_load
//------------------------------

  bool preset_load_from_location(uint32_t location_kind, const char *location, const char *load_key) override {
    return loadPreset(location,load_key);
    // switch (location_kind) {
    //   case CLAP_PRESET_DISCOVERY_LOCATION_FILE: {
    //     SAT_Print("CLAP_PRESET_DISCOVERY_LOCATION_FILE location '%s', load_key '%s'\n",location,load_key);
    //     MHost->preset_load_loaded(location_kind,location,load_key);
    //     return true;
    //   }
    //   case CLAP_PRESET_DISCOVERY_LOCATION_PLUGIN: {
    //     SAT_Print("CLAP_PRESET_DISCOVERY_LOCATION_PLUGIN location '%s', load_key '%s'\n",location,load_key);
    //     MHost->preset_load_loaded(location_kind,location,load_key);
    //     return true;
    //   }
    //   default: {
    //     SAT_Print("unknown location kind (%i : '%s','%s')\n",location_kind,location,load_key);
    //     return false;
    //   }
    // }
    // return false;
  }
  
//------------------------------
protected: // draft: remote_controls
//------------------------------

  uint32_t remote_controls_count() override {
    return 1;
  }
  
  //----------

  bool remote_controls_get(uint32_t page_index, clap_remote_controls_page_t *page) override {
    SAT_Strlcpy(page->page_name,"Perform",CLAP_NAME_SIZE);
    SAT_Strlcpy(page->section_name,"",CLAP_NAME_SIZE);
    page->page_id = 0;
    page->is_for_preset = false;
    uint32_t numpar = MParameters.size();
    for (uint32_t i=0; i<8; i++) {
      if (i < numpar) page->param_ids[i] = i;// CLAP_INVALID_ID;
      else page->param_ids[i] = CLAP_INVALID_ID;
    }
    return true;
  }
  
//------------------------------
protected: // draft: resource_directory
//------------------------------

  void resource_directory_set_directory(const char *path, bool is_shared) override {
    // MResourceDirectory = path;
    // MResourceDirectoryIsShared = is_shared;
  }

  //----------

  void resource_directory_collect(bool all) override {
  }

  //----------

  uint32_t resource_directory_get_files_count() override {
    return 0;
  }

  //----------

  int32_t resource_directory_get_file_path(uint32_t index, char *path, uint32_t path_size) override {
    return 0;
  }

//------------------------------
protected: // draft: state_context
//------------------------------

  bool state_context_save(const clap_ostream_t *stream, uint32_t context_type) override {
    switch (context_type) {
      case CLAP_STATE_CONTEXT_FOR_DUPLICATE:  return state_save(stream);
      case CLAP_STATE_CONTEXT_FOR_PRESET:     return state_save(stream);
    }
    return true;
  }
  
  //----------

  bool state_context_load(const clap_istream_t *stream, uint32_t context_type) override {
    switch (context_type) {
      case CLAP_STATE_CONTEXT_FOR_DUPLICATE:  return state_load(stream);
      case CLAP_STATE_CONTEXT_FOR_PRESET:     return state_load(stream);
    }
    return true;
  }

//------------------------------
protected: // draft: surround
//------------------------------

  bool surround_is_channel_mask_supported(uint64_t channel_mask) override {
    return false;
  }

  //----------

  uint32_t surround_get_channel_map(bool is_input, uint32_t port_index, uint8_t *channel_map, uint32_t channel_map_capacity) override {
    return 0;
  }

//------------------------------
protected: // draft: track_info
//------------------------------

  // uint64_t      flags;                 // see the flags above
  // char          name[CLAP_NAME_SIZE];  // track name, available if flags contain CLAP_TRACK_INFO_HAS_TRACK_NAME
  // clap_color_t  color;                 // track color, available if flags contain CLAP_TRACK_INFO_HAS_TRACK_COLOR
  // int32_t       audio_channel_count;   // available if flags contain CLAP_TRACK_INFO_HAS_AUDIO_CHANNEL. see audio-ports.h, struct clap_audio_port_info to learn how to use channel count and port type
  // const char*   audio_port_type;

  void track_info_changed() override {
    // const clap_host_t* host = getClapHost();
    // const clap_host_track_info_t* track_info = (const clap_host_track_info_t*)host->get_extension(host,CLAP_EXT_TRACK_INFO);
    // clap_track_info_t info;
    // if (track_info->get(host,&info)) {
    // }
  }

//------------------------------
protected: // draft: triggers
//------------------------------

  uint32_t triggers_count() override {
    return 0;
  }

  //----------

  bool triggers_get_info(uint32_t index, clap_trigger_info_t *trigger_info) override {
    return false;
  }

//------------------------------
protected: // draft: tunung
//------------------------------

  void tuning_changed() override {
  }

};

//----------------------------------------------------------------------
#endif

