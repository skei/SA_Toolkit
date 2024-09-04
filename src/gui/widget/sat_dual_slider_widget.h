#ifndef sat_dual_slider_widget_included
#define sat_dual_slider_widget_included
//----------------------------------------------------------------------

/*
  TODO
  - click middle, drag both values
  - collide
    - stop (default)
    - swap and continue (shift)
    - push other value (alt)
*/

#include "gui/widget/sat_slider_widget.h"

//----------------------------------------------------------------------

class SAT_DualSliderWidget
: public SAT_SliderWidget {

//------------------------------
private:
//------------------------------

  double  MHoverDistance    = 5.0;
  int32_t MClosestIndex     = -1;
  int32_t MDraggingIndex    = -1;

  double    MEdgeWidth      = 2.0;
  SAT_Color MEdgeColor      = SAT_LightGrey;
  double    MEdgeHoverWidth = 3.0;
  SAT_Color MEdgeHoverColor = SAT_White;

  char MValueBuffer0[32] = {0};
  char MValueBuffer1[32] = {0};

//------------------------------
public:
//------------------------------

  SAT_DualSliderWidget(SAT_Rect ARect, const char* AText="")
  : SAT_SliderWidget(ARect,AText/*,AValue)*/) {
    setName("SAT_DualSliderWidget");
    setHint("SAT_DualSliderWidget");
    setCursor(SAT_CURSOR_DEFAULT);
    setNumValues(2);
    setTextAlignment(SAT_TEXT_ALIGN_CENTER);
    setDragDirection(SAT_DIRECTION_RIGHT);
    Options.wantHoverEvents = true;

    setValueTextOffset(SAT_Rect(5,0,5,0));


  }

  //----------

  virtual ~SAT_DualSliderWidget() {
  }

//------------------------------
public:
//------------------------------

  double getRange(bool AHorizontal=true) {
    SAT_Rect rect = getRect();
    double range;
    if (AHorizontal) {
      range = rect.w;
      if (range <= 0.0) return -1;
    }
    else { // vertical
      range = rect.h;
      if (range <= 0.0) return -1;
    }
    return range;
  }

  //----------

  double getPos(double AXpos, double AYpos, bool AHorizontal=true) {
    double range = getRange(AHorizontal);
    SAT_Rect rect = getRect();
    double pos;
    if (AHorizontal)  pos = (AXpos - rect.x) / range;
    else              pos = (AYpos - rect.y) / range;
    return pos;
  }

  //----------

  double getVal(int32_t index) {
    SAT_Parameter* param = (SAT_Parameter*)getParameter(index);
    double val;
    if (param) val = param->getNormalizedValue();
    else val = getValue(index);
    return val;
  }

  //----------

  double getDist(double pos, int32_t index) {
    // SAT_Parameter* param = (SAT_Parameter*)getParameter(index);
    // double val;
    // if (param) val = param->getNormalizedValue();
    // else val = getValue(index);
    double val = getVal(index);
    double dist = abs(pos - val);
    return dist;
  }

  //----------



  // find closest (from mouse cursor) value
  // if two values are identical, will fid the first one..
  // todo: check if we're on the left or right side if values are equal (or similar)
  // todo: ..ClosestValueHorizontal

  // return
  // -1   : none
  // >= 0 : index

  int32_t findClosestIndex(double AXpos, double AYpos, bool AHorizontal=true) {
    //if (getNumValues() == 0) return 0;
    //else if (getNumValues() == 1) return getValue();
    if (getNumValues() < 2) {
      return 0;
    }
    else {
      double S = getWindowScale();
      SAT_Rect mrect = getRect();
      if (!mrect.contains(AXpos,AYpos)) return -1;

      double range = getRange(AHorizontal);;

      // double pos;
      // if (AHorizontal) {
      //   range = mrect.w;
      //   if (range <= 0.0) return -1;
      //   pos = (AXpos - mrect.x) / range;
      // }
      // else { // vertical
      //   range = mrect.h;
      //   if (range <= 0.0) return -1;
      //   pos = (AYpos - mrect.y) / range;
      // }

      double pos = getPos(AXpos,AYpos,AHorizontal);

      double  closest_dist  = range;
      int32_t closest_index = -1;
      //int32_t closest = -1;
      for (uint32_t i=0; i<getNumValues(); i++) {

        // SAT_Parameter* param = (SAT_Parameter*)getParameter(i);
        // double val;
        // if (param) val = param->getNormalizedValue();
        // else val = getValue(i);
        // double dist = abs(pos - val);

        double dist = getDist(pos,i);

        double hoverdist = (MHoverDistance * S);
        //SAT_PRINT("pos %.3f i %i val %.3f dist %.3f hoverdist %.3f closest_dist %i closest_index %i\n",pos,i,val,dist,hoverdist,closest_dist,closest_index);
        if ((dist*range) < hoverdist) {
          if (dist < closest_dist) {
            closest_dist = dist;
            closest_index = i;
          }
        }
      }

      // check 'which' -1
      if (closest_index == -1) {
        if (pos < getVal(0)) {
          SAT_PRINT("left\n");
        }
        if ((pos > getVal(0)) && (pos < getVal(1))) {
          SAT_PRINT("center\n");
        }
        if (pos > getVal(1)) {
          SAT_PRINT("right\n");
        }
      }

      return closest_index;
    }
  }

  //----------

  void checkClosest(int32_t AXpos, int32_t AYpos) {
    int32_t closest = findClosestIndex(AXpos,AYpos,true);
    if (closest != MClosestIndex) {
      MClosestIndex = closest;
      do_Widget_redraw(this);
    }
    if (MClosestIndex != -1) do_Widget_set_cursor(this,SAT_CURSOR_ARROW_LEFT_RIGHT);
    else do_Widget_set_cursor(this,SAT_CURSOR_DEFAULT);
  }
  
  //----------

  void drawDualSliderBar(SAT_PaintContext* AContext) {
      SAT_Painter* painter = AContext->painter;
      SAT_Rect rect = getRect();
      double scale = getWindowScale();
      double x = rect.x;
      double y = rect.y;
      double w = rect.w;
      double h = rect.h;
      double v0 = calcValue(0);
      double v1 = calcValue(1);
      //double m = calcModulation();

      double v0w = (v0 * w);
      double v1w = (v1 * w);
      double vw = (v1w - v0w);
      v0 *= w;
      v1 *= w;

      painter->setFillColor(MSliderBarColor);

      //if (v0 > 0) {
        painter->fillRect(x+v0,y,vw,h);

        //switch (MDrawDirection) {
        //  case SAT_DIRECTION_LEFT:
        //    painter->fillRect((x+w-vw),y,vw,h);
        //    break;
        //  case SAT_DIRECTION_RIGHT:
        //    painter->fillRect(x,y,vw,h);
        //    break;
        //  case SAT_DIRECTION_UP:
        //    painter->fillRect(x,(y+h-vh),w,vh);
        //    break;
        //  case SAT_DIRECTION_DOWN:
        //    painter->fillRect(x,y,w,vh);
        //    break;
        //}
      //}

      //if (isDragging()) {
        //if (MDraggingIndex == 0) {
        
        if (MClosestIndex == 0) {
          painter->setDrawColor(MEdgeHoverColor);
          painter->setLineWidth(MEdgeHoverWidth*scale);
        }
        else {
          painter->setDrawColor(MEdgeColor);
          painter->setLineWidth(MEdgeWidth*scale);
        }

        painter->drawLine(x+v0,y,x+v0,y+h);

        //if (MDraggingIndex == 1) {
        if (MClosestIndex == 1) {
          painter->setDrawColor(MEdgeHoverColor);
          painter->setLineWidth(MEdgeHoverWidth*scale);
        }
        else {
          painter->setDrawColor(MEdgeColor);
          painter->setLineWidth(MEdgeWidth*scale);
        }
      //}

      painter->drawLine(x+v1,y,x+v1,y+h);

      // if (MDrawModulation && getParameter()) {
      // }


  }

//------------------------------
public:
//------------------------------

  void drawValueText(SAT_PaintContext* AContext) override {
    double val0;
    double val1;
    const char* txt0;
    const char* txt1;

    SAT_Parameter* param0 = (SAT_Parameter*)getParameter(0);
    if (param0) {
      val0 = param0->getValue();
      txt0 = param0->valueToText(val0);
    }
    else {
      val0 = getValue(0);
      sprintf(MValueBuffer0,"%.2f",val0);
      txt0 = MValueBuffer0;
    }

    SAT_Parameter* param1 = (SAT_Parameter*)getParameter(1);
    if (param1) {
      val1 = param1->getValue();
      txt1 = param1->valueToText(val1);
    }
    else {
      val1 = getValue(1);
      sprintf(MValueBuffer1,"%.2f",val1);
      txt1 = MValueBuffer1;
    }

    //SAT_SliderWidget::drawValueText(AContext);
    SAT_Painter* painter = AContext->painter;
    SAT_Rect rect = getRect();
    double scale = getWindowScale();

    SAT_Rect offset = getValueTextOffset();
    offset.scale(scale);
    rect.shrink(offset);

    painter->setTextColor(SAT_Black);
    painter->drawTextBox(rect,txt0,SAT_TEXT_ALIGN_LEFT);
    painter->drawTextBox(rect,txt1,SAT_TEXT_ALIGN_RIGHT);

  }

  //----------

  void on_Widget_paint(SAT_PaintContext* AContext) override {
    //SAT_PRINT("MClosestValue: %i\n",MClosestValue);
    fillBackground(AContext);
    drawDualSliderBar(AContext);
    drawQuantized(AContext);
    //drawValueBar(AContext,getDragValue());
    drawText(AContext);
    drawValueText(AContext);
    paintChildren(AContext);
    drawIndicators(AContext);
    drawBorder(AContext);
  }
  
  //----------

  void on_Widget_mouse_click(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    if (AButton == SAT_BUTTON_LEFT) {
      if (MClosestIndex >= 0) {
        MDraggingIndex = MClosestIndex;
        startDrag(AXpos,AYpos,MDraggingIndex);
      }
      // else {
      //   SAT_PRINT("MClosestIndex %i\n",MClosestIndex);
      // }
    }
  }
  
  //----------
  
  void on_Widget_mouse_release(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    if (AButton == SAT_BUTTON_LEFT) {
      endDrag();
      MDraggingIndex = -1;
      checkClosest(AXpos,AYpos);
    }
  }
  
  //----------
  
  void on_Widget_mouse_move(int32_t AXpos, int32_t AYpos, uint32_t AState, uint32_t ATime) override {
    if (isDragging()) {
      double value = calcDragValue(AXpos,AYpos,AState);

      //SAT_PRINT("MDraggingIndex %i\n",MDraggingIndex);

      // left

      if ((MDraggingIndex==0) && ( value > getValue(1))) {
        if (AState & SAT_STATE_SHIFT) {
          // swap
          value = getValue(1);
          MDraggingIndex = 1;
          MClosestIndex = 1;
        }
        else if (AState & SAT_STATE_ALT) {
          // push
          setValue(value,1);
          do_Widget_update(this,1);
        }
        else {
          // stop
          value = getValue(1);
        }
      }

      // right

      else if ((MDraggingIndex==1) && ( value < getValue(0))) {
        if (AState & SAT_STATE_SHIFT) {
          // swap
          value = getValue(0);
          MDraggingIndex = 0;
          MClosestIndex = 0;
        }
        else if (AState & SAT_STATE_ALT) {
          // push
          setValue(value,0);
          do_Widget_update(this,0);
        }
        else {
          // stop
          value = getValue(0);
        }

      }

      setValue(value,MDraggingIndex);
      do_Widget_update(this,MDraggingIndex);
      do_Widget_redraw(this);

    }
    else {
      checkClosest(AXpos,AYpos);
      //int32_t closest = findClosestIndex(AXpos,AYpos,true);
      //if (closest != MClosestIndex) {
      //  MClosestIndex = closest;
      //  do_Widget_redraw(this);
      //}
    }
    //SAT_SliderWidget::on_Widget_mouse_move(AXpos,AYpos,AState,ATime);
  }

  //----------

  void on_Widget_enter(SAT_Widget* AFrom, int32_t AXpos, int32_t AYpos, uint32_t ATime) override {
    SAT_SliderWidget::on_Widget_enter(AFrom,AXpos,AYpos,ATime);
    checkClosest(AXpos,AYpos);
    //int32_t closest = findClosestIndex(AXpos,AYpos,true);
    ////SAT_PRINT("closest: %i\n",closest);
    //if (closest != MClosestIndex) {
    //  MClosestIndex = closest;
    //  do_Widget_redraw(this);
    //}
  }
  
  //----------
  
  void on_Widget_leave(SAT_Widget* ATo, int32_t AXpos, int32_t AYpos, uint32_t ATime) override {
    SAT_SliderWidget::on_Widget_leave(ATo,AXpos,AYpos,ATime);
    //SAT_PRINT("closest: -1\n");
    if (MClosestIndex != -1) {
      MClosestIndex = -1;
      do_Widget_redraw(this);
    }
  }
  
  //----------

};

  // virtual void setValue(double AValue)                          { MValues[MValueIndex] = AValue; }
  // virtual void setValue(double AValue, uint32_t AIndex)         { MValues[AIndex] = AValue; }
  // virtual void setValueIndex(uint32_t AIndex)                   { MValueIndex = AIndex; }


//----------------------------------------------------------------------
#endif