#ifndef sat_tree_widget_included
#define sat_tree_widget_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/widget/sat_tree_item_widget.h"
#include "gui/widget/sat_visual_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_TreeWidget
: public SAT_VisualWidget {

//------------------------------
private:
//------------------------------

  bool MClosed = true;

//------------------------------
public:
//------------------------------

  SAT_TreeWidget(SAT_Rect ARect)
  : SAT_VisualWidget(ARect) {
    setName("SAT_TreeWidget");
    setHint("SAT_TreeWidget");
  }

  //----------

  virtual ~SAT_TreeWidget() {
  }

//------------------------------
public:
//------------------------------



};

//----------------------------------------------------------------------
#endif
