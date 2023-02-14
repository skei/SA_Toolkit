#ifndef sat_drag_value_widget_included
#define sat_drag_value_widget_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "base/utils/sat_math.h"
#include "gui/widgets/sat_value_widget.h"

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

  bool    MIsDragging       = false;
  double  MClickedXpos      = 0.0;
  double  MClickedYpos      = 0.0;
  double  MPreviousXpos     = 0.0;
  double  MPreviousYpos     = 0.0;

  double  MDragValue        = 0.0;
  double MDragSensitivity   = 0.004;
  double MShiftSensitivity  = 0.05;

//------------------------------
public:
//------------------------------

  SAT_DragValueWidget(SAT_Rect ARect, const char* AText, double AValue)
  : SAT_ValueWidget(ARect,AText,AValue) {
  }

  virtual ~SAT_DragValueWidget() {
  }

//------------------------------
public:
//------------------------------


//------------------------------
public:
//------------------------------



//------------------------------
public:
//------------------------------

  void on_widget_mouse_click(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    if (AButton == SAT_BUTTON_LEFT) {
      MIsDragging   = true;
      MClickedYpos  = AYpos;
      MClickedYpos  = AYpos;
      MDragValue    = getValue();
      MPreviousXpos = AXpos;
      MPreviousYpos = AYpos;
    }
  }

  void on_widget_mouse_release(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    if (AButton == SAT_BUTTON_LEFT) {
      MIsDragging = false;
    }
  }

  void on_widget_mouse_move(double AXpos, double AYpos, uint32_t AState, uint32_t ATime) override {
    if (MIsDragging) {
      double diff = (MPreviousYpos - AYpos);
      diff *= MDragSensitivity;
      if (AState & SAT_STATE_SHIFT) diff *= MShiftSensitivity;
      MDragValue += diff;
      MDragValue = SAT_Clamp(MDragValue,0,1);
      setValue(MDragValue);
      do_widget_update(this,0);
      do_widget_redraw(this,0);
    }
    MPreviousXpos = AXpos;
    MPreviousYpos = AYpos;
  }

};

//----------------------------------------------------------------------
#endif



