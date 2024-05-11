#ifndef sat_plugin_queues_included
#define sat_plugin_queues_included
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/processor/sat_process_context.h"
#include "plugin/sat_editor.h"
#include "plugin/sat_note.h"
#include "plugin/sat_parameter.h"
#include "plugin/sat_processor.h"

//----------------------------------------------------------------------

#ifndef SAT_NO_GUI

  // 128 bit
  struct SAT_ParamQueueItem {
    uint32_t    type;
    uint32_t    index;
    union {
      double    value;
      SAT_Note  note; // 64bit
    };
  };

  typedef SAT_Queue<SAT_ParamQueueItem,SAT_PLUGIN_MAX_MOD_EVENTS_PER_BLOCK>   SAT_ModFromHostToGuiQueue;
  typedef SAT_Queue<SAT_ParamQueueItem,SAT_PLUGIN_MAX_PARAM_EVENTS_PER_BLOCK> SAT_ParamFromHostToGuiQueue;
  typedef SAT_Queue<SAT_ParamQueueItem,SAT_PLUGIN_MAX_GUI_EVENTS_PER_BLOCK>   SAT_ParamFromGuiToAudioQueue;
  typedef SAT_Queue<SAT_ParamQueueItem,SAT_PLUGIN_MAX_GUI_EVENTS_PER_BLOCK>   SAT_ParamFromGuiToHostQueue;

#endif

//----------------------------------------------------------------------

class SAT_PluginQueues {

//------------------------------
private:
//------------------------------

  #ifndef SAT_NO_GUI
    SAT_ParamFromHostToGuiQueue       MParamFromHostToGui     = {};
    SAT_ModFromHostToGuiQueue         MModFromHostToGui       = {};
    SAT_ParamFromGuiToAudioQueue      MParamFromGuiToAudio    = {};
    SAT_ParamFromGuiToHostQueue       MParamFromGuiToHost     = {};
  #endif

  // SAT_NoteEndFromAudioToHostQueue   MNoteEndFromAudioToHost = {};

//------------------------------
public: // host -> gui
//------------------------------

  #ifndef SAT_NO_GUI

  // called from
  // SAT_Plugin.on_processorOwner_updateParamFromHostToGui
  // (from SAT_Processor.processParamValueEvent)

  void queueParamFromHostToGui(uint32_t AIndex, sat_param_t AValue) {
    //SAT_Print("%i = %f\n",AIndex,AValue);
    SAT_ParamQueueItem item;
    item.type   = CLAP_EVENT_PARAM_VALUE;
    item.index  = AIndex;
    item.value  = AValue;
    if (!MParamFromHostToGui.write(item)) {
      SAT_PRINT("queueParamFromHostToGui: couldn't write to queue\n");
    }
  }

  //----------

  // called from
  // (todo: gui timer)

  void flushParamFromHostToGui(SAT_ProcessContext* AContext, SAT_Editor* AEditor) {
    SAT_ParameterArray* parameters = AContext->parameters;
    uint32_t count = 0;
    //SAT_Print("\n");
    SAT_ParamQueueItem item;
    while (MParamFromHostToGui.read(&item)) {
      count += 1;
      SAT_Parameter* parameter = parameters->getItem(item.index);
      if (AEditor) AEditor->updateParameterFromHost(parameter,item.value);
    }
    //if (count > 0) { SAT_Print("flushParamFromHostToGui: %i events\n",count); }
    SAT_GLOBAL.ANALYTICS.set_ParamFromHostToGui(count);
  }

  //----------

  // called from
  // SAT_Plugin.on_processorOwner_updateModFromHostToGui
  // (from SAT_Processor.processParamModEvent)

  void queueModFromHostToGui(uint32_t AIndex, sat_param_t AValue) {
    // todo: is_gui_open?
    //SAT_Print("%i = %f\n",AIndex,AValue);
    SAT_ParamQueueItem item;
    item.type   = CLAP_EVENT_PARAM_MOD;
    item.index  = AIndex;
    item.value  = AValue;
    if (!MModFromHostToGui.write(item)) {
      SAT_PRINT("queueModFromHostToGui: couldn't write to queue\n");
    }
  }

  //----------

  // called from
  // (todo: gui timer)

  void flushModFromHostToGui(SAT_ProcessContext* AContext, SAT_Editor* AEditor) {
    SAT_ParameterArray* parameters = AContext->parameters;
    //SAT_Print("\n");
    SAT_ParamQueueItem item;
    uint32_t count = 0;
    while (MModFromHostToGui.read(&item)) {
      count += 1;
      SAT_Parameter* parameter = parameters->getItem(item.index);
      if (AEditor) AEditor->updateModulationFromHost(parameter,item.value);
    }
    //if (count > 0) { SAT_Print("flushModFromHostToGui: %i events\n",count); }
    SAT_GLOBAL.ANALYTICS.set_ModFromHostToGui(count);
  }

  #endif // no gui

//------------------------------
public: // gui -> host
//------------------------------

  #ifndef SAT_NO_GUI

  // called from
  // SAT_Plugin.on_editorListener_update
  // from SAT_EmbeddedEditor.on_windowListener_update)
  //   (SAT_WidgetWindow.on_widgetListener_update)
  //   (SAT_RootWidget.do_widget_update)

  void queueParamFromGuiToHost(uint32_t AIndex, sat_param_t AValue) {
    //SAT_Print("%i = %f\n",AIndex,AValue);
    SAT_ParamQueueItem item;
    item.type   = CLAP_EVENT_PARAM_VALUE;
    item.index  = AIndex;
    item.value  = AValue;
    if (!MParamFromGuiToHost.write(item)) {
      //SAT_Log("queueParamFromGuiToHost: couldn't write to queue\n");
    }
  }

  //----------

  // called from
  // SAT_Plugin.process

  void flushParamFromGuiToHost(SAT_ProcessContext* AContext) {
    SAT_ParameterArray* parameters = AContext->parameters;
    uint32_t count = 0;
    uint32_t blocksize = AContext->process->frames_count;
    const clap_output_events_t* out_events = AContext->process->out_events;

    //SAT_Print("\n");
    SAT_ParamQueueItem item;
    while (MParamFromGuiToHost.read(&item)) {
      count += 1;
      //SAT_Print("%i = %.3f\n",item.index,item.value);
      SAT_Parameter* parameter = parameters->getItem(item.index);
      // gesture begin
      {
        clap_event_param_gesture_t event;
        event.header.size     = sizeof(clap_event_param_gesture_t);
        event.header.time     = blocksize - 1; //0; // should this be (blocksize-1), since we're called at the end of the audioblock?
        event.header.space_id = CLAP_CORE_EVENT_SPACE_ID;
        event.header.type     = CLAP_EVENT_PARAM_GESTURE_BEGIN;
        event.header.flags    = 0;
        event.param_id        = item.index;
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
        event.param_id        = item.index;
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
        event.param_id        = item.index;
        const clap_event_header_t* header = (const clap_event_header_t*)&event;
        out_events->try_push(out_events,header);
      }
    } // while
    //if (count > 0) { SAT_Print("flushParamFromGuiToHost: %i events\n",count); }
    SAT_GLOBAL.ANALYTICS.set_ParamFromGuiToHost(count);
  }

  #endif // no gui

//------------------------------
public: // gui -> audio
//------------------------------

  #ifndef SAT_NO_GUI

  // called from
  // SAT_Plugin.on_editorListener_update
  // (from SAT_EmbeddedEditor.on_windowListener_update)
  //   (SAT_WidgetWindow.on_widgetListener_update)
  //   (SAT_RootWindow.do_widget_update)


  void queueParamFromGuiToAudio(uint32_t AIndex, sat_param_t AValue) {
    //SAT_Print("%i = %f\n",AIndex,AValue);
    SAT_ParamQueueItem item;
    item.type   = CLAP_EVENT_PARAM_VALUE;
    item.index  = AIndex;
    item.value  = AValue;
    if (!MParamFromGuiToAudio.write(item)) {
      //SAT_Log("queueParamFromGuiToAudio: couldn't write to queue\n");
    }
  }

  //----------

  // called from
  // SAT_Plugin.process

  void flushParamFromGuiToAudio(/*SAT_ProcessContext* AContext,*/ SAT_Processor* AProcessor) {
    uint32_t count = 0;
    //SAT_Print("\n");
    SAT_ParamQueueItem item;
    while (MParamFromGuiToAudio.read(&item)) {
      count += 1;
      //SAT_PRINT;
      clap_event_param_value_t event;
      event.header.size     = sizeof(clap_event_param_value_t);
      event.header.time     = 0;
      event.header.space_id = CLAP_CORE_EVENT_SPACE_ID;
      event.header.type     = CLAP_EVENT_PARAM_VALUE;
      event.header.flags    = 0; // CLAP_EVENT_IS_LIVE, CLAP_EVENT_DONT_RECORD
      event.param_id        = item.index;
      event.cookie          = nullptr; // set?
      event.note_id         = -1;
      event.port_index      = -1;
      event.channel         = -1;
      event.key             = -1;
      event.value           = item.value;
      AProcessor->processParamValueEvent(&event);
    }
    //if (count > 0) { SAT_Print("flushParamFromGuiToAudio: %i events\n",count); }
    SAT_GLOBAL.ANALYTICS.set_ParamFromGuiToAudio(count);
  }

  #endif // no gui

//------------------------------
public: // audio -> host
//------------------------------

  // void queueNoteEndFromAudioToGui() {
  // }

  //----------

  // void flushNoteEndsFromAudioToGui() {
  // }

//------------------------------
public: // audio -> gui
//------------------------------

  // timer?

};

//----------------------------------------------------------------------
#endif