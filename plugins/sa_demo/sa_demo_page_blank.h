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

    //
    
    SAT_NodeWidget* node1 = new SAT_NodeWidget(SAT_Rect(50,50,100,75),"Node 1",false);
    node_editor->appendChild(node1);
      SAT_KnobWidget* knob1 = new SAT_KnobWidget(SAT_Rect(5,5,30,30),"..",0.3);
      node1->appendChild(knob1);
      knob1->setValueTextSize(6);
      knob1->setKnobArcThickness(4);
      SAT_KnobWidget* knob1b = new SAT_KnobWidget(SAT_Rect(40,5,30,30),"::",0.3);
      node1->appendChild(knob1b);
      knob1b->setValueTextSize(6);
      knob1b->setKnobArcThickness(4);
      knob1b->setKnobArcColor(SAT_Red);
      // name, signal (else data) , input (else output)
      node1->appendPin("data input",    false, true);
      node1->appendPin("data input",    false, true);
      node1->appendPin("signal input",  true,  true);
      node1->appendPin("data output",   false, false);
      node1->appendPin("signal output", true,  false);
      node1->appendPin("signal output", true,  false);

    SAT_NodeWidget* node2 = new SAT_NodeWidget(SAT_Rect(50,150,100,75),"Node 2",true);
    node_editor->appendChild(node2);
      SAT_KnobWidget* knob2 = new SAT_KnobWidget(SAT_Rect(5,5,30,30),"-",0.7);
      node2->appendChild(knob2);
      knob2->setValueTextSize(6);
      knob2->setKnobArcThickness(4);
      // name, signal (else data) , input (else output)
      node2->appendPin("data input",    false, true);
      node2->appendPin("data input",    false, true);
      node2->appendPin("data input",    false, true);
      node2->appendPin("signal input",  true,  true);
      node2->appendPin("signal input",  true,  true);
      node2->appendPin("data output",   false, false);
      node2->appendPin("data output",   false, false);
      node2->appendPin("signal output", true,  false);
      node2->appendPin("signal output", true,  false);

    // node_editor->appendWire(node1,0,node2,0);
    // node_editor->appendWire(node1,2,node2,4);

    SAT_NodeWidget* node3 = new SAT_NodeWidget(SAT_Rect(200,50,100,75),"Node 3",false);
    node_editor->appendChild(node3);
      SAT_KnobWidget* knob3 = new SAT_KnobWidget(SAT_Rect(5,5,30,30),"..",0.3);
      node3->appendChild(knob3);
      knob3->setValueTextSize(6);
      knob3->setKnobArcThickness(4);
      SAT_KnobWidget* knob3b = new SAT_KnobWidget(SAT_Rect(40,5,30,30),"::",0.3);
      node3->appendChild(knob3b);
      knob3b->setValueTextSize(6);
      knob3b->setKnobArcThickness(4);
      knob3b->setKnobArcColor(SAT_Red);
      // name, signal (else data) , input (else output)
      node3->appendPin("data input",    false, true);
      node3->appendPin("data input",    false, true);
      node3->appendPin("signal input",  true,  true);
      node3->appendPin("data output",   false, false);
      node3->appendPin("signal output", true,  false);
      node3->appendPin("signal output", true,  false);

    SAT_NodeWidget* node4 = new SAT_NodeWidget(SAT_Rect(200,150,100,75),"Node 4",true);
    node_editor->appendChild(node4);
      SAT_KnobWidget* knob4 = new SAT_KnobWidget(SAT_Rect(5,5,30,30),"-",0.7);
      node4->appendChild(knob4);
      knob4->setValueTextSize(6);
      knob4->setKnobArcThickness(4);
      // name, signal (else data) , input (else output)
      node4->appendPin("data input",    false, true);
      node4->appendPin("data input",    false, true);
      node4->appendPin("data input",    false, true);
      node4->appendPin("signal input",  true,  true);
      node4->appendPin("signal input",  true,  true);
      node4->appendPin("data output",   false, false);
      node4->appendPin("data output",   false, false);
      node4->appendPin("signal output", true,  false);
      node4->appendPin("signal output", true,  false);

    node_editor->appendWire(node1,0,node2,0);
//    node_editor->appendWire(node1,2,node2,1);

  }

  //----------

  virtual ~sa_demo_page_blank() {
  }


};

//----------------------------------------------------------------------
#endif

