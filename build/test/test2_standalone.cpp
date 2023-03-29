
#include "base/sat.h"
#include "gui/sat_window.h"
#include "gui/sat_widgets.h"

//----------------------------------------------------------------------

int main() {
  
  SAT_PRINT;
  SAT_Print("hello world!\n");
  SAT_DPrint("hello world!\n");

  SAT_Window* window = new SAT_Window(400,400,0,nullptr);
  
  
  SAT_PanelWidget* root = new SAT_PanelWidget(0);
  //SAT_Print("root: %p\n",root);
  window->appendRootWidget(root);
  
    SAT_KnobWidget* knob = new SAT_KnobWidget(SAT_Rect(100,100,200,200),"Knob",0.5);
    //SAT_Print("knob: %p\n",knob);
    root->appendChildWidget(knob);
    knob->setArcThickness(20);
    knob->setTextSize(20);
    knob->setValueSize(50);
    //knob->setDrawBorder(true);
  
  // win32 automatically receive a wm_paint after opening?
  // linux doesn't?
  // investigate
  
  #ifdef SAT_LINUX
    root->redraw();
  #endif
  
  window->show();
  window->eventLoop();
  window->hide();
  delete window;

  return 0;
}


