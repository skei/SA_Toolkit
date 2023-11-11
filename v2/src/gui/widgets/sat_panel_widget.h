#ifndef sat_panel_widget_included
#define sat_panel_widget_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/sat_widget.h"
#include "plugin/sat_parameter.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_PanelWidget
: public SAT_Widget {

//------------------------------
protected:
//------------------------------

  // indicators

  SAT_Rect  MMappedIndicatorOffset      = SAT_Rect();
  SAT_Rect  MAutomationIndicatorOffset  = SAT_Rect();

  // background
  
  bool      MFillBackground             = true;
  SAT_Color MBackgroundColor            = SAT_DarkGrey;

  // gradient
  
  bool      MFillGradient               = false;
  SAT_Color MGradientColor1             = SAT_LightGrey;
  SAT_Color MGradientColor2             = SAT_DarkGrey;
  double    MGradientFactor1            = 0.2;
  double    MGradientFactor2            = 0.2;

  // border
  
  bool      MDrawBorder                 = true;
  SAT_Color MBorderColor                = SAT_DarkerGrey;
  double    MBorderWidth                = 1.0;
  uint32_t  MBorderEdges                = SAT_EDGE_ALL;
  uint32_t  MBorderCorners              = SAT_CORNER_NONE;

  // rounded corners
  
  bool      MRoundedCorners             = false;
  double    MTLCorner                   = 0.0;
  double    MTRCorner                   = 0.0;
  double    MBRCorner                   = 0.0;
  double    MBLCorner                   = 0.0;

  // drop shadow
  
  bool      MDrawDropShadow             = false;
  double    MDropShadowFeather          = 10.0;
  double    MDropShadowCorner           = 0.0;
  double    MDropShadowXOffset          = 0.0;
  double    MDropShadowYOffset          = 0.0;
  SAT_Color MDropShadowIColor           = SAT_DarkestGrey;
  SAT_Color MDropShadowOColor           = SAT_DarkGrey;
  bool      MDropShadowInner            = false;

  double    MDropShadowXOffsetTMP       = 0.0;
  double    MDropShadowYOffsetTMP       = 0.0;

  // text

  bool        MDrawText             = true;
  SAT_Color   MTextColor            = SAT_White;
  double      MTextSize             = 16.0;
  uint32_t    MTextAlignment        = SAT_TEXT_ALIGN_CENTER;
  SAT_Rect    MTextOffset           = {};
//  const char* MText                 = nullptr;
  char        MText[1024]           = {0};
  
  bool        MAutoSize             = false;
  SAT_Rect    MAutoSizeBorder       = SAT_Rect(0,0,0,0);
  bool        MAutoTextSize         = false;
  bool        MDrawParamText        = false;

  bool        MDrawTextDropShadow   = false;
  double      MTextDropShadowSize   = 4.0;
  SAT_Color   MTextDropShadowColor  = SAT_Black;
  SAT_Point   MTextDropShadowOffset = {2,2};

  // value

  bool      MDrawValue              = false;
  SAT_Color MValueColor             = SAT_White;
  double    MValueSize              = 16;
  uint32_t  MValueAlignment         = SAT_TEXT_ALIGN_RIGHT;
  SAT_Rect  MValueOffset            = {0,0,0,0};
  char      MValueText[256]         = {0};

  bool      MDrawValueDropShadow    = false;
  double    MValueDropShadowSize    = 4.0;
  SAT_Color MValueDropShadowColor   = SAT_Black;//SAT_Color(0,0,0,0.75);
  SAT_Point MValueDropShadowOffset  = {2,2};

  //

//  SAT_Color MDisabledColor          = SAT_Grey;
//  double    MDisabledAlpha          = 0.5;

//  bool      MDrawModulation         = true;
//  bool      MIsInteracting          = false;



//------------------------------
public:
//------------------------------

  SAT_PanelWidget(SAT_Rect ARect)
  : SAT_Widget(ARect) {
    setName("SAT_PanelWidget");
  }

  virtual ~SAT_PanelWidget() {
  }

//------------------------------
public:
//------------------------------

  // background

  virtual void setFillBackground(bool AFill=true) {
    MFillBackground = AFill;
  }
  
  virtual void setBackgroundColor(SAT_Color AColor) {
    MBackgroundColor = AColor;
  }
  
  // gradient

  virtual void setFillGradient(bool AGradient=true) {
    MFillGradient = AGradient;
  }
  
  virtual void setGradientColors(SAT_Color AColor1, SAT_Color AColor2) {
    MGradientColor1 = AColor1;
    MGradientColor2 = AColor2;
  }
  
  virtual void setGradientFactors(double AFactor1, double AFactor2) {
    MGradientFactor1 = AFactor1;
    MGradientFactor2 = AFactor2;
  }
  
  virtual void setGradientColors(SAT_Color AColor, double AFactor1, double AFactor2) {
    MGradientFactor1 = AFactor1;
    MGradientFactor2 = AFactor2;
    MGradientColor1 = AColor;
    MGradientColor2 = AColor;
    MGradientColor1.blend(SAT_White,AFactor1);
    MGradientColor2.blend(SAT_Black,AFactor2);
  }
  
  virtual void setGradientColors(SAT_Color AColor) {
    MGradientColor1 = AColor;
    MGradientColor2 = AColor;
    MGradientColor1.blend(SAT_White,MGradientFactor1);
    MGradientColor2.blend(SAT_Black,MGradientFactor2);
  }
  
  // border

  virtual void setDrawBorder(bool ADraw=true) {
    MDrawBorder = ADraw;
  }
  
  virtual void setBorderColor(SAT_Color AColor) {
    MBorderColor = AColor;
  }
  
  virtual void setBorderWidth(double AWidth) {
    MBorderWidth = AWidth;
  }
  
  virtual void setBorderEdges(uint32_t AEdges) {
    MBorderEdges = AEdges;
  }
  
  virtual void setBorderCorners(uint32_t ACorners) {
    MBorderCorners = ACorners;
  }
  
  // rounded corners

  virtual void setRoundedCorners(bool ARounded=true) {
    MRoundedCorners = ARounded;
    if (!ARounded) {
      MTLCorner = 0;
      MTRCorner = 0;
      MBRCorner = 0;
      MBLCorner = 0;
    }
  }
  
  virtual void setCornerSize(double c)  {
    MTLCorner = c;
    MTRCorner = c;
    MBRCorner = c;
    MBLCorner = c;
    MDropShadowCorner = c;
  }
  
  virtual void setCornerSizes(double top_left, double top_right, double bottom_right, double bottom_left)  {
    MTLCorner = top_left;
    MTRCorner = top_right;
    MBRCorner = bottom_right;
    MBLCorner = bottom_left;
    MDropShadowCorner = top_left;
  }
  
  // drop shadow

  virtual void setDrawDropShadow(bool ADraw=true) {
    MDrawDropShadow = ADraw;
  }
  
  virtual void setDropShadowFeather(double AValue) {
    MDropShadowFeather = AValue;
  }
  
  virtual void setDropShadowCorner(double AValue) {
    MDropShadowCorner = AValue;
  }
  
  virtual void setDropShadowOffset(double x, double y) {
    MDropShadowXOffset = x;
    MDropShadowYOffset = y;
  }
  
  virtual void setDropShadowColors(SAT_Color AIColor, SAT_Color AOColor) {
    MDropShadowIColor = AIColor;
    MDropShadowOColor = AOColor;
  }

  virtual void setDropShadowInner(bool AInner=true) {
    MDropShadowInner = AInner;
  }

  // text

  virtual void setDrawText(bool ADraw=true)               { MDrawText = ADraw; }
  virtual void setTextColor(SAT_Color AColor)             { MTextColor = AColor; }
  virtual void setTextSize(double ASize)                  { MTextSize = ASize; }
  virtual void setTextAlignment(uint32_t AAlign)          { MTextAlignment = AAlign; }
  virtual void setTextOffset(SAT_Rect AOffset)            { MTextOffset = AOffset; }
  
  //virtual void setText(const char* AText)                 { MText = AText; }
  virtual void setText(const char* AText)                 { strcpy(MText,AText); }
  
  virtual void setAutoSize(bool AAuto=true)               { MAutoSize = AAuto; }
  virtual void setAutoTextSize(bool AAuto=true)           { MAutoTextSize = AAuto; }

  virtual void setDrawTextDropShadow(bool ADraw=true)     { MDrawTextDropShadow = ADraw; }
  virtual void setTextDropShadowSize(double ASize)        { MTextDropShadowSize = ASize; }
  virtual void setTextDropShadowColor(SAT_Color AColor)   { MTextDropShadowColor  = AColor; }
  virtual void setTextDropShadowOffset(SAT_Point AOffset) { MTextDropShadowOffset = AOffset; }
  
  //

  virtual const char* getText()     { return MText; }
  virtual double      getTextSize() { return MTextSize; }
  
  virtual SAT_Color   getTextColor()        { return MTextColor; }
  virtual SAT_Color   getBackgroundColor()  { return MBackgroundColor; }

  // value

//  virtual void      setIsInteracting(bool AState=true)          { MIsInteracting = AState; }
  virtual void      setDrawValue(bool ADraw=true)               { MDrawValue = ADraw; }
  virtual void      setValueColor(SAT_Color AColor)             { MValueColor = AColor; }
  virtual void      setValueSize(double ASize)                  { MValueSize = ASize; }
  virtual void      setValueAlignment(uint32_t AAlign)          { MValueAlignment = AAlign; }
  virtual void      setValueOffset(SAT_Rect AOffset)            { MValueOffset = AOffset; }
//  virtual void      setDrawModulation(bool ADraw=true)          { MDrawModulation = ADraw; }
  virtual void      setDrawValueDropShadow(bool ADraw=true)     { MDrawValueDropShadow = ADraw; }
  virtual void      setValueDropShadowSize(double ASize)        { MValueDropShadowSize = ASize; }
  virtual void      setValueDropShadowColor(SAT_Color AColor)   { MValueDropShadowColor  = AColor; }
  virtual void      setValueDropShadowOffset(SAT_Point AOffset) { MValueDropShadowOffset = AOffset; }

//  virtual SAT_Color getDisabledColor()  { return MDisabledColor; }
//  virtual double    getDisabledAlpha()  { return MDisabledAlpha; }
  //virtual bool      getDrawModulation() { return MDrawModulation; }
  virtual bool      getDrawValue()      { return MDrawValue; }
  //virtual SAT_Rect  getValueOffset()    { return MValueOffset; }
  //virtual char*     getValueText()      { return MValueText; }
//  virtual bool      isInteracting()     { return MIsInteracting; }



//------------------------------
public:
//------------------------------

  virtual void drawDropShadow(SAT_PaintContext* AContext) {
    SAT_Assert(AContext);
    if (MDrawDropShadow) {
      double S = AContext->scale;//getWindowScale();
      SAT_BasePainter* painter = AContext->painter;
      SAT_Assert(painter);
      SAT_Rect mrect = getRect();
      //if (MDrawBorder) mrect.shrink(MBorderWidth * 0.5);
      //mrect.overlap(AContext->update_rect);
      if (mrect.w <= 0.0) return;
      if (mrect.h <= 0.0) return;
      SAT_Rect sr = mrect;
      sr.shrink(MDropShadowFeather * (S*0.5));
      //sat_nanovg_paint_t shadow = painter->boxGradient(sr.x,sr.y,sr.w,sr.h,MDropShadowCorner*S,MDropShadowFeather*S,MDropShadowIColor,MDropShadowOColor);
      if (MDropShadowInner) {
        //sr.x -=  MDropShadowXOffset * S;
        //sr.y -=  MDropShadowYOffset * S;
        //sr.w +=  MDropShadowXOffset * S * 2;
        //sr.h +=  MDropShadowYOffset * S * 2;
        sr.w += (MDropShadowFeather*S*0.5);
        sr.h += (MDropShadowFeather*S*0.5);
      }

//      sat_nanovg_paint_t shadow = painter->boxGradient(sr.x,sr.y,sr.w,sr.h,MDropShadowCorner*S,MDropShadowFeather*S,MDropShadowIColor,MDropShadowOColor);

      //painter->setFillPaint(shadow);
      //painter->fillRect(mrect.x,mrect.y,mrect.w,mrect.h);

//      painter->setFillPaint(shadow);

      //painter->fillRect(mrect.x,mrect.y,mrect.w,mrect.h);
      if (MRoundedCorners) {
        painter->fillRoundedRect(mrect.x,mrect.y,mrect.w,mrect.h,MTLCorner*S,MTRCorner*S,MBRCorner*S,MBLCorner*S);
      }
      else {
        painter->fillRect(mrect.x,mrect.y,mrect.w,mrect.h);
      }
    }
  }

  //----------

  virtual void fillBackground(SAT_PaintContext* AContext) {
    SAT_Assert(AContext);
    if (MFillBackground) {
      double S = AContext->scale;//getWindowScale();
      SAT_BasePainter* painter = AContext->painter;
      SAT_Assert(painter);
      SAT_Rect mrect = getRect();

      //if (MDrawBorder) mrect.shrink(MBorderWidth * 0.5);

      //mrect.overlap(AContext->update_rect);
      if (mrect.w <= 0.0) return;
      if (mrect.h <= 0.0) return;
      
      //SAT_Rect sr = mrect;
      if (MDrawDropShadow) {
        mrect.shrink(MDropShadowFeather * (S*0.5));
        mrect.x -= (MDropShadowXOffset*S);
        mrect.y -= (MDropShadowYOffset*S);
      }     

      if (MFillGradient) {
        // down
        painter->setFillGradient(mrect.x,mrect.y,mrect.x,mrect.y2(),MGradientColor1,MGradientColor2);
      }
      else {
        painter->setFillColor(MBackgroundColor);
      }
      
      if (MRoundedCorners) {
        painter->fillRoundedRect(mrect.x,mrect.y,mrect.w,mrect.h,MTLCorner*S,MTRCorner*S,MBRCorner*S,MBLCorner*S);
      }
      else {
        painter->fillRect(mrect.x,mrect.y,mrect.w,mrect.h);
      }
      
    }
  }

  //----------

  virtual void drawBorder(SAT_PaintContext* AContext) {
    SAT_Assert(AContext);
    if (MDrawBorder) {
      double S = AContext->scale;//getWindowScale();
      SAT_BasePainter* painter = AContext->painter;
      SAT_Assert(painter);
      SAT_Rect mrect = getRect();
      
      //mrect.shrink(0.5);

      //if (MDrawBorder) mrect.shrink(MBorderWidth * 0.5);
      
      //if (MDrawDropShadow) {
      //  mrect.shrink(MDropShadowFeather * S);
      //}
      
      if (mrect.w <= 0.0) return;
      if (mrect.h <= 0.0) return;
      
      if (MDrawDropShadow && !MDropShadowInner) {
        mrect.shrink(MDropShadowFeather * (S*0.5));
        mrect.x -= (MDropShadowXOffset*S);
        mrect.y -= (MDropShadowYOffset*S);
      }
      
      painter->setDrawColor(MBorderColor);
      painter->setLineWidth(MBorderWidth * S);
      
      if (MRoundedCorners) {
        painter->drawRoundedRect(mrect.x,mrect.y,mrect.w,mrect.h,MTLCorner*S,MTRCorner*S,MBRCorner*S,MBLCorner*S);
      }
      else {
        //painter->drawRect(mrect.x,mrect.y,mrect.w,mrect.h);
        if (MBorderEdges & SAT_EDGE_TOP)    painter->drawLine( mrect.x,    mrect.y,    mrect.x2(), mrect.y    );
        if (MBorderEdges & SAT_EDGE_BOTTOM) painter->drawLine( mrect.x,    mrect.y2(), mrect.x2(), mrect.y2() );
        if (MBorderEdges & SAT_EDGE_LEFT)   painter->drawLine( mrect.x,    mrect.y,    mrect.x,    mrect.y2() );
        if (MBorderEdges & SAT_EDGE_RIGHT)  painter->drawLine( mrect.x2(), mrect.y,    mrect.x2(), mrect.y2() );
      }
      
      
    }
  }

  //----------

  virtual void drawText(SAT_PaintContext* AContext) {
    SAT_Assert(AContext);
    if (MDrawText) {
      
      // if MOwner == null, this will return 1.0
      double S = AContext->scale; // getWindowScale();
      
      //SAT_Print("S %f\n",S);
      SAT_BasePainter* painter = AContext->painter;
      SAT_Assert(painter);
      SAT_Rect mrect = getRect();
      SAT_Rect textoffset = MTextOffset;
      textoffset.scale(S);
      mrect.shrink(textoffset);
      if (mrect.w <= 0.0) return;
      if (mrect.h <= 0.0) return;
      double textsize = MTextSize * S;

//      painter->setTextColor(MTextColor);
//      painter->setTextSize(textsize); // try original..
      
      // if autosize is true, the text will be scaled up as much as
      // possible to fit inside the rect..

      if (MAutoTextSize) {
        double bounds[4];
        //if (painter->getTextBounds(MText,bounds)) {
        //  //SAT_Print("%.2f, %.2f - %.2f, %.2f\n",bounds[0],bounds[1],bounds[2],bounds[3]);
          painter->getTextBounds(MText,bounds);
          double width = bounds[2] - bounds[0];
          double height = bounds[3] - bounds[1];
          double wratio = 1.0;
          double hratio = 1.0;
          if (width > 0)  wratio = mrect.w / width;
          if (height > 0) hratio = mrect.h / height;
          if (wratio < hratio) textsize *= wratio;
          else textsize *= hratio;
          //SAT_Print("mrect.w %.2f mrect.h %.2f wratio %.2f hratio %.2f\n",mrect.w,mrect.h,wratio,hratio);
//          painter->setTextSize(textsize);
        //}
      }
      
//      SAT_Print("textsize %f winscale %f\n",textsize,S); // prints textsize: 8, winscale: 1.0
//      SAT_Print("textsize %f winscale %f owner %p\n",textsize,S,getOwner()); // prints textsize: 8, winscale: 1.0
      
      const char* text = MText;
      painter->setTextSize(textsize);
      
      SAT_Parameter* param = (SAT_Parameter*)getParameter();
      if (param) {
        // drop shadow
        //painter->drawTextBox(mrect,param->getName(),MTextAlignment);
        if (MDrawParamText) text = param->getName();
        SAT_Point p = painter->getTextPos(mrect,text,MTextAlignment);
        if (MDrawTextDropShadow) {
          painter->setFontBlur(MTextDropShadowSize);
          painter->setTextColor(MTextDropShadowColor);
          painter->drawText(p.x + MTextDropShadowOffset.x, p.y + MTextDropShadowOffset.y, text);
          painter->setFontBlur(0);
        }
        
        painter->setTextColor(MTextColor);
        painter->drawText(p.x,p.y,text);
        
      }
      else {
        // drop shadow
        //painter->drawTextBox(mrect,MText,MTextAlignment);
        SAT_Point p = painter->getTextPos(mrect,MText,MTextAlignment);
        if (MDrawTextDropShadow) {
          painter->setFontBlur(MTextDropShadowSize);
          painter->setTextColor(MTextDropShadowColor);
          painter->drawText(p.x + MTextDropShadowOffset.x, p.y + MTextDropShadowOffset.y, MText);
          painter->setFontBlur(0);
        }
        painter->setTextColor(MTextColor);
        painter->drawText(p.x,p.y,text);
      }
      
    }
  }

  //----------

  virtual void drawValue(SAT_PaintContext* AContext) {
    SAT_Assert(AContext);
    if (MDrawValue) {
      // if MOwner == null, this will return 1.0
      double S = AContext->scale;//getWindowScale();
      SAT_BasePainter* painter = AContext->painter;
      SAT_Assert(painter);
      SAT_Rect mrect = getRect();
      SAT_Rect valueoffset = MValueOffset;
      valueoffset.scale(S);
      mrect.shrink(valueoffset);
      if (mrect.w <= 0.0) return;
      if (mrect.h <= 0.0) return;
      SAT_Parameter* param = (SAT_Parameter*)getParameter();
      if (param) {

        double pv = getValue();
        //double pv = getSelectedValue();

        double dpv = param->denormalize(pv);
        //double wv = param->getValue();
        //double dwv = param->denormalizeValue(wv);
        //SAT_Print("denormalize. param %f -> dpv %f. widget %i ->  dwv %f\n",pv,dpv,wv,dwv);
        param->valueToText(dpv);
      }
      else {

        double v = getValue();
        //double v = getSelectedValue();

        sprintf(MValueText,"%.2f",v);
      }
      
      painter->setTextSize(MValueSize*S);
      SAT_Point p = painter->getTextPos(mrect,MValueText,MValueAlignment);

      if (MDrawValueDropShadow) {
        painter->setFontBlur(MValueDropShadowSize);
        painter->setTextColor(MValueDropShadowColor);
        painter->drawText(p.x + MValueDropShadowOffset.x, p.y + MValueDropShadowOffset.y, MValueText);
        painter->setFontBlur(0);
      }
      
      painter->setTextColor(MValueColor);
      painter->drawText(p.x, p.y, MValueText);
      
    }
  }

  //----------

  virtual void drawHostIndicators(SAT_PaintContext* AContext) {
    SAT_Assert(AContext);
    
    SAT_BasePainter* painter = AContext->painter;
    SAT_Assert(painter);
    double S = AContext->scale;//getWindowScale();
    SAT_Rect mrect = getRect();
    
    SAT_Parameter* param = (SAT_Parameter*)getParameter();
    if (param) {

      uint32_t state = param->getAutomationState();
      if (state != CLAP_PARAM_INDICATION_AUTOMATION_NONE) {
        //SAT_PRINT;
        double x = mrect.x2() - (4.0 * S);
        double y = mrect.y + (4.0 * S);
        SAT_Color color = param->getAutomationColor();
        painter->setFillColor(color);
        painter->fillCircle(x,y,3*S);
      }
      
      bool mapped = param->getIsMapped();
      if (mapped) {
        //SAT_PRINT;
        
      //  double x = mrect.x + (4.0 * S);
      //  double y = mrect.y + (4.0 * S);
      //  SAT_Color color = param->getMappedColor();
      //  painter->setFillColor(color);
      //  painter->fillCircle(x,y,3*S); // TODO: draw triangle
        
        double coords[] = {
          mrect.x + (1.0 * S),        mrect.y + (1.0 * S),
          mrect.x + (5.0 * S),        mrect.y + (1.0 * S),
          mrect.x + (1.0 * S),        mrect.y + (5.0 * S),
          mrect.x + (1.0 * S),        mrect.y + (1.0 * S)
        };
        SAT_Color color = param->getMappedColor();
        //painter->setLineWidth(MPenWidth * S);
        painter->setFillColor(color);
        painter->fillLineStrip(4,coords);
        
      }
      
    }
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(SAT_PaintContext* AContext) override {
    SAT_Print("%s : counter %i w/h %i,%i\n",getName(),AContext->counter,getWidth(),getHeight());
    drawDropShadow(AContext);
    fillBackground(AContext);
    drawText(AContext);
    drawValue(AContext);
    paintChildWidgets(AContext);
    drawBorder(AContext);
    drawHostIndicators(AContext);
  }
  
};

//----------------------------------------------------------------------
#endif