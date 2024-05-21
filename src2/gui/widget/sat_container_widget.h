#ifndef sat_container_widget_included
#define sat_container_widget_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/widget/sat_visual_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_ContainerWidget
: public SAT_VisualWidget {

//------------------------------
public:
//------------------------------

  SAT_ContainerWidget(SAT_Rect ARect)
  : SAT_VisualWidget(ARect) {
    setName("SAT_ContainerWidget");
  }

  //----------

  virtual ~SAT_ContainerWidget() {
  }

//------------------------------
public:
//------------------------------

  

  virtual void open() {
  }

  virtual void close() {
  }

};

//----------------------------------------------------------------------
#endif