#ifndef sat_clap_incuded
#define sat_clap_incuded
//----------------------------------------------------------------------

#include "extern/clap/clap.h"

// not included by clap.h
#include "extern/clap/ext/draft/check-for-update.h"
#include "extern/clap/ext/draft/transport-control.h"

//----------------------------------------------------------------------

#define NUM_CLAP_PLUGIN_EXTENSIONS 29

const char* SAT_ClapPluginExtensionIds[NUM_CLAP_PLUGIN_EXTENSIONS] = {
    CLAP_EXT_AMBISONIC,
    CLAP_EXT_AUDIO_PORTS_ACTIVATION,
    CLAP_EXT_AUDIO_PORTS_CONFIG,
    CLAP_EXT_AUDIO_PORTS,
    CLAP_EXT_CHECK_FOR_UPDATE,
    CLAP_EXT_CONTEXT_MENU,
    CLAP_EXT_CV,
    CLAP_EXT_GUI,
    CLAP_EXT_LATENCY,
    CLAP_EXT_MIDI_MAPPINGS,
    CLAP_EXT_NOTE_NAME,
    CLAP_EXT_NOTE_PORTS,
    CLAP_EXT_PARAM_INDICATION,
    CLAP_EXT_PARAMS,
    CLAP_EXT_POSIX_FD_SUPPORT,
    CLAP_EXT_PRESET_LOAD,
    CLAP_EXT_REMOTE_CONTROLS,
    CLAP_EXT_RENDER,
    CLAP_EXT_RESOURCE_DIRECTORY,
    CLAP_EXT_STATE,
    CLAP_EXT_STATE_CONTEXT,
    CLAP_EXT_SURROUND,
    CLAP_EXT_TAIL,
    CLAP_EXT_THREAD_POOL,
    CLAP_EXT_TIMER_SUPPORT,
    CLAP_EXT_TRACK_INFO,
    CLAP_EXT_TRIGGERS,
    CLAP_EXT_TUNING,
    CLAP_EXT_VOICE_INFO
};

struct SAT_ClapPluginExtensions {
  const clap_plugin_ambisonic_t*              ambisonic               = nullptr; // draft
  const clap_plugin_audio_ports_activation_t* audio_ports_activation  = nullptr; // draft
  const clap_plugin_audio_ports_config_t*     audio_ports_config      = nullptr;
  const clap_plugin_audio_ports_t*            audio_ports             = nullptr;
  const clap_plugin_check_for_update_t*       check_for_update        = nullptr; // draft
  const clap_plugin_context_menu_t*           context_menu            = nullptr; // draft
  const clap_plugin_cv_t*                     cv                      = nullptr; // draft
  const clap_plugin_gui_t*                    gui                     = nullptr;
  const clap_plugin_latency_t*                latency                 = nullptr;
  const clap_plugin_midi_mappings_t*          midi_mappings           = nullptr; // draft
  const clap_plugin_note_name_t*              note_name               = nullptr;
  const clap_plugin_note_ports_t*             note_ports              = nullptr;
  const clap_plugin_param_indication_t*       param_indication        = nullptr; // draft
  const clap_plugin_params_t*                 params                  = nullptr;
  const clap_plugin_posix_fd_support_t*       posix_fd_support        = nullptr;
  const clap_plugin_preset_load_t*            preset_load             = nullptr; // draft
  const clap_plugin_remote_controls_t*        remote_controls         = nullptr; // draft
  const clap_plugin_render_t*                 render                  = nullptr;
  const clap_plugin_resource_directory_t*     resource_directory      = nullptr; // draft
  const clap_plugin_state_t*                  state                   = nullptr;
  const clap_plugin_state_context_t*          state_context           = nullptr; // draft
  const clap_plugin_surround_t*               surround                = nullptr; // draft
  const clap_plugin_tail_t*                   tail                    = nullptr;
  const clap_plugin_thread_pool_t*            thread_pool             = nullptr;
  const clap_plugin_timer_support_t*          timer_support           = nullptr;
  const clap_plugin_track_info_t*             track_info              = nullptr; // draft
  const clap_plugin_voice_info_t*             voice_info              = nullptr;
  const clap_plugin_triggers_t*               triggers                = nullptr; // draft
  const clap_plugin_tuning_t*                 tuning                  = nullptr; // draft
};

//----------

struct SAT_ClapHostExtensions {
  const clap_host_ambisonic_t*                ambisonic               = nullptr; // draft
  const clap_host_audio_ports_config_t*       audio_ports_config      = nullptr;
  const clap_host_audio_ports_t*              audio_ports             = nullptr;
  const clap_host_check_for_update_t*         check_for_update        = nullptr; // draft
  const clap_host_context_menu_t*             context_menu            = nullptr; // draft
  const clap_host_cv_t*                       cv                      = nullptr; // draft
  const clap_host_event_registry_t*           event_registry          = nullptr;
  const clap_host_gui_t*                      gui                     = nullptr;
  const clap_host_latency_t*                  latency                 = nullptr;
  const clap_host_log_t*                      log                     = nullptr;
  const clap_host_midi_mappings_t*            midi_mappings           = nullptr; // draft
  const clap_host_note_name_t*                note_name               = nullptr;
  const clap_host_note_ports_t*               note_ports              = nullptr;
  const clap_host_params_t*                   params                  = nullptr;
  const clap_host_posix_fd_support_t*         posix_fd_support        = nullptr;
  const clap_host_preset_load_t*              preset_load             = nullptr; // draft
  const clap_host_remote_controls_t*          remote_controls         = nullptr; // draft
  const clap_host_resource_directory_t*       resource_directory      = nullptr; // draft
  const clap_host_state_t*                    state                   = nullptr;
  const clap_host_surround_t*                 surround                = nullptr; // draft
  const clap_host_tail_t*                     tail                    = nullptr;
  const clap_host_thread_check_t*             thread_check            = nullptr;
  const clap_host_thread_pool_t*              thread_pool             = nullptr;
  const clap_host_timer_support_t*            timer_support           = nullptr;
  const clap_host_track_info_t*               track_info              = nullptr; // draft
  const clap_host_transport_control_t*        transport_control       = nullptr; // draft
  const clap_host_triggers_t*                 triggers                = nullptr; // draft
  const clap_host_tuning_t*                   tuning                  = nullptr; // draft
};

//----------------------------------------------------------------------
#endif
