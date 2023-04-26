#ifndef sa_tyr_widgets_included
#define sa_tyr_widgets_included
//----------------------------------------------------------------------

#include "base/mip.h"
#include "gui/mip_widgets.h"

//----------------------------------------------------------------------
//
// voice
//
//----------------------------------------------------------------------

class sa_tyr_VoiceWidget
: public MIP_PanelWidget {

//------------------------------
public:

  sa_tyr_VoiceWidget(MIP_DRect ARect)
  : MIP_PanelWidget(ARect) {
    setFillBackground(false);
  }

//------------------------------
public:
//------------------------------

  uint32_t voice_state[SA_TYR_NUM_VOICES]  = {0};

  //----------

  void on_widget_paint(MIP_PaintContext* AContext) final {
    MIP_DRect mrect = getRect();
    MIP_Painter* painter = AContext->painter;
    float w = mrect.w / SA_TYR_NUM_VOICES;
    MIP_DRect rect = MIP_DRect(mrect.x,mrect.y,w, mrect.h);
    MIP_Color color = MIP_COLOR_DARK_GRAY;
    for (uint32_t i=0; i<SA_TYR_NUM_VOICES; i++) {

//      painter->beginPath();
//      painter->rectangle(rect);

      switch (voice_state[i]) {
        case MIP_VOICE_OFF:       color = MIP_COLOR_BLACK;        break;
        case MIP_VOICE_WAITING:   color = MIP_COLOR_LIGHT_YELLOW; break;
        case MIP_VOICE_PLAYING:   color = MIP_COLOR_BRIGHT_GREEN; break;
        case MIP_VOICE_RELEASED:  color = MIP_COLOR_GREEN;        break;
        case MIP_VOICE_FINISHED:  color = MIP_COLOR_WHITE;        break;
      }

//      painter->fillColor(color);
//      painter->fill();

      painter->setFillColor(color);
      painter->fillRect(rect.x,rect.y,rect.w,rect.h);

      rect.x += w; // 8
    }
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_tyr_MenuWidget
: public MIP_MenuWidget {

public:

  sa_tyr_MenuWidget(uint32_t AWidth, uint32_t AHeight, const char** AText, uint32_t ACount)
  : MIP_MenuWidget(MIP_DRect(0,0,AWidth,ACount*AHeight),nullptr) {

    for (uint32_t i=0; i<ACount; i++) {
      MIP_MenuItemWidget* w = new MIP_MenuItemWidget( MIP_DRect(0, i*AHeight, AWidth, AHeight ), AText[i] );
      //w->Layout.scaleMode = MIP_WIDGET_SCALE_MODE_INITIAL_RATIO;
      //w->setTextSize(-0.7);
      appendChildWidget(w);
    }
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_tyr_SectionPanelWidget
: public MIP_PanelWidget {

public:

  sa_tyr_SectionPanelWidget(MIP_DRect ARect, const char* AText, uint32_t AHeaderHeight=20)
  : MIP_PanelWidget(ARect) {

    setFillBackground(true);
    setBackgroundColor(0.55);

    MIP_TextWidget* text = new MIP_TextWidget(MIP_DRect(ARect.x,ARect.y,ARect.w,AHeaderHeight),AText);
    appendChildWidget(text);
    text->setTextAlignment(MIP_TEXT_ALIGN_CENTER);
    text->setTextSize(15);
    text->setDrawBorder(true);
    text->setTextColor(MIP_COLOR_LIGHT_GRAY);
    text->setDrawBorder(false);
    text->setFillBackground(true);
    text->setBackgroundColor(MIP_COLOR_DARK_GRAY);
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------


class sa_tyr_SelectorWidget
: public MIP_SelectorWidget {

public:

  sa_tyr_SelectorWidget(MIP_DRect ARect, const char* AText, MIP_MenuWidget* AMenu)
  : MIP_SelectorWidget(ARect,AText,AMenu) {
    //setTextSize(-0.7);
    //setSelected(0);
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_tyr_DragValueWidget
: public MIP_DragValueWidget {

public:

  sa_tyr_DragValueWidget(MIP_DRect ARect, const char* AText, double AValue=0.0, bool ABipolar=false, double ASnapPos=0.5, double ASnapDist=0.05)
  : MIP_DragValueWidget(ARect,AText,AValue) {
    setDrawBorder(true);
    setBorderColor(0.4);
    setFillBackground(true);
    setBackgroundColor(0.6);
    if (ABipolar) {
      setSnap(true);
      setSnapPos(ASnapPos);
      setSnapDist(ASnapDist);
      setBipolar(true);
    }
    setDrawModulation(true);
    setModulationColor(MIP_Color(1,0,0,0.25));
    setModulationOffset(MIP_DRect( 4,4,4,4 ));
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_tyr_SliderWidget
: public MIP_SliderWidget {

public:

  sa_tyr_SliderWidget(MIP_DRect ARect, const char* AText, double AValue=0.0, bool ABipolar=false, double ASnapPos=0.5, double ASnapDist=0.05)
  : MIP_SliderWidget(ARect,AText,AValue) {
    if (ABipolar) {
      setSnap(true);
      setSnapPos(ASnapPos);
      setSnapDist(ASnapDist);
      //setBipolar(true);
    }
    setDrawModulation(true);
    setModulationColor(MIP_Color(1,0,0,0.25));
    //setModulationOffset(MIP_DRect( 4,4,4,4 ));
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_tyr_BigKnobWidget
: public MIP_Knob2Widget {

public:

  sa_tyr_BigKnobWidget(MIP_DRect ARect, const char* AText, double AValue=0.0, bool ABipolar=false, double ASnapPos=0.5, double ASnapDist=0.05)
  : MIP_Knob2Widget(ARect,AText,AValue,true,false) {
    if (ABipolar) {
      MKnobWidget->setSnap(true);
      MKnobWidget->setSnapPos(ASnapPos);
      MKnobWidget->setSnapDist(ASnapDist);
      MKnobWidget->setBipolar(true);
    }
    MLabelWidget->setTextSize(15);
    MKnobWidget->setModArcThickness(5);
    MKnobWidget->setModArcOffset(2);
    MKnobWidget->setArcThickness(10);
    MKnobWidget->setValueSize(12);
    MKnobWidget->setDrawModulation(true);
    MKnobWidget->setModulationColor(MIP_Color(1,0,0,0.5));
    MKnobWidget->setModArcThickness(5);
    MKnobWidget->setModArcOffset(3);

  }

};

//----------

class sa_tyr_SmallKnobWidget
: public MIP_Knob2Widget {

public:

  sa_tyr_SmallKnobWidget(MIP_DRect ARect, const char* AText, double AValue=0.0, bool ABipolar=false, double ASnapPos=0.5, double ASnapDist=0.05)
  : MIP_Knob2Widget(ARect,AText,AValue,true,false) {
    if (ABipolar) {
      MKnobWidget->setSnap(true);
      MKnobWidget->setSnapPos(ASnapPos);
      MKnobWidget->setSnapDist(ASnapDist);
      MKnobWidget->setBipolar(true);
    }
    MKnobWidget->setArcThickness(5);
    MKnobWidget->setValueSize(8);
    MLabelWidget->setTextSize(12);
    MKnobWidget->setDrawModulation(true);
    MKnobWidget->setModulationColor(MIP_Color(1,0,0,0.5));
    MKnobWidget->setModArcThickness(3);
    MKnobWidget->setModArcOffset(1.5);
  }

};

//----------------------------------------------------------------------
#endif
