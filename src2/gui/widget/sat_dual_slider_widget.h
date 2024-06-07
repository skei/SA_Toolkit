#ifndef sat_dual_slider_widget_included
#define sat_dual_slider_widget_included
//----------------------------------------------------------------------

#include "gui/widget/sat_slider_widget.h"

//----------------------------------------------------------------------

class SAT_DualSliderWidget
: public SAT_SliderWidget {

//------------------------------
private:
//------------------------------

  uint32_t  MHoverDistance = 5;

//------------------------------
public:
//------------------------------

  SAT_DualSliderWidget(SAT_Rect ARect)
  : SAT_SliderWidget(ARect) {
    setName("SAT_DualSliderWidget");
    setCursor(SAT_CURSOR_ARROW_LEFT_RIGHT);
    setNumValues(2);
  }

  //----------

  virtual ~SAT_DualSliderWidget() {
  }

  //----------

  // find closest (from mouse cursor) value
  // if two values are identical, will fid the first one..
  // todo: check if we're on the left or right side if values are equal (or similar)
  // todo: ..ClosestValueHorizontal

  int32_t findClosestValue(double AXpos, double AYpos, bool AHorizontal=true) {
    //if (getNumValues() == 0) return 0;
    //else if (getNumValues() == 1) return getValue();
    if (getNumValues() < 2) {
      return 0;
    }
    else {
      double S = getWindowScale();
      SAT_Rect mrect = getRect();
      if (!mrect.contains(AXpos,AYpos)) return -2;
      double hoverdist = MHoverDistance * S;
      double range;
      double pos;
      if (AHorizontal) {
        range = mrect.w;
        if (range <= 0.0) return -2;
        pos = (AXpos - mrect.x) / range;
      }
      else { // vertical
        range = mrect.h;
        if (range <= 0.0) return -2;
        pos = (AYpos - mrect.y) / range;
      }
      double min_dist = range;
      int32_t index = -1;
      for (uint32_t i=0; i<getNumValues(); i++) {
        double val = getValue(i);
        double dist = abs(pos - val);
        if (dist < hoverdist) {
          if (dist < min_dist) {
            min_dist = dist;
            index = i;
          }
        }
      }
      return index;
    }
  }

};

  // virtual void setValue(double AValue)                          { MValues[MValueIndex] = AValue; }
  // virtual void setValue(double AValue, uint32_t AIndex)         { MValues[AIndex] = AValue; }
  // virtual void setValueIndex(uint32_t AIndex)                   { MValueIndex = AIndex; }


//----------------------------------------------------------------------
#endif