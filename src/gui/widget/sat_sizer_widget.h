#ifndef sat_sizer_widget_included
#define sat_sizer_widget_included
//----------------------------------------------------------------------

#include "gui/widget/sat_visual_widget.h"


//----------------------------------------------------------------------

class SAT_SizerWidget
: public SAT_VisualWidget {

//------------------------------
  protected:
//------------------------------

    int32_t     prevx       = 0.0f;
    int32_t     prevy       = 0.0f;
    bool        MIsDragging = false;
    SAT_Widget* MTarget     = nullptr;
    uint32_t    MMode       = SAT_DIRECTION_NONE;
    //SAT_Color   MFillColor  = SAT_Color( 0.45 );

//------------------------------
public:
//------------------------------

  SAT_SizerWidget(SAT_Rect ARect,uint32_t AMode=SAT_DIRECTION_NONE,SAT_Widget* ATarget=nullptr)
  : SAT_VisualWidget(ARect) {
    setName("SAT_SizerWidget");
    setHint("SAT_SizerWidget");
    //setHint("sizer");
    setMode(AMode);
    setTarget(ATarget);
    switch (AMode) {
      case SAT_DIRECTION_NONE:
        setCursor(SAT_CURSOR_DEFAULT);
      case SAT_DIRECTION_LEFT:
        //Layout.alignment = SAT_WIDGET_ALIGN_FILL_LEFT;
//        setAlignment(SAT_WIDGET_ALIGN_LEFT);
//        setStretching(SAT_WIDGET_STRETCH_VERTICAL);
        Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_LEFT;
        Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_VERT;
        Layout.flags |= SAT_WIDGET_LAYOUT_FILL_LEFT;
        setCursor(SAT_CURSOR_ARROW_LEFT_RIGHT);
        break;
      case SAT_DIRECTION_RIGHT:
        //Layout.alignment = SAT_WIDGET_ALIGN_FILL_RIGHT;
//        setAlignment(SAT_WIDGET_ALIGN_RIGHT);
//        setStretching(SAT_WIDGET_STRETCH_VERTICAL);
        Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_RIGHT;
        Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_VERT;
        Layout.flags |= SAT_WIDGET_LAYOUT_FILL_RIGHT;
        setCursor(SAT_CURSOR_ARROW_LEFT_RIGHT);
        break;
      case SAT_DIRECTION_TOP:
        //Layout.alignment = SAT_WIDGET_ALIGN_FILL_TOP;
//        setAlignment(SAT_WIDGET_ALIGN_TOP);
//        setStretching(SAT_WIDGET_STRETCH_HORIZONTAL);
        Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP;
        Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
        Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;
        setCursor(SAT_CURSOR_ARROW_UP_DOWN);
        break;
      case SAT_DIRECTION_BOTTOM:
        //Layout.alignment = SAT_WIDGET_ALIGN_FILL_BOTTOM;
//        setAlignment(SAT_WIDGET_ALIGN_BOTTOM);
//        setStretching(SAT_WIDGET_STRETCH_HORIZONTAL);
        Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_BOTTOM;
        Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
        Layout.flags |= SAT_WIDGET_LAYOUT_FILL_BOTTOM;
        setCursor(SAT_CURSOR_ARROW_UP_DOWN);
        break;
      //case SAT_WIDGET_SIZER_WINDOW:
      //  //Layout.alignment = SAT_WIDGET_ALIGN_LEFT_BOTTOM;
      //  setCursor(SAT_CURSOR_CROSS2);
      //  break;
    }

    setFillBackground(true);
    setBackgroundColor(0.3);
    setDrawBorder(false);
    
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

  // virtual void setFillColor(SAT_Color AColor) {
  //   MFillColor = AColor;
  // }

//------------------------------
public:
//------------------------------

  void on_widget_mouse_click(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) final {
    if (AButton == SAT_BUTTON_LEFT) {
      MIsDragging = true;
      prevx = AXpos;
      prevy = AYpos;
    }
  }

  //----------

  void on_widget_mouse_release(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) final {
    if (AButton == SAT_BUTTON_LEFT) MIsDragging = false;
  }

  //----------

  void on_widget_mouse_move(int32_t AXpos, int32_t AYpos, uint32_t AState, uint32_t ATime) final {
    if (MIsDragging) {
      int32_t deltax = AXpos - prevx;
      int32_t deltay = AYpos - prevy;
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

      if (MTarget) {
        //SAT_PRINT("deltax %i deltay %i\n",deltax,deltay);
        MTarget->do_widget_resize(this,deltax,deltay);
      }
      prevx = AXpos;
      prevy = AYpos;
    }
  }

};

//----------------------------------------------------------------------
#endif