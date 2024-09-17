#ifndef sat_voices_widget_included
#define sat_voices_widget_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/widget/sat_visual_widget.h"

#define MAX_VOICES 1024

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------


class SAT_VoicesWidget
: public SAT_VisualWidget {

//------------------------------
protected:
//------------------------------

  bool      MDrawVoices               = true;
  uint32_t  MMaxVoices                = 0;
  SAT_Color MPlayingColor             = SAT_Color(0.8);
  SAT_Color MReleasedColor            = SAT_Color(0.6);
  SAT_Color MOffColor                 = SAT_Color(0.45);
  uint32_t  MVoiceStates[MAX_VOICES]  = {0};

  double    MVoiceHeight              = 10.0;
  double    MTextSize                 = 6.0;

  uint32_t MNumPlayingVoices = 0;
  uint32_t MNumReleasedVoices = 0;

//------------------------------
public:
//------------------------------

  SAT_VoicesWidget(SAT_Rect ARect, uint32_t AMaxVoices)
  : SAT_VisualWidget(ARect) {
    setName("SAT_VoicesWidget");
    setHint("SAT_VoicesWidget");
    MMaxVoices = AMaxVoices;
    setFillBackground(false);
    setDrawBorder(false);
  }

  //----------

  virtual ~SAT_VoicesWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setMaxVoices(uint32_t AValue)        { MMaxVoices = AValue; }
  virtual void setPlayingColor(SAT_Color AColor)    { MPlayingColor = AColor; }
  virtual void setReleasedColor(SAT_Color AColor)   { MReleasedColor = AColor; }
  virtual void setOffColor(SAT_Color AColor)        { MOffColor = AColor; }

  virtual void setNumPlayingVoices(uint32_t ANum)   { MNumPlayingVoices = ANum; }
  virtual void setNumReleasedVoices(uint32_t ANum)  { MNumReleasedVoices = ANum; }

  virtual void setVoiceState(uint32_t AIndex, uint32_t AState) {
    MVoiceStates[AIndex] = AState;
  }

  virtual uint32_t getVoiceState(uint32_t AIndex) {
    return MVoiceStates[AIndex];
  }

//------------------------------
public:
//------------------------------

  //void prepare(SAT_WidgetOwner* AOwner) override {
  //  SAT_PanelWidget::prepare(AOwner);
  //  SAT_Window* window = (SAT_Window*)AOwner;
  //  window->registerTimerWidget(this);
  //}
  
  //----------

  //void on_Widget_timer(uint32_t AId, double ADelta) override {
  //  SAT_PRINT;
  //}

  //----------

  virtual void drawVoices(SAT_PaintContext* AContext) {
    if (MDrawVoices) {
      SAT_Painter* painter = AContext->painter;
      SAT_Rect mrect = getRect();
      double scale = getWindowScale();
      if (MMaxVoices > 0) {
        double width = mrect.w / MMaxVoices;
        for (uint32_t i=0; i<MMaxVoices; i++) {
          double x = mrect.x + (width * i);
          double y = mrect.y;
          double w = width;
          double h = MVoiceHeight;// mrect.h;
          SAT_Color color = SAT_Black;
          switch (MVoiceStates[i]) {
            case SAT_VOICE_OFF:       { color = MOffColor; break; }
            case SAT_VOICE_PLAYING:   { color = MPlayingColor; break; }
            case SAT_VOICE_RELEASED:  { color = MReleasedColor; break; }
          }
          painter->setFillColor(color);
          painter->fillRect(x,y,w,h);
        }
      }
      painter->setTextColor(SAT_Black);
      painter->setTextSize(MTextSize * scale);
      char temp[256] = {0};
      sprintf(temp,"Playing: %i Released %i",MNumPlayingVoices,MNumReleasedVoices);
      double x = mrect.x;
      double y = mrect.y + MVoiceHeight + 5;
      double w = mrect.w;
      double h = mrect.h - MVoiceHeight - 5;
      painter->drawTextBox(SAT_Rect(x,y,w,h),temp,SAT_TEXT_ALIGN_LEFT);
    }
  }

//------------------------------
public:
//------------------------------

  void on_Widget_paint(SAT_PaintContext* AContext) override {
    drawDropShadow(AContext);
    fillBackground(AContext);
    drawVoices(AContext);
    paintChildren(AContext);
    drawBorder(AContext);

  }

  //----------


};

#undef MAX_VOICES

//----------------------------------------------------------------------
#endif
