
// /DISKS/sda2/Code/SAT2/build/compile.sh -i build.cpp -o build.exe -f exe
// nc -U -l -k /tmp/sat.socket

//----------

#include "sat.h"
#include "gui/sat_window.h"
#include "gui/glx/sat_glx_renderer.h"

//----------------------------------------------------------------------

int main() {

  SAT_Window* window = new SAT_Window(640,480);
  window->open();
  window->eventLoop();
  window->close();
  delete window;

  return 0;
}
