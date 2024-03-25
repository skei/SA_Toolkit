#ifndef sat_drag_value_widget_included
#define sat_drag_value_widget_included
//----------------------------------------------------------------------

#include "sat.h"
#include "base/utils/sat_math.h"
#include "base/utils/sat_interpolation.h"
#include "gui/widgets/sat_panel_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_DragValueWidget
: public SAT_PanelWidget {

//------------------------------
private:
//------------------------------

  bool      MIsDragging       = false;
  double    MClickedXpos      = 0.0;
  double    MClickedYpos      = 0.0;
  double    MPreviousXpos     = 0.0;
  double    MPreviousYpos     = 0.0;
  bool      MWaitingForDrag   = false;
  double    MDragValue        = 0.0;
  uint32_t  MDragIndex        = 0;
  uint32_t  MDragDirection    = SAT_DIRECTION_UP;
  double    MDragSensitivity  = 0.001;
  double    MShiftSensitivity = 0.1;
  bool      MAutoHideCursor   = true;
  bool      MAutoLockCursor   = true;
  bool      MSnap             = false;
  double    MSnapPos          = 0.5;
  double    MSnapDist         = 0.2;
  double    MSnapSpeed        = 1.5;
  bool      MQuantize         = false;
  uint32_t  MQuantizeSteps    = 1;
  bool      MBipolar          = false;
  double    MBipolarCenter    = 0.5;
  //uint32_t  MNumValues        = 0;
  double    MHoverDistance    = 0.01;

//------------------------------
public:
//------------------------------

  SAT_DragValueWidget(SAT_Rect ARect, const char* AText, double AValue)
  : SAT_PanelWidget(ARect) {
    setName("SAT_DragValueWidget");
    setFillBackground(true);
    setBackgroundColor(0.25);
    setDrawText(true);
    setText(AText);
    setTextSize(10);
    setTextColor(SAT_LightGrey);
    setTextAlignment(SAT_TEXT_ALIGN_LEFT);
    setDrawValue(true);
    setValue(AValue);
    setValueSize(10);
    setValueColor(SAT_LightestGrey);
    setDrawBorder(false);
    switch (MDragDirection) {
      case SAT_DIRECTION_UP:    setCursor(SAT_CURSOR_ARROW_UP_DOWN);     break;
      case SAT_DIRECTION_DOWN:  setCursor(SAT_CURSOR_ARROW_UP_DOWN);     break;
      case SAT_DIRECTION_LEFT:  setCursor(SAT_CURSOR_ARROW_LEFT_RIGHT);  break;
      case SAT_DIRECTION_RIGHT: setCursor(SAT_CURSOR_ARROW_LEFT_RIGHT);  break;
    }

    setDrawParamText(true);

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
  //virtual void    setNumValues(uint32_t ANum)         { MNumValues = ANum; }
  virtual void    setHoverDistance(double ADist)      { MHoverDistance = ADist; }
  
//------------------------------
public:
//------------------------------

  // virtual uint32_t  getNumValues()  {
  //   return MNumValues;
  // }

//------------------------------
private:
//------------------------------

  double snapValue(double AValue) {
    double value = AValue;
    if (MSnapDist > 0) {
      double diff = abs(MSnapPos - value);
      if (diff < MSnapDist) {
        // 1 at snappos, 0 at snapdist
        double s = 1.0 - (diff / MSnapDist);
        s *= MSnapSpeed;
        s = SAT_Clamp(s,0,1);
        value = SAT_Interpolate_Linear(s,value,MSnapPos);
      }
    }
    return value;
  }
  
  //----------

  double quantizeValue(double AValue) {
    if (MQuantizeSteps > 1) {
      double qs = (double)(MQuantizeSteps - 1);
      double v = AValue * (double)MQuantizeSteps;
      return SAT_Trunc(v) / qs;
    }
    return AValue;
  }
  
  //----------
  
  // find closest (from mouse cursor) value
  // if two values are identical, will fid the first one..
  // todo: check if we're on the left or right side if values are equal (or similar)
  // todo: ..ClosestValueHorizontal

  int32_t findClosestValue(double AXpos, double AYpos, bool AHorizontal=true) {
    if (getNumValues() == 0) return 0;
    else if (getNumValues() == 1) return getValue();
    else {
      double S = getWindowScale();
      SAT_Rect mrect = getRect();
      if (!mrect.contains(AXpos,AYpos)) return -2;
      double hoverdist = MHoverDistance * S;

      double range;
      double pos;

      if (AHorizontal) {
        range = mrect.w;
        if (range <= 0.0) return -2;
        pos = (AXpos - mrect.x) / range;
      }
      else {
        range = mrect.h;
        if (range <= 0.0) return -2;
        pos = (AYpos - mrect.y) / range;
      }

      double min_dist = range;
      int32_t index = -1;

      for (uint32_t i=0; i<getNumValues(); i++) {
        double val = getValue(i);
        double dist = abs(pos - val);
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

  // we should set the MDragValue to the value needed for snapValue
  // to result in the current value..

  void on_widget_mouseClick(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    if (AButton == SAT_BUTTON_LEFT) {
      setInteracting(true);
      MClickedYpos  = AYpos;
      MClickedYpos  = AYpos;
      int32_t index = findClosestValue(AXpos,AYpos,true);

//SAT_Print("index %i\n",index);

      if (index < 0) return;
      //selectValue(index);
      //double value = getSelectedValue();
      double value = getValue(index);

//SAT_Print("index %i value %.2f\n",index,value);

      MDragValue = value;
      MDragIndex = index;
      MPreviousXpos = AXpos;
      MPreviousYpos = AYpos;
      MWaitingForDrag = true;
    }
  }

  //----------

  void on_widget_mouseRelease(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    if (AButton == SAT_BUTTON_LEFT) {
      setInteracting(false);
      do_widget_redraw(this,0,0);
      MIsDragging = false;
      MWaitingForDrag = false;
      if (MAutoHideCursor) do_widget_setCursor(this,SAT_CURSOR_SHOW);
      if (MAutoLockCursor) do_widget_setCursor(this,SAT_CURSOR_UNLOCK);
    }
  }

  //----------

  void on_widget_mouseMove(double AXpos, double AYpos, uint32_t AState, uint32_t ATime) override {
    if (MWaitingForDrag) {
      MWaitingForDrag = false;
      MIsDragging = true;
      if (MAutoHideCursor) do_widget_setCursor(this,SAT_CURSOR_HIDE);
      if (MAutoLockCursor) do_widget_setCursor(this,SAT_CURSOR_LOCK);
    }
    else if (MIsDragging) {
      
      int32_t index = MDragIndex; // getSelectedValueIndex();    // selected
      double value = MDragValue;
      int32_t index2 = 1 - index;                 // the other one
      double value2 = getValue(index2);

//SAT_Print("index %i value %.2f index2 %i value2 %.2f\n",index,value,index2,value2);

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

      if (getNumValues() == 2) {

SAT_Print("index %i value %.3f\n",index,value);

        if (index == 0) {
          // if (value > value2) {
          //   if (AState & SAT_STATE_ALT) {
          //     // push
          //     // setValue(value,index);
          //     // do_widget_update(this,AState);
          //     // do_widget_redraw(this);
          //   }
          //   else {
          //     // stop
          //     value = value2;
          //     // setValue(value,index);
          //     // do_widget_update(this,AState);
          //     // do_widget_redraw(this);
          //   }
          // }
          setValue(value,index);
          do_widget_update(this,index,AState);
          do_widget_redraw(this,index,0);
        }

        else if (index == 1) {
          // if (value < value2) {
          //   if (AState & SAT_STATE_ALT) {
          //     // push
          //     // setValue(value,index);
          //     // do_widget_update(this,AState);
          //     // do_widget_redraw(this);
          //   }
          //   else {
          //     // stop
          //     value = value2;
          //     // setValue(value,index);
          //     // do_widget_update(this,AState);
          //     // do_widget_redraw(this);
          //   }
          // }
          setValue(value,index);
          do_widget_update(this,index,AState);
          do_widget_redraw(this,index,0);
        }
      
      } // num values == 2
      else {
        setValue(value,index);
        do_widget_update(this,index,AState);
        do_widget_redraw(this,index,0);
      }
    } // dragging
    
    else {
      int32_t index = findClosestValue(AXpos,AYpos);
      if (index >= 0) do_widget_setCursor(this,getCursor());
      else do_widget_setCursor(this,SAT_CURSOR_DEFAULT);
    }
    MPreviousXpos = AXpos;
    MPreviousYpos = AYpos;
  }
  
  //----------
  
  void on_widget_enter(SAT_Widget* AFrom, double AXpos, double AYpos) override {
    if (getNumValues() > 1) {
      int32_t index = findClosestValue(AXpos,AYpos);
      if (index >= 0) do_widget_setCursor(this,getCursor());
      else do_widget_setCursor(this,SAT_CURSOR_DEFAULT);
    }
    else {
      SAT_PanelWidget::on_widget_enter(AFrom,AXpos,AYpos);
    }
  }

  //----------
  
  void on_widget_leave(SAT_Widget* ATo, double AXpos, double AYpos) override {
    if (getNumValues() > 1) {
      do_widget_setCursor(this,SAT_CURSOR_DEFAULT);
    }
    else {
      SAT_PanelWidget::on_widget_leave(ATo,AXpos,AYpos);
    }
  }
  

};

//----------------------------------------------------------------------
#endif


