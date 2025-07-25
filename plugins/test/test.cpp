
#include "sat.h"
#include "gui/sat_window.h"
#include "gui/sat_widgets.h"

//----------------------------------------------------------------------

//SAT_Window* window;

//----------------------------------------------------------------------

int main() {
  SAT_Window* window = new SAT_Window(640,480);

  SAT_RootWidget* root = new SAT_RootWidget(window);
  window->setRootWidget(root);
 
  root->setDrawBorder(false);
  root->setFillBackground(false);

  SAT_CachedWidget* cached = new SAT_CachedWidget(SAT_Rect());
  root->appendChild(cached);

    SAT_TextWidget* text = new SAT_TextWidget(SAT_Rect(10,30,200,100),"Hello world!");
    text->setTextSize(30);
    cached->appendChild(text);

  SAT_SliderWidget* slider = new SAT_SliderWidget(SAT_Rect(50,20,100,20),"slider",0.5);
  root->appendChild(slider);

  window->show();
  window->eventLoop();
  window->hide();
  delete window;
  return 0;
}