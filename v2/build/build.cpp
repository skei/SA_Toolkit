
// ./compile.sh -i build.cpp -o build.exe -f exe -g x11 -d
// nc -U -l -k /tmp/sat.socket

//----------

//#define SAT_NO_TESTS
//#define SAT_PRINT_SOCKET

#include "sat.h"
#include "gui/sat_window.h"
#include "gui/sat_widget.h"

//----------------------------------------------------------------------

class myWidget
: public SAT_Widget {
public:
  myWidget(SAT_Rect ARect) : SAT_Widget(ARect) {}
  void on_widget_paint(SAT_PaintContext* AContext) final {
    SAT_PRINT;
    SAT_BasePainter* painter = AContext->painter;
    painter->setFillColor(SAT_Yellow);
    painter->fillCircle(200,200,150);
  }

};

//----------------------------------------------------------------------

// class myWindow
// : public SAT_Window {
// public:
//   myWindow(uint32_t AWidth, uint32_t AHeight, intptr_t AParent=0)
//   : SAT_Window(AWidth,AHeight,AParent) {
//   }
// public:
// };

//----------------------------------------------------------------------

bool build_test1() {
  SAT_Print("Hello world!\n");
  return true;
}

SAT_TEST("BUILD: test1",build_test1)

//----------------------------------------------------------------------

int main() {
  SAT_Print("Hello world!\n");

  SAT_RUN_TESTS

  SAT_Window* window = new SAT_Window(640,480);

  myWidget* root = new myWidget(100);
  window->setRootWidget(root);

  window->open();
  window->eventLoop();
  window->close();

  delete root;
  delete window;

  return 0;
}

void SAT_Register(SAT_Registry* ARegistry) {
  SAT_Print("Hello world!\n");
}

