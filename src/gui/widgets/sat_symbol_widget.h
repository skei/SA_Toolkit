#ifndef sat_symbol_widget_included
#define sat_symbol_widget_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "gui/widgets/sat_panel_widget.h"




//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_SymbolWidget
: public SAT_PanelWidget {

//------------------------------
private:
//------------------------------

  bool      MDrawSymbol = true;
  uint32_t  MSymbol     = SAT_SYMBOL_NONE;
  SAT_Color MColor      = SAT_LightGrey;
  double    MPenWidth   = 1.0;
  SAT_Rect  MOffset     = SAT_Rect(0,0,0,0);

//------------------------------
public:
//------------------------------

  SAT_SymbolWidget(SAT_Rect ARect, uint32_t ASymbol=SAT_SYMBOL_NONE)
  : SAT_PanelWidget(ARect) {
    setName("SAT_SymbolWidget");
    MSymbol = ASymbol;
    setFillBackground(false);
    setDrawBorder(false);
    setCursor(SAT_CURSOR_QUESTION);
  }

  virtual ~SAT_SymbolWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setSymbol(uint32_t ASymbol)        { MSymbol = ASymbol; }
  virtual void setColor(SAT_Color AColor)         { MColor = AColor; }
  virtual void setOffset(SAT_Rect AOffset)        { MOffset = AOffset; }

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
      mrect.shrink(MOffset);
      switch (MSymbol) {
        
        case SAT_SYMBOL_RECT: {
          painter->setLineWidth(MPenWidth * S);
          painter->setDrawColor(MColor);
          painter->drawRect(mrect.x,mrect.y,mrect.w,mrect.h);
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
        
        case SAT_SYMBOL_FILLED_RECT: {
          //painter->setLineWidth(MPenWidth * S);
          painter->setFillColor(MColor);
          painter->fillRect(mrect.x,mrect.y,mrect.w,mrect.h);
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
    drawDropShadow(AContext);
    fillBackground(AContext);
    //paintChildren(AContext);
    drawSymbol(AContext);
    drawBorder(AContext);
  }

};

//----------------------------------------------------------------------
#endif

