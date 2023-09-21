#ifndef sat_main_menu_widget_included
#define sat_main_menu_widget_included
//----------------------------------------------------------------------

#include "base/sat.h"
//#include "gui/widgets/sat_popup_widget.h"
#include "gui/widgets/sat_menu_widget.h"
#include "gui/widgets/sat_menu_item_widget.h"
#include "gui/widgets/sat_panel_widget.h"
#include "gui/widgets/sat_selector_widget.h"
//#include "gui/widgets/sat_value_widget.h"

//----------

#define SAT_WIDGET_MAIN_MENU_MAX_MENUS    32
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

//------------------------------
public:
//------------------------------

  SAT_MainMenuWidget(SAT_Rect ARect)
  : SAT_PanelWidget(ARect) {
    setName("SAT_MainMenuWidget");
    setDrawBorder(false);
    setFillBackground(true);
    setBackgroundColor(0.35);
  }

  //----------

  virtual ~SAT_MainMenuWidget() {
  }
  
//------------------------------
public:
//------------------------------

  virtual uint32_t appendMenu(const char* AName) {
    uint32_t index = 0;
    return index;
  }

  //----------
  
  virtual uint32_t appendSubMenu(uint32_t AIndex, const char* AName) {
    uint32_t index = 0;
    return index;
  }

  //----------
  
  /*
    called from om_widget_open()
    just before realignChildWidgets
  */

  void prepare(SAT_WidgetOwner* AOwner) override {
    SAT_PanelWidget::prepare(AOwner);
//    SAT_Window* window = (SAT_Window*)AOwner;
//    SAT_Assert(window);
//    SAT_Painter* painter = window->getPainter();
//    SAT_Assert(painter);
//    double x = 0.0;
//    double y = 0.0;
//    double w = 0.0;
//    double h = getHeight();
//    uint32_t num_menus = MMenus.size();
//    for (uint32_t i=0; i<num_menus; i++) {
//      const char* text = MMenus[i];
//      SAT_Print("text %s\n",text);
//      double bounds[4];
//      if (painter->getTextBounds(text,bounds)) {
//        double width = bounds[2] - bounds[0];
//        SAT_Print("width %f\n",width);
//        uint32_t num_submenus = MSubMenus[i].size();
//        SAT_Print("num_submenus %i\n",num_submenus);
//        SAT_MenuWidget* menu = new SAT_MenuWidget(SAT_Rect(x,y+h,w,h));
//        w += width;
//        SAT_SelectorWidget* selector = new SAT_SelectorWidget(SAT_Rect(x,y,w,h),text,menu);
//        appendChildWidget(selector);
//        //if (width > 0) width /= S;
//        //...
//      }
//      else {
//        // error..
//      }
//    }
  }
  
};

//----------------------------------------------------------------------
#endif


