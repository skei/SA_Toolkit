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

template <typename T>
class SAT_Rect {

//------------------------------
public:
//------------------------------

  union {
    struct { T x,y,w,h; };
    struct { T left,top,right,bottom; };
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

  SAT_Rect(T _v) {
    x = 0;
    y = 0;
    w = _v;
    h = _v;
  }

  SAT_Rect(T _w, T _h) {
    x = 0;
    y = 0;
    w = _w;
    h = _h;
  }

  SAT_Rect(T _x, T _y, T _w, T _h) {
    x = _x;
    y = _y;
    w = _w;
    h = _h;
  }

  SAT_Rect(SAT_Point<T> APos) {}
  SAT_Rect(SAT_Point<T> APos, SAT_Point<T> ASize) {}

//------------------------------
public:
//------------------------------

  SAT_Rect<T>& operator += (SAT_Rect<T> R) {
    add(R);
    return *this;
  }

  SAT_Rect<T>& operator -= (SAT_Rect<T> R) {
    sub(R);
    return *this;
  }

//  SAT_Rect<T>& operator += (SAT_Point<T> P) {
//    add(P);
//    return *this;
//  }
//
//  SAT_Rect<T>& operator -= (SAT_Point<T> P) {
//    sub(P);
//    return *this;
//  }

//------------------------------
public:
//------------------------------

  void setX1(T _x1) { w -= (_x1 - x);   x = _x1; }
  void setY1(T _y1) { h -= (_y1 - y);   y = _y1; }
  void setX2(T _x2) { w += (_x2 - x - w); }
  void setY2(T _y2) { h += (_y2 - y - h); }

  T x1() { return x; }
  T y1() { return y; }
  T x2() { return x + w; }
  T y2() { return y + h; }

  SAT_Point<T> getPos()   { return SAT_Point<T>(x,y); }
  SAT_Point<T> getSize()  { return SAT_Point<T>(w,h); }

  //----------

  void setPos(SAT_Point<T> APos) {
    x = APos.x;
    y = APos.y;
  }

  //----------

  void setPos(T AXpos, T AYpos) {
    x = AXpos;
    y = AYpos;
  }

  //----------

  void setSize(SAT_Point<T> ASize) {
    w = ASize.w;
    h = ASize.h;
  }

  //----------

  void setSize(T AWidth, T AHeight) {
    w = AWidth;
    h = AHeight;
  }

  //----------

  void scale(T AScale) {
    x *= AScale;
    y *= AScale;
    w *= AScale;
    h *= AScale;
  }

  void scale(T sx, T sy) {
    x *= sx;
    y *= sy;
    w *= sx;
    h *= sy;
  }

  void scale(SAT_Point<T> s) {
    x *= s.x;
    y *= s.y;
    w *= s.x;
    h *= s.y;
  }

  void scale(T sx, T sy, T sw, T sh) {
    x *= sx;
    y *= sy;
    w *= sw;
    h *= sh;
  }


  //----------

  void add(T AValue) {
    x += AValue;
    y += AValue;
    w += AValue;
    h += AValue;
  }

  void add(T ax, T ay, T aw, T ah) {
    x += ax;
    y += ay;
    w += aw;
    h += ah;
  }

  void add(SAT_Rect<T> R) {
    x += R.x;
    y += R.y;
    w += R.w;
    h += R.h;
  }

  void addPos(SAT_Point<T> P) {
    x += P.x;
    y += P.y;
  }

  void sub(T AValue) {
    x -= AValue;
    y -= AValue;
    w -= AValue;
    h -= AValue;
  }

  void sub(T ax, T ay, T aw, T ah) {
    x -= ax;
    y -= ay;
    w -= aw;
    h -= ah;
  }

  void sub(SAT_Rect<T> R) {
    x -= R.x;
    y -= R.y;
    w -= R.w;
    h -= R.h;
  }

  void subPos(SAT_Point<T> P) {
    x += P.x;
    y += P.y;
  }

  void grow(T AValue) {
    x -= AValue;
    y -= AValue;
    w += (AValue + AValue);
    h += (AValue + AValue);
  }

  void grow(T ax, T ay, T aw, T ah) {
    x -= ax;
    y -= ay;
    w += (ax + aw);
    h += (ay + ah);
  }

  void grow(SAT_Rect<T> R) {
    x -= R.x;
    y -= R.y;
    w += (R.x + R.w);
    h += (R.y + R.h);
  }

  void grow(SAT_Point<T> P) {
    x -= P.x;
    y -= P.y;
    w += (P.x * 2.0);
    h += (P.y * 2.0);
  }

  void shrink(T AValue) {
    x += AValue;
    y += AValue;
    w -= (AValue + AValue);
    h -= (AValue + AValue);
  }

  void shrink(T ax, T ay, T aw, T ah) {
    x += ax;
    y += ay;
    w -= (ax + aw);
    h -= (ay + ah);
  }

  void shrink(SAT_Rect<T> R) {
    x += R.x;
    y += R.y;
    w -= (R.x + R.w);
    h -= (R.y + R.h);
  }

  void shrink(SAT_Point<T> P) {
    x += P.x;
    y += P.y;
    w -= (P.x * 2.0);
    h -= (P.y * 2.0);
  }

  void addLeft(T ASize)    { x -= ASize; w += ASize; }
  void addRight(T ASize)   { w += ASize; }
  void addTop(T ASize)     { y -= ASize; h += ASize; }
  void addBottom(T ASize)  { h += ASize; }

  void removeLeft(T ASize)    { x += ASize; w -= ASize; }
  void removeRight(T ASize)   { w -= ASize; }
  void removeTop(T ASize)     { y += ASize; h -= ASize; }
  void removeBottom(T ASize)  { h -= ASize; }

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
  bool inside(SAT_Rect<T> ARect) {
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

  //bool touches(SAT_Rect<T> ARect) {
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

  bool intersects(SAT_Rect<T> R) {
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

  bool contains(T xpos, T ypos) {
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

  void overlap(SAT_Rect<T> R) {

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

//  void overlap(SAT_Rect<T> R) {
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

  void combine(SAT_Rect<T> R) {
    //printf("combine %.0f,%.0f,%.0f,%.0f R %.0f,%.0f,%.0f,%.0f -> ",x,y,w,h,R.x,R.y,R.w,R.h);
    if ( R.x1() < x1() ) setX1( R.x1() );
    if ( R.x2() > x2() ) setX2( R.x2() );
    if ( R.y1() < y1() ) setY1( R.y1() );
    if ( R.y2() > y2() ) setY2( R.y2() );
    //printf("%.0f,%.0f,%.0f,%.0f\n",x,y,w,h);
  }

  //----------

  // remove ARect (so that it lies outside)

  void remove(SAT_Rect<T> ARect)   {
  }


//  // largest x
//  // smallest x2
//
//  void intersection(SAT_Rect<T> ARect) {
//    if (ARect.x2()  > x2()) x2(ARect.x2());
//  }


  //----------

//  void shrink(SAT_Rect<T> ARect) {
//  }

//  void grow(SAT_Rect<T> ARect) {
//  }

  //----------

  //----------


};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

typedef SAT_Rect<float>    SAT_FRect;
typedef SAT_Rect<int32_t>  SAT_IRect;
typedef SAT_Rect<double>   SAT_DRect;

//----------------------------------------------------------------------
#endif
