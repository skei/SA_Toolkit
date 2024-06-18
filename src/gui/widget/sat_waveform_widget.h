#ifndef sat_waveform_widget_included
#define sat_waveform_widget_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/widget/sat_visual_widget.h"

#define NUM_MARKERS 16
#define NUM_AREAS   16

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_WaveformWidget
: public SAT_VisualWidget {

//------------------------------
protected:
//------------------------------

  bool      MDrawWaveform   = true;
  double    MWaveformWidth  = 1.0;
  SAT_Color MWaveformColor  = SAT_Color(0.25);//SAT_COLOR_DARK_GRAY;

  bool      MDrawZeroLine   = true;
  double    MZeroLineWidth  = 0.5;
  SAT_Color MZeroLineColor  = SAT_Color(0.2);//SAT_COLOR_BLACK;

  //

  float*    MLeftBuffer   = nullptr;
  float*    MRightBuffer  = nullptr;

  float*    MBuffer       = nullptr;
  uint32_t  MNumSamples   = 0;

  uint32_t  MNumGrid      = 0;
  uint32_t  MNumSubGrid   = 0;

  double    MGridSize     = 1.0;
  double    MSubGridSize  = 0.8;

  SAT_Color MGridColor    = SAT_Color(0.3);//SAT_COLOR_BLACK;
  SAT_Color MSubGridColor = SAT_Color(0.35);//SAT_COLOR_DARK_GRAY;

  bool      MMarkerActive[NUM_MARKERS]  = {false};
  uint32_t  MMarkerPos[NUM_MARKERS]     = {0};
  SAT_Color MMarkerColor[NUM_MARKERS]   = { SAT_Color(0.25)/*SAT_COLOR_DARK_GRAY*/};
  double    MMarkerWidth[NUM_MARKERS]   = {1};

  bool      MAreaActive[NUM_AREAS]    = {false};
  uint32_t  MAreaStart[NUM_AREAS]     = {0};
  uint32_t  MAreaLength[NUM_AREAS]    = {0};
  SAT_Color MAreaColor[NUM_AREAS]     = { SAT_Color(0,0.75,0,1)/*SAT_COLOR_GREEN*/};

  double    MCoords[4096] = {0};
  uint32_t  MNumCoords    = 0;

//------------------------------
public:
//------------------------------

  SAT_WaveformWidget(SAT_Rect ARect)
  : SAT_VisualWidget(ARect) {
    setName("SAT_WaveformWidget");
    setHint("SAT_WaveformWidget");
  }

  //----------

  virtual ~SAT_WaveformWidget() {
  }

//------------------------------
public:
//------------------------------

  //test
  virtual int getInt() { return 1; };

  virtual void setDrawWaveform(bool ADraw=true) { MDrawWaveform = ADraw; }
  virtual void setWaveformWidth(double AWidth) { MWaveformWidth = AWidth; }
  virtual void setWaveformColor(SAT_Color AColor) { MWaveformColor = AColor; }

  virtual void setNumGrid(uint32_t ANum)    { MNumGrid = ANum; }
  virtual void setNumSubGrid(uint32_t ANum) { MNumSubGrid = ANum; }

  virtual void setBuffer(float* ABuffer, uint32_t ASamples) {
    MBuffer       = ABuffer;
    MNumSamples   = ASamples;
  }

  virtual void setBuffers(float* ALeftBuffer, float* ARightBuffer, uint32_t ASamples) {
    MLeftBuffer   = ALeftBuffer;
    MRightBuffer  = ARightBuffer;
    MNumSamples   = ASamples;
  }

  virtual void setMarkerActive(uint32_t AIndex, bool AActive) {
    MMarkerActive[AIndex] = AActive;
  }

  virtual void setMarkerPos(uint32_t AIndex, uint32_t APos) {
    MMarkerPos[AIndex]    = APos;
  }

  virtual void setMarkerColor(uint32_t AIndex, SAT_Color AColor) {
    MMarkerColor[AIndex]  = AColor;
  }

  virtual void setMarkerWidth(uint32_t AIndex, double AWidth) {
    MMarkerWidth[AIndex]  = AWidth;
  }

  virtual void setAreaActive(uint32_t AIndex, bool AActive) {
    MAreaActive[AIndex] = AActive;
  }

  virtual void setAreaStart(uint32_t AIndex, uint32_t AStart) {
    MAreaStart[AIndex]  = AStart;
  }

  virtual void setAreaLength(uint32_t AIndex, uint32_t ALength) {
    MAreaLength[AIndex] = ALength;
  }

  virtual void setAreaColor(uint32_t AIndex, SAT_Color AColor) {
    MAreaColor[AIndex]  = AColor;
  }

//------------------------------
public: // parent to child
//------------------------------

//  void on_widget_move(double AXpos, double AYpos)  override {}
//  void on_widget_resize(double AWidth, double AHeight) override {}
//  void on_widget_align(bool ARecursive=true) override {}

  void on_widget_paint(SAT_PaintContext* AContext) override {
    drawDropShadow(AContext);
    fillBackground(AContext);
    drawWaveform(AContext);
    paintChildren(AContext);
    drawBorder(AContext);
  }

//  void on_widget_key_press(uint8_t AChar, uint32_t AKeySym, uint32_t AState, uint32_t ATime) override {}
//  void on_widget_key_release(uint32_t AKey, uint32_t AState, uint32_t ATime) override {}
//  void on_widget_mouse_click(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {}
//  void on_widget_mouse_release(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {}
//  void on_widget_mouse_move(uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {}
//  void on_widget_enter(SAT_Widget* AFrom, double AXpos, double AYpos, uint32_t ATime) override {}
//  void on_widget_leave(SAT_Widget* ATo, double AXpos, double AYpos, uint32_t ATime) override {}
//  //void on_widget_connect(SAT_Parameter* AParameter) override {}

//------------------------------
public:
//------------------------------

  virtual void drawWaveform(SAT_PaintContext* AContext) {

    double S = getWindowScale();
    SAT_Painter* painter = AContext->painter;
    SAT_Rect mrect = getRect();

    //SAT_Print("MRect.w %.2f MBuffer %p MNumSamples %i\n",MRect.w,MBuffer,MNumSamples);
    double widget_width = mrect.w;
    if (widget_width <= 0.0) return;
    double half_height = mrect.h * 0.5;
    double mid_y = mrect.y + half_height;

    //SAT_Painter* painter = AContext->painter;

    // waveform

    if (MDrawWaveform) {
      if (MBuffer && (MNumSamples > 0)) {
        double pos = 0;
        double step = (double)MNumSamples / mrect.w;
        //        painter->beginPath();
        //        painter->strokeColor(MWaveformColor);
        //        painter->strokeWidth(MWaveformWidth);
        MNumCoords = 0;
        for (uint32_t i=0; i<mrect.w; i++) {
          uint32_t p = (uint32_t)pos;
          double v = MBuffer[p];
          v = SAT_Clamp(v,-1,1);
          v *= half_height;
          double y = mid_y - v;
          //          if (i==0) painter->moveTo(MRect.x,y);
          //          else painter->lineTo(MRect.x+i,y);
          MCoords[MNumCoords++] = mrect.x + i;
          MCoords[MNumCoords++] = y;

          pos += step;
        }
        //        painter->stroke();
        painter->setDrawColor(MWaveformColor);
        painter->setLineWidth(MWaveformWidth * S);
        painter->drawLineStrip(MNumCoords / 2,MCoords);

      }
    }

    // zero-line

    if (MDrawZeroLine) {
      painter->setDrawColor(MZeroLineColor);
      painter->setLineWidth(MZeroLineWidth * S);
      painter->drawLine(mrect.x,mid_y, mrect.x2(),mid_y);
    }

    // sub grid

    uint32_t num = MNumGrid * MNumSubGrid;
    if (num > 0) {
      double width = mrect.w / (double)num;
      double x = mrect.x + width;
      //      painter->beginPath();
      painter->setDrawColor(MSubGridColor);

      painter->setLineWidth(MSubGridSize * S);

      for (uint32_t i=0; i<(num-1); i++) {
        //        painter->moveTo(x,mrect.y);
        //        painter->lineTo(x,mrect.y2());
        painter->drawLine(x,mrect.y, x,mrect.y2());
        x += width;
      }
      //      painter->stroke();
    }

    // grid

    num = MNumGrid;
    if (num > 0) {
      double width = mrect.w / (double)num;
      double x = mrect.x + width;
      //      painter->beginPath();
      painter->setDrawColor(MGridColor);

      painter->setLineWidth(MGridSize * S);

      for (uint32_t i=0; i<(num-1); i++) {
        //        painter->moveTo(x,mrect.y);
        //        painter->lineTo(x,mrect.y2());
        painter->drawLine(x,mrect.y,x,mrect.y2());
        x += width;
      }
      //      painter->stroke();
    }

    // areas

    for (uint32_t i=0; i<NUM_AREAS; i++) {
      if (MAreaActive[i]) {
        double x = (double)MAreaStart[i]  * widget_width / (double)MNumSamples;
        double w = (double)MAreaLength[i] * widget_width / (double)MNumSamples;
        //        painter->beginPath();
        //        painter->rect(mrect.x + x,mrect.y, w,mrect.h);
        //        painter->fillColor(MAreaColor[i]);
        //        painter->fill();
        painter->setFillColor(MAreaColor[i]);
        painter->fillRect(mrect.x + x,mrect.y, w,mrect.h);

      }
    }

    // markers

    for (uint32_t i=0; i<NUM_MARKERS; i++) {
      if (MMarkerActive[i]) {
        double x = (double)MMarkerPos[i] * widget_width / (double)MNumSamples;
        //        painter->beginPath();
        //        painter->moveTo(mrect.x + x,mrect.y);
        //        painter->lineTo(mrect.x + x,mrect.y2());
        //        painter->strokeWidth(MMarkerWidth[i]);
        //        painter->strokeColor(MMarkerColor[i]);
        //        painter->stroke();
        painter->setDrawColor(MMarkerColor[i]);
        painter->setLineWidth(MMarkerWidth[i] * S);
        painter->drawLine(mrect.x + x,mrect.y, mrect.x + x,mrect.y2());
      }
    }


  }

};

//----------------------------------------------------------------------
#endif

