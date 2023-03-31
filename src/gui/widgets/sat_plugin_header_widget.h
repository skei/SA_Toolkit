#ifndef sat_plugin_header_included
#define sat_plugin_header_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "gui/widgets/sat_panel_widget.h"

class SAT_PluginHeaderWidget
: public SAT_PanelWidget {
  
//------------------------------
private:
//------------------------------

  SAT_LogoWidget* MLogo             = nullptr;

  char            MPluginName[SAT_MAX_NAME_LENGTH]    = {0};
  SAT_Color       MPluginNameColor                    = SAT_White;
  double          MPluginNameSize                     = 29.0;
  SAT_Rect        MPluginNameOffset                   = SAT_Rect(40,8,0,0);
  uint32_t        MPluginNameAlignment                = SAT_TEXT_ALIGN_LEFT;

  char            MFormat[SAT_MAX_NAME_LENGTH] = {0};
  SAT_Color       MFormatColor                 = SAT_Grey;
  double          MFormatSize                  = 7.0;
  SAT_Rect        MFormatOffset                = SAT_Rect(0,5,5,0);
  uint32_t        MFormatAlignment             = SAT_TEXT_ALIGN_RIGHT | SAT_TEXT_ALIGN_TOP;

  char            MSatVer[SAT_MAX_NAME_LENGTH]  = {0};
  SAT_Color       MSatVerColor                  = SAT_Grey;
  double          MSatVerSize                   = 7.0;
  SAT_Rect        MSatVerOffset                 = SAT_Rect(0,14,5,0);
  uint32_t        MSatVerAlignment              = SAT_TEXT_ALIGN_RIGHT | SAT_TEXT_ALIGN_TOP;

  char            MClapVer[SAT_MAX_NAME_LENGTH] = {0};
  SAT_Color       MClapVerColor                 = SAT_Grey;
  double          MClapVerSize                  = 7.0;
  SAT_Rect        MClapVerOffset                = SAT_Rect(0,23,5,0);
  uint32_t        MClapVerAlignment             = SAT_TEXT_ALIGN_RIGHT | SAT_TEXT_ALIGN_TOP;


//------------------------------
public:
//------------------------------

  SAT_PluginHeaderWidget(SAT_Rect ARect, const char* AName, const char* AFormat)
  : SAT_PanelWidget(ARect) {
    setName("SAT_PluginHeaderWidget");
    
    strcpy(MPluginName,AName);
    //sprintf(MFormat,"Format: %s",AFormat);
    strcpy(MFormat,AFormat);
    
    #ifdef SAT_LINUX
      strcat(MFormat," (Linux)");
    #endif
    #ifdef SAT_WIN32
      strcat(MFormat," (Win32)");
    #endif
    
    sprintf(MClapVer,"CLAP %i.%i.%i",CLAP_VERSION_MAJOR,CLAP_VERSION_MINOR,CLAP_VERSION_REVISION);
    sprintf(MSatVer,"SAT %s",SAT_VERSION);
    
    MLogo = new SAT_LogoWidget(SAT_Rect(5,5,30,30));
    appendChildWidget(MLogo);
    MLogo->setLogoColor(SAT_White);
    //MLogo->setAlignment(SAT_WIDGET_ALIGN_FILL_PARENT);
    
    setFillBackground(true);
    setBackgroundColor(SAT_DarkestGrey);
    setDrawBorder(false);
    
    setAlignment(SAT_EDGE_TOP);
    setStretching(SAT_EDGE_LEFT | SAT_EDGE_RIGHT);
    
  }
  
  //----------

  virtual ~SAT_PluginHeaderWidget() {
  }
  
//------------------------------
public:
//------------------------------

  void on_widget_paint(SAT_PaintContext* AContext) override {
    //SAT_PanelWidget::on_widget_paint(AContext);
    drawDropShadow(AContext);
    fillBackground(AContext);
    
    SAT_Painter* painter= AContext->painter;
    SAT_Rect mrect = getRect();
    double S = getWindowScale();
    
    int header_font = painter->getHeaderFont();
    int default_font = painter->getDefaultFont();
    
    // plugin name
    
    painter->selectFont(header_font);
    painter->setTextColor(MPluginNameColor);
    painter->setTextSize( MPluginNameSize * S );
    
    SAT_Rect nofs = MPluginNameOffset;
    nofs.scale(S);
    SAT_Rect nrect = mrect;
    nrect.shrink(nofs);
    painter->drawTextBox(nrect,MPluginName,MPluginNameAlignment);
    
    // format
    
    painter->selectFont(default_font);
    painter->setTextColor(MFormatColor);
    painter->setTextSize( MFormatSize * S );

    SAT_Rect fofs = MFormatOffset;
    fofs.scale(S);
    SAT_Rect frect = mrect;
    frect.shrink(fofs);
    painter->drawTextBox(frect,MFormat,MFormatAlignment);
    
    // SAT version

    //painter->selectFont(default_font);
    painter->setTextColor(MSatVerColor);
    painter->setTextSize( MSatVerSize * S );

    SAT_Rect svofs = MSatVerOffset;
    svofs.scale(S);
    SAT_Rect svrect = mrect;
    svrect.shrink(svofs);
    painter->drawTextBox(svrect,MSatVer,MSatVerAlignment);
    
    // clap version
    
    //painter->selectFont(default_font);
    painter->setTextColor(MClapVerColor);
    painter->setTextSize( MClapVerSize * S );

    SAT_Rect cvofs = MClapVerOffset;
    cvofs.scale(S);
    SAT_Rect cvrect = mrect;
    cvrect.shrink(cvofs);
    painter->drawTextBox(cvrect,MClapVer,MClapVerAlignment);
    
    //

    //painter->selectFont(default_font);
    paintChildWidgets(AContext);
    drawBorder(AContext);

  }
  
};

//----------------------------------------------------------------------
#endif
