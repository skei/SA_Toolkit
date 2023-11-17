#ifndef sat_plugin_footer_included
#define sat_plugin_footer_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/widgets/sat_text_widget.h"

class SAT_PluginFooterWidget
: public SAT_TextWidget {
  
//------------------------------
private:
//------------------------------

//------------------------------
public:
//------------------------------

  SAT_PluginFooterWidget(SAT_Rect ARect)
  : SAT_TextWidget(ARect,"...") {
    setName("SAT_PluginFooterWidget");
    
    setFillBackground(true);
    setBackgroundColor(SAT_DarkestGrey);
    setDrawBorder(false);
    setTextColor(SAT_LightestGrey);
    setTextAlignment(SAT_TEXT_ALIGN_LEFT);
    setTextOffset(SAT_Rect(5,0,5,0));

    setAlignment(SAT_WIDGET_ALIGN_BOTTOM);
    setStretching(SAT_WIDGET_STRETCH_HORIZONTAL);
    
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
