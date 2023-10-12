#ifndef sat_rect_included
#define sat_rect_included
//----------------------------------------------------------------------

#include "sat.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

struct SAT_Point {
  union {
    struct { double x,y; };
    struct { double w,h; };
  };
};

//----------

struct SAT_Rect {
  union {
    struct { double x,y,w,h; };
    struct { double left,top,right,bottom; };
    struct { SAT_Point pos; SAT_Point size; };
    double data[4];
  };
};

//----------------------------------------------------------------------
#endif