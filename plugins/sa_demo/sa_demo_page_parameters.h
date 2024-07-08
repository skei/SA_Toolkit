#ifndef sa_demo_page_parameters_included
#define sa_demo_page_parameters_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/sat_widgets.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_demo_page_parameters
: public SAT_VisualWidget {

//------------------------------
public:
//------------------------------

  sa_demo_page_parameters(SAT_Plugin* APlugin, SAT_Editor* AEditor)
  : SAT_VisualWidget(0) {
    setName("sa_demo_page_parameters");
    setHint("sa_demo_page_parameters");
    //Options.autoClip = true;
    //Options.realignInvisible = true;
    Layout.flags = SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
    Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_ALL;
    setDrawBorder(false);
    setFillBackground(false);
    //setBackgroundColor(SAT_Green);

    SAT_KnobWidget* knob1 = new SAT_KnobWidget(SAT_Rect( 10, 10, 90,90),"p1",0.1);
    SAT_KnobWidget* knob2 = new SAT_KnobWidget(SAT_Rect(110, 10, 90,90),"p2",0.2);
    SAT_KnobWidget* knob3 = new SAT_KnobWidget(SAT_Rect(210, 10, 90,90),"p3",0.3);
    SAT_KnobWidget* knob4 = new SAT_KnobWidget(SAT_Rect(310, 10, 90,90),"p4",0.4);
    SAT_KnobWidget* knob5 = new SAT_KnobWidget(SAT_Rect( 10,110, 90,90),"p5",0.5);
    SAT_KnobWidget* knob6 = new SAT_KnobWidget(SAT_Rect(110,110, 90,90),"p6",0.6);
    SAT_KnobWidget* knob7 = new SAT_KnobWidget(SAT_Rect(210,110, 90,90),"p7",0.7);
    SAT_KnobWidget* knob8 = new SAT_KnobWidget(SAT_Rect(310,110, 90,90),"p8",0.8);
    
    appendChild(knob1);
    appendChild(knob2);
    appendChild(knob3);
    appendChild(knob4);
    appendChild(knob5);
    appendChild(knob6);
    appendChild(knob7);
    appendChild(knob8);

    AEditor->connect(knob1,APlugin->getParameter(SA_DEMO_PARAM_P1));
    AEditor->connect(knob2,APlugin->getParameter(SA_DEMO_PARAM_P2));
    AEditor->connect(knob3,APlugin->getParameter(SA_DEMO_PARAM_P3));
    AEditor->connect(knob4,APlugin->getParameter(SA_DEMO_PARAM_P4));
    AEditor->connect(knob5,APlugin->getParameter(SA_DEMO_PARAM_P5));
    AEditor->connect(knob6,APlugin->getParameter(SA_DEMO_PARAM_P6));
    AEditor->connect(knob7,APlugin->getParameter(SA_DEMO_PARAM_P7));
    AEditor->connect(knob8,APlugin->getParameter(SA_DEMO_PARAM_P8));

    knob1->setKnobArcThickness(15);

  }

  //----------

  virtual ~sa_demo_page_parameters() {
  }

};

//----------------------------------------------------------------------
#endif

