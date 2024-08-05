#ifndef sat_visual_widget_included
#define sat_visual_widget_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/base/sat_widget_listener.h"
#include "gui/sat_widget.h"
#include "plugin/sat_parameter.h"

//----------

class SAT_VisualWidget
: public SAT_Widget {

//------------------------------
private:
//------------------------------

  bool        MFillBackground   = true;
  bool        MDrawBorder       = true;

  SAT_Color   MBackgroundColor  = SAT_Grey;
  SAT_Color   MBorderColor      = SAT_Black;
  double      MBorderWidth      = 1.0;

  bool        MDrawIndicators   = true;

  bool        MFillGradient               = false;
  SAT_Color   MGradientColor1             = SAT_Color(0.6, 0.6, 0.6, 1.0 );
  SAT_Color   MGradientColor2             = SAT_Color(0.4, 0.4, 0.4, 1.0 );

  bool        MDrawDropShadow             = false;
  bool        MDropShadowInner            = false;
  double      MDropShadowFeather          = 5.0;
  SAT_Color   MDropShadowInnerColor       = SAT_Color(0.0, 0.0, 0.0, 0.4 );
  SAT_Color   MDropShadowOuterColor       = SAT_Color(0.5, 0.5, 0.5, 0.4 );
  double      MDropShadowCorner           = 0.0;
//SAT_Point   MDropShadowOffset           = {0,0};

  bool        MDrawRoundedCorners         = false;
  uint32_t    MRoundedCorners             = SAT_CORNER_ALL;
  double      MRoundedCornerSize          = 5.0;

  double      MMappedIndicatorSize        = 8.0;
  double      MAutomationIndicatorSize    = 4.0;

  //

  bool      MSizable            = false;
  SAT_Rect  MSizableBorder      = SAT_Rect(5,5,5,5);
  uint32_t  MSizableEdges       = SAT_EDGE_ALL;
  uint32_t  MSizableFlags       = 0; // not used.. (todo: limit_to_parent)

  bool      MMovable            = false;
  SAT_Rect  MMovableOffset      = SAT_Rect(0,0,0,0);
  uint32_t  MMovableDirections  = SAT_DIRECTION_ALL;
  uint32_t  MMovableFlags       = 0; // 1 = invert_offset, (todo: limit_to_parent)

  //private

  bool      MCanMove            = false;
  uint32_t  MResizeEdge         = SAT_EDGE_NONE;
  bool      MMoving             = false;
  bool      MResizing           = false;
  int32_t   MMouseClickedX      = 0;
  int32_t   MMouseClickedY      = 0;
  int32_t   MMousePreviousX     = 0;
  int32_t   MMousePreviousY     = 0;

  double    MPrevScale          = 1.0;



//------------------------------
public:
//------------------------------

  SAT_VisualWidget(SAT_Rect ARect)
  : SAT_Widget(ARect) {
    setName("SAT_VisualWidget");
    setHint("SAT_VisualWidget");
    //Options.autoClip = true;
  }

  //----------

  virtual ~SAT_VisualWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setFillBackground(bool AFill=true) {
    MFillBackground = AFill;
//    if (AFill) Options.opaque = true;
  }

  virtual void setBackgroundColor(SAT_Color AColor)       { MBackgroundColor = AColor; }

  virtual void setDrawBorder(bool ADraw=true)             { MDrawBorder = ADraw; }
  virtual void setBorderColor(SAT_Color AColor)           { MBorderColor = AColor; }
  virtual void setBorderWidth(double AWidth)              { MBorderWidth = AWidth; }

  virtual void setDrawIndicators(bool ADraw)              { MDrawIndicators = ADraw; }

  //

  virtual void setFillGradient(bool AFill=true)         { MFillGradient = AFill; }
  virtual void setGradientColor1(SAT_Color AColor)      { MGradientColor1 = AColor; }
  virtual void setGradientColor2(SAT_Color AColor)      { MGradientColor2 = AColor; }

  virtual void setDrawDropShadow(bool ADraw=true)         { MDrawDropShadow = ADraw; }
  virtual void setDropShadowInner(bool AInner=true)       { MDropShadowInner = AInner; }
  virtual void setDropShadowInnerColor(SAT_Color AColor)  { MDropShadowInnerColor = AColor; }
  virtual void setDropShadowOuterColor(SAT_Color AColor)  { MDropShadowOuterColor = AColor; }
  virtual void setDropShadowFeather(double AFeather)      { MDropShadowFeather = AFeather; }
  virtual void setDropShadowCorner(double ACorner)        { MDropShadowCorner = ACorner; }
//virtual void setDropShadowOffset(SAT_Point AOffset)     { MDropShadowOffset = AOffset; }

  virtual void setDrawRoundedCorners(bool ADraw)          { MDrawRoundedCorners = ADraw; }
  virtual void setRoundedCorners(uint32_t ACorners)       { MRoundedCorners = ACorners; }
  virtual void setRoundedCornerSize(double ASize)         { MRoundedCornerSize = ASize; }

  virtual void setSizable(bool ASizable=true)             { MSizable = ASizable; }
  virtual void setSizableBorder(SAT_Rect ABorder)         { MSizableBorder = ABorder; }
  virtual void setSizableEdges(uint32_t AEdges)           { MSizableEdges = AEdges; }
  virtual void setSizableFlags(uint32_t AFlags)           { MSizableFlags = AFlags; }

  virtual void setMovable(bool AMovable=true)             { MMovable = AMovable; }
  virtual void setMovableOffset(SAT_Rect AOffset)         { MMovableOffset = AOffset; }
  virtual void setMovableDirections(uint32_t ADirections) { MMovableDirections = ADirections; }
  virtual void setMovableFlags(uint32_t AFlags)           { MMovableFlags = AFlags; }



//------------------------------
public:
//------------------------------

  virtual bool      getFillBackground()   { return MFillBackground; }
  virtual SAT_Color getBackgroundColor()  { return MBackgroundColor; }

  // virtual bool      isMovable()             { return MMovable; }
  // virtual SAT_Rect  getMovableOffset()      { return  MMovableOffset; }
  // virtual uint32_t  getMovableDirections()  { return MMovableDirections; }
  // virtual uint32_t  getMovableFlags()       { return MMovableFlags; }

  // virtual bool      isSizable()             { return MMovable; }
  // virtual SAT_Rect  getSizableBorder()      { return MSizableBorder; }
  // virtual uint32_t  getSizableEdges()       { return MSizableEdges; }
  // virtual uint32_t  getSizableFlags()       { return MSizableFlags; }


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
    double scale = getWindowScale();
    if (MFillBackground) {
      SAT_Painter* painter = AContext->painter;
      SAT_Rect rect = getRect();
      //SAT_PRINT("%.2f, %.2f, %.2f, %.2f\n",rect.x,rect.y,rect.w,rect.h);

      if (MFillGradient) {
        SAT_Color c1 = MGradientColor1;
        SAT_Color c2 = MGradientColor2;
        painter->setFillLinearGradient(rect.x,rect.y,rect.x,rect.y2(),c1,c2); // top to bottom
      }
      else {
        painter->setFillColor(MBackgroundColor);
      }

      if (MDrawRoundedCorners) {
        double size = MRoundedCornerSize * scale;
        double ul = (MRoundedCorners & SAT_CORNER_UPPER_LEFT)  ? size : 0.0;
        double ur = (MRoundedCorners & SAT_CORNER_UPPER_RIGHT) ? size : 0.0;
        double lr = (MRoundedCorners & SAT_CORNER_LOWER_RIGHT) ? size : 0.0;
        double ll = (MRoundedCorners & SAT_CORNER_LOWER_LEFT)  ? size : 0.0;
        painter->fillRoundedRect(rect.x,rect.y,rect.w,rect.h,ul,ur,lr,ll);
      }
      else {
        painter->fillRect(rect.x,rect.y,rect.w,rect.h);
      }

    }
  }

  //----------

  virtual void drawBorder(SAT_PaintContext* AContext) {
    if (MDrawBorder) {
      //SAT_TRACE;
      SAT_Painter* painter = AContext->painter;
      SAT_Rect rect = getRect();
      double scale = getWindowScale();
      painter->setDrawColor(MBorderColor);
      painter->setLineWidth(MBorderWidth*scale);

      if (MDrawRoundedCorners) {
        double size = MRoundedCornerSize * scale;
        double ul = (MRoundedCorners & SAT_CORNER_UPPER_LEFT)  ? size : 0.0;
        double ur = (MRoundedCorners & SAT_CORNER_UPPER_RIGHT) ? size : 0.0;
        double lr = (MRoundedCorners & SAT_CORNER_LOWER_RIGHT) ? size : 0.0;
        double ll = (MRoundedCorners & SAT_CORNER_LOWER_LEFT)  ? size : 0.0;
        painter->drawRoundedRect(rect.x,rect.y,rect.w,rect.h,ul,ur,lr,ll);
      }
      else {
        painter->drawRect(rect.x,rect.y,rect.w,rect.h);
      }
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
      SAT_Color icolor = MDropShadowInnerColor;
      SAT_Color ocolor = MDropShadowOuterColor;
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
      SAT_Parameter* param = (SAT_Parameter*)getParameter();
      if (param) {
        SAT_Painter* painter = AContext->painter;
        SAT_Rect rect = getRect();
        double scale = getWindowScale();

        if (param->getIndicateMapped() == true) {
          //SAT_PRINT("mapped\n");
          SAT_Color color = param->getIndicateMappedColor();
          painter->setFillColor(color);
          double ms = MMappedIndicatorSize * scale;
          double coords[] = {
            rect.x,       rect.y,
            rect.x + ms,  rect.y,
            rect.x,       rect.y + ms,
            rect.x,       rect.y
          };
          painter->fillLineStrip(4,coords);
        }
        
        if (param->getIndicateAutomationState() != CLAP_PARAM_INDICATION_AUTOMATION_NONE) {
          //SAT_PRINT("automated\n");
          SAT_Color color = param->getIndicateAutomationColor();
          painter->setFillColor(color);
          double as = MAutomationIndicatorSize * scale;
          double xc = rect.x2() - as;
          double yc = rect.y    + as;
          painter->fillCircle(xc,yc,as);

          color.blend(SAT_White,0.5);
          painter->setLineWidth(1);
          painter->setDrawColor(color);
          painter->drawCircle(xc,yc,as);
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

  //----------

  SAT_Rect on_widget_postAlign(SAT_Rect ARect) override {
    double scale = getWindowScale();
    //SAT_Assert(scale > 0.0);
    if (scale > 0.0) {
      SAT_Assert(MPrevScale > 0.0);
      double ratio = scale / MPrevScale;
      //SAT_PRINT("ratio %f\n",ratio);
      if (ratio > 0.0) {
        MManualMoved *= ratio;
      }
      MPrevScale = scale;
    }
    return ARect;
  }

  //----------

  void on_widget_paint(SAT_PaintContext* AContext) override {
    drawDropShadow(AContext);
    fillBackground(AContext);
    paintChildren(AContext);
    drawIndicators(AContext);
    drawBorder(AContext);
  }

  //----------

  void on_widget_mouse_click(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    if (MSizable) {
      MMouseClickedX = AXpos;
      MMouseClickedY = AYpos;
      if (AButton == SAT_BUTTON_LEFT) {
        if (MSizable && (MResizeEdge != SAT_EDGE_NONE)) {
          MResizing = true;
          //SAT_PRINT("MResizing %i\n",MResizing);
        }
        else if (MMovable && MCanMove) {
          MMoving = true;
          //SAT_PRINT("MMoving %i MResizing %i\n",MMoving,MResizing);
        }
      }
    }
  }

  //----------

  void on_widget_mouse_release(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    if (AButton == SAT_BUTTON_LEFT) {
      if (MSizable && MResizing) MResizing = false;
      if (MMovable && MMoving) MMoving = false;
    }
  }

  //----------

  /*
    mrect contains layout, so if we move, and then sets new rect/base based on
    this.. the base rect will already contain borders, so next time it is aligned,
    borders will reapply.. on and on..
    we can only have 
  */

  void on_widget_mouse_move(int32_t AXpos, int32_t AYpos, uint32_t AState, uint32_t ATime) override {

    if (MMoving) {
      // SAT_Rect rect = getRect();
      // double S = getWindowScale();
      // int32_t x = AXpos - rect.x;
      // int32_t y = AYpos - rect.y;
      // SAT_PRINT("x %i y %i S %.3f\n",x,y,S);
      // if (MMovableDirections & SAT_DIRECTION_HORIZ) mrect.x += deltax;
      // if (MMovableDirections & SAT_DIRECTION_VERT)  mrect.y += deltay;
      // // SAT_Point offset = getLayoutOffset();
      // // mrect.x -= offset.x;
      // // mrect.y -= offset.y;
      // // setRect(mrect);
      // setRectAndBase(mrect);
      // do_widget_realign(this,SAT_WIDGET_REALIGN_GUI);
      // //realignParent();

      double deltax = (AXpos - MMousePreviousX);
      double deltay = (AYpos - MMousePreviousY);
      if (MMovableDirections & SAT_DIRECTION_HORIZ) { MManualMoved.x += deltax; }
      if (MMovableDirections & SAT_DIRECTION_VERT)  { MManualMoved.y += deltay; }
      do_widget_realign(this,SAT_WIDGET_REALIGN_GUI);
    }
    else if (MResizing) {
      double deltax = (AXpos - MMousePreviousX);
      double deltay = (AYpos - MMousePreviousY);
      if (MResizeEdge & SAT_EDGE_LEFT)    { MManualMoved.removeLeft(deltax); }
      if (MResizeEdge & SAT_EDGE_RIGHT)   { MManualMoved.addRight(deltax);   }
      if (MResizeEdge & SAT_EDGE_TOP)     { MManualMoved.removeTop(deltay);  }
      if (MResizeEdge & SAT_EDGE_BOTTOM)  { MManualMoved.addBottom(deltay);  }
      do_widget_realign(this,SAT_WIDGET_REALIGN_GUI);
    }
    else {
      checkHover(AXpos,AYpos);
    }
    MMousePreviousX = AXpos;
    MMousePreviousY = AYpos;
  }

  //----------

  // void on_widget_enter(SAT_Widget* AFrom, double AXpos, double AYpos) override {
  //   checkHover(AXpos,AYpos);
  // }

  //----------

  // void on_widget_leave(SAT_Widget* ATo, double AXpos, double AYpos) override {
  // }  

//------------------------------
private:
//------------------------------

  // returns edge that is (potentially) resizable

  uint32_t checkCanResize(double AXpos, double AYpos) {
    if (MSizable) {
      double S = getWindowScale();
      SAT_Rect rect = getRect();
      SAT_Rect offset = MSizableBorder;
      offset.scale(S);
      rect.shrink(offset);
      if (rect.contains(AXpos,AYpos))      return SAT_EDGE_NONE;
      if ((MSizableEdges & SAT_EDGE_LEFT)   && (AXpos < rect.x))     return SAT_EDGE_LEFT;
      if ((MSizableEdges & SAT_EDGE_RIGHT)  && (AXpos > rect.x2()))  return SAT_EDGE_RIGHT;
      if ((MSizableEdges & SAT_EDGE_TOP)    && (AYpos < rect.y))     return SAT_EDGE_TOP;
      if ((MSizableEdges & SAT_EDGE_BOTTOM) && (AYpos > rect.y2()))  return SAT_EDGE_BOTTOM;
    }
    return SAT_EDGE_NONE;
  }

  //----------

  // returns true if we can resize

  bool checkCanMove(double AXpos, double AYpos) {
    if (MMovable) {
      //double S = getWindowScale();
      SAT_Rect mrect = getRect();
      SAT_Rect offset = MMovableOffset;
      //offset.scale(S);
      mrect.shrink(offset);
      if (mrect.contains(AXpos,AYpos)) {
        if (MMovableFlags & 1) return false;
        else return true;
      }
    }
    if (MMovableFlags & 1) return true;
    else return false;
  }

  //----------

  void checkHover(double AXpos, double AYpos) {
    if (MSizable) {
      MResizeEdge = checkCanResize(AXpos,AYpos);
      if (MResizeEdge != SAT_EDGE_NONE) {
        if (MResizeEdge & SAT_EDGE_LEFT)    { do_widget_set_cursor(this,SAT_CURSOR_ARROW_LEFT);   return; }
        if (MResizeEdge & SAT_EDGE_RIGHT)   { do_widget_set_cursor(this,SAT_CURSOR_ARROW_RIGHT);  return; }
        if (MResizeEdge & SAT_EDGE_TOP)     { do_widget_set_cursor(this,SAT_CURSOR_ARROW_UP);     return; }
        if (MResizeEdge & SAT_EDGE_BOTTOM)  { do_widget_set_cursor(this,SAT_CURSOR_ARROW_DOWN);   return; }
      }
      MCanMove = checkCanMove(AXpos,AYpos);
      if (MCanMove) {
        if (MMovableDirections == SAT_DIRECTION_ALL)        { do_widget_set_cursor(this,SAT_CURSOR_MOVE);              return; }
        else if (MMovableDirections == SAT_DIRECTION_HORIZ) { do_widget_set_cursor(this,SAT_CURSOR_ARROW_LEFT_RIGHT);  return; }
        else if (MMovableDirections == SAT_DIRECTION_VERT)  { do_widget_set_cursor(this,SAT_CURSOR_ARROW_UP_DOWN);     return; }
      }
      do_widget_set_cursor(this,SAT_CURSOR_DEFAULT);
    }
  }

};

//----------------------------------------------------------------------
#endif