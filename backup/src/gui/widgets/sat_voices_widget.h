#ifndef sat_voices_widget_included
#define sat_voices_widget_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "gui/widgets/sat_panel_widget.h"

#define MAX_VOICES 1024

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------


class SAT_VoicesWidget
: public SAT_PanelWidget {

//------------------------------
protected:
//------------------------------

  bool      MDrawVoices               = true;
  uint32_t  MMaxVoices                = 0;
  SAT_Color MPlayingColor             = SAT_Color(0.8);
  SAT_Color MReleasedColor            = SAT_Color(0.5);
  SAT_Color MOffColor                 = SAT_Color(0.2);
  uint32_t  MVoiceStates[MAX_VOICES]  = {0};

//------------------------------
public:
//------------------------------

  SAT_VoicesWidget(SAT_Rect ARect, uint32_t AMaxVoices)
  : SAT_PanelWidget(ARect) {
    setName("SAT_VoicesWidget");
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

  virtual void setMaxVoices(uint32_t AValue)      { MMaxVoices = AValue; }
  virtual void setPlayingColor(SAT_Color AColor)  { MPlayingColor = AColor; }
  virtual void setReleasedColor(SAT_Color AColor) { MReleasedColor = AColor; }
  virtual void setOffColor(SAT_Color AColor)      { MOffColor = AColor; }

  virtual void setVoiceState(uint32_t AIndex, uint32_t AState) {
    MVoiceStates[AIndex] = AState;
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

  //void on_widget_timer(uint32_t AId, double ADelta) override {
  //  SAT_PRINT;
  //}

  //----------

  virtual void drawVoices(SAT_PaintContext* AContext) {
    if (MDrawVoices) {
      if (MMaxVoices > 0) {
        SAT_Painter* painter = AContext->painter;
        SAT_Rect mrect = getRect();

        double width = mrect.w / MMaxVoices;

        for (uint32_t i=0; i<MMaxVoices; i++) {

          double x = mrect.x + (width * i);
          double y = mrect.y;
          double w = width;
          double h = mrect.h;

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
    }
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(SAT_PaintContext* AContext) override {
    drawDropShadow(AContext);
    fillBackground(AContext);
    drawVoices(AContext);
    paintChildWidgets(AContext);
    drawBorder(AContext);

  }

  //----------


};

#undef MAX_VOICES

//----------------------------------------------------------------------
#endif
