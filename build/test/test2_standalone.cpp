
#include "base/sat.h"
#include "gui/sat_window.h"
//#include "gui/x11/sat_x11_window.h"

int main() {
  //int* ptr = nullptr;
  //int a = *ptr;
  SAT_Print("Hello world!\n");
  
  //SAT_X11Window* window = new SAT_X11Window(640,480,0);
  SAT_Window* window = new SAT_Window(640,480,0,nullptr);
  window->show();
  window->eventLoop();
  window->hide();
  delete window;
  return 0;
}