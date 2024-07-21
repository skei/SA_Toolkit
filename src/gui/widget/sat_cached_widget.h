#ifndef sat_cached_widget_included
#define sat_cached_widget_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/widget/sat_visual_widget.h"

class SAT_Widget;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_CachedWidget
: public SAT_VisualWidget {

//------------------------------
private:
//------------------------------

  void* MRenderBuffer = nullptr;

//------------------------------
public:
//------------------------------

  SAT_CachedWidget(SAT_Rect ARect)
  : SAT_VisualWidget(ARect) {
    setName("SAT_CachedWidget");
    setHint("SAT_CachedWidget");
  }

  //----------

  virtual ~SAT_CachedWidget() {
  }

//------------------------------
public:
//------------------------------

  void on_widget_open(SAT_WidgetOwner* AOwner) override {
    SAT_Painter* painter = AOwner->on_WidgetOwner_getPainter();
    MRenderBuffer = painter->createRenderBuffer(256,256);
    SAT_VisualWidget::on_widget_open(AOwner);
  }

  //----------

  void on_widget_close(SAT_WidgetOwner* AOwner) override {
    SAT_Painter* painter = AOwner->on_WidgetOwner_getPainter();
    painter->deleteRenderBuffer(MRenderBuffer);
    SAT_VisualWidget::on_widget_close(AOwner);
  }

  //----------

  void on_widget_resize(uint32_t AWidth, uint32_t AHeight) override {
    // resize/recreate render buffer
  }

  //----------

  void on_widget_paint(SAT_PaintContext* AContext) override {
    // a. update cached buffer
    // b. copy buffer to window/screen
  }

};

//----------------------------------------------------------------------
#endif
