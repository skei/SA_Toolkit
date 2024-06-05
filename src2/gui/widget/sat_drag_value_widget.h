#ifndef sat_drag_value_widget_included
#define sat_drag_value_widget_included
//---------------------------------------------------------------------

#include "sat.h"
#include "gui/widget/sat_value_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_DragValueWidget
: public SAT_ValueWidget {

//------------------------------
private:
//------------------------------

  bool      MIsDragging       = false;
  double    MSensitivity      = (1.0 / 200.0);
  double    MSensitivity2     = (1.0 / 20.0);
  int32_t   MClickedX         = 0;
  int32_t   MClickedY         = 0;

  uint32_t  MStartDragButton  = SAT_BUTTON_LEFT;
  uint32_t  MPreciseKey       = SAT_STATE_CTRL;
  uint32_t  MUnsnapKey        = SAT_STATE_SHIFT;

  uint32_t  MPrevPreciseKey   = SAT_STATE_NONE;
  uint32_t  MPrevUnsnapKey    = SAT_STATE_NONE;

  bool      MBipolar          = false;
  double    MBipolarCenter    = 0.5;

  bool      MQuantize         = false;
  uint32_t  MQuantizeSteps    = 5;

  bool      MSnap             = true;
  double    MSnapPos          = 0.5;
  double    MSnapDist         = 0.2;
  double    MSnapSpeed        = 1.5;

  double    MDragValue        = 0.0;

//------------------------------
public:
//------------------------------

  SAT_DragValueWidget(SAT_Rect ARect)
  : SAT_ValueWidget(ARect) {
    setName("SAT_DragValueWidget");
    setCursor(SAT_CURSOR_ARROW_UP_DOWN);
  }

  //----------

  virtual ~SAT_DragValueWidget() {
  }

//------------------------------
protected:
//------------------------------

  // returns 0..1

  double calcValue() {
    double value = getValue();
    //SAT_PRINT("value %.3f\n",value);
    SAT_Parameter* param = (SAT_Parameter*)getParameter();
    //SAT_PRINT("param %p\n",param);
    if (param) {
      value = param->getNormalizedValue();
      //value = param->normalize(value);
      //SAT_PRINT("normalized value %.3f\n",value);
    }
    return value;
  }

  //----------

  // returns 0..1 (final modulated and clamped value)

  //double calcModulation(double value) {
  double calcModulation() {
    double modulation = 0.0;//getValue();
    SAT_Parameter* param = (SAT_Parameter*)getParameter();
    if (param) {
      modulation = param->getValue() + param->getModulation();
      modulation = param->normalize(modulation);
      modulation = SAT_Clamp(modulation,0,1);
    }
    return modulation;
  }

  //----------

  // float SAT_Interpolate_Linear(float t, float y1,float y2) {
  //   return ( y1*(1-t) + y2*t );
  // }  

  double snapValue(double AValue) {
    double value = AValue;
    if (MSnapDist > 0) {
      double diff = abs(MSnapPos - value);
      if (diff < MSnapDist) {
        // 1 at snappos, 0 at snapdist
        double s = 1.0 - (diff / MSnapDist);
        s *= MSnapSpeed;
        s = SAT_Clamp(s,0,1);
        //value = SAT_Interpolate_Linear(s,value,MSnapPos);
        value = value * (1 - s) + MSnapPos * s;
      }
    }
    return value;
  }
  
  //----------

  double quantizeValue(double AValue) {
    if (MQuantizeSteps > 1) {
      double qsteps = (double)(MQuantizeSteps - 1);
      double qval = AValue * (double)MQuantizeSteps;
      double q = SAT_Trunc(qval) / qsteps;
      return SAT_Clamp(q,0,1);
    }
    return AValue;
  }
  
  //----------

  // find closest (from mouse cursor) value
  // if two values are identical, will fid the first one..
  // todo: check if we're on the left or right side if values are equal (or similar)
  // todo: ..ClosestValueHorizontal

  // int32_t findClosestValue(double AXpos, double AYpos, bool AHorizontal=true) {
  //   //if (getNumValues() == 0) return 0;
  //   //else if (getNumValues() == 1) return getValue();
  //   if (getNumValues() < 2) return 0;
  //   else {
  //     double S = getWindowScale();
  //     SAT_Rect mrect = getRect();
  //     if (!mrect.contains(AXpos,AYpos)) return -2;
  //     double hoverdist = MHoverDistance * S;
  //     double range;
  //     double pos;
  //     if (AHorizontal) {
  //       range = mrect.w;
  //       if (range <= 0.0) return -2;
  //       pos = (AXpos - mrect.x) / range;
  //     }
  //     else {
  //       range = mrect.h;
  //       if (range <= 0.0) return -2;
  //       pos = (AYpos - mrect.y) / range;
  //     }
  //     double min_dist = range;
  //     int32_t index = -1;
  //     for (uint32_t i=0; i<getNumValues(); i++) {
  //       double val = getValue(i);
  //       double dist = abs(pos - val);
  //       if (dist < hoverdist) {
  //         if (dist < min_dist) {
  //           min_dist = dist;
  //           index = i;
  //         }
  //       }
  //     }
  //     return index;
  //   }
  // }

//------------------------------
public: // on_widget
//------------------------------

  void on_widget_mouse_click(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    if (AButton == MStartDragButton) {
      MIsDragging = true;
      MClickedX = AXpos;
      MClickedY = AYpos;
      do_widget_set_cursor(this,SAT_CURSOR_LOCK);
      MDragValue = calcValue();
    }
  }

  //----------

  void on_widget_mouse_release(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    if (AButton == MStartDragButton) {
      MIsDragging = false;
      do_widget_set_cursor(this,SAT_CURSOR_UNLOCK);
    }
  }

  //----------

  void on_widget_mouse_move(int32_t AXpos, int32_t AYpos, uint32_t AState, uint32_t ATime) override {
    if (MIsDragging) {

      bool unsnap_key = (AState & MUnsnapKey);
      if (unsnap_key) {
        if (!MPrevUnsnapKey) {
          //SAT_PRINT("unsnap_key pressed\n");
          MDragValue = getValue();
        }
      }
      else {
        if (MPrevUnsnapKey) {
          //SAT_PRINT("unsnap_key released\n");
          MDragValue = getValue();
        }
      }
      MPrevUnsnapKey = unsnap_key;

      // value

      double value = MDragValue;

      // sensitivity

      double sensitivity = MSensitivity;

      bool precise_key = (AState & MPreciseKey);
      if (precise_key) {
        if (!MPrevPreciseKey) {
          SAT_PRINT("precise_key pressed\n");
        }
        sensitivity *= MSensitivity2;
      }
      else {
        if (MPrevPreciseKey) {
          SAT_PRINT("precise_key released\n");
        }
      }
      MPrevPreciseKey = precise_key;

      // delta

      double deltax = AXpos - MClickedX;
      double deltay = AYpos - MClickedY;
      double delta  = (deltay * sensitivity);

      double new_value = value - delta;
      new_value = SAT_Clamp(new_value,0,1);

      MDragValue = new_value;

      // quantize

      if (MQuantize) {
        if (!(AState & MUnsnapKey)) {
          new_value = quantizeValue(new_value);
        }
      }

      // snap

      if (MSnap) {
        if (!(AState & MUnsnapKey)) {
          new_value = snapValue(new_value);
        }
      }

      SAT_PRINT("MDragValue %.3f new_value = %.3f\n", MDragValue, new_value );


      // set


      // SAT_PRINT("new_value %.3f MDragValue %.3f\n",new_value,MDragValue);

      if (new_value != value) {
        //setValue(new_value);
        setValue(new_value);
        do_widget_update(this,SAT_WIDGET_UPDATE_VALUE);
        do_widget_redraw(this,SAT_WIDGET_REDRAW_PARAM);
      }
    }
  }

};

//---------------------------------------------------------------------
#endif

