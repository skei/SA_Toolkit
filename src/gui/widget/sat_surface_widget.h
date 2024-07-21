#ifndef sat_surface_widget_included
#define sat_surface_widget_included
//----------------------------------------------------------------------

#if 0

#include "sat.h"
#include "gui/widget/sat_visual_widget.h"
#include "gui/sat_surface.h"

//----------

class SAT_SurfaceWidget
: public SAT_VisualWidget {

//------------------------------
private:
//------------------------------

  SAT_Surface* MSurface = nullptr;

  bool MDrawSurface = true;

//------------------------------
public:
//------------------------------

  SAT_SurfaceWidget(SAT_Rect ARect, SAT_Surface* ASurface=nullptr)
  : SAT_VisualWidget(ARect) {
    setName("SAT_SurfaceWidget");
    setHint("SAT_SurfaceWidget");
    MSurface = ASurface;
  }

  //----------

  virtual ~SAT_SurfaceWidget() {
  }

//------------------------------
public:
//------------------------------

  void setDrawSurface(bool ADraw=true)    { MDrawSurface = ADraw; }
  void setSurface(SAT_Surface* ASurface)  { MSurface = ASurface; }

//------------------------------
public:
//------------------------------

  virtual void drawSurface(SAT_PaintContext* AContext) {
    if (MDrawSurface) {
      //SAT_TRACE;
      SAT_Painter* painter = AContext->painter;
      SAT_Rect rect = getRect();
      //painter->drawSurface(rect.x,rect.y,MSurface);
      painter->drawSurface(rect.x,rect.y,MSurface,SAT_Rect(0,0,rect.w,rect.h));
    }
  }

//------------------------------
public: // on_widget
//------------------------------

  void on_widget_paint(SAT_PaintContext* AContext) override {
    //SAT_TRACE;
    //fillBackground(AContext);
    drawSurface(AContext);
    paintChildren(AContext);
    drawBorder(AContext);
  }

};

#endif // 0

//----------------------------------------------------------------------
#endif