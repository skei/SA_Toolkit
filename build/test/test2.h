
#include "gui/sat_window.h"
#include "gui/sat_widgets.h"

//----------------------------------------------------------------------


//----------------------------------------------------------------------

int main() {
  SAT_Window* window = new SAT_Window(640,480,0,nullptr);
  
  SAT_PanelWidget* panel = new SAT_PanelWidget(0);
  window->appendRootWidget(panel);
  
  SAT_SliderWidget* slider = new SAT_SliderWidget( SAT_Rect(10,10, 200,20), "Slider", 0.25 );
  panel->appendChildWidget(slider);
  //slider->setAutoHideCursor(false);
  
  window->show();
  window->eventLoop();
  window->hide();
  delete window;
  return 0;
}

  