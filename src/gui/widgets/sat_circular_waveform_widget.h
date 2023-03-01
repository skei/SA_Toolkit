#ifndef sat_circular_waveform_widget_included
#define sat_circular_waveform_widget_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "base/utils/sat_interpolation.h"
#include "gui/sat_paint_context.h"
#include "gui/widgets/sat_panel_widget.h"

//----------------------------------------------------------------------

#define MAX_WAVEFORM_LINES    4096
#define MAX_WAVEFORM_AREAS    16
#define MAX_WAVEFORM_MARKERS  16

#define DEFAULT_NUM_LINES     360

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

struct SAT_WaveformArea {
  bool      active  = false;
  double    size    = 0.0;
  double    pos     = 0.0;
  SAT_Color color   = SAT_Green;
};

//----------------------------------------------------------------------

class SAT_CircularWaveformWidget
: public SAT_PanelWidget {

//------------------------------
private:
//------------------------------

  uint32_t  MNumWaveformLineCoords                          = 0;
  double    MWaveformLineCoords[MAX_WAVEFORM_LINES * 4]     = {0};
  double    MWaveformDrawCoords[MAX_WAVEFORM_LINES * 4]     = {0};

//------------------------------
protected:
//------------------------------

  uint32_t  MAudioBufferSize                        = 0;
  float*    MAudioBuffer                            = nullptr;
  bool      MAudioBufferIsStereo                    = true;

  double    MOuterRadius                            = 1.0;
  double    MInnerRadius                            = 0.5;
  bool      MBipolar                                = true; // -> SAT_WaveformWidget?

  SAT_Color MWaveformColor                          = SAT_Color(1.0);
  double    MWaveformWidth                          = 1.0;
  SAT_Color MWaveformBackgroundColor                = SAT_Color(0.25);

  SAT_Color MZeroLineColor                          = SAT_Color(0.6);
  double    MZeroLineWidth                          = 1.0;

  uint32_t  MNumGrid                                = 4;
  uint32_t  MNumSubGrid                             = 4;
  double    MGridWidth                              = 0.002;
  double    MSubGridWidth                           = 0.001;
  SAT_Color MGridColor                              = SAT_Color(0.1);
  SAT_Color MSubGridColor                           = SAT_Color(0.2);

  //uint32_t  MNumMarkers                             = 0;
  //uint32_t  MNumAreas                               = 0;

  SAT_WaveformArea  MMarkers[MAX_WAVEFORM_MARKERS]  = {};
  SAT_WaveformArea  MAreas[MAX_WAVEFORM_AREAS]      = {};

//------------------------------
public:
//------------------------------

  SAT_CircularWaveformWidget(SAT_Rect ARect, uint32_t ANumLines=DEFAULT_NUM_LINES)
  : SAT_PanelWidget(ARect) {
    setFillBackground(true);
    setBackgroundColor(0.2);
    setDrawBorder(false);
    precalcLineCoords(ANumLines,ARect);
  }

  //----------

  virtual ~SAT_CircularWaveformWidget() {
    //free(MAudioBuffer);
  }

//------------------------------
public:
//------------------------------

  virtual void setAudioBuffer(uint32_t ASize, float* ABuffer, bool AStereo=true) {
    MAudioBufferSize = ASize;
    MAudioBuffer = ABuffer;
    MAudioBufferIsStereo = AStereo;
  }

  virtual void setRadius(double AOuter, double AInner) {
    MOuterRadius = AOuter;
    MInnerRadius = AInner;
  }

  virtual void setBipolar(bool ABipolar) {
    MBipolar = ABipolar;
  }

  virtual void setWaveformColor(SAT_Color AColor)           { MWaveformColor = AColor; }
  virtual void setWaveformWidth(double AWidth)              { MWaveformWidth = AWidth; }
  virtual void setWaveformBackgroundColor(SAT_Color AColor) { MWaveformBackgroundColor = AColor; }

  virtual void setZeroLineColor(SAT_Color AColor)           { MZeroLineColor = AColor; }
  virtual void setZeroLineWidth(double AWidth)              { MZeroLineWidth = AWidth; }

  virtual void setGrid(uint32_t AGrid, uint32_t ASubGrid) {
    MNumGrid = AGrid;
    MNumSubGrid = ASubGrid;
  }

  virtual void setGrid(uint32_t AGrid) {
    MNumGrid = AGrid;
  }

  virtual void setSubGrid(uint32_t ASubGrid) {
    MNumSubGrid = ASubGrid;
  }

  virtual void setGridColor(SAT_Color AGrid, SAT_Color ASubGrid) {
    MGridColor = AGrid;
    MSubGridColor = ASubGrid;
  }

  virtual void setGridWidth(double AGrid=0.002, double ASubGrid=0.001) {
    MGridWidth = AGrid;
    MSubGridWidth = ASubGrid;
  }

  //virtual void setNumAreas(uint32_t ANum) {
  //  MNumAreas = ANum;
  //}

  virtual void setAreaActive(uint32_t AIndex, bool AActive=true) {
    MAreas[AIndex].active = AActive;
  }

  virtual void setAreaPos(uint32_t AIndex, double APos, double ASize) {
    MAreas[AIndex].pos = APos;
    MAreas[AIndex].size = ASize;
  }

  virtual void setAreaColor(uint32_t AIndex, SAT_Color AColor) {
    MAreas[AIndex].color = AColor;
  }

  //virtual void setNumMarkers(uint32_t ANum) {
  //  MNumMarkers = ANum;
  //}

  virtual void setMarkerActive(uint32_t AIndex, bool AActive=true) {
    MMarkers[AIndex].active = AActive;
  }

  virtual void setMarkerPos(uint32_t AIndex, double APos, double ASize=0.0025) {
    MMarkers[AIndex].pos = APos;
    MMarkers[AIndex].size = ASize;
  }

  virtual void setMarkerColor(uint32_t AIndex, SAT_Color AColor) {
    MMarkers[AIndex].color =AColor;
  }

//------------------------------
private:
//------------------------------

  void drawSegment(SAT_Painter* APainter, double cx, double cy, double r, double v1, double v2) {
    if (v2 > 0.0) {
      double a1 = v1 + 0.75 + v2;
      double a2 = v2;
      a1 *= SAT_PI2;
      a2 *= SAT_PI2;
      APainter->drawArc(cx,cy,r,a1,a2);
    }
  }

//------------------------------
public:
//------------------------------

  virtual void precalcLineCoords(uint32_t ANum, SAT_Rect ARect) {
    MNumWaveformLineCoords = ANum;
    if (MNumWaveformLineCoords > 0) {
      double size = SAT_Min(ARect.w,ARect.h) * 0.5;
      double center_x = ARect.x + size;
      double center_y = ARect.y + size;
      double angle = -0.25; // start at 12 o'clock
      double angle_add = 1.0f / (double)MNumWaveformLineCoords;
      for (uint32_t i=0; i<MNumWaveformLineCoords; i++) {
        //double a1 = angle - (angle_add * 0.5);
        //double a2 = angle + (angle_add * 0.5);
        //a1 = SAT_Fract(a1);
        //a2 = SAT_Fract(a2);
        angle = SAT_Fract(angle);
        double cos_angle = cosf(angle * SAT_PI2);
        double sin_angle = sinf(angle * SAT_PI2);
        double x1 = center_x + (cos_angle * (size * MInnerRadius));
        double y1 = center_y + (sin_angle * (size * MInnerRadius));
        double x2 = center_x + (cos_angle * (size * MOuterRadius));
        double y2 = center_y + (sin_angle * (size * MOuterRadius));
        //APainter->drawLine(xcenter+prevx,ycenter+prevy,xcenter+x,ycenter+y,MLineColor);
        //APainter->lineTo(xcenter+x,ycenter+y);
        MWaveformLineCoords[(i * 4)    ] = x1;
        MWaveformLineCoords[(i * 4) + 1] = y1;
        MWaveformLineCoords[(i * 4) + 2] = x2;
        MWaveformLineCoords[(i * 4) + 3] = y2;
        angle += angle_add;
      }
    }
  }

  //----------

  virtual void drawAreas(SAT_PaintContext* AContext) {
    //if (MNumAreas > 0) {
      SAT_Painter* painter = AContext->painter;
      SAT_Rect mrect = getRect();
      //SAT_Window* window = (SAT_Window*)getOwnerWindow();
      //double S = window->getWindowScale();
      double cx = mrect.x + (mrect.w * 0.5);
      double cy = mrect.y + (mrect.h * 0.5);
      double size = SAT_Min(mrect.w,mrect.h) * 0.5;
      double ri = size * MInnerRadius;
      double ro = size * MOuterRadius;
      double rw = ro - ri;
      double r  = ri + (rw * 0.5);
      painter->setLineWidth(rw);
      painter->setDrawColor( MWaveformBackgroundColor );
      drawSegment(painter,cx,cy,r,0,SAT_PI2);
      painter->setLineWidth(rw);
      //for (uint32_t i=0; i<MNumAreas; i++) {
      for (uint32_t i=0; i<MAX_WAVEFORM_AREAS; i++) {
        if (MAreas[i].active) {
          double m1 = MAreas[i].pos;
          double m2 = MAreas[i].size;
          painter->setDrawColor(MAreas[i].color);
          drawSegment(painter,cx,cy,r,m1,m2);
        }
      }
    //}
  }

  //----------

  virtual void drawMarkers(SAT_PaintContext* AContext) {
    //if (MNumMarkers > 0) {
      SAT_Painter* painter = AContext->painter;
      SAT_Rect mrect = getRect();
      //SAT_Window* window = (SAT_Window*)getOwnerWindow();
      double S = getWindowScale();
      double cx = mrect.x + (mrect.w * 0.5);
      double cy = mrect.y + (mrect.h * 0.5);
      double size = SAT_Min(mrect.w,mrect.h) * 0.5;
      double ri = size * MInnerRadius;
      double ro = size * MOuterRadius;
      double rw = ro - ri;
      double r  = ri + (rw * 0.5);
      painter->setLineWidth(rw);
      //for (uint32_t i=0; i<MNumMarkers; i++) {
      for (uint32_t i=0; i<MAX_WAVEFORM_MARKERS; i++) {
        if (MMarkers[i].active) {
          double m1 = MMarkers[i].pos;
          double m2 = MMarkers[i].size * S;
          painter->setDrawColor(MMarkers[i].color);
          drawSegment(painter,cx,cy,r,m1,m2);
        }
      }
    //}
  }

  //----------

  virtual void drawGrid(SAT_PaintContext* AContext) {
    uint32_t num = MNumGrid * MNumSubGrid;
    if (num > 1) {
      SAT_Painter* painter = AContext->painter;
      SAT_Rect mrect = getRect();
      //SAT_Window* window = (SAT_Window*)getOwnerWindow();
      //double S = window->getWindowScale();
      double cx = mrect.x + (mrect.w * 0.5);
      double cy = mrect.y + (mrect.h * 0.5);
      double size = SAT_Min(mrect.w,mrect.h) * 0.5;
      double ri = size * MInnerRadius;
      double ro = size * MOuterRadius;
      double rw = ro - ri;
      double r  = ri + (rw * 0.5);
      painter->setLineWidth(rw);
      double pos = 0.0;
      double adder = 1.0 / (double)num;
      for (uint32_t i=0; i<num; i++) {
        if ((i % MNumSubGrid) == 0) {
          painter->setDrawColor(MGridColor);
          drawSegment(painter,cx,cy,r,pos,MGridWidth);
        }
        else {
          painter->setDrawColor(MSubGridColor);
          drawSegment(painter,cx,cy,r,pos,MSubGridWidth);
        }
        pos += adder;
      }

    }
  }

  //----------

  virtual void drawWaveform(SAT_PaintContext* AContext) {
    //SAT_Window* window = (SAT_Window*)getOwnerWindow();
    double S = getWindowScale();
    SAT_Painter* painter = AContext->painter;
    SAT_Rect mrect = getRect();
    if (MAudioBuffer && (MAudioBufferSize > 0)) {
      // waveform
      painter->setDrawColor(MWaveformColor);
      painter->setLineWidth(MWaveformWidth * S);
      double bufferpos = 0.0;
      double bufferadd = (double)MAudioBufferSize / (double)MNumWaveformLineCoords;
      for (uint32_t i=0; i<MNumWaveformLineCoords; i++) {
        uint32_t ip = SAT_Trunc(bufferpos);
        bufferpos += bufferadd;
        float vv = 0.0;
        if (MAudioBufferIsStereo) {
          float v1 = MAudioBuffer[(ip * 2)    ]; // left
          float v2 = MAudioBuffer[(ip * 2) + 1]; // right
          vv = (v1 + v2) * 0.5;
        }
        else {
          vv = MAudioBuffer[ip];
        }
        SAT_Clamp(vv,-1,1);
        float v  = (vv * 0.5) + 0.5;            //  0..1
        double x1 = MWaveformLineCoords[(i * 4)    ] * S;
        double y1 = MWaveformLineCoords[(i * 4) + 1] * S;
        double x2 = MWaveformLineCoords[(i * 4) + 2] * S;
        double y2 = MWaveformLineCoords[(i * 4) + 3] * S;
        double sx = SAT_Interpolate_Linear(0.5,x1,x2);
        double sy = SAT_Interpolate_Linear(0.5,y1,y2);
        double ex = SAT_Interpolate_Linear(v,x1,x2);
        double ey = SAT_Interpolate_Linear(v,y1,y2);
        MWaveformDrawCoords[(i * 4)    ] = sx;
        MWaveformDrawCoords[(i * 4) + 1] = sy;
        MWaveformDrawCoords[(i * 4) + 2] = ex;
        MWaveformDrawCoords[(i * 4) + 3] = ey;
        //painter->drawLine(sx,sy,ex,ey);

      }
      painter->drawLines(MNumWaveformLineCoords,MWaveformDrawCoords);

    }
    // zero-line
    double cx = mrect.x + (mrect.w * 0.5);
    double cy = mrect.y + (mrect.h * 0.5);
    double size = SAT_Min(mrect.w,mrect.h) * 0.5;
    double ri = size * MInnerRadius;
    double ro = size * MOuterRadius;
    double rw = ro - ri; //MOuterRadius - MInnerRadius;
    double r  = ri + (rw * 0.5);
    painter->setDrawColor(MZeroLineColor);
    painter->setLineWidth(MZeroLineWidth * S);
    drawSegment(painter,cx,cy,r,0,SAT_PI2);
  }

  //----------

//------------------------------
public:
//------------------------------

  void on_widget_paint(SAT_PaintContext* AContext) override {
    fillBackground(AContext);
    drawAreas(AContext);
    drawMarkers(AContext);
    drawGrid(AContext);
    drawWaveform(AContext);
    //paintChildWidgets(AContext);
    drawBorder(AContext);
  }

};

//----------------------------------------------------------------------

#undef MAX_WAVEFORM_LINES
#undef MAX_WAVEFORM_AREAS
#undef MAX_WAVEFORM_MARKERS
#undef NUM_TEST_SAMPLES
#undef DEFAULT_NUM_LINES

//----------------------------------------------------------------------
#endif
