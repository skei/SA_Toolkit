#ifndef sa_demo_page_audio_included
#define sa_demo_page_audio_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "gui/sat_widgets.h"

class sa_demo_page_audio
: public SAT_PanelWidget {
  
//------------------------------
public:
//------------------------------

  sa_demo_page_audio(SAT_Rect ARect, SAT_Plugin* APlugin)
  : SAT_PanelWidget(ARect) {
    setName("sa_demo_page_audio");
    setStretching(SAT_WIDGET_STRETCH_ALL);
    
    SAT_TextWidget* text1 = new SAT_TextWidget(SAT_Rect(10,10,200,15),"audio");
    appendChildWidget(text1);
    text1->setTextSize(9);
    
  }
  
  //----------

  virtual ~sa_demo_page_audio() {
  }

};

//----------------------------------------------------------------------
#endif