#ifndef sat_base_widget_included
#define sat_base_widget_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/sat_widget_listener.h"

//----------------------------------------------------------------------

class SAT_BaseWidget
: public SAT_WidgetListener {

//------------------------------
public:
//------------------------------

  SAT_BaseWidget() {
  }

  //----------

  virtual ~SAT_BaseWidget() {
  }


};

//----------------------------------------------------------------------
#endif
