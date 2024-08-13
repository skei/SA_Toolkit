#ifndef sat_main_menu_widget_included
#define sat_main_menu_widget_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/widget/sat_menu_widget.h"
#include "gui/widget/sat_text_widget.h"
#include "gui/widget/sat_visual_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_MainMenuItemWidget
: public SAT_ButtonWidget {

//------------------------------
private:
//------------------------------

  const char*     MText = "menuitem";
  SAT_MenuWidget* MMenu = nullptr;

//------------------------------
public:
//------------------------------

  SAT_MainMenuItemWidget(SAT_Rect ARect, const char* AText, SAT_MenuWidget* AMenu)
  : SAT_ButtonWidget(ARect,true) {
    MText = AText;
    MMenu = AMenu;
    Layout.flags  = SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
    Layout.flags |= SAT_WIDGET_LAYOUT_FILL_LEFT;
    setTextOffset(SAT_Rect(5,0,5,0));
    setTexts(AText,AText);
  }

  //----------

  virtual ~SAT_MainMenuItemWidget() {
  }

  virtual const char* getText() { return MText; }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_MainMenuWidget
: public SAT_VisualWidget {

//------------------------------
public:
//------------------------------

  SAT_Array<SAT_MainMenuItemWidget*>  MMenus        = {};
  double                              MMenuSpacing  = 10.0;

//------------------------------
public:
//------------------------------

  SAT_MainMenuWidget(SAT_Rect ARect)
  : SAT_VisualWidget(ARect) {
    setName("SAT_MainMenuWidget");
    setHint("SAT_MainMenuWidget");
    Layout.flags  = SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
    Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
    Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;
    setFillBackground(true);
    setBackgroundColor(0.45);
    setDrawBorder(false);
  }

  //----------

  virtual ~SAT_MainMenuWidget() {
  }

//------------------------------
public:
//------------------------------

  void on_widget_open(SAT_WidgetOwner* AOwner) override {
    SAT_Assert(AOwner);
    double scale = getWindowScale();
    SAT_Assert(scale > 0.0);
    SAT_Painter* painter = AOwner->on_WidgetOwner_getPainter();
    SAT_Assert(painter);
    for (uint32_t i=0; i<MMenus.size(); i++) {
      SAT_MainMenuItemWidget* item = MMenus[i];
      const char* text = item->getText();
      double textsize = item->getTextSize();
      painter->setTextSize(textsize * scale);
      double bounds[4] = {0,0,0,0};
      double advance = painter->getTextBounds(text,bounds);
      //SAT_PRINT("text %s size %.2f scale %.2f bounds %.2f,%.2f,%.2f,%.2f advance %.2f\n",text,textsize,scale,bounds[0],bounds[1],bounds[2],bounds[3],advance);

      //item->setWidth(advance);
      SAT_Rect rect = item->getBaseRect();
      rect.w = advance + MMenuSpacing;
      rect.w /= scale;
      item->setBaseRect(rect);

    }
  }



//------------------------------
public:
//------------------------------

  virtual int32_t appendMenu(const char* AText, SAT_MenuWidget* AMenu) {
    uint32_t index = MMenus.size();
    SAT_MainMenuItemWidget* item = new SAT_MainMenuItemWidget(20,AText,AMenu);
    MMenus.append(item);
    appendChild(item);
    return index;
  }

};

//----------------------------------------------------------------------
#endif





#if 0

#include "sat.h"
//#include "gui/widgets/sat_popup_widget.h"
#include "gui/widgets/sat_menu_widget.h"
#include "gui/widgets/sat_menu_item_widget.h"
#include "gui/widgets/sat_selector_widget.h"
#include "gui/widgets/sat_panel_widget.h"

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
    setHint("SAT_MainMenuWidget");
    setDrawBorder(false);
    setFillBackground(true);
    setBackgroundColor(0.35);
//    setLayout(SAT_WIDGET_ALIGN_TOP_LEFT,SAT_WIDGET_STRETCH_HORIZONTAL);
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

  // -> on_widget_open()
  #if 0

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

  }
  
  #endif // 0

};

#endif // 0


