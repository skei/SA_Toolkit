#ifndef sat_curve_widget_includedbutton
#define sat_curve_widget_included
//----------------------------------------------------------------------

#include "gui/widgets/sat_drag_value_widget.h"

class SAT_CurveWidget
: public SAT_DragValueWidget {

//------------------------------
protected:
//------------------------------

    SAT_Color MCurveColor     = SAT_DarkestGrey;
    SAT_Color MInteractColor  = SAT_Color(1);//SAT_COLOR_WHITE;
    bool      MDecay          = false;
    double    MCoords[1024]   = {0};
    double    MCurveWidth     = 2.0;

    //SAT_Point  MCoords[1024]   = {0};

//------------------------------
public:
//------------------------------

  SAT_CurveWidget(SAT_Rect ARect, bool ADecay=true)
  : SAT_DragValueWidget(ARect,"",0.5) {
    setName("SAT_CurveWidget");
    MDecay = ADecay;
    setText("curve");
    setBackgroundColor(SAT_DarkGrey);
    setBorderWidth(0.5);
    setBorderColor(SAT_Grey);
  }

  //----------

  virtual ~SAT_CurveWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setCurveColor(SAT_Color AColor)    { MCurveColor = AColor; }
  virtual void setCurveWidth(double AWidth)       { MCurveWidth = AWidth; }
  virtual void setInteractColor(SAT_Color AColor) { MInteractColor = AColor; }
  
  virtual void drawCurve(SAT_PaintContext* AContext) {
    SAT_Painter* painter = AContext->painter;
    SAT_Rect mrect = getRect();
    double S = getWindowScale();
    mrect.shrink(MCurveWidth * S);
    int32_t num = mrect.w;
    // calc coords

    if (num > 0) {
      float x,xadd;
      if (MDecay) {
        x = 1.0;
        xadd = -1.0f / num;
      }
      else {
        x = 0;
        xadd = 1.0f / num;
      }
      //float h = (float)mrect.h;
      //int32 y2 = mrect.y2() + 1;
      float value = getValue();
      for (int32_t i=0; i<num; i++) {
        float n = SAT_Curve(x,value);
        MCoords[ i * 2     ] = mrect.x + i;
        MCoords[(i * 2) + 1] = mrect.y2() + 1 - (n * (float)mrect.h);
        x += xadd;
      }
      
    }

    if (MDecay) {
      MCoords[ (num      * 2)    ] = mrect.x2();
      MCoords[ (num      * 2) + 1] = mrect.y2() + 1;
      MCoords[((num + 1) * 2)    ] = mrect.x;
      MCoords[((num + 1) * 2) + 1] = mrect.y2() + 1;
    }
    else {
      MCoords[ (num      * 2)    ] = mrect.x2();
      MCoords[ (num      * 2) + 1] = mrect.y + 1;
      MCoords[((num + 1) * 2)    ] = mrect.x2();
      MCoords[((num + 1) * 2) + 1] = mrect.y2() + 1;
    }

    // background

    //APainter->setFillColor(MBackgroundColor);

    //painter->beginPath();
    //painter->rect(mrect.x,mrect.y,mrect.w,mrect.h);
    //painter->fillColor(MBackgroundColor);
    //painter->fill();

    // curve

    //APainter->setPenSize(2);

    SAT_Color color = MCurveColor;
    //if (MIsInteractive) color = MInteractColor;

    //painter->beginPath();
    //painter->moveTo(MXCoords[0],MYCoords[0]);
    //for (int32_t i=1; i<num; i++) {
    //  painter->lineTo(MXCoords[i],MYCoords[i]);
    //}
    //painter->strokeWidth(2);
    //painter->strokeColor(color);
    //painter->stroke();
    
    painter->setDrawColor(color);
    painter->setLineWidth(MCurveWidth * S);
    painter->drawLineStrip(num+1,MCoords);

    //APainter->setPenSize(1);
    
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(SAT_PaintContext* AContext) override {
    drawDropShadow(AContext);
    fillBackground(AContext);
    drawCurve(AContext);
    paintChildWidgets(AContext);
    drawBorder(AContext);
  }

};

//----------------------------------------------------------------------
#endif