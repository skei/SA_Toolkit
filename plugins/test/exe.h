
#include "sat.h"
#include "gui/sat_window.h"
#include "gui/sat_widgets.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

int main() {
  SAT_Window* window = new SAT_Window(640,480);
  SAT_RootWidget* root = new SAT_RootWidget(window);
  window->setRootWidget(root);
  window->setMouseCursorShape(SAT_CURSOR_FINGER);
  window->setTitle("Hello World!");
  window->show();
  window->eventLoop();
  window->hide();
  delete window;
  return 0;
}
