#ifndef sat_cached_widget_included
#define sat_cached_widget_included
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

class SAT_CachedWidget
: public SAT_VisualWidget {

//------------------------------
private:
//------------------------------

  void*     MRenderBuffer = nullptr;
  uint32_t  MCachedWidth  = 0;
  uint32_t  MCachedHeight = 0;
  bool      MNeedRepaint  = true;

//------------------------------
public:
//------------------------------

  SAT_CachedWidget(SAT_Rect ARect)
  : SAT_VisualWidget(ARect) {
    setName("SAT_CachedWidget");
    setHint("SAT_CachedWidget");
    Layout.flags = SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
    Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_ALL;
    setFillBackground(false);
  }

  //----------

  virtual ~SAT_CachedWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void forceRepaint(bool ARepaint=true) {
    MNeedRepaint = ARepaint;
  }

//------------------------------
public:
//------------------------------

  void on_Widget_paint(SAT_PaintContext* AContext) override {
    SAT_Rect rect = getRect();
    uint32_t width = ceil(rect.w);
    uint32_t height = ceil(rect.h);
    SAT_Painter* painter = AContext->painter;
    SAT_Assert(painter);
    if ( (width != MCachedWidth) || (height != MCachedHeight) ) {
      //SAT_PRINT("deleting render buffer\n");
      painter->deleteRenderBuffer(MRenderBuffer);
      //SAT_PRINT("(re-) creating render buffer (%i,%i)\n",width,height);
      MRenderBuffer = painter->createRenderBuffer(width,height);
      SAT_Assert(MRenderBuffer);
      MCachedWidth = width;
      MCachedHeight = height;
      MNeedRepaint = true;
    }
    if (MNeedRepaint) {
      //SAT_PRINT("repainting buffer\n");
      painter->selectRenderBuffer(MRenderBuffer);
      SAT_VisualWidget::on_Widget_paint(AContext);
      painter->selectRenderBuffer(nullptr);
      MNeedRepaint = false;
    }
    // copy buffer to window/screen
    int32_t image = painter->getImageFromRenderBuffer(MRenderBuffer);
    painter->drawImage(rect,image,SAT_Rect(0,0,rect.w,rect.h));
  }

//------------------------------
public: // on_widget
//------------------------------

  void do_Widget_redraw(SAT_Widget* AWidget, uint32_t AIndex=0, uint32_t AMode=SAT_WIDGET_REDRAW_SELF) override {
    MNeedRepaint = true;
    SAT_VisualWidget::do_Widget_redraw(AWidget,AIndex,AMode);
  }

};

//----------------------------------------------------------------------
#endif
