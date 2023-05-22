#ifndef sat_dual_value_widget_included
#define sat_dual_value_widget_included
//----------------------------------------------------------------------

#include "base/sat.h"
//#include "plugin/sat_parameter.h"
//#include "gui/sat_window.h"
//#include "gui/widgets/sat_movable_widget.h"
#include "gui/widgets/sat_drag_value_widget.h"
#include "gui/widgets/sat_panel_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_DualValueWidget
: public SAT_PanelWidget {

//------------------------------
private:
//------------------------------

  SAT_DragValueWidget*  MDragValue1 = nullptr;
  SAT_DragValueWidget*  MDragValue2 = nullptr;
  
  bool MPushOther = true;
  bool MStopOther = false;

//------------------------------
public:
//------------------------------

  SAT_DualValueWidget(SAT_Rect ARect, double AValue1, double AValue2)
  : SAT_PanelWidget(ARect) {
    setName("SAT_DualValueWidget");
    //strcpy(MText,AText);
    //setFillBackground(true);
    
    MDragValue1 = new SAT_DragValueWidget(SAT_Rect(-50,0),"",AValue1);
    appendChildWidget(MDragValue1);
    MDragValue1->setLayout(SAT_WIDGET_ALIGN_LEFT_TOP,SAT_WIDGET_STRETCH_VERTICAL);
    MDragValue1->setValueAlignment(SAT_TEXT_ALIGN_LEFT);
    MDragValue1->setValueOffset(SAT_Rect(5,0,0,0));    

    MDragValue2 = new SAT_DragValueWidget(SAT_Rect(-50,0),"",AValue2);
    appendChildWidget(MDragValue2);
    MDragValue2->setLayout(SAT_WIDGET_ALIGN_RIGHT_TOP,SAT_WIDGET_STRETCH_VERTICAL);
    
    
  }
  
  //----------

  virtual ~SAT_DualValueWidget() {
  }

//------------------------------
public:
//------------------------------

//  virtual void setDrawText(bool ADraw=true)       { MDrawText = ADraw; }
//  virtual void setTextColor(SAT_Color AColor)     { MTextColor = AColor; }
//  virtual void setTextSize(double ASize)          { MTextSize = ASize; }
//  virtual void setTextAlignment(uint32_t AAlign)  { MTextAlignment = AAlign; }
//  virtual void setTextOffset(SAT_Rect AOffset)    { MTextOffset = AOffset; }
//  virtual void setText(const char* AText)         { strcpy(MText,AText); }
//  virtual void setAutoTextSize(bool AAuto=true)   { MAutoTextSize = AAuto; }
//
//  virtual void setDrawTextDropShadow(bool ADraw=true)     { MDrawTextDropShadow = ADraw; }
//  virtual void setTextDropShadowSize(double ASize)        { MTextDropShadowSize = ASize; }
//  virtual void setTextDropShadowColor(SAT_Color AColor)   { MTextDropShadowColor  = AColor; }
//  virtual void setTextDropShadowOffset(SAT_Point AOffset) { MTextDropShadowOffset = AOffset; }
//
//  virtual const char* getText() { return MText; }

//------------------------------
public:
//------------------------------

//  void setModulation(double AValue, uint32_t AIndex=0) {
//    SAT_PanelWidget::setModulation(AValue,AIndex);
//  }
//  
//  void setValue(double AValue, uint32_t AIndex=0) {
//    SAT_PanelWidget::setValue(AValue,AIndex);
//  }
  
  void connect(void* AParameter, uint32_t AIndex=0) {
    SAT_PanelWidget::connect(AParameter,AIndex);
  }
  

  


//------------------------------
public:
//------------------------------

//  void on_widget_paint(SAT_PaintContext* AContext) override {
//    drawDropShadow(AContext);
//    fillBackground(AContext);
//    drawText(AContext);
//    paintChildWidgets(AContext);
//    drawBorder(AContext);
//  }

//------------------------------
public:
//------------------------------

  void do_widgetListener_update(SAT_Widget* ASender, uint32_t AMode, uint32_t AIndex=0) override {
    if (ASender == MDragValue1) {
      double v1 = MDragValue1->getValue();
      double v2 = MDragValue2->getValue();
      if (v1 > v2) {
        if (MPushOther) {
          MDragValue2->setValue(v1);
          MDragValue2->parentUpdate();
          MDragValue2->parentRedraw();
        }
        else if (MStopOther) {
          MDragValue1->setValue(v2);
          //MDragValue1->parentUpdate();
          //MDragValue1->parentRedraw();
        }
      }
    }
    else if (ASender == MDragValue2) {
      double v1 = MDragValue1->getValue();
      double v2 = MDragValue2->getValue();
      if (v2 < v1) {
        if (MPushOther) {
          MDragValue1->setValue(v2);
          MDragValue1->parentUpdate();
          MDragValue1->parentRedraw();
        }
        else if (MStopOther) {
          MDragValue2->setValue(v1);
          //MDragValue2->parentUpdate();
          //MDragValue2->parentRedraw();
        }
      }
    }
    SAT_PanelWidget::do_widgetListener_update(ASender,AMode,AIndex);
  }
  
  //void do_widgetListener_redraw(SAT_Widget* ASender, uint32_t AMode, uint32_t AIndex=0) override {
  //}

};

//----------------------------------------------------------------------
#endif
