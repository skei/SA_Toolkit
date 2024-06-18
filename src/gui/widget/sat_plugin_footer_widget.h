#ifndef sat_plugin_footer_included
#define sat_plugin_footer_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/widget/sat_visual_widget.h"

class SAT_PluginFooterWidget
: public SAT_TextWidget {
  
//------------------------------
private:
//------------------------------

//------------------------------
public:
//------------------------------

  SAT_PluginFooterWidget(SAT_Rect ARect, const char* AText="")
  : SAT_TextWidget(ARect) {
    setName("SAT_PluginFooterWidget");
    setHint("SAT_PluginFooterWidget");
    setText(AText);
    
    setFillBackground(true);
    setBackgroundColor(SAT_DarkestGrey);
    setDrawBorder(false);
    setDrawText(true);
    setTextColor(SAT_LightestGrey);
    setTextSize(10);
    setTextAlignment(SAT_TEXT_ALIGN_LEFT);
    //setTextOffset(SAT_Rect(5,0,5,0));

    //setAlignment(SAT_WIDGET_ALIGN_BOTTOM);
    //setStretching(SAT_WIDGET_STRETCH_HORIZONTAL);
    Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_BOTTOM;
    Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
    Layout.flags |= SAT_WIDGET_LAYOUT_FILL_BOTTOM;
    
  }
  
  //----------

  virtual ~SAT_PluginFooterWidget() {
  }
  
//------------------------------
public:
//------------------------------

  void on_widget_hint(SAT_Widget* AWidget, const char* AHint) override {
    setText(AHint);
    do_widget_redraw(this);
  }

  
};

//----------------------------------------------------------------------
#endif
