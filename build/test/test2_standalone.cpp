
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
  
//  root->redraw(); // ???
  
  window->show();
  window->eventLoop();
  window->hide();
  delete window;
  return 0;
}


