#ifndef sa_demo_page_blank_included
#define sa_demo_page_blank_included
//----------------------------------------------------------------------

#include "sat.h"

#include "gui/widget/sat_knob_widget.h"
#include "gui/widget/sat_visual_widget.h"

#include "gui/widget/sat_graph2_widget.h"
#include "gui/widget/sat_graph2_module_widget.h"

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

  sa_demo_page_blank(SAT_Plugin* APlugin, SAT_Editor* AEditor)
  : SAT_VisualWidget(0) {
    setName("sa_demo_page_blank");
    setHint("sa_demo_page_blank");
    //Options.autoClip = true;
    Layout.flags = SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
    Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_ALL;
    setDrawBorder(false);
    setFillBackground(false);
    //setBackgroundColor(SAT_Green);

    SAT_Graph2Widget* graph2 = new SAT_Graph2Widget(0);
    appendChild(graph2);
    graph2->Layout.flags  = SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
    graph2->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_ALL;
    
    SAT_Graph2ModuleWidget* module1 = new SAT_Graph2ModuleWidget(SAT_Rect(50,50,100,75));
    graph2->appendChild(module1);
      SAT_KnobWidget* knob1 = new SAT_KnobWidget(SAT_Rect(5,5,30,30),"%",0.3);
      module1->appendChild(knob1);
      knob1->setValueTextSize(5);
      knob1->setKnobArcThickness(3);
      SAT_KnobWidget* knob1b = new SAT_KnobWidget(SAT_Rect(40,5,30,30),"%",0.3);
      module1->appendChild(knob1b);
      knob1b->setValueTextSize(5);
      knob1b->setKnobArcThickness(3);
      knob1b->setKnobArcColor(SAT_Red);

    SAT_Graph2ModuleWidget* module2 = new SAT_Graph2ModuleWidget(SAT_Rect(50,150,100,75),true);
    graph2->appendChild(module2);
      SAT_KnobWidget* knob2 = new SAT_KnobWidget(SAT_Rect(5,5,30,30),"%",0.7);
      module2->appendChild(knob2);
      knob2->setValueTextSize(5);
      knob2->setKnobArcThickness(3);

  }

  //----------

  virtual ~sa_demo_page_blank() {
  }


};

//----------------------------------------------------------------------
#endif

