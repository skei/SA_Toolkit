#ifndef sat_point_included
#define sat_point_included
//----------------------------------------------------------------------

template <typename T>
class SAT_Point {

public:

  union {
    struct { T x,y; };
    struct { T w,h; };
    struct { T horiz,vert; };
  };

public:

  SAT_Point() {
    x = 0;
    y = 0;
  }

  SAT_Point(T v) {
    x = v;
    y = v;
  }

  SAT_Point(T x_, T y_) {
    x = x_;
    y = y_;
  }

  //SAT_Point(SAT_Point<T> APoint) {}

public:

//------------------------------
public:
//------------------------------

  SAT_Point<T>& operator += (SAT_Point<T> P) {
    add(P);
    return *this;
  }

  SAT_Point<T>& operator -= (SAT_Point<T> P) {
    sub(P);
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

  void scale(T AScale) {
    x *= AScale;
    y *= AScale;
  }

  void scale(T sx, T sy) {
    x *= sx;
    y *= sy;
  }

  void scale(SAT_Point<T> s) {
    x *= s.x;
    y *= s.y;
  }

  //----------

  void add(T AValue) {
    x += AValue;
    y += AValue;
  }

  void add(T ax, T ay) {
    x += ax;
    y += ay;
  }

  void sub(T AValue) {
    x -= AValue;
    y -= AValue;
  }

  void sub(T ax, T ay) {
    x -= ax;
    y -= ay;
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------


typedef SAT_Point<float>    SAT_FPoint;
typedef SAT_Point<double>   SAT_DPoint;
typedef SAT_Point<int32_t>  SAT_IPoint;

//----------------------------------------------------------------------
#endif
