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

//typedef SAT_Array<SAT_SelectorWidget*>  SAT_SelectorArray;

struct SAT_MenuItem;
typedef SAT_Array<SAT_MenuItem*> SAT_MenuItems;

//
  
struct SAT_MenuItem {
  SAT_Widget*   owner     = nullptr;
  const char*   text      = "";
  SAT_MenuItems subitems  = {};
  ~SAT_MenuItem() {
    #ifndef SAT_NO_AUTODELETE
      for (uint32_t i=0; i<subitems.size(); i++) {
        delete subitems[i];
      }
    #endif
  }
};

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

  SAT_MenuItems MMenuItems = {};

//------------------------------
public:
//------------------------------

  SAT_MainMenuWidget(SAT_Rect ARect)
  : SAT_PanelWidget(ARect) {
    setName("SAT_MainMenuWidget");
    setDrawBorder(false);
    setFillBackground(true);
    setBackgroundColor(0.35);
    setLayout(SAT_WIDGET_ALIGN_TOP_LEFT,SAT_WIDGET_STRETCH_HORIZONTAL);
  }

  //----------

  virtual ~SAT_MainMenuWidget() {
  }
  
//------------------------------
public:
//------------------------------

  virtual uint32_t appendMenu(const char* AText) {
    uint32_t index = MMenuItems.size();
    SAT_MenuItem* item = new SAT_MenuItem();
    item->owner = this;
    item->text = AText;
    MMenuItems.append(item);
    return index;
  }

  //----------
  
  virtual uint32_t appendSubMenu(uint32_t AIndex, const char* AText) {
    uint32_t index = MMenuItems[AIndex]->subitems.size();
    SAT_MenuItem* item = new SAT_MenuItem();
    item->owner = this;
    item->text = AText;
    MMenuItems[AIndex]->subitems.append(item);
    return index;
  }

  //----------
  
  /*
    called from om_widget_open()
    just before realignChildWidgets
  */

  //void prepare(SAT_WidgetOwner* AOwner) override {
  void on_widget_prepare() override {
    //SAT_WidgetOwner* AOwner = getOwner();
    SAT_Window* window = (SAT_Window*)getOwner();
    SAT_Widget* root = window->getRootWidget();
    
    //SAT_PanelWidget::prepare(AOwner);

    //SAT_Window* window = (SAT_Window*)AOwner;
    //SAT_Assert(window);
    
    SAT_Painter* painter = window->getPainter();
    SAT_Assert(painter);
    
    double x = 0.0;
    double y = 0.0;
    double w = 0.0;
    double h = getHeight();
    
    //int font = painter->getDefaultFont();
    //painter->selectFont(font);
    
    double textsize = 12;//getTextSize();
    double S = 1.0;//getWindowScale();
    painter->setTextSize( textsize * S );;
    
    uint32_t num_menus = MMenuItems.size();
    for (uint32_t i=0; i<num_menus; i++) {
      const char* text = MMenuItems[i]->text;
      double bounds[4];
      w = painter->getTextBounds(text,bounds);

      SAT_MenuWidget* menu = new SAT_MenuWidget(SAT_Rect(x,y+h,150,75));

      uint32_t num_submenus = MMenuItems[i]->subitems.size();
      for (uint32_t j=0; j<num_submenus; j++) {
        const char* subtext = MMenuItems[i]->subitems[j]->text;
        SAT_Print("%i.%i: %s\n",i,j,subtext);
        SAT_MenuItemWidget* submenu = new SAT_MenuItemWidget(j*10,subtext);
        submenu->setLayout(SAT_WIDGET_ALIGN_TOP,SAT_WIDGET_STRETCH_HORIZONTAL);
        
        menu->appendChildWidget(submenu);
      }
      
      SAT_SelectorWidget* selector = new SAT_SelectorWidget(SAT_Rect(x,y,w,h),text,menu);
      selector->setDrawArrow(false);
      selector->setDrawBorder(false);
      selector->setFillBackground(false);
      selector->setTextSize(10);
      //selector->setIsDropDown(true);
      selector->setDrawSelectedText(false);

      appendChildWidget(selector);
      root->appendChildWidget(menu);  // !!!

      x += w;

    }

    //SAT_PanelWidget::prepare(AOwner);
    
  }
  
};

//----------------------------------------------------------------------
#endif


