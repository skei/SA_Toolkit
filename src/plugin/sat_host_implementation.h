#ifndef sat_host_implementation_included
#define sat_host_implementation_included
//----------------------------------------------------------------------

// aka SAT_HostImplementation

#include "plugin/clap/sat_clap_host_implementation.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_HostImplementation
: public SAT_ClapHostImplementation {

//------------------------------
public:
//------------------------------

  SAT_HostImplementation()
  : SAT_ClapHostImplementation() {
  }

  //----------

  virtual ~SAT_HostImplementation() {
  }

//------------------------------
public:
//------------------------------

  // Query an extension.
  // The returned pointer is owned by the host.
  // It is forbidden to call it before plugin->init().
  // You can call it within plugin->init() call, and after.
  // [thread-safe]

  //TODO: see SAT_Plugin (dictionary)

  const void* get_extension(const char *extension_id) override {
    //SAT_Print("id %s\n",extension_id);
    if (strcmp(extension_id,CLAP_EXT_AMBISONIC)           == 0) return &MAmbisonicExt;
    if (strcmp(extension_id,CLAP_EXT_AUDIO_PORTS)         == 0) return &MAudioPortsExt;
    if (strcmp(extension_id,CLAP_EXT_AUDIO_PORTS_CONFIG)  == 0) return &MAudioPortsExt;
    if (strcmp(extension_id,CLAP_EXT_CHECK_FOR_UPDATE)    == 0) return &MCheckForUpdateExt;
    if (strcmp(extension_id,CLAP_EXT_CV)                  == 0) return &MCVExt;
    if (strcmp(extension_id,CLAP_EXT_EVENT_REGISTRY)      == 0) return &MEventRegistryExt;
    if (strcmp(extension_id,CLAP_EXT_GUI)                 == 0) return &MGuiExt;
    if (strcmp(extension_id,CLAP_EXT_LATENCY)             == 0) return &MLatencyExt;
    if (strcmp(extension_id,CLAP_EXT_LOG)                 == 0) return &MLogExt;
    if (strcmp(extension_id,CLAP_EXT_MIDI_MAPPINGS)       == 0) return &MMidiMappingsExt;
    if (strcmp(extension_id,CLAP_EXT_NOTE_NAME)           == 0) return &MNoteNameExt;
    if (strcmp(extension_id,CLAP_EXT_NOTE_PORTS)          == 0) return &MNotePortsExt;
    if (strcmp(extension_id,CLAP_EXT_PARAMS)              == 0) return &MParamsExt;
    if (strcmp(extension_id,CLAP_EXT_POSIX_FD_SUPPORT)    == 0) return &MPosixFdSupportExt;
    if (strcmp(extension_id,CLAP_EXT_PRESET_LOAD)         == 0) return &MPresetLoadExt;
    if (strcmp(extension_id,CLAP_EXT_REMOTE_CONTROLS)     == 0) return &MRemoteControlsExt;
    if (strcmp(extension_id,CLAP_EXT_RESOURCE_DIRECTORY)  == 0) return &MResourceDirectoryExt;
    if (strcmp(extension_id,CLAP_EXT_STATE)               == 0) return &MStateExt;
    if (strcmp(extension_id,CLAP_EXT_SURROUND)            == 0) return &MSurroundExt;
    if (strcmp(extension_id,CLAP_EXT_TAIL)                == 0) return &MTailExt;
    if (strcmp(extension_id,CLAP_EXT_THREAD_CHECK)        == 0) return &MThreadCheckExt;
    if (strcmp(extension_id,CLAP_EXT_THREAD_POOL)         == 0) return &MThreadPoolExt;
    if (strcmp(extension_id,CLAP_EXT_TIMER_SUPPORT)       == 0) return &MTimerSupportExt;
    if (strcmp(extension_id,CLAP_EXT_TRACK_INFO)          == 0) return &MTrackInfoExt;
    if (strcmp(extension_id,CLAP_EXT_TRANSPORT_CONTROL)   == 0) return &MTransportControlExt;
    if (strcmp(extension_id,CLAP_EXT_TRIGGERS)            == 0) return &MTriggersExt;
    if (strcmp(extension_id,CLAP_EXT_TUNING)              == 0) return &MTuningExt;
    if (strcmp(extension_id,CLAP_EXT_VOICE_INFO)          == 0) return &MVoiceInfoExt;
    return nullptr;
  }

  //----------

  // Request the host to deactivate and then reactivate the plugin.
  // The operation may be delayed by the host.
  // [thread-safe]

  void request_restart() override {
  }

  //----------

  // Request the host to activate and start processing the plugin.
  // This is useful if you have external IO and need to wake up the plugin from "sleep".
  // [thread-safe]

  void request_process() override {
  }

  //----------

  // Request the host to schedule a call to plugin->on_main_thread(plugin) on the main thread.
  // [thread-safe]

  void request_callback() override {
  }

//------------------------------
public: // ambisonic (draft)
//------------------------------

  // Informs the host that the info has changed.
  // The info can only change when the plugin is de-activated.
  // [main-thread]

  void ambisonic_changed() override {
  }

//------------------------------
public: // audio ports
//------------------------------

  // Checks if the host allows a plugin to change a given aspect of the audio ports definition.
  // [main-thread]

  bool audio_ports_is_rescan_flag_supported(uint32_t flag) override {
    return false;
  }

  //----------

  // Rescan the full list of audio ports according to the flags.
  // It is illegal to ask the host to rescan with a flag that is not supported.
  // Certain flags require the plugin to be de-activated.
  // [main-thread]

  void audio_ports_rescan(uint32_t flags) override {
  }

//------------------------------
public: // audio ports config
//------------------------------

  // Rescan the full list of configs.
  // [main-thread]

  void audio_ports_config_rescan() override {
  }

//------------------------------
public: // check for update (draft)
//------------------------------

  // [main-thread]

  void check_for_update_on_new_version(const clap_check_for_update_info_t *update_info) override {
  }

//------------------------------
public: // context menu (draft)
//------------------------------

  // Insert host's menu items into the menu builder.
  // If target is null, assume global context.
  // [main-thread]


  bool context_menu_populate(const clap_context_menu_target_t *target, const clap_context_menu_builder_t *builder) override {
    return false;
  }

  //----------

  // Performs the given action, which was previously provided to the plugin via populate().
  // If target is null, assume global context.
  // [main-thread]

  bool context_menu_perform(const clap_context_menu_target_t *target, clap_id action_id) override {
    return false;
  }

  //----------

  // Returns true if the host can display a popup menu for the plugin.
  // This may depends upon the current windowing system used to display the plugin, so the
  // return value is invalidated after creating the plugin window.
  // [main-thread]

  bool context_menu_can_popup() override {
    return false;
  }

  //----------

  // Shows the host popup menu for a given parameter.
  // If the plugin is using embedded GUI, then x and y are relative to the plugin's window,
  // otherwise they're absolute coordinate, and screen index might be set accordingly.
  // If target is null, assume global context.
  // [main-thread]

  bool context_menu_popup(const clap_context_menu_target_t *target, int32_t screen_index, int32_t x, int32_t y) override {
    return false;
  }

//------------------------------
public: // cv (draft)
//------------------------------

  // Informs the host that the channels type have changed.
  // The channels type can only change when the plugin is de-activated.
  // [main-thread,!active]

  void cv_changed() override {
  }

//------------------------------
public: // event registry
//------------------------------

  // Queries an event space id.
  // The space id 0 is reserved for CLAP's core events. See CLAP_CORE_EVENT_SPACE.
  // Return false and sets *space to UINT16_MAX if the space name is unknown to the host.
  // [main-thread]

  bool event_registry_query(const char *space_name, uint16_t *space_id) override {
    return false;
  }

//------------------------------
public: //
//------------------------------

  // The host should call get_resize_hints() again.
  // [thread-safe]

  void gui_resize_hints_changed() override {
  }

  //----------

  // Request the host to resize the client area to width, height.
  // Return true if the new size is accepted, false otherwise.
  // The host doesn't have to call set_size().
  // Note: if not called from the main thread, then a return value simply means that the host
  // acknowledged the request and will process it asynchronously. If the request then can't be
  // satisfied then the host will call set_size() to revert the operation.
  // [thread-safe]

  bool gui_request_resize(uint32_t width, uint32_t height) override {
    return false;
  }

  //----------

  //Request the host to show the plugin gui.
  // Return true on success, false otherwise.
  // [thread-safe]

  bool gui_request_show() override {
    return false;
  }

  //----------

  // Request the host to hide the plugin gui.
  // Return true on success, false otherwise.
  // [thread-safe]

  bool gui_request_hide() override {
    return false;
  }

  //----------

  // The floating window has been closed, or the connection to the gui has been lost.
  // If was_destroyed is true, then the host must call clap_plugin_gui->destroy() to acknowledge
  // the gui destruction.
  // [thread-safe]

  void gui_closed(bool was_destroyed) override {
  }

//------------------------------
public: // latency
//------------------------------

  // Tell the host that the latency changed.
  // The latency is only allowed to change if the plugin is deactivated.
  // If the plugin is activated, call host->request_restart()
  // [main-thread]

  void latency_changed() override {
  }

//------------------------------
public: // log
//------------------------------

  // Log a message through the host.
  // [thread-safe]

  //CLAP_LOG_DEBUG,
  //CLAP_LOG_INFO,
  //CLAP_LOG_WARNING,
  //CLAP_LOG_ERROR,
  //CLAP_LOG_FATAL,
  //CLAP_LOG_HOST_MISBEHAVING,
  //CLAP_LOG_PLUGIN_MISBEHAVING,

  void log_log(clap_log_severity severity, const char *msg) override {
  }

//------------------------------
public: // midi mappings (draft)
//------------------------------

  // [main-thread]

  void midi_mappings_changed() override {
  }

  //----------

  // Informs the host that the note names have changed.
  // [main-thread]

  void note_name_changed() override {
  }

//------------------------------
public: // note ports
//------------------------------

  // Query which dialects the host supports
  // [main-thread]

  uint32_t  note_ports_supported_dialects() override {
    return 0;
  }

  //----------

  // Rescan the full list of note ports according to the flags.
  // [main-thread]

  void note_ports_rescan(uint32_t flags) override {
  }

//------------------------------
public: // params
//------------------------------

  // Rescan the full list of parameters according to the flags.
  // [main-thread]

  void params_rescan(clap_param_rescan_flags flags) override {
  }

  //----------

  // Clears references to a parameter.
  // [main-thread]

  void params_clear(clap_id param_id, clap_param_clear_flags flags) override {
  }

  //----------

  // Request a parameter flush.
  // The host will then schedule a call to either:
  // - clap_plugin.process()
  // - clap_plugin_params.flush()
  // This function is always safe to use and should not be called from an [audio-thread] as the
  // plugin would already be within process() or flush().
  // [thread-safe,!audio-thread]

  void params_request_flush() override {
  }

//------------------------------
public: // posix fd support
//------------------------------

  // [main-thread]

  // CLAP_POSIX_FD_READ
  // CLAP_POSIX_FD_WRITE
  // CLAP_POSIX_FD_ERROR

  bool posix_fd_support_register_fd(int fd, clap_posix_fd_flags_t flags) override {
    return false;
  }

  //----------

  // [main-thread]

  bool posix_fd_support_modify_fd(int fd, clap_posix_fd_flags_t flags) override {
    return false;
  }

  //----------

  // [main-thread]

  bool posix_fd_support_unregister_fd(int fd) override {
    return false;
  }

//------------------------------
public: // preset load (draft)
//------------------------------

  // Called if clap_plugin_preset_load.load() failed.
  // os_error: the operating system error, if applicable. If not applicable set it to a non-error
  // value, eg: 0 on unix and Windows.
  // [main-thread]

  void preset_load_on_error(uint32_t location_kind, const char *location, const char *load_key,int32_t os_error, const char *msg) override {
  }

  //----------

  // Informs the host that the following preset has been loaded.
  // This contributes to keep in sync the host preset browser and plugin preset browser.
  // If the preset was loaded from a container file, then the load_key must be set, otherwise it
  // must be null.
  // [main-thread]

  void preset_load_loaded(uint32_t location_kind, const char *location, const char *load_key) override {
  }

//------------------------------
public: // remote controls (draft)
//------------------------------

  // Informs the host that the remote controls have changed.
  // [main-thread]

  void remote_controls_changed() override {
  }

  //----------

  // Suggest a page to the host because it correspond to what the user is currently editing in the
  // plugin's GUI.
  // [main-thread]

  void remote_controls_suggest_page(clap_id page_id) override {
  }

//------------------------------
public: // resource directory (draft)
//------------------------------

  // Request the host to setup a resource directory with the specified sharing.
  // Returns true if the host will perform the request.
  // [main-thread]

  bool resource_directory_request_directory(bool is_shared) override {
    return false;
  }

  //----------

  // Tell the host that the resource directory of the specified sharing is no longer required.
  // If is_shared = false, then the host may delete the directory content.
  // [main-thread]

  void resource_directory_release_directory(bool is_shared) override {
  }

//------------------------------
public: // state
//------------------------------

  // Tell the host that the plugin state has changed and should be saved again.
  // If a parameter value changes, then it is implicit that the state is dirty.
  // [main-thread]

  void state_mark_dirty() override {
  }

//------------------------------
public: // surround (draft)
//------------------------------

  // Informs the host that the channel map has changed.
  // The channel map can only change when the plugin is de-activated.
  // [main-thread]

  void surround_changed() override {
  }

  //----------

  // Ask the host what is the prefered/project surround channel map.
  // [main-thread]

  void surround_get_preferred_channel_map(uint8_t *channel_map, uint32_t channel_map_capacity, uint32_t *channel_count) override {
  }

//------------------------------
public: // tail
//------------------------------

  // Tell the host that the tail has changed.
  // [audio-thread]

  void tail_changed() override {
  }

//------------------------------
public: //
//------------------------------

  // Returns true if "this" thread is the main thread.
  // [thread-safe]

  bool thread_check_is_main_thread() override {
    return false;
  }

  //----------

  // Returns true if "this" thread is one of the audio threads.
  // [thread-safe]

  bool thread_check_is_audio_thread() override {
    return false;
  }

//------------------------------
public: // thread pool
//------------------------------

  // Schedule num_tasks jobs in the host thread pool.
  // It can't be called concurrently or from the thread pool.
  // Will block until all the tasks are processed.
  // This must be used exclusively for realtime processing within the process call.
  // Returns true if the host did execute all the tasks, false if it rejected the request.
  // The host should check that the plugin is within the process call, and if not, reject the exec
  // request.
  // [audio-thread]

  bool thread_pool_request_exec(uint32_t num_tasks) override {
    return false;
  }

//------------------------------
public: // timer support
//------------------------------

  // Registers a periodic timer.
  // The host may adjust the period if it is under a certain threshold.
  // 30 Hz should be allowed.
  // [main-thread]

  bool timer_support_register_timer(uint32_t period_ms, clap_id *timer_id) override {
    return false;
  }

  //----------

  // [main-thread]

  bool timer_support_unregister_timer(clap_id timer_id) override {
    return false;
  }

//------------------------------
public: // track info (draft)
//------------------------------

  // Get info about the track the plugin belongs to.
  // [main-thread]

  bool track_info_get(clap_track_info_t *info) override {
    return false;
  }

//------------------------------
public: // transport control (draft)
//------------------------------

  // Jumps back to the start point and starts the transport
  // [main-thread]

  void transport_control_request_start() override {
  }

  //----------

  // Stops the transport, and jumps to the start point
  // [main-thread]

  void transport_control_request_stop() override {
  }

  //----------

  // If not playing, starts the transport from its current position
  // [main-thread]

  void transport_control_request_continue() override {
  }

  //----------

  // If playing, stops the transport at the current position
  // [main-thread]

  void transport_control_request_pause() override {
  }

  //----------

  // Equivalent to what "space bar" does with most DAWs
  // [main-thread]

  void transport_control_request_toggle_play() override {
  }

  //----------

  // Jumps the transport to the given position.
  // Does not start the transport.
  // [main-thread]

  void transport_control_request_jump(clap_beattime position) override {
  }

  //----------

  // Sets the loop region
  // [main-thread]

  void transport_control_request_loop_region(clap_beattime start, clap_beattime duration) override {
  }

  //----------

  // Toggles looping
  // [main-thread]

  void transport_control_request_toggle_loop() override {
  }

  //----------

  // Enables/Disables looping
  // [main-thread]

  void transport_control_request_enable_loop(bool is_enabled) override {
  }

  //----------

  // Enables/Disables recording
  // [main-thread]

  void transport_control_request_record(bool is_recording) override {
  }

  //----------

  // Toggles recording
  // [main-thread]

  void transport_control_request_toggle_record() override {
  }

//------------------------------
public: // triggers (draft)
//------------------------------

  // Rescan the full list of triggers according to the flags.
  // [main-thread]

  void triggers_rescan(clap_trigger_rescan_flags flags) override {
  }

  //----------

  // Clears references to a trigger.
  // [main-thread]

  void triggers_clear(clap_id trigger_id, clap_trigger_clear_flags flags) override {
  }

//------------------------------
public: // tuning (draft)
//------------------------------

  // Gets the relative tuning in semitones against equal temperament with A4=440Hz.
  // The plugin may query the tuning at a rate that makes sense for *low* frequency modulations.
  // If the tuning_id is not found or equals to CLAP_INVALID_ID,
  // then the function shall gracefuly return a sensible value.
  // sample_offset is the sample offset from the begining of the current process block.
  // should_play(...) should be checked before calling this function.
  // [audio-thread & in-process]

  double tuning_get_relative(clap_id tuning_id, int32_t channel, int32_t key, uint32_t sample_offset) override {
    return 0.0;
  }

  //----------

  // Returns true if the note should be played.
  // [audio-thread & in-process]

  bool tuning_should_play(clap_id tuning_id, int32_t channel, int32_t key) override {
    return false;
  }

  //----------

  // Returns the number of tunings in the pool.
  // [main-thread]

  uint32_t tuning_get_tuning_count() override {
    return 0;
  }

  //----------

  // Gets info about a tuning
  // [main-thread]

  bool tuning_get_info(uint32_t tuning_index, clap_tuning_info_t *info) override {
    return false;
  }

//------------------------------
public: // voice info
//------------------------------

  // informs the host that the voice info has changed
  // [main-thread]

  void voice_info_changed() override {
  }

  //----------


};

//----------------------------------------------------------------------
#endif
