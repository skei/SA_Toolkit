
#include "sat.h"
#include "gui/sat_window.h"
//#include "gui/sat_widgets.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

int main() {
  SAT_Window* window = new SAT_Window(640,480);

    //SAT_RootWidget* root = new SAT_RootWidget(window,SAT_Rect(0,0,100,100));
    //window->setRootWidget(root);

  window->show();
  window->setMouseCursor(SAT_CURSOR_FINGER);
  window->setTitle("Hello World!");
  window->eventLoop();
  window->hide();
  delete window;
  return 0;
}
