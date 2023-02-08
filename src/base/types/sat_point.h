#ifndef sat_point_included
#define sat_point_included
//----------------------------------------------------------------------

class SAT_Point {

public:

  union {
    struct { double x,y; };
    struct { double w,h; };
    struct { double horiz,vert; };
  };

public:

  SAT_Point() {
    x = 0;
    y = 0;
  }

  SAT_Point(double v) {
    x = v;
    y = v;
  }

  SAT_Point(double x_, double y_) {
    x = x_;
    y = y_;
  }

  //SAT_Point(SAT_Point APoint) {}

public:

//------------------------------
public:
//------------------------------

  SAT_Point& operator += (SAT_Point P) {
    add(P);
    return *this;
  }

  SAT_Point& operator -= (SAT_Point P) {
    sub(P);
    return *this;
  }

//  SAT_Rect& operator += (SAT_Point P) {
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

  void add(SAT_Point APoint) {
    x += APoint.x;
    y += APoint.y;
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

  void sub(SAT_Point APoint) {
    x -= APoint.x;
    y -= APoint.y;
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------



//----------------------------------------------------------------------
#endif
