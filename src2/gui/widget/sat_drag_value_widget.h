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

  bool    MIsDragging = false;
  double  MSensitivity  = (1.0 / 200.0);
  int32_t MMousePrevX   = 0;
  int32_t MMousePrevY   = 0;

//------------------------------
public:
//------------------------------

  SAT_DragValueWidget(SAT_Rect ARect)
  : SAT_ValueWidget(ARect) {
    setName("SAT_DragValueWidget");
  }

  //----------

  virtual ~SAT_DragValueWidget() {
  }

//------------------------------
public:
//------------------------------

//------------------------------
public: // on_widget
//------------------------------

  void on_widget_mouse_click(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    if (AButton == SAT_BUTTON_LEFT) {
      MIsDragging = true;
      MMousePrevX = AXpos;
      MMousePrevY = AYpos;
    }
    //SAT_TRACE;
  }

  //----------

  void on_widget_mouse_release(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    //SAT_TRACE;
    if (AButton == SAT_BUTTON_LEFT) {
      MIsDragging = false;
    }
  }

  //----------

  void on_widget_mouse_move(int32_t AXpos, int32_t AYpos, uint32_t AState, uint32_t ATime) override {
    if (MIsDragging) {

      double deltax = AXpos - MMousePrevX;
      double deltay = AYpos - MMousePrevY;
      double deltav = (deltay * MSensitivity);

      double value = getValue();
      double new_value = value - deltav;
      new_value = SAT_Clamp(new_value,0,1);

      if (new_value != value) {
        //SAT_TRACE;
        setValue(new_value);
        do_widget_update(this,SAT_WIDGET_UPDATE_VALUE);
        do_widget_redraw(this,SAT_WIDGET_REDRAW_PARAM);
      }

      MMousePrevX = AXpos;
      MMousePrevY = AYpos;
    }
  }

};

//---------------------------------------------------------------------
#endif

