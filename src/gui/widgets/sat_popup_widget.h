#ifndef sat_popup_widget_included
#define sat_popup_widget_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "gui/widgets/sat_panel_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_PopupWidget
: public SAT_PanelWidget {

//------------------------------
private:
//------------------------------


//------------------------------
public:
//------------------------------

  SAT_PopupWidget(SAT_Rect ARect)
  : SAT_PanelWidget(ARect) {
  }

  //----------

  virtual ~SAT_PopupWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual bool open(SAT_Rect ARect) {
    do_widget_set_state(this,SAT_WIDGET_STATE_MODAL);
    setPos(ARect.x,ARect.y);
    setSize(ARect.w,ARect.h);
    setActive(true);
    setVisible(true);
    redraw();
    return true;
  }

  //----------

  virtual void close() {
    do_widget_set_state(this,SAT_WIDGET_STATE_NORMAL);
    setActive(false);
    setVisible(false);
    redraw();
  }

//------------------------------
public:
//------------------------------

  // close if:
  // - left click outside
  // - right click

  //void on_widget_mouse_click(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
  //  if (AButton == SAT_BUTTON_RIGHT) close();
  //}

  //----------

  // close if:
  // - esc pressed

  //void on_widget_key_press(uint32_t AKey, uint32_t AState, uint32_t ATime) override {
  //  if (AKey == SAT_KEY_ESC) close();
  //}

//------------------------------
public:
//------------------------------


};

//----------------------------------------------------------------------
#endif


