#ifndef sat_layout_widget_included
#define sat_layout_widget_included
//----------------------------------------------------------------------

// aka SAT_ContainerWidget

#include "sat.h"
#include "gui/sat_widget.h"
//#include "plugin/sat_parameter.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_LayoutWidget
: public SAT_VisualWidget {

//------------------------------
protected:
//------------------------------

  SAT_Rect    MLayoutOffset = { 0,0,0,0 };  // offset from 'pure' layout (scrolled, moved, undo before realigning))

//------------------------------
public:
//------------------------------

  SAT_LayoutWidget(SAT_Rect ARect)
  : SAT_VisualWidget(ARect) {
    setName("SAT_LayoutWidget");
  }

  virtual ~SAT_LayoutWidget() {
  }

//------------------------------
public:
//------------------------------

  //virtual bool do_widget_realign()

};

//----------------------------------------------------------------------
#endif
