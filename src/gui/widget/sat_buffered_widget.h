#ifndef sat_buffered_widget_included
#define sat_buffered_widget_included
//----------------------------------------------------------------------

// doesn't work properly.. :-/

#include "sat.h"
#include "gui/widget/sat_visual_widget.h"

class SAT_Widget;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_BufferedWidget
: public SAT_VisualWidget {

//------------------------------
private:
//------------------------------

  void*     MRenderBuffer = nullptr;
  uint32_t  MBufferWidth  = 0;
  uint32_t  MBufferHeight = 0;
  bool      MNeedRepaint  = true;

//------------------------------
public:
//------------------------------

  SAT_BufferedWidget(SAT_Rect ARect)
  : SAT_VisualWidget(ARect) {
    setName("SAT_BufferedWidget");
    setHint("SAT_BufferedWidget");
    Layout.flags = SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
    Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_ALL;
    setDrawBorder(false);
    setFillBackground(false);
  }

  //----------

  virtual ~SAT_BufferedWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void forceRepaint(bool ARepaint=true) {
    MNeedRepaint = ARepaint;
  }

//------------------------------
private:
//------------------------------

  void resizeRenderBuffer(SAT_PaintContext* AContext, uint32_t AWidth, uint32_t AHeight) {
    SAT_Painter* painter = AContext->painter;
    SAT_Assert(painter);
    if (MRenderBuffer) {
      SAT_PRINT("deleting buffer\n");
      painter->deleteRenderBuffer(MRenderBuffer);
    }
    SAT_PRINT("creating buffer (%i,%i)\n",AWidth,AHeight);
    // TODO: nextPowerOfTwo?
    MRenderBuffer = painter->createRenderBuffer(AWidth,AHeight);
    SAT_Assert(MRenderBuffer);
    MBufferWidth = AWidth;
    MBufferHeight = AHeight;
  }

  //----------

  void updateRenderBuffer(SAT_PaintContext* AContext) {
    SAT_PRINT("updating buffer\n");
    SAT_Painter* painter = AContext->painter;
    SAT_Assert(painter);

    SAT_WidgetOwner* owner = getOwner();
    SAT_Assert(owner);
    uint32_t w = owner->on_WidgetOwner_getWidth();
    uint32_t h = owner->on_WidgetOwner_getHeight();

    painter->selectRenderBuffer(MRenderBuffer);
    //painter->beginFrame(MBufferWidth,MBufferHeight);

    SAT_VisualWidget::on_Widget_paint(AContext);

    painter->selectRenderBuffer(nullptr);
    //painter->beginFrame(w,h);

  }

  //----------

  void paintBuffer(SAT_PaintContext* AContext) {
    SAT_Rect rect = getRect();
    SAT_PRINT("painting with buffer (%.2f,%.2f)\n",rect.w,rect.h);
    SAT_Painter* painter = AContext->painter;
    SAT_Assert(painter);
    int32_t image = painter->getImageFromRenderBuffer(MRenderBuffer);
    SAT_Assert(image);
    //painter->drawImage(rect,image,SAT_Rect(0,0,rect.w,rect.h));
    painter->setFillImage(image,0,0,1,1);
    painter->fillRect(rect.x,rect.y,rect.w,rect.h);
  }

//------------------------------
public: // on_widget
//------------------------------

  // first time this is called, it will create a render buffer,
  // because width/height = 0,0 = != draw-rect

  void on_Widget_paint(SAT_PaintContext* AContext) override {

    SAT_Rect rect = getRect();
    uint32_t width = ceil(rect.w);
    uint32_t height = ceil(rect.h);
    if ( (width != MBufferWidth) || (height != MBufferHeight) ) {
      resizeRenderBuffer(AContext,width,height);
      MNeedRepaint = true;
    }
    if (MNeedRepaint) {
      updateRenderBuffer(AContext);
      MNeedRepaint = false;
    }

//    paintBuffer(AContext);

    //SAT_VisualWidget::on_Widget_paint(AContext);

    // drawDropShadow(AContext);
    // fillBackground(AContext);
    //paintChildren(AContext);
    // drawIndicators(AContext);
    // drawBorder(AContext);    

  }

  //----------

  void on_Widget_resize(uint32_t AWidth, uint32_t AHeight) override {
    SAT_TRACE;
    SAT_VisualWidget::on_Widget_resize(AWidth,AHeight);
  }

//------------------------------
public: // do_widget
//------------------------------

  // if any sub-widgets needs to be redrawn, we set repaint to true,
  // so that we update/redraw the buffer (next time we paint)

  void do_Widget_redraw(SAT_Widget* AWidget, uint32_t AIndex=0, uint32_t AMode=SAT_WIDGET_REDRAW_SELF) override {
    SAT_TRACE;
    MNeedRepaint = true;
    SAT_VisualWidget::do_Widget_redraw(AWidget,AIndex,AMode);
  }

  //----------

};

//----------------------------------------------------------------------
#endif
