#ifndef sat_plugin_header_included
#define sat_plugin_header_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/widgets/sat_panel_widget.h"

class SAT_PluginHeaderWidget
: public SAT_PanelWidget {
  
//------------------------------
private:
//------------------------------

  SAT_LogoWidget* MLogo                               = nullptr;
  double          MLogoSize                           = 0.0;

  char            MPluginName[SAT_MAX_NAME_LENGTH]    = {0};
  SAT_Color       MPluginNameColor                    = SAT_White;
  double          MPluginNameSize                     = 18.0;
  SAT_Rect        MPluginNameOffset                   = SAT_Rect(35,7,0,0);
  uint32_t        MPluginNameAlignment                = SAT_TEXT_ALIGN_LEFT;

//------------------------------
public:
//------------------------------

  SAT_PluginHeaderWidget(SAT_Rect ARect, const char* AName/*, const char* AFormat*/)
  : SAT_PanelWidget(ARect) {
    setName("SAT_PluginHeaderWidget");

    // if ( ((AName[0] == 's') || (AName[0] == 'S'))
    //   && ((AName[1] == 'a') || (AName[1] == 'A'))
    //   &&  (AName[2] == '_')) {
    //   AName += 3;
    // }

    MLogoSize = ARect.h - (10*2);

    strcpy(MPluginName,AName);

    MLogo = new SAT_LogoWidget(SAT_Rect(10,10,MLogoSize,MLogoSize));
    appendChildWidget(MLogo);
    MLogo->setLogoColor(SAT_White);
    MLogo->setLayoutFlags(SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT);
    
    setFillBackground(true);
    setBackgroundColor(SAT_DarkestGrey);
    setDrawBorder(false);
    
    addLayoutFlag(SAT_WIDGET_LAYOUT_ANCHOR_TOP);
    addLayoutFlag(SAT_WIDGET_LAYOUT_STRETCH_HORIZ);
    addLayoutFlag(SAT_WIDGET_LAYOUT_CROP_TOP);
    
  }
  
  //----------

  virtual ~SAT_PluginHeaderWidget() {
  }
  
//------------------------------
public:
//------------------------------

  virtual void setPluginNameColor(SAT_Color AColor)         { MPluginNameColor = AColor; }
  virtual void setPluginNameSize(double ASize)              { MPluginNameSize = ASize; }
  virtual void setPluginNameOffset(SAT_Rect AOffset)        { MPluginNameOffset = AOffset; }
  virtual void setPluginNameAlignment(uint32_t AAlignment)  { MPluginNameAlignment = AAlignment; }

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
    
    //

    painter->selectFont(default_font);
    paintChildWidgets(AContext);
    drawBorder(AContext);

  }
  
};

//----------------------------------------------------------------------
#endif
