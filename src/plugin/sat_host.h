#ifndef sat_host_included
#define sat_host_included
//----------------------------------------------------------------------

// aka SAT_HostReference

#include "plugin/clap/sat_clap_host.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Host
: public SAT_ClapHost {

//------------------------------
public:
//------------------------------

  SAT_Host(const clap_host_t* AHost)
  : SAT_ClapHost(AHost) {
  }

  //----------

  virtual ~SAT_Host() {
  }

//------------------------------
public:
//------------------------------

  void logHostExtensions() {
    if (ext.ambisonic)           SAT_Log("  host supports draft ext %s\n", CLAP_EXT_AMBISONIC);
    if (ext.audio_ports_config)  SAT_Log("  host supports ext %s\n",       CLAP_EXT_AUDIO_PORTS_CONFIG);
    if (ext.audio_ports)         SAT_Log("  host supports ext %s\n",       CLAP_EXT_AUDIO_PORTS);
    if (ext.check_for_update)    SAT_Log("  host supports draft ext %s\n", CLAP_EXT_CHECK_FOR_UPDATE);
    if (ext.context_menu)        SAT_Log("  host supports draft ext %s\n", CLAP_EXT_CONTEXT_MENU);
    if (ext.cv)                  SAT_Log("  host supports draft ext %s\n", CLAP_EXT_CV);
    if (ext.event_registry)      SAT_Log("  host supports ext %s\n",       CLAP_EXT_EVENT_REGISTRY);
    if (ext.gui)                 SAT_Log("  host supports ext %s\n",       CLAP_EXT_GUI);
    if (ext.latency)             SAT_Log("  host supports ext %s\n",       CLAP_EXT_LATENCY);
    if (ext.log)                 SAT_Log("  host supports ext %s\n",       CLAP_EXT_LOG);
    if (ext.midi_mappings)       SAT_Log("  host supports draft ext %s\n", CLAP_EXT_MIDI_MAPPINGS);
    if (ext.note_name)           SAT_Log("  host supports ext %s\n",       CLAP_EXT_NOTE_NAME);
    if (ext.note_ports)          SAT_Log("  host supports ext %s\n",       CLAP_EXT_NOTE_PORTS);
    if (ext.params)              SAT_Log("  host supports ext %s\n",       CLAP_EXT_PARAMS);
    if (ext.posix_fd_support)    SAT_Log("  host supports ext %s\n",       CLAP_EXT_POSIX_FD_SUPPORT);
    if (ext.preset_load)         SAT_Log("  host supports draft ext %s\n", CLAP_EXT_PRESET_LOAD);
    if (ext.remote_controls)     SAT_Log("  host supports draft ext %s\n", CLAP_EXT_REMOTE_CONTROLS);
    if (ext.resource_directory)  SAT_Log("  host supports draft ext %s\n", CLAP_EXT_RESOURCE_DIRECTORY);
    if (ext.state)               SAT_Log("  host supports ext %s\n",       CLAP_EXT_STATE);
    if (ext.surround)            SAT_Log("  host supports draft ext %s\n", CLAP_EXT_SURROUND);
    if (ext.tail)                SAT_Log("  host supports ext %s\n",       CLAP_EXT_TAIL);
    if (ext.thread_check)        SAT_Log("  host supports ext %s\n",       CLAP_EXT_THREAD_CHECK);
    if (ext.thread_pool)         SAT_Log("  host supports ext %s\n",       CLAP_EXT_THREAD_POOL);
    if (ext.timer_support)       SAT_Log("  host supports ext %s\n",       CLAP_EXT_TIMER_SUPPORT);
    if (ext.track_info)          SAT_Log("  host supports draft ext %s\n", CLAP_EXT_TRACK_INFO);
    if (ext.transport_control)   SAT_Log("  host supports draft ext %s\n", CLAP_EXT_TRANSPORT_CONTROL);
    if (ext.triggers)            SAT_Log("  host supports draft ext %s\n", CLAP_EXT_TRIGGERS);
    if (ext.tuning)              SAT_Log("  host supports draft ext %s\n", CLAP_EXT_TUNING);
  }


};

//----------------------------------------------------------------------
#endif
