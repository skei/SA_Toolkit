#ifndef sa_demo_page_host_included
#define sa_demo_page_host_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "gui/sat_widgets.h"

class sa_demo_page_host
: public SAT_PanelWidget {
  
  char host_name[100]         = {0};
  char host_vendor[100]       = {0};
  char host_version[100]      = {0};
  char host_url[100]          = {0};
  
  char event_reg_buffer[300]  = {0};
  char track_info_buffer[300] = {0};
  
//------------------------------
public:
//------------------------------

  sa_demo_page_host(SAT_Rect ARect, SAT_Host* AHost)
  : SAT_PanelWidget(ARect) {
    setName("sa_demo_page_host");
    setStretching(SAT_WIDGET_STRETCH_ALL);
    
    const clap_host_t* host = AHost->getHost();
    
    SAT_TextBoxWidget* textbox = new SAT_TextBoxWidget(200);
    appendChildWidget(textbox);
    textbox->setLayout(SAT_WIDGET_ALIGN_LEFT_TOP,SAT_WIDGET_STRETCH_VERTICAL);
    textbox->setTextSize(8);
    textbox->getContentWidget()->setInnerBorder(SAT_Rect(5,5,5,5));
    textbox->setWidthLimits(100,500);
    
    {
      
      textbox->appendLine("",false);
      textbox->appendLine("The quick brown fox jumps over the lazy dog",false);
      textbox->appendLine("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.",false);
      textbox->appendLine("",false);

      strcpy(host_name,       "Name: ");        strcat(host_name,AHost->getName());
      strcpy(host_vendor,     "Vendor: ");      strcat(host_vendor,AHost->getVendor());
      strcpy(host_version,    "Version: ");     strcat(host_version,AHost->getVersion());
      strcpy(host_url,        "Url: ");         strcat(host_url,AHost->getUrl());
      
      textbox->appendLine(host_name,false);
      textbox->appendLine(host_vendor,false);
      textbox->appendLine(host_version,false);
      textbox->appendLine(host_url,false);
      
      textbox->appendLine("",false);
      textbox->appendLine("host supports:",false);
      textbox->appendLine("",false);
      
      if (AHost->ext.ambisonic) {
        textbox->appendLine(CLAP_EXT_AMBISONIC);
      }

      if (AHost->ext.audio_ports) {
        textbox->appendLine(CLAP_EXT_AUDIO_PORTS);
        if (AHost->ext.audio_ports->is_rescan_flag_supported(host,CLAP_AUDIO_PORTS_RESCAN_NAMES))         textbox->appendLine("    supported: CLAP_AUDIO_PORTS_RESCAN_NAMES");
        if (AHost->ext.audio_ports->is_rescan_flag_supported(host,CLAP_AUDIO_PORTS_RESCAN_FLAGS))         textbox->appendLine("    supported: CLAP_AUDIO_PORTS_RESCAN_FLAGS");
        if (AHost->ext.audio_ports->is_rescan_flag_supported(host,CLAP_AUDIO_PORTS_RESCAN_CHANNEL_COUNT)) textbox->appendLine("    supported: CLAP_AUDIO_PORTS_RESCAN_CHANNEL_COUNT");
        if (AHost->ext.audio_ports->is_rescan_flag_supported(host,CLAP_AUDIO_PORTS_RESCAN_PORT_TYPE))     textbox->appendLine("    supported: CLAP_AUDIO_PORTS_RESCAN_PORT_TYPE");
        if (AHost->ext.audio_ports->is_rescan_flag_supported(host,CLAP_AUDIO_PORTS_RESCAN_IN_PLACE_PAIR)) textbox->appendLine("    supported: CLAP_AUDIO_PORTS_RESCAN_IN_PLACE_PAIR");
        if (AHost->ext.audio_ports->is_rescan_flag_supported(host,CLAP_AUDIO_PORTS_RESCAN_LIST))          textbox->appendLine("    supported: LAP_AUDIO_PORTS_RESCAN_LIST");
      }

      if (AHost->ext.audio_ports_config) {
        textbox->appendLine(CLAP_EXT_AUDIO_PORTS_CONFIG);
      }
        
      if (AHost->ext.check_for_update) {
        textbox->appendLine(CLAP_EXT_CHECK_FOR_UPDATE);
      }
      
      if (AHost->ext.context_menu) {
        textbox->appendLine(CLAP_EXT_CONTEXT_MENU);
        if (AHost->ext.context_menu->can_popup(host)) textbox->appendLine("    can_popup");
      }
      
      if (AHost->ext.cv) {
        textbox->appendLine(CLAP_EXT_CV);
      }
        
      if (AHost->ext.event_registry) {
        textbox->appendLine(CLAP_EXT_EVENT_REGISTRY);
      }
        
      if (AHost->ext.gui) {
        textbox->appendLine(CLAP_EXT_GUI);
      }
        
      if (AHost->ext.latency) {
        textbox->appendLine(CLAP_EXT_LATENCY);
      }
        
      if (AHost->ext.log) {
        textbox->appendLine(CLAP_EXT_LOG);
      }
        
      if (AHost->ext.midi_mappings) {
        textbox->appendLine(CLAP_EXT_MIDI_MAPPINGS);
      }
        
      if (AHost->ext.note_name) {
        textbox->appendLine(CLAP_EXT_NOTE_NAME);
      }

      if (AHost->ext.note_ports) {
        textbox->appendLine(CLAP_EXT_NOTE_PORTS);
        uint32_t dialects = AHost->ext.note_ports->supported_dialects(host);
        if (dialects & CLAP_NOTE_DIALECT_CLAP)      textbox->appendLine("    supported: CLAP_NOTE_DIALECT_CLAP");
        if (dialects & CLAP_NOTE_DIALECT_MIDI)      textbox->appendLine("    supported: CLAP_NOTE_DIALECT_MIDI");
        if (dialects & CLAP_NOTE_DIALECT_MIDI_MPE)  textbox->appendLine("    supported: CLAP_NOTE_DIALECT_MIDI_MPE");
        if (dialects & CLAP_NOTE_DIALECT_MIDI2)     textbox->appendLine("    supported: CLAP_NOTE_DIALECT_MIDI2");
      }


      if (AHost->ext.params) {
        textbox->appendLine(CLAP_EXT_PARAMS);
      }
        
      if (AHost->ext.posix_fd_support) {
        textbox->appendLine(CLAP_EXT_POSIX_FD_SUPPORT);
      }
        
      if (AHost->ext.preset_load) {
        textbox->appendLine(CLAP_EXT_PRESET_LOAD);
      }
        
      if (AHost->ext.remote_controls) {
        textbox->appendLine(CLAP_EXT_REMOTE_CONTROLS);
      }
        
      if (AHost->ext.resource_directory) {
        textbox->appendLine(CLAP_EXT_RESOURCE_DIRECTORY);
      }
        
      if (AHost->ext.state) {
        textbox->appendLine(CLAP_EXT_STATE);
      }

      if (AHost->ext.surround) {
        textbox->appendLine(CLAP_EXT_SURROUND);      
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
        textbox->appendLine(CLAP_EXT_TAIL);
      }

      if (AHost->ext.thread_check) {
        textbox->appendLine(CLAP_EXT_THREAD_CHECK);
      }
        
      if (AHost->ext.thread_check) {
        if (AHost->ext.thread_check->is_main_thread(host))   textbox->appendLine("    is_main_thread");
        if (AHost->ext.thread_check->is_audio_thread(host))  textbox->appendLine("    is_audio_thread");
      }

      if (AHost->ext.thread_pool) {
        textbox->appendLine(CLAP_EXT_THREAD_POOL);
      }
        
      if (AHost->ext.timer_support) {
        textbox->appendLine(CLAP_EXT_TIMER_SUPPORT);
      }

      if (AHost->ext.track_info) {
        textbox->appendLine(CLAP_EXT_TRACK_INFO);
        clap_track_info_t info;
        AHost->ext.track_info->get(host,&info);
        //if (info.flags & CLAP_TRACK_INFO_HAS_TRACK_NAME)      textbox->appendLine("    CLAP_TRACK_INFO_HAS_TRACK_NAME\n");
        //if (info.flags & CLAP_TRACK_INFO_HAS_TRACK_COLOR)     textbox->appendLine("    CLAP_TRACK_INFO_HAS_TRACK_COLOR\n");
        //if (info.flags & CLAP_TRACK_INFO_HAS_AUDIO_CHANNEL)   textbox->appendLine("    CLAP_TRACK_INFO_HAS_AUDIO_CHANNEL\n");
        //if (info.flags & CLAP_TRACK_INFO_IS_FOR_RETURN_TRACK) textbox->appendLine("    CLAP_TRACK_INFO_IS_FOR_RETURN_TRACK\n");
        //if (info.flags & CLAP_TRACK_INFO_IS_FOR_BUS)          textbox->appendLine("    CLAP_TRACK_INFO_IS_FOR_BUS\n");
        //if (info.flags & CLAP_TRACK_INFO_IS_FOR_MASTER)       textbox->appendLine("    CLAP_TRACK_INFO_IS_FOR_MASTER\n");
        sprintf(track_info_buffer,"    name: '%s' color %i,%i,%i chan: %i port '%s'",info.name,info.color.red,info.color.green,info.color.blue,info.audio_channel_count,info.audio_port_type);
        textbox->appendLine(track_info_buffer);
      }

      if (AHost->ext.transport_control) {
        textbox->appendLine(CLAP_EXT_TRANSPORT_CONTROL);
      }
        
      if (AHost->ext.triggers) {
        textbox->appendLine(CLAP_EXT_TRIGGERS);
      }
      
      if (AHost->ext.tuning) {
        textbox->appendLine(CLAP_EXT_TUNING);
        //uint32_t count = AHost->ext.tuning->get_tuning_count(host);
        //textbox->appendLine("    tuning_count %i\n",count);
      }
      
      if (AHost->ext.voice_info) {
        textbox->appendLine(CLAP_EXT_VOICE_INFO);
      }
      
    } // textbox
    
    // sizer
    
    SAT_SizerWidget* sizer = new SAT_SizerWidget(SAT_Rect(3),SAT_DIRECTION_LEFT,textbox);
    appendChildWidget(sizer);
    sizer->setFillBackground(true);
    sizer->setBackgroundColor(0.3);
    sizer->setDrawBorder(false);
    
  }
    
  //----------
  
  
  
  //----------

  virtual ~sa_demo_page_host() {
  }

};

//----------------------------------------------------------------------
#endif