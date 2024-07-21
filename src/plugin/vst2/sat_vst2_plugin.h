#ifndef sat_vst2_plugin_included
#define sat_vst2_plugin_included
//----------------------------------------------------------------------

// clap-as-vst2

#define SAT_VST2_MESSAGE_QUEUE_SIZE   32
#define SAT_VST2_MAX_MIDI_SEND        256

//----------

#include "sat.h"
#include "base/type/sat_queue.h"
#include "plugin/lib/sat_clap.h"
#include "plugin/lib/sat_vst2.h"
#include "plugin/lib/sat_vst2_utils.h"
#include "plugin/sat_host_implementation.h"

//----------------------------------------------------------------------


typedef SAT_Queue<uint32_t,SAT_VST2_MESSAGE_QUEUE_SIZE> SAT_Vst2MsgQueue; // was SAT_IntQueue;

struct SAT_VstEvents {
  VstInt32  numEvents;
  VstIntPtr reserved;
  VstEvent* events[SAT_VST2_MAX_MIDI_SEND];
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Vst2Plugin {
  
  friend class SAT_Vst2Entry;

//------------------------------
private:
//------------------------------

  const clap_host_t*              MHost                 = nullptr;
  const clap_plugin_t*            MPlugin               = nullptr;
  const clap_plugin_descriptor_t* MDescriptor           = nullptr;
  AEffect                         MAEffect              = {0};
  audioMasterCallback             MAudioMaster          = nullptr;
  //SAT_Vst2HostImplementation*     MVst2Host             = nullptr;
  SAT_HostImplementation*     MVst2Host             = nullptr;

  const clap_plugin_gui_t*        MGui                  = nullptr;
  const clap_plugin_params_t*     MParams               = nullptr;
  float*                          MParameterValues      = nullptr;

  SAT_Vst2MsgQueue                MProcessMessageQueue  = {};
  SAT_Vst2MsgQueue                MGuiMessageQueue      = {};

  SAT_VstEvents                   MVstEvents;//            = {0};
  VstMidiEvent                    MVstMidiSendEvents[SAT_VST2_MAX_MIDI_SEND];//  = {0};
  
  bool                            MIsOpen               = false;
  bool                            MIsProcessing         = false;
  bool                            MIsEditorOpen         = false;
  bool                            MIsSuspended          = false;
  //bool                            MIsInitialized        = false;
  uint32_t                        MCurrentProgram       = 0;
  uint32_t                        MKnobMode             = 0;
  float                           MSampleRate           = 0.0f;
  uint32_t                        MMaxBlockSize         = 0;
  ERect                           MVstRect;//              = {0};

  //#ifndef SAT_NO_GUI
  //  SAT_Editor*         MEditor             = nullptr;
  //#endif // SAT_NO_GUI

  #ifndef SAT_PLUGIN_VST2_NO_SHELL
    int32_t   MShellPluginLastQueried = -1;
    uint32_t  MShellPluginCurrentId = 0;
  #endif
  

//------------------------------
public:
//------------------------------

  SAT_Vst2Plugin(const clap_host_t* host, const clap_plugin_t* plugin, audioMasterCallback audioMaster) {
    //SAT_PRINT("\n");
    MHost = host;
    MPlugin = plugin;
    MDescriptor = MPlugin->desc;
    MAudioMaster = audioMaster;
    MGui          = (const clap_plugin_gui_t*)MPlugin->get_extension(MPlugin,CLAP_EXT_GUI);
    MParams       = (const clap_plugin_params_t*)MPlugin->get_extension(MPlugin,CLAP_EXT_PARAMS);
    uint32_t num_params = MParams->count(MPlugin);
    MParameterValues = (float*)malloc(num_params * sizeof(float));
    memset(&MVstMidiSendEvents,0,sizeof(MVstMidiSendEvents));
    for (uint32_t i=0; i<SAT_VST2_MAX_MIDI_SEND; i++) {
      MVstEvents.events[i] = (VstEvent*)&MVstMidiSendEvents[i];
    }
  }
  
  //----------
  
  ~SAT_Vst2Plugin() {
    //SAT_PRINT("\n");
    free(MParameterValues);
    if (MPlugin) MPlugin->destroy(MPlugin);
    if (MVst2Host) delete MVst2Host;
  }

//------------------------------
public:
//------------------------------

  AEffect* getAEffect() {
    return &MAEffect;
  }
  
  //----------
  
  // void setHost(SAT_Vst2HostImplementation* AHost) {
  //   MVst2Host = AHost;
  // }
  
  void setHost(SAT_HostImplementation* AHost) {
    MVst2Host = AHost;
  }
  
//------------------------------
private:
//------------------------------

  // called from: vst2_setParameter

  void queueProcessMessage(uint32_t AMessage) {
//    MProcessMessageQueue.write(AMessage);
  }

  //----------

  // called from process()
  // via updateParametersInProcess()
  // TODO: setup for process->in_events

  void flushProcessMessages() {
    uint32_t message = 0;
    while (MProcessMessageQueue.read(&message)) {
//      float value = MParameterValues[message];
//      value = MDescriptor->getParameter(message)->from01(value);
//      MInstance->on_plugin_parameter(message,value);
    }
  }

  //----------

  // called from: vst2_setParameter

  void queueGuiMessage(uint32_t AMessage) {
    MGuiMessageQueue.write(AMessage);
  }

  //----------

  // called from: updateEditorInIdle

  void flushGuiMessages() {
    uint32_t message = 0;
    while (MGuiMessageQueue.read(&message)) {
//      float value = MParameterValues[message];
//      MEditor->updateParameter(message,value);
//      void setParameterValue(message,value,false);
    }
  }

  //----------

  // see vst2_host->flushMidi

  void queueMidiOut(uint32_t AOffset, uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3) {
    int32_t       num   = MVstEvents.numEvents;
    VstMidiEvent* event = &MVstMidiSendEvents[num];
    event->type         = kVstMidiType;
    event->deltaFrames  = AOffset;
    event->midiData[0]  = AMsg1;
    event->midiData[1]  = AMsg2;
    event->midiData[2]  = AMsg3;
    event->midiData[3]  = 0;
    event->byteSize     = sizeof(VstMidiEvent) - 2 * sizeof(int32_t);
    event->flags        = 0; // kVstMidiEventIsRealtime;
    event->noteLength   = 0;
    event->noteOffset   = 0;
    event->detune       = 0;
    num += 1;
    MVstEvents.numEvents = num;
  }

  //----------

  void updateParametersInProcess() {
    flushProcessMessages();
  }

  //----------

  #ifndef SAT_NO_GUI
  void updateEditorInIdle() {
    flushGuiMessages();
  }
  #endif

//------------------------------
public:
//------------------------------

  void vst2_setParameter(VstInt32 index, float parameter) {
    //SAT_PRINT("index %i parameter %f\n",index,parameter);
    MParameterValues[index] = parameter;
    queueProcessMessage(index);
    #ifndef SAT_NO_GUI
      if (MIsEditorOpen) queueGuiMessage(index);
    #endif
  }

  //----------

  float vst2_getParameter(VstInt32 index) {
    //SAT_PRINT("index %i -> %f\n",index,MParameterValues[index]);
    return MParameterValues[index];
    return 0.0;
  }

  //----------

  void vst2_process(float** inputs, float** outputs, VstInt32 sampleFrames) {
    updateParametersInProcess();
//    //MHost->updateTime();
//    uint32_t i;
//    for (i=0; i<MDescriptor->getNumInputs(); i++)  { MProcessContext.inputs[i]  = inputs[i]; }
//    for (i=0; i<MDescriptor->getNumOutputs(); i++) { MProcessContext.outputs[i] = outputs[i]; }
//    MProcessContext.numsamples    = sampleFrames;
//    //MProcessContext.oversample    = 1;
//    MProcessContext.samplerate    = MSampleRate;
//    MProcessContext.samplepos     = MSamplePos;    // getSamplePos();
//    MProcessContext.beatpos       = MBeatPos;
//    MProcessContext.tempo         = MTempo;        // getTempo();
//    MProcessContext.timesignum    = MTimeSigNum;   // getTimeSigNum();
//    MProcessContext.timesigdenom  = MTimeSigDenom; // getTimeSigDenom();
//    MProcessContext.playstate     = MPlayState;
//    MInstance->on_plugin_process(&MProcessContext);
//    //on_postProcess();
//    if (MDescriptor->canSendMidi()) {
//      MVst2Host->flushMidi( (VstEvents*)&MVstEvents );
//      MVstEvents.numEvents = 0;
//    }
  }

  //----------

  void vst2_processDouble(double** inputs, double** outputs, VstInt32 sampleFrames) {
    SAT_PRINT("\n");
  }
    
  //----------

  VstIntPtr vst2_dispatcher(VstInt32 opcode, VstInt32 index, VstIntPtr value, void* ptr, float opt) {
    VstIntPtr result    = 0;
    char*     cptr      = (char*)ptr;
    //char      str[256]  = {0};

    switch (opcode) {

      //--------------------

      /*
        called when plug-in is initialized
        initialise this plugin instance
      */

      case effOpen: { // 0
        //SAT_PRINT("effOpen\n");
        MIsOpen = true;

        //MPlugin->init(MPlugin);

        //MInstance->on_plugin_init();
        //MInstance->setDefaultParameterValues();
        //MInstance->updateAllParameters();
        break;
      }

      //----------

      /*
        called when plug-in will be released
        exit this plugin instance, release all memory and other resources
      */

      case effClose: { // 1
        //SAT_PRINT("effClose\n");
        MIsOpen = false;
        //MInstance->on_plugin_destroy();

        //MPlugin->destroy(MPlugin);

        break;
      }

      //----------

      case effSetProgram: { // 2
        uint32_t program = (uint32_t)value;
        //SAT_PRINT("effSetProgram %i\n",program);
        if (program != MCurrentProgram) {
          //on_programChange(program);
          MCurrentProgram = program;
        }
        break;
      }

      //----------

      /*
        host requests the current program number
        return: current program number
        * reaper calls this repeatedly, 4-5 times per second (if editor open?)
        * EnergyXt calls this before/after each effEditIdle (even if we don't have an editor)
      */

      case effGetProgram: { // 3
        //SAT_PRINT("effGetProgram -> %i\n",MCurrentProgram);
        return MCurrentProgram;
        break;
      }

      //----------

      /*
        host has changed the current program name
        ptr: pointer to new program name (max 24 char + 0)
        stuff the name field of the current program with name.
        Limited to kVstMaxProgNameLen.
      */

      case effSetProgramName: { // 4
        //SAT_PRINT("effSetProgramName '%s'\n",ptr);
        //SAT_Strcpy(MProgramName,txt);
        break;
      }

      //----------

      /*
        host requests the current program name
        ptr: returns string-pointer to program name (max 24 char + 0)
        stuff name with the name of the current program.
        Limited to kVstMaxProgNameLen.
      */

      case effGetProgramName: { // 5
        //SAT_PRINT("effGetProgramName\n");
        *cptr = 0;
        break;
        //SAT_Strcpy((char*)ptr,MProgramName);
        //return 1;
      }

      //----------

      /*
        host requests parameter label
        index: parameter index
        ptr: returns string-pointer to parameter label (max 8 char + 0),
             examples: sec, dB, type
        Stuff label with the units in which parameter index is displayed
        (i.e. "sec", "dB", "type", etc...).
        Limited to kVstMaxParamStrLen.
      */

      case effGetParamLabel: { // 6
        //SAT_PRINT("effGetParamLabel -> ''\n");
        strcpy((char*)ptr,"");
        return 1;
//        break;
      }

      //----------

      /*
        stuff text with a string representation of the value of parameter index.
        ("0.5", "-3", "PLATE", etc...)
        Limited to kVstMaxParamStrLen.
      */

      case effGetParamDisplay: { // 7
        float value = MParameterValues[index];
        if (MParams->value_to_text(MPlugin,index,value,cptr,strlen(cptr))) {
          //SAT_PRINT("effGetParamDisplay %i -> '%s'\n",index,cptr);
          return 1;
        }
        else {
          //SAT_PRINT("effGetParamDisplay %i -> ?'\n",index);
        }
        break;
      }

      //----------

      /*
        stuff text with the name of parameter index.
        ("Time", "Gain", "RoomType", etc...)
        Limited to kVstMaxParamStrLen.
      */

      case effGetParamName: { // 8
        //SAT_Vst2Trace("vst2: dispatcher/effGetParamName %i\n",index);
        clap_param_info_t info;
        if (MParams->get_info(MPlugin,index,&info)) {
          strcpy((char*)ptr,info.name);
          //SAT_PRINT("effGetParamName %i -> '%s'\n",index,ptr);
          return 1;
        }
        else {
          //SAT_PRINT("effGetParamName %i -> ?'\n",index);
        }
        break;
      }

      //----------

      /*
        called when the sample rate changes
        host sets SampleRate
        opt: float value in Hz, for example 44100.0Hz
        (always in a suspend state).
      */

      case effSetSampleRate: { // 10
        //SAT_PRINT("effSetSamplerate %f\n",opt);
        MSampleRate = opt;
//        MInstance->on_sampleRate(MSampleRate);
        break;
      }

      //----------

      /*
        called when the maximun block size changes (always in a suspend state).
        note that the sampleFrames in process calls could be smaller than
        this block size, but NOT bigger.
      */

      case effSetBlockSize: { // 11
        //SAT_PRINT("effSetBlockSize %i\n",value);
        MMaxBlockSize = (VstInt32)value;
        break;
      }

      //----------

      /*
        the host has switched the 'power on' button. This only switches audio
        processing on/off.
        value: 0 = OFF, the plugin must call Resume() which in turn must flush
                   any delay buffers etc. here because otherwise pending data
                   would sound again when the effect is switched on next time.
               1 = ON, the plug must call Suspend()

        This method is called when the effect is turned off by the user.
        ex: The buffer gets flushed here because otherwise pending data would
            sound again when the effect is switched on next time.

        called from non-rt thread..

        I believe resume() is called from a non-real-time thread, so you can
        allocate memory and stuff. startProcessing() is called from a real-time
        thread, so it's supposed to be "fast" in each plug-in. Thus, you can do
        things like taking initial timer measurements, spin up hardware buffers
        if needed, etc. startProcessing() gives you a chance to do the "last half"
        of your process() functionality, if you're interfacing with something that
        needs double-buffering, for example.
        -
        Ah, right, now that I checked it, the documentation says startProcess()/
        stopProcess() are supposed to be called in RT thread, and resume()/
        suspend() are not in the list. Ok fine, that makes sense.
        -
        flush event list(s) on resume?

        suspend: called when plug-in is switched to off
        resume:  called when plug-in is switched to on
      */

      case effMainsChanged: { // 12
        //SAT_PRINT("effMainsChanged %i\n",value);
        //SAT_Vst2Trace("vst2: dispatcher/effMainsChanged %i\n",(int)value);
        if (value == 0) { // suspend
          MIsProcessing = false;
          MIsSuspended = true;
          //MInstance->on_plugin_deactivate();

          SAT_PRINT("deactivate\n");
          MPlugin->deactivate(MPlugin);

        }
        else { // resume
          //if (!MIsInitialized) {
          //  //MInstance->on_prepare();
          //  MIsInitialized = true;
          //}
          MIsProcessing = true;
          MIsSuspended = false;
          //MInstance->on_plugin_activate(MSampleRate,0,MMaxBlockSize);

          SAT_PRINT("activate\n");
          MPlugin->activate(MPlugin,MSampleRate,0,MMaxBlockSize);

        }
        break;
      }

      //----------

      /*
        ptr: pointer to rect*(top, left, bottom, right)
        return: 1 = successful
        called repeatedly by reaper (4-5 times per second)
        NOTE:
          ERect has 16 bit integers.. does it matter?
      */

      case effEditGetRect: { // 13
        //SAT_PRINT("effEditGetRect\n");
        //SAT_Vst2Trace("vst2: dispatcher/effEditGetRect\n");
        if (MGui) {
          uint32_t width  = 0;
          uint32_t height = 0;
          MGui->get_size(MPlugin,&width,&height);
          //if (w == 0) w = MInstance->getDefaultEditorWidth();
          //if (h == 0) h = MInstance->getDefaultEditorHeight();
          MVstRect.left     = 0;
          MVstRect.top      = 0;
          MVstRect.right    = width;
          MVstRect.bottom   = height;
          *(ERect**)ptr     = &MVstRect;
          return 1;
        }
        break;
      }

      //----------

      /*
        ptr: contains system dependant Window handle/pointer
        return: 1 = successful
        ext2/linux:  display ptr (32-bit?)
      */

      case effEditOpen: { // 14
        //SAT_PRINT("effEditOpen\n");
        #ifndef SAT_NO_GUI

          #ifdef SAT_LINUX
          if (MGui && MGui->is_api_supported(MPlugin,CLAP_WINDOW_API_X11,false)) {
          #endif

          #ifdef SAT_WIN32
          if (MGui && MGui->is_api_supported(MPlugin,CLAP_WINDOW_API_WIN32,false)) {
          #endif

            if (!MIsEditorOpen) {
              MIsEditorOpen = true;

              #ifdef SAT_LINUX
              MGui->create(MPlugin,CLAP_WINDOW_API_X11,false);
              #endif

              #ifdef SAT_WIN32
              MGui->create(MPlugin,CLAP_WINDOW_API_WIN32,false);
              #endif

              MGui->set_scale(MPlugin,1.0);
              uint32_t width = 0;
              uint32_t height = 0;
              MGui->get_size(MPlugin,&width,&height);
              MGui->set_size(MPlugin,width,height);
              clap_window_t clap_window = {};

              #ifdef SAT_LINUX
              clap_window.api = CLAP_WINDOW_API_X11;
              clap_window.x11 = (clap_xwnd)ptr;
              #endif

              #ifdef SAT_WIN32
              clap_window.api = CLAP_WINDOW_API_WIN32;
              clap_window.win32 = (clap_hwnd)ptr;
              #endif

              MGui->set_parent(MPlugin,&clap_window);
              //updateAllEditorParameters(MEditor,false);
              MGui->show(MPlugin);
              return 1;
            }
          }
        #endif // SAT_NO_GUI
        break;
          }

      //----------

      /*
      */

      case effEditClose: { // 15
        //SAT_PRINT("effEditClose\n");
        #ifndef SAT_NO_GUI
          if (MGui) {
            if (MIsEditorOpen) {
              MIsEditorOpen = false;
              MGui->hide(MPlugin);
              MGui->destroy(MPlugin);
              return 1;
            }
          }
        #endif // SAT_NO_GUI
        break;
      }

      //----------

      /*
        no arguments. Be gentle!
        (it would be better if this call could be avoided)
        don't push stuff or changes to editor,
        notify editor that we want to change, and do it in the editor
        NOTE:
        EnergyXt calls this even if we don't have our own editor!
        FL studio --"--
        reaper, also in 'pause' state (when processing is not being called)
      */

      case effEditIdle: { // 19
        //SAT_PRINT("effEditIdle\n");
        #ifndef SAT_NO_GUI
//          if (MGui) {
//            if (MIsEditorOpen) {
//              //SAT_Assert(MEditor);
////              MInstance->on_plugin_updateEditor(MEditor);
//              updateEditorInIdle();
//            }
//          }
        #endif // SAT_NO_GUI
        break;
      }

      //----------

      /*
        host requests pointer to a Bank/Program chunk,
        plugin is responsible for memory allocation/de-allocation of chunk
        index: 0 = isBank, 1 = isProgram
        ptr: pointer to chunk memory-block
        return: byteSize

        VstInt32 AudioEffect::getChunk (void** data, bool isPreset)
        data - should point to the newly allocated memory block containg state
        data. You can savely release it in next suspend/resume call.
        isPreset - true when saving a single program, false for all programs
        note: If your plug-in is configured to use chunks (see
        AudioEffect::programsAreChunks) the Host will ask for a block of memory
        describing the current plug-in state for saving.
        To restore the state at a later stage, the same data is passed back to
        AudioEffect::setChunk. Alternatively, when not using chunk, the Host
        will simply save all parameter values.

        SONAR is sending the setChunks on separated cores. (dual-core machine
        here).. Or on separated threads..
        i think:
        - reaper asks for chunk repeatedly (1.5 sec intervals?)
        - bitwig per 5 sec ??
        see:
          "proper way to load and save presets with getChunk setChunk"
          https://www.kvraudio.com/forum/viewtopic.php?p=6477202
          "Correctly restoring a VST plugin current program on host saved project"
          https://www.kvraudio.com/forum/viewtopic.php?p=6330607
          "getChunk question"
          http://www.kvraudio.com/forum/viewtopic.php?p=5530705
          sonigen:
          can you just assign the new chunk and delete the previous one in the
          getChunk call? The host is just going to keep one "chunk" pointer
          isnt it? So as long as that points to the last chunk created
          everything is ok?
          very angry mobster:
          I only use the one chunk and have not noticed any problems yet.
          mystran:
          Yeah, same here.. my chunk is simply an std::string and I just
          replace the contents on every new call to getChunk(), then pass the
          data() and length() back to the host. Never heard of any issues
          anywhere.
      */

      case effGetChunk: { // 23
        //SAT_PRINT("effGetChunk\n");
        //if (index==0) return MInstance->on_saveBank((void**)ptr);
        //else return MInstance->on_saveProgram((void**)ptr);
        {
//          void* buffer = nullptr;
//          uint32_t size = MInstance->on_plugin_saveState(&buffer,0);
//          if ((size == 0) && (buffer == nullptr)) {
//            buffer = MInstance->getParameterValueBuffer();
//            size = MDescriptor->getNumParameters() * sizeof(float);
//          }
//          *(void**)ptr = buffer;
//          return size;
        }
        break;
      }

      //----------

      /*
        plug-in receives a Bank/Program chunk,
        (host restores plug-in state)
        host is responsible for memory allocation/de-allocation of chunk
        index: 0 = isBank, 1 = isProgram
        value: byteSize
        ptr: pointer to chunk memory-block

        VstInt32 AudioEffect::setChunk (void* data, VstInt32 byteSize, bool isPreset)
        data - pointer to state data (owned by Host)
        byteSize - size of state data
        isPreset - true when restoring a single program, false for all programs
      */

      case effSetChunk: { // 24
        //SAT_PRINT("effSetChunk\n");
//          //if (index==0) return MInstance->on_loadBank(ptr,value); // was not retrurn
//          //else  /*if (index==1)*/ return MInstance->on_loadProgram(ptr,value);
//          float* buffer = (float*)ptr;
//          uint32_t num_params = MDescriptor->getNumParameters();;
//          for (uint32_t i=0; i<num_params; i++) {
//            float v = buffer[i];
//            MInstance->setParameterValue(i,v);
//          }
//          MInstance->updateAllParameters();
        break;
      }

      //case effGetChunk: v = getChunk ((void**)ptr, index ? true : false); break;
      //case effSetChunk: v = setChunk (ptr, (VstInt32)value, index ? true : false); break;

//--------------------
// vst 2
//--------------------

      /*
        ptr: pointer to VstEvents*, host usually call ProcessEvents just before
             calling ProcessReplacing
        return: VST2.4: host should ignore return value, <=VST2.3: 0 = 'wants no
                more', else return=1

        assumptions:
          * called just before process(), from the same thread,
            non-overlapping (non-concurrent?)
          * sorted by offset
        todo:
          * check: negative offsets, offsets past buffer
                   concurrency (what happens IF it is called during process()

        Are you taking deltaFrames offsets into account? (Including the fact that
        they can be negative, indicating the event should have happened in the
        last sample buffer. As will necessarily be the case with any real-time
        MIDI input.)

        called repeatedly! by fl studio, even if nothing happens???
        energyxt calls this repeatedly (with num = 0), before each block?
      */

      case effProcessEvents: { // 25
        //SAT_PRINT("effProcessEvents\n");
        //if ((MDescriptor->isSynth()) || (MDescriptor->canReceiveMidi())) {
        //if (strstr(MDescriptor->features,"instrument")) {
          VstEvents* ev = (VstEvents*)ptr;
          int num_events = ev->numEvents;
          for (int32_t i=0; i<num_events; i++) {
            VstMidiEvent* event = (VstMidiEvent*)ev->events[i];
            if (event->type == kVstMidiType) {
              // todo: buffer, handle all in process..
//              MInstance->on_plugin_midi(event->deltaFrames,event->midiData[0],event->midiData[1],event->midiData[2]);
            }
          }
        //}
        // todo: sort?
        return 1;
      }

      //----------

      /*
        hosts asks if parameter can be automated, this should be called each time
        before attempting to call SetParameter()
        index: parameter index
        return: 1 = parameter can be automated
      */

      case effCanBeAutomated: { // 26
        //SAT_PRINT("effCanBeAutomated\n");
        //if (MParams) {
        //  clap_param_info_t info;
        //  MParams->get_info(MPlugin,index,&info);
        //  if (info.flags & CLAP_PARAM_IS_MODULATABLE) {}
        //}

        // assume all parameters can be automated,
        // TODO: not if hidden?
        return 1;
      }

      //----------

      /*
        host must call this two-fold:
        1) host checks the capability of plugin if it can convert a string into a parameter float value
            index: parameter index
            ptr: NULL
            return: 1 = plug CAN convert string2Parameter,
                    0 = plug CANNOT convert string2Parameter
        2) host requests plug to set a parameter from the parameter textual representation into a parameter
           float value (range 0.0 to 1.0), if this conversion is successful then internally the plugin must
           call setParameter as part of this call
            index: parameter index
            ptr: string-pointer to textual representation of parameter
            return: 1 = conversion successful,
                    0 = fail
      */

      case effString2Parameter: { // 27
        //SAT_PRINT("effString2Parameter\n");
        //TODO
        break;
      }

      //----------

      /*
        host requests program name
        index: program index
        value: deprecated in VST2.4
        ptr: returns string-pointer to program name (max 24 char + 0)
        return: 1 = OK, 0 = fail
        energy xt 2 calls this before effClose ??? (investigate)
      */

      case effGetProgramNameIndexed: { // 29
        //SAT_PRINT("effGetProgramNameIndexed\n");
        //KStrcpy((char*)ptr,MProgramName);
        //return (VstIntPtr)MPrograms[index]->getName();
        strncpy((char*)ptr,"default",24);
        break;
      }

      //----------

      /*
        struct VstPinProperties {
          char label[kVstMaxLabelLen];	          // pin name
          VstInt32 flags;					                // @see VstPinPropertiesFlags
          VstInt32 arrangementType;		            // @see VstSpeakerArrangementType
          char shortLabel[kVstMaxShortLabelLen];  // short name (recommended: 6 + delimiter)
          char future[48];				                // reserved for future use
        };
        enum VstPinPropertiesFlags {
          kVstPinIsActive   = 1 << 0,   // pin is active, ignored by Host
          kVstPinIsStereo   = 1 << 1,   // pin is first of a stereo pair
          kVstPinUseSpeaker = 1 << 2    // #VstPinProperties::arrangementType is valid and can be used to get the wanted arrangement
        };
        enum VstSpeakerArrangementType {
          kSpeakerArrUserDefined = -2,  // user defined
          kSpeakerArrEmpty = -1,        // empty arrangement
          kSpeakerArrMono  =  0,        // M
          kSpeakerArrStereo,            // L R
          kSpeakerArrStereoSurround,    // Ls Rs
          ...
        };
      */

      //TODO
      case effGetInputProperties: { // 33
        //SAT_PRINT("effGetInputProperties\n");
        VstPinProperties* pin = (VstPinProperties*)ptr;
        /* char* pc = */ strcpy(pin->label,"input "); // returns ptr to end of string? ptr to the '\0', or after the 0?
        /* *pc++ = SAT_HEX_TABLE[index&0x0f];
        *pc = 0; */
        pin->flags = 1; // active
        if ((index & 1) == 0) pin->flags |= 2; // first of stereo pair
        return 1;
      }

      //----------

      /*
      */

      //TODO
      case effGetOutputProperties: { // 34
        //SAT_PRINT("effGetOutputProperties\n");
        VstPinProperties* pin = (VstPinProperties*)ptr;
        /* char* pc = */ strcpy(pin->label,"output ");
        /* *pc++ = SAT_HEX_TABLE[index&0x0f];
        *pc = 0; */
        pin->flags = 1; // active
        if ((index & 1) == 0) pin->flags |= 2; // first of stereo pair
        return 1;
        break;
      }

      //----------

      /*
        host requests plugin category
        return: category number:
           0 = kPlugCategUnknown        Unknown, category not implemented
           1 = kPlugCategEffect         Simple Effect,
           2 = kPlugCategSynth          VST Instrument (Synths, samplers,...)
           3 = kPlugCategAnalysis       Analysis. Scope, Tuner, ...
           4 = kPlugCategMastering      Mastering, Dynamics, ...
           5 = kPlugCategSpacializer    Spacializer (panners),
           6 = kPlugCategRoomFx         RoomFx (delays and reverbs),
           7 = kPlugSurroundFx          SurroundFx (dedicated surround processors),
           8 = kPlugCategRestoration    Restoration, denoiser
           9 = kPlugCategOfflineProcess OfflineProcess,
          10 = kPlugCategShell          Shell (is only a container of plugins), @see effShellGetNextPlugin
          11 = kPlugCategGenerator      Generator, TonGenerator, ..
      */

      case effGetPlugCategory: { // 35
        uint32_t res = 0;
        //SAT_Plugin* plugin = (SAT_Plugin*)MPlugin->plugin_data;

        #ifndef SAT_PLUGIN_VST2_NO_SHELL
          if (MShellPluginCurrentId == 0) {
            if (SAT_GLOBAL.REGISTRY.getNumDescriptors() > 1) {
              //SAT_PRINT("effGetPlugCategory -> shell\n");
              res = kPlugCategShell;
              MShellPluginLastQueried = 0;
            }
          }
          else
        #endif
        {
          if (strstr(MDescriptor->id,"instrument")) res = kPlugCategSynth;
          else if (strstr(MDescriptor->id,"audio_effect")) res = kPlugCategEffect;
          //if (MPlugin->hasFlag(kpf_tool)) res = kPlugCategGenerator;
          //SAT_PRINT("effGetPluginCategory -> %i\n",res);
        }
        return res;
      }

      //----------

      /*
        index: start flag
        value: count
        ptr: pointer to VstAudioFile array
        return: 1 = successful
      */

      case effOfflineNotify: { // 38
        //SAT_Vst2Trace("vst2: dispatcher/effOfflineNotify\n");
        break;
      }

      //----------

      /*
        value: count
        ptr: pointer to VstOfflineTask array
        return: 1 = successful
      */

      case effOfflinePrepare: { // 39
        //SAT_PRINT("effOfflinePrepare\n");
        break;
      }

      //----------

      /*
        value: count
        ptr: pointer to VstOfflineTask array
        return: 1 = successful
      */

      case effOfflineRun: { // 40
        //SAT_PRINT("effOfflineRun\n");
        break;
      }

      //----------

      /*
        ptr: pointer to VariableIO*
        return: 1 = successful
      */

      case effProcessVarIo: { // 41
        //SAT_PRINT("effProcessVarIo\n");
        break;
      }

      //----------

      /*
        value: pointer to VstSpeakerArrangement* pluginInput
        ptr: pointer to VstSpeakerArrangement* pluginOutput
        return: 1 = successful
      */

      case effSetSpeakerArrangement: { // 42
        //SAT_PRINT("effSetSpeakerArrangement\n");
        break;
      }

      //----------

      /*
        the host has switched the 'softBypass' state
        value: onOff (0 = off, 1 = On)
        return: 1 = successful
        ---
        returns true if the plug-in supports SoftBypass, which allows the process
        to be called, even if the plug-in was bypassed. This is extremely useful
        if the plug-in should be able to maintain a processing state even when
        turned off, e.g. surround decoders/encoders. Moreover, this feature may be
        automated by the host.
      */

      case effSetBypass: { // 44
        //SAT_PRINT("effSetBypass %i\n",value);
        return 1;
      }

      //----------

      /*
        ptr: returns string-pointer to a string with effect name (max 32 bytes)
        return: 1 = successful
      */

      case effGetEffectName: { // 45
        //SAT_PRINT("effGetEffectName -> '%s'\n",MDescriptor->name);
        strcpy((char*)ptr,MDescriptor->name);
//        strcpy(str,MDescriptor->name);
        //#ifdef SAT_32BIT
        //  str += "_32";
        //#endif
        //#ifdef SAT_64BIT
        //  str += "_64";
        //#endif
//        #ifdef SAT_DEBUG
//          strcat(str,"_debug"); // 32-6 = max 26....
//        #endif
//        strcpy((char*)ptr,(char*)str);
        return 1;
      }

      //----------

      /*
        ptr: returns string-pointer to a string identifying the vendor (max 64 ch)
        return: 1 = successful
      */

      case effGetVendorString: { // 47
        //SAT_PRINT("effGetVendorString -> '%s'\n",MDescriptor->vendor);
        strcpy((char*)ptr,(char*)MDescriptor->vendor);
        break;
      }

      //----------

      /*
        ptr: returns string-pointer to a string with product name (max 64 char)
        return: 1 = successful
      */

      case effGetProductString: { // 48
        //SAT_PRINT("effGetProductStringn -> '%s'",MDescriptor->description);
        strcpy((char*)ptr,(char*)MDescriptor->description);
        break;
      }

      //----------

      /*
        return: vendor-specific version
        hmm.. clap has version string, vst2 wants uint32
        a) parse clap version string :-/
        b) hash version string
      */

      case effGetVendorVersion: { // 49
        //SAT_PRINT("effGetVendorVersion\n");
        return 1;//MDescriptor->getVersion();
        //break;
      }

      //----------

      /*
        no definition, vendor specific handling
        The MouseWheel handling is a bit of an after thought, and has been
        implemented by Stenberg via the effVendorSpecific call as follows:
        index: 'stCA' / $73744341
        value: 'Whee' / $57686565
        ptr: 1.0 = wheel-up, -1.0 = wheel-down
        return: 1 = MouseWheel used

        case effVendorSpecific:
          if (index == effGetParamDisplay && ptr) {
            if (value>=0 && value<NUM_PARAMS) {
              sprintf(ptr,"%f",opt);
              return 0xbeef;
            }
          }
      */

      case effVendorSpecific: { // 50
        //SAT_PRINT("effVendorSpecific\n");
        break;
      }

      //----------

     /*
        host queries a plugin 'cando' capability
        ptr: string-pointer to cando string
        return: 1='cando', 0='don't know' (default), -1='No'

        Bitwig asks about:
          midiSingleNoteTuningChange
          midiKeyBasedInstrumentControl
          MPE

        Jost asks about:
          plugAsChannelInsert
          plugAsSend
          mixDryWet

        Reaper asks about:
          hasCockosNoScrollUI
          hasCockosExtensions

        see: MIDI specifications for Multidimensional Polyphonic Expression (MPE)
        https://docs.google.com/document/d/1-26r0pVtVBrZHM6VGA05hpF-ij5xT6aaXY9BfDzyTx8/edit?pli=1#

        reaper calls this after resume & start plugin..
      */

      case effCanDo: { // 51
        //SAT_PRINT("effCanDo '%s'\n",ptr);
        char* p = (char*)ptr;

        // plug-in will send Vst/MIDI events to Host
        if (strcmp(p,"sendVstEvents")) { return 0; }
        if (strcmp(p,"sendVstMidiEvent")) { return 0; }

        // plug-in can receive Vst/MIDI events to Host
        if (strcmp(p,"receiveVstEvents")) { return 1; }
        if (strcmp(p,"receiveVstMidiEvent")) { return 1; }

        // plug-in can receive Time info from Host
        if (strcmp(p,"receiveVstTimeInfo")) { return 1; }

        // plug-in supports offline functions (#offlineNotify, #offlinePrepare, #offlineRun)
        if (strcmp(p,"offline")) { return 0; }

        // plug-in supports function #getMidiProgramName ()
        if (strcmp(p,"midiProgramNames")) { return 0; }

        // plug-in supports function setBypass()
        if (strcmp(p,"bypass")) { return 0; }
        if (strcmp(p,"MPE")) { return 0; }

        // nope.. ask host about this...
        //if (strcmp(p,"shellCategory")) { return 1; }

        /*
          http://www.reaper.fm/sdk/vst/vst_ext.php
          A REAPER aware VST plug-in can respond to effCanDo "hasCockosExtensions",
          with 0xbeef0000 (returning this value), which will inform the host that
          it can call certain extended calls. A plug-in that responds to
          "hasCockosExtensions" may choose to implement any subset of the extended
          calls.
        */
        if (strcmp(p,"hasCockosExtensions")) { return 0xbeef0000; }

        return 0;
      }

      //----------

      /*
        returns tail size; 0 is default (return 1 for 'no tail')
        called a lot by Bitwig
      */

      case effGetTailSize: { // 52
        //SAT_PRINT("effGetTailSize -> 0\n");
        //if (MTail == -1) return 1;
        return 0;
        //break;
      }

      //----------

      /*
        host sends this idle call in response to plugin's 'audioMasterneedIdle'
        request
        return: 1 must be returned to keep idle calls being issued by host

        energyXt2 calls this
        reaper too, if editor is open?
        reaper calls this even if we don't have an editor..
      */

      //#ifndef SAT_PLUGIN_VST2_VESTIGE
      //case DECLARE_VST_DEPRECATED(effIdle): // 53
      //  //SAT_Vst2Trace("vst2: dispatcher/effIdle (deprecated)\n");
      //  break;
      //#endif

      //----------

      /*
        of parameter <index>, VstParameterProperties* in <ptr>
        return=1 when successful
      */

      case effGetParameterProperties: { // 56
        //SAT_PRINT("effGetParameterProperties %i\n",index);
        break;
      }

      //----------

      /*
        returns 2; older versions return 0
      */

      case effGetVstVersion: { // 58
        //SAT_PRINT("effGetVstVersion -> 2\n");
        //SAT_Vst2Trace("vst2: dispatcher/effGetVstVersion\n");
        return 2;
        //break;
      }

//--------------------
// vst 2.1
//--------------------

      /*
        index: character
        value: virtual key-code
        opt: key-modifiers
        return: 0 = KeyDown not used, 1 = KeyDown used
      */

      case effEditKeyDown: { // 59
        //SAT_Vst2Trace("vst2: dispatcher/effEditKeyDown %i %i %i\n",index,(int)value,(int)opt);
        break;
      }

      //----------

      /*
        index: character
        value: virtual key-code
        opt: key-modifiers
        return: 0 = not KeyUp used, 1 = KeyUp used
      */

      case effEditKeyUp: { // 60
        //SAT_Vst2Trace("vst2: dispatcher/effEditKeyUp %i %i %i\n",index,(int)value,(int)opt);
        break;
      }

      //----------

      /*
        value: knobmode 0=circular, 1=circular relativ, 2=linear
        return: 1 = successful
      */

      case effSetEditKnobMode: { // 61
        //SAT_Vst2Trace("vst2: dispatcher/effSetEditKnobMode %i\n",(int)value);
        MKnobMode = (uint32_t)value;
        //return 1;
        break;
      }

      //----------

      /*
        ptr: passed points to MidiProgramName struct.
        ptr: struct will be filled with information for 'thisProgramIndex'.
        return: number of used programIndexes.
                if 0 is returned, no MidiProgramNames supported.
      */

      case effGetMidiProgramName: { // 62
        ////SAT_Vst2Trace("vst2: dispatcher/effGetMidiProgramName\n");
        break;
      }

      //----------

      /*
        ptr: passed points to MidiProgramName struct.
        ptr: struct will be filled with information for the current program.
        return: the programIndex of the current program.
      */

      case effGetCurrentMidiProgram: { // 63
        //SAT_Vst2Trace("vst2: dispatcher/effGetCurrentMidiProgram\n");
        break;
      }

      //----------

      /*
        ptr: passed points to MidiProgramCategory struct.
        ptr: struct will be filled with information for 'thisCategoryIndex'.
        return: number of used categoryIndexes.
                if 0 is returned, no MidiProgramCategories supported.
      */

      case effGetMidiProgramCategory: { // 64
        //SAT_Vst2Trace("vst2: dispatcher/effGetMidiProgramCategory\n");
        break;
      }

      //----------

      /*
        return 1 if the MidiProgramNames or MidiKeyNames had changed on this
        channel, 0 otherwise. <ptr> ignored.
      */

      case effHasMidiProgramsChanged: { // 65
        //SAT_Vst2Trace("vst2: dispatcher/effHasMidiProgramsChanged\n");
        break;
      }

      //----------

      /*
        ptr: passed points to MidiKeyName struct.
        ptr: struct will be filled with information for 'thisProgramIndex' and 'thisKeyNumber'.
      */

      case effGetMidiKeyName: { // 66
        //SAT_Vst2Trace("vst2: dispatcher/effGetMidiKeyName\n");
        break;
      }

      //----------

      /*
        called before all setParameter calls when changing program..
        host calls this before a new program (effSetProgram) is loaded
      */

      case effBeginSetProgram: { // 67
        //SAT_Vst2Trace("vst2: dispatcher/effBeginSetProgram\n");
        break;
      }

      //----------

      /*
        host calls this after the new program (effSetProgram) has been loaded
      */

      case effEndSetProgram: { // 68
        //SAT_Vst2Trace("vst2: dispatcher/effEndSetProgram\n");
        break;
      }

//--------------------
// vst 2.3
//--------------------

      /*
        value: pointer to VstSpeakerArrangement** pluginInput
        ptr: pointer to VstSpeakerArrangement** pluginOutput
      */

      case effGetSpeakerArrangement: { // 69
        //SAT_PRINT("effGetSpeakerArrangement\n");
        break;
      }

      //----------

      /*
        This opcode is only called, if plugin is of type kPlugCategShell.
        <ptr> points to a char buffer of size 64, which is to be filled with the
        name of the plugin including the terminating zero.
        return: the next plugin's uniqueID.
      */

      // OUCH
      // here we reuse indeo 0, instead of having an initial empty/shell..
      // seems like this causes a recursive loop in reaper..
      // bitwig has some safety mechanisms maybe?
      // todo: fix this!

      case effShellGetNextPlugin: { // 70
        #ifndef SAT_PLUGIN_VST2_NO_SHELL
          if (MShellPluginLastQueried >= (int32_t)SAT_GLOBAL.REGISTRY.getNumDescriptors()) return 0;
          const clap_plugin_descriptor_t* descriptor = SAT_GLOBAL.REGISTRY.getDescriptor(MShellPluginLastQueried);
          MShellPluginLastQueried += 1;
          const char* name = descriptor->name;
          uint32_t unique_id = sat_vst2_create_unique_id(descriptor);
          strncat((char*)ptr,name,64);
          SAT_PRINT("effShellGetNextPlugin %i name '%s' id %i\n",MShellPluginLastQueried,name,unique_id);
          return unique_id;
        #endif
        break;
      }

      //----------

      /*
        host calls this before the start of process call and after e.g. resume()

        http://ygrabit.steinberg.de/~ygrabit/public_html/vstsdk/OnlineDoc/vstsdk2.3/html/plug/2.0/AudioEffectX.html#startProcess
        Called one time before the start of process call.
        example:
          resume () // the user activates the plugin (User Interface Thread)
          startProcess () // Audio Thread
          process () // or ProcessReplacing ()
          process ()
          process ()
          stopProcess () // something has stopped the engine...or delayed the call
          startProcess ()
          process ()
          process ()
          process ()
          stopProcess () // yet the user has really stopped the plugin (Off)
          suspend ()

        pre vst 2.3 hosts don't call this, do they?
        should we handle it in suspend/resume?

        ext2 2.5 doesn't call this
        http://www.kvraudio.com/forum/viewtopic.php?t=277899
      */

      case effStartProcess: { //71
        //SAT_PRINT("effStartProcess\n");
        MIsProcessing = true;
        break;
      }

      //----------

      /*
        host calls this after the stop of process call and before e.g. suspend()
        - not called in energy xt2 linux
      */

      case effStopProcess: { // 72
        //SAT_PRINT("effStopProcess\n");
        MIsProcessing = false;
        break;
      }

      //----------

      /*
        Called in offline (non RealTime) Process before process is called,
        indicates how many sample will be processed
      */

      case effSetTotalSampleToProcess: { // 73
        //SAT_Vst2Trace("vst2: dispatcher/effSetTotalSampleToProcess\n");
        break;
      }

      //----------

      /*
        PanLaw:
          Type (Linear, Equal Power,.. see enum PanLaw Type) in <value>,
          Gain in <opt>: for Linear :
          [1.0 => 0dB PanLaw], [~0.58 => -4.5dB], [0.5 => -6.02dB]
      */

      case effSetPanLaw: { // 74
        //SAT_Vst2Trace("vst2: dispatcher/effSetPanLaw %i %.3f\n",(int)value,opt);
        break;
      }

      //----------

      /*
        Called before a Bank is loaded, <ptr> points to VstPatchChunkInfo structure
        return: -1 if the Bank cannot be loaded, 1 if it can be loaded, 0 (for compatibility)
        ala beginSetProgram
        called by: ableton live
      */

      case effBeginLoadBank: { // 75
        //SAT_Vst2Trace("vst2: dispatcher/effBeginLoadBank\n");
        break;
      }

      //----------

      /*
        Called before a Program is loaded, <ptr> points to VstPatchChunkInfo structure
        return: -1 if the Program cannot be loaded, 1 if it can be loaded, 0 (for compatibility)
      */

      case effBeginLoadProgram: { // 76
        //SAT_Vst2Trace("vst2: dispatcher/effBeginLoadProgram\n");
        break;
      }

//--------------------
// vst 2.4
//--------------------

      /*
        value
        Symbolic precision constants used for effSetProcessPrecision.
      */

      case effSetProcessPrecision: { // 77
        //SAT_PRINT("effSetprocessPrecision\n");
        break;
      }

      //----------

      /*
        return: number of used MIDI input channels (1-15)
      */

      case effGetNumMidiInputChannels: { // 78
        //SAT_Vst2Trace("vst2: dispatcher/effGetNumMidiInputChannels\n");
        break;
      }

      //----------

      /*
        return: number of used MIDI output channels (1-15)
      */

      case effGetNumMidiOutputChannels: { // 79
        //SAT_Vst2Trace("vst2: dispatcher/effGetNumMidiOutputChannels\n");
        break;
      }

      //----------

      default: {
        //SAT_Vst2Trace("unhandled opcode: %i\n",opcode);
        break;
      }

      //--------------------

    } // switch
    return result;
    
  }

};

//----------------------------------------------------------------------
#endif
