#ifndef sat_main_menu_widget_included
#define sat_main_menu_widget_included
//----------------------------------------------------------------------

#include "base/sat.h"
//#include "gui/widgets/sat_popup_widget.h"
#include "gui/widgets/sat_menu_widget.h"
#include "gui/widgets/sat_menu_item_widget.h"
#include "gui/widgets/sat_panel_widget.h"
//#include "gui/widgets/sat_value_widget.h"

//----------

#define SAT_WIDGET_MAIN_MENU_MAX_SUBMENUS 32

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_MainMenuWidget
: public SAT_PanelWidget
//, public SAT_ValueWidget
, public SAT_MenuListener {


//------------------------------
private:
//------------------------------

  SAT_MenuWidget* MMenus[SAT_WIDGET_MAIN_MENU_MAX_SUBMENUS]   = {0};

//------------------------------
public:
//------------------------------

  SAT_MainMenuWidget(SAT_Rect ARect)
  : SAT_PanelWidget(ARect) {
    setName("SAT_MainMenuWidget");
  }

  //----------

  virtual ~SAT_MainMenuWidget() {
  }
  
//------------------------------
public:
//------------------------------

//  virtual void prepare(SAT_WidgetOwner* AOwner) override {
//    SAT_PanelWidget::prepare(AOwner);
//    SAT_Window* window = (SAT_Window*)AOwner;
//    SAT_Painter* painter = window->getPainter();
//    for (uint32_t i=0; i<getNumChildWidgets(); i++) {
//      SAT_MenuItemWidget* child = (SAT_MenuItemWidget*)getChildWidget(i);
//      const char* text = child->getText();
//      double bounds[4];
//      if (painter->getTextBounds(text,bounds)) {
//        double width = bounds[2] - bounds[0];
//        //if (width > 0) width /= S;
//        //...
//      }
//    }
//  }
  
};

//----------------------------------------------------------------------
#endif


