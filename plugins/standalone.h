
#include "sat.h"
#include "gui/sat_window.h"
#include "gui/sat_widgets.h"

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

int main(void) {

  SAT_Window* window = new SAT_Window(800,600);

  SAT_RootWidget* root = new SAT_RootWidget();
  window->setRootWidget(root);
  window->setTitle("Hello world!");
  window->setInitialSize(200,150);

    SAT_KnobWidget* knob = new SAT_KnobWidget(SAT_Rect(20,20,100,100),"%",0.5);
    root->appendChildWidget(knob);
    knob->setTextSize(25);
    knob->setValueSize(30);

  window->open();
  window->eventLoop();
  window->close();
  
  delete window;
  return 0;
}

