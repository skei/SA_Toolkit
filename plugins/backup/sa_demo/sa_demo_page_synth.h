#ifndef sa_demo_page_synth_included
#define sa_demo_page_synth_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "gui/sat_widgets.h"

class sa_demo_page_synth
: public SAT_PanelWidget {
  
//------------------------------
public:
//------------------------------

  sa_demo_page_synth(SAT_Rect ARect, SAT_Plugin* APlugin)
  : SAT_PanelWidget(ARect) {
    setName("sa_demo_page_synth");
    setStretching(SAT_WIDGET_STRETCH_ALL);
    
    SAT_TextWidget* text1 = new SAT_TextWidget(SAT_Rect(10,10,200,15),"synth");
    appendChildWidget(text1);
    text1->setTextSize(9);
    
  }
  
  //----------

  virtual ~sa_demo_page_synth() {
  }

};

//----------------------------------------------------------------------
#endif