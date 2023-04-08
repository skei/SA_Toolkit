#ifndef sat_drag_value_widget_included
#define sat_drag_value_widget_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "base/utils/sat_math.h"
#include "base/utils/sat_interpolation.h"
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

  bool      MIsDragging       = false;
  double    MClickedXpos      = 0.0;
  double    MClickedYpos      = 0.0;
  double    MPreviousXpos     = 0.0;
  double    MPreviousYpos     = 0.0;
  double    MDragValue        = 0.0;

  uint32_t  MDragDirection    = SAT_DIRECTION_UP;
  double    MDragSensitivity  = 0.001;
  double    MShiftSensitivity = 0.1;
  bool      MAutoHideCursor   = false;
  bool      MAutoLockCursor   = true;
  bool      MSnap             = false;
  double    MSnapPos          = 0.5;
  double    MSnapDist         = 0.1;
  double    MSnapSpeed        = 1.5;
  bool      MQuantize         = false;

  bool      MBipolar          = false;
  double    MBipolarCenter    = 0.5;
  
  bool      MWaitingForDrag   = false;

  //bool      MIsDraggingLeft   = false;
  //bool      MIsDraggingRight  = false;
  //bool      MHoverLeftEdge    = false;
  //bool      MHoverRightEdge   = false;

//------------------------------
public:
//------------------------------

  SAT_DragValueWidget(SAT_Rect ARect, const char* AText, double AValue)
  : SAT_ValueWidget(ARect,AText,AValue) {
    setName("SAT_DragValueWidget");
    
    //setCursor(SAT_CURSOR_ARROW_UP_DOWN);
    switch (MDragDirection) {
      case SAT_DIRECTION_UP:    setCursor(SAT_CURSOR_ARROW_UP_DOWN);     break;
      case SAT_DIRECTION_DOWN:  setCursor(SAT_CURSOR_ARROW_UP_DOWN);     break;
      case SAT_DIRECTION_LEFT:  setCursor(SAT_CURSOR_ARROW_LEFT_RIGHT);  break;
      case SAT_DIRECTION_RIGHT: setCursor(SAT_CURSOR_ARROW_LEFT_RIGHT);  break;
    }
    
  }

  //----------

  virtual ~SAT_DragValueWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setDragDirection(uint32_t ADir) {
    MDragDirection = ADir;
    switch (MDragDirection) {
      case SAT_DIRECTION_UP:    setCursor(SAT_CURSOR_ARROW_UP_DOWN);     break;
      case SAT_DIRECTION_DOWN:  setCursor(SAT_CURSOR_ARROW_UP_DOWN);     break;
      case SAT_DIRECTION_LEFT:  setCursor(SAT_CURSOR_ARROW_LEFT_RIGHT);  break;
      case SAT_DIRECTION_RIGHT: setCursor(SAT_CURSOR_ARROW_LEFT_RIGHT);  break;
    }
  }
  
  virtual void    setDragSensitivity(double ASens)    { MDragSensitivity = ASens; }
  virtual void    setShiftSensitivity(double ASens)   { MShiftSensitivity = ASens; }

  virtual void    setAutoHideCursor(bool AAuto)       { MAutoHideCursor = AAuto; }
  virtual void    setAutoLockCursor(bool AAuto)       { MAutoLockCursor = AAuto; }

  virtual void    setSnap(bool ASnap)                 { MSnap = ASnap; }
  virtual void    setSnapPos(double APos)             { MSnapPos = APos; }
  virtual void    setSnapDist(double ADist)           { MSnapDist = ADist; }
  virtual void    setSnapSpeed(double ASpeed)         { MSnapSpeed = ASpeed; }
  virtual void    setQuantize(bool AQuant)            { MQuantize = AQuant; }

  virtual void    setBipolar(bool ABipolar)           { MBipolar = ABipolar; }
  virtual void    setBipolarCenter(double APos)       { MBipolarCenter = APos; }
  virtual bool    isBipolar()                         { return MBipolar; }
  virtual double  getBipolarCenter()                  { return MBipolarCenter; }


//------------------------------
private:
//------------------------------

  double snapValue(double AValue) {
    double value = AValue;
    if (MSnapDist > 0) {
      double diff = abs(MSnapPos - value);
      if (diff < MSnapDist) {
        double s = 1.0 - (diff / MSnapDist); // 1 at snappos, 0 at snapdist
        s *= MSnapSpeed;
        s = SAT_Clamp(s,0,1);
        value = SAT_Interpolate_Linear(s,value,MSnapPos);
        //SAT_Print("sdiff %.3f s %.3f\n",sdiff,s);
      }
    }
    return value;
  }

//------------------------------
public:
//------------------------------


  //uint32_t getNumPopupMenuItems() override {
  //  return 3;
  //}
  
  //const char* getPopupMenuItem(uint32_t AIndex) override {
  //  switch (AIndex) {
  //    case 0: return "Item1"; break;
  //    case 1: return "Item2"; break;
  //    case 2: return "Item3"; break;
  //  }
  //  return nullptr;
  //}

//  uint32_t getNumPopupMenuItems() override {
//    return 3;
//  }
//  
//  //----------
//  
//  const char* getPopupMenuItem(uint32_t AIndex) override {
//    switch (AIndex) {
//      case 0: return "Item1"; break;
//      case 1: return "Item2"; break;
//      case 2: return "Item3"; break;
//    }
//    return nullptr;
//  }


//------------------------------
public:
//------------------------------

  /*
    we should set the MDragValue to the value needed for snapValue
    to result in the current value..
  */

  void on_widget_mouse_click(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    if (AButton == SAT_BUTTON_LEFT) {
      MIsDragging   = true;
      MClickedYpos  = AYpos;
      MClickedYpos  = AYpos;
      
      //if (MHoverLeftEdge) {
      //}
      //else if (MHoverRightEdge) {
      //}
      //else {
        MDragValue    = getValue();
        //if (MSnap) MDragValue = snapValue(MDragValue);
        MPreviousXpos = AXpos;
        MPreviousYpos = AYpos;
        MWaitingForDrag = true;
        //if (MAutoHideCursor) do_widget_set_cursor(this,SAT_CURSOR_HIDE);
        //if (MAutoLockCursor) do_widget_set_cursor(this,SAT_CURSOR_LOCK);
      //}
    }
  }

  //----------

  void on_widget_mouse_release(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    if (AButton == SAT_BUTTON_LEFT) {
      MIsDragging = false;
      MWaitingForDrag = false;
      //MHoverLeftEdge = false;
      //MHoverRightEdge = false;
      if (MAutoHideCursor) do_widget_set_cursor(this,SAT_CURSOR_SHOW);
      if (MAutoLockCursor) do_widget_set_cursor(this,SAT_CURSOR_UNLOCK);
    }
  }

  //----------

  void on_widget_mouse_move(double AXpos, double AYpos, uint32_t AState, uint32_t ATime) override {
    
    if (MWaitingForDrag) {
      MWaitingForDrag = false;
      if (MAutoHideCursor) do_widget_set_cursor(this,SAT_CURSOR_HIDE);
      if (MAutoLockCursor) do_widget_set_cursor(this,SAT_CURSOR_LOCK);
    }
    
    else if (MIsDragging) {
      double value = MDragValue;
      double sens = MDragSensitivity;
      if (AState & SAT_STATE_CTRL) sens *= MShiftSensitivity;
      double diff = 0;
      switch (MDragDirection) {
        case SAT_DIRECTION_UP:
          diff = MPreviousYpos - AYpos;
          value += (diff * sens);
          break;
        case SAT_DIRECTION_DOWN:
          diff = MPreviousYpos - AYpos;
          value -= (diff * sens);
          break;
        case SAT_DIRECTION_LEFT:
          diff = MPreviousXpos - AXpos;
          value += (diff * sens);
          break;
        case SAT_DIRECTION_RIGHT:
          diff = MPreviousXpos - AXpos;
          value -= (diff * sens);
          break;
      }
      MDragValue = SAT_Clamp(value,0,1);
      if (MSnap && !(AState & SAT_STATE_SHIFT)) {
        value = snapValue(value);
      }
      
      value = SAT_Clamp(value,0,1);
      setValue(value);
      do_widget_update(this,0);
      do_widget_redraw(this,0);
    }
    
    //else {
    //  // check if hover left/right edge?
    //  MHoverLeftEdge = false;
    //  MHoverRightEdge = false;
    //  // if...
    //}
    
    MPreviousXpos = AXpos;
    MPreviousYpos = AYpos;
  }

};

//----------------------------------------------------------------------
#endif



