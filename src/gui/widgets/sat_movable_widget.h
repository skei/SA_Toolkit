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
  }

  //----------

  virtual ~SAT_MovableWidget() {
  }
  
//------------------------------
public:
//------------------------------

  void on_widget_mouse_click(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    if (AButton == SAT_BUTTON_LEFT) {
      MIsMoving = true;
      SAT_Rect mrect = getRect();
      MXClick = AXpos;
      MYClick = AYpos;
      
      //SAT_Rect border = getInnerBorder(); // parent!
      //border.scale(S);
      
      MXClickOffset = AXpos - mrect.x;
      MYClickOffset = AYpos - mrect.y;
      SAT_Print("offset %.1f,%.1f\n",MXClickOffset,MYClickOffset);
    }
  }
  
  //----------

  void on_widget_mouse_release(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    if (MIsMoving && (AButton == SAT_BUTTON_LEFT)) {
      MIsMoving = false;
      SAT_PRINT;
    }
  }

  //----------

  void on_widget_mouse_move(double AXpos, double AYpos, uint32_t AState, uint32_t ATime) override {
    if (MIsMoving) {
      SAT_Rect mrect = getRect();
      double S = getWindowScale();
      double invscale = 1.0 / S;
      // ugh.. need parent_inner_border!
      //SAT_Rect border = getInnerBorder();
      //mrect.x -= border.x;
      //mrect.y -= border.y;
      mrect = getRect();
      mrect.x = AXpos - MXClickOffset;
      mrect.y = AYpos - MYClickOffset;
      //setPos(x,y);
      setRect(mrect);
      // jumps by inner_border?
      mrect.scale(invscale);
      setBasisRect(mrect);
      //SAT_Print("realigning\n");
      //realignChildWidgets();      
      notify(SAT_WIDGET_NOTIFY_MOVED,0);
    }
  }

  
  
  
};

//----------------------------------------------------------------------
#endif