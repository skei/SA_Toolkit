#ifndef sat_button_grid_included
#define sat_button_grid_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/widget/sat_grid_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_ButtonGridWidget
: public SAT_GridWidget {

//------------------------------
public:
//------------------------------

  uint32_t*     MStates             = nullptr;
  SAT_Color     MOnBackgroundColor  = SAT_Red;
  SAT_Color     MOffBackgroundColor = 0.45;
  SAT_Color     MOnTextColor        = SAT_White;
  SAT_Color     MOffTextColor       = SAT_Black;
  const char**  MButtonTexts        = nullptr;
  double        MTextSize           = 12.0;

//------------------------------
public:
//------------------------------

  SAT_ButtonGridWidget(SAT_Rect ARect, uint32_t AColumns, uint32_t ARows)
  : SAT_GridWidget(ARect,AColumns,ARows) {
    setName("SAT_ButtonGridWidget");
    setCursor(SAT_CURSOR_FINGER);
    //setNumCells(AColumns,ARows);
    uint32_t size = AColumns * ARows * sizeof(uint32_t);
    MStates = (uint32_t*)malloc(size);
    memset(MStates,0,size);
  }

  //----------

  virtual ~SAT_ButtonGridWidget() {
    if (MStates) free(MStates);
  }

//------------------------------
public:
//------------------------------

  virtual void setButtonTexts(const char** ATexts) { MButtonTexts = ATexts; }

//------------------------------
public:
//------------------------------

  // void on_initCell(SAT_Rect ARect, int32_t AX, int32_t AY) override {
  //   SAT_PRINT("x %i y %i\n",AX,AY);
  // }

  //----------

  void on_clickCell(int32_t AX, int32_t AY, int32_t AB, int32_t AS) override {
    //SAT_PRINT("x %i y %i b %i s %i\n",AX,AY,AB,AS);
    uint32_t index = (AY * MNumColumns) + AX;
    if (MStates[index] == 0) {
      MStates[index] = 1;
    }
    else {
      MStates[index] = 0;
    }
  }

  //----------

  // void on_releaseCell(int32_t AX, int32_t AY, int32_t AB, int32_t AS) override {
  //   SAT_PRINT("x %i y %i b %i s %i\n",AX,AY,AB,AS);
  // }

  //----------

  // void on_dragCell(int32_t AX, int32_t AY, int32_t AB, int32_t AS) override {
  //   SAT_PRINT("x %i y %i b %i s %i\n",AX,AY,AB,AS);
  // }

  //----------

  void on_paintCell(SAT_PaintContext* AContext, SAT_Rect ARect, int32_t AX, int32_t AY) override {
    //SAT_PRINT("x %i y %i\n",AX,AY);
    uint32_t index = (AY * MNumColumns) + AX;
    SAT_Painter* painter = AContext->painter;
    double scale = getWindowScale();
    const char* txt = MButtonTexts[index];
    if (MStates[index] == 0) painter->setFillColor(MOffBackgroundColor);
    else painter->setFillColor(MOnBackgroundColor);
    painter->fillRect(ARect.x,ARect.y,ARect.w,ARect.h);
    if (MStates[index] == 0) painter->setTextColor(MOffTextColor);
    else painter->setTextColor(MOnTextColor);
    painter->setTextSize(MTextSize * scale);
    painter->drawTextBox(ARect,txt,SAT_TEXT_ALIGN_CENTER);
  }

  //----------

  // void on_selectCells(int32_t AX, int32_t AY, int32_t AW, int32_t AH) override {
  //   SAT_PRINT("x %i y %i w %i h %i\n",AX,AY,AW,AH);
  // }

  //----------


};

//----------------------------------------------------------------------
#endif