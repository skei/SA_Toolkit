#ifndef sa_automata_widgets_included
#define sa_automata_widgets_included
//----------------------------------------------------------------------

/*
  note: we read from MBuffer when painting
  buffer might be modified from audio thread..
*/

#include "sat.h"
#include "gui/sat_widgets.h"

//----------------------------------------------------------------------
//
// grid
//
//----------------------------------------------------------------------

class sa_automata_grid
: public SAT_GridWidget {

//------------------------------
private:
//------------------------------

//  uint8_t   MStates[256 * 256] = {0};
uint8_t* MStates = nullptr;

//------------------------------
public:
//------------------------------

  sa_automata_grid(SAT_Rect ARect, uint32_t AColumns, uint32_t ARows, uint8_t* AStateBuffer)
  : SAT_GridWidget(ARect,AColumns,ARows) {
    MStates = AStateBuffer;
    setSelectCell(true);
    setSelectMultipleCells(true);

  }

//----------

  //virtual ~sa_automata_grid() {
  //}

//------------------------------
public:
//------------------------------

  void on_initCell(SAT_Rect ARect, int32_t AX, int32_t AY) final {
    //SAT_Print("x %i y %i\n",AX,AY);
    MStates[(AY*256) + AX] = 0;
  }

  //----------

  void on_clickCell(int32_t AX, int32_t AY, int32_t AB, int32_t AS) final {
    SAT_Print("x %i y %i\n",AX,AY);
    //uint32_t state = MStates[(AY*256)+AX];
    if (AB == SAT_BUTTON_LEFT) {
      if (AS & SAT_STATE_SHIFT) MStates[(AY*256)+AX] |= 2;
      else if (AS & SAT_STATE_CTRL) MStates[(AY*256)+AX] |= 4;
      else if (AS & SAT_STATE_ALT) MStates[(AY*256)+AX] |= 8;
      else MStates[(AY*256)+AX] |= 1;
    }
    else if (AB == SAT_BUTTON_RIGHT) {
      if (AS & SAT_STATE_SHIFT) MStates[(AY*256)+AX] &= (255 - 2);
      else if (AS & SAT_STATE_CTRL) MStates[(AY*256)+AX] &= (255 - 4);
      else if (AS & SAT_STATE_ALT) MStates[(AY*256)+AX] &= (255 - 8);
      else MStates[(AY*256)+AX] &= (255 - 1);
    }
  }

  //----------

  // void on_selectCells(int32_t AX, int32_t AY, int32_t AW, int32_t AH) final {
  //   SAT_Print("x %i y %i w %i h %i\n",AX,AY,AW,AH);
  // }

  //----------

  void on_paintCell(SAT_PaintContext* AContext, SAT_Rect ARect, int32_t AX, int32_t AY) final {
    //SAT_Print("x %i y %i\n",AX,AY);
    SAT_Painter* painter = AContext->painter;
    uint32_t state = 0;
    if (MStates) state = MStates[(AY*256)+AX];
    double r = 0.375;
    double g = 0.375;
    double b = 0.375;

    if (state & 2) r = 0.625;
    if (state & 4) g = 0.625;
    if (state & 8) b = 0.625;
    if (state & 1) {
      r *= 1.2;
      g *= 1.2;
      b *= 1.2;
    }
    painter->setFillColor(SAT_Color(r,g,b));
    painter->fillRect(ARect.x,ARect.y,ARect.w,ARect.h);
  }

  //----------

  uint8_t getCellState(int32_t AX, int32_t AY) {
    uint32_t state = 0;
    if (MStates) state = MStates[(AY*256)+AX];
    return state;
  }

};

//----------------------------------------------------------------------
//
// root widget
//
//----------------------------------------------------------------------

class sa_automata_root_widget
: public SAT_RootWidget {

  friend class sa_automata_plugin;
  
private:

  SAT_DragValueWidget*  WGridWidth  = nullptr;
  SAT_DragValueWidget*  WGridHeight = nullptr;
  sa_automata_grid*     WGrid       = nullptr;

public:

  sa_automata_root_widget(SAT_WidgetListener* AListener=nullptr)
  : SAT_RootWidget(AListener) {
  }

  virtual ~sa_automata_root_widget() {
  }

public:

  void do_widget_update(SAT_Widget* AWidget, uint32_t AIndex, uint32_t AMode) override {
    double value = 0.0;
    SAT_Parameter* param = (SAT_Parameter*)AWidget->getParameter();
    if (AWidget == WGridWidth) {
      double value = WGridWidth->getValue();
      if (param) value = param->denormalize(value);
      WGrid->setNumColumns(value);
      WGrid->do_widget_update(WGrid,0,0);
      WGrid->do_widget_redraw(WGrid,0,0);
    }
    else if (AWidget == WGridHeight) {
      double value = WGridHeight->getValue();
      if (param) value = param->denormalize(value);
      WGrid->setNumRows(value);
      WGrid->do_widget_update(WGrid,0,0);
      WGrid->do_widget_redraw(WGrid,0,0);
    }
    SAT_RootWidget::do_widget_update(AWidget,AIndex,AMode);
  }

};

//----------------------------------------------------------------------
#endif
