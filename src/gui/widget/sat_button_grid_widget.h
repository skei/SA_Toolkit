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
  const char**  MTexts              = nullptr;

  SAT_Color     MOnBackgroundColor  = SAT_Red;
  SAT_Color     MOffBackgroundColor = 0.45;
  SAT_Color     MOnTextColor        = SAT_White;
  SAT_Color     MOffTextColor       = SAT_Black;
  double        MTextSize           = 12.0;
  bool          MHorizSingle        = false;
  bool          MVertSingle         = false;

  int32_t       MLastClickedX       = 0;
  int32_t       MLastClickedY       = 0;


//------------------------------
public:
//------------------------------

  SAT_ButtonGridWidget(SAT_Rect ARect, uint32_t AColumns, uint32_t ARows, const char** ATexts=nullptr)
  : SAT_GridWidget(ARect,AColumns,ARows) {
    setName("SAT_ButtonGridWidget");
    setCursor(SAT_CURSOR_FINGER);
    //setNumCells(AColumns,ARows);
    uint32_t size = AColumns * ARows * sizeof(uint32_t);
    MStates = (uint32_t*)malloc(size);
    memset(MStates,0,size);
    size = AColumns * ARows * sizeof(const char*);
    MTexts = (const char**)malloc(size);
    if (ATexts) memcpy(MTexts,ATexts,size);
  }

  //----------

  virtual ~SAT_ButtonGridWidget() {
    if (MStates) free(MStates);
    if (MTexts) free(MTexts);
  }

//------------------------------
public:
//------------------------------

  virtual void setButtonText(const char* AText, int32_t AX, int32_t AY) {
    uint32_t index = (AY * MNumColumns) + AX;
    MTexts[index] = AText;
  }

  //----------

  virtual void setButtonTexts(const char** ATexts) {
    MTexts = ATexts;
  }

  //----------

  virtual void setSingle(bool AHoriz, bool AVert) {
    MHorizSingle = AHoriz;
    MVertSingle = AVert;
  }

  //----------

  virtual void resize(int32_t AX, int32_t AY) {
    uint32_t prev_size = MNumColumns * MNumRows;
    uint32_t new_size  = AX * AY;
    uint32_t copy_size = prev_size;
    if (new_size < prev_size) copy_size = new_size;
    uint32_t* states = (uint32_t*)malloc(new_size * sizeof(uint32_t));
    memset(states,0,new_size * sizeof(uint32_t));
    if (MStates) {
      memcpy(states,MStates,copy_size * sizeof(uint32_t));
      free(MStates);
    }
    MStates = states;
    const char** texts = (const char**)malloc(new_size * sizeof(const char*));
    memset(texts,0,new_size * sizeof(const char*));
    if (MTexts) {
      memcpy(texts,MTexts,copy_size * sizeof(const char*));
      free(MTexts);
    }
    MTexts = texts;
    setNumColumns(AX);
    setNumRows(AY);
  }

  // ----------

  int32_t getClickedColumn()  { return MLastClickedX; }
  int32_t getClickedRow()     { return MLastClickedY; }

  void selectCell(int32_t AX, int32_t AY) {
    MLastClickedX = AX;
    MLastClickedY = AY;
    uint32_t index = (AY * MNumColumns) + AX;
    if (MStates[index] == 0) {
      MStates[index] = 1;
    }
    else {
      MStates[index] = 0;
    }
    index = 0;
    for (int32_t y=0; y<MNumRows; y++) {
      for (int32_t x=0; x<MNumColumns; x++) {
        index = (y * MNumColumns) + x;
        if ((x != AX) && (y == AY) && MHorizSingle) MStates[index] = 0;
        if ((x == AX) && (y != AY) && MVertSingle) MStates[index] = 0;
      }
    }
    do_widget_update(this,0);
  }

//------------------------------
public:
//------------------------------

  // void on_initCell(SAT_Rect ARect, int32_t AX, int32_t AY) override {
  //   SAT_PRINT("x %i y %i\n",AX,AY);
  // }

  //----------

  void on_clickCell(int32_t AX, int32_t AY, int32_t AB, int32_t AS) override {
    //SAT_PRINT("x %i y %i b %i s %i\n",AX,AY,AB,AS);
    selectCell(AX,AY);
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

    if (!MStates) return;

    SAT_Painter* painter = AContext->painter;
    double scale = getWindowScale();
    const char* txt = "";
    int32_t index = (AY * MNumColumns) + AX;
    if (MStates[index] == 0) painter->setFillColor(MOffBackgroundColor);
    else painter->setFillColor(MOnBackgroundColor);
    painter->fillRect(ARect.x,ARect.y,ARect.w,ARect.h);
    txt = MTexts[index];
    if (txt) {
      if (MStates[index] == 0) painter->setTextColor(MOffTextColor);
      else painter->setTextColor(MOnTextColor);
      painter->setTextSize(MTextSize * scale);
      painter->drawTextBox(ARect,txt,SAT_TEXT_ALIGN_CENTER);
    }
  }

  //----------

  // void on_selectCells(int32_t AX, int32_t AY, int32_t AW, int32_t AH) override {
  //   SAT_PRINT("x %i y %i w %i h %i\n",AX,AY,AW,AH);
  // }

  //----------


};

//----------------------------------------------------------------------
#endif