/*

  in '/bin' directory:
    ../build/compile -i ../build/build.cpp -o build.exe -f exe -g x11 -d  

  in separate console:
    nc -U -l -k /tmp/sat.socket

*/
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/sat_window.h"
#include "gui/sat_widgets.h"

//----------

#define WINDOW_WIDTH  500
#define WINDOW_HEIGHT 500

//----------------------------------------------------------------------
//
// main
//
//----------------------------------------------------------------------

int main() {
  SAT_Window* window = new SAT_Window(WINDOW_WIDTH,WINDOW_HEIGHT);
  SAT_RootWidget* root = new SAT_RootWidget(0,window);
  window->setRootWidget(root);

    SAT_PanelWidget* panel1 = new SAT_PanelWidget(SAT_Rect(10,10,100,100));
    root->appendChildWidget(panel1);

  window->open();
  window->eventLoop();
  window->close();
  //delete root;
  delete window;
  return 0;
}

