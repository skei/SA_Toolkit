#ifndef sat_pages_widget_included
#define sat_pages_widget_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "gui/sat_widget.h"
#include "gui/sat_paint_context.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_PagesWidget
: public SAT_Widget {

//------------------------------
protected:
//------------------------------

  uint32_t MActivePage = 0;

//------------------------------
public:
//------------------------------

  SAT_PagesWidget(SAT_Rect ARect)
  : SAT_Widget(ARect) {
  }

  //----------

  virtual ~SAT_PagesWidget() {
  }

//------------------------------
public:
//------------------------------

  int32_t appendPage(SAT_Widget* APage) {
    //APage->State.active = false;
    APage->setActive(false);
    APage->setVisible(false);
    return appendChildWidget(APage)->getIndex();
  }

  //----------

  void setPage(int32_t APage) {
    int num = getNumChildWidgets();//MChildren.size();
    if (APage >= num) APage = num - 1;
    for (int32_t i=0; i<num; i++) {
      SAT_Widget* wdg = getChildWidget(i);//MChildren[i];
      if (i == APage) {
        wdg->setActive(true);
        wdg->setVisible(true);
      }
      else {
        wdg->setActive(false);
        wdg->setVisible(false);
      }
    }
    MActivePage = APage;
  }

};

//----------------------------------------------------------------------
#endif

