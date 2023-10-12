
// ./compile.sh -i build.cpp -o build.exe -f exe -g x11 -d
// nc -U -l -k /tmp/sat.socket

//----------

#include "sat.h"
#include "gui/sat_window.h"

//----------------------------------------------------------------------

class myWindow
: public SAT_Window {

public:

  myWindow(uint32_t AWidth, uint32_t AHeight, intptr_t AParent=0)
  : SAT_Window(AWidth,AHeight,AParent) {
  }

  //----------

  virtual ~myWindow() {
  }

public:

  void on_window_paint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) final {
    printf("myWindow.paint\n");

    SAT_Renderer* renderer = getRenderer();
    SAT_Painter* painter = getPainter();

    printf("renderer: %p\n",renderer);
    printf("painter: %p\n",painter);

    renderer->beginRendering(0,0,100,100);
    painter->beginFrame(100,100,1.0);
    painter->setDrawColor(SAT_White);
    painter->drawRect(10,10,80,80);
    painter->endFrame();
    renderer->endRendering();

  }

};

//----------------------------------------------------------------------

int main() {

  myWindow* window = new myWindow(640,480);
  window->open();

  window->eventLoop();
  window->close();
  delete window;

  return 0;
}
