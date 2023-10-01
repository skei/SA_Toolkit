
// #define SAT_LINUX_WAYLAND

#include "gui/sat_window.h"
#include "gui/sat_widgets.h"

//----------------------------------------------------------------------

int test2() {
  SAT_Window* window = new SAT_Window(1000,1000,0,nullptr);
  SAT_PanelWidget* panel = new SAT_PanelWidget(0);
  window->appendRootWidget(panel);
    SAT_SymbolWidget* symbol = new SAT_SymbolWidget( SAT_Rect(0,0,1000,1000), SAT_SYMBOL_SINE );
    panel->appendChildWidget(symbol);
    symbol->setPenWidth(5);
  //window->resize(960,300);
  window->show();
  window->eventLoop();
  window->hide();
  delete window;
  return 0;
}

//----------------------------------------------------------------------

int main() {
  return test2();
}

  