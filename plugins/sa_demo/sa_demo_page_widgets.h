#ifndef sa_demo_page_widgets_included
#define sa_demo_page_widgets_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "gui/sat_widgets.h"

class sa_demo_page_widgets
: public SAT_PanelWidget {
  
//------------------------------
public:
//------------------------------

  sa_demo_page_widgets(SAT_Rect ARect, SAT_Plugin* APlugin)
  : SAT_PanelWidget(ARect) {
    setName("sa_demo_page_widgets");
    setStretching(SAT_WIDGET_STRETCH_ALL);
    setInnerBorder(SAT_Rect(10,10,10,10));
    //setSpacing(SAT_Point(5,5));
    
    SAT_PanelWidget* left_panel = new SAT_PanelWidget(200);
    appendChildWidget(left_panel);
    left_panel->setLayout(SAT_WIDGET_ALIGN_LEFT_TOP,SAT_WIDGET_STRETCH_VERTICAL);
    left_panel->setWidthLimits(100,400);
    {
    }
    
    SAT_SizerWidget* sizer = new SAT_SizerWidget(SAT_Rect(3),SAT_DIRECTION_LEFT,left_panel);
    appendChildWidget(sizer);
    sizer->setFillBackground(true);
    sizer->setBackgroundColor(0.3);
    sizer->setDrawBorder(false);

    SAT_PanelWidget* right_panel = new SAT_PanelWidget(0);
    appendChildWidget(right_panel);
    right_panel->setLayout(SAT_WIDGET_ALIGN_LEFT_TOP,SAT_WIDGET_STRETCH_ALL);
    {
    }
    
  }
  
  //----------

  virtual ~sa_demo_page_widgets() {
  }

};

//----------------------------------------------------------------------
#endif