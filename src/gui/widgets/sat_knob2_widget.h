#ifndef sat_knob2_widget_included
#define sat_knob2_widget_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "gui/widgets/sat_knob_widget.h"
#include "gui/widgets/sat_panel_widget.h"
#include "gui/widgets/sat_text_widget.h"
#include "gui/widgets/sat_value_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Knob2Widget
: public SAT_PanelWidget {
  
//------------------------------
//private:
public:
//------------------------------

  SAT_KnobWidget*   MKnobWidget   = nullptr;
  SAT_TextWidget*   MTextWidget   = nullptr;
  SAT_ValueWidget*  MValueWidget  = nullptr;
  
  double MTextHeight  = 12;
  double MValueHeight = 10;

//------------------------------
public:
//------------------------------

  SAT_Knob2Widget(SAT_Rect ARect, const char* AText="%", double AValue=0.0)
  : SAT_PanelWidget(ARect) {
    
    double w = ARect.w;
    double h = ARect.h - MTextHeight - MValueHeight;
    double size = SAT_Min(w,h);
    double r = size * 0.2;
    
    MTextWidget  = new SAT_TextWidget( SAT_Rect(ARect.w,MTextHeight),"Text");
    MKnobWidget  = new SAT_KnobWidget( SAT_Rect(size,size),"",0.0);
    MValueWidget = new SAT_ValueWidget(SAT_Rect(ARect.w,MValueHeight),"Value",0.0);
    
    MTextWidget->setLayout(SAT_WIDGET_ALIGN_TOP_LEFT,SAT_WIDGET_STRETCH_HORIZONTAL);
    MTextWidget->setDrawBorder(false);
    
    MKnobWidget->setLayout(SAT_WIDGET_ALIGN_TOP_CENTER,SAT_WIDGET_STRETCH_NONE);
    MKnobWidget->setDrawValue(false);
    MKnobWidget->setArcThickness(r);

    MValueWidget->setLayout(SAT_WIDGET_ALIGN_BOTTOM_LEFT,SAT_WIDGET_STRETCH_HORIZONTAL);
    MValueWidget->setDrawText(false);
    MValueWidget->setTextOffset(0);
    MValueWidget->setValueOffset(0);
    MValueWidget->setValueAlignment(SAT_TEXT_ALIGN_CENTER);
    MValueWidget->setDrawBorder(false);
    
    appendChildWidget(MTextWidget);
    appendChildWidget(MKnobWidget);
    appendChildWidget(MValueWidget);
    
  }
  
  //----------

  virtual ~SAT_Knob2Widget() {
  }

//------------------------------
public:
//------------------------------

//  double getValue(uint32_t AIndex=0) override {
//    //MIP_PRINT;
//    double v = MKnobWidget->getValue();
//    SAT_Print("v %f\n",v);
//    return v;
//  }
  
  //----------

//  void setValue(double v, uint32_t AIndex=0) override {
//    SAT_Print("v %f\n",v);
//    MKnobWidget->setValue(v);
  //  SAT_Parameter* parameter = MKnobWidget->getParameter();
  //  if (parameter) {
  //    parameter->valueToText(v,MKnobValueText,31);
  //  }
  //  else {
  //    sprintf(MKnobValueText,"%.2f",v);
  //  }
  //  MValueWidget->setText(MKnobValueText);
  //  MValueWidget->redraw();
//  }

  //----------

  //double getMinValue() override {
  //  //MIP_PRINT;
  //  return MKnobWidget->getMinValue();
  //}

  //void setMinValue(double v) override {
  //  //MIP_PRINT;
  //  MKnobWidget->setMinValue(v);
  //}

  //double getMaxValue() override {
  //  //MIP_PRINT;
  //  return MKnobWidget->getMaxValue();
  //}

  //void setMaxValue(double v) override {
  //  //MIP_PRINT;
  //  MKnobWidget->setMaxValue(v);
  //}
  
  //double getDefaultValue() override {
  //  //MIP_PRINT;
  //  return MKnobWidget->getDefaultValue();
  //}

  //void setDefaultValue(double v) override {
  //  //MIP_PRINT;
  //  MKnobWidget->setDefaultValue(v);
  //}

  //SAT_Parameter* getParameter() override {
  //  return MKnobWidget->getParameter();
  //}

  //void setParameter(SAT_Parameter* AParam) override {
  //  MKnobWidget->setParameter(AParam);
  //}

//------------------------------
public:
//------------------------------

  void connect(void* AParameter, uint32_t AIndex=0) override {
    MKnobWidget->connect(AParameter,AIndex);
  }

  //----------

  void do_widgetListener_update(SAT_Widget* ASender, uint32_t AMode, uint32_t AIndex=0) override {
    if (ASender == MKnobWidget) {
      double value = MKnobWidget->getValue();
      MValueWidget->setValue(value);
      MValueWidget->parentRedraw();
    }
    SAT_Widget::do_widgetListener_update(ASender,AMode,AIndex);
  }
  
};

//----------------------------------------------------------------------
#endif
