#ifndef sa_demo_page_animation_included
#define sa_demo_page_animation_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "gui/sat_widgets.h"

class sa_demo_page_animation
: public SAT_PanelWidget {
  
//------------------------------
public:
//------------------------------

  sa_demo_page_animation(SAT_Rect ARect, SAT_Plugin* APlugin)
  : SAT_PanelWidget(ARect) {
    setName("sa_demo_page_animation");
    setStretching(SAT_WIDGET_STRETCH_ALL);
    
    SAT_TextWidget* text1 = new SAT_TextWidget(SAT_Rect(10,10,200,15),"animation");
    appendChildWidget(text1);
    text1->setTextSize(9);
    
  }
  
  //----------

  virtual ~sa_demo_page_animation() {
  }

};

//----------------------------------------------------------------------
#endif