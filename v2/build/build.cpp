
// /DISKS/sda2/Code/SAT2/build/compile.sh -i build.cpp -o build.exe -f exe
// nc -U -l -k /tmp/sat.socket

//----------

#include "sat.h"
#include "gui/sat_window.h"
#include "gui/sat_renderer.h"

//----------------------------------------------------------------------

bool main_test1() { return true; }
bool main_test2() { return true; }

SAT_AddTest("MAIN: running test1",main_test1);
SAT_AddTest("MAIN: another test2",main_test2);

//----------------------------------------------------------------------

int main() {

  printf("hello, world!\n");

  if (!SAT_GLOBAL.TEST.runAllTests()) return -666;

  SAT_Renderer* renderer = new SAT_Renderer();
  renderer->initialize(nullptr);

  SAT_Window* window = new SAT_Window();
  window->open();
  window->eventLoop();
  window->close();
  delete window;

  delete renderer;

  printf("goodbye, world!\n");
  return 0;
}
