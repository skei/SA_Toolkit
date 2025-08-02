#ifndef sat_buffered_widget_included
#define sat_buffered_widget_included
//----------------------------------------------------------------------

// ugh.. need to rethink all of this...

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
      SAT_PRINT("deleting render buffer\n");
      painter->deleteRenderBuffer(MRenderBuffer);
    }
    SAT_PRINT("(re-) creating render buffer (%i,%i)\n",AWidth,AHeight);
    // TODO: nextPowerOfTwo?
    MRenderBuffer = painter->createRenderBuffer(AWidth,AHeight);
    SAT_Assert(MRenderBuffer);
    MBufferWidth = AWidth;
    MBufferHeight = AHeight;
  }

  //----------

  void paintRenderBuffer(SAT_PaintContext* AContext) {
    //SAT_PRINT("repainting buffer\n");
    SAT_Painter* painter = AContext->painter;
    SAT_Assert(painter);
    painter->selectRenderBuffer(MRenderBuffer);
    SAT_VisualWidget::on_Widget_paint(AContext);
    painter->selectRenderBuffer(nullptr);
  }

  //----------

  void paintBuffered(SAT_PaintContext* AContext) {
    //SAT_PRINT("painting with buffer\n");
    SAT_Painter* painter = AContext->painter;
    SAT_Assert(painter);
    SAT_Rect rect = getRect();
    //SAT_PRINT("rect.w %.2f rect.h %.2f\n",rect.w,rect.h);
    int32_t image = painter->getImageFromRenderBuffer(MRenderBuffer);
    SAT_Assert(image);
    painter->drawImage(rect,image,SAT_Rect(0,0,rect.w,rect.h));
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
      paintRenderBuffer(AContext);
      MNeedRepaint = false;
    }
    paintBuffered(AContext);
    //paintChildren(AContext);
    SAT_VisualWidget::on_Widget_paint(AContext);

  }

  //----------

  void on_Widget_resize(uint32_t AWidth, uint32_t AHeight) override {
    SAT_TRACE;
    SAT_VisualWidget::on_Widget_resize(AWidth,AHeight);
  }

//------------------------------
public: // do_widget
//------------------------------

  void do_Widget_redraw(SAT_Widget* AWidget, uint32_t AIndex=0, uint32_t AMode=SAT_WIDGET_REDRAW_SELF) override {
    SAT_TRACE;
    MNeedRepaint = true;
    SAT_VisualWidget::do_Widget_redraw(AWidget,AIndex,AMode);
  }

  //----------

};

//----------------------------------------------------------------------
#endif
