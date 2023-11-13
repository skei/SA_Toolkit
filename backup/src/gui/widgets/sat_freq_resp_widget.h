#ifndef sat_freq_resp_widget_included
#define sat_freq_resp_widget_included
//----------------------------------------------------------------------

// a mess..
// in progress..

#include "base/sat.h"
#include "plugin/sat_parameter.h"
#include "gui/widgets/sat_panel_widget.h"

#define FFT_SIZE  128

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_TestFreqRes {
private:
  float value   = 0.0;
  float factor  = 0.3;
public:
  void reset() {
    value = 0.0;
  }
  float process(float AInput) {
    value += (AInput - value) * factor;
    return value;
    //float result = (value + AInput) * 0.5;
    //value = AInput;
    //return result; // (AInput - result);
  }
};


//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

template <class PROC>
class SAT_FreqRespWidget
: public SAT_PanelWidget {

//------------------------------
private:
//------------------------------

  bool      MDrawFreqResp = true;
  bool      MIsBipolar    = false;
  SAT_Color MLineColor    = SAT_White;
  SAT_Color MFillColor    = SAT_Grey;
  PROC      MProcessor    = {};

  float rbuffer[FFT_SIZE]  = {0};
  float ibuffer[FFT_SIZE]  = {0};
  float  buffer[FFT_SIZE]  = {0};

//------------------------------
public:
//------------------------------

  SAT_FreqRespWidget(SAT_Rect ARect)
  : SAT_PanelWidget(ARect) {
    setName("SAT_FreqRespWidget");
    do_fft();
  }
  
  //----------

  virtual ~SAT_FreqRespWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setDrawFreqResp(bool ADraw=true)   { MDrawFreqResp = ADraw; }
  virtual void setLineColor(SAT_Color AColor)     { MLineColor = AColor; }
  virtual void setFillColor(SAT_Color AColor)     { MFillColor = AColor; }

//------------------------------
private:
//------------------------------

  /*
    in-place complex fft
    After Cooley, Lewis, and Welch; from Rabiner & Gold (1975)
    program adapted from FORTRAN
    by K. Steiglitz  (ken@princeton.edu)
    Computer Science Dept.
    Princeton University 08544
  */

  void fft(int32_t N, float *ar, float *ai) {
    int32_t NV2 = N >> 1;
    int32_t NM1 = N - 1;
    int32_t TEMP = N; // get M = log N
    int32_t M = 0;
    while (TEMP >>= 1) ++M;
    // shuffle
    int32_t j = 1;
    for (int32_t i=1; i<=NM1; i++) {
      if (i < j) {
        // swap a[i] and a[j]
        float t = ar[j-1];
        ar[j-1] = ar[i-1];
        ar[i-1] = t;
        t = ai[j-1];
        ai[j-1] = ai[i-1];
        ai[i-1] = t;
      }
      // bit-reversed counter
      int32_t k = NV2;
      while (k < j) {
        j -= k;
        k /= 2;
      }
      j += k;
    }
    int32_t LE = 1;
    for (int32_t L = 1; L <= M; L++) {      // stage L
      int32_t LE1 = LE;                     // (LE1 = LE/2)
      LE *= 2;                              // (LE = 2^L)
      float Ur = 1.0;
      float Ui = 0.;
      float Wr = cos(SAT_PI / (float)LE1);
      float Wi = -sin(SAT_PI / (float)LE1);   // Cooley, Lewis, and Welch have "+" here
      for (j = 1; j <= LE1; j++) {
        // butterfly
        for (int32_t i = j; i <= N; i += LE) {
          int32_t ip = i + LE1;
          float Tr = ar[ip - 1] * Ur - ai[ip - 1] * Ui;
          float Ti = ar[ip - 1] * Ui + ai[ip - 1] * Ur;
          ar[ip - 1] = ar[i - 1] - Tr;
          ai[ip - 1] = ai[i - 1] - Ti;
          ar[i  - 1] = ar[i - 1] + Tr;
          ai[i  - 1] = ai[i - 1] + Ti;
        }
        float Ur_old = Ur;
        Ur = Ur_old * Wr - Ui * Wi;
        Ui = Ur_old * Wi + Ui * Wr;
      }
    }
  }
  
  //----------
  
  void do_fft() {
    MProcessor.reset();
    for (uint32_t i=0; i<FFT_SIZE; i++) {
      float v = 0.0;
      if (i == 0) v = 1.0;
      rbuffer[i] = MProcessor.process(v);
      ibuffer[i] = 0.0;
    }
    fft(FFT_SIZE,rbuffer,ibuffer);
    for (uint32_t i=0; i<FFT_SIZE; i++) {
      buffer[i] = sqrt((rbuffer[i] * rbuffer[i]) + (ibuffer[i] * ibuffer[i]));
    }
  }

//------------------------------
public:
//------------------------------

  virtual void drawFreqResp(SAT_PaintContext* AContext) {
    //SAT_PRINT;
    SAT_Assert(AContext);
    if (MDrawFreqResp) {
      double S = getWindowScale();
      SAT_Painter* painter = AContext->painter;
      SAT_Assert(painter);
      SAT_Rect mrect = getRect();
      mrect.shrink(2);
      if (mrect.w <= 0.0) return;
      if (mrect.h <= 0.0) return;
      float h05 = mrect.h * 0.5;
      painter->setDrawColor(MLineColor);
      painter->setLineWidth(1*S);
      float index = 0.0;
      float adder = (1.0 / (double)mrect.w) * (double)FFT_SIZE * 0.5;
      float prevy;
      if (MIsBipolar) prevy = mrect.y + h05;
      else prevy = mrect.y2();
      double xpos = mrect.x;
      for (uint32_t i=0; i<mrect.w; i++) {
        // https://www.kvraudio.com/forum/viewtopic.php?p=6204732#p6204732
        float freqMin = 1;                // 20 (hz)
        float freqMax = FFT_SIZE * 0.5;   // 22050 (hz)
        float width   = mrect.w;          // 500 (pixels)
        //float F = (double)i;
        float X = (double)i;
        float F = freqMin * exp(log(freqMax / freqMin) * X / width);
        //float X = width * log( F / freqMin) / log(freqMax / freqMin);
        float fr = SAT_Fract(F);
        uint32_t p = (uint32_t)SAT_Trunc(F);
        float v = (buffer[p]   * (1.0 - fr)) + (buffer[p+1] * fr);
        float y;
        if (MIsBipolar) y = mrect.y + h05 - (v * h05);
        else y = mrect.y2() - (v * mrect.h);
        if (i==0) prevy = y;
        painter->drawLine(xpos,prevy,xpos+1.0,y);
        xpos += 1.0;
        prevy = y;
        index += adder;
      }
    }
  }


//------------------------------
public:
//------------------------------

  void on_widget_paint(SAT_PaintContext* AContext) override {
    drawDropShadow(AContext);
    fillBackground(AContext);
    drawFreqResp(AContext);
    paintChildWidgets(AContext);
    drawBorder(AContext);
  }

};

#undef FFT_SIZE

//----------------------------------------------------------------------
#endif

