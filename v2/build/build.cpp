
// ./compile.sh -i build.cpp -o build.exe -f exe -g x11 -d
// nc -U -l -k /tmp/sat.socket

//----------

#include "sat.h"
#include "gui/sat_window.h"

//----------------------------------------------------------------------

int main() {

  SAT_Window* window = new SAT_Window(640,480);
  window->open();

  window->eventLoop();
  window->close();
  delete window;

  return 0;
}
