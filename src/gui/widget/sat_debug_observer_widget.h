#ifndef sat_debug_observer_widget_included
#define sat_debug_observer_widget_included
//----------------------------------------------------------------------

#if 0

#include "sat.h"
#include "plugin/sat_parameter.h"
#include "gui/widgets/sat_movable_widget.h"
#include "gui/widgets/sat_panel_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_DebugObserverWidget
: public SAT_PanelWidget {
//: public SAT_MovableWidget {

//------------------------------
private:
//------------------------------

  bool      MDrawObserver     = true;
  SAT_Color MTextColor        = SAT_White;
  double    MTextSize         = 10.0;
  uint32_t  MTextAlignment    = SAT_TEXT_ALIGN_LEFT;
  SAT_Rect  MTextOffset       = {5,5,5,5};
  char      MText[256]        = {0};

//------------------------------
public:
//------------------------------

  SAT_DebugObserverWidget(SAT_Rect ARect)
  : SAT_PanelWidget(ARect) {
    setName("SAT_DebugObserverWidget");
    //strcpy(MText,AText);
    setFillBackground(true);
  }

  virtual ~SAT_DebugObserverWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setDrawObserver(bool ADraw=true)   { MDrawObserver = ADraw; }
  virtual void setTextColor(SAT_Color AColor)     { MTextColor = AColor; }
  virtual void setTextSize(double ASize)          { MTextSize = ASize; }
  virtual void setTextAlignment(uint32_t AAlign)  { MTextAlignment = AAlign; }
  virtual void setTextOffset(SAT_Rect AOffset)    { MTextOffset = AOffset; }
  virtual void setText(const char* AText)         { strcpy(MText,AText); }

  virtual const char* getText() { return MText; }

//------------------------------
public:
//------------------------------

  virtual void drawObservers(SAT_PaintContext* AContext) {
    
    SAT_Assert(AContext);
    if (MDrawObserver) {
      double S = getWindowScale();
      SAT_Painter* painter = AContext->painter;
      SAT_Assert(painter);
      SAT_Rect mrect = getRect();
      SAT_Rect textoffset = MTextOffset;
      textoffset.scale(S);
      mrect.shrink(textoffset);
      if (mrect.w <= 0.0) return;
      if (mrect.h <= 0.0) return;
      painter->setTextColor(MTextColor);
      painter->setTextSize(MTextSize * S);
      SAT_Rect rect = SAT_Rect(mrect.x,mrect.y,mrect.w,MTextSize * S);
      char buffer[100] = {0};
      uint32_t num_obs = SAT_GLOBAL.DEBUG.getNumObservers();
      for (uint32_t i=0; i<num_obs; i++) {
        SAT_Observable* obs = SAT_GLOBAL.DEBUG.getObserver(i);
        switch (obs->type) {
          case SAT_OBSERVE_STRING:  sprintf(buffer,"%s : %s",obs->desc,(char*)obs->ptr);     break;
          case SAT_OBSERVE_UINT32:  sprintf(buffer,"%s : %i",obs->desc,*(uint32_t*)obs->ptr); break;
          case SAT_OBSERVE_INT32:   sprintf(buffer,"%s : %i",obs->desc,*(int32_t*)obs->ptr);  break;
          case SAT_OBSERVE_FLOAT:   sprintf(buffer,"%s : %f",obs->desc,*(float*)obs->ptr);    break;
          case SAT_OBSERVE_DOUBLE:  sprintf(buffer,"%s : %f",obs->desc,*(double*)obs->ptr);   break;
        }
        painter->drawTextBox(rect,buffer,MTextAlignment);
        rect.y += (MTextSize + 1) * S;
      }
      
//
//      SAT_Parameter* param = (SAT_Parameter*)getConnection(0);
//      if (param) {
//        painter->drawTextBox(mrect,param->getName(),MTextAlignment);
//      }
//      else {
//        painter->drawTextBox(mrect,MText,MTextAlignment);
//      }
//      
    }
  }

//------------------------------
public:
//------------------------------

  void prepare(SAT_WidgetOwner* AOwner) override {
    SAT_PanelWidget::prepare(AOwner);
    SAT_Window* window = (SAT_Window*)AOwner;
    window->registerTimerWidget(this);      // TODO: -> SAT_WidgetOwner
  }
  
  void on_widget_timer(uint32_t AId, double ADelta) override {
    //SAT_PRINT;
    if (SAT_GLOBAL.DEBUG.getNumObservers() > 0) {
      do_widgetListener_redraw(this,0);
    }
  }
  

  void on_widget_paint(SAT_PaintContext* AContext) override {
    drawDropShadow(AContext);
    fillBackground(AContext);
    drawObservers(AContext);
    paintChildWidgets(AContext);
    drawBorder(AContext);
  }

};

#endif // 0

//----------------------------------------------------------------------
#endif

