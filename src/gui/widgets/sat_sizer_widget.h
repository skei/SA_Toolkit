#ifndef sat_sizer_widget_included
#define sat_sizer_widget_included
//----------------------------------------------------------------------

//#include "gui/widgets/v0/sat_panel_widget.h"
#include "gui/sat_widget.h"


//----------------------------------------------------------------------

class SAT_SizerWidget
: public SAT_PanelWidget {

//------------------------------
  protected:
//------------------------------

    float       prevx       = 0.0f;
    float       prevy       = 0.0f;
    bool        MIsDragging = false;
    SAT_Widget* MTarget     = nullptr;
    uint32_t    MMode       = SAT_DIRECTION_NONE;
    SAT_Color   MFillColor  = SAT_Color( 0.45 );

//------------------------------
public:
//------------------------------

  SAT_SizerWidget(SAT_Rect ARect,uint32_t AMode=SAT_DIRECTION_NONE,SAT_Widget* ATarget=nullptr)
  : SAT_PanelWidget(ARect) {
    setName("SAT_SizerWidget");
    //setHint("sizer");
    setMode(AMode);
    setTarget(ATarget);
    switch (AMode) {
      case SAT_DIRECTION_NONE:
        setCursor(SAT_CURSOR_DEFAULT);
      case SAT_DIRECTION_LEFT:
        //Layout.alignment = SAT_WIDGET_ALIGN_FILL_LEFT;
        setAlignment(SAT_WIDGET_ALIGN_LEFT);
        setStretching(SAT_WIDGET_STRETCH_VERTICAL);
        setCursor(SAT_CURSOR_ARROW_LEFT_RIGHT);
        break;
      case SAT_DIRECTION_RIGHT:
        //Layout.alignment = SAT_WIDGET_ALIGN_FILL_RIGHT;
        setAlignment(SAT_WIDGET_ALIGN_RIGHT);
        setStretching(SAT_WIDGET_STRETCH_VERTICAL);
        setCursor(SAT_CURSOR_ARROW_LEFT_RIGHT);
        break;
      case SAT_DIRECTION_TOP:
        //Layout.alignment = SAT_WIDGET_ALIGN_FILL_TOP;
        setAlignment(SAT_WIDGET_ALIGN_TOP);
        setStretching(SAT_WIDGET_STRETCH_HORIZONTAL);
        setCursor(SAT_CURSOR_ARROW_UP_DOWN);
        break;
      case SAT_DIRECTION_BOTTOM:
        //Layout.alignment = SAT_WIDGET_ALIGN_FILL_BOTTOM;
        setAlignment(SAT_WIDGET_ALIGN_BOTTOM);
        setStretching(SAT_WIDGET_STRETCH_HORIZONTAL);
        setCursor(SAT_CURSOR_ARROW_UP_DOWN);
        break;
      //case SAT_WIDGET_SIZER_WINDOW:
      //  //Layout.alignment = SAT_WIDGET_ALIGN_LEFT_BOTTOM;
      //  setCursor(SAT_CURSOR_CROSS2);
      //  break;
    }
  }

  virtual ~SAT_SizerWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setMode(uint32_t AMode) {
    MMode = AMode;
    switch(MMode) {
      case SAT_DIRECTION_NONE:     setCursor(SAT_CURSOR_DEFAULT);             break;
      case SAT_DIRECTION_LEFT:     setCursor(SAT_CURSOR_ARROW_LEFT_RIGHT);    break;
      case SAT_DIRECTION_RIGHT:    setCursor(SAT_CURSOR_ARROW_LEFT_RIGHT);    break;
      case SAT_DIRECTION_TOP:      setCursor(SAT_CURSOR_ARROW_UP_DOWN);       break;
      case SAT_DIRECTION_BOTTOM:   setCursor(SAT_CURSOR_ARROW_UP_DOWN);       break;
      //case SAT_DIRECTION_NONE_WINDOW:   MMouseCursor = SAT_CURSOR_CROSS2;               break;
    }
  }

  virtual void setTarget(SAT_Widget* ATarget) {
    MTarget = ATarget;
  }

  virtual void setFillColor(SAT_Color AColor) {
    MFillColor = AColor;
  }

//------------------------------
public:
//------------------------------

//  void on_widget_paint(SAT_PaintContext* AContext) final {
//    SAT_Painter* painter = AContext->painter;
//    SAT_Rect mrect = getRect();
//    painter->setFillColor(MFillColor);
//    painter->fillRect(mrect.x,mrect.y,mrect.w,mrect.h);
//  }

  //----------

  void on_widget_mouse_click(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) final {
    if (AButton == SAT_BUTTON_LEFT) {
      MIsDragging = true;
      prevx = AXpos;
      prevy = AYpos;
    }
  }

  //----------

  void on_widget_mouse_release(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) final {
    if (AButton == SAT_BUTTON_LEFT) MIsDragging = false;
  }

  //----------

  void on_widget_mouse_move(double AXpos, double AYpos, uint32_t AState, uint32_t ATime) final {
    if (MIsDragging) {
      float deltax = AXpos - prevx;
      float deltay = AYpos - prevy;
      switch(MMode) {
        case SAT_DIRECTION_LEFT:
          deltay = 0;
          break;
        case SAT_DIRECTION_RIGHT:
          deltay = 0;
          deltax = -deltax;
          break;
        case SAT_DIRECTION_TOP:
          deltax = 0;
          break;
        case SAT_DIRECTION_BOTTOM:
          deltax = 0;
          deltay = -deltay;
          break;
        //case SAT_WIDGET_SIZER_WINDOW:
        //  break;

      } // switch mode

      //if FMode = ksm_Horizontal then deltax := 0;
      //if FMode = ksm_Vertical then deltay := 0;
      //if (MMode == SAT_SIZER_WINDOW) {
      //  if (MParent) MParent->do_widgetListener_resized(this,deltax,deltay);
      //}
      //else {

        if (MTarget) {

          //if (MMode == SAT_WIDGET_SIZER_WINDOW) {
          //  MTarget->do_widgetListener_resized(this,deltax,deltay);
          //}
          //else {
            float tw = MTarget->getRect().w;
            float th = MTarget->getRect().h;
            // todo: check flags.sizePercent
            tw += deltax;
            th += deltay;
            
            //SAT_DPoint tmin = MTarget->Layout.minSize;
            //SAT_DPoint tmax = MTarget->Layout.maxSize;
            //if ( (tw > tmin.w) && (tw < tmax.w)
            //  && (th > tmin.h) && (th < tmax.h) ) {
  
              //MTarget->do_widgetListener_resized(this,deltax,deltay);
              
              MTarget->do_widgetListener_resized(this,deltax,deltay);

              //parentNotify(SAT_WIDGET_NOTIFY_REALIGN,0);
              //do_widgetListener_realign(this);
              parentRealign();
              
            //}

          //}
        } // sizer
      //  else self.do_widgetListener_resized(self,deltax,deltay,FMode);
      //}
      prevx = AXpos;
      prevy = AYpos;
    } // dragging
    //axWidget::doMouseMove(aXpos,aYpos,aButton);
  }

};

//----------------------------------------------------------------------
#endif