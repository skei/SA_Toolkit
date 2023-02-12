#ifndef sat_paint_style_included
#define sat_paint_style_included
//----------------------------------------------------------------------

#include "base/sat.h"


//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_PaintStyle {

//------------------------------
public:
//------------------------------

  SAT_PaintStyle() {
  }

  //----------

  virtual ~SAT_PaintStyle() {
  }

//------------------------------
public:
//------------------------------

  virtual SAT_Color getBackgroundColor()  { return SAT_Grey; }
  virtual SAT_Color getBorderColor()      { return SAT_DarkGrey; }
  virtual SAT_Color getTextColor()        { return SAT_LightGrey; }

};

//----------------------------------------------------------------------
#endif
