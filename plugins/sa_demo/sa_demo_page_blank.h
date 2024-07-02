#ifndef sa_demo_page_blank_included
#define sa_demo_page_blank_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/widget/sat_visual_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_demo_page_blank
: public SAT_VisualWidget {

//------------------------------
public:
//------------------------------

  sa_demo_page_blank()
  : SAT_VisualWidget(0) {
    setName("sa_demo_page_blank");
    setHint("sa_demo_page_blank");
    //Options.autoClip = true;
    Layout.flags = SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
    Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_ALL;
    setDrawBorder(false);
    setFillBackground(false);
    //setBackgroundColor(SAT_Green);
  }

  //----------

  virtual ~sa_demo_page_blank() {
  }


};

//----------------------------------------------------------------------
#endif

