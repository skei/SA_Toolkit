#ifndef sat_movable_widget_included
#define sat_movable_widget_included
//----------------------------------------------------------------------

#include "sat.h"
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

  bool      MIsMovable          = true;
  SAT_Rect  MMovableOffset      = SAT_Rect(0,50,0,0);
  uint32_t  MMovableDirections  = SAT_DIRECTION_ALL;
  uint32_t  MMovableFlags       = 2; // 1 = limit_to_parent, 2 = invert_pffset

  bool      MIsSizable          = true;
  SAT_Rect  MSizableBorder      = SAT_Rect(10,10,10,10);
  uint32_t  MSizableEdges       = SAT_EDGE_ALL;
  uint32_t  MSizableFlags       = 0; // 1 =limit_to_parent

//------------------------------
private:
//------------------------------

  bool      MCanMove            = false;
  uint32_t  MResizeEdge         = SAT_EDGE_NONE;
  bool      MIsMoving           = false;
  bool      MIsResizing         = false;
  double    MPrevX              = 0.0;
  double    MPrevY              = 0.0;
  double    MMovedX             = 0.0;
  double    MMovedY             = 0.0;

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

  bool      isMovable()             { return MIsMovable; }
  SAT_Rect  getMovableOffset()      { return  MMovableOffset; }
  uint32_t  getMovableDirections()  { return MMovableDirections; }
  uint32_t  getMovableFlags()       { return MMovableFlags; }

  bool      isSizable()             { return MIsMovable; }
  SAT_Rect  getSizableBorder()      { return MSizableBorder; }
  uint32_t  getSizableEdges()       { return MSizableEdges; }
  uint32_t  getSizableFlags()       { return MSizableFlags; }

  void      setIsMovable(bool AMovable=true)            { MIsMovable = AMovable; }
  void      setMovableOffset(SAT_Rect AOffset)          { MMovableOffset = AOffset; }
  void      setMovableDirections(uint32_t ADirections)  { MMovableDirections = ADirections; }
  void      setMovableFlags(uint32_t AFlags)            { MMovableFlags = AFlags; }

  void      setIsSizable(bool ASizable=true)            { MIsMovable = ASizable; }
  void      setSizableBorder(SAT_Rect ABorder)          { MSizableBorder = ABorder; }
  void      setSizableEdges(uint32_t AEdges)            { MSizableEdges = AEdges; }
  void      setSizableFlags(uint32_t AFlags)            { MSizableFlags = AFlags; }

//------------------------------
private:
//------------------------------

  // returns edge that is (potentially) resizable

  uint32_t checkCanResize(double AXpos, double AYpos) {
    if (MIsSizable) {
      //double S = getWindowScale();
      SAT_Rect mrect = getRect();
      SAT_Rect offset = MSizableBorder;
      //offset.scale(S);
      mrect.shrink(offset);
      if (mrect.contains(AXpos,AYpos))      return SAT_EDGE_NONE;
      if ((MSizableEdges & SAT_EDGE_LEFT)   && (AXpos < mrect.x))     return SAT_EDGE_LEFT;
      if ((MSizableEdges & SAT_EDGE_RIGHT)  && (AXpos > mrect.x2()))  return SAT_EDGE_RIGHT;
      if ((MSizableEdges & SAT_EDGE_TOP)    && (AYpos < mrect.y))     return SAT_EDGE_TOP;
      if ((MSizableEdges & SAT_EDGE_BOTTOM) && (AYpos > mrect.y2()))  return SAT_EDGE_BOTTOM;
    }
    return SAT_EDGE_NONE;
  }

  //----------

  // returns true if we can resize

  bool checkCanMove(double AXpos, double AYpos) {
    if (MIsMovable) {
      //double S = getWindowScale();
      SAT_Rect mrect = getRect();
      SAT_Rect offset = MMovableOffset;
      //offset.scale(S);
      mrect.shrink(offset);
      if (mrect.contains(AXpos,AYpos)) {
        if (MMovableFlags & 2) return false;
        else return true;
      }
    }
    if (MMovableFlags & 2) return true;
    else return false;
  }

  //----------

  void checkHover(double AXpos, double AYpos) {
    MResizeEdge = checkCanResize(AXpos,AYpos);
    if (MResizeEdge != SAT_EDGE_NONE) {
      if (MResizeEdge & SAT_EDGE_LEFT)    { do_widget_setCursor(this,SAT_CURSOR_ARROW_LEFT);  return; }
      if (MResizeEdge & SAT_EDGE_RIGHT)   { do_widget_setCursor(this,SAT_CURSOR_ARROW_RIGHT);  return; }
      if (MResizeEdge & SAT_EDGE_TOP)     { do_widget_setCursor(this,SAT_CURSOR_ARROW_UP);     return; }
      if (MResizeEdge & SAT_EDGE_BOTTOM)  { do_widget_setCursor(this,SAT_CURSOR_ARROW_DOWN);     return; }
    }
    MCanMove = checkCanMove(AXpos,AYpos);
    if (MCanMove) {
      if (MMovableDirections == SAT_DIRECTION_ALL)        { do_widget_setCursor(this,SAT_CURSOR_MOVE);              return; }
      else if (MMovableDirections == SAT_DIRECTION_HORIZ) { do_widget_setCursor(this,SAT_CURSOR_ARROW_LEFT_RIGHT);  return; }
      else if (MMovableDirections == SAT_DIRECTION_VERT)  { do_widget_setCursor(this,SAT_CURSOR_ARROW_UP_DOWN);     return; }
    }
    do_widget_setCursor(this,SAT_CURSOR_DEFAULT);
  }

//------------------------------
public:
//------------------------------

  void on_widget_mouseClick(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp) override {
    MPrevX = AXpos;
    MPrevY = AYpos;
    if (AButton == SAT_BUTTON_LEFT) {
      if (MIsSizable && (MResizeEdge != SAT_EDGE_NONE)) {
        MIsResizing = true;
        SAT_Print("MIsMoving %i MIsResizing %i\n",MIsMoving,MIsResizing);
      }
      else if (MIsMovable && MCanMove) {
        MIsMoving = true;
        SAT_Print("MIsMoving %i MIsResizing %i\n",MIsMoving,MIsResizing);
      }
    }
  }

  //----------

  void on_widget_mouseRelease(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp) override {
    SAT_Print("MIsMoving 0 MIsResizing 0\n");
    MIsMoving = false;
    MIsResizing = false;
  }

  //----------

  /*
    mrect contains layout, so if we move, and then sets new rect/base based on
    this.. the base rect will already contain borders, so next time it is aligned,
    borders will reapply.. on and on..
    we can only have 
  */

  void on_widget_mouseMove(double AXpos, double AYpos, uint32_t AState, uint32_t ATimeStamp) override {
    if (MIsMoving) {
      SAT_Rect mrect = getRect();
      double deltax = AXpos - MPrevX;
      double deltay = AYpos - MPrevY;
      if (MMovableDirections & SAT_DIRECTION_HORIZ) mrect.x += deltax;
      if (MMovableDirections & SAT_DIRECTION_VERT)  mrect.y += deltay;

      //SAT_Point offset = getLayoutOffset();
      //mrect.x -= offset.x;
      //mrect.y -= offset.y;

      setRectAndBase(mrect);
      do_widget_realign(this);
    }
    else if (MIsResizing) {
      SAT_Rect mrect = getRect();
      double deltax = AXpos - MPrevX;
      double deltay = AYpos - MPrevY;
      if (MResizeEdge & SAT_EDGE_LEFT)    { mrect.removeLeft(deltax); }
      if (MResizeEdge & SAT_EDGE_RIGHT)   { mrect.addRight(deltax);   }
      if (MResizeEdge & SAT_EDGE_TOP)     { mrect.removeTop(deltay);  }
      if (MResizeEdge & SAT_EDGE_BOTTOM)  { mrect.addBottom(deltay);  }
      setRectAndBase(mrect);
      do_widget_realign(this);
    }
    else {
      checkHover(AXpos,AYpos);
    }
    MPrevX = AXpos;
    MPrevY = AYpos;
  }

  //----------

  //void on_widget_enter(SAT_Widget* AFrom, double AXpos, double AYpos) override {
  //  checkHover(AXpos,AYpos);
  //}

  //----------

  //void on_widget_leave(SAT_Widget* ATo, double AXpos, double AYpos) override {
  //}

};


//----------------------------------------------------------------------
#endif