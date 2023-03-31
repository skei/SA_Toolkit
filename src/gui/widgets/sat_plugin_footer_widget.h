#ifndef sat_plugin_footer_included
#define sat_plugin_footer_included
//----------------------------------------------------------------------

#include "base/sat.h"
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

    setAlignment(SAT_EDGE_BOTTOM);
    setStretching(SAT_EDGE_LEFT | SAT_EDGE_RIGHT);
    
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
