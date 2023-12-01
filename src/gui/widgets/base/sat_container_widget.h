#ifndef sat_container_widget_included
#define sat_container_widget_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/base/sat_visual_widget.h"

//----------------------------------------------------------------------

class SAT_ContainerWidget
: public class SAT_VisualWidget {

//------------------------------
public:
//------------------------------

  // children

//------------------------------
public:
//------------------------------

  SAT_ContainerWidget(SAT_Rect ARect)
  : SAT_VisualWidget(ARect) {
  }

  virtual ~SAT_ContainerWidget() {
  }

//------------------------------
public:
//------------------------------

  // find widget
  // layout

};

//----------------------------------------------------------------------
#endif
