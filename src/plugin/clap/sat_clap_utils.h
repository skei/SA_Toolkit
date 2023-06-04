#ifndef sat_clap_utils_included
#define sat_clap_utils_included
//----------------------------------------------------------------------

#include "plugin/clap/sat_clap.h"

//----------------------------------------------------------------------

bool SAT_ClapIsInstrument(const clap_plugin_descriptor_t* descriptor) {
  const char* const* feature = descriptor->features;
  uint32_t index = 0;
  while (feature[index]) {
    if (strcmp(feature[index], CLAP_PLUGIN_FEATURE_INSTRUMENT) == 0) return true;
    index++;
  }
  return false;
}

//----------------------------------------------------------------------

  void SAT_LogClapHostInfo(SAT_ClapHost* AHost) {

    const clap_host_t* host = AHost->getHost();

    SAT_Log("  host name %s\n",AHost->getName());
    SAT_Log("  host vendor %s\n",AHost->getVendor());
    SAT_Log("  host version %s\n",AHost->getVersion());
    SAT_Log("  host url %s\n",AHost->getUrl());

    if (AHost->ext.ambisonic) {
      SAT_Log("  host supports ext '%s' (draft)\n", CLAP_EXT_AMBISONIC);
    }

    if (AHost->ext.audio_ports_config) {
      SAT_Log("  host supports ext '%s'\n", CLAP_EXT_AUDIO_PORTS_CONFIG);
    }

    if (AHost->ext.audio_ports) {
      SAT_Log("  host supports ext '%s'\n", CLAP_EXT_AUDIO_PORTS);
      if (AHost->ext.audio_ports->is_rescan_flag_supported(host,CLAP_AUDIO_PORTS_RESCAN_NAMES))         SAT_Log("    CLAP_AUDIO_PORTS_RESCAN_NAMES\n");
      if (AHost->ext.audio_ports->is_rescan_flag_supported(host,CLAP_AUDIO_PORTS_RESCAN_FLAGS))         SAT_Log("    CLAP_AUDIO_PORTS_RESCAN_FLAGS\n");
      if (AHost->ext.audio_ports->is_rescan_flag_supported(host,CLAP_AUDIO_PORTS_RESCAN_CHANNEL_COUNT)) SAT_Log("    CLAP_AUDIO_PORTS_RESCAN_CHANNEL_COUNT\n");
      if (AHost->ext.audio_ports->is_rescan_flag_supported(host,CLAP_AUDIO_PORTS_RESCAN_PORT_TYPE))     SAT_Log("    CLAP_AUDIO_PORTS_RESCAN_PORT_TYPE\n");
      if (AHost->ext.audio_ports->is_rescan_flag_supported(host,CLAP_AUDIO_PORTS_RESCAN_IN_PLACE_PAIR)) SAT_Log("    CLAP_AUDIO_PORTS_RESCAN_IN_PLACE_PAIR\n");
      if (AHost->ext.audio_ports->is_rescan_flag_supported(host,CLAP_AUDIO_PORTS_RESCAN_LIST))          SAT_Log("    CLAP_AUDIO_PORTS_RESCAN_LIST\n");
    }

    if (AHost->ext.check_for_update) {
      SAT_Log("  host supports ext '%s' (draft)\n", CLAP_EXT_CHECK_FOR_UPDATE);
    }

    if (AHost->ext.context_menu) {
      SAT_Log("  host supports ext '%s' (draft)\n", CLAP_EXT_CONTEXT_MENU);
      if (AHost->ext.context_menu->can_popup(host)) SAT_Log("    can_popup\n");
    }

    if (AHost->ext.cv)                  SAT_Log("  host supports ext '%s' (draft)\n",  CLAP_EXT_CV);
    if (AHost->ext.event_registry)      SAT_Log("  host supports ext '%s'\n",          CLAP_EXT_EVENT_REGISTRY);
    if (AHost->ext.gui)                 SAT_Log("  host supports ext '%s'\n",          CLAP_EXT_GUI);
    if (AHost->ext.latency)             SAT_Log("  host supports ext '%s'\n",          CLAP_EXT_LATENCY);
    if (AHost->ext.log)                 SAT_Log("  host supports ext '%s'\n",          CLAP_EXT_LOG);
    if (AHost->ext.midi_mappings)       SAT_Log("  host supports ext '%s' (draft)\n",  CLAP_EXT_MIDI_MAPPINGS);
    if (AHost->ext.note_name)           SAT_Log("  host supports ext '%s'\n",          CLAP_EXT_NOTE_NAME);

    if (AHost->ext.note_ports) {
      SAT_Log("  host supports ext '%s'\n", CLAP_EXT_NOTE_PORTS);
      uint32_t dialects = AHost->ext.note_ports->supported_dialects(host);
      if (dialects & CLAP_NOTE_DIALECT_CLAP)      SAT_Log("    CLAP_NOTE_DIALECT_CLAP\n");
      if (dialects & CLAP_NOTE_DIALECT_MIDI)      SAT_Log("    CLAP_NOTE_DIALECT_MIDI\n");
      if (dialects & CLAP_NOTE_DIALECT_MIDI_MPE)  SAT_Log("    CLAP_NOTE_DIALECT_MIDI_MPE\n");
      if (dialects & CLAP_NOTE_DIALECT_MIDI2)     SAT_Log("    CLAP_NOTE_DIALECT_MIDI2\n");
    }

    if (AHost->ext.params)              SAT_Log("  host supports ext '%s'\n",          CLAP_EXT_PARAMS);
    if (AHost->ext.posix_fd_support)    SAT_Log("  host supports ext '%s'\n",          CLAP_EXT_POSIX_FD_SUPPORT);
    if (AHost->ext.preset_load)         SAT_Log("  host supports ext '%s' (draft)\n",  CLAP_EXT_PRESET_LOAD);
    if (AHost->ext.remote_controls)     SAT_Log("  host supports ext '%s' (draft)\n",  CLAP_EXT_REMOTE_CONTROLS);
    if (AHost->ext.resource_directory)  SAT_Log("  host supports ext '%s' (draft)\n",  CLAP_EXT_RESOURCE_DIRECTORY);
    if (AHost->ext.state)               SAT_Log("  host supports ext '%s'\n",          CLAP_EXT_STATE);

    if (AHost->ext.surround) {
      SAT_Log("  host supports ext '%s' (draft)\n", CLAP_EXT_SURROUND);
      //uint8_t channel_map[256] = {0};
      //uint32_t count = 0;
      //AHost->ext.surround->get_preferred_channel_map(host,channel_map, 256, &count);
      //for (uint32_t i=0; i<count; i++) {
      //  switch (channel_map[i]) {
      //    case CLAP_SURROUND_FL:  SAT_Log("CLAP_SURROUND_FL\n");  break;
      //    case CLAP_SURROUND_FR:  SAT_Log("CLAP_SURROUND_FR\n");  break;
      //    case CLAP_SURROUND_FC:  SAT_Log("CLAP_SURROUND_FC\n");  break;
      //    case CLAP_SURROUND_LFE: SAT_Log("CLAP_SURROUND_LFE\n"); break;
      //    case CLAP_SURROUND_BL:  SAT_Log("CLAP_SURROUND_BL\n");  break;
      //    case CLAP_SURROUND_BR:  SAT_Log("CLAP_SURROUND_BR\n");  break;
      //    case CLAP_SURROUND_FLC: SAT_Log("CLAP_SURROUND_FLC\n"); break;
      //    case CLAP_SURROUND_FRC: SAT_Log("CLAP_SURROUND_FRC\n"); break;
      //    case CLAP_SURROUND_BC:  SAT_Log("CLAP_SURROUND_BC\n");  break;
      //    case CLAP_SURROUND_SL:  SAT_Log("CLAP_SURROUND_SL\n");  break;
      //    case CLAP_SURROUND_SR:  SAT_Log("CLAP_SURROUND_SR\n");  break;
      //    case CLAP_SURROUND_TC:  SAT_Log("CLAP_SURROUND_TC\n");  break;
      //    case CLAP_SURROUND_TFL: SAT_Log("CLAP_SURROUND_TFL\n"); break;
      //    case CLAP_SURROUND_TFC: SAT_Log("CLAP_SURROUND_TFC\n"); break;
      //    case CLAP_SURROUND_TFR: SAT_Log("CLAP_SURROUND_TFR\n"); break;
      //    case CLAP_SURROUND_TBL: SAT_Log("CLAP_SURROUND_TBL\n"); break;
      //    case CLAP_SURROUND_TBC: SAT_Log("CLAP_SURROUND_TBC\n"); break;
      //    case CLAP_SURROUND_TBR: SAT_Log("CLAP_SURROUND_TBR\n"); break;
      //  }
      //}
    }

    if (AHost->ext.tail) {
      SAT_Log("  host supports ext '%s'\n", CLAP_EXT_TAIL);
    }

    if (AHost->ext.thread_check) {
      SAT_Log("  host supports ext '%s'\n", CLAP_EXT_THREAD_CHECK);
      if (AHost->ext.thread_check->is_main_thread(host))   SAT_Log("    is_main_thread\n");
      if (AHost->ext.thread_check->is_audio_thread(host))  SAT_Log("    is_audio_thread\n");
    }

    if (AHost->ext.thread_pool)         SAT_Log("  host supports ext '%s'\n",          CLAP_EXT_THREAD_POOL);
    if (AHost->ext.timer_support)       SAT_Log("  host supports ext '%s'\n",          CLAP_EXT_TIMER_SUPPORT);

    if (AHost->ext.track_info) {
      SAT_Log("  host supports ext '%s' (draft)\n",  CLAP_EXT_TRACK_INFO);
      clap_track_info_t info;
      AHost->ext.track_info->get(host,&info);
      SAT_Log("    name: %s\n",info.name);
      SAT_Log("    color %.2f, .2f, .2f\n",info.color.red,info.color.green,info.color.blue);
      SAT_Log("    channel_count: %i\n",info.audio_channel_count);
      SAT_Log("    port_type: %s\n",info.audio_port_type);
      if (info.flags & CLAP_TRACK_INFO_HAS_TRACK_NAME)      SAT_Log("    CLAP_TRACK_INFO_HAS_TRACK_NAME\n");
      if (info.flags & CLAP_TRACK_INFO_HAS_TRACK_COLOR)     SAT_Log("    CLAP_TRACK_INFO_HAS_TRACK_COLOR\n");
      if (info.flags & CLAP_TRACK_INFO_HAS_AUDIO_CHANNEL)   SAT_Log("    CLAP_TRACK_INFO_HAS_AUDIO_CHANNEL\n");
      if (info.flags & CLAP_TRACK_INFO_IS_FOR_RETURN_TRACK) SAT_Log("    CLAP_TRACK_INFO_IS_FOR_RETURN_TRACK\n");
      if (info.flags & CLAP_TRACK_INFO_IS_FOR_BUS)          SAT_Log("    CLAP_TRACK_INFO_IS_FOR_BUS\n");
      if (info.flags & CLAP_TRACK_INFO_IS_FOR_MASTER)       SAT_Log("    CLAP_TRACK_INFO_IS_FOR_MASTER\n");
    }

    if (AHost->ext.transport_control)   SAT_Log("  host supports ext '%s' (draft)\n",  CLAP_EXT_TRANSPORT_CONTROL);
    if (AHost->ext.triggers)            SAT_Log("  host supports ext '%s' (draft)\n",  CLAP_EXT_TRIGGERS);

    if (AHost->ext.tuning) {
      SAT_Log("  host supports ext '%s' (draft)\n",  CLAP_EXT_TUNING);
      uint32_t count = AHost->ext.tuning->get_tuning_count(host);
      SAT_Log("    tuning_count %i\n",count);
    }
  }


//----------------------------------------------------------------------
#endif
