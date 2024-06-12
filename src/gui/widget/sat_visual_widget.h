#ifndef sat_visual_widget_included
#define sat_visual_widget_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/widget/sat_widget_listener.h"
#include "gui/sat_widget.h"

//----------

class SAT_VisualWidget
: public SAT_Widget {

//------------------------------
private:
//------------------------------

//uint32_t    MCursor           = SAT_CURSOR_DEFAULT;
//const char* MHint             = "";

  bool        MFillBackground   = true;
  bool        MDrawBorder       = true;

  SAT_Color   MBackgroundColor  = SAT_Grey;
  SAT_Color   MBorderColor      = SAT_Black;
  double      MBorderWidth      = 1.0;

  bool        MDrawIndicators   = true;

//------------------------------
public:
//------------------------------

  SAT_VisualWidget(SAT_Rect ARect)
  : SAT_Widget(ARect) {
    setName("SAT_VisualWidget");
  }

  //----------

  virtual ~SAT_VisualWidget() {
  }

//------------------------------
public:
//------------------------------

  void setFillBackground(bool AFill=true)   { MFillBackground = AFill; }
  void setBackgroundColor(SAT_Color AColor) { MBackgroundColor = AColor; }

  void setDrawBorder(bool ADraw=true)       { MDrawBorder = ADraw; }
  void setBorderColor(SAT_Color AColor)     { MBorderColor = AColor; }
  void setBorderWidth(double AWidth)        { MBorderWidth = AWidth; }

  void setDrawIndicators(bool ADraw)        { MDrawIndicators = ADraw; }

//------------------------------
public:
//------------------------------

//------------------------------
public:
//------------------------------

  virtual void fillBackground(SAT_PaintContext* AContext) {
    if (MFillBackground) {
      SAT_Painter* painter = AContext->painter;
      SAT_Rect rect = getRect();
      //SAT_PRINT("%.2f, %.2f, %.2f, %.2f\n",rect.x,rect.y,rect.w,rect.h);
      painter->setFillColor(MBackgroundColor);
      painter->fillRect(rect.x,rect.y,rect.w,rect.h);
    }
  }

  //----------

  virtual void drawBorder(SAT_PaintContext* AContext) {
    if (MDrawBorder) {
      //SAT_TRACE;
      SAT_Painter* painter = AContext->painter;
      SAT_Rect rect = getRect();
      double scale = getWindowScale();
      painter->setDrawColor(MBorderColor);
      painter->setLineWidth(MBorderWidth*scale);
      painter->drawRect(rect.x,rect.y,rect.w,rect.h);
    }
  }

  //----------

  virtual void drawIndicators(SAT_PaintContext* AContext) {
    if (MDrawIndicators) {
      //SAT_TRACE;
      SAT_Parameter* param = (SAT_Parameter*)getParameter();
      if (param) {
        SAT_Painter* painter = AContext->painter;
        SAT_Rect rect = getRect();
        double scale = getWindowScale();

        if (param->getIndicateMapped()) {
            SAT_Color color = param->getIndicateMappedColor();
            painter->setFillColor(color);
            //painter->drawRect(rect.x,rect.y,rect.w,rect.h);
        }
        // switch (param->getIndicateAutomationState() ) {
        //   case CLAP_PARAM_INDICATION_AUTOMATION_NONE: // The host doesn't have an automation for this parameter
        //   case CLAP_PARAM_INDICATION_AUTOMATION_PRESENT: // The host has an automation for this parameter, but it isn't playing it
        //   case CLAP_PARAM_INDICATION_AUTOMATION_PLAYING: // The host is playing an automation for this parameter
        //   case CLAP_PARAM_INDICATION_AUTOMATION_RECORDING: // The host is recording an automation on this parameter
        //   case CLAP_PARAM_INDICATION_AUTOMATION_OVERRIDING: // The host should play an automation for this parameter, but the user has started to adjust this parameter and is overriding the automation playback
        // }
        if (param->getIndicateAutomationState() != CLAP_PARAM_INDICATION_AUTOMATION_NONE) {
          SAT_Color color = param->getIndicateAutomationColor();
          painter->setDrawColor(color);
          //painter->drawRect(rect.x,rect.y,rect.w,rect.h);
        }
      }
    }
  }

//------------------------------
public: // on_widget
//------------------------------

  // void on_widget_open(SAT_WidgetOwner* AOwner) override {
  //   MSurface = new SAT_Surface(100,100,0);
  //   SAT_PRINT("MSurface: %p\n",MSurface);
  // }

  // void on_widget_close(SAT_WidgetOwner* AOwner) override {
  //   SAT_PRINT("MSurface: %p\n",MSurface);
  //   if (MSurface) delete MSurface;
  // }

  void on_widget_paint(SAT_PaintContext* AContext) override {
    //SAT_TRACE;
    fillBackground(AContext);
    paintChildren(AContext);
    drawIndicators(AContext);
    drawBorder(AContext);
  }

};

//----------------------------------------------------------------------
#endif