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

  SAT_Rect  MMappedIndicatorOffset        = SAT_Rect();
  SAT_Rect  MAutomationIndicatorOffset    = SAT_Rect();

  // background
  
  bool      MFillBackground               = true;
  SAT_Color MBackgroundColor              = SAT_DarkGrey;
  SAT_Color MIBackgroundColor             = SAT_DarkCyan2;

  // gradient
  
  bool      MFillGradient                 = false;
  SAT_Color MGradientColor1               = SAT_LightGrey;
  SAT_Color MGradientColor2               = SAT_DarkGrey;
  double    MGradientFactor1              = 0.2;
  double    MGradientFactor2              = 0.2;

  // diaabled
  SAT_Color MDisabledColor                = SAT_DarkGrey;
  double    MDisabledFactor               = 0.6;

  // border
  
  bool        MDrawBorder                 = true;
  SAT_Color   MBorderColor                = SAT_DarkerGrey;
  SAT_Color   MIBorderColor               = SAT_DarkGrey;
  double      MBorderWidth                = 1.0;
  uint32_t    MBorderEdges                = SAT_EDGE_ALL;
  uint32_t    MBorderCorners              = SAT_CORNER_NONE;

  // rounded corners
  
  bool        MRoundedCorners             = false;
  double      MTLCorner                   = 0.0;
  double      MTRCorner                   = 0.0;
  double      MBRCorner                   = 0.0;
  double      MBLCorner                   = 0.0;

  // drop shadow
  
  bool        MDrawDropShadow             = false;
  double      MDropShadowFeather          = 10.0;
  double      MDropShadowCorner           = 0.0;
  double      MDropShadowXOffset          = 0.0;
  double      MDropShadowYOffset          = 0.0;
  SAT_Color   MDropShadowIColor           = SAT_Color(0.0, 0.0, 0.0, 0.5 );
  SAT_Color   MDropShadowOColor           = SAT_Color(0.33, 0.33, 0.33, 0.5 );
  bool        MDropShadowInner            = false;

  double      MDropShadowXOffsetTMP       = 0.0;
  double      MDropShadowYOffsetTMP       = 0.0;

  // text

  bool        MDrawText                   = true;
  SAT_Color   MTextColor                  = SAT_White;
  double      MTextSize                   = 12.0;
  uint32_t    MTextAlignment              = SAT_TEXT_ALIGN_CENTER;
  SAT_Rect    MTextOffset                 = {};
  char        MText[1024]                 = {0};
  int32_t     MTextFont                   = -1;
  
  bool        MAutoSize                   = false;
  SAT_Rect    MAutoSizeBorder             = SAT_Rect(0,0,0,0);
  bool        MAutoTextSize               = false;
  bool        MDrawParamText              = false;

  bool        MDrawTextDropShadow         = false;
  double      MTextDropShadowSize         = 4.0;
  SAT_Color   MTextDropShadowColor        = SAT_Black;
  SAT_Point   MTextDropShadowOffset       = {2,2};

  // value

  bool        MDrawValue                  = false;
  SAT_Color   MValueColor                 = SAT_White;
  double      MValueSize                  = 12.0;
  uint32_t    MValueAlignment             = SAT_TEXT_ALIGN_RIGHT;
  SAT_Rect    MValueOffset                = {0,0,0,0};
  char        MValueText[256]             = {0};

  bool        MDrawValueDropShadow        = false;
  double      MValueDropShadowSize        = 4.0;
  SAT_Color   MValueDropShadowColor       = SAT_Black;
  SAT_Point   MValueDropShadowOffset      = {2,2};

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

  virtual void setFillLinearGradient(bool AGradient=true) {
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
  
  virtual void setGradientColorsInv(SAT_Color AColor, double AFactor1, double AFactor2) {
    MGradientFactor1 = AFactor1;
    MGradientFactor2 = AFactor2;
    MGradientColor1 = AColor;
    MGradientColor2 = AColor;
    MGradientColor1.blend(SAT_Black,AFactor1);
    MGradientColor2.blend(SAT_White,AFactor2);
  }
  
  virtual void setGradientColors(SAT_Color AColor) {
    MGradientColor1 = AColor;
    MGradientColor2 = AColor;
    MGradientColor1.blend(SAT_White,MGradientFactor1);
    MGradientColor2.blend(SAT_Black,MGradientFactor2);
  }
  
  virtual void setGradientColorsInv(SAT_Color AColor) {
    MGradientColor1 = AColor;
    MGradientColor2 = AColor;
    MGradientColor1.blend(SAT_Black,MGradientFactor1);
    MGradientColor2.blend(SAT_White,MGradientFactor2);
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
  
  virtual void setText(const char* AText)                 { strcpy(MText,AText); }
  virtual void setTextFont(int32_t AFont)                 { MTextFont = AFont; }
  
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

  virtual void      setDrawValue(bool ADraw=true)               { MDrawValue = ADraw; }
  virtual void      setValueColor(SAT_Color AColor)             { MValueColor = AColor; }
  virtual void      setValueSize(double ASize)                  { MValueSize = ASize; }
  virtual void      setValueAlignment(uint32_t AAlign)          { MValueAlignment = AAlign; }
  virtual void      setValueOffset(SAT_Rect AOffset)            { MValueOffset = AOffset; }
  virtual void      setDrawValueDropShadow(bool ADraw=true)     { MDrawValueDropShadow = ADraw; }
  virtual void      setValueDropShadowSize(double ASize)        { MValueDropShadowSize = ASize; }
  virtual void      setValueDropShadowColor(SAT_Color AColor)   { MValueDropShadowColor  = AColor; }
  virtual void      setValueDropShadowOffset(SAT_Point AOffset) { MValueDropShadowOffset = AOffset; }

  virtual bool      getDrawValue()      { return MDrawValue; }



//------------------------------
public:
//------------------------------

  virtual void drawDropShadow(SAT_PaintContext* AContext) {
    SAT_Assert(AContext);
    if (MDrawDropShadow) {
      double S = AContext->scale;
      SAT_Painter* painter = AContext->painter;
      SAT_Assert(painter);
      SAT_Rect mrect = getRect();
      if (mrect.w <= 0.0) return;
      if (mrect.h <= 0.0) return;
      SAT_Rect sr = mrect;
      sr.shrink(MDropShadowFeather * (S*0.5));

      SAT_Color icolor = MDropShadowIColor;
      SAT_Color ocolor = MDropShadowOColor;
      if (!isEnabled()) {
        icolor.blend(MDisabledColor,MDisabledFactor);
        ocolor.blend(MDisabledColor,MDisabledFactor);
      }
      
      sat_paint_t shadow;
      if (MDropShadowInner) {
        sr.w += (MDropShadowFeather*S*0.5);
        sr.h += (MDropShadowFeather*S*0.5);
        shadow = painter->boxGradient(sr.x,sr.y,sr.w,sr.h,MDropShadowCorner*S,MDropShadowFeather*S,ocolor,icolor);//MDropShadowOColor,MDropShadowIColor);
      }
      else {
        shadow = painter->boxGradient(sr.x,sr.y,sr.w,sr.h,MDropShadowCorner*S,MDropShadowFeather*S,icolor,ocolor);//MDropShadowIColor,MDropShadowOColor);
      }
      painter->setFillPaint(shadow);

      if (MRoundedCorners) painter->fillRoundedRect(mrect.x,mrect.y,mrect.w,mrect.h,MTLCorner*S,MTRCorner*S,MBRCorner*S,MBLCorner*S);
      else painter->fillRect(mrect.x,mrect.y,mrect.w,mrect.h);
    }
  }

  //----------

  virtual void fillBackground(SAT_PaintContext* AContext) {
    SAT_Assert(AContext);
    if (MFillBackground) {
      double S = AContext->scale;
      SAT_Painter* painter = AContext->painter;
      SAT_Assert(painter);
      SAT_Rect mrect = getRect();
      if (mrect.w <= 0.0) return;
      if (mrect.h <= 0.0) return;
      if (MDrawDropShadow) {
        mrect.shrink(MDropShadowFeather * (S*0.5));
        mrect.x -= (MDropShadowXOffset*S);
        mrect.y -= (MDropShadowYOffset*S);
      }

      if (isEnabled()) {
        if (MFillGradient) {
          SAT_Color c1 = MGradientColor1;
          SAT_Color c2 = MGradientColor2;
          painter->setFillLinearGradient(mrect.x,mrect.y,mrect.x,mrect.y2(),c1,c2);
        }
        else {
          SAT_Color bc = MBackgroundColor;
          painter->setFillColor(bc);
        }
      }
      else {
        if (MFillGradient) {
          SAT_Color c1 = MGradientColor1;
          SAT_Color c2 = MGradientColor2;
          c1.blend(MDisabledColor,MDisabledFactor);
          c2.blend(MDisabledColor,MDisabledFactor);
          painter->setFillLinearGradient(mrect.x,mrect.y,mrect.x,mrect.y2(),c1,c2);
        }
        else {
          SAT_Color bc = MBackgroundColor;
          bc.blend(MDisabledColor,MDisabledFactor);
          painter->setFillColor(bc);
        }
      }

      if (MRoundedCorners) painter->fillRoundedRect(mrect.x,mrect.y,mrect.w,mrect.h,MTLCorner*S,MTRCorner*S,MBRCorner*S,MBLCorner*S);
      else painter->fillRect(mrect.x,mrect.y,mrect.w,mrect.h);
    }
  }

  //----------

  virtual void drawBorder(SAT_PaintContext* AContext) {
    SAT_Assert(AContext);
    if (MDrawBorder) {
      double S = AContext->scale;
      SAT_Painter* painter = AContext->painter;
      SAT_Assert(painter);
      SAT_Rect mrect = getRect();
      if (mrect.w <= 0.0) return;
      if (mrect.h <= 0.0) return;
      if (MDrawDropShadow && !MDropShadowInner) {
        mrect.shrink(MDropShadowFeather * (S*0.5));
        mrect.x -= (MDropShadowXOffset*S);
        mrect.y -= (MDropShadowYOffset*S);
      }

      SAT_Color color = MBorderColor;
      if (!isEnabled()) color.blend(MDisabledColor,MDisabledFactor);
      painter->setDrawColor(color);

      painter->setLineWidth(MBorderWidth * S);
      if (MRoundedCorners) {
        painter->drawRoundedRect(mrect.x,mrect.y,mrect.w,mrect.h,MTLCorner*S,MTRCorner*S,MBRCorner*S,MBLCorner*S);
      }
      else {
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
      double S = AContext->scale;
      SAT_Painter* painter = AContext->painter;
      SAT_Assert(painter);
      SAT_Rect mrect = getRect();
      SAT_Rect textoffset = MTextOffset;
      textoffset.scale(S);
      mrect.shrink(textoffset);
      if (mrect.w <= 0.0) return;
      if (mrect.h <= 0.0) return;
      double textsize = MTextSize * S;
      if (MAutoTextSize) {
        double bounds[4];
        painter->getTextBounds(MText,bounds);
        double width = bounds[2] - bounds[0];
        double height = bounds[3] - bounds[1];
        double wratio = 1.0;
        double hratio = 1.0;
        if (width > 0)  wratio = mrect.w / width;
        if (height > 0) hratio = mrect.h / height;
        if (wratio < hratio) textsize *= wratio;
        else textsize *= hratio;
      }
      const char* text = MText;
      if (MTextFont != -1) painter->selectFont(MTextFont);
      painter->setTextSize(textsize);
      SAT_Parameter* param = (SAT_Parameter*)getParameter();
      if (param) {
        // drop shadow
        if (MDrawParamText) text = param->getName();
        SAT_Point p = painter->getTextPos(mrect,text,MTextAlignment);
        if (MDrawTextDropShadow) {
          painter->setFontBlur(MTextDropShadowSize);
          SAT_Color color = MTextDropShadowColor;
          if (!isEnabled()) color.blend(MDisabledColor,MDisabledFactor);
          painter->setTextColor(color);
          painter->drawText(p.x + MTextDropShadowOffset.x, p.y + MTextDropShadowOffset.y, text);
          painter->setFontBlur(0);
        }

        SAT_Color color = MTextColor;
        if (!isEnabled()) color.blend(MDisabledColor,MDisabledFactor);
        painter->setTextColor(color);

        painter->drawText(p.x,p.y,text);
      }
      else {
        // drop shadow
        SAT_Point p = painter->getTextPos(mrect,MText,MTextAlignment);
        if (MDrawTextDropShadow) {
          painter->setFontBlur(MTextDropShadowSize);
          SAT_Color color = MTextDropShadowColor;
          if (!isEnabled()) color.blend(MDisabledColor,MDisabledFactor);
          painter->setTextColor(color);
          painter->drawText(p.x + MTextDropShadowOffset.x, p.y + MTextDropShadowOffset.y, MText);
          painter->setFontBlur(0);
        }
        SAT_Color color = MTextColor;
        if (!isEnabled()) color.blend(MDisabledColor,MDisabledFactor);
        painter->setTextColor(color);
        painter->drawText(p.x,p.y,text);
      }
      if (MTextFont != -1) {
        int32_t default_font = painter->getDefaultFont();
        painter->selectFont(default_font);
      }
    }
  }

  //----------

  virtual void drawValue(SAT_PaintContext* AContext) {
    SAT_Assert(AContext);
    if (MDrawValue) {
      double S = AContext->scale;
      SAT_Painter* painter = AContext->painter;
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
        const char* txt = param->valueToText(pv);
        strcpy(MValueText,txt);
      }
      else {
        double v = getValue();
        sprintf(MValueText,"%.2f",v);
      }
      painter->setTextSize(MValueSize*S);
      SAT_Point p = painter->getTextPos(mrect,MValueText,MValueAlignment);
      if (MDrawValueDropShadow) {
        painter->setFontBlur(MValueDropShadowSize);
          SAT_Color color = MValueDropShadowColor;
          if (!isEnabled()) color.blend(MDisabledColor,MDisabledFactor);
        painter->setTextColor(color);
        painter->drawText(p.x + MValueDropShadowOffset.x, p.y + MValueDropShadowOffset.y, MValueText);
        painter->setFontBlur(0);
      }
      SAT_Color color = MValueColor;
      if (!isEnabled()) color.blend(MDisabledColor,MDisabledFactor);
      painter->setTextColor(color);
      painter->drawText(p.x, p.y, MValueText);
    }
  }

  //----------

  // todo: if (MDrawHostIndicators)

  virtual void drawHostIndicators(SAT_PaintContext* AContext) {
    SAT_Assert(AContext);
    SAT_Painter* painter = AContext->painter;
    SAT_Assert(painter);
    double S = AContext->scale;
    SAT_Rect mrect = getRect();
    SAT_Parameter* param = (SAT_Parameter*)getParameter();
    if (param) {
      uint32_t state = param->getAutomationState();
      if (state != CLAP_PARAM_INDICATION_AUTOMATION_NONE) {
        double x = mrect.x2() - (4.0 * S);
        double y = mrect.y + (4.0 * S);
        SAT_Color color = param->getAutomationColor();
        painter->setFillColor(color);
        painter->fillCircle(x,y,3*S);
      }
      bool mapped = param->getIsMapped();
      if (mapped) {
        double coords[] = {
          mrect.x + (1.0 * S),        mrect.y + (1.0 * S),
          mrect.x + (5.0 * S),        mrect.y + (1.0 * S),
          mrect.x + (1.0 * S),        mrect.y + (5.0 * S),
          mrect.x + (1.0 * S),        mrect.y + (1.0 * S)
        };
        SAT_Color color = param->getMappedColor();
        painter->setFillColor(color);
        painter->fillLineStrip(4,coords);
      }
    }
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(SAT_PaintContext* AContext) override {
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