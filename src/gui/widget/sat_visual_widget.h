#ifndef sat_visual_widget_included
#define sat_visual_widget_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/widget/sat_widget_listener.h"
#include "gui/sat_widget.h"

//----------

class SAT_VisualWidget
: public SAT_Widget {

//------------------------------
private:
//------------------------------

//uint32_t    MCursor           = SAT_CURSOR_DEFAULT;
//const char* MHint             = "";

  bool        MFillBackground   = true;
  bool        MDrawBorder       = true;

  SAT_Color   MBackgroundColor  = SAT_Grey;
  SAT_Color   MBorderColor      = SAT_Black;
  double      MBorderWidth      = 1.0;

  bool        MDrawIndicators   = true;

  // drop shadow
  
  bool        MDrawDropShadow             = false;
  bool        MDropShadowInner            = false;
  double      MDropShadowFeather          = 10.0;
  SAT_Color   MDropShadowIColor           = SAT_Color(0.0, 0.0, 0.0, 0.5 );
  SAT_Color   MDropShadowOColor           = SAT_Color(0.33, 0.33, 0.33, 0.5 );
  double      MDropShadowCorner           = 0.0;
  double      MDropShadowXOffset          = 0.0;
  double      MDropShadowYOffset          = 0.0;
  // double      MDropShadowXOffsetTMP       = 0.0;
  // double      MDropShadowYOffsetTMP       = 0.0;

//------------------------------
public:
//------------------------------

  SAT_VisualWidget(SAT_Rect ARect)
  : SAT_Widget(ARect) {
    setName("SAT_VisualWidget");

    //Options.autoClip = true;

  }

  //----------

  virtual ~SAT_VisualWidget() {
  }

//------------------------------
public:
//------------------------------

  void setFillBackground(bool AFill=true)   { MFillBackground = AFill; }
  void setBackgroundColor(SAT_Color AColor) { MBackgroundColor = AColor; }

  void setDrawBorder(bool ADraw=true)       { MDrawBorder = ADraw; }
  void setBorderColor(SAT_Color AColor)     { MBorderColor = AColor; }
  void setBorderWidth(double AWidth)        { MBorderWidth = AWidth; }

  void setDrawIndicators(bool ADraw)        { MDrawIndicators = ADraw; }

//------------------------------
public:
//------------------------------

//------------------------------
public:
//------------------------------

  /*
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
  */

  virtual void fillBackground(SAT_PaintContext* AContext) {
    if (MFillBackground) {
      SAT_Painter* painter = AContext->painter;
      SAT_Rect rect = getRect();
      //SAT_PRINT("%.2f, %.2f, %.2f, %.2f\n",rect.x,rect.y,rect.w,rect.h);
      painter->setFillColor(MBackgroundColor);
      painter->fillRect(rect.x,rect.y,rect.w,rect.h);
    }
  }

  //----------

  /*
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
  */

  virtual void drawBorder(SAT_PaintContext* AContext) {
    if (MDrawBorder) {
      //SAT_TRACE;
      SAT_Painter* painter = AContext->painter;
      SAT_Rect rect = getRect();
      double scale = getWindowScale();
      painter->setDrawColor(MBorderColor);
      painter->setLineWidth(MBorderWidth*scale);
      painter->drawRect(rect.x,rect.y,rect.w,rect.h);
    }
  }

  //----------

  /*
  virtual void drawBevel(SAT_PaintContext* AContext) {
    SAT_Assert(AContext);
    if (MDrawBevel) {
      double S = AContext->scale;
      SAT_Painter* painter = AContext->painter;
      SAT_Assert(painter);
      SAT_Rect mrect = getRect();
      if (mrect.w <= 0.0) return;
      if (mrect.h <= 0.0) return;
      //mrect.x -= MBorderWidth;
      //mrect.y -= MBorderWidth;
      //mrect.w += MBorderWidth;
      //mrect.h += MBorderWidth;
      // if (MDrawDropShadow && !MDropShadowInner) {
      //   mrect.shrink(MDropShadowFeather * (S*0.5));
      //   mrect.x -= (MDropShadowXOffset*S);
      //   mrect.y -= (MDropShadowYOffset*S);
      // }
      painter->setLineWidth(MBevelWidth * S);
      SAT_Color color;
      if (MRoundedCorners) {
        double x1 = mrect.x;
        double y1 = mrect.y;
        double x2 = mrect.x2();
        double y2 = mrect.y2();
        double tl = MTLCorner * S;
        double tr = MTRCorner * S;
        double br = MBRCorner * S;
        double bl = MBLCorner * S;
        double bx1 = x1 + tl;
        double by1 = y1 + tl;
        double bx2 = x2 - tr;
        double by2 = y1 + tr;
        double bx3 = x1 + bl;
        double by3 = y2 - bl;
        double bx4 = x2 - br;
        double by4 = y2 - br;
        // 12
        // 34
        if (MBevelInset) color = MBevelLight;
        else color = MBevelDark;
        if (!isEnabled()) color.blend(MDisabledColor,MDisabledFactor);
        painter->setDrawColor(color);
        painter->drawLine( bx3, y2,  bx4, y2  ); // bottom, 3-4
        painter->drawLine( x2,  by2, x2,  by4 ); // right,  2-4
        painter->drawArc( bx2,by2, br, 0.875*SAT_PI2, 1.000*SAT_PI2); // half of top-right
        painter->drawArc( bx4,by4, br, 0.000*SAT_PI2, 0.250*SAT_PI2); // bottom right
        painter->drawArc( bx3,by3, br, 0.250*SAT_PI2, 0.375*SAT_PI2); // half of bottom-left
        if (MBevelInset) color = MBevelDark;
        else color = MBevelLight;
        if (!isEnabled()) color.blend(MDisabledColor,MDisabledFactor);
        painter->setDrawColor(color);
        painter->drawLine( bx1, y1,  bx2, y1  ); // top,    1-2
        painter->drawLine( x1,  by1, x1,  by3 ); // left,   1-3
        painter->drawArc( bx3,by3, br, 0.375*SAT_PI2, 0.500*SAT_PI2); // half of bottom-left
        painter->drawArc( bx1,by1, br, 0.500*SAT_PI2, 0.750*SAT_PI2); // top-left
        painter->drawArc( bx2,by2, br, 0.750*SAT_PI2, 0.875*SAT_PI2); // half of top-right
      }
      else {
        if (MBevelInset) color = MBevelLight;        
        else color = MBevelDark;
        if (!isEnabled()) color.blend(MDisabledColor,MDisabledFactor);
        painter->setDrawColor(color);
        painter->drawLine( mrect.x,    mrect.y2(), mrect.x2(), mrect.y2() );
        painter->drawLine( mrect.x2(), mrect.y,    mrect.x2(), mrect.y2() );
        if (MBevelInset) color = MBevelDark;
        else color = MBevelLight;
        if (!isEnabled()) color.blend(MDisabledColor,MDisabledFactor);
        painter->setDrawColor(color);
        painter->drawLine( mrect.x,    mrect.y,    mrect.x2(), mrect.y    );
        painter->drawLine( mrect.x,    mrect.y,    mrect.x,    mrect.y2() );
      }
    }
  }
  */

  // void drawBevel(SAT_PaintContext* AContext) {
  //   double S = AContext->scale;
  //   SAT_Painter* painter = AContext->painter;
  //   SAT_Assert(painter);
  //   SAT_Rect mrect = getRect();
  //   if (mrect.w <= 0.0) return;
  //   if (mrect.h <= 0.0) return;
  //   mrect.shrink(1);
  //   painter->bndBevelInset(mrect.x,mrect.y,mrect.w,mrect.h,1,1);
  //   painter->bndBevel(mrect.x,mrect.y,mrect.w,mrect.h);
  // }

  //----------

  virtual void drawDropShadow(SAT_PaintContext* AContext) {
    SAT_Assert(AContext);
    if (MDrawDropShadow) {
      double S = getWindowScale(); // AContext->scale;
      SAT_Painter* painter = AContext->painter;
      SAT_Assert(painter);
      SAT_Rect mrect = getRect();
      if (mrect.w <= 0.0) return;
      if (mrect.h <= 0.0) return;
      SAT_Rect sr = mrect;
      sr.shrink(MDropShadowFeather * (S*0.5));
      SAT_Color icolor = MDropShadowIColor;
      SAT_Color ocolor = MDropShadowOColor;
      // if (State.disabled) {
      //   icolor.blend(MDisabledColor,MDisabledFactor);
      //   ocolor.blend(MDisabledColor,MDisabledFactor);
      // }
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

      /*if (MRoundedCorners) painter->fillRoundedRect(mrect.x,mrect.y,mrect.w,mrect.h,MTLCorner*S,MTRCorner*S,MBRCorner*S,MBLCorner*S);
      else*/
      painter->fillRect(mrect.x,mrect.y,mrect.w,mrect.h);
    }
  }

  //----------



  //----------

  /*
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
        if (!isEnabled()) color.blend(MDisabledColor,MDisabledFactor);
        painter->setFillColor(color);
        painter->fillCircle(x,y,3*S);
      }
      bool mapped = param->getIsMapped();
      if (mapped) {
        double coords[] = {
          mrect.x + (1.0 * S),  mrect.y + (1.0 * S),
          mrect.x + (5.0 * S),  mrect.y + (1.0 * S),
          mrect.x + (1.0 * S),  mrect.y + (5.0 * S),
          mrect.x + (1.0 * S),  mrect.y + (1.0 * S)
        };
        SAT_Color color = param->getMappedColor();
        if (!isEnabled()) color.blend(MDisabledColor,MDisabledFactor);
        painter->setFillColor(color);
        painter->fillLineStrip(4,coords);
      }
    }
  }
  */

  virtual void drawIndicators(SAT_PaintContext* AContext) {
    if (MDrawIndicators) {
      //SAT_TRACE;
      SAT_Parameter* param = (SAT_Parameter*)getParameter();
      if (param) {
        SAT_Painter* painter = AContext->painter;
        SAT_Rect rect = getRect();
        double scale = getWindowScale();

        if (param->getIndicateMapped()) {
            SAT_Color color = param->getIndicateMappedColor();
            painter->setFillColor(color);
            //painter->drawRect(rect.x,rect.y,rect.w,rect.h);
        }
        // switch (param->getIndicateAutomationState() ) {
        //   case CLAP_PARAM_INDICATION_AUTOMATION_NONE: // The host doesn't have an automation for this parameter
        //   case CLAP_PARAM_INDICATION_AUTOMATION_PRESENT: // The host has an automation for this parameter, but it isn't playing it
        //   case CLAP_PARAM_INDICATION_AUTOMATION_PLAYING: // The host is playing an automation for this parameter
        //   case CLAP_PARAM_INDICATION_AUTOMATION_RECORDING: // The host is recording an automation on this parameter
        //   case CLAP_PARAM_INDICATION_AUTOMATION_OVERRIDING: // The host should play an automation for this parameter, but the user has started to adjust this parameter and is overriding the automation playback
        // }
        if (param->getIndicateAutomationState() != CLAP_PARAM_INDICATION_AUTOMATION_NONE) {
          SAT_Color color = param->getIndicateAutomationColor();
          painter->setDrawColor(color);
          //painter->drawRect(rect.x,rect.y,rect.w,rect.h);
        }
      }
    }
  }

//------------------------------
public: // on_widget
//------------------------------

  // void on_widget_open(SAT_WidgetOwner* AOwner) override {
  //   MSurface = new SAT_Surface(100,100,0);
  //   SAT_PRINT("MSurface: %p\n",MSurface);
  // }

  // void on_widget_close(SAT_WidgetOwner* AOwner) override {
  //   SAT_PRINT("MSurface: %p\n",MSurface);
  //   if (MSurface) delete MSurface;
  // }

  void on_widget_paint(SAT_PaintContext* AContext) override {
    //SAT_TRACE;
    drawDropShadow(AContext);
    fillBackground(AContext);
    paintChildren(AContext);
    drawIndicators(AContext);
    drawBorder(AContext);
  }

};

//----------------------------------------------------------------------
#endif