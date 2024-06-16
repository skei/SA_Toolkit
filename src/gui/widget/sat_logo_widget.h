#ifndef sat_logo_widget_included
#define sat_logo_widget_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/widget/sat_visual_widget.h"

#include "../data/img/logo_svg.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_LogoWidget
: public SAT_VisualWidget {

//------------------------------
private:
//------------------------------

  bool        MDrawLogo     = true;
  SAT_Color   MLogoColor    = SAT_White;
  double      MLogoWidth    = 1.0;
  char*       MBuffer       = nullptr;
  SAT_Rect    MLogoOffset   = {0,0,0,0};

  #ifdef SAT_PAINTER_NANOVG
    NSVGimage*  MLogoImage    = nullptr;
  #endif

//------------------------------
public:
//------------------------------

  SAT_LogoWidget(SAT_Rect ARect)
  : SAT_VisualWidget(ARect) {
    setName("SAT_LogoWidget");
    setFillBackground(false);
    setDrawBorder(false);
    #ifdef SAT_PAINTER_NANOVG
      MBuffer = (char*)malloc(logo_svg_size + 1);
      memcpy(MBuffer,logo_svg,logo_svg_size);
      MBuffer[logo_svg_size] = 0;
      MLogoImage = nsvgParse(MBuffer, "px", 96);
      //SAT_Assert(MLogoImage);
    #endif
  }

  //----------

  virtual ~SAT_LogoWidget() {
    #ifdef SAT_PAINTER_NANOVG
      if (MBuffer) free(MBuffer);
      nsvgDelete(MLogoImage);
    #endif
  }

//------------------------------
public:
//------------------------------

  virtual void setDrawLogo(bool ADraw=true)     { MDrawLogo = ADraw; }
  virtual void setLogoColor(SAT_Color AColor)   { MLogoColor = AColor; }
  virtual void setLogoWidth(double AWidth)      { MLogoWidth = AWidth; }
  virtual void setLogoOffset(SAT_Rect AOffset)  { MLogoOffset = AOffset; }

//------------------------------
public:
//------------------------------

  virtual void drawLogo(SAT_PaintContext* AContext) {
    SAT_Assert(AContext);
    #ifdef SAT_PAINTER_NANOVG
      if (MDrawLogo) {

        double S = getWindowScale();

        SAT_Painter* painter = AContext->painter;
        SAT_Assert(painter);

        SAT_Rect mrect = getRect();
        SAT_Rect ofs = MLogoOffset;
        ofs.scale(S);
        mrect.shrink(ofs);

        if (!MLogoImage) return;
        if (MLogoImage->width <= 0.0 ) return;
        if (MLogoImage->height <= 0.0 ) return;

        painter->setDrawColor(MLogoColor);
        painter->setLineWidth(MLogoWidth*S);
        painter->setLineCap(NVG_ROUND);       // NVG_BUTT (default), NVG_ROUND, NVG_SQUARE
        //painter->setLineJoin(NVG_MITER);    // NVG_MITER (default), NVG_ROUND, NVG_BEVEL

        double wscale  = (mrect.w / MLogoImage->width);
        double hscale  = (mrect.h / MLogoImage->height);
        
        double scale = wscale;
        if (hscale < wscale) scale = hscale;

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
    #endif // SAT_PAINTER_NANOVG
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(SAT_PaintContext* AContext) override {
    drawDropShadow(AContext);
    fillBackground(AContext);
    drawLogo(AContext);
    paintChildren(AContext);
    drawBorder(AContext);
  }

};

//----------------------------------------------------------------------
#endif

