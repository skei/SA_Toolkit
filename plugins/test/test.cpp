
#include "sat.h"
#include "gui/sat_window.h"
#include "gui/sat_widgets.h"

//----------------------------------------------------------------------

//SAT_Window* window;

//----------------------------------------------------------------------

// BufferedWidget doesn't work properly.. :-/

int main() {
  SAT_Window* window = new SAT_Window(640,480);

  SAT_RootWidget* root = new SAT_RootWidget(window);
  window->setRootWidget(root);
    root->setDrawBorder(false);
    root->setFillBackground(false);
  //root->setBackgroundColor(SAT_DarkGreen2);

  SAT_BufferedWidget* buff = new SAT_BufferedWidget(SAT_Rect(10,10,300,200));
  root->appendChild(buff);
    buff->Layout.flags = SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
    //buff->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_ALL;
    buff->setFillBackground(true);
    buff->setBackgroundColor(SAT_DarkRed);

    SAT_TextWidget* text = new SAT_TextWidget(SAT_Rect(20,20,200,100),"Hello world!");
    buff->appendChild(text);
      text->setTextSize(30);

  SAT_SliderWidget* slider = new SAT_SliderWidget(SAT_Rect(50,20,100,20),"slider",0.5);
  root->appendChild(slider);

  window->show();
  window->eventLoop();
  window->hide();
  delete window;
  return 0;
}