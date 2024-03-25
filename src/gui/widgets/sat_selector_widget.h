#ifndef sat_selector_widget_included
#define sat_selector_widget_included
//----------------------------------------------------------------------

#include "sat.h"
#include "base/utils/sat_random.h"
#include "gui/widgets/sat_menu_widget.h"
#include "gui/widgets/sat_menu_item_widget.h"
#include "gui/widgets/sat_panel_widget.h"
#include "gui/widgets/sat_symbol_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_SelectorWidget
//: public SAT_TextWidget
: public SAT_PanelWidget
, public SAT_MenuListener {

//------------------------------
private:
//------------------------------

  SAT_MenuWidget*   MMenu               = nullptr;
  SAT_SymbolWidget* MArrow              = nullptr;

  bool              MDrawSelectedText   = true;
  bool              MDrawArrow          = true;
  
  bool              MIsDropDown         = true;
  uint32_t          MDropDownDirection  = SAT_DIRECTION_DOWN;
  
  bool              MAutoSizeMenu       = false;
  
  uint32_t          MSelected           = 0;

//------------------------------
public:
//------------------------------

  SAT_SelectorWidget(SAT_Rect ARect, const char* AText, SAT_MenuWidget* AMenu, SAT_MenuListener* AListener=nullptr)
  //: SAT_TextWidget(ARect,AText) {
  : SAT_PanelWidget(ARect) {
    setName("SAT_SelectorWidget");
    setCursor(SAT_CURSOR_FINGER);
    MMenu = AMenu;
    
    if (AListener) MMenu->setMenuListener(AListener);
    else MMenu->setMenuListener(this);
    
    //setLayoutOuterBorder(SAT_Rect(2,2,2,2));

    setDrawText(true);
    setText(AText);

    setDrawValue(false);
    //setTextSize(6);
    
    if (MDrawArrow) {
      MArrow = new SAT_SymbolWidget(SAT_Rect(7,6),SAT_SYMBOL_FILLED_TRI_DOWN);
      appendChildWidget(MArrow);

      //MArrow->setAlignment(SAT_WIDGET_ALIGN_RIGHT_BOTTOM);
      MArrow->addLayoutFlag(SAT_WIDGET_LAYOUT_ANCHOR_RIGHT);
      MArrow->addLayoutFlag(SAT_WIDGET_LAYOUT_ANCHOR_BOTTOM);

      MArrow->setColor(SAT_DarkerGrey);
      MArrow->setActive(false);
    }
    
  }

  //----------

  virtual ~SAT_SelectorWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual SAT_MenuWidget* getMenuWidget() {
    return MMenu;
  }

  virtual void setDrawSelectedText(bool ADraw) {
    MDrawSelectedText = ADraw;
  }
  
  virtual void setDrawArrow(bool ADraw) {
    //MDrawSelectedText = ADraw;
    MArrow->setActive(ADraw);
    MArrow->setVisible(ADraw);
  }
  
  virtual void setIsDropDown(bool ADropDown=true)         { MIsDropDown = ADropDown; }
  virtual void setDropDownDirection(uint32_t ADirection)  { MDropDownDirection = ADirection; }
  virtual void setAutoSizeMenu(bool AAutoSize=true)       { MAutoSizeMenu = AAutoSize; }

//------------------------------
public: // widget
//------------------------------

  void on_widget_mouseClick(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    if (AButton == SAT_BUTTON_LEFT) {
      if (MMenu) {
        
        SAT_Rect rect = getRect();

        double width = 0;//MMenu->getWidth();
        double height = 0;//MMenu->getHeight();

        if (MAutoSizeMenu) {
          SAT_Window* window = getOwner()->on_widgetOwner_getWindow();
          SAT_Painter* painter = window->getPainter();
          double S = getWindowScale(); // window->getScale();
          SAT_Rect border = getLayoutInnerBorder(); // MMenu->getInnerBorder();
          border.scale(S);
          uint32_t num_items = MMenu->getNumChildWidgets();
          double bounds[4];
          for (uint32_t i=0; i<num_items; i++) {
            SAT_MenuItemWidget* widget = (SAT_MenuItemWidget*)MMenu->getChildWidget(i);
            const char* text = widget->getText();
            double text_size = widget->getTextSize();
            painter->setTextSize(text_size * S);
            double w = painter->getTextBounds(text,bounds);
            //w += (border.x + border.w);
            //SAT_Print("item %i w %.f h %.f\n",i,w,widget->getHeight());
            if (w > width) width = w;
            height += widget->getHeight();
          }
          width  += (border.x + border.w);
          height += (border.y + border.h);
        }
        else {
          width = MMenu->getWidth();
          height = MMenu->getHeight();
        }
        
        if (MIsDropDown) {
          //rect.x += border.x;
          //rect.y += border.y;
          switch (MDropDownDirection) {
            case SAT_DIRECTION_LEFT:
              rect.x -= width;
              break;
            case SAT_DIRECTION_RIGHT:
              rect.x += rect.w;
              break;
            case SAT_DIRECTION_UP:
              rect.y -= height;
              break;
            case SAT_DIRECTION_DOWN:
              rect.y += rect.h;
              break;
          }
        }
        else {
          rect.x = AXpos;
          rect.y = AYpos;
        }
        
        rect.w = width;//MMenu->getWidth();
        rect.h = height;//MMenu->getHeight();
        MMenu->open(rect);
      }
    }
  }
  
  //----------
  
  void setValue(double value, uint32_t index=0) override {
    //SAT_Print("value %f\n",value);
    SAT_PanelWidget::setValue(value,index);
    if (MDrawSelectedText) {
      SAT_Parameter* param = (SAT_Parameter*)getParameter();
      if (param) {
        uint32_t i = param->denormalize(value);
        SAT_PanelWidget* widget = (SAT_PanelWidget*)MMenu->getChildWidget(i);
        const char* text = widget->getText();
        //SAT_Print("%s\n",text);
        setText(text);
        //redrawParent();
        do_widget_redraw(this,0,0);
      }
    }
  }

//------------------------------
public: // menu listener
//------------------------------

  void do_menu_select(int32_t AIndex) override {
    
    // test/debug
    //{
    //  SAT_TextWidget* widget = (SAT_TextWidget*)MMenu->getChildWidget(AIndex);
    //  const char* text = widget->getText();
    //  SAT_Print("%s/%s\n",getName(),text);
    //}

    SAT_Parameter* param = (SAT_Parameter*)getParameter();
    if (param) {
      double value = param->normalize(AIndex);
      setValue(value);
      do_widget_update(this,0,0);
    }
    else {
      SAT_MenuWidget* menuwidget = getMenuWidget();
      uint32_t num = menuwidget->getNumChildWidgets();
      if (num > 0) {
        double v = (double)AIndex / (double)(num - 1);
        //SAT_Print("AIndex %i num %i -> v %f\n",AIndex,num,v);
        setValue(v);
      }
      do_widget_update(this,0,0);
    }
    
    if (MDrawSelectedText) {
      SAT_PanelWidget* widget = (SAT_PanelWidget*)MMenu->getChildWidget(AIndex);
      const char* text = widget->getText();
      //SAT_Print("%s\n",text);
      setText(text);
      //redrawParent();
      do_widget_redraw(this,0,0);
    }
  }

  //----------

  void do_menu_cancel() override {
  }

};

//----------------------------------------------------------------------
#endif