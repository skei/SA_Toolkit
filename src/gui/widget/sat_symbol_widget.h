#ifndef sat_symbol_widget_included
#define sat_symbol_widget_included
//----------------------------------------------------------------------

// /DISKS/sda3/download/code/audio/fontaudio-1.1
// SAT_LogoWidget

//----------------------------------------------------------------------

#include "sat.h"
#include "gui/widget/sat_visual_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_SymbolWidget
: public SAT_VisualWidget {

//------------------------------
private:
//------------------------------

  bool      MDrawSymbol = true;
  uint32_t  MSymbol     = SAT_SYMBOL_NONE;
  SAT_Color MColor      = SAT_LightGrey;
  double    MPenWidth   = 1.0;

//------------------------------
public:
//------------------------------

  SAT_SymbolWidget(SAT_Rect ARect, uint32_t ASymbol=SAT_SYMBOL_NONE)
  : SAT_VisualWidget(ARect) {
    setName("SAT_SymbolWidget");
    MSymbol = ASymbol;
    setFillBackground(false);
    setDrawBorder(false);
  }

  virtual ~SAT_SymbolWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setDrawSymbol(bool ADraw)          { MDrawSymbol = ADraw; }
  virtual void setSymbol(uint32_t ASymbol)        { MSymbol = ASymbol; }
  virtual void setColor(SAT_Color AColor)         { MColor = AColor; }
  virtual void setPenWidth(double AWidth)         { MPenWidth = AWidth; }

//------------------------------
public:
//------------------------------

  virtual void drawSymbol(SAT_PaintContext* AContext) {
    SAT_Assert(AContext);
    if (MDrawSymbol) {
      double S = getWindowScale();
      SAT_Painter* painter = AContext->painter;
      SAT_Assert(painter);
      SAT_Rect mrect = getRect();
      mrect.shrink((MPenWidth*0.5)*S);

      //SAT_Rect offset = MOffset;
      //offset.scale(S);
      //mrect.shrink(offset);
      
      switch (MSymbol) {
        
        case SAT_SYMBOL_RECT: {
          painter->setLineWidth(MPenWidth * S);
          painter->setDrawColor(MColor);
          painter->drawRect(mrect.x,mrect.y,mrect.w,mrect.h);
          break;
        }
        
        case SAT_SYMBOL_FILLED_RECT : {
          //painter->setLineWidth(MPenWidth * S);
          painter->setFillColor(MColor);
          painter->fillRect(mrect.x,mrect.y,mrect.w,mrect.h);
          break;
        }
        
        case SAT_SYMBOL_CIRCLE: {
          double w = mrect.w;
          if (mrect.h < mrect.w) w = mrect.h;
          double w05 = w * 0.5;
          painter->setLineWidth(MPenWidth * S);
          painter->setDrawColor(MColor);
          painter->drawCircle(mrect.x + w05, mrect.y + w05, w05 );
          break;
        }
        
        case SAT_SYMBOL_FILLED_CIRCLE: {
          double w = mrect.w;
          if (mrect.h < mrect.w) w = mrect.h;
          double w05 = w * 0.5;
          //painter->setLineWidth(MPenWidth * S);
          painter->setFillColor(MColor);
          painter->fillCircle(mrect.x + w05, mrect.y + w05, w05 );
          break;
        }
        
        //case SAT_SYMBOL_TRI_UP:
        
        case SAT_SYMBOL_TRI_DOWN: {
          double coords[] = {
            mrect.x,                    mrect.y,
            mrect.x2(),                 mrect.y,
            mrect.x + (mrect.w * 0.5),  mrect.y2(),
            mrect.x,                    mrect.y
          };
          painter->setLineWidth(MPenWidth * S);
          painter->setDrawColor(MColor);
          painter->drawLineStrip(4,coords);
          break;
        }
        
        //case SAT_SYMBOL_TRI_LEFT:
        //case SAT_SYMBOL_TRI_RIGHT:
        
        //case SAT_SYMBOL_FILLED_TRI_UP:
        case SAT_SYMBOL_FILLED_TRI_DOWN: {
          double coords[] = {
            mrect.x,                    mrect.y,
            mrect.x2(),                 mrect.y,
            mrect.x + (mrect.w * 0.5),  mrect.y2(),
            mrect.x,                    mrect.y
          };
          //painter->setLineWidth(MPenWidth * S);
          painter->setFillColor(MColor);
          painter->fillLineStrip(4,coords);
          break;
        }
        
        //case SAT_SYMBOL_FILLED_TRI_LEFT:
        //case SAT_SYMBOL_FILLED_TRI_RIGHT:
        
        case SAT_SYMBOL_SINE: {
          double x1 = mrect.x;
          double y1 = mrect.y;
          double x2 = mrect.x2(); 
          double y2 = mrect.y2();
          double xm = (x1 + x2) * 0.5;
          double ym = (y1 + y2) * 0.5;
          painter->setDrawColor(MColor);
          painter->setLineWidth(MPenWidth * S);
          painter->drawCurveBezier(x1,ym,xm,ym,x1,y1-(mrect.h / 6.0),xm,y1-(mrect.h / 6.0));
          painter->drawCurveBezier(xm,ym,x2,ym,xm,y2+(mrect.h / 6.0),x2,y2+(mrect.h / 6.0));
          break;
        }

        case SAT_SYMBOL_SAW: {
          double x1 = mrect.x;
          double y1 = mrect.y;
          double x2 = mrect.x2(); 
          double y2 = mrect.y2();
          double xm = (x1 + x2) * 0.5;
          double ym = (y1 + y2) * 0.5;
          painter->setDrawColor(MColor);
          painter->setLineWidth(MPenWidth * S);
          painter->drawLine(x1,ym,xm,y1);
          painter->drawLine(xm,y1,xm,y2);
          painter->drawLine(xm,y2,x2,ym);
          break;
        }

        case SAT_SYMBOL_SQUARE: {
          double x1 = mrect.x;
          double y1 = mrect.y;
          double x2 = mrect.x2(); 
          double y2 = mrect.y2();
          double xm = (x1 + x2) * 0.5;
          double ym = (y1 + y2) * 0.5;
          painter->setDrawColor(MColor);
          painter->setLineWidth(MPenWidth * S);
          painter->drawLine(x1,ym,x1,y1);
          painter->drawLine(x1,y1,xm,y1);
          painter->drawLine(xm,y1,xm,y2);
          painter->drawLine(xm,y2,x2,y2);
          painter->drawLine(x2,y2,x2,ym);
          break;
        }

        case SAT_SYMBOL_TRI: {
          double x1 = mrect.x;
          double x2 = x1 + (mrect.w * 0.333);
          double x3 = x1 + (mrect.w * 0.666);
          double x4 = mrect.x2(); 
          double y1 = mrect.y;
          double y2 = mrect.y2();
          double ym = (y1 + y2) * 0.5;
          painter->setDrawColor(MColor);
          painter->setLineWidth(MPenWidth * S);
          painter->drawLine(x1,ym,x2,y1);
          painter->drawLine(x2,y1,x3,y2);
          painter->drawLine(x3,y2,x4,ym);
          break;
        }

        case SAT_SYMBOL_NOISE: {
          break;
        }

        
      }
      //painter->setTextColor(MTextColor);
      //painter->setTextSize(MTextSize*S);
      //painter->drawTextBox(mrect,MText,MTextAlignment);
    }
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(SAT_PaintContext* AContext) override {
    //drawDropShadow(AContext);
    fillBackground(AContext);
    drawSymbol(AContext);
    paintChildren(AContext);
    drawBorder(AContext);
  }

};

//----------------------------------------------------------------------
#endif
