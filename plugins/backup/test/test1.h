
//#define SAT_USE_GLES

#include "base/sat.h"
#include "gui/sat_widgets.h"
#include "gui/sat_window.h"

//----------

int main() {
  SAT_Window* window = new SAT_Window(200,100);
  SAT_PanelWidget* panel = new SAT_PanelWidget(0);
  window->appendRootWidget(panel);
  
//    SAT_MenuWidget* menu = new SAT_MenuWidget(SAT_Rect(100,200));
//    SAT_SelectorWidget* selector = new SAT_SelectorWidget(SAT_Rect(10,10,500,20),"Select",menu);
//    panel->appendChildWidget(selector);
//    panel->appendChildWidget(menu);

    SAT_MainMenuWidget* mainmenu = new SAT_MainMenuWidget(SAT_Rect(10,10,500,20));
    panel->appendChildWidget(mainmenu);
    mainmenu->appendMenu("File");
    mainmenu->appendSubMenu(0,"New");
    mainmenu->appendSubMenu(0,"Open");
    mainmenu->appendSubMenu(0,"Save");
    mainmenu->appendMenu("Edit");
    mainmenu->appendSubMenu(1,"Cut");
    mainmenu->appendSubMenu(1,"Copy");
    mainmenu->appendSubMenu(1,"Paste");
    mainmenu->appendMenu("Help");
    mainmenu->appendSubMenu(2,"About");
  
  window->show();
  window->eventLoop();
  window->hide();
  delete window;
  return 0;
}