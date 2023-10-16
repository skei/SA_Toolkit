
// ./compile.sh -i build.cpp -o build.exe -f exe -g x11 -d
// nc -U -l -k /tmp/sat.socket

//----------

#define SAT_PRINT_SOCKET

#include "sat.h"
#include "gui/sat_window.h"
#include "gui/sat_renderer.h"
//#include "gui/sat_widget.h"

//----------------------------------------------------------------------

class myWindow
: public SAT_Window {

public:

  myWindow(uint32_t AWidth, uint32_t AHeight, intptr_t AParent=0)
  : SAT_Window(AWidth,AHeight,AParent) {
  }

public:

  void on_window_paint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) final {
    SAT_Renderer* renderer = getRenderer();
    SAT_Painter* painter = getPainter();
    renderer->beginRendering(0,0,AWidth,AHeight); // (0,0,640,480);
    painter->beginFrame(AWidth,AHeight,1);        // (640,480,1.0);

      const char* painter_type_name = painter->getTypeName();

      painter->setFillColor(SAT_Yellow);
      painter->fillCircle(200,200,150);

      // need to load a font first..
      //painter->setTextColor(SAT_BrightRed);
      //painter->setTextSize(32);
      //painter->drawText(100,100,"Hello, %s!",painter_type_name);

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
