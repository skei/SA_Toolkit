
#include "sat.h"
#ifndef SAT_NO_GUI
#include "gui/sat_window.h"
#endif
//#include "gui/sat_widgets.h"

#define SAT_PLUGIN_EXE_NO_MAIN
#include "plugin/sat_entry.h"

//----------------------------------------------------------------------

int main() {

#ifndef SAT_NO_GUI
  SAT_Window* window = new SAT_Window(640,480);
  // SAT_RootWidget* root = new SAT_RootWidget(window,SAT_Rect(0,0,100,100));
  // window->setRootWidget(root);
  //   SAT_VisualWidget* wdg1 = new SAT_VisualWidget(SAT_Rect(10,10,100,50));
  //   root->appendChild(wdg1);
  //   SAT_TextWidget* text1 = new SAT_TextWidget(SAT_Rect(120,10,100,50));
  //   root->appendChild(text1);
  //   SAT_ValueWidget* value1 = new SAT_ValueWidget(SAT_Rect(230,10,100,50));
  //   root->appendChild(value1);
  //   SAT_ButtonWidget* button1 = new SAT_ButtonWidget(SAT_Rect(10,70,100,50));
  //   root->appendChild(button1);
  //   //button1->setBackgroundColor(SAT_DarkGrey);
  //   SAT_DragValueWidget* drag1 = new SAT_DragValueWidget(SAT_Rect(120,70,100,50));
  //   root->appendChild(drag1);
  //   // SAT_SliderWidget* slider1 = new SAT_SliderWidget(SAT_Rect(120,70,100,50));
  //   // root->appendChild(slider1);
  // window->show();
  // window->eventLoop();
  // window->hide();
  delete window;
#endif

  return 0;
}
