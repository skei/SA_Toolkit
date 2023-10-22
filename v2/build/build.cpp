
// ./compile.sh -i build.cpp -o build.exe -f exe -g x11 -d
// nc -U -l -k /tmp/sat.socket

//----------

//#define SAT_NO_TESTS
//#define SAT_DEBUG_PRINT_SOCKET

#include "sat.h"
#include "gui/sat_window.h"
//#include "gui/sat_widget.h"

#define WINDOW_WIDTH  500
#define WINDOW_HEIGHT 500

//----------------------------------------------------------------------

class myRootWidget
: public SAT_RootWidget {
private:
  SAT_Color color1 = SAT_Yellow;
  SAT_Color color2 = SAT_Red;
public:
  myRootWidget(SAT_Rect ARect,SAT_WidgetListener* AListener) : SAT_RootWidget(ARect,AListener) {}
  void on_widget_paint(SAT_PaintContext* AContext) final {
    SAT_Print("***\n");
    SAT_BasePainter* painter = AContext->painter;
    SAT_Rect mrect = getRect();
    painter->setFillColor(color1);
    painter->fillRect(mrect.x,mrect.y,mrect.w,mrect.h);
    painter->setFillColor(color2);
    double x = mrect.x + (mrect.w * 0.5); // 200
    double y = mrect.y + (mrect.h * 0.5); // 200
    double rx = mrect.w * 0.4;
    double ry = mrect.h * 0.4; // 150
    painter->fillEllipse(x,y,rx,ry);
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

  SAT_Window* window = new SAT_Window(WINDOW_WIDTH,WINDOW_HEIGHT);

  // note, this is not automatically deleted! (set, not append)
  myRootWidget* root = new myRootWidget(0,window);
  window->setRootWidget(root);

  window->open();
  window->eventLoop();
  window->close();

  delete root;
  delete window;

  return 0;
}

//----------------------------------------------------------------------

void SAT_Register(SAT_Registry* ARegistry) {
  SAT_Print("Hello world!\n");
}
