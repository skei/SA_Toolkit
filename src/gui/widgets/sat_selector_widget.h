#ifndef sat_selector_widget_included
#define sat_selector_widget_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "base/utils/sat_random.h"
#include "gui/widgets/sat_menu_widget.h"
#include "gui/widgets/sat_text_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_SelectorWidget
//: public SAT_TextWidget
: public SAT_ValueWidget
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

//------------------------------
public:
//------------------------------

  SAT_SelectorWidget(SAT_Rect ARect, const char* AText, SAT_MenuWidget* AMenu)
  //: SAT_TextWidget(ARect,AText) {
  : SAT_ValueWidget(ARect,AText,0) {
    setName("SAT_SelectorWidget");
    setCursor(SAT_CURSOR_FINGER);
    MMenu = AMenu;
    MMenu->setMenuListener(this);
    setInnerBorder(SAT_Rect(2,2));
    setDrawValue(false);
    setTextSize(6);
    
    if (MDrawArrow) {
      MArrow = new SAT_SymbolWidget(SAT_Rect(7,6),SAT_SYMBOL_FILLED_TRI_DOWN);
      appendChildWidget(MArrow);
      MArrow->setAlignment(SAT_WIDGET_ALIGN_RIGHT_BOTTOM);
      MArrow->setColor(SAT_DarkerGrey);
    }
    
  }

  //----------

  virtual ~SAT_SelectorWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setDrawSelectedText(bool ADraw) {
    MDrawSelectedText = ADraw;
  }
  
  virtual void setDrawArrow(bool ADraw) {
    MDrawSelectedText = ADraw;
    MArrow->setActive(ADraw);
    MArrow->setVisible(ADraw);
  }
  
  virtual void setIsDropDown(bool ADropDown=true) { MIsDropDown = ADropDown; }
  virtual void setDropDownDirection(uint32_t ADirection) { MDropDownDirection = ADirection; }
  virtual void setAutoSizeMenu(bool AAutoSize=true) { MAutoSizeMenu = AAutoSize; }

//------------------------------
public: // widget
//------------------------------

  void on_widget_mouse_click(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    if (AButton == SAT_BUTTON_LEFT) {
      if (MMenu) {
        
        double width = 0;//MMenu->getWidth();
        double height = 0;//MMenu->getHeight();
        SAT_Rect rect = getRect();

        if (MAutoSizeMenu) {
        
          SAT_Window* window = (SAT_Window*)getOwner();
          double S = window->getScale();
          SAT_Painter* painter = window->getPainter();
          SAT_Rect border = MMenu->getInnerBorder();
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
  
  void setValue(double value, uint32_t index=0) override {
    //SAT_Print("value %f\n",value);
    SAT_ValueWidget::setValue(value,index);
    if (MDrawSelectedText) {
      SAT_Parameter* param = (SAT_Parameter*)getParameter();
      if (param) {
        uint32_t i = param->denormalizeValue(value);
        SAT_TextWidget* widget = (SAT_TextWidget*)MMenu->getChildWidget(i);
        const char* text = widget->getText();
        //SAT_Print("%s\n",text);
        setText(text);
        //parentRedraw();
        do_widgetListener_redraw(this,0);
      }
    }
  }

//------------------------------
public: // menu listener
//------------------------------

  void do_menu_select(int32_t AIndex) override {
    
    // test/debug
    {
      SAT_TextWidget* widget = (SAT_TextWidget*)MMenu->getChildWidget(AIndex);
      const char* text = widget->getText();
      SAT_Print("%s/%s\n",getName(),text);
    }

    SAT_Parameter* param = (SAT_Parameter*)getParameter();
    if (param) {
      double value = param->normalizeValue(AIndex);
      setValue(value);
      do_widgetListener_update(this,0);
    }
    
    if (MDrawSelectedText) {
      SAT_TextWidget* widget = (SAT_TextWidget*)MMenu->getChildWidget(AIndex);
      const char* text = widget->getText();
      //SAT_Print("%s\n",text);
      setText(text);
      //parentRedraw();
      do_widgetListener_redraw(this,0);
    }
  }

  //----------

  void do_menu_cancel() override {
  }

};

//----------------------------------------------------------------------
#endif
