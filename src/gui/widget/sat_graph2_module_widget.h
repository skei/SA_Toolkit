#ifndef sat_graph2_module_widget_included
#define sat_graph2_module_widget_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/widget/sat_text_widget.h"
#include "gui/widget/sat_visual_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Graph2ModuleWidget
: public SAT_VisualWidget {

//------------------------------
private:
//------------------------------

  double            MTitleHeight      = 15;
  double            MPinWidth         = 7;
  double            MPinHeight        = 7;
  double            MPinHDist         = 3;
  double            MPinVDist         = 3;
  SAT_VisualWidget* MContentWidget    = nullptr;
  SAT_TextWidget*   MTitleWidget      = nullptr;
  bool              MVertical         = false;

//------------------------------
public:
//------------------------------

  SAT_Graph2ModuleWidget(SAT_Rect ARect, bool AVertical=false)
  : SAT_VisualWidget(ARect) {

    MVertical = AVertical;

    setName("SAT_Graph2ModuleWidget");
    setHint("SAT_Graph2ModuleWidget");

    setDrawBorder(false);
    setFillBackground(true);
    setBackgroundColor(0.47);

    setMovable(true);
    Options.wantHoverEvents = true;

    double pin_w = (MPinWidth + MPinHDist);
    double pin_h = (MPinHeight + MPinVDist);

    SAT_Rect title_rect;
    SAT_Rect content_rect;

    if (MVertical) {
      title_rect.x = pin_w;
      title_rect.y = 0.0;
      title_rect.w = ARect.w - (2.0 * pin_w);
      title_rect.h = MTitleHeight;
      content_rect.x = title_rect.x;
      content_rect.y = title_rect.y + MTitleHeight;
      content_rect.w = title_rect.w;
      content_rect.h = ARect.h - title_rect.h;
      setMovableOffset(SAT_Rect( pin_w, 0, pin_w, 0 ));
    }
    else {
      title_rect.x = 0.0;
      title_rect.y = pin_h;
      title_rect.w = ARect.w;
      title_rect.h = MTitleHeight;
      content_rect.x = title_rect.x;
      content_rect.y = title_rect.y + MTitleHeight;
      content_rect.w = title_rect.w;
      content_rect.h = ARect.h - title_rect.h - (2.0 * (MPinHeight + MPinVDist));
      setMovableOffset(SAT_Rect( 0, pin_h, 0, pin_h ));
    }

    MTitleWidget = new SAT_TextWidget(title_rect,"Module");
    SAT_VisualWidget::appendChild(MTitleWidget);

    MTitleWidget->setFillBackground(true);
    MTitleWidget->setBackgroundColor(SAT_DarkerGrey);
    MTitleWidget->setDrawText(true);
    MTitleWidget->setDrawParamText(false);
    MTitleWidget->setText("Module");
    MTitleWidget->setTextAlignment(SAT_TEXT_ALIGN_LEFT);
    MTitleWidget->setTextColor(SAT_White);
    MTitleWidget->setTextOffset(SAT_Rect(3,0,0,0));
    MTitleWidget->setTextSize(8);
    MTitleWidget->setActive(false,false);

    MContentWidget = new SAT_VisualWidget(content_rect);
    SAT_VisualWidget::appendChild(MContentWidget);
    MContentWidget->setDrawBorder(true);
    MContentWidget->setFillBackground(true);
    MContentWidget->setBackgroundColor(0.53);

  }

  //----------

  virtual ~SAT_Graph2ModuleWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual double            getTitleHeight()    { return MTitleHeight; }
  virtual double            getPinWidth()       { return MPinWidth; }
  virtual double            getPinHeight()      { return MPinHeight; }
  virtual double            getPinHDist()       { return MPinHDist; }
  virtual double            getPinVDist()       { return MPinVDist; }
  virtual SAT_VisualWidget* getContentWidget()  { return MContentWidget; }
  virtual SAT_TextWidget*   getTitleWidget()    { return MTitleWidget; }
  virtual bool              isVertical()        { return MVertical; }

//------------------------------
public:
//------------------------------

  SAT_Rect getPinRect(uint32_t AIndex, bool AInput=true) {
    SAT_Rect rect = getRect();
    SAT_Rect pin_rect;
    double scale = getWindowScale();
    double pw = (MPinWidth  + MPinHDist) * scale;
    double ph = (MPinHeight + MPinVDist) * scale;
    pin_rect.w = (MPinWidth * scale);
    pin_rect.h = (MPinHeight * scale);
    if (MVertical) {
      if (AInput) {
        pin_rect.x = rect.x;
        pin_rect.y = rect.y + (AIndex * ph);
      }
      else {
        pin_rect.x = rect.x2() - (MPinWidth * scale);
        pin_rect.y = rect.y + (AIndex * ph);
      }      
    }
    else {
      if (AInput) {
        pin_rect.x = rect.x + (AIndex * pw);
        pin_rect.y = rect.y;
      }
      else {
        pin_rect.x = rect.x + (AIndex * pw);
        pin_rect.y = rect.y2() - (MPinHeight * scale);

      }
    }
    return pin_rect;
  }


//------------------------------
public:
//------------------------------

  SAT_Widget* appendChild(SAT_Widget* AWidget) override {
    return MContentWidget->appendChild(AWidget);
  }

//------------------------------
public:
//------------------------------

  //virtual void drawHorizontalPins(SAT_PaintContext* AContext) {
  virtual void drawPins(SAT_PaintContext* AContext) {
    SAT_Painter* painter = AContext->painter;

    painter->setFillColor(SAT_DarkerGrey);
    for (uint32_t i=0; i<1; i++) {
      SAT_Rect r = getPinRect(i,true);
      painter->fillRect(r.x,r.y,r.w,r.h);
    }

    painter->setFillColor(SAT_LighterGrey);
    for (uint32_t i=1; i<4; i++) {
      SAT_Rect r = getPinRect(i,true);
      painter->fillRect(r.x,r.y,r.w,r.h);
    }

    painter->setFillColor(SAT_DarkerGrey);
    for (uint32_t i=0; i<2; i++) {
      SAT_Rect r = getPinRect(i,false);
      painter->fillRect(r.x,r.y,r.w,r.h);
    }

    painter->setFillColor(SAT_LighterGrey);
    for (uint32_t i=2; i<5; i++) {
      SAT_Rect r = getPinRect(i,false);
      painter->fillRect(r.x,r.y,r.w,r.h);
    }
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(SAT_PaintContext* AContext) override {

    drawDropShadow(AContext);
    fillBackground(AContext);
    paintChildren(AContext);
    drawPins(AContext);
    drawIndicators(AContext);
    drawBorder(AContext);

  };

};

//----------------------------------------------------------------------
#endif
