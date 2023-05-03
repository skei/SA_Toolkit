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
  
//------------------------------
public:
//------------------------------

  sa_demo_page_host(SAT_Rect ARect, SAT_Host* AHost)
  : SAT_PanelWidget(ARect) {
    setName("sa_demo_page_host");
    setStretching(SAT_WIDGET_STRETCH_ALL);
    
    SAT_TextBoxWidget* textbox = new SAT_TextBoxWidget(SAT_Rect(10,10,300,400));
    appendChildWidget(textbox);
    textbox->setTextSize(8);
    textbox->getContentWidget()->setInnerBorder(SAT_Rect(5,5,5,5));

    strcpy(host_name,       "Name: ");        strcat(host_name,AHost->getName());
    strcpy(host_vendor,     "Vendor: ");      strcat(host_vendor,AHost->getVendor());
    strcpy(host_version,    "Version: ");     strcat(host_version,AHost->getVersion());
    strcpy(host_url,        "Url: ");         strcat(host_url,AHost->getUrl());
    
    textbox->appendLine(host_name,false);
    textbox->appendLine(host_vendor,false);
    textbox->appendLine(host_version,false);
    textbox->appendLine(host_url,false);
    textbox->appendLine("",false);
    textbox->appendLine("Extensions:",false);
    
    if (AHost->ext.ambisonic)                 textbox->appendLine(CLAP_EXT_AMBISONIC);
    if (AHost->ext.audio_ports)               textbox->appendLine(CLAP_EXT_AUDIO_PORTS);
    if (AHost->ext.audio_ports_config)        textbox->appendLine(CLAP_EXT_AUDIO_PORTS_CONFIG);
    if (AHost->ext.check_for_update)          textbox->appendLine(CLAP_EXT_CHECK_FOR_UPDATE);
    if (AHost->ext.context_menu)              textbox->appendLine(CLAP_EXT_CONTEXT_MENU);
  //if (AHost->ext.configurable_audio_ports)  textbox->appendLine(CLAP_EXT_CONFIGURABLE_AUDIO_PORTS);
    if (AHost->ext.cv)                        textbox->appendLine(CLAP_EXT_CV);
    if (AHost->ext.event_registry)            textbox->appendLine(CLAP_EXT_EVENT_REGISTRY);
    if (AHost->ext.gui)                       textbox->appendLine(CLAP_EXT_GUI);
    if (AHost->ext.latency)                   textbox->appendLine(CLAP_EXT_LATENCY);
    if (AHost->ext.log)                       textbox->appendLine(CLAP_EXT_LOG);
    if (AHost->ext.midi_mappings)             textbox->appendLine(CLAP_EXT_MIDI_MAPPINGS);
    if (AHost->ext.note_name)                 textbox->appendLine(CLAP_EXT_NOTE_NAME);
    if (AHost->ext.note_ports)                textbox->appendLine(CLAP_EXT_NOTE_PORTS);
    if (AHost->ext.params)                    textbox->appendLine(CLAP_EXT_PARAMS);
    if (AHost->ext.posix_fd_support)          textbox->appendLine(CLAP_EXT_POSIX_FD_SUPPORT);
    if (AHost->ext.preset_load)               textbox->appendLine(CLAP_EXT_PRESET_LOAD);
    if (AHost->ext.remote_controls)           textbox->appendLine(CLAP_EXT_REMOTE_CONTROLS);
    if (AHost->ext.resource_directory)        textbox->appendLine(CLAP_EXT_RESOURCE_DIRECTORY);
  //if (AHost->ext.render)                    textbox->appendLine(CLAP_EXT_RENDER);
    if (AHost->ext.state)                     textbox->appendLine(CLAP_EXT_STATE);
    if (AHost->ext.surround)                  textbox->appendLine(CLAP_EXT_SURROUND);
    if (AHost->ext.tail)                      textbox->appendLine(CLAP_EXT_TAIL);
    if (AHost->ext.thread_check)              textbox->appendLine(CLAP_EXT_THREAD_CHECK);
    if (AHost->ext.thread_pool)               textbox->appendLine(CLAP_EXT_THREAD_POOL);
    if (AHost->ext.timer_support)             textbox->appendLine(CLAP_EXT_TIMER_SUPPORT);
    if (AHost->ext.track_info)                textbox->appendLine(CLAP_EXT_TRACK_INFO);
    if (AHost->ext.transport_control)         textbox->appendLine(CLAP_EXT_TRANSPORT_CONTROL);
    if (AHost->ext.triggers)                  textbox->appendLine(CLAP_EXT_TRIGGERS);
    if (AHost->ext.tuning)                    textbox->appendLine(CLAP_EXT_TUNING);
    if (AHost->ext.voice_info)                textbox->appendLine(CLAP_EXT_VOICE_INFO);
    if (AHost->ext.voice_info)                textbox->appendLine(CLAP_EXT_VOICE_INFO);

  }
    
  //----------

  virtual ~sa_demo_page_host() {
  }

};

//----------------------------------------------------------------------
#endif