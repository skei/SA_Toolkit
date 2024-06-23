#ifndef sa_mael_widgets_included
#define sa_mael_widgets_included
//----------------------------------------------------------------------

#include "sat.h"
#include "audio/synthesis/sat_morph_oscillator.h"
#include "gui/sat_widgets.h"

//----------------------------------------------------------------------

class sa_mael_osc_knob_widget
: public SAT_KnobWidget {

//------------------------------
private:
//------------------------------

  SAT_MorphOscillator<sat_sample_t> MOsc = {};

//------------------------------
public:
//------------------------------

  sa_mael_osc_knob_widget(SAT_Rect ARect, const char* AText="%", double AValue=0.0)
  : SAT_KnobWidget(ARect,AText,AValue) {
  }

  //----------

  virtual ~sa_mael_osc_knob_widget() {
  }

//------------------------------
public:
//------------------------------

  // void drawValue(SAT_PaintContext* AContext) override {
  //   SAT_Assert(AContext);
  //   if (MDrawValue) {
  //     double S = AContext->scale;
  //     SAT_Painter* painter = AContext->painter;
  //     SAT_Assert(painter);
  //     SAT_Rect mrect = getRect();
  //     SAT_Rect valueoffset = MValueOffset;
  //     valueoffset.scale(S);
  //     mrect.shrink(valueoffset);
  //     if (mrect.w < 1.0) return;
  //     if (mrect.h < 1.0) return;
  //
  //     SAT_Parameter* param = (SAT_Parameter*)getParameter();
  //
  //     double value = getValue(); // assume 0..1
  //     MOsc.setMorph(value);
  //
  //     double width = mrect.w;
  //     double height = mrect.h;
  //
  //     double ph = 0.0;
  //     double phadd = (1.0 / width);
  //
  //     SAT_Color color = MValueColor;
  //     if (!isEnabled()) color.blend(MDisabledColor,MDisabledFactor);
  //     painter->setDrawColor(color);
  //     painter->setLineWidth(2.0 * S);
  //
  //     //painter->moveTo(mrect.x, mrect.y + (mrect.h * 0.5));
  //
  //     MOsc.setSampleRate(44100); // to satisfy assert.. :-/
  //     MOsc.setPhase(0.0);
  //     MOsc.setPhaseAdd(phadd);
  //     double v = MOsc.process();  // -1..1
  //     v = (v + 1.0) * 0.5;        // 0..1
  //
  //     double xx = mrect.x;
  //     double yy = mrect.y2() - (mrect.h * v);
  //
  //     for (uint32_t x=0; x<mrect.w; x++) {
  //       MOsc.setSampleRate(44100); // to satisfy assert.. :-/
  //       MOsc.setPhase(ph);
  //       MOsc.setPhaseAdd(phadd);
  //       double v = MOsc.process();  // -1..1
  //       v = (v + 1.0) * 0.5;        // 0..1
  //       double y = mrect.y2() - (v * height);
  //       //painter->lineTo(mrect.x + x, y);
  //       painter->drawLineStrip(mrect.w,coords);
  //       xx = mrect.x + x;
  //       yy = y;
  //       ph += phadd;
  //     }
  //   }
  //
  // }

};

//----------------------------------------------------------------------
#endif
