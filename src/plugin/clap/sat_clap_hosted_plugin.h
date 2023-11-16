#if 0



#ifndef sat_clap_hosted_plugin_included
#define sat_clap_hosted_plugin_included
//----------------------------------------------------------------------

// plugin from a host perspective
// (called from host)

#include "plugin/clap/sat_clap.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_ClapHostedPlugin {

//------------------------------
protected:
//------------------------------

  clap_plugin_t*          MPlugin   = nullptr;
  clap_host_t*            MHost     = nullptr;

//------------------------------
public:
//------------------------------

  sat_plugin_ext_t        ext       = {};
  sat_plugin_ext_draft_t  ext_draft = {};

//------------------------------
public:
//------------------------------

  SAT_ClapHostedPlugin(clap_host_t* AHost, clap_plugin_t* APlugin) {

    //SAT_Assert(AHost);

    MHost = AHost;
    MPlugin = APlugin;

    //  ext.audio_ports_config        = (clap_plugin_audio_ports_config_t*)MHost->get_extension(MHost,CLAP_EXT_AUDIO_PORTS_CONFIG);
    //  ext.audio_ports               = (clap_host_audio_ports_t*)MHost->get_extension(MHost,CLAP_EXT_AUDIO_PORTS);
    //  ext.event_registry            = (clap_host_event_registry_t*)MHost->get_extension(MHost,CLAP_EXT_EVENT_REGISTRY);
    //  ext.gui                       = (clap_host_gui_t*)MHost->get_extension(MHost,CLAP_EXT_GUI);
    //  ext.latency                   = (clap_host_latency_t*)MHost->get_extension(MHost,CLAP_EXT_LATENCY);
    //  ext.log                       = (clap_host_log_t*)MHost->get_extension(MHost,CLAP_EXT_LOG);
    //  ext.note_name                 = (clap_host_note_name_t*)MHost->get_extension(MHost,CLAP_EXT_NOTE_NAME);
    //  ext.note_ports                = (clap_host_note_ports_t*)MHost->get_extension(MHost,CLAP_EXT_NOTE_PORTS);
    //  ext.params                    = (clap_host_params_t*)MHost->get_extension(MHost,CLAP_EXT_PARAMS);
    //  ext.posix_fd_support          = (clap_host_posix_fd_support_t*)MHost->get_extension(MHost,CLAP_EXT_POSIX_FD_SUPPORT);
    //  ext.state                     = (clap_host_state_t*)MHost->get_extension(MHost,CLAP_EXT_STATE);
    //  ext.tail                      = (clap_host_tail_t*)MHost->get_extension(MHost,CLAP_EXT_TAIL);
    //  ext.thread_check              = (clap_host_thread_check_t*)MHost->get_extension(MHost,CLAP_EXT_THREAD_CHECK);
    //  ext.thread_pool               = (clap_host_thread_pool_t*)MHost->get_extension(MHost,CLAP_EXT_THREAD_POOL);
    //  ext.timer_support             = (clap_host_timer_support_t*)MHost->get_extension(MHost,CLAP_EXT_TIMER_SUPPORT);
    //
    //  ext_draft.ambisonic           = (clap_host_ambisonic_t*)MHost->get_extension(MHost,CLAP_EXT_AMBISONIC);
    //  ext_draft.check_for_update    = (clap_host_check_for_update_t*)MHost->get_extension(MHost,CLAP_EXT_CHECK_FOR_UPDATE);
    //  ext_draft.context_menu        = (clap_host_context_menu_t*)MHost->get_extension(MHost,CLAP_EXT_CONTEXT_MENU);
    //  ext_draft.cv                  = (clap_host_cv_t*)MHost->get_extension(MHost,CLAP_EXT_CV);
    //  ext_draft.midi_mappings       = (clap_host_midi_mappings_t*)MHost->get_extension(MHost,CLAP_EXT_MIDI_MAPPINGS);
    //  ext_draft.preset_load         = (clap_host_preset_load_t*)MHost->get_extension(MHost,CLAP_EXT_PRESET_LOAD);
    //  ext_draft.remote_controls     = (clap_host_remote_controls_t*)MHost->get_extension(MHost,CLAP_EXT_REMOTE_CONTROLS);
    //  ext_draft.resource_directory  = (clap_host_resource_directory_t*)MHost->get_extension(MHost,CLAP_EXT_RESOURCE_DIRECTORY);
    //  ext_draft.surround            = (clap_host_surround_t*)MHost->get_extension(MHost,CLAP_EXT_SURROUND);
    //  ext_draft.track_info          = (clap_host_track_info_t*)MHost->get_extension(MHost,CLAP_EXT_TRACK_INFO);
    //  ext_draft.transport_control   = (clap_host_transport_control_t*)MHost->get_extension(MHost,CLAP_EXT_TRANSPORT_CONTROL);
    //  ext_draft.triggers            = (clap_host_triggers_t*)MHost->get_extension(MHost,CLAP_EXT_TRIGGERS);
    //  ext_draft.tuning              = (clap_host_tuning_t*)MHost->get_extension(MHost,CLAP_EXT_TUNING);

  };

  //----------

  virtual ~SAT_ClapHostedPlugin() {
  }

};

//----------------------------------------------------------------------
#endif




#endif