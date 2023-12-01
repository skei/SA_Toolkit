#ifndef sat_visual_widget_included
#define sat_visual_widget_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/sat_widget.h"

//----------------------------------------------------------------------

class SAT_VisualWidget
: public class SAT_Widget {

//------------------------------
public:
//------------------------------

  SAT_VisualWidget(SAT_Rect ARect)
  : SAT_Widget(ARect) {
  }

  virtual ~SAT_VisualWidget() {
  }

};

//----------------------------------------------------------------------
#endif
