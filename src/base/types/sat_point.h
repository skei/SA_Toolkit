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
    struct { double horiz,vert; };
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
public:
//------------------------------

  SAT_Point& operator += (double P) {
    add(P);
    return *this;
  }

  SAT_Point& operator -= (double P) {
    sub(P);
    return *this;
  }

//  MIP_Rect& operator += (double P) {
//    add(P);
//    return *this;
//  }
//
//  MIP_Rect& operator -= (double P) {
//    sub(P);
//    return *this;
//  }

//------------------------------
public:
//------------------------------

  void scale(double AScale) {
    x *= AScale;
    y *= AScale;
  }

  void scale(double sx, double sy) {
    x *= sx;
    y *= sy;
  }

  void scale(SAT_Point s) {
    x *= s.x;
    y *= s.y;
  }

  //----------

  void add(double AValue) {
    x += AValue;
    y += AValue;
  }

  void add(double ax, double ay) {
    x += ax;
    y += ay;
  }

  void sub(double AValue) {
    x -= AValue;
    y -= AValue;
  }

  void sub(double ax, double ay) {
    x -= ax;
    y -= ay;
  }

};

//----------------------------------------------------------------------
#endif
