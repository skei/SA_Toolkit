#ifndef sat_plugin_queues_included
#define sat_plugin_queues_included
//----------------------------------------------------------------------
/*
  from host   to audio: automation, modulation, notes, ..
  from host   to gui:   automation, modulation

  from gui    to audio: knobs (params)
  from gui    to host:  knobs (param_fvalue)

  from audio  to gui:   [vu, spectrum..]
  from audio  to host:  knobs (param value, via gui to audio),
                        note events (note effect)
                        synth voice (note end)
*/
//----------------------------------------------------------------------

#include "sat.h"
//#include "plugin/processor/sat_process_context.h"
#include "plugin/sat_plugin_base.h"
#include "plugin/sat_editor.h"
#include "plugin/sat_note.h"
#include "plugin/sat_parameter.h"
#include "plugin/sat_processor.h"

//----------------------------------------------------------------------

// 128 bit
struct SAT_PluginQueueItem {
  union {
    uint32_t  param_id;
    int32_t   note_id;
  };
  uint32_t    dummy;
  union {
    double    value;
    SAT_Note  note;
  };
};

//----------------------------------------------------------------------

class SAT_PluginQueues {

  #ifndef SAT_NO_GUI
    //typedef SAT_AtomicQueue<SAT_PluginQueueItem,SAT_PLUGIN_MAX_MOD_EVENTS_PER_BLOCK>   SAT_ModFromHostToGuiQueue;
    //typedef SAT_AtomicQueue<SAT_PluginQueueItem,SAT_PLUGIN_MAX_PARAM_EVENTS_PER_BLOCK> SAT_ParamFromHostToGuiQueue;
    //typedef SAT_AtomicQueue<SAT_PluginQueueItem,SAT_PLUGIN_MAX_GUI_EVENTS_PER_BLOCK>   SAT_ParamFromGuiToAudioQueue;
    //typedef SAT_AtomicQueue<SAT_PluginQueueItem,SAT_PLUGIN_MAX_GUI_EVENTS_PER_BLOCK>   SAT_ParamFromGuiToHostQueue;
    typedef moodycamel::ReaderWriterQueue<SAT_PluginQueueItem> SAT_ModFromHostToGuiQueue;
    typedef moodycamel::ReaderWriterQueue<SAT_PluginQueueItem> SAT_ParamFromHostToGuiQueue;
    typedef moodycamel::ReaderWriterQueue<SAT_PluginQueueItem> SAT_ParamFromGuiToAudioQueue;
    typedef moodycamel::ReaderWriterQueue<SAT_PluginQueueItem> SAT_ParamFromGuiToHostQueue;
  #endif

  //typedef SAT_AtomicQueue<SAT_PluginQueueItem,SAT_PLUGIN_MAX_NOTE_ENDS_PER_BLOCK>   SAT_NoteEndFromAudioToHostQueue;
  //typedef moodycamel::ReaderWriterQueue<SAT_PluginQueueItem> SAT_NoteEndFromAudioToHostQueue;
  
//------------------------------
private:
//------------------------------

  #ifndef SAT_NO_GUI
    SAT_ParamFromHostToGuiQueue       MParamFromHostToGui;//     = {};
    SAT_ModFromHostToGuiQueue         MModFromHostToGui;//       = {};
    SAT_ParamFromGuiToAudioQueue      MParamFromGuiToAudio;//    = {};
    SAT_ParamFromGuiToHostQueue       MParamFromGuiToHost;//     = {};
  #endif

  // SAT_NoteEndFromAudioToHostQueue   MNoteEndFromAudioToHost;//   = {};

//----------------------------------------------------------------------
public: // host -> gui
//----------------------------------------------------------------------

  // when parameter is updated (via process()),
  // we need to tell the gui about it

  //----------

  #ifndef SAT_NO_GUI

  // called from [audio]
  //   SAT_Plugin.on_ProcessorListener_updateParamFromHostToGui
  //   (from SAT_Processor.processParamValueEvent)

  void queueParamFromHostToGui(uint32_t AParamId, sat_param_t AValue) {
    SAT_PluginQueueItem item;
    //item.type = CLAP_EVENT_PARAM_VALUE;
    item.param_id  = AParamId;
    item.value  = AValue;
    //if (!MParamFromHostToGui.write(item)) {
    if (!MParamFromHostToGui.enqueue(item)) {
      // SAT_PRINT("couldn't write to MParamFromHostToGui queue\n");
    }
  }

  //----------

  // called from [timer]
  //   SAT_Plugin.on_EditorListener_timer  

  void flushParamFromHostToGui(SAT_ParameterArray* AParameters, SAT_Editor* AEditor) {
    uint32_t count = 0;
    SAT_PluginQueueItem item;
    //while (MParamFromHostToGui.read(&item)) {
    while (MParamFromHostToGui.try_dequeue(item)) {
      count += 1;
      SAT_Parameter* parameter = AParameters->getItem(item.param_id);
      if (AEditor) AEditor->updateParameterFromHost(parameter,item.value);
    }
    SAT_GLOBAL.ANALYTICS.report_ParamFromHostToGui_count(count);
  }

  //------------------------------
  //
  //------------------------------

  // when parameter is modulated (via process()),
  // we need to tell the gui about it

  //----------

  // called from [audio]
  //   SAT_Plugin.on_ProcessorListener_updateModFromHostToGui
  //   (from SAT_Processor.processParamModEvent)

  void queueModFromHostToGui(uint32_t AParamId, sat_param_t AValue) {
    // todo: is_gui_open?
    SAT_PluginQueueItem item;
    //item.type = CLAP_EVENT_PARAM_MOD;
    item.param_id  = AParamId;
    item.value  = AValue;
    //if (!MModFromHostToGui.write(item)) {
    if (!MModFromHostToGui.enqueue(item)) {
      // SAT_PRINT("couldn't write to MModFromHostToGui queue\n");
    }
  }

  //----------

  // called from [timer]
  //   SAT_Plugin.on_EditorListener_timer

  void flushModFromHostToGui(SAT_ParameterArray* AParameters, SAT_Editor* AEditor) {
    SAT_PluginQueueItem item;
    uint32_t count = 0;
    //while (MModFromHostToGui.read(&item)) {
    while (MModFromHostToGui.try_dequeue(item)) {
      count += 1;
      SAT_Parameter* parameter = AParameters->getItem(item.param_id);
      if (AEditor) AEditor->updateModulationFromHost(parameter,item.value);
    }
    SAT_GLOBAL.ANALYTICS.report_ModFromHostToGui_count(count);
  }

  #endif // no gui

//----------------------------------------------------------------------
public: // gui -> host
//----------------------------------------------------------------------

  // when a widget on the gui is tweaked, and the widget is connected to a parameter
  // we need to tell the host about it

  //----------

  #ifndef SAT_NO_GUI

  // called from [gui]
  //   SAT_Plugin.on_EditorListener_update
  //   (from SAT_EmbeddedEditor.on_windowListener_update)
  //        (SAT_WidgetWindow.on_widgetListener_update)
  //        (SAT_RootWidget.do_Widget_update)

  void queueParamFromGuiToHost(uint32_t AParamId, sat_param_t AValue) {
    SAT_PluginQueueItem item;
    //item.type = CLAP_EVENT_PARAM_VALUE;
    item.param_id  = AParamId;
    item.value  = AValue;
    //if (!MParamFromGuiToHost.write(item)) {
    if (!MParamFromGuiToHost.enqueue(item)) {
      // SAT_PRINT("couldn't write to MParamFromGuiToHost queue\n");
    }
  }

  //----------

  // called from [audio]
  //   SAT_Plugin.process

  void flushParamFromGuiToHost(SAT_ProcessContext* AContext) {
    SAT_ParameterArray* parameters = AContext->parameters;
    uint32_t count = 0;
    uint32_t blocksize = AContext->process->frames_count;
    const clap_output_events_t* out_events = AContext->process->out_events;
    SAT_PluginQueueItem item;
    //while (MParamFromGuiToHost.read(&item)) {
    while (MParamFromGuiToHost.try_dequeue(item)) {
      count += 1;
      SAT_Parameter* parameter = parameters->getItem(item.param_id);
      // gesture begin
      {
        clap_event_param_gesture_t event;
        event.header.size     = sizeof(clap_event_param_gesture_t);
        event.header.time     = blocksize - 1; //0; // should this be (blocksize-1), since we're called at the end of the audioblock?
        event.header.space_id = CLAP_CORE_EVENT_SPACE_ID;
        event.header.type     = CLAP_EVENT_PARAM_GESTURE_BEGIN;
        event.header.flags    = 0;
        event.param_id        = item.param_id;
        const clap_event_header_t* header = (const clap_event_header_t*)&event;
        out_events->try_push(out_events,header);
      }
      // param value
      {
        clap_event_param_value_t event;
        event.header.size     = sizeof(clap_event_param_value_t);
        event.header.time     = blocksize - 1; //0; // same here (blocksize-1)
        event.header.space_id = CLAP_CORE_EVENT_SPACE_ID;
        event.header.type     = CLAP_EVENT_PARAM_VALUE;
        event.header.flags    = 0; // CLAP_EVENT_IS_LIVE, CLAP_EVENT_DONT_RECORD
        event.param_id        = item.param_id;
        event.cookie          = parameter->getCookie(); // reaper needs the event.cookie ??
        event.note_id         = -1;
        event.port_index      = -1;
        event.channel         = -1;
        event.key             = -1;
        event.value           = item.value;
        // to host..
        const clap_event_header_t* header = (const clap_event_header_t*)&event;
        out_events->try_push(out_events,header);
      }
      // gesture end
      {
        clap_event_param_gesture_t event;
        event.header.size     = sizeof(clap_event_param_gesture_t);
        event.header.time     = blocksize - 1; //0; // ..and here (blocksize-1)
        event.header.space_id = CLAP_CORE_EVENT_SPACE_ID;
        event.header.type     = CLAP_EVENT_PARAM_GESTURE_END;
        event.header.flags    = 0;
        event.param_id        = item.param_id;
        const clap_event_header_t* header = (const clap_event_header_t*)&event;
        out_events->try_push(out_events,header);
      }
    } // while
    SAT_GLOBAL.ANALYTICS.report_ParamFromGuiToHost_count(count);
  }

  #endif // no gui

//----------------------------------------------------------------------
public: // gui -> audio
//----------------------------------------------------------------------

  // when a widget on the gui is tweaked, and the widget is connected to a parameter
  // we need to tell the processor (audio thread) about it

  //----------

  #ifndef SAT_NO_GUI

  // called from [gui]
  //   SAT_Plugin.on_EditorListener_update
  //   (from SAT_EmbeddedEditor.on_windowListener_update)
  //        (SAT_WidgetWindow.on_widgetListener_update)
  //        (SAT_RootWindow.do_Widget_update)

  void queueParamFromGuiToAudio(uint32_t AParamId, sat_param_t AValue) {
    SAT_PluginQueueItem item;
    //item.type = CLAP_EVENT_PARAM_VALUE;
    item.param_id  = AParamId;
    item.value  = AValue;
    //if (!MParamFromGuiToAudio.write(item)) {
    if (!MParamFromGuiToAudio.enqueue(item)) {
      // SAT_PRINT("couldn't write to MParamFromGuiToAudio queue\n");
    }
  }

  //----------

  // called from [audio]
  //   SAT_Plugin.process

  void flushParamFromGuiToAudio(SAT_Processor* AProcessor) {
    uint32_t count = 0;
    SAT_PluginQueueItem item;
    //while (MParamFromGuiToAudio.read(&item)) {
    while (MParamFromGuiToAudio.try_dequeue(item)) {
      count += 1;
      clap_event_param_value_t event;
      event.header.size     = sizeof(clap_event_param_value_t);
      event.header.time     = 0;
      event.header.space_id = CLAP_CORE_EVENT_SPACE_ID;
      event.header.type     = CLAP_EVENT_PARAM_VALUE;
      event.header.flags    = 0; // CLAP_EVENT_IS_LIVE, CLAP_EVENT_DONT_RECORD
      event.param_id        = item.param_id;
      event.cookie          = nullptr; // set?
      event.note_id         = -1;
      event.port_index      = -1;
      event.channel         = -1;
      event.key             = -1;
      event.value           = item.value;
      AProcessor->processParamValueEvent(&event);
    }
    SAT_GLOBAL.ANALYTICS.report_ParamFromGuiToAudio_count(count);
  }

  #endif // no gui

//----------------------------------------------------------------------
public: // audio -> host
//----------------------------------------------------------------------

  // handled in SAT_VoiceProcessor.postProcessEvents()

  // when a wnote has reached its end (envelope faded out, etc)
  // we need to tell the host about it

  //----------

  // called from [audio]
  //   todo (event processing)

  // void queueNoteEndFromAudioToHost(uint32_t ANoteId, int16_t APort=-1, int16_t AChannel=-1, int16_t AKey=-1) {
  //   SAT_PluginQueueItem item;
  //   //item.type       = CLAP_EVENT_NOTE_END;
  //   item.note_id       = ANoteId;
  //   //item.value      = 0.0;
  //   item.note.port    = APort;
  //   item.note.channel = AChannel;
  //   item.note.key     = AKey;
  //   // item.note.dummy   = 0;
  //   //if (!MNoteEndFromAudioToHost.write(item)) {
  //   if (!MNoteEndFromAudioToHost.enqueue(item)) {
  //     // SAT_PRINT("couldn't write to MNoteEndFromAudioToHost queue\n");
  //   }
  // }

  //----------

  // called from [audio]
  //   SAT_Plugin.process

  // void flushNoteEndsFromAudioToHost(SAT_ProcessContext* AContext) {
  //   const clap_output_events_t* out_events = AContext->process->out_events;
  //   uint32_t blocksize = AContext->process->frames_count;
  //   uint32_t count = 0;
  //   SAT_PluginQueueItem item;
  //   //while (MNoteEndFromAudioToHost.read(&item)) {
  //   while (MNoteEndFromAudioToHost.try_dequeue(item)) {
  //     count += 1;
  //     clap_event_note_t event;
  //     event.header.size     = sizeof(clap_event_note_t);
  //     event.header.time     = blocksize - 1;
  //     event.header.space_id = CLAP_CORE_EVENT_SPACE_ID;
  //     event.header.type     = CLAP_EVENT_NOTE_END;
  //     event.header.flags    = 0; // CLAP_EVENT_IS_LIVE, CLAP_EVENT_DONT_RECORD
  //     event.note_id         = item.note_id;
  //     event.channel         = item.note.channel;
  //     event.key             = item.note.key;
  //     event.port_index      = item.note.port;
  //     event.velocity        = 0.0;//item.value;
  //     const clap_event_header_t* header = (const clap_event_header_t*)&event;
  //     out_events->try_push(out_events,header);
  //   }
  //   SAT_GLOBAL.ANALYTICS.report_NoteEndFromAudioToHost_count(count);

  // }

};

//----------------------------------------------------------------------
#endif