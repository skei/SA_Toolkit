#ifndef sat_color_included
#define sat_color_included
//----------------------------------------------------------------------

#include "base/sat.h"

template <typename T>
class SAT_Color {

public:

  //T r = 0.0f;
  //T g = 0.0f;
  //T b = 0.0f;
  //T a = 1.0f;

  union {
    T bgra[4];
    struct { T r,g,b,a; };
  };

//------------------------------
public:
//------------------------------

  SAT_Color() {}

  //----------

  SAT_Color(T v) {
    r = v;
    g = v;
    b = v;
    a = 1.0f;
  }

  //----------

  SAT_Color(T ar, T ag, T ab, T aa=1.0f) {
    r = ar;
    g = ag;
    b = ab;
    a = aa;
  }

//------------------------------
public:
//------------------------------

  operator uint32_t () {
    uint8_t rr = r * 255.0f;
    uint8_t gg = g * 255.0f;
    uint8_t bb = b * 255.0f;
    uint8_t aa = a * 255.0f;
    uint32_t color = (aa << 24) + (rr << 16) + (gg << 8) + bb;
    return color;
  }

  //----------

  SAT_Color& operator = (uint32_t color) {
    T aa = (color & 0xff000000) >> 24;
    T rr = (color & 0x00ff0000) >> 16;
    T gg = (color & 0x0000ff00) >> 8;
    T bb = (color & 0x000000ff);
    r = rr * SAT_INV255;
    g = gg * SAT_INV255;
    b = bb * SAT_INV255;
    a = aa * SAT_INV255;
    return *this;
  }

  //----------

  //SAT_Color& operator += (uint32_t color) {
  //  return *this;
  //}

  //----------

  //bool operator == (const char* str) {
  //  return (strcmp(MBuffer,str) == 0) ? true : false;
  //}

//------------------------------
public:
//------------------------------

  void add(SAT_Color color) {
    r += color.r;
    g += color.g;
    b += color.b;
    a += color.a;
  }

  //----------

  void mul(SAT_Color color) {
    r *= color.r;
    g *= color.g;
    b *= color.b;
    a *= color.a;
  }

  //----------

  //void blend(SAT_Color color1, SAT_Color color2, T blend) {
  //  SAT_Color result;
  //  result.r = (color1.r * blend) + (color2.r * (1.0 - blend));
  //  result.g = (color1.g * blend) + (color2.g * (1.0 - blend));
  //  result.b = (color1.b * blend) + (color2.b * (1.0 - blend));
  //  result.a = (color1.a * blend) + (color2.a * (1.0 - blend));
  //}

  //----------

  void blend(SAT_Color color, T alpha) {
    r = (color.r * alpha) + (r * (1.0 - alpha));
    g = (color.g * alpha) + (g * (1.0 - alpha));
    b = (color.b * alpha) + (b * (1.0 - alpha));
    a = 1;//(color.a * alpha) + (a * (1.0 - alpha));
  }

  //----------

  void blend(SAT_Color color) {
    blend(color,color.a);
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

typedef SAT_Color<float>    SAT_FColor;
typedef SAT_Color<int32_t>  SAT_IColor;
typedef SAT_Color<double>   SAT_DColor;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

/*
const SAT_Color SAT_COLOR_DARK_RED        = SAT_Color( 0.25, 0.00, 0.00 );
const SAT_Color SAT_COLOR_RED             = SAT_Color( 0.50, 0.00, 0.00 );
const SAT_Color SAT_COLOR_LIGHT_RED       = SAT_Color( 0.75, 0.00, 0.00 );
const SAT_Color SAT_COLOR_BRIGHT_RED      = SAT_Color( 1.00, 0.00, 0.00 );

const SAT_Color SAT_COLOR_DARK_ORANGE     = SAT_Color( 0.25, 0.12, 0.00 );
const SAT_Color SAT_COLOR_ORANGE          = SAT_Color( 0.50, 0.25, 0.00 );
const SAT_Color SAT_COLOR_LIGHT_ORANGE    = SAT_Color( 0.75, 0.37, 0.00 );
const SAT_Color SAT_COLOR_BRIGHT_ORANGE   = SAT_Color( 1.00, 0.50, 0.00 );

const SAT_Color SAT_COLOR_DARK_YELLOW     = SAT_Color( 0.25, 0.25, 0.00 );
const SAT_Color SAT_COLOR_YELLOW          = SAT_Color( 0.50, 0.50, 0.00 );
const SAT_Color SAT_COLOR_LIGHT_YELLOW    = SAT_Color( 0.75, 0.75, 0.00 );
const SAT_Color SAT_COLOR_BRIGHT_YELLOW   = SAT_Color( 1.00, 1.00, 0.00 );

const SAT_Color SAT_COLOR_DARK_LIME       = SAT_Color( 0.12, 0.25, 0.00 );
const SAT_Color SAT_COLOR_LIME            = SAT_Color( 0.25, 0.50, 0.00 );
const SAT_Color SAT_COLOR_LIGHT_LIME      = SAT_Color( 0.37, 0.75, 0.00 );
const SAT_Color SAT_COLOR_BRIGHT_LIME     = SAT_Color( 0.50, 1.00, 0.00 );


const SAT_Color SAT_COLOR_DARK_GREEN      = SAT_Color( 0.00, 0.25, 0.00 );
const SAT_Color SAT_COLOR_GREEN           = SAT_Color( 0.00, 0.50, 0.00 );
const SAT_Color SAT_COLOR_LIGHT_GREEN     = SAT_Color( 0.00, 0.75, 0.00 );
const SAT_Color SAT_COLOR_BRIGHT_GREEN    = SAT_Color( 0.00, 1.00, 0.00 );

const SAT_Color SAT_COLOR_DARK_GREEN2     = SAT_Color( 0.00, 0.25, 0.12 );
const SAT_Color SAT_COLOR_GREEN2          = SAT_Color( 0.00, 0.50, 0.35 );
const SAT_Color SAT_COLOR_LIGHT_GREEN2    = SAT_Color( 0.00, 0.75, 0.37 );
const SAT_Color SAT_COLOR_BRIGHT_GREEN2   = SAT_Color( 0.00, 1.00, 0.50 );

const SAT_Color SAT_COLOR_DARK_CYAN       = SAT_Color( 0.00, 0.25, 0.25 );
const SAT_Color SAT_COLOR_CYAN            = SAT_Color( 0.00, 0.50, 0.50 );
const SAT_Color SAT_COLOR_LIGHT_CYAN      = SAT_Color( 0.00, 0.75, 0.75 );
const SAT_Color SAT_COLOR_BRIGHT_CYAN     = SAT_Color( 0.00, 1.00, 1.00 );

const SAT_Color SAT_COLOR_DARK_CYAN2      = SAT_Color( 0.00, 0.18, 0.25 );
const SAT_Color SAT_COLOR_CYAN2           = SAT_Color( 0.00, 0.37, 0.50 );
const SAT_Color SAT_COLOR_LIGHT_CYAN2     = SAT_Color( 0.00, 0.50, 0.75 );
const SAT_Color SAT_COLOR_BRIGHT_CYAN2    = SAT_Color( 0.00, 0.75, 1.00 );

const SAT_Color SAT_COLOR_DARK_BLUE1      = SAT_Color( 0.00, 0.12, 0.25 );
const SAT_Color SAT_COLOR_BLUE1           = SAT_Color( 0.00, 0.25, 0.50 );
const SAT_Color SAT_COLOR_LIGHT_BLUE1     = SAT_Color( 0.00, 0.37, 0.75 );
const SAT_Color SAT_COLOR_BRIGHT_BLUE1    = SAT_Color( 0.00, 0.50, 1.00 );

const SAT_Color SAT_COLOR_DARK_BLUE       = SAT_Color( 0.00, 0.00, 0.25 );
const SAT_Color SAT_COLOR_BLUE            = SAT_Color( 0.00, 0.00, 0.50 );
const SAT_Color SAT_COLOR_LIGHT_BLUE      = SAT_Color( 0.00, 0.00, 0.75 );
const SAT_Color SAT_COLOR_BRIGHT_BLUE     = SAT_Color( 0.00, 0.00, 1.00 );

const SAT_Color SAT_COLOR_DARK_BLUE2      = SAT_Color( 0.12, 0.00, 0.25 );
const SAT_Color SAT_COLOR_BLUE2           = SAT_Color( 0.25, 0.00, 0.50 );
const SAT_Color SAT_COLOR_LIGHT_BLUE2     = SAT_Color( 0.37, 0.00, 0.75 );
const SAT_Color SAT_COLOR_BRIGHT_BLUE2    = SAT_Color( 0.50, 0.00, 1.00 );

const SAT_Color SAT_COLOR_DARK_MAGENTA    = SAT_Color( 0.25, 0.00, 0.25 );
const SAT_Color SAT_COLOR_MAGENTA         = SAT_Color( 0.50, 0.00, 0.50 );
const SAT_Color SAT_COLOR_LIGHT_MAGENTA   = SAT_Color( 0.75, 0.00, 0.75 );
const SAT_Color SAT_COLOR_BRIGHT_MAGENTA  = SAT_Color( 1.00, 0.00, 1.00 );

const SAT_Color SAT_COLOR_DARK_RED2       = SAT_Color( 0.25, 0.00, 0.12 );
const SAT_Color SAT_COLOR_RED2            = SAT_Color( 0.50, 0.00, 0.25 );
const SAT_Color SAT_COLOR_LIGHT_RED2      = SAT_Color( 0.75, 0.00, 0.37 );
const SAT_Color SAT_COLOR_BRIGHT_RED2     = SAT_Color( 1.00, 0.00, 0.50 );

const SAT_Color SAT_COLOR_BLACK           = SAT_Color( 0.00 );
const SAT_Color SAT_COLOR_DARK_GRAY       = SAT_Color( 0.25 );
const SAT_Color SAT_COLOR_GRAY            = SAT_Color( 0.50 );
const SAT_Color SAT_COLOR_LIGHT_GRAY      = SAT_Color( 0.75 );
const SAT_Color SAT_COLOR_WHITE           = SAT_Color( 1.00 );
*/

//----------------------------------------------------------------------
#endif

