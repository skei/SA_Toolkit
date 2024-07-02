#ifndef sat_pages_widget_included
#define sat_pages_widget_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/widget/sat_visual_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_PagesWidget
: public SAT_VisualWidget {

//------------------------------
protected:
//------------------------------

  uint32_t MActivePage = 0;

//------------------------------
public:
//------------------------------

  SAT_PagesWidget(SAT_Rect ARect)
  : SAT_VisualWidget(ARect) {
    setName("SAT_PagesWidget");
    setHint("SAT_PagesWidget");
    Options.realignInvisible = true;
  }

  //----------

  virtual ~SAT_PagesWidget() {
  }

//------------------------------
public:
//------------------------------

  uint32_t getActivePage() {
    return MActivePage;
  }

  //----------

  int32_t appendPage(SAT_Widget* APage) {
    //SAT_Print("index %i\n",getNumChildWidgets());
    //APage->State.active = false;
    APage->setActive(false);
    APage->setVisible(false);

    APage->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
    APage->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_ALL;
    APage->Options.realignInvisible = true;

    appendChild(APage);
    return APage->getIndex();
  }

  //----------

  void setPage(int32_t APage) {
    //SAT_Print("page %i\n",APage);
    int num = getNumChildren();//MChildren.size();
    if (APage >= num) APage = num - 1;
    for (int32_t i=0; i<num; i++) {
      SAT_Widget* wdg = getChild(i);//MChildren[i];
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
