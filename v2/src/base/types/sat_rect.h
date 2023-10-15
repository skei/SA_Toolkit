#ifndef sat_rect_included
#define sat_rect_included
//----------------------------------------------------------------------

#include "sat.h"
#include "base/types/sat_point.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

struct SAT_Rect {

//------------------------------
// private:
//------------------------------

  union {
    struct { double x,y,w,h; };
    struct { double left,top,right,bottom; };
    //struct { SAT_Point pos; SAT_Point size; };
    double data[4];
  };

//------------------------------
// public:
//------------------------------

  SAT_Rect() {
    x = 0;
    y = 0;
    w = 0;
    h = 0;
  }

  SAT_Rect(double v) {
    x = v;
    y = v;
    w = v;
    h = v;
  }

  SAT_Rect(double aw, double ah) {
    x = 0;
    y = 0;
    w = aw;
    h = ah;
  }

  SAT_Rect(SAT_Point asize) {
    x = 0;
    y = 0;
    w = asize.w;
    h = asize.h;
  }

  SAT_Rect(double ax, double ay, double aw, double ah) {
    x = ax;
    y = ay;
    w = aw;
    h = ah;
  }

  SAT_Rect(SAT_Point apos, SAT_Point asize) {
    x = apos.x;
    y = apos.y;
    w = asize.w;
    h = asize.h;
  }

//------------------------------
// public:
//------------------------------

  SAT_Point getPos() {
    return SAT_Point(x,y);
  }

  SAT_Point getSize() {
    return SAT_Point(w,h);
  }

//------------------------------

  void setPos(SAT_Point p) {
    x = p.x;
    y = p.y;
  }

  void setPos(double ax, double ay) {
    x = ax;
    y = ay;
  }

  void setSize(SAT_Point s) {
    w = s.w;
    h = s.h;
  }

  void setSize(double aw, double ah) {
    w = aw;
    h = ah;
  }

};

//----------------------------------------------------------------------
#endif