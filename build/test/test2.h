
#include "gui/sat_window.h"
#include "gui/sat_widgets.h"

#include "gui/wayland/sat_wayland.h"

//----------------------------------------------------------------------

/*
int test_window() {
  SAT_Window* window = new SAT_Window(320,100,0,nullptr);
  SAT_PanelWidget* panel = new SAT_PanelWidget(0);
  window->appendRootWidget(panel);
    SAT_SliderWidget* slider = new SAT_SliderWidget( SAT_Rect(10,10,300,25), "Slider", 0.25 );
    panel->appendChildWidget(slider);
    slider->setNumValues(2);
    slider->setValue(0.75,1);
  window->resize(960,300);
  window->show();
  window->eventLoop();
  window->hide();
  delete window;
  return 0;
}
*/

//----------------------------------------------------------------------

int test_wayland() {
  SAT_WaylandWindow wayland;
  wayland.setup();
  //
  wayland.cleanup();
  return 0;
}

//----------------------------------------------------------------------

int main() {
  //return test_window();
  return test_wayland();
}

  