#ifndef kode_dssi_instance_included
#define kode_dssi_instance_included
//----------------------------------------------------------------------

#include "kode.h"
#include "plugin/kode_instance.h"
#include "plugin/kode_process_context.h"
//#include "plugin/kode_instance_listener.h"

class KODE_DssiInstance
/*: public KODE_InstanceListener*/ {

//----------
//------------------------------
private:
//------------------------------

  KODE_Instance*          MInstance           = nullptr;
  KODE_Descriptor*        MDescriptor         = nullptr;
  float**                 MInputPtrs          = nullptr;
  float**                 MOutputPtrs         = nullptr;
  float**                 MParameterPtrs      = nullptr;
  DSSI_Program_Descriptor MProgramDescriptor  = {0};
  uint32_t                MCurrentBank        = 0;
  uint32_t                MCurrentProgram     = 0;
  uint32_t                MNumInputs          = 0;
  uint32_t                MNumOutputs         = 0;
  uint32_t                MNumParameters      = 0;
  float                   MSampleRate         = 0.0f;

  KODE_ProcessContext     MProcessContext     = {0};


  //KODE_Rect               MEditorRect         = KODE_Rect(0);
  //float*                  MHostValues     = nullptr;
  //float*                  MProcessValues  = nullptr;

//------------------------------
public:
//------------------------------

  KODE_DssiInstance(KODE_Instance* AInstance, uint32_t ASampleRate) {
    MInstance = AInstance;
    MDescriptor = AInstance->getDescriptor();
    MSampleRate = ASampleRate;
    MNumInputs      = MDescriptor->getNumInputs();
    MNumOutputs     = MDescriptor->getNumOutputs();
    MNumParameters  = MDescriptor->getNumParameters();
    MInputPtrs      = (float**)malloc(MNumInputs     * sizeof(float*));
    MOutputPtrs     = (float**)malloc(MNumOutputs    * sizeof(float*));
    MParameterPtrs  = (float**)malloc(MNumParameters * sizeof(float*));
    //MHostValues     = (float*) KODE_Malloc(MNumParameters * sizeof(float ));
    //MProcessValues  = (float*) KODE_Malloc(MNumParameters * sizeof(float ));
  }

  virtual ~KODE_DssiInstance() {
    if (MInstance)      delete MInstance;
    if (MInputPtrs)     free(MInputPtrs);
    if (MOutputPtrs)    free(MOutputPtrs);
    if (MParameterPtrs) free(MParameterPtrs);
    //if (MHostValues)    free(MHostValues);
    //if (MProcessValues) free(MProcessValues);
  }

//------------------------------
public:
//------------------------------

  //void setSampleRate(float ASampleRate) {
  //  MSampleRate = ASampleRate;
  //}

//------------------------------
private:
//------------------------------

  void updateParametersInProcess(void) {
    for (uint32_t i=0; i<MNumParameters; i++) {
     float v = *MParameterPtrs[i];
      if (v != MInstance->getParameterValue(i)) {
        MInstance->setParameterValue(i,v);
        KODE_Parameter* param = MDescriptor->getParameter(i);
        v = param->from01(v);
//        MInstance->on_parameterChange(i,v);
      }
    }
  }

  //----------

  void updateMidiInputInProcess(snd_seq_event_t* Events, unsigned long EventCount) {
    uint32_t event_index = 0;
    while ((event_index < EventCount)) {
      snd_seq_event_t* event = &Events[event_index];
      uint32_t ofs = event->time.tick;
      uint32_t chn = 0;
      uint32_t idx = 0;
      uint32_t val = 0;
      switch (event->type) {
        case SND_SEQ_EVENT_NOTEOFF:
          // note off
          chn = event->data.note.channel;
          idx = event->data.note.note;
          val = event->data.note.velocity;
          MInstance->on_plugin_midi(ofs,0x80+chn,idx,val);
          break;
        case SND_SEQ_EVENT_NOTEON:
          // note on
          chn = event->data.note.channel;
          idx = event->data.note.note;
          val = event->data.note.velocity;
//          if (val > 0) MInstance->on_midiInput(ofs,0x90+chn,idx,val);
//          else MInstance->on_midiInput(ofs,0x80+chn,idx,val);
          break;
        case SND_SEQ_EVENT_KEYPRESS:
          // polyphonic key pressure (aftertouch)
          chn = event->data.note.channel;
          idx = event->data.note.note;
          val = event->data.note.velocity;
//          MInstance->on_midiInput(ofs,0xA0+chn,idx,val);
          break;
        case SND_SEQ_EVENT_CONTROLLER:
          //control_change
          chn = event->data.control.channel;
          idx = event->data.control.param;
          val = event->data.control.value;
//          MInstance->on_midiInput(ofs,0xB0+chn,idx,val);
          break;
        //case // 0xC0 = program change
        //  break;
        case SND_SEQ_EVENT_CHANPRESS:
          //channel_pressure;
          chn = event->data.control.channel;
          idx = 0;
          val = event->data.control.value;
//          MInstance->on_midiInput(ofs,0xD0+chn,0,val);
          break;
        case SND_SEQ_EVENT_PITCHBEND:
          //pitch_bend
          chn = event->data.control.channel;
          idx = 0;
          val = event->data.control.value;
//          MInstance->on_midiInput(ofs,0xE0+chn,0,val);
          break;
      }
      event_index++;
    }
  }

//------------------------------
public:
//------------------------------

  void dssi_connect_port(unsigned long Port, LADSPA_Data* DataLocation) {
    if (Port < MNumInputs) {
      MInputPtrs[Port] = (float*)DataLocation;
      return;
    }
    Port -= MNumInputs;
    if (Port < MNumOutputs) {
      MOutputPtrs[Port] = (float*)DataLocation;
      return;
    }
    Port -= MNumOutputs;
    if (Port < MNumParameters) {
      MParameterPtrs[Port] = (float*)DataLocation;
      return;
    }
    Port -= MNumParameters;
  }

  //----------

  void dssi_activate() {
//    MInstance->on_activate();
  }

  //----------

  void dssi_run(unsigned long SampleCount) {
    updateParametersInProcess();

    // setup...

    MInstance->on_plugin_process(&MProcessContext);
    //on_processBlock(MInputPtrs,MOutputPtrs,SampleCount);
  }

  //----------

  //void dssi_run_adding(unsigned long SampleCount) {
  //  //DSSI_Trace("dssi: run_adding SampleCount:%i\n",SampleCount);
  //}

  //----------

  //void dssi_set_run_adding_gain(LADSPA_Data Gain) {
  //  //DSSI_Trace("dssi: set_run_adding_gain Gain:%.3f\n",Gain);
  //}

  //----------

  void dssi_deactivate() {
//    MInstance->on_deactivate();
  }

  //----------

  void dssi_cleanup() {
//    MInstance->on_terminate();
//    MInstance->on_close();
  }

//------------------------------
// dssi
//------------------------------

public:

    /**
     * configure()
     *
     * This member is a function pointer that sends a piece of
     * configuration data to the plugin.  The key argument specifies
     * some aspect of the synth's configuration that is to be changed,
     * and the value argument specifies a new value for it.  A plugin
     * that does not require this facility at all may set this member
     * to NULL.
     *
     * This call is intended to set some session-scoped aspect of a
     * plugin's behaviour, for example to tell the plugin to load
     * sample data from a particular file.  The plugin should act
     * immediately on the request.  The call should return NULL on
     * success, or an error string that may be shown to the user.  The
     * host will free the returned value after use if it is non-NULL.
     *
     * Calls to configure() are not automated as timed events.
     * Instead, a host should remember the last value associated with
     * each key passed to configure() during a given session for a
     * given plugin instance, and should call configure() with the
     * correct value for each key the next time it instantiates the
     * "same" plugin instance, for example on reloading a project in
     * which the plugin was used before.  Plugins should note that a
     * host may typically instantiate a plugin multiple times with the
     * same configuration values, and should share data between
     * instances where practical.
     *
     * Calling configure() completely invalidates the program and bank
     * information last obtained from the plugin.
     *
     * Reserved and special key prefixes
     * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     * The DSSI: prefix
     * ----------------
     * Configure keys starting with DSSI: are reserved for particular
     * purposes documented in the DSSI specification.  At the moment,
     * there is one such key: DSSI:PROJECT_DIRECTORY.  A host may call
     * configure() passing this key and a directory path value.  This
     * indicates to the plugin and its UI that a directory at that
     * path exists and may be used for project-local data.  Plugins
     * may wish to use the project directory as a fallback location
     * when looking for other file data, or as a base for relative
     * paths in other configuration values.
     *
     * The GLOBAL: prefix
     * ------------------
     * Configure keys starting with GLOBAL: may be used by the plugin
     * and its UI for any purpose, but are treated specially by the
     * host.  When one of these keys is used in a configure OSC call
     * from the plugin UI, the host makes the corresponding configure
     * call (preserving the GLOBAL: prefix) not only to the target
     * plugin but also to all other plugins in the same instance
     * group, as well as their UIs.  Note that if any instance
     * returns non-NULL from configure to indicate error, the host
     * may stop there (and the set of plugins on which configure has
     * been called will thus depend on the host implementation).
     * See also the configure OSC call documentation in RFC.txt.
     */

  char* dssi_configure(const char* Key, const char* Value) {
    return nullptr;
  }

    /**
     * get_program()
     *
     * This member is a function pointer that provides a description
     * of a program (named preset sound) available on this synth.  A
     * plugin that does not support programs at all should set this
     * member to NULL.
     *
     * The Index argument is an index into the plugin's list of
     * programs, not a program number as represented by the Program
     * field of the DSSI_Program_Descriptor.  (This distinction is
     * needed to support synths that use non-contiguous program or
     * bank numbers.)
     *
     * This function returns a DSSI_Program_Descriptor pointer that is
     * guaranteed to be valid only until the next call to get_program,
     * deactivate, or configure, on the same plugin instance.  This
     * function must return NULL if passed an Index argument out of
     * range, so that the host can use it to query the number of
     * programs as well as their properties.
     */

  /*
    This function returns a DSSI_Program_Descriptor pointer that is
    guaranteed to be valid only until the next call to get_program,
    deactivate, or configure, on the same plugin instance.  This
    function must return NULL if passed an Index argument out of
    range, so that the host can use it to query the number of
    programs as well as their properties.
  */

  const DSSI_Program_Descriptor* dssi_get_program(unsigned long Index) {
    MProgramDescriptor.Bank = 0;
    MProgramDescriptor.Program = 0;
    MProgramDescriptor.Name = "name";
    return nullptr;
  }

    /**
     * select_program()
     *
     * This member is a function pointer that selects a new program
     * for this synth.  The program change should take effect
     * immediately at the start of the next run_synth() call.  (This
     * means that a host providing the capability of changing programs
     * between any two notes on a track must vary the block size so as
     * to place the program change at the right place.  A host that
     * wanted to avoid this would probably just instantiate a plugin
     * for each program.)
     *
     * A plugin that does not support programs at all should set this
     * member NULL.  Plugins should ignore a select_program() call
     * with an invalid bank or program.
     *
     * A plugin is not required to select any particular default
     * program on activate(): it's the host's duty to set a program
     * explicitly.  The current program is invalidated by any call to
     * configure().
     *
     * A plugin is permitted to re-write the values of its input
     * control ports when select_program is called.  The host should
     * re-read the input control port values and update its own
     * records appropriately.  (This is the only circumstance in
     * which a DSSI plugin is allowed to modify its own input ports.)
     */

  void dssi_select_program(unsigned long Bank, unsigned long Program) {
    MCurrentBank = Bank;
    MCurrentProgram = Program;
  }

    /**
     * get_midi_controller_for_port()
     *
     * This member is a function pointer that returns the MIDI
     * controller number or NRPN that should be mapped to the given
     * input control port.  If the given port should not have any MIDI
     * controller mapped to it, the function should return DSSI_NONE.
     * The behaviour of this function is undefined if the given port
     * number does not correspond to an input control port.  A plugin
     * that does not want MIDI controllers mapped to ports at all may
     * set this member NULL.
     *
     * Correct values can be got using the macros DSSI_CC(num) and
     * DSSI_NRPN(num) as appropriate, and values can be combined using
     * bitwise OR: e.g. DSSI_CC(23) | DSSI_NRPN(1069) means the port
     * should respond to CC #23 and NRPN #1069.
     *
     * The host is responsible for doing proper scaling from MIDI
     * controller and NRPN value ranges to port ranges according to
     * the plugin's LADSPA port hints.  Hosts should not deliver
     * through run_synth any MIDI controller events that have already
     * been mapped to control port values.
     *
     * A plugin should not attempt to request mappings from
     * controllers 0 or 32 (MIDI Bank Select MSB and LSB).
     */

  int dssi_get_midi_controller_for_port(unsigned long Port) {
    return 0;
  }

  //----------

    /**
     * run_synth()
     *
     * This member is a function pointer that runs a synth for a
     * block.  This is identical in function to the LADSPA run()
     * function, except that it also supplies events to the synth.
     *
     * A plugin may provide this function, run_multiple_synths() (see
     * below), both, or neither (if it is not in fact a synth).  A
     * plugin that does not provide this function must set this member
     * to NULL.  Authors of synth plugins are encouraged to provide
     * this function if at all possible.
     *
     * The Events pointer points to a block of EventCount ALSA
     * sequencer events, which is used to communicate MIDI and related
     * events to the synth.  Each event is timestamped relative to the
     * start of the block, (mis)using the ALSA "tick time" field as a
     * frame count. The host is responsible for ensuring that events
     * with differing timestamps are already ordered by time.
     *
     * See also the notes on activation, port connection etc in
     * ladpsa.h, in the context of the LADSPA run() function.
     *
     * Note Events
     * ~~~~~~~~~~~
     * There are two minor requirements aimed at making the plugin
     * writer's life as simple as possible:
     *
     * 1. A host must never send events of type SND_SEQ_EVENT_NOTE.
     * Notes should always be sent as separate SND_SEQ_EVENT_NOTE_ON
     * and NOTE_OFF events.  A plugin should discard any one-point
     * NOTE events it sees.
     *
     * 2. A host must not attempt to switch notes off by sending
     * zero-velocity NOTE_ON events.  It should always send true
     * NOTE_OFFs.  It is the host's responsibility to remap events in
     * cases where an external MIDI source has sent it zero-velocity
     * NOTE_ONs.
     *
     * Bank and Program Events
     * ~~~~~~~~~~~~~~~~~~~~~~~
     * Hosts must map MIDI Bank Select MSB and LSB (0 and 32)
     * controllers and MIDI Program Change events onto the banks and
     * programs specified by the plugin, using the DSSI select_program
     * call.  No host should ever deliver a program change or bank
     * select controller to a plugin via run_synth.
     */

  void dssi_run_synth(unsigned long SampleCount, snd_seq_event_t* Events, unsigned long EventCount) {
    updateParametersInProcess();
    updateMidiInputInProcess(Events,EventCount);
    dssi_run(SampleCount);
  }

  //----------

    /**
     * run_synth_adding()
     *
     * This member is a function pointer that runs an instance of a
     * synth for a block, adding its outputs to the values already
     * present at the output ports.  This is provided for symmetry
     * with LADSPA run_adding(), and is equally optional.  A plugin
     * that does not provide it must set this member to NULL.
     */

  //void dssi_run_synth_adding(unsigned long SampleCount, snd_seq_event_t *Events, unsigned long EventCount) {
  //  //DSSI_Trace("dssi: run_synth_adding SampleCount:%i EventCount:%i\n",SampleCount,EventCount);
  //}

    /**
     * run_multiple_synths()
     *
     * This member is a function pointer that runs multiple synth
     * instances for a block.  This is very similar to run_synth(),
     * except that Instances, Events, and EventCounts each point to
     * arrays that hold the LADSPA handles, event buffers, and
     * event counts for each of InstanceCount instances.  That is,
     * Instances points to an array of InstanceCount pointers to
     * DSSI plugin instantiations, Events points to an array of
     * pointers to each instantiation's respective event list, and
     * EventCounts points to an array containing each instantiation's
     * respective event count.
     *
     * A host using this function must guarantee that ALL active
     * instances of the plugin are represented in each call to the
     * function -- that is, a host may not call run_multiple_synths()
     * for some instances of a given plugin and then call run_synth()
     * as well for others.  'All .. instances of the plugin' means
     * every instance sharing the same LADSPA label and shared object
     * (*.so) file (rather than every instance sharing the same *.so).
     * 'Active' means any instance for which activate() has been called
     * but deactivate() has not.
     *
     * A plugin may provide this function, run_synths() (see above),
     * both, or neither (if it not in fact a synth).  A plugin that
     * does not provide this function must set this member to NULL.
     * Plugin authors implementing run_multiple_synths are strongly
     * encouraged to implement run_synth as well if at all possible,
     * to aid simplistic hosts, even where it would be less efficient
     * to use it.
     */

  //void dssi_run_multiple_synths(unsigned long InstanceCount, LADSPA_Handle *Instances, unsigned long SampleCount, snd_seq_event_t **Events, unsigned long *EventCounts) {
  //}

    /**
     * run_multiple_synths_adding()
     *
     * This member is a function pointer that runs multiple synth
     * instances for a block, adding each synth's outputs to the
     * values already present at the output ports.  This is provided
     * for symmetry with both the DSSI run_multiple_synths() and LADSPA
     * run_adding() functions, and is equally optional.  A plugin
     * that does not provide it must set this member to NULL.
     */

  //void dssi_run_multiple_synths_adding(unsigned long InstanceCount, LADSPA_Handle *Instances, unsigned long SampleCount, snd_seq_event_t **Events, unsigned long *EventCounts) {
  //}

};

//----------------------------------------------------------------------
#endif
