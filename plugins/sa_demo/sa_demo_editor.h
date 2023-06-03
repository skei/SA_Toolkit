#ifndef sa_demo_editor_included
#define sa_demo_editor_included
//----------------------------------------------------------------------

#include "sa_demo_page_host.h"
#include "sa_demo_page_plugin.h"
#include "sa_demo_page_parameters.h"
#include "sa_demo_page_audio.h"
#include "sa_demo_page_synth.h"
#include "sa_demo_page_widgets.h"
#include "sa_demo_page_layout.h"
#include "sa_demo_page_painting.h"
#include "sa_demo_page_animation.h"

class sa_demo_editor
: public SAT_Editor
/*, public SAT_WidgetListener*/ {
  
//------------------------------
private:
//------------------------------

  SAT_Host*   MHost   = nullptr;
  SAT_Plugin* MPlugin = nullptr;
  
  sa_demo_page_widgets* MPageWidgets = nullptr;

  #define NUM_DEMOS 9

  const char* demo_buttontext[NUM_DEMOS] = {
    "Host",
    "Plugin",
    "Parameters",
    "Audio",
    "Synth",
    "Widgets",
    "Layout",
    "Painting",
    "Animation"
  };
  
  SAT_PanelWidget*        MRoot         = nullptr;
  SAT_PluginHeaderWidget* MPluginHeader = nullptr;
  SAT_PluginFooterWidget* MPluginFooter = nullptr;
  SAT_PanelWidget*        MCenterPanel  = nullptr;
  SAT_PanelWidget*        MLeftColumn   = nullptr;
  SAT_PagesWidget*        MDemoPages    = nullptr;
  SAT_ButtonRowWidget*    MDemoButtons  = nullptr;

//------------------------------
public:
//------------------------------

  sa_demo_editor(SAT_EditorListener* AListener, uint32_t AWidth, uint32_t AHeight, SAT_Host* AHost, SAT_Plugin* APlugin)
  : SAT_Editor(AListener,AWidth,AHeight) {
    MHost = AHost;
    MPlugin = APlugin;
  }

  //----------

  virtual ~sa_demo_editor() {
  }
  
//------------------------------
public:
//------------------------------

  void set_track_name() {
  }

//------------------------------
public:
//------------------------------

//  void do_widgetListener_select(SAT_Widget* ASender, int32_t AIndex, int32_t ASubIndex) {
//    if (ASender == MDemoButtons) {
//      SAT_PRINT;
//    }
//  }

  void do_windowListener_update_widget(SAT_Widget* ASender, uint32_t AMode, uint32_t AIndex) override {
    if (ASender == MDemoButtons) {
      SAT_ButtonRowWidget* buttons = (SAT_ButtonRowWidget*)ASender;
      uint32_t page = buttons->getSelected();
      //SAT_Print("selected %i\n",page);
      MDemoPages->setPage(page);
      MDemoPages->parentRedraw();
    }
    SAT_Editor::do_windowListener_update_widget(ASender,AMode,AIndex);
  }
  
  //----------
  
//  void do_windowListener_timer(SAT_Window* AWindow, double AElapsed) {
//    if (MHost->ext.track_info) {
//      SAT_Print("track info!\n");
//      char buf1[512] = {0};
//      char buf2[512] = {0};
//      clap_track_info_t info;
//      if (MHost->ext.track_info->get(MHost->getHost(),&info)) {
//        //uint64_t flags; // see the flags above
//        sprintf(buf1,"flags: %i ",(int)info.flags);
//        strcat(buf2,buf1);
//        if (info.flags & CLAP_TRACK_INFO_HAS_TRACK_NAME) {
//          //char name[CLAP_NAME_SIZE]; // track name, available if flags contain CLAP_TRACK_INFO_HAS_TRACK_NAME
//          sprintf(buf1,"name: '%s' ",info.name);
//          strcat(buf2,buf1);
//        }
//        if (info.flags & CLAP_TRACK_INFO_HAS_TRACK_COLOR) {
//          //clap_color_t color; // track color, available if flags contain CLAP_TRACK_INFO_HAS_TRACK_COLOR
//          sprintf(buf1,"color: %i,%i,%i ",info.color.red,info.color.green,info.color.blue);
//          strcat(buf2,buf1);
//        }
//        if (info.flags & CLAP_TRACK_INFO_HAS_AUDIO_CHANNEL) {
//          //int32_t audio_channel_count;
//          //const char *audio_port_type;
//          sprintf(buf1,"channels %i port<_type '%s' ",info.audio_channel_count,info.audio_port_type);
//          strcat(buf2,buf1);
//        }
//        if (info.flags & CLAP_TRACK_INFO_IS_FOR_RETURN_TRACK) { strcat(buf2,"Return"); }  // This plugin is on a return track, initialize with wet 100%
//        if (info.flags & CLAP_TRACK_INFO_IS_FOR_BUS) { strcat(buf2,"Bus"); }           // This plugin is on a bus track, initialize with appropriate settings for bus processing
//        if (info.flags & CLAP_TRACK_INFO_IS_FOR_MASTER) { strcat(buf2,"Master"); }        // This plugin is on the master, initialize with appropriate settings for channel processing
//        SAT_Print("%s\n",buf2);
//      }
//    }
//    else {
//      SAT_Print("no track info!\n");
//    }
//    SAT_Editor::do_windowListener_timer(AWindow,AElapsed);
//  }

//------------------------------
public:
//------------------------------

  bool initialize(SAT_Window* AWindow, const char* AFormat) {
    //SAT_PRINT;
    
    // root

    MRoot = new SAT_PanelWidget( SAT_Rect(0,0,EDITOR_WIDTH,EDITOR_HEIGHT) );
    AWindow->appendRootWidget(MRoot);
    MRoot->setFillBackground(false);
    MRoot->setDrawBorder(false);
    
    // header
    
    //const char* format = getPluginFormat();
    MPluginHeader = new SAT_PluginHeaderWidget(SAT_Rect(0,0,EDITOR_WIDTH,40),"toolkit demo",AFormat);
    MRoot->appendChildWidget(MPluginHeader);
    
    // footer
    
    MPluginFooter = new SAT_PluginFooterWidget(SAT_Rect(0,0,EDITOR_WIDTH,20));
    MRoot->appendChildWidget(MPluginFooter);
    
    // main
    
    MCenterPanel = new SAT_PanelWidget(0);
    MRoot->appendChildWidget(MCenterPanel);
    MCenterPanel->setLayout(SAT_WIDGET_ALIGN_FILL,SAT_WIDGET_STRETCH_ALL);
    MCenterPanel->setFillBackground(false);
    MCenterPanel->setDrawBorder(false);
    {
      
      // left column
      
      MLeftColumn = new SAT_PanelWidget(100);
      MCenterPanel->appendChildWidget(MLeftColumn);
      MLeftColumn->setLayout(SAT_WIDGET_ALIGN_LEFT_TOP,SAT_WIDGET_STRETCH_VERTICAL);
      MLeftColumn->setInnerBorder(SAT_Rect(10,10,10,10));
      MLeftColumn->setSpacing(SAT_Point(10,10));
      MLeftColumn->setFillBackground(true);
      MLeftColumn->setBackgroundColor(0.2);
      MLeftColumn->setDrawBorder(false);
      {
        
        // buttons
        
        //MDemoButtons = new SAT_ButtonRowWidget(SAT_Rect(0,0,100,NUM_DEMOS*15),NUM_DEMOS,demo_buttontext,SAT_BUTTON_ROW_SINGLE,true);
        MDemoButtons = new SAT_ButtonRowWidget(SAT_Rect(NUM_DEMOS*15),NUM_DEMOS,demo_buttontext,SAT_BUTTON_ROW_SINGLE,true);
        MLeftColumn->appendChildWidget(MDemoButtons);
        MDemoButtons->setLayout(SAT_WIDGET_ALIGN_TOP_LEFT,SAT_WIDGET_STRETCH_HORIZONTAL);
        MDemoButtons->setTextSize(9);
        MDemoButtons->setTextColor(0.5);
        MDemoButtons->setActiveTextColor(SAT_LightOrange);
        MDemoButtons->setCellFillBackground(true);
        MDemoButtons->setCellBackgroundColor(0.2);
        MDemoButtons->setCellActiveBackgroundColor(0.16);
        MDemoButtons->setCellDrawBorder(false);
        MDemoButtons->selectButton(0);
        
        // test
        
        //SAT_TextWidget* test_text = new SAT_TextWidget(SAT_Rect(200,100),"Test");
        //MLeftColumn->appendChildWidget(test_text);
        //test_text->setLayout(SAT_WIDGET_ALIGN_TOP_LEFT,SAT_WIDGET_STRETCH_HORIZONTAL);
        //test_text->setFillBackground(true);
        //test_text->setBackgroundColor(SAT_LightBlue2);
        //test_text->setAutoTextSize(true);
        
      } // MLeftColumn
      
      // pages

      MDemoPages = new SAT_PagesWidget(0);
      MCenterPanel->appendChildWidget(MDemoPages);
      MDemoPages->setLayout(SAT_WIDGET_ALIGN_LEFT_TOP,SAT_WIDGET_STRETCH_RIGHT + SAT_WIDGET_STRETCH_BOTTOM);
      MDemoPages->setFillBackground(true);
      MDemoPages->setBackgroundColor(0.25);
      MDemoPages->setDrawBorder(false);
      {
        
        MPageWidgets = new sa_demo_page_widgets(0,MPlugin);

        MDemoPages->appendPage( new sa_demo_page_host(0,MHost) );
        MDemoPages->appendPage( new sa_demo_page_plugin(0,MPlugin) );
        MDemoPages->appendPage( new sa_demo_page_parameters(0,MPlugin) );
        MDemoPages->appendPage( new sa_demo_page_audio(0,MPlugin) );
        MDemoPages->appendPage( new sa_demo_page_synth(0,MPlugin) );
        MDemoPages->appendPage( MPageWidgets );
        MDemoPages->appendPage( new sa_demo_page_layout(0,MPlugin) );
        MDemoPages->appendPage( new sa_demo_page_painting(0,MPlugin) );
        MDemoPages->appendPage( new sa_demo_page_animation(0,MPlugin) );
        
      } // MDemoPages
      
      MDemoPages->setPage(0);

      //SAT_SelectorWidget* selector = MPageWidgets->getSelector1();
      //MRoot->appendChildWidget(MPageWidgets);
      SAT_MenuWidget* menu1 = MPageWidgets->getMenu1();
      MRoot->appendChildWidget(menu1);
      

    } // center_panel

    //--------------------
    
    return true;
  }
  
  
};

#undef NUM_DEMOS

//----------------------------------------------------------------------
#endif

