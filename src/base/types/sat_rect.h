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
    x = 0;
    y = 0;
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

  SAT_Rect& operator += (double V) {
    add(V);
    return *this;
  }

  SAT_Rect& operator += (SAT_Point P) {
    add(P);
    return *this;
  }

  SAT_Rect& operator += (SAT_Rect R) {
    add(R);
    return *this;
  }

  //---

  SAT_Rect& operator -= (double V) {
    sub(V);
    return *this;
  }

  SAT_Rect& operator -= (SAT_Point P) {
    sub(P);
    return *this;
  }
  
  SAT_Rect& operator -= (SAT_Rect R) {
    sub(R);
    return *this;
  }

  //---

  SAT_Rect& operator *= (double V) {
    scale(V);
    return *this;
  }

  SAT_Rect& operator *= (SAT_Point P) {
    scale(P);
    return *this;
  }

  SAT_Rect& operator *= (SAT_Rect R) {
    scale(R);
    return *this;
  }

  //---

  SAT_Rect& operator /= (double V) {
    double v;
    v = (1.0 / V);
    scale(v);
    return *this;
  }

  SAT_Rect& operator /= (SAT_Point P) {
    SAT_Point p = P;
    p.x = (1.0 / P.x);
    p.y = (1.0 / P.y);
    scale(p);
    return *this;
  }

  SAT_Rect& operator /= (SAT_Rect R) {
    SAT_Rect r;
    r.x = (1.0 / R.x);
    r.y = (1.0 / R.y);
    r.w = (1.0 / R.w);
    r.h = (1.0 / R.h);
    scale(r);
    return *this;
  }

//  SAT_Rect& operator += (SAT_Point> P) {
//    return *this;
//  }
//
//  SAT_Rect& operator -= (SAT_Point P) {
//    sub(P);
//    return *this;
//  }

//------------------------------
//public:
//------------------------------

  SAT_Point getPos() {
    return SAT_Point(x,y);
  }

  SAT_Point getSize() {
    return SAT_Point(w,h);
  }

//------------------------------
//public:
//------------------------------

  void set(double _v) {
    x = _v;
    y = _v;
    w = _v;
    h = _v;
  }

  void set(double _w, double _h) {
    x = 0;
    y = 0;
    w = _w;
    h = _h;
  }

  void set(double _x, double _y, double _w, double _h) {
    x = _x;
    y = _y;
    w = _w;
    h = _h;
  }

  void setX1(double _x1) { w -= (_x1 - x);   x = _x1; }
  void setY1(double _y1) { h -= (_y1 - y);   y = _y1; }
  void setX2(double _x2) { w += (_x2 - x - w); }
  void setY2(double _y2) { h += (_y2 - y - h); }

  double x1() { return x; }
  double y1() { return y; }
  double x2() { return x + w; }
  double y2() { return y + h; }

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

//------------------------------
//public:
//------------------------------

  void scale(double AScale) {
    x *= AScale;
    y *= AScale;
    w *= AScale;
    h *= AScale;
  }

  void scale(double sx, double sy) {
    x *= sx;
    y *= sy;
    w *= sx;
    h *= sy;
  }

  void scale(double sx, double sy, double sw, double sh) {
    x *= sx;
    y *= sy;
    w *= sw;
    h *= sh;
  }

  void scale(SAT_Point s) {
    x *= s.x;
    y *= s.y;
    w *= s.x;
    h *= s.y;
  }

  void scale(SAT_Rect r) {
    x *= r.x;
    y *= r.y;
    w *= r.w;
    h *= r.h;
  }

  //----------

  void add(double AValue) {
    x += AValue;
    y += AValue;
    w += AValue;
    h += AValue;
  }

  void add(double ax, double ay, double aw, double ah) {
    x += ax;
    y += ay;
    w += aw;
    h += ah;
  }

  void add(SAT_Rect R) {
    x += R.x;
    y += R.y;
    w += R.w;
    h += R.h;
  }

  void addPos(SAT_Point P) {
    x += P.x;
    y += P.y;
  }

  void sub(double AValue) {
    x -= AValue;
    y -= AValue;
    w -= AValue;
    h -= AValue;
  }

  void sub(double ax, double ay, double aw, double ah) {
    x -= ax;
    y -= ay;
    w -= aw;
    h -= ah;
  }

  void sub(SAT_Rect R) {
    x -= R.x;
    y -= R.y;
    w -= R.w;
    h -= R.h;
  }

  void subPos(SAT_Point P) {
    x += P.x;
    y += P.y;
  }

  void grow(double AValue) {
    x -= AValue;
    y -= AValue;
    w += (AValue + AValue);
    h += (AValue + AValue);
  }

  void grow(double ax, double ay, double aw, double ah) {
    x -= ax;
    y -= ay;
    w += (ax + aw);
    h += (ay + ah);
  }

  void grow(SAT_Rect R) {
    x -= R.x;
    y -= R.y;
    w += (R.x + R.w);
    h += (R.y + R.h);
  }

  void grow(SAT_Point P) {
    x -= P.x;
    y -= P.y;
    w += (P.x * 2.0);
    h += (P.y * 2.0);
  }

  void shrink(double AValue) {
    x += AValue;
    y += AValue;
    w -= (AValue + AValue);
    h -= (AValue + AValue);
  }

  void shrink(double ax, double ay, double aw, double ah) {
    x += ax;
    y += ay;
    w -= (ax + aw);
    h -= (ay + ah);
  }

  void shrink(SAT_Rect R) {
    x += R.x;
    y += R.y;
    w -= (R.x + R.w);
    h -= (R.y + R.h);
  }

  void shrink(SAT_Point P) {
    x += P.x;
    y += P.y;
    w -= (P.x * 2.0);
    h -= (P.y * 2.0);
  }

  void addLeft(double ASize)    { x -= ASize; w += ASize; }
  void addRight(double ASize)   { w += ASize; }
  void addTop(double ASize)     { y -= ASize; h += ASize; }
  void addBottom(double ASize)  { h += ASize; }

  void removeLeft(double ASize)    { x += ASize; w -= ASize; }
  void removeRight(double ASize)   { w -= ASize; }
  void removeTop(double ASize)     { y += ASize; h -= ASize; }
  void removeBottom(double ASize)  { h -= ASize; }

//------------------------------
//public:
//------------------------------

  bool isEmpty() {
    if (w <= 0.0f) return true;
    if (h <= 0.0f) return true;
    return false;
  }

  //----------

  bool isNotEmpty() {
    if ((w > 0.0f) && (h > 0.0f)) return true;
    return false;
  }

  //----------

  // returns true if any of the edges intersects

  bool intersects(SAT_Rect R) {
    if (R.x1() > x2() ) return false; // too far right
    if (R.y1() > y2() ) return false; // too far down
    if (R.x2() < x1() ) return false; // too far left
    if (R.y2() < y1() ) return false; // too far up
    return true;
  }

  //----------

  // returns true if APoint is inside
  // if widget.contains(mousex,mousey)

  bool contains(double xpos, double ypos) {
    if (xpos < x   ) return false;
    if (xpos > x2()) return false;
    if (ypos < y   ) return false;
    if (ypos > y2()) return false;
    return true;
  }

//------------------------------
//public:
//------------------------------

  /*
   _____
  |   _ |__
  |  |  |  |
  |__|__|  |
     |_____|


         _____
      _ |__   |
     |  |  |  |
     |  |__|__|
     |_____|


  */

  //----------

  // intersection
  // largest x
  // smallest x2

  void overlap(SAT_Rect R) {

    //if (intersects(R)) {
      if ( R.x1() > x1() ) setX1( R.x1() );
      if ( R.x2() < x2() ) setX2( R.x2() );
      if ( R.y1() > y1() ) setY1( R.y1() );
      if ( R.y2() < y2() ) setY2( R.y2() );
    //}
    //else {
    //  R.w = 0;
    //  R.h = 0;
    //}
    //}
  }

//  void overlap(SAT_Rect R) {
//    if ( R.x1() > x1() ) setX1( R.x1() );
//    if ( R.y1() > y1() ) setY1( R.y1() );
//    if ( R.x2() < x2() ) setX2( R.x2() );
//    if ( R.y2() < y2() ) setX2( R.y2() );
//  }

  //----------

  // union / combine
  // smallest x
  // largest x2

  //----------

  //----------

  // combine rects
  // (so that both of them fits inside)
  // parent.combine(child)

  void combine(SAT_Rect R) {
    //printf("combine %.0f,%.0f,%.0f,%.0f R %.0f,%.0f,%.0f,%.0f -> ",x,y,w,h,R.x,R.y,R.w,R.h);
    if ( R.x1() < x1() ) setX1( R.x1() );
    if ( R.x2() > x2() ) setX2( R.x2() );
    if ( R.y1() < y1() ) setY1( R.y1() );
    if ( R.y2() > y2() ) setY2( R.y2() );
    //printf("%.0f,%.0f,%.0f,%.0f\n",x,y,w,h);
  }

  //----------

  // remove ARect (so that it lies outside)

  //void remove(SAT_Rect ARect) {
  //}

};

//----------------------------------------------------------------------
#endif