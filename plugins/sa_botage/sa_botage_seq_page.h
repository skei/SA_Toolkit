#ifndef sa_botage_seq_page_included
#define sa_botage_seq_page_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "gui/sat_widgets.h"
  
//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_botage_seq_page_widget
: public SAT_PanelWidget {

public:

  sa_botage_seq_page_widget(SAT_Rect ARect)
  : SAT_PanelWidget(ARect) {
    
    setFillBackground(true);
    setBackgroundColor(0.18);
    
    setDrawBorder(false);
    setBorderColor(0.2);
    
  }
  
};
  
 
//----------------------------------------------------------------------
#endif