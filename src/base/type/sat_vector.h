#ifndef sat_vector_included
#define sat_vector_included
//----------------------------------------------------------------------

#include "sat.h"
#include "base/util/sat_math.h"

#include <math.h>

//----------------------------------------------------------------------
// SAT_Vector2
//----------------------------------------------------------------------

struct SAT_Vector2 {

  union {
    float el[2];
    struct { float x,y; };
  };

  //----------

  SAT_Vector2()                    {}
  SAT_Vector2(float v)             { x=v; y=v; }
  SAT_Vector2(float ax, float ay)  { x=ax; y=ay; }

  SAT_Vector2  operator +  (const SAT_Vector2& rhs)  const { return SAT_Vector2(x+rhs.x,y+rhs.y); }
  SAT_Vector2  operator -  (const SAT_Vector2& rhs)  const { return SAT_Vector2(x-rhs.x,y-rhs.y); }
  SAT_Vector2  operator *  (const SAT_Vector2& rhs)  const { return SAT_Vector2(x*rhs.x,y*rhs.y); }
  SAT_Vector2  operator /  (const SAT_Vector2& rhs)  const { return SAT_Vector2(x/rhs.x,y/rhs.y); }
  SAT_Vector2& operator += (const SAT_Vector2& v)          { x+=v.x; y+=v.y; return *this; }
  SAT_Vector2& operator -= (const SAT_Vector2& v)          { x-=v.x; y-=v.y; return *this; }
  SAT_Vector2& operator *= (const SAT_Vector2& v)          { x*=v.x; y*=v.y; return *this; }
  SAT_Vector2& operator /= (const SAT_Vector2& v)          { x/=v.x; y/=v.y; return *this; }
  SAT_Vector2  operator +  (float s)          const { return SAT_Vector2(x+s,y+s); }
  SAT_Vector2  operator -  (float s)          const { return SAT_Vector2(x-s,y-s); }
  SAT_Vector2  operator *  (float s)          const { return SAT_Vector2(x*s,y*s); }
  SAT_Vector2  operator /  (float s)          const { return SAT_Vector2(x/s,y/s); }
  SAT_Vector2& operator += (const float& s)         { x+=s; y+=s; return *this; }
  SAT_Vector2& operator -= (const float& s)         { x-=s; y-=s; return *this; }
  SAT_Vector2& operator *= (const float& s)         { x*=s; y*=s; return *this; }
  SAT_Vector2& operator /= (const float& s)         { x/=s; y/=s; return *this; }
  SAT_Vector2  operator -  ()                 const { return SAT_Vector2(-x,-y); }

  float dot(SAT_Vector2 v1) {
    return x * v1.x + y * v1.y;
  }

  SAT_Vector2 cross(SAT_Vector2 v1) {
    //TODO
    return SAT_Vector2(
      v1.x,
      v1.y
    );
  }

  void cross(SAT_Vector2 v0, SAT_Vector2 v1) {
    //TODO
    x = v0.x + v1.x;
    y = v0.y + v1.y;
  }

  float length(void) {
    return sqrtf(x*x + y*y);
  }

  float length_sq(void) {
    return (x*x + y*y);
  }

  void normalize(void) {
    float il = 1.0f / length();
    x *= il;
    y *= il;
  }

  void rotate(float AAngle) {
    float sa = sin(AAngle);
    float ca = cos(AAngle);
    x = (x * ca) - (y * sa);
    y = (x * sa) + (y * ca);
  }

};

//----------------------------------------------------------------------

SAT_Vector2  SAT_Vector2_add(SAT_Vector2 v0, SAT_Vector2 v1)  { SAT_Vector2 r = { v0.x + v1.x, v0.y + v1.y }; return r; }
SAT_Vector2  SAT_Vector2_sub(SAT_Vector2 v0, SAT_Vector2 v1)  { SAT_Vector2 r = { v0.x - v1.x, v0.y - v1.y }; return r; }
SAT_Vector2  SAT_Vector2_mul(SAT_Vector2 v0, SAT_Vector2 v1)  { SAT_Vector2 r = { v0.x * v1.x, v0.y * v1.y }; return r; }
SAT_Vector2  SAT_Vector2_div(SAT_Vector2 v0, SAT_Vector2 v1)  { SAT_Vector2 r = { v0.x / v1.x, v0.y / v1.y }; return r; }

SAT_Vector2  SAT_Vector2_add(SAT_Vector2 v, float f)   { SAT_Vector2 r = { v.x + f, v.y + f }; return r; }
SAT_Vector2  SAT_Vector2_sub(SAT_Vector2 v, float f)   { SAT_Vector2 r = { v.x - f, v.y - f }; return r; }
SAT_Vector2  SAT_Vector2_mul(SAT_Vector2 v, float f)   { SAT_Vector2 r = { v.x * f, v.y * f }; return r; }
SAT_Vector2  SAT_Vector2_div(SAT_Vector2 v, float f)   { SAT_Vector2 r = { v.x / f, v.y / f }; return r; }

float SAT_Vector2_dot(SAT_Vector2 v0, SAT_Vector2 v1) {
  return (v0.x * v1.x)
       + (v0.y * v1.y);
}

SAT_Vector2 SAT_Vector2_cross(SAT_Vector2 v0, SAT_Vector2 v1) {
  //TODO
  return SAT_Vector2(
    v0.x + v1.x,
    v0.y + v1.y
  );
} // USUALLY FAIL

float SAT_Vector2_length(SAT_Vector2 v) {
  return sqrtf( (v.x*v.x) + (v.y*v.y) );
}

float SAT_Vector2_length_sq(SAT_Vector2 v) {
  return ( (v.x*v.x) + (v.y*v.y) );
}

SAT_Vector2 SAT_Vector2_normalize(SAT_Vector2 v) {
  float fi = 1.0f / SAT_Vector2_length(v);
  return SAT_Vector2(
    v.x * fi,
    v.y * fi
  );
}

//----------------------------------------------------------------------
// SAT_Vector3
//----------------------------------------------------------------------

struct SAT_Vector3 {

  union {
    float el[3];
    struct { float x,y,z; };
    struct { float r,g,b; };
    SAT_Vector2 xy;
  };

  //----------

  SAT_Vector3()                              { x=0.0f; y=0.0f; z=0.0f; }
  SAT_Vector3(float v)                       { x=v; y=v; z=v; }
  SAT_Vector3(float ax, float ay, float az)  { x=ax; y=ay; z=az; }
  //SAT_Vector3(SAT_Vector2 axy, float az, float aw)            { x=axy.x; y=axy.y; z=az; w=aw; }
  //SAT_Vector3(SAT_Vector3 axyz, float aw)                     { x=axyz.x; y=axyz.y; z=axyz.z; w=aw; }

  SAT_Vector3  operator +  (const SAT_Vector3& rhs) const { return SAT_Vector3(x+rhs.x,y+rhs.y,z+rhs.z); }
  SAT_Vector3  operator -  (const SAT_Vector3& rhs) const { return SAT_Vector3(x-rhs.x,y-rhs.y,z-rhs.z); }
  SAT_Vector3  operator *  (const SAT_Vector3& rhs) const { return SAT_Vector3(x*rhs.x,y*rhs.y,z*rhs.z); }
  SAT_Vector3  operator /  (const SAT_Vector3& rhs) const { return SAT_Vector3(x/rhs.x,y/rhs.y,z/rhs.z); }
  SAT_Vector3& operator += (const SAT_Vector3& v)         { x+=v.x; y+=v.y; z+=v.z; return *this; }
  SAT_Vector3& operator -= (const SAT_Vector3& v)         { x-=v.x; y-=v.y; z-=v.z; return *this; }
  SAT_Vector3& operator *= (const SAT_Vector3& v)         { x*=v.x; y*=v.y; z*=v.z; return *this; }
  SAT_Vector3& operator /= (const SAT_Vector3& v)         { x/=v.x; y/=v.y; z/=v.z; return *this; }
  SAT_Vector3  operator +  (float s)           const { return SAT_Vector3(x+s,y+s,z+s); }
  SAT_Vector3  operator -  (float s)           const { return SAT_Vector3(x-s,y-s,z-s); }
  SAT_Vector3  operator *  (float s)           const { return SAT_Vector3(x*s,y*s,z*s); }
  SAT_Vector3  operator /  (float s)           const { return SAT_Vector3(x/s,y/s,z/s); }
  SAT_Vector3& operator += (const float& s)          { x+=s; y+=s; z+=s; return *this; }
  SAT_Vector3& operator -= (const float& s)          { x-=s; y-=s; z-=s; return *this; }
  SAT_Vector3& operator *= (const float& s)          { x*=s; y*=s; z*=s; return *this; }
  SAT_Vector3& operator /= (const float& s)          { x/=s; y/=s; z/=s; return *this; }
  SAT_Vector3  operator -  ()                  const { return SAT_Vector3(-x,-y,-z); }

  float dot(SAT_Vector3 v1) {
    return (x * v1.x) + (y * v1.y) + (z * v1.z);
  }

  SAT_Vector3 cross(SAT_Vector3 v1) {
    return SAT_Vector3(
      (y * v1.z) - (z * v1.y),
      (z * v1.x) - (x * v1.z),
      (x * v1.y) - (y * v1.x)
    );
  }

  void cross(SAT_Vector3 v0, SAT_Vector3 v1) {
    x = (v0.y * v1.z) - (v0.z * v1.y);
    y = (v0.z * v1.x) - (v0.x * v1.z);
    z = (v0.x * v1.y) - (v0.y * v1.x);
  }

  float length(void) {
    return sqrtf(x*x + y*y + z*z);
  }

  float length_sq(void) {
    return (x*x + y*y + z*z);
  }

  void normalize(void) {
    float il = 1.0f / length();
    x *= il;
    y *= il;
    z *= il;
  }

};

//----------------------------------------------------------------------

SAT_Vector3  SAT_Vector3_add(SAT_Vector3 v0, SAT_Vector3 v1)  { SAT_Vector3 r = { v0.x + v1.x, v0.y + v1.y, v0.z + v1.z }; return r; }
SAT_Vector3  SAT_Vector3_sub(SAT_Vector3 v0, SAT_Vector3 v1)  { SAT_Vector3 r = { v0.x - v1.x, v0.y - v1.y, v0.z - v1.z }; return r; }
SAT_Vector3  SAT_Vector3_mul(SAT_Vector3 v0, SAT_Vector3 v1)  { SAT_Vector3 r = { v0.x * v1.x, v0.y * v1.y, v0.z * v1.z }; return r; }
SAT_Vector3  SAT_Vector3_div(SAT_Vector3 v0, SAT_Vector3 v1)  { SAT_Vector3 r = { v0.x / v1.x, v0.y / v1.y, v0.z / v1.z }; return r; }

SAT_Vector3  SAT_Vector3_add(SAT_Vector3 v, float f)   { SAT_Vector3 r = { v.x + f, v.y + f, v.z + f }; return r; }
SAT_Vector3  SAT_Vector3_sub(SAT_Vector3 v, float f)   { SAT_Vector3 r = { v.x - f, v.y - f, v.z - f }; return r; }
SAT_Vector3  SAT_Vector3_mul(SAT_Vector3 v, float f)   { SAT_Vector3 r = { v.x * f, v.y * f, v.z * f }; return r; }
SAT_Vector3  SAT_Vector3_div(SAT_Vector3 v, float f)   { SAT_Vector3 r = { v.x / f, v.y / f, v.z / f }; return r; }

float SAT_Vector3_dot(SAT_Vector3 v0, SAT_Vector3 v1) {
  return (v0.x * v1.x)
       + (v0.y * v1.y)
       + (v0.z * v1.z);
}

SAT_Vector3 SAT_Vector3_cross(SAT_Vector3 v0, SAT_Vector3 v1) {
  return SAT_Vector3(
    (v0.y * v1.z) - (v0.z * v1.y),
    (v0.z * v1.x) - (v0.x * v1.z),
    (v0.x * v1.y) - (v0.y * v1.x)
  );
}

float SAT_Vector3_length(SAT_Vector3 v) {
  return sqrtf( (v.x*v.x) + (v.y*v.y) + (v.z*v.z) );
}

float SAT_Vector3_length_sq(SAT_Vector3 v) {
  return ( (v.x*v.x) + (v.y*v.y) + (v.z*v.z) );
}

SAT_Vector3 SAT_Vector3_normalize(SAT_Vector3 v) {
  float fi = 1.0f / SAT_Vector3_length(v);
  return SAT_Vector3(
    v.x * fi,
    v.y * fi,
    v.z * fi
  );
}

//----------------------------------------------------------------------
#endif
