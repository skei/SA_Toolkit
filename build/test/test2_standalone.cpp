
#include "base/sat.h"
#include "gui/sat_window.h"
#include "gui/sat_widgets.h"

//----------------------------------------------------------------------

int main() {
  
  SAT_PRINT;
  SAT_Print("hello world!\n");
  SAT_DPrint("hello world!\n");

  SAT_Window* window = new SAT_Window(640,480,0,nullptr);
  
  SAT_PanelWidget* root = new SAT_PanelWidget(0);
  window->appendRootWidget(root);
  
    SAT_KnobWidget* knob = new SAT_KnobWidget(SAT_Rect(100,100,200,200),"Knob",0.5);
    root->appendChildWidget(knob);
    knob->setArcThickness(20);
    knob->setTextSize(20);
    knob->setValueSize(50);
  
  //root->redraw();
  window->show();
  window->eventLoop();
  window->hide();
  delete window;

  return 0;
}


