#ifndef sat_scroll_bar_widget_included
#define sat_scroll_bar_widget_included
//----------------------------------------------------------------------

#include "sat.h"
#include "base/util/sat_math.h"
#include "gui/sat_widget.h"
#include "gui/widget/sat_visual_widget.h"
//#include "gui/sat_paint_context.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_ScrollBarWidget
: public SAT_VisualWidget {

//------------------------------
protected:
//------------------------------

//SAT_Color MBackgroundColor  = SAT_COLOR_DARK_GRAY;

  SAT_Color MThumbColor       = SAT_Color(0.55);//SAT_COLOR_LIGHT_GRAY;
  SAT_Color MThumbBorderColor = SAT_Color(0.25);//SAT_COLOR_DARK_GRAY;
  SAT_Color MInteractiveColor = SAT_Color(1);//SAT_COLOR_WHITE;

  float     MThumbPos         = 0.0f;
  float     MPrevThumbPos     = 0.0f;
  float     MThumbSize        = 0.2f;
  float     MPageSize         = 0.2f;

  SAT_Rect  MThumbRect         = SAT_Rect(0,0);
  bool      MIsDragging       = false;
  float     MClickedX         = 0.0f;
  float     MClickedY         = 0.0f;
  float     MClickedPos       = 0.0f;

  uint32_t  MDirection        = SAT_DIRECTION_HORIZ;
  
  double    MRoundedThumb     = 0.0;

//------------------------------
public:
//------------------------------

  SAT_ScrollBarWidget(SAT_Rect ARect)
  : SAT_VisualWidget(ARect) {
    setName("SAT_ScrollBarWidget");
    setHint("SAT_ScrollBarWidget");
    //if (AVertical) setFlag(SAT_WIDGET_VERTICAL);
    setBackgroundColor(0.3);
    setDrawBorder(false);
    //setBorderColor(0);
    //setCursor(SAT_CURSOR_ARROW_UP_DOWN);
  }

  //----------

  virtual ~SAT_ScrollBarWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setDirection(uint32_t ADir) {
    MDirection = ADir;
  }

  //----------

  virtual float getThumbPos() {
    return MThumbPos;
  }

  virtual float getPrevThumbPos() {
    return MPrevThumbPos;
  }

  virtual float getThumbSize() {
    return MThumbSize;
  }

  // virtual void setBackgroundColor(SAT_Color AColor) {
  //   MBackgroundColor = AColor;
  // }

  //----------

  virtual void setThumbColor(SAT_Color AColor) {
    MThumbColor = AColor;
  }

  //----------

  virtual void setInteractiveColor(SAT_Color AColor) {
    MInteractiveColor = AColor;
  }

  //----------

  virtual void setThumbPos(float APos, bool ARedraw=true) {
    MPrevThumbPos = MThumbPos;
    MThumbPos = APos;
    MThumbPos = SAT_Clamp(MThumbPos,0.0f,1.0f);
    //recalcThumbRect();
    if (ARedraw) do_widget_redraw(this);
  }

  //----------

  virtual void setThumbSize(float ASize, bool ARedraw=true) {
    //SAT_PRINT;
    MThumbSize = ASize;
    //MThumbSize = SAT_Max(0.05,ASize);
    if (MThumbSize >= 1.0f) {
      MPrevThumbPos = MThumbPos;
      MThumbPos = 0.0f;
    }
    MThumbSize = SAT_Clamp(MThumbSize,0.0f,1.0f);
    //recalcThumbRect();
    if (ARedraw) do_widget_redraw(this);
  }

  //----------

  virtual void setPageSize(float ASize) {
    MPageSize = ASize;
  }

  //----------

  virtual void recalcThumbRect() {
    SAT_Rect mrect = getRect();
    if (MDirection == SAT_DIRECTION_VERT) {
      float thumb = mrect.h * MThumbSize;
      thumb = SAT_Max(20,thumb);
      float available = mrect.h - thumb;
      MThumbRect.x = mrect.x;
      MThumbRect.y = mrect.y + (MThumbPos * available);
      MThumbRect.w = mrect.w;
      MThumbRect.h = thumb;
    }
    else {
      float thumb = mrect.w * MThumbSize;
      thumb = SAT_Max(20,thumb);
      float available = mrect.w - thumb;
      MThumbRect.x = mrect.x + (MThumbPos * available);
      MThumbRect.y = mrect.y;
      MThumbRect.w = thumb;
      MThumbRect.h = mrect.h;
    }
    //SAT_Trace("ThumbRect %.2f %.2f %.2f %.2f\n",MThumbRect.x,MThumbRect.y,MThumbRect.w,MThumbRect.h);
  }

//------------------------------
private:
//------------------------------

//  void draw_background(SAT_Painter* APainter, SAT_Rect ARect) {
//    APainter->roundedRectangle(ARect,6,SAT_CORNER_ALL,SAT_EDGE_ALL);
//    APainter->setColor(MBackgroundColor);
//    APainter->fillPath();
//  }

  //----------

      /*
      if (MDrawRoundedCorners) painter->roundedRectVarying(MRect.x,MRect.y,MRect.w,MRect.h, MULCornerRadius, MURCornerRadius, MBRCornerRadius, MBLCornerRadius);
      else painter->rect(MRect.x,MRect.y,MRect.w,MRect.h);
      if (MFillGradient) {
        SAT_PaintSource paint = painter->linearGradient(MRect.x,MRect.y,MRect.x,MRect.y + MRect.y2(), MGradientColor1, MGradientColor2);
        painter->fillPaint(paint);
      }
      else {
        painter->fillColor(MBackgroundColor);
      }
      painter->fill();
      */

  void draw_thumb(SAT_Painter* APainter, SAT_Rect ARect, bool AVertical=true) {
    
    double S = getWindowScale();

//    APainter->beginPath();
//    //APainter->roundedRectangle(rect,5,SAT_CORNER_ALL,SAT_EDGE_ALL);
//    APainter->roundedRectVarying(ARect.x,ARect.y,ARect.w,ARect.h, 5,5,5,5);
//    //    SAT_Color c1 = MThumbColor;
//    //    SAT_Color c2 = MThumbColor;
//    //    c1.blend(SAT_COLOR_WHITE,0.2);
//    //    c2.blend(SAT_COLOR_BLACK,0.2);
//    //    APainter->fillPathGradient(rect.x,rect.y,rect.x2(),rect.y2(),c1,c2,AVertical);
//    APainter->fillColor(MThumbColor);
//    APainter->fill();

    APainter->setFillColor(MThumbColor);
    APainter->fillRoundedRect(ARect.x,ARect.y,ARect.w,ARect.h, MRoundedThumb*S );

    // border

//    APainter->beginPath();
//    //APainter->roundedRectangle(rect,5,SAT_CORNER_ALL,SAT_EDGE_ALL);
//    APainter->roundedRectVarying(ARect.x,ARect.y,ARect.w,ARect.h, 5,5,5,5);
//    APainter->strokeWidth(1);
//    APainter->strokeColor(MThumbBorderColor);
//    APainter->stroke();


    APainter->setLineWidth(1*S);
    APainter->setDrawColor(MThumbBorderColor);
    APainter->drawRoundedRect(ARect.x,ARect.y,ARect.w,ARect.h, MRoundedThumb*S );

  }

//------------------------------
public: // parent to child
//------------------------------

//  void on_widget_move(double AXpos, double AYpos)  override {}
//  void on_widget_resize(double AWidth, double AHeight) override {}
//  void on_widget_align(bool ARecursive=true) override {}

  void on_widget_paint(SAT_PaintContext* AContext) override {
    drawDropShadow(AContext);
    fillBackground(AContext);
    SAT_Painter* painter = AContext->painter;
    recalcThumbRect();
    //draw_background(painter,MRect);
    bool vgrad = false;
    if (MDirection == SAT_DIRECTION_HORIZ) vgrad = true;
    draw_thumb(painter,MThumbRect,vgrad);
    paintChildren(AContext);
    drawIndicators(AContext);
    drawBorder(AContext);
  }

//  void on_widget_key_press(uint8_t AChar, uint32_t AKeySym, uint32_t AState, uint32_t ATime) override {}
//  void on_widget_key_release(uint8_t AChar, uint32_t AKeySym, uint32_t AState, uint32_t ATime) override {}

  void on_widget_mouse_click(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    //SAT_Rect mrect = MRect;
    //SAT_Widget::on_widget_mouseClick(AXpos,AYpos,AButton,AState);
    if (AButton == SAT_BUTTON_LEFT) {
      if (MThumbRect.contains(AXpos,AYpos)) {
        MClickedX = AXpos;
        MClickedY = AYpos;
        MClickedPos = MThumbPos;
        MIsDragging = true;
        //MIsInteractive  = true;
        //do_widgetListener_redraw(this,mrect,0);
      }
      else {
        if (MDirection == SAT_DIRECTION_VERT) {
          if (AYpos < MThumbRect.y) {
            MPrevThumbPos = MThumbPos;
            MThumbPos -= MPageSize;
            //recalcThumbRect();
            do_widget_update(this);
            do_widget_redraw(this);
          }
          else if (AYpos > MThumbRect.y2()) {
            MPrevThumbPos = MThumbPos;
            MThumbPos += MPageSize;
            //recalcThumbRect();
            do_widget_update(this);
            do_widget_redraw(this);
          }
        }
        else {
          if (AXpos < MThumbRect.x) {
            MPrevThumbPos = MThumbPos;
            MThumbPos -= MPageSize;
            //recalcThumbRect();
            do_widget_update(this);
            do_widget_redraw(this);
          }
          else if (AXpos > MThumbRect.x2()) {
            MPrevThumbPos = MThumbPos;
            MThumbPos += MPageSize;
            //recalcThumbRect();
            do_widget_update(this);
            do_widget_redraw(this);
          }
        } // !vertical
      } // !contains
    } // left button
  }

  void on_widget_mouse_release(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    //SAT_Widget::on_mouseRelease(AXpos,AYpos,AButton,AState);
    if (AButton == SAT_BUTTON_LEFT) {
      if (MIsDragging) {
        MIsDragging = false;
        //MIsInteractive = false;
        do_widget_redraw(this);
      }
    }
  }

  void on_widget_mouse_move(int32_t AXpos, int32_t AYpos, uint32_t AState, uint32_t ATime) override {
    SAT_Rect mrect = getRect();
    //SAT_Widget::on_mouseMove(AXpos,AYpos,AState);
    if (MIsDragging) {

      float available = 0.0f;
      float dist = 0.0f;
      float thumb_size = 0.0f;
      if (MDirection == SAT_DIRECTION_VERT) {
        dist = AYpos - MClickedY;
        thumb_size = mrect.h * MThumbSize;
        available = mrect.h - thumb_size;
      }
      else {
        dist = AXpos - MClickedX;
        thumb_size = mrect.w * MThumbSize;
        available = mrect.w - thumb_size;
      }
      if (available > 0.0f) {
        float v = dist / available;
        MPrevThumbPos = MThumbPos;
        MThumbPos = MClickedPos + v;
        MThumbPos = SAT_Clamp(MThumbPos,0.0f,1.0f);
        //recalcThumbRect();
        do_widget_update(this);
        do_widget_redraw(this);
      }
    }
  }

//  void on_widget_enter(SAT_Widget* AFrom, double AXpos, double AYpos, uint32_t ATime) override {}
//  void on_widget_leave(SAT_Widget* ATo, double AXpos, double AYpos, uint32_t ATime) override {}
//  //void on_widget_connect(SAT_Parameter* AParameter) override {}

//  virtual void on_widget_modal(bool AModal) {}


//------------------------------
public: // child to parent
//------------------------------

//  void do_widgetListener_update(SAT_Widget* ASender, uint32_t AMode=0) override {}
//  void do_widgetListener_redraw(SAT_Widget* ASender, uint32_t AMode=0) override {}
//  void do_widgetListener_modal(SAT_Widget* ASender, uint32_t AMode=0) override {}
//  void do_widgetListener_cursor(SAT_Widget* ASender, uint32_t ACursor) override {}
//  void do_widgetListener_hint(SAT_Widget* ASender, const char* AHint) override {}
//  void do_widgetListener_notify(SAT_Widget* ASender, uint32_t AMode, int32_t AValue) override {}

};

//----------------------------------------------------------------------
#endif