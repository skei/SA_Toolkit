#ifndef sat_slider_bank_widget_included
#define sat_slider_bank_widget_included
//----------------------------------------------------------------------

#include "base/util/sat_random.h"
#include "gui/widget/sat_visual_widget.h"
//#include "gui/sat_widget.h"

#define SAT_MAX_NUM_SLIDERS 256

class SAT_SliderBankWidget
: public SAT_VisualWidget {

//------------------------------
private:
//------------------------------

    int32_t     MNumSliders   = 0;
    int32_t     MHoverSlider  = -1;
    SAT_Color  MBarColor     = SAT_DarkerGrey;
    SAT_Color  MHoverColor   = SAT_LightestGrey;
    bool        MIsDragging   = false;

    float       MSliderValues[SAT_MAX_NUM_SLIDERS] = {0};

//------------------------------
public:
//------------------------------

  SAT_SliderBankWidget(SAT_Rect ARect, int32_t ANum=16)
  : SAT_VisualWidget(ARect) {

    setName("SAT_SliderBankWidget");
    setHint("SAT_SliderBankWidget");
    setCursor(SAT_CURSOR_FINGER);

//    Options.wantHoverEvents = true;
//    Options.autoHideCursor = true;

    setFillBackground(true);
    setBackgroundColor(SAT_Color(0.55f));
    setDrawBorder(false);

    MNumSliders = ANum;

    //setFlag(SAT_WIDGET_HOVER);
    //KMemset(MSliderValues,0,sizeof(MSliderValues));

    for (int32_t i=0; i<SAT_MAX_NUM_SLIDERS; i++) {
      float v = SAT_RandomRange(0,1);
      //SAT_Print("%i = %.2f\n",i,v);
      MSliderValues[i] = v;
    }

  }

  virtual ~SAT_SliderBankWidget() {
  }

//------------------------------
public:
//------------------------------

    void setNumSliders(int32_t ANum)     { MNumSliders = ANum; }
    void setBarColor(SAT_Color AColor)  { MBarColor = AColor; }

  //public:
  //
  //  void drawBar(SPainter* APainter, int32 AIndex) {
  //    float w = ( (float)MRect.w / (float)MNumSliders );
  //    float x = (float)MRect.x + (w * AIndex);
  //    float y = MRect.y2() - floorf(MSliderValues[AIndex] * MRect.h);
  //    APainter->fillRectangle( floorf(x), y/*MRect.y*/, floorf(x+w)-2, MRect.y2() );
  //  }

//------------------------------
public:
//------------------------------


  void on_widget_paint(SAT_PaintContext* AContext) override {
    SAT_Rect mrect = getRect();
    SAT_Painter* painter = AContext->painter;

    drawDropShadow(AContext);
    fillBackground(AContext);

    //STrace("painting %s\n",MName);
    if (MNumSliders > 1) {
      float w = ( (float)mrect.w / (float)MNumSliders );
      int32_t iw2 = floorf(w) - 2;
      float x = (float)mrect.x /*+ sliderwidth - 1*/;
      for (int32_t i=0; i<MNumSliders; i++) {
        float v = MSliderValues[i] * mrect.h;
        float y = mrect.y2() - v;
        float h = v;//mrect.h - v;
        int32_t ix = floorf(x);

        if ((i==MHoverSlider) && MIsDragging) {
          //APainter->fillRectangle( SAT_DRect(ix,y,iw2,h),MHoverColor );
          painter->setFillColor(MHoverColor);
          painter->fillRect(ix,y,iw2,h);
        }
        else {
          //APainter->fillRectangle( SAT_DRect(ix,y,iw2,h), MBarColor );
          painter->setFillColor(MBarColor);
          painter->fillRect(ix,y,iw2,h);
        }

        x += w;
      }
    }

    paintChildren(AContext);
    drawBorder(AContext);

  }

  //----------

  void on_widget_mouse_click(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime=0) override {
    if (AButton == SAT_BUTTON_LEFT) {
      MIsDragging = true;
      SAT_Rect mrect = getRect();
      int32_t i = (AXpos - mrect.x) * MNumSliders / mrect.w;
      float v = (mrect.y2() - AYpos) / mrect.h;

      MSliderValues[i] = SAT_Clamp(v,0,1);
      MHoverSlider = i;
      
      do_widget_redraw(this);

//      if (Options.autoHideCursor) {
//        do_widgetListener_set_cursor(this,SAT_CURSOR_LOCK);
//        do_widgetListener_set_cursor(this,SAT_CURSOR_HIDE);
//      }

    }
  }

  //----------

  void on_widget_mouse_release(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime=0) override {
    if (AButton == SAT_BUTTON_LEFT) {
      MIsDragging = false;
      do_widget_redraw(this);
      MHoverSlider = -1;
      
//      if (Options.autoHideCursor) {
//        do_widgetListener_set_cursor(this,SAT_CURSOR_UNLOCK);
//        do_widgetListener_set_cursor(this,SAT_CURSOR_SHOW);
//      }

    }
  }

  //----------

  void on_widget_mouse_move(int32_t AXpos, int32_t AYpos, uint32_t AState, uint32_t ATime=0) override {
    SAT_Rect mrect = getRect();
    //SWidget::on_mouseMove(AXpos,AYpos,AState);
    float w = (float)(AXpos - mrect.x) / (float)mrect.w; // 0..1
    int32_t index = (w * (float)MNumSliders);
    if (MIsDragging) {
      float v = (mrect.y2() - AYpos) / mrect.h;
      MSliderValues[index] = SAT_Clamp(v,0,1);
      if (index != MHoverSlider) MHoverSlider = index;
      do_widget_redraw(this);
    }
    else {
      if (index != MHoverSlider) MHoverSlider = index;
      do_widget_redraw(this);
    }
  }

  //----------

  void on_widget_leave(SAT_Widget* ATo, int32_t AXpos, int32_t AYpos, uint32_t ATime) override {
    SAT_VisualWidget::on_widget_leave(ATo,AXpos,AYpos,ATime);
    //SAT_DRect mrect = getRect();
    //SWidget::on_leave(AWidget);
    if (MHoverSlider >= 0) {
      MHoverSlider = -1;
      do_widget_redraw(this);
    }
    //if (flags.autoCursor) do_widgetListener_setMouseCursor(this,SAT_CURSOR_DEFAULT);
    //if (flags.autoHint) do_widgetListener_setHint(this,"");
  }

};

#undef SAT_MAX_NUM_SLIDERS


//----------------------------------------------------------------------
#endif