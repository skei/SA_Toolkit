
#include "base/sat.h"
#include "gui/sat_widgets.h"
#include "gui/sat_window.h"

//----------

int main() {
  SAT_Window* window = new SAT_Window(640,480);

    SAT_PanelWidget* panel = new SAT_PanelWidget(0);
    window->appendRootWidget(panel);
    
    SAT_KnobWidget* knob = new SAT_KnobWidget(SAT_Rect(10,10,100,100),"Knob",0.3);
    panel->appendChildWidget(knob);
  
  window->show();
  window->eventLoop();
  window->hide();
  delete window;
  return 0;
}