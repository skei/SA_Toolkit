
#include "gui/sat_window.h"
#include "gui/sat_widgets.h"

//#include "base/sat_print.h"
//#include "base/debug/sat_debug_call_stack.h"
//#include "base/debug/sat_debug_crash_handler.h"
//#include "base/debug/sat_debug_memtrace.h"
//#include "base/debug/sat_debug_observer.h"
//#include "base/debug/sat_debug_print.h"
//#include "base/debug/sat_debug_window.h"

//----------------------------------------------------------------------


//----------------------------------------------------------------------

int main() {
  SAT_Window* window = new SAT_Window(640,480,0,nullptr);
  
  SAT_PanelWidget* panel = new SAT_PanelWidget(0);
  window->appendRootWidget(panel);
  
  SAT_SliderWidget* slider = new SAT_SliderWidget( SAT_Rect(10,10,300,30), "Slider", 0.25 );
  panel->appendChildWidget(slider);
  slider->setTextSize(16);
  slider->setValueSize(16);
  
  window->show();
  window->eventLoop();
  window->hide();
  delete window;
  return 0;
}

  