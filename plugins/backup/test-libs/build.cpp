
#include "sat_wayland_window.h"

int main() {
  SAT_WaylandWindow* window = new SAT_WaylandWindow(640,480);
  window->show();
  window->eventLoop();
  window->hide();
  delete window;
  return 0;
}

