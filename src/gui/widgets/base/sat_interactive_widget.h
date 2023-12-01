#ifndef sat_interactive_widget_included
#define sat_interactive_widget_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/base/sat_container_widget.h"

//----------------------------------------------------------------------

class SAT_InteractiveWidget
: public SAT_ContainerWidget {

//------------------------------
public:
//------------------------------

  bool MClickable = false;
  bool MDraggable = false;
  bool MMovable   = false;
  bool MSizable   = false;

//------------------------------
public:
//------------------------------

  SAT_InteractiveWidget(SAT_Rect ARect)
  : SAT_ContainerWidget(ARect) {
  }

  virtual ~SAT_InteractiveWidget() {
  }

};

//----------------------------------------------------------------------
#endif
