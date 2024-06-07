#ifndef sat_drag_value_widget_included
#define sat_drag_value_widget_included
//---------------------------------------------------------------------

#include "sat.h"
#include "gui/widget/sat_value_widget.h"
#include "plugin/sat_parameter.h"

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

  int32_t   MPrevX              = 0;
  int32_t   MPrevY              = 0;
  bool      MIsDragging         = false;
  double    MDragValue          = 0.0;
  uint32_t  MPrevPreciseKey     = SAT_STATE_NONE;
  uint32_t  MPrevUnsnapKey      = SAT_STATE_NONE;

  //

  uint32_t  MDragDirection      = SAT_DIRECTION_UP;

  double    MSensitivity        = (1.0 / 200.0);
  double    MSensitivity2       = (1.0 / 20.0);

  uint32_t  MDragButton         = SAT_BUTTON_LEFT;
  uint32_t  MPreciseKey         = SAT_STATE_CTRL;
  uint32_t  MUnsnapKey          = SAT_STATE_SHIFT;

  bool      MQuantize           = false;
  uint32_t  MQuantizeSteps      = 5;

  bool      MSnap               = false;
  double    MSnapPos            = 0.5;
  double    MSnapDist           = 0.2;
  double    MSnapSpeed          = 1.5;

  bool      MDrawValueBar       = false;
  SAT_Color MValueBarColor      = SAT_Black;
  double    MValueBarThickness  = 3.0;
  uint32_t  MValueBarDirection  = SAT_DIRECTION_RIGHT;
  SAT_Rect  MValueBarEdge       = SAT_EDGE_BOTTOM;

//------------------------------
public:
//------------------------------

  SAT_DragValueWidget(SAT_Rect ARect)
  : SAT_ValueWidget(ARect) {
    setName("SAT_DragValueWidget");
    setCursor(SAT_CURSOR_ARROW_UP_DOWN);
  }

  //----------

  virtual ~SAT_DragValueWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setSensitivity(double AValue)              { MSensitivity = AValue; }
  virtual void setSensitivity2(double AValue)             { MSensitivity2 = AValue; }
  virtual void setDragButton(uint32_t AButton)            { MDragButton  = AButton; }
  virtual void setPreciseKey(uint32_t AKey)               { MPreciseKey = AKey; }
  virtual void setUnsnapKey(uint32_t AKey)                { MUnsnapKey = AKey; }

  virtual void setDragQuantize(bool AState=true)          { MQuantize = AState; }
  virtual void setDragQuantizeSteps(uint32_t ASteps)      { MQuantizeSteps = ASteps; }

  virtual void setDragSnap(bool AState=true)              { MSnap = AState; }
  virtual void setDragSnapPos(double AValue)              { MSnapPos = AValue; }
  virtual void setDragSnapDist(double AValue)             { MSnapDist = AValue; }
  virtual void setDragSnapSpeed(double AValue)            { MSnapSpeed = AValue; }


  virtual void setDrawValueBar(bool ADraw=true)           { MDrawValueBar = ADraw; }
  virtual void setValueBarColor(SAT_Color AColor)         { MValueBarColor = AColor; }
  virtual void setValueBarThickness(double ASize)         { MValueBarThickness = ASize; }
  virtual void setValueBarDirection(uint32_t ADirection)  { MValueBarDirection = ADirection; }
  virtual void setValueBarEdge(uint32_t AEdge)            { MValueBarEdge = AEdge; }

  //

  virtual double    getDragValue()      { return MDragValue; }
  
  virtual bool      getQuantize()        { return MQuantize; }
  virtual bool      getSnap()            { return MSnap; }

  virtual uint32_t  getQuantizeSteps()  { return MQuantizeSteps; }
  virtual double    getSnapPos()        { return MSnapPos; }
  virtual double    getSnapDist()       { return MSnapDist; }
  virtual double    getSnapSpeed()      { return MSnapSpeed; }

//------------------------------
protected:
//------------------------------

  // returns 0..1

  double calcValue() {
    double value = getValue();
    //SAT_PRINT("value %.3f\n",value);
    SAT_Parameter* param = (SAT_Parameter*)getParameter();
    //SAT_PRINT("param %p\n",param);
    if (param) {
      value = param->getNormalizedValue();
      //value = param->normalize(value);
      //SAT_PRINT("normalized value %.3f\n",value);
    }
    return value;
  }

  //----------

  // returns 0..1 (final modulated and clamped value)

  //double calcModulation(double value) {
  double calcModulation() {
    double modulation = 0.0;//getValue();
    SAT_Parameter* param = (SAT_Parameter*)getParameter();
    if (param) {
      modulation = param->getValue() + param->getModulation();
      modulation = param->normalize(modulation);
      modulation = SAT_Clamp(modulation,0,1);
    }
    return modulation;
  }

  //----------

  // float SAT_Interpolate_Linear(float t, float y1,float y2) {
  //   return ( y1*(1-t) + y2*t );
  // }  

  double snapValue(double AValue) {
    double value = AValue;
    if (MSnapDist > 0) {
      double diff = abs(MSnapPos - value);
      if (diff < MSnapDist) {
        // 1 at snappos, 0 at snapdist
        double s = 1.0 - (diff / MSnapDist);
        s *= MSnapSpeed;
        s = SAT_Clamp(s,0,1);
        //value = SAT_Interpolate_Linear(s,value,MSnapPos);
        value = value * (1 - s) + MSnapPos * s;
      }
    }
    return value;
  }
  
  //----------

  double quantizeValue(double AValue) {
    if (MQuantizeSteps > 1) {
      double qsteps = (double)(MQuantizeSteps - 1);
      double qval = AValue * (double)MQuantizeSteps;
      double q = SAT_Trunc(qval) / qsteps;
      return SAT_Clamp(q,0,1);
    }
    return AValue;
  }
  
//------------------------------
public:
//------------------------------

  virtual void drawValueBar(SAT_PaintContext* AContext, double AValue) {
    if (MDrawValueBar) {
      //SAT_TRACE;
      SAT_Painter* painter = AContext->painter;
      SAT_Rect rect = getRect();
      double scale = getWindowScale();
      double v = AValue * rect.w;
      double x1 = rect.x + 1.0;
      double y1 = rect.y + rect.h - (MValueBarThickness * 0.5);
      double x2 = rect.x + v;
      double y2 = y1;
      if (v > 0) {
        painter->setDrawColor(MValueBarColor);
        painter->setLineWidth(MValueBarThickness*scale);
        painter->drawLine(x1,y1,x2,y2);
      }
    }

  }

//------------------------------
public: // on_widget
//------------------------------

  void on_widget_open(SAT_WidgetOwner* AOwner) override {
    MDragValue = calcValue();
    //SAT_PRINT("MDragValue %.3f\n",MDragValue);
    SAT_ValueWidget::on_widget_open(AOwner);
  }

  //----------

  void on_widget_mouse_click(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    //SAT_PRINT("AXpos %i AYpos %i\n",AXpos,AYpos);
    if (AButton == MDragButton) {
      MIsDragging = true;
      MPrevX = AXpos;
      MPrevY = AYpos;
      do_widget_set_cursor(this,SAT_CURSOR_LOCK);
      MDragValue = calcValue(); // starting value
    }
  }

  //----------

  void on_widget_mouse_release(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    if (AButton == MDragButton) {
      MIsDragging = false;
      do_widget_set_cursor(this,SAT_CURSOR_UNLOCK);
    }
  }

  //----------

  void on_widget_mouse_move(int32_t AXpos, int32_t AYpos, uint32_t AState, uint32_t ATime) override {
    //SAT_PRINT("AXpos %i AYpos %i MPrevX %i MPrevY %i\n",AXpos,AYpos,MPrevX,MPrevY);

    if (MIsDragging) {

      int32_t diff = 0;
      switch (MDragDirection) {
        case SAT_DIRECTION_UP:    diff = MPrevY - AYpos;  break;
        case SAT_DIRECTION_DOWN:  diff = AYpos - MPrevY;  break;
        case SAT_DIRECTION_LEFT:  diff = AXpos - MPrevX;  break;
        case SAT_DIRECTION_RIGHT: diff = MPrevX - AXpos;  break;
      }

      bool unsnapkey = (AState & MUnsnapKey);
      bool precisekey = (AState & MPreciseKey);

      if (diff != 0) {

        double sensitivity = MSensitivity;
        if (precisekey) sensitivity *= MSensitivity2;
        double delta = (double)diff * sensitivity;;

        MDragValue += delta;
        MDragValue = SAT_Clamp(MDragValue,0,1); 
        //double value = 0.0;
        double value = MDragValue;

        //
        
        if (MSnap) {
          if (unsnapkey) { // not snap
            if (!MPrevUnsnapKey) {
              //SAT_PRINT("shift released\n");              
              //MDragValue = calcValue();
            }
          }
          else { // snap
            //if (MPrevUnsnapKey) {
            //  SAT_PRINT("shift pressed\n");
            //}
            value = snapValue(MDragValue);
          }
          MPrevUnsnapKey = unsnapkey;
        }

        //

        if (MQuantize && !unsnapkey) {
          value = quantizeValue(MDragValue);
        }

        double prev_value = getValue();
//        if (value != prev_value) {
          setValue(value);
          do_widget_update(this,SAT_WIDGET_UPDATE_VALUE);
          do_widget_redraw(this,SAT_WIDGET_REDRAW_PARAM);
//        }

      } // diff
    } // dragging

    MPrevX = AXpos;
    MPrevY = AYpos;
  }

  //----------

  void on_widget_paint(SAT_PaintContext* AContext) override {
    //SAT_TRACE;
    fillBackground(AContext);
    drawText(AContext);
    drawValueText(AContext);
    drawValueBar(AContext,MDragValue);
    paintChildren(AContext);
    drawBorder(AContext);
  }  

};

//---------------------------------------------------------------------
#endif

