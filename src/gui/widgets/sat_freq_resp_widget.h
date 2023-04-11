#ifndef sat_freq_resp_widget_included
#define sat_freq_resp_widget_included
//----------------------------------------------------------------------

// a mess..
// in progress..

#include "base/sat.h"
#include "plugin/sat_parameter.h"
#include "gui/widgets/sat_panel_widget.h"

#define FFT_SIZE  128
#define NUM_FREQUS 128

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_FR_Noop {
private:
  float value   = 0.0;
  float factor  = 0.5;
public:
  void reset() {
    value = 0.0;
  }
  float process(float AInput) {
    value += (AInput - value) * factor;
    return value;
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

  //void makeWave(float *waveSamples, int32_t tableLen) {
  //  float *freqRe = MIP_New float [tableLen];
  //  float *freqIm = MIP_New float [tableLen];
  //  // take FFT
  //  for (int32_t idx = 0; idx < tableLen; idx++) {
  //    freqIm[idx] = waveSamples[idx];
  //    freqRe[idx] = 0.0;
  //  }
  //  fft(tableLen, freqRe, freqIm);
  //}
  
  void fft(int32_t N, float *ar, float *ai) {
    /*
      in-place complex fft
      After Cooley, Lewis, and Welch; from Rabiner & Gold (1975)
      program adapted from FORTRAN
      by K. Steiglitz  (ken@princeton.edu)
      Computer Science Dept.
      Princeton University 08544
    */
    int32_t i, j, k, L;             // indexes
    int32_t M, TEMP, LE, LE1, ip;   // M = log N
    int32_t NV2, NM1;
    float t;                    // temp
    float Ur, Ui, Wr, Wi, Tr, Ti;
    float Ur_old;
    // if ((N > 1) && !(N & (N - 1)))   // make sure we have a power of 2
    NV2 = N >> 1;
    NM1 = N - 1;
    TEMP = N; // get M = log N
    M = 0;
    while (TEMP >>= 1) ++M;
    // shuffle
    j = 1;
    for (i = 1; i <= NM1; i++) {
      if(i<j) {             // swap a[i] and a[j]
        t = ar[j-1];
        ar[j-1] = ar[i-1];
        ar[i-1] = t;
        t = ai[j-1];
        ai[j-1] = ai[i-1];
        ai[i-1] = t;
      }
      k = NV2;             // bit-reversed counter
      while(k < j) {
        j -= k;
        k /= 2;
      }
      j += k;
    }
    LE = 1.;
    for (L = 1; L <= M; L++) {            // stage L
      LE1 = LE;                         // (LE1 = LE/2)
      LE *= 2;                          // (LE = 2^L)
      Ur = 1.0;
      Ui = 0.;
      Wr = cos(M_PI/(float)LE1);
      Wi = -sin(M_PI/(float)LE1); // Cooley, Lewis, and Welch have "+" here
      for (j = 1; j <= LE1; j++) {
        for (i = j; i <= N; i += LE) { // butterfly
          ip = i+LE1;
          Tr = ar[ip-1] * Ur - ai[ip-1] * Ui;
          Ti = ar[ip-1] * Ui + ai[ip-1] * Ur;
          ar[ip-1] = ar[i-1] - Tr;
          ai[ip-1] = ai[i-1] - Ti;
          ar[i-1]  = ar[i-1] + Tr;
          ai[i-1]  = ai[i-1] + Ti;
        }
        Ur_old = Ur;
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
    //DFT(rbuffer,FFT_SIZE,tbuffer,FFT_SIZE);
    fft(FFT_SIZE,rbuffer,ibuffer);
    for (uint32_t i=0; i<FFT_SIZE; i++) buffer[i] = sqrt((rbuffer[i]*rbuffer[i]) + (ibuffer[i]*ibuffer[i]));
  }

//------------------------------
public:
//------------------------------

  virtual void drawFreqResp(SAT_PaintContext* AContext) {
    SAT_PRINT;
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
      
      //float xx = mrect.x;
      float index = 0.0;
      float adder = (1.0 / (double)mrect.w) * (double)FFT_SIZE * 0.5;
      
      float prevy;
      if (MIsBipolar) prevy = mrect.y + h05;
      else prevy = mrect.y2();

      double xpos = mrect.x;
      
      for (uint32_t i=0; i<mrect.w; i++) {
        uint32_t p = (uint32_t)SAT_Trunc(index);

//        float re = rbuffer[p];
//        float im = ibuffer[p];
//        float v = sqrt((re*re) + (im*im));

        float v = buffer[p];

        //SAT_Print("%i (index %f adder %f) = %f\n",i,index,adder,v);
        
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
#undef NUM_FREQUS


//----------------------------------------------------------------------
#endif

