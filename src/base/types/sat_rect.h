#ifndef sat_rect_included
#define sat_rect_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "base/types/sat_point.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Rect {

//------------------------------
public:
//------------------------------

  union {
    struct { double x,y,w,h; };
    struct { double left,top,right,bottom; };
  };

//------------------------------
public:
//------------------------------

  SAT_Rect() {
    x = 0;
    y = 0;
    w = 0;
    h = 0;
  }

  SAT_Rect(double _v) {
    x = 0;
    y = 0;
    w = _v;
    h = _v;
  }

  SAT_Rect(double _w, double _h) {
    x = 0;
    y = 0;
    w = _w;
    h = _h;
  }

  SAT_Rect(double _x, double _y, double _w, double _h) {
    x = _x;
    y = _y;
    w = _w;
    h = _h;
  }

  SAT_Rect(SAT_Point APos) {}
  SAT_Rect(SAT_Point APos, SAT_Point ASize) {}

//------------------------------
public:
//------------------------------

  SAT_Rect& operator += (SAT_Rect R) {
    add(R);
    return *this;
  }

  SAT_Rect& operator -= (SAT_Rect R) {
    sub(R);
    return *this;
  }

//  SAT_Rect& operator += (SAT_Point> P) {
//    add(P);
//    return *this;
//  }
//
//  SAT_Rect& operator -= (SAT_Point P) {
//    sub(P);
//    return *this;
//  }

//------------------------------
public:
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

  SAT_Point getPos()   { return SAT_Point(x,y); }
  SAT_Point getSize()  { return SAT_Point(w,h); }

  //----------

  void setPos(SAT_Point APos) {
    x = APos.x;
    y = APos.y;
  }

  //----------

  void setPos(double AXpos, double AYpos) {
    x = AXpos;
    y = AYpos;
  }

  //----------

  void setSize(SAT_Point ASize) {
    w = ASize.w;
    h = ASize.h;
  }

  //----------

  void setSize(double AWidth, double AHeight) {
    w = AWidth;
    h = AHeight;
  }

  //----------

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

  void scale(SAT_Point s) {
    x *= s.x;
    y *= s.y;
    w *= s.x;
    h *= s.y;
  }

  void scale(double sx, double sy, double sw, double sh) {
    x *= sx;
    y *= sy;
    w *= sw;
    h *= sh;
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
public: // tests
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

  // returns true if ARect is completely inside this
  // (no clipping needed, opacity)
  // if widget.inside(cliprect)

  /*
  bool inside(SAT_Rect ARect) {
    if (ARect.x     < x   ) return false;
    if (ARect.x2()  > x2()) return false;
    if (ARect.y     < y   ) return false;
    if (ARect.y2()  > y2()) return false;
    return true;
  }
  */

  //----------

  // touches (intersects)
  // false:
  // x > r.x2
  // x2 < r.x

  // if widget.intersect(updaterect)
  // ax2 < x
  // ax > x2

  //bool touches(SAT_Rect ARect) {
  //  if (ARect.x     > x2()) return false;
  //  if (ARect.x2()  < x   ) return false;
  //  if (ARect.y     > y2()) return false;
  //  if (ARect.y2()  < y   ) return false;
  //  return true;
  //}
  //
  ////----------
  //

  // returns true if any of the edges intersects

  bool intersects(SAT_Rect R) {
    if (R.x1() > x2() ) return false; // too far right
    if (R.y1() > y2() ) return false; // too far down
    if (R.x2() < x1() ) return false; // too far left
    if (R.y2() < y1() ) return false; // too far up
    return true;
  }

  //----------

  // is_fully_inside
  // false
  // x < r.x
  // x2 > r.x2

  //----------

  // fully_contains
  // false
  // x > ax
  // x2 < ax2

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
public: // change
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

  void remove(SAT_Rect ARect)   {
  }


//  // largest x
//  // smallest x2
//
//  void intersection(SAT_Rect ARect) {
//    if (ARect.x2()  > x2()) x2(ARect.x2());
//  }


  //----------

//  void shrink(SAT_Rect ARect) {
//  }

//  void grow(SAT_Rect ARect) {
//  }

  //----------

  //----------


};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------


//----------------------------------------------------------------------
#endif
