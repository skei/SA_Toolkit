#ifndef sat_logo_widget_included
#define sat_logo_widget_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "gui/widgets/sat_panel_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_LogoWidget
: public SAT_PanelWidget {

//------------------------------
private:
//------------------------------

  bool        MDrawLogo   = true;
  NSVGimage*  MLogoImage  = nullptr;
  SAT_Color   MLogoColor  = SAT_White;
  double      MLogoWidth  = 8;

//------------------------------
public:
//------------------------------

  SAT_LogoWidget(SAT_Rect ARect)
  : SAT_PanelWidget(ARect) {
    setFillBackground(false);
    setDrawBorder(false);
    // open() ?
    MLogoImage = nsvgParseFromFile("/home/skei/Code/SA_Toolkit/data/img/SA_black circle.svg", "px", 96);
    //SAT_Assert(MLogoImage);
  }

  //----------

  virtual ~SAT_LogoWidget() {
    nsvgDelete(MLogoImage);
  }

//------------------------------
public:
//------------------------------

  virtual void setDrawLogo(bool ADraw=true)   { MDrawLogo = ADraw; }
  virtual void setLogoColor(SAT_Color AColor) { MLogoColor = AColor; }
  virtual void setLogoWidth(double AWidth)    { MLogoWidth = AWidth; }

//------------------------------
public:
//------------------------------

  virtual void drawLogo(SAT_PaintContext* AContext) {
    SAT_Assert(AContext);

    if (MDrawLogo) {

      double S = getWindowScale();

      SAT_Painter* painter = AContext->painter;
      SAT_Assert(painter);

      SAT_Rect mrect = getRect();

      if (!MLogoImage) return;
      if (MLogoImage->width <= 0.0 ) return;
      if (MLogoImage->height <= 0.0 ) return;

      painter->setDrawColor(MLogoColor);
      painter->setLineWidth(MLogoWidth*S);
      painter->setLineCap(NVG_ROUND);       // NVG_BUTT (default), NVG_ROUND, NVG_SQUARE
      //painter->setLineJoin(NVG_MITER);    // NVG_MITER (default), NVG_ROUND, NVG_BEVEL

      double scale  = (mrect.w / MLogoImage->width);

      for (NSVGshape* shape = MLogoImage->shapes; shape != NULL; shape = shape->next) {
        for (NSVGpath* path = shape->paths; path != NULL; path = path->next) {
          //painter->setLineWidth( S * shape->strokeWidth );
          for (int i = 0; i < path->npts-1; i += 3) {
            float* p = &path->pts[i*2];
            double x1 = mrect.x + (p[0] * scale);
            double y1 = mrect.y + (p[1] * scale);
            double x2 = mrect.x + (p[2] * scale);
            double y2 = mrect.y + (p[3] * scale);
            double x3 = mrect.x + (p[4] * scale);
            double y3 = mrect.y + (p[5] * scale);
            double x4 = mrect.x + (p[6] * scale);
            double y4 = mrect.y + (p[7] * scale);
            painter->drawCurveBezier(x1,y1, x4,y4, x2,y2, x3,y3);
          }
        }
      }

      // reset
      painter->setLineCap(NVG_BUTT);

    }
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(SAT_PaintContext* AContext) override {
    fillBackground(AContext);
    drawLogo(AContext);
    drawBorder(AContext);
  }

};

//----------------------------------------------------------------------
#endif

