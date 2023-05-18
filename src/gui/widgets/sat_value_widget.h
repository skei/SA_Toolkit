#ifndef sat_value_widget_included
#define sat_value_widget_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "gui/widgets/sat_text_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_ValueWidget
: public SAT_TextWidget {

//------------------------------
private:
//------------------------------

  bool      MDrawValue      = true;
  SAT_Color MValueColor     = SAT_White;
  double    MValueSize      = 9;
  uint32_t  MValueAlignment = SAT_TEXT_ALIGN_RIGHT;
  SAT_Rect  MValueOffset    = {};
  char      MValueText[256] = {0};

  SAT_Color MDisabledColor  = SAT_Grey;
  double    MDisabledAlpha  = 0.5;

  bool      MDrawModulation = true;
  bool      MIsInteracting  = false;

//------------------------------
protected:
//------------------------------

//------------------------------
public:
//------------------------------

  SAT_ValueWidget(SAT_Rect ARect, const char* AText, double AValue)
  : SAT_TextWidget(ARect,AText) {
    setName("SAT_ValueWidget");
    setValue(AValue);
    setTextAlignment(SAT_TEXT_ALIGN_LEFT);
    setTextOffset(SAT_Rect(5,0,0,0));
    setValueOffset(SAT_Rect(0,0,5,0));
  }

  virtual ~SAT_ValueWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual SAT_Color   getDisabledColor()  { return MDisabledColor; }
  virtual double      getDisabledAlpha()  { return MDisabledAlpha; }
  virtual bool        getDrawModulation() { return MDrawModulation; }
  virtual bool        getDrawValue()      { return MDrawValue; }
  virtual bool        isInteracting()     { return MIsInteracting; }
  //virtual SAT_Rect    getValueOffset()    { return MValueOffset; }
  //virtual char*       getValueText()      { return MValueText; }

  virtual void        setIsInteracting(bool AState=true) { MIsInteracting = AState; }
  virtual void        setDrawValue(bool ADraw=true)      { MDrawValue = ADraw; }
  virtual void        setValueColor(SAT_Color AColor)    { MValueColor = AColor; }
  virtual void        setValueSize(double ASize)         { MValueSize = ASize; }
  virtual void        setValueAlignment(uint32_t AAlign) { MValueAlignment = AAlign; }
  virtual void        setValueOffset(SAT_Rect AOffset)   { MValueOffset = AOffset; }
  virtual void        setDrawModulation(bool ADraw=true) { MDrawModulation = ADraw; }

//------------------------------
public:
//------------------------------

  virtual void drawValue(SAT_PaintContext* AContext) {
    SAT_Assert(AContext);
    if (MDrawValue) {
      double S = getWindowScale();
      SAT_Painter* painter = AContext->painter;
      SAT_Assert(painter);
      SAT_Rect mrect = getRect();
      SAT_Rect valueoffset = MValueOffset;
      valueoffset.scale(S);
      mrect.shrink(valueoffset);
      if (mrect.w <= 0.0) return;
      if (mrect.h <= 0.0) return;
      
      SAT_Parameter* param = (SAT_Parameter*)getParameter(0);
      if (param) {
        
        double pv = getValue();
        double dpv = param->denormalizeValue(pv);

        //double wv = param->getValue();
        //double dwv = param->denormalizeValue(wv);
        
        //SAT_Print("denormalize. param %f -> dpv %f. widget %i ->  dwv %f\n",pv,dpv,wv,dwv);
        
        param->valueToText(dpv,MValueText,255);

      }
      else {
        
        double v = getValue();
        sprintf(MValueText,"%.2f",v);

      }
      
      painter->setTextColor(MValueColor);
      painter->setTextSize(MValueSize*S);
      painter->drawTextBox(mrect,MValueText,MValueAlignment);
    }
  }

  //----------

  virtual void drawModulation(SAT_PaintContext* AContext) {
    if (MDrawModulation) {
    }
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(SAT_PaintContext* AContext) override {
    //SAT_PRINT;
    drawDropShadow(AContext);
    fillBackground(AContext);
    drawText(AContext);
    drawValue(AContext);
    drawModulation(AContext);
    drawHostIndicators(AContext);
    paintChildWidgets(AContext);
    drawBorder(AContext);
  }

};

//----------------------------------------------------------------------
#endif


