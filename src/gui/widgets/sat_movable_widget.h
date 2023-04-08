#ifndef sat_movable_widget_included
#define sat_movable_widget_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "gui/widgets/sat_panel_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_MovableWidget
: public SAT_PanelWidget {
  
//------------------------------
private:
//------------------------------

  bool MIsMoving        = false;
  double MXClick        = 0.0;
  double MYClick        = 0.0;
  double MXClickOffset  = 0.0;
  double MYClickOffset  = 0.0;

//------------------------------
public:
//------------------------------

  SAT_MovableWidget(SAT_Rect ARect)
  : SAT_PanelWidget(ARect) {
    setName("SAT_MovableWidget");
  }

  //----------

  virtual ~SAT_MovableWidget() {
  }
  
//------------------------------
public:
//------------------------------

  void on_widget_mouse_click(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    if (AButton == SAT_BUTTON_LEFT) {
      MIsMoving       = true;
      SAT_Rect mrect  = getRect();
      MXClick         = AXpos;
      MYClick         = AYpos;
      MXClickOffset   = AXpos - mrect.x;
      MYClickOffset   = AYpos - mrect.y;
    }
  }
  
  //----------

  void on_widget_mouse_release(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    if (MIsMoving && (AButton == SAT_BUTTON_LEFT)) {
      MIsMoving = false;
    }
  }

  //----------

  void on_widget_mouse_move(double AXpos, double AYpos, uint32_t AState, uint32_t ATime) override {
    if (MIsMoving) {
      SAT_Rect mrect = getRect();
      double S = getWindowScale();
      double invscale = 1.0 / S;
      mrect = getRect();
      mrect.x = AXpos - MXClickOffset;
      mrect.y = AYpos - MYClickOffset;
      setRect(mrect);
      mrect.scale(invscale);
      setBasisRect(mrect);
      //parentNotify(SAT_WIDGET_NOTIFY_REALIGN,0);
      do_widget_realign(this);
    }
  }

  
  
  
};

//----------------------------------------------------------------------
#endif
