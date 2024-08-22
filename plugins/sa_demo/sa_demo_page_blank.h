#ifndef sa_demo_page_blank_included
#define sa_demo_page_blank_included
//----------------------------------------------------------------------

#include "sat.h"

#include "gui/widget/sat_knob_widget.h"
#include "gui/widget/sat_visual_widget.h"

#include "gui/widget/sat_node_widget.h"
#include "gui/widget/sat_node_editor_widget.h"

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

    SAT_NodeEditorWidget* node_editor = new SAT_NodeEditorWidget(0);
    appendChild(node_editor);
    node_editor->Layout.flags  = SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
    node_editor->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_ALL;
    
    SAT_NodeWidget* module1 = new SAT_NodeWidget(SAT_Rect(50,50,100,75),"Node 1",false);
    node_editor->appendChild(module1);
      SAT_KnobWidget* knob1 = new SAT_KnobWidget(SAT_Rect(5,5,30,30),"..%",0.3);
      module1->appendChild(knob1);
      knob1->setValueTextSize(6);
      knob1->setKnobArcThickness(4);
      SAT_KnobWidget* knob1b = new SAT_KnobWidget(SAT_Rect(40,5,30,30),"..",0.3);
      module1->appendChild(knob1b);
      knob1b->setValueTextSize(6);
      knob1b->setKnobArcThickness(4);
      knob1b->setKnobArcColor(SAT_Red);

      // name, signal (else data) , input (else output)
      module1->appendPin("data input",    false, true);
      module1->appendPin("data input",    false, true);
      module1->appendPin("signal input",  true,  true);
      module1->appendPin("data output",   false, false);
      module1->appendPin("signal output", true,  false);
      module1->appendPin("signal output", true,  false);

    SAT_NodeWidget* module2 = new SAT_NodeWidget(SAT_Rect(50,150,100,75),"Node 2",true);
    node_editor->appendChild(module2);
      SAT_KnobWidget* knob2 = new SAT_KnobWidget(SAT_Rect(5,5,30,30),"..",0.7);
      module2->appendChild(knob2);
      knob2->setValueTextSize(6);
      knob2->setKnobArcThickness(4);

      // name, signal (else data) , input (else output)
      module2->appendPin("data input", false, true);
      module2->appendPin("data input", false, true);
      module2->appendPin("data input", false, true);
      module2->appendPin("signal input", true,  true);
      module2->appendPin("signal input", true,  true);
      module2->appendPin("data output", false, false);
      module2->appendPin("data output", false, false);
      module2->appendPin("signal output", true,  false);
      module2->appendPin("signal output", true,  false);

  }

  //----------

  virtual ~sa_demo_page_blank() {
  }


};

//----------------------------------------------------------------------
#endif

