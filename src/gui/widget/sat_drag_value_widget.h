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

  SAT_DragValueWidget(SAT_Rect ARect, const char* AText="", double AValue=0.0)
  : SAT_ValueWidget(ARect,AText,AValue) {
    setName("SAT_DragValueWidget");
    setHint("SAT_DragValueWidget");
    setCursor(SAT_CURSOR_ARROW_UP_DOWN);
    setText(AText);
    setFillBackground(true);
    setBackgroundColor(0.45);
    setDrawBorder(false);
  }

  //----------

  virtual ~SAT_DragValueWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setDragDirection(uint32_t ADirection)      { MDragDirection = ADirection; }

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

  // move to SAT_VisualWidget (like indicators)?
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

  virtual bool isDragging() { return MIsDragging; }

//------------------------------
protected:
//------------------------------

  // returns 0..1

  double calcValue(uint32_t AIndex=0) {
    double value = getValue(AIndex);
    //SAT_PRINT("value %.3f\n",value);
    SAT_Parameter* param = (SAT_Parameter*)getParameter(AIndex);
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
  double calcModulation(uint32_t AIndex=0) {
    double modulation = 0.0;//getValue();
    SAT_Parameter* param = (SAT_Parameter*)getParameter(AIndex);
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

  // move to SAT_VisualWidget (like indicators)?
  
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

  //

  virtual double calcDragValue(int32_t AXpos, int32_t AYpos, uint32_t AState) {
    bool unsnapkey = (AState & MUnsnapKey);
    bool precisekey = (AState & MPreciseKey);
    int32_t diff = 0;
    switch (MDragDirection) {
      case SAT_DIRECTION_UP:    diff = MPrevY - AYpos;  break;
      case SAT_DIRECTION_DOWN:  diff = AYpos - MPrevY;  break;
      case SAT_DIRECTION_RIGHT: diff = AXpos - MPrevX;  break;
      case SAT_DIRECTION_LEFT:  diff = MPrevX - AXpos;  break;
    }
    MPrevX = AXpos;
    MPrevY = AYpos;
    double value = MDragValue;
    if (diff != 0) {
      double sensitivity = MSensitivity;
      if (precisekey) sensitivity *= MSensitivity2;
      double delta = (double)diff * sensitivity;;
      MDragValue += delta;
      MDragValue = SAT_Clamp(MDragValue,0,1); 
      if (MSnap) {
        if (unsnapkey) { // not snap
          if (!MPrevUnsnapKey) {
          }
        }
        else { // snap
          value = snapValue(MDragValue);
        }
        MPrevUnsnapKey = unsnapkey;
      }
      if (MQuantize && !unsnapkey) {
        value = quantizeValue(MDragValue);
      }
      // setValue(value);
      // do_Widget_update(this);
      // do_Widget_redraw(this);
    } // diff
    return value;
  }

  //----------

  virtual void startDrag(int32_t AXpos, int32_t AYpos, uint32_t AIndex) {
    MIsDragging = true;
    MPrevX = AXpos;
    MPrevY = AYpos;
    do_Widget_set_cursor(this,SAT_CURSOR_LOCK);
    MDragValue = calcValue(AIndex); // starting value
  }

  //----------

  virtual void endDrag() {
    MIsDragging = false;
    do_Widget_set_cursor(this,SAT_CURSOR_UNLOCK);
  }


//------------------------------
public: // on_widget
//------------------------------

  void on_Widget_open(SAT_WidgetOwner* AOwner) override {
    MDragValue = calcValue();
    SAT_ValueWidget::on_Widget_open(AOwner);
  }

  //----------

  void on_Widget_mouse_click(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    if (AButton == MDragButton) {
      startDrag(AXpos,AYpos,0);
    }
  }

  //----------

  void on_Widget_mouse_release(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    if (AButton == MDragButton) {
      endDrag();
    }
  }

  //----------

  void on_Widget_mouse_move(int32_t AXpos, int32_t AYpos, uint32_t AState, uint32_t ATime) override {
    double value = calcDragValue(AXpos,AYpos,AState);
    setValue(value,0);
    do_Widget_update(this);
    do_Widget_redraw(this);
  }

  //----------

  void on_Widget_paint(SAT_PaintContext* AContext) override {
    fillBackground(AContext);
    drawText(AContext);
    drawValueText(AContext);
    drawValueBar(AContext,MDragValue);
    paintChildren(AContext);
    drawIndicators(AContext);
    drawBorder(AContext);
  }  

};

//---------------------------------------------------------------------
#endif

