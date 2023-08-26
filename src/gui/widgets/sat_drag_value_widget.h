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
  bool      MWaitingForDrag   = false;

  //bool      MIsDraggingLeft   = false;
  //bool      MIsDraggingRight  = false;
  //bool      MHoverLeftEdge    = false;
  //bool      MHoverRightEdge   = false;
  
  double    MDragValue        = 0.0;

  uint32_t  MDragDirection    = SAT_DIRECTION_UP;
  double    MDragSensitivity  = 0.001;
  double    MShiftSensitivity = 0.1;
  bool      MAutoHideCursor   = true;
  bool      MAutoLockCursor   = true;

  bool      MSnap             = false;
  double    MSnapPos          = 0.5;
  double    MSnapDist         = 0.1;
  double    MSnapSpeed        = 1.5;

  bool      MQuantize         = false;
  uint32_t  MQuantizeSteps    = 1;

  bool      MBipolar          = false;
  double    MBipolarCenter    = 0.5;
  
  uint32_t  MNumValues        = 0; //1; = 0 = whole widget can be dragged, 1=closest..
  double    MHoverDistance    = 0.01;

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
  virtual void    setQuantizeSteps(uint32_t ASteps)   { MQuantizeSteps = ASteps; }

  virtual void    setBipolar(bool ABipolar)           { MBipolar = ABipolar; }
  virtual void    setBipolarCenter(double APos)       { MBipolarCenter = APos; }
  virtual bool    isBipolar()                         { return MBipolar; }
  virtual double  getBipolarCenter()                  { return MBipolarCenter; }

  virtual void    setNumValues(uint32_t ANum)         { MNumValues = ANum; }
  virtual void    setHoverDistance(double ADist)      { MHoverDistance = ADist; }
  
//------------------------------
public:
//------------------------------

  virtual uint32_t  getNumValues()  { return MNumValues; }

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
  
  //----------

  double quantizeValue(double AValue) {
    if (MQuantizeSteps > 1) {
      double qs = (double)(MQuantizeSteps + 1);
      double v = AValue * qs;
      return SAT_Trunc(v) / qs;
    }
    return AValue;
  }
  
  //----------
  
  
  // find closest (from mouse cursor) value
  // if two values are identical, will fid the first one..
  // todo: check if we're on the left or right side if values are equal (or similar)
  
  int32_t findClosestValue(double AXpos, double AYpos) {
    if (MNumValues == 0) {
      return 0;
    }
    else {
      double S = getWindowScale();
      SAT_Rect mrect = getRect();
      if (!mrect.contains(AXpos,AYpos)) return -2;
      double hoverdist = MHoverDistance * S;
      double range = mrect.w;
      if (range <= 0.0) return -2;
      double min_dist = range;
      int32_t index = -1;
      for (uint32_t i=0; i<MNumValues; i++) {
        
        double v = getValue(i);
        //double v = getSelectedValue(i);
        
        //double x  = mrect.x + (v * range);
        double mx = (AXpos - mrect.x) / range;
        double dist = abs(mx - v);
        //SAT_Print("i %i v %.3f x %.3f mx %.3f dist %.3f\n",i,v,x,mx,dist);
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
  

//------------------------------
public:
//------------------------------

  /*
    we should set the MDragValue to the value needed for snapValue
    to result in the current value..
  */

  void on_widget_mouse_click(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    if (AButton == SAT_BUTTON_LEFT) {
      
      //MIsDragging   = true;
      MClickedYpos  = AYpos;
      MClickedYpos  = AYpos;

      int32_t index = findClosestValue(AXpos,AYpos);
      if (index < 0) return;
      selectValue(index);
      
//      double value = getValue();
      double value = getSelectedValue();
      
      MDragValue = value;
      //if (MSnap) MDragValue = snapValue(MDragValue);
      
      MPreviousXpos = AXpos;
      MPreviousYpos = AYpos;
      MWaitingForDrag = true;
    }
  }

  //----------

  void on_widget_mouse_release(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    if (AButton == SAT_BUTTON_LEFT) {
      MIsDragging = false;
      MWaitingForDrag = false;
      //MHoverLeftEdge = false;
      //MHoverRightEdge = false;
      if (MAutoHideCursor) do_widgetListener_set_cursor(this,SAT_CURSOR_SHOW);
      if (MAutoLockCursor) do_widgetListener_set_cursor(this,SAT_CURSOR_UNLOCK);
    }
  }

  //----------

  void on_widget_mouse_move(double AXpos, double AYpos, uint32_t AState, uint32_t ATime) override {
    
    if (MWaitingForDrag) {
      MWaitingForDrag = false;
      MIsDragging = true;
      if (MAutoHideCursor) do_widgetListener_set_cursor(this,SAT_CURSOR_HIDE);
      if (MAutoLockCursor) do_widgetListener_set_cursor(this,SAT_CURSOR_LOCK);
    }
    
    else if (MIsDragging) {

      int32_t index = getSelectedValueIndex();
      double value = MDragValue;
      
      int32_t index2 = 1 - index;
      double value2 = getValue(index2);
      
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
      
      if (MQuantize && !(AState & SAT_STATE_SHIFT)) value = quantizeValue(value);
      if (MSnap && !(AState & SAT_STATE_SHIFT)) value = snapValue(value);
      value = SAT_Clamp(value,0,1);
      
      if (MNumValues == 2) {
        
      if (index == 0) {
        if (value > value2) {
          if (AState & SAT_STATE_ALT) {
            // push
            setValue(value,index2);
            do_widgetListener_update(this,0,index2);
            do_widgetListener_redraw(this,0,index2);
          }
          else {
            // stop
            value = value2;
          }
        }
      }
      else if (index == 1) {
        if (value < value2) {
          if (AState & SAT_STATE_ALT) {
            // push
            setValue(value,index2);
            do_widgetListener_update(this,0,index2);
            do_widgetListener_redraw(this,0,index2);
          }
          else {
            // stop
            value = value2;
          }
        }
      }
      
      } // numparams > 0
      
      //setSelectedValue(value);
      setValue(value,index);
      do_widgetListener_update(this,0,index);
      do_widgetListener_redraw(this,0,index);
      
    } // dragging
    
    else {
      int32_t index = findClosestValue(AXpos,AYpos);
      //SAT_Print("closest: %i\n",index);
      if (index >= 0) do_widgetListener_set_cursor(this,getMouseCursor());
      else do_widgetListener_set_cursor(this,SAT_CURSOR_DEFAULT);
    }
    
    MPreviousXpos = AXpos;
    MPreviousYpos = AYpos;
  }
  
  //----------
  
  void on_widget_mouse_enter(SAT_BaseWidget* AFrom, double AXpos, double AYpos, uint32_t ATimestamp) override {
    if (MNumValues > 1) {
      int32_t index = findClosestValue(AXpos,AYpos);
      //SAT_Print("closest: %i\n",index);
      if (index >= 0) do_widgetListener_set_cursor(this,getMouseCursor());
      else do_widgetListener_set_cursor(this,SAT_CURSOR_DEFAULT);
    }
    else {
      SAT_ValueWidget::on_widget_mouse_enter(AFrom,AXpos,AYpos,ATimestamp);
    }
  }

  //----------
  
  void on_widget_mouse_leave(SAT_BaseWidget* ATo, double AXpos, double AYpos, uint32_t ATimestamp) override {
    if (MNumValues > 1) {
      do_widgetListener_set_cursor(this,SAT_CURSOR_DEFAULT);
    }
    else {
      SAT_ValueWidget::on_widget_mouse_leave(ATo,AXpos,AYpos,ATimestamp);
    }
  }
  

};

//----------------------------------------------------------------------
#endif



