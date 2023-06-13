#ifndef sa_host_editor_included
#define sa_host_editor_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "plugin/sat_editor.h"
#include "plugin/sat_host.h"
#include "plugin/sat_plugin.h"

//----------------------------------------------------------------------

class sa_host_editor
: public SAT_Editor
/*, public SAT_WidgetListener*/ {
  
//------------------------------
private:
//------------------------------

  SAT_Host*           MHost           = nullptr;
  SAT_Plugin*         MPlugin         = nullptr;
  
  SAT_MenuWidget*     MFileMenu       = nullptr;
  SAT_MenuWidget*     MHostMenu       = nullptr;
  SAT_MenuWidget*     MPluginMenu     = nullptr;
  SAT_MenuWidget*     MSetupMenu      = nullptr;
  
  SAT_SelectorWidget* MFileSelector   = nullptr;
  SAT_SelectorWidget* MHostSelector   = nullptr;
  SAT_SelectorWidget* MPluginSelector = nullptr;
  SAT_SelectorWidget* MSetupSelector  = nullptr;
  
//------------------------------
public:
//------------------------------

  sa_host_editor(SAT_EditorListener* AListener, uint32_t AWidth, uint32_t AHeight, SAT_Host* AHost, SAT_Plugin* APlugin)
  : SAT_Editor(AListener,AWidth,AHeight) {
    SAT_PRINT;
    MHost = AHost;
    MPlugin = APlugin;
  }

  //----------

  virtual ~sa_host_editor() {
    SAT_PRINT;
  }
  
//------------------------------
public:
//------------------------------

  void do_windowListener_update_widget(SAT_Widget* ASender, uint32_t AMode, uint32_t AIndex) override {
    //SAT_PRINT;
    SAT_Editor::do_windowListener_update_widget(ASender,AMode,AIndex);
    double v = 0.0;
    uint32_t i = 0;
    
    if (ASender == MFileSelector) {
      uint32_t num = MFileMenu->getNumChildWidgets();
      if (num > 0) {
        v = MFileSelector->getValue();
        i = v * (num - 1);
        SAT_Print("file/%i\n",i);
      }
    }
    else if (ASender == MHostSelector) {
      uint32_t num = MHostMenu->getNumChildWidgets();
      if (num > 0) {
        v = MHostSelector->getValue();
        i = v * (num - 1);
        SAT_Print("host/%i\n",i);
      }
    }
    
  }
  
//------------------------------
public:
//------------------------------

  bool initialize(SAT_Window* AWindow, const char* AFormat) {
    SAT_PRINT;
    
    SAT_Window* window = getWindow();
    SAT_PanelWidget* panel = new SAT_PanelWidget(0);
    window->appendRootWidget(panel);
    
    //----------
    
    MFileMenu   = new SAT_MenuWidget(SAT_Rect(60, (3*15)+10 ));
    MHostMenu   = new SAT_MenuWidget(SAT_Rect(60, (3*15)+10 ));
    MPluginMenu = new SAT_MenuWidget(SAT_Rect(60, (3*15)+10 ));
    MSetupMenu  = new SAT_MenuWidget(SAT_Rect(60, (4*15)+10 ));
    
    MFileMenu->setInnerBorder(SAT_Rect(5,5,5,5));
    MFileMenu->appendChildWidget( new SAT_MenuItemWidget(SAT_Rect(15),"Load"));
    MFileMenu->appendChildWidget( new SAT_MenuItemWidget(SAT_Rect(15),"Save"));
    MFileMenu->appendChildWidget( new SAT_MenuItemWidget(SAT_Rect(15),"Exit"));
    
    MHostMenu->setInnerBorder(SAT_Rect(5,5,5,5));
    MHostMenu->appendChildWidget( new SAT_MenuItemWidget(SAT_Rect(15),"..."));
    MHostMenu->appendChildWidget( new SAT_MenuItemWidget(SAT_Rect(15),"..."));
    MHostMenu->appendChildWidget( new SAT_MenuItemWidget(SAT_Rect(15),".."));
    
    MPluginMenu->setInnerBorder(SAT_Rect(5,5,5,5));
    MPluginMenu->appendChildWidget( new SAT_MenuItemWidget(SAT_Rect(15),"Load plugin"));
    MPluginMenu->appendChildWidget( new SAT_MenuItemWidget(SAT_Rect(15),"Open editor"));
    MPluginMenu->appendChildWidget( new SAT_MenuItemWidget(SAT_Rect(15),"Load preset"));

    MSetupMenu->setInnerBorder(SAT_Rect(5,5,5,5));
    MSetupMenu->appendChildWidget( new SAT_MenuItemWidget(SAT_Rect(15),"Audio"));
    MSetupMenu->appendChildWidget( new SAT_MenuItemWidget(SAT_Rect(15),"Midi"));
    MSetupMenu->appendChildWidget( new SAT_MenuItemWidget(SAT_Rect(15),"Paths"));
    MSetupMenu->appendChildWidget( new SAT_MenuItemWidget(SAT_Rect(15),"Options"));

    //----------
    
    MFileSelector   = new SAT_SelectorWidget(SAT_Rect(  0,0, 40,15), "File",   MFileMenu  );
    MHostSelector   = new SAT_SelectorWidget(SAT_Rect( 40,0, 40,15), "Host",   MHostMenu  );
    MPluginSelector = new SAT_SelectorWidget(SAT_Rect( 80,0, 40,15), "Plugin", MPluginMenu);
    MSetupSelector  = new SAT_SelectorWidget(SAT_Rect(120,0, 40,15), "Setup",  MSetupMenu );

    panel->appendChildWidget(MFileSelector);
    panel->appendChildWidget(MHostSelector);
    panel->appendChildWidget(MPluginSelector);
    panel->appendChildWidget(MSetupSelector);

    MFileSelector->setName("File");
    MFileSelector->setDrawSelectedText(false);
    MFileSelector->setDrawArrow(false);
    MFileSelector->setTextSize(8);
    //MFileSelector->setAutoSizeMenu(true);
    
    MHostSelector->setName("Host");
    MHostSelector->setDrawSelectedText(false);
    MHostSelector->setDrawArrow(false);
    MHostSelector->setTextSize(8);
    //MHostSelector->setAutoSizeMenu(true);
    
    MPluginSelector->setName("Plugin");
    MPluginSelector->setDrawSelectedText(false);
    MPluginSelector->setDrawArrow(false);
    MPluginSelector->setTextSize(8);
    //MPluginSelector->setAutoSizeMenu(true);
    
    MSetupSelector->setName("Setup");
    MSetupSelector->setDrawSelectedText(false);
    MSetupSelector->setDrawArrow(false);
    MSetupSelector->setTextSize(8);
    //MSetupSelector->setAutoSizeMenu(true);
    
    //----------
    
    panel->appendChildWidget(MFileMenu);
    panel->appendChildWidget(MHostMenu);
    panel->appendChildWidget(MPluginMenu);
    panel->appendChildWidget(MSetupMenu);
    
    return true;
  }  

};

//----------------------------------------------------------------------
#endif


