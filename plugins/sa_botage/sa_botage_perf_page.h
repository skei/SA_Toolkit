#ifndef sa_botage_perf_page_included
#define sa_botage_perf_page_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/sat_widgets.h"
  
//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_botage_perf_page_widget
: public SAT_VisualWidget {

public:

  sa_botage_perf_page_widget(SAT_Rect ARect)
  : SAT_VisualWidget(ARect) {
    
    setFillBackground(true);
    setBackgroundColor(0.18);
    
    setDrawBorder(false);
    setBorderColor(0.2);
    
  }
  
};
  
 
//----------------------------------------------------------------------
#endif