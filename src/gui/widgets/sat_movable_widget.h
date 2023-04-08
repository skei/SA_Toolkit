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

  bool      MIsMoving       = false;
  bool      MIsSizing       = false;
  uint32_t  MSizingEdge     = SAT_EDGE_NONE;
  
  double    MXClick         = 0.0;
  double    MYClick         = 0.0;
  double    MXPrev          = 0.0;
  double    MYPrev         = 0.0;
  double    MXClickOffset   = 0.0;
  double    MYClickOffset   = 0.0;
  
  double    MHoverDist        = 5.0;
  uint32_t  MHoverEdge        = SAT_EDGE_NONE;
  
  bool      MCanResizeLeft    = true;
  bool      MCanResizeRoght   = true;
  bool      MCanResizeTop     = true;
  bool      MCanResizeBottom  = true;

//------------------------------
public:
//------------------------------

  SAT_MovableWidget(SAT_Rect ARect)
  : SAT_PanelWidget(ARect) {
    setName("SAT_MovableWidget");
    setAutoHoverCursor(true);
    setCursor(SAT_CURSOR_MOVE);
  }

  //----------

  virtual ~SAT_MovableWidget() {
  }
  
//------------------------------
public:
//------------------------------

  virtual uint32_t findEdge(double AXpos, double AYpos, double AMaxDist) {
    SAT_Rect mrect = getRect();
    uint32_t edge  = SAT_EDGE_NONE;
    double left_dist    = AXpos      - mrect.x;
    double right_dist   = mrect.x2() - AXpos;
    double top_dist     = AYpos      - mrect.y;
    double bottom_dist  = mrect.y2() - AYpos;
    if      (left_dist   < AMaxDist) edge = SAT_EDGE_LEFT;
    else if (right_dist  < AMaxDist) edge = SAT_EDGE_RIGHT;
    else if (top_dist    < AMaxDist) edge = SAT_EDGE_TOP;
    else if (bottom_dist < AMaxDist) edge = SAT_EDGE_BOTTOM;
    //else edge  = SAT_EDGE_NONE;
    return edge;
  }

//------------------------------
public:
//------------------------------

  void on_widget_mouse_click(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    if (AButton == SAT_BUTTON_LEFT) {
      
      SAT_Rect mrect  = getRect();
      MXPrev          = AXpos;
      MYPrev          = AYpos;
      MXClick         = AXpos;
      MYClick         = AYpos;
      MXClickOffset   = AXpos - mrect.x;
      MYClickOffset   = AYpos - mrect.y;

      if (MHoverEdge != SAT_EDGE_NONE) {
        MIsSizing = true;
        MSizingEdge = MHoverEdge;
      }
      else {
        MIsMoving = true;
      }
    }
  }
  
  //----------

  void on_widget_mouse_release(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    if (AButton == SAT_BUTTON_LEFT) {
      MIsMoving = false;
      MIsSizing = false;
    }
  }

  //----------

  void on_widget_mouse_move(double AXpos, double AYpos, uint32_t AState, uint32_t ATime) override {
    SAT_Rect mrect = getRect();
    double S = getWindowScale();
    double invscale = 1.0 / S;
    
    double xdiff = AXpos - MXPrev;
    double ydiff = AYpos - MYPrev;
    MXPrev = AXpos;
    MYPrev = AYpos;
    
    //SAT_Print("diff %.f,%.f\n",xdiff,ydiff);
    
    if (MIsMoving) {
      mrect.x = AXpos - MXClickOffset;
      mrect.y = AYpos - MYClickOffset;
      setRect(mrect);
      mrect.scale(invscale);
      setBasisRect(mrect);
      parentRealign();
    }
    
    else if (MIsSizing) {
      switch (MSizingEdge) {
        case SAT_EDGE_LEFT: {
          mrect.x += xdiff;
          mrect.w += -xdiff;
          break;
        }
        case SAT_EDGE_RIGHT: {
          mrect.w += xdiff;
          break;
        }
        case SAT_EDGE_TOP: {
          mrect.y += ydiff;
          mrect.h += -ydiff;
          break;
        }
        case SAT_EDGE_BOTTOM: {
          mrect.h += ydiff;
          break;
        }
      }
      setRect(mrect);
      mrect.scale(invscale);
      setBasisRect(mrect);
      parentRealign();
    }
    
    else {
      MHoverEdge = findEdge(AXpos,AYpos,MHoverDist);
      if      (MHoverEdge == SAT_EDGE_LEFT)   do_widget_set_cursor(this,SAT_CURSOR_ARROW_LEFT_RIGHT);
      else if (MHoverEdge == SAT_EDGE_RIGHT)  do_widget_set_cursor(this,SAT_CURSOR_ARROW_LEFT_RIGHT);
      else if (MHoverEdge == SAT_EDGE_TOP)    do_widget_set_cursor(this,SAT_CURSOR_ARROW_UP_DOWN);
      else if (MHoverEdge == SAT_EDGE_BOTTOM) do_widget_set_cursor(this,SAT_CURSOR_ARROW_UP_DOWN);
      else                                    do_widget_set_cursor(this,getMouseCursor());
    }
    
  }

  
  
  
};

//----------------------------------------------------------------------
#endif
