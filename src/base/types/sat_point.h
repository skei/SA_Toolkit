#ifndef sat_point_included
#define sat_point_included
//----------------------------------------------------------------------

#include "sat.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

struct SAT_Point {

//------------------------------
// private:
//------------------------------

  union {
    struct { double x,y; };
    struct { double w,h; };
  };

//------------------------------
// public:
//------------------------------

  SAT_Point() {
    x = 0;
    y = 0;
  }

  SAT_Point(double v) {
    x = v;
    y = v;
  }

  SAT_Point(double ax, double ay) {
    x = ax;
    y = ay;
  }

  SAT_Point(const SAT_Point& p) {
    x = p.x;
    y = p.y;
  }

//------------------------------
// public:
//------------------------------

};

//----------------------------------------------------------------------
#endif