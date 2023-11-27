#ifndef sat_plugin_footer_included
#define sat_plugin_footer_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/widgets/sat_panel_widget.h"

class SAT_PluginFooterWidget
: public SAT_PanelWidget {
  
//------------------------------
private:
//------------------------------

//------------------------------
public:
//------------------------------

  SAT_PluginFooterWidget(SAT_Rect ARect, const char* AText="")
  : SAT_PanelWidget(ARect) {
    setName("SAT_PluginFooterWidget");
    setText(AText);
    
    setFillBackground(true);
    setBackgroundColor(SAT_DarkestGrey);
    setDrawBorder(false);
    setDrawText(true);
    setTextColor(SAT_LightestGrey);
    setTextAlignment(SAT_TEXT_ALIGN_LEFT);
    setTextOffset(SAT_Rect(5,0,5,0));

    //setAlignment(SAT_WIDGET_ALIGN_BOTTOM);
    //setStretching(SAT_WIDGET_STRETCH_HORIZONTAL);
    addLayoutFlag(SAT_WIDGET_LAYOUT_ANCHOR_BOTTOM);
    addLayoutFlag(SAT_WIDGET_LAYOUT_STRETCH_HORIZ);
    addLayoutFlag(SAT_WIDGET_LAYOUT_CROP_BOTTOM);
    
  }
  
  //----------

  virtual ~SAT_PluginFooterWidget() {
  }
  
//------------------------------
public:
//------------------------------
  
};

//----------------------------------------------------------------------
#endif
