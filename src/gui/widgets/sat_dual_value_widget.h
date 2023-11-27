#ifndef sat_dual_value_widget_included
#define sat_dual_value_widget_included
//----------------------------------------------------------------------

#include "sat.h"
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
  
  bool MPushOther = false;
  bool MStopOther = true;

//------------------------------
public:
//------------------------------

  SAT_DualValueWidget(SAT_Rect ARect, double AValue1, double AValue2)
  : SAT_PanelWidget(ARect) {
    setName("SAT_DualValueWidget");
    //strcpy(MText,AText);
    //setFillBackground(true);
    
    MDragValue1 = new SAT_DragValueWidget(SAT_Rect(50,0),"",AValue1);
    appendChildWidget(MDragValue1);

    //MDragValue1->setLayout(SAT_WIDGET_ALIGN_LEFT_TOP,SAT_WIDGET_STRETCH_VERTICAL);
    MDragValue1->addLayoutFlag(SAT_WIDGET_LAYOUT_PERCENT_PARENT);
    MDragValue1->addLayoutFlag(SAT_WIDGET_LAYOUT_ANCHOR_TOP);
    MDragValue1->addLayoutFlag(SAT_WIDGET_LAYOUT_ANCHOR_LEFT);
    MDragValue1->addLayoutFlag(SAT_WIDGET_LAYOUT_STRETCH_VERT);
    //MDragValue1->addLayoutFlag(SAT_WIDGET_LAYOUT_CROP_TOP);

    MDragValue1->setValueAlignment(SAT_TEXT_ALIGN_LEFT);
    MDragValue1->setValueOffset(SAT_Rect(5,0,0,0));    

    MDragValue2 = new SAT_DragValueWidget(SAT_Rect(50,0),"",AValue2);
    appendChildWidget(MDragValue2);

    //MDragValue2->setLayout(SAT_WIDGET_ALIGN_RIGHT_TOP,SAT_WIDGET_STRETCH_VERTICAL);
    MDragValue2->addLayoutFlag(SAT_WIDGET_LAYOUT_PERCENT_PARENT);
    MDragValue2->addLayoutFlag(SAT_WIDGET_LAYOUT_ANCHOR_TOP);
    MDragValue2->addLayoutFlag(SAT_WIDGET_LAYOUT_ANCHOR_RIGHT);
    MDragValue2->addLayoutFlag(SAT_WIDGET_LAYOUT_STRETCH_VERT);
    //MDragValue2->addLayoutFlag(SAT_WIDGET_LAYOUT_CROP_TOP);
    
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

  //void on_widget_paint(SAT_PaintContext* AContext) override {
  //  drawDropShadow(AContext);
  //  fillBackground(AContext);
  //  drawText(AContext);
  //  paintChildWidgets(AContext);
  //  drawBorder(AContext);
  //}

  //----------

  // we won't receive events, since the child widgets fill the entire rect,
  // and 'takes over' the event..
  //
  //virtual void on_widget_mouseMove(double AXpos, double AYpos, uint32_t AState, uint32_t ATimeStamp) override {
  //  SAT_PRINT;
  //  SAT_PanelWidget::on_widget_mouseMove(AXpos,AYpos,AState,ATimeStamp);
  //}


//------------------------------
public:
//------------------------------

  void do_widget_update(SAT_Widget* ASender, uint32_t AMode=0) override {
    bool push = MPushOther;
    bool stop = MStopOther;
    if (AMode == SAT_STATE_SHIFT) push = true;

    if (ASender == MDragValue1) {
      double v1 = MDragValue1->getValue();
      double v2 = MDragValue2->getValue();
      if (AMode == SAT_STATE_SHIFT) push = true;
      if (v1 > v2) {
        if (push) {
          MDragValue2->setValue(v1);
// !!!
          //MDragValue2->parentUpdate();
          //MDragValue2->parentRedraw();
          do_widget_update(MDragValue2,AMode);  // parent...
          do_widget_redraw(MDragValue2);

        }
        else if (stop) {
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
        if (push) {
          MDragValue1->setValue(v2);
// !!!
          //MDragValue1->parentUpdate();
          //MDragValue1->parentRedraw();
          do_widget_update(MDragValue1);  // parent...
          do_widget_redraw(MDragValue1);

        }
        else if (stop) {
          MDragValue2->setValue(v1);
          //MDragValue2->parentUpdate();
          //MDragValue2->parentRedraw();
        }
      }
    }
    SAT_PanelWidget::do_widget_update(ASender);
  }
  
  //void do_widgetListener_redraw(SAT_Widget* ASender, uint32_t AMode, uint32_t AIndex=0) override {
  //}

};

//----------------------------------------------------------------------
#endif

