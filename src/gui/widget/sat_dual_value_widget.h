#ifndef sat_dual_value_widget_included
#define sat_dual_value_widget_included
//----------------------------------------------------------------------

#include "gui/widget/sat_drag_value_widget.h"

//----------------------------------------------------------------------

class SAT_DualValueWidget
: public SAT_DragValueWidget {

//------------------------------
public:
//------------------------------

  SAT_DualValueWidget(SAT_Rect ARect)
  : SAT_DragValueWidget(ARect) {
    setName("SAT_DualValueWidget");
    setHint("SAT_DualValueWidget");
    setCursor(SAT_CURSOR_ARROW_LEFT_RIGHT);
  }

  //----------

  virtual ~SAT_DualValueWidget() {
  }

};

//----------------------------------------------------------------------
#endif