#ifndef sa_tyr_widgets_included
#define sa_tyr_widgets_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/sat_widgets.h"

//----------------------------------------------------------------------
//
// voice
//
//----------------------------------------------------------------------

class sa_tyr_VoiceWidget
: public SAT_PanelWidget {

//------------------------------
public:

  sa_tyr_VoiceWidget(SAT_Rect ARect)
  : SAT_PanelWidget(ARect) {
    setFillBackground(false);
  }

//------------------------------
public:
//------------------------------

  uint32_t voice_state[SA_TYR_NUM_VOICES]  = {0};

  //----------

  void on_widget_paint(SAT_PaintContext* AContext) final {
    SAT_Rect mrect = getRect();
    SAT_Painter* painter = AContext->painter;
    float w = mrect.w / SA_TYR_NUM_VOICES;
    SAT_Rect rect = SAT_Rect(mrect.x,mrect.y,w, mrect.h);
    SAT_Color color = SAT_DarkGrey;
    for (uint32_t i=0; i<SA_TYR_NUM_VOICES; i++) {

//      painter->beginPath();
//      painter->rectangle(rect);

      switch (voice_state[i]) {
        case SAT_VOICE_OFF:       color = SAT_Black;        break;
        case SAT_VOICE_WAITING:   color = SAT_LightYellow;  break;
        case SAT_VOICE_PLAYING:   color = SAT_BrightGreen;  break;
        case SAT_VOICE_RELEASED:  color = SAT_Green;        break;
        case SAT_VOICE_FINISHED:  color = SAT_White;        break;
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
: public SAT_MenuWidget {

public:

  sa_tyr_MenuWidget(uint32_t AWidth, uint32_t AHeight, const char** AText, uint32_t ACount)
  : SAT_MenuWidget(SAT_Rect(0,0,AWidth,ACount*AHeight)) {

    for (uint32_t i=0; i<ACount; i++) {
      SAT_MenuItemWidget* w = new SAT_MenuItemWidget( SAT_Rect(0, i*AHeight, AWidth, AHeight ), AText[i] );
      //w->Layout.scaleMode = SAT_WIDGET_SCALE_MODE_INITIAL_RATIO;
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
: public SAT_PanelWidget {

public:

  sa_tyr_SectionPanelWidget(SAT_Rect ARect, const char* AText, uint32_t AHeaderHeight=20)
  : SAT_PanelWidget(ARect) {

    setFillBackground(true);
    setBackgroundColor(0.55);

    SAT_PanelWidget* text = new SAT_PanelWidget(SAT_Rect(ARect.x,ARect.y,ARect.w,AHeaderHeight));
    text->setText(AText);
    appendChildWidget(text);
    text->setTextAlignment(SAT_TEXT_ALIGN_CENTER);
    text->setTextSize(15);
    text->setDrawBorder(true);
    text->setTextColor(SAT_LightGrey);
    text->setDrawBorder(false);
    text->setFillBackground(true);
    text->setBackgroundColor(SAT_DarkGrey);
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------


class sa_tyr_SelectorWidget
: public SAT_SelectorWidget {

public:

  sa_tyr_SelectorWidget(SAT_Rect ARect, const char* AText, SAT_MenuWidget* AMenu)
  : SAT_SelectorWidget(ARect,AText,AMenu) {
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
: public SAT_DragValueWidget {

public:

  sa_tyr_DragValueWidget(SAT_Rect ARect, const char* AText, double AValue=0.0, bool ABipolar=false, double ASnapPos=0.5, double ASnapDist=0.05)
  : SAT_DragValueWidget(ARect,AText,AValue) {
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

//    setDrawModulation(true);

    //    setModulationColor(SAT_Color(1,0,0,0.25));
    //    setModulationOffset(SAT_Rect( 4,4,4,4 ));
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_tyr_SliderWidget
: public SAT_SliderWidget {

public:

  sa_tyr_SliderWidget(SAT_Rect ARect, const char* AText, double AValue=0.0, bool ABipolar=false, double ASnapPos=0.5, double ASnapDist=0.05)
  : SAT_SliderWidget(ARect,AText,AValue) {
    if (ABipolar) {
      setSnap(true);
      setSnapPos(ASnapPos);
      setSnapDist(ASnapDist);
      //setBipolar(true);
    }
    setDrawModulation(true);
    setModulationColor(SAT_Color(1,0,0,0.25));
    //setModulationOffset(SAT_Rect( 4,4,4,4 ));
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_tyr_BigKnobWidget
: public SAT_KnobWidget {

public:

  sa_tyr_BigKnobWidget(SAT_Rect ARect, const char* AText, double AValue=0.0, bool ABipolar=false, double ASnapPos=0.5, double ASnapDist=0.05)
  : SAT_KnobWidget(ARect,AText,AValue) { // ,true,false) {
    if (ABipolar) {
      MKnobWidget->setSnap(true);
      MKnobWidget->setSnapPos(ASnapPos);
      MKnobWidget->setSnapDist(ASnapDist);
      MKnobWidget->setBipolar(true);
    }
//    MLabelWidget->setTextSize(15);
    MKnobWidget->setModArcThickness(5);
    MKnobWidget->setModArcOffset(2);
    MKnobWidget->setArcThickness(10);
    MKnobWidget->setValueSize(12);
    MKnobWidget->setDrawModulation(true);
//    MKnobWidget->setModulationColor(SAT_Color(1,0,0,0.5));
    MKnobWidget->setModArcThickness(5);
    MKnobWidget->setModArcOffset(3);

  }

};

//----------

class sa_tyr_SmallKnobWidget
: public SAT_KnobWidget {

public:

  sa_tyr_SmallKnobWidget(SAT_Rect ARect, const char* AText, double AValue=0.0, bool ABipolar=false, double ASnapPos=0.5, double ASnapDist=0.05)
  : SAT_KnobWidget(ARect,AText,AValue) { // ,true,false) {
    if (ABipolar) {
      MKnobWidget->setSnap(true);
      MKnobWidget->setSnapPos(ASnapPos);
      MKnobWidget->setSnapDist(ASnapDist);
      MKnobWidget->setBipolar(true);
    }
    MKnobWidget->setArcThickness(5);
    MKnobWidget->setValueSize(8);
//    MLabelWidget->setTextSize(12);
    MKnobWidget->setDrawModulation(true);
//    MKnobWidget->setModulationColor(SAT_Color(1,0,0,0.5));
    MKnobWidget->setModArcThickness(3);
    MKnobWidget->setModArcOffset(1.5);
  }

};

//----------------------------------------------------------------------
#endif
