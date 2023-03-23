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

  SAT_LogoWidget* MLogo = nullptr;
  const char*     MName = "";

//------------------------------
public:
//------------------------------

  SAT_PluginHeaderWidget(SAT_Rect ARect, const char* AName)
  : SAT_PanelWidget(ARect) {
    
    MName = AName;

    MLogo = new SAT_LogoWidget(SAT_Rect(5,5,30,30));
    appendChildWidget(MLogo);
    MLogo->setLogoColor(SAT_White);
    //MLogo->setAlignment(SAT_WIDGET_ALIGN_FILL_PARENT);
    
    setFillBackground(true);
    setBackgroundColor(SAT_DarkestGrey);
    setDrawBorder(false);
    
    setAlignment(SAT_EDGE_TOP);
    setAnchors(SAT_EDGE_LEFT | SAT_EDGE_RIGHT);
    
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
    
    painter->selectFont(header_font);
    painter->setTextColor(SAT_White);
    painter->setTextSize( 29 * S );
    
    SAT_Rect title_rect = mrect;
    title_rect.x += (40.0 * S);
    title_rect.y += (3.0  * S);
    painter->drawTextBox(title_rect,MName,SAT_TEXT_ALIGN_LEFT);
    
    painter->selectFont(default_font);

    paintChildWidgets(AContext);
    drawBorder(AContext);

  }
  
};

//----------------------------------------------------------------------
#endif
