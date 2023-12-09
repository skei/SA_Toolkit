
#include "base/sat.h"
#include "gui/sat_window.h"
#include "gui/sat_widgets.h"

//----------------------------------------------------------------------

int main() {
  SAT_Window* window = new SAT_Window(400,440,0,nullptr);
  window->setInitialSize(400,440);
  SAT_PanelWidget* root = new SAT_PanelWidget(0);
  window->appendRootWidget(root);
  SAT_PluginHeaderWidget* header = new SAT_PluginHeaderWidget(SAT_Rect(0,0,400,40),"test2_standalone","EXE");
  root->appendChildWidget(header);
  SAT_KnobWidget* knob = new SAT_KnobWidget(SAT_Rect(100,140,200,200),"Knob",0.5);
  root->appendChildWidget(knob);
  knob->setArcThickness(20);
  knob->setTextSize(20);
  knob->setValueSize(50);
  window->show();
  window->eventLoop();
  window->hide();
  delete window;
  return 0;
}
