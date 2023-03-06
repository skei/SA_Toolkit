
#include "base/sat.h"
#include "gui/sat_window.h"

int main() {
  SAT_Print("Hello world!\n");
  SAT_Window* window = new SAT_Window(640,480,0,nullptr);
  window->show();
  window->eventLoop();
  window->hide();
  delete window;
  return 0;
}