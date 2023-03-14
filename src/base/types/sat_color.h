#ifndef sat_color_included
#define sat_color_included
//----------------------------------------------------------------------

#include "base/sat.h"

class SAT_Color {

public:

  union {
    struct {
      double r = 0.0;
      double g = 0.0;
      double b = 0.0;
      double a = 1.0;
    };
    double rgba[4];
  };

//------------------------------
public:
//------------------------------

  SAT_Color() {
  }

  //----------

  SAT_Color(double v) {
    r = v;
    g = v;
    b = v;
    a = 1.0f;
  }

  //----------

  SAT_Color(double ar, double ag, double ab, double aa=1.0f) {
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
    double aa = (color & 0xff000000) >> 24;
    double rr = (color & 0x00ff0000) >> 16;
    double gg = (color & 0x0000ff00) >> 8;
    double bb = (color & 0x000000ff);
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

  void blend(SAT_Color color, double alpha) {
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

#define SAT_DarkRed        SAT_Color( 0.25, 0.00, 0.00 )
#define SAT_Red            SAT_Color( 0.50, 0.00, 0.00 )
#define SAT_LightRed       SAT_Color( 0.75, 0.00, 0.00 )
#define SAT_BrightRed      SAT_Color( 1.00, 0.00, 0.00 )

#define SAT_DarkOrange     SAT_Color( 0.25, 0.12, 0.00 )
#define SAT_Orange         SAT_Color( 0.50, 0.25, 0.00 )
#define SAT_LightOrange    SAT_Color( 0.75, 0.37, 0.00 )
#define SAT_Brightorange   SAT_Color( 1.00, 0.50, 0.00 )

#define SAT_DarkYellow     SAT_Color( 0.25, 0.25, 0.00 )
#define SAT_Yellow         SAT_Color( 0.50, 0.50, 0.00 )
#define SAT_LightYellow    SAT_Color( 0.75, 0.75, 0.00 )
#define SAT_BrightYellow   SAT_Color( 1.00, 1.00, 0.00 )

#define SAT_DarkLime       SAT_Color( 0.12, 0.25, 0.00 )
#define SAT_Lime           SAT_Color( 0.25, 0.50, 0.00 )
#define SAT_LightLime      SAT_Color( 0.37, 0.75, 0.00 )
#define SAT_BrightLime     SAT_Color( 0.50, 1.00, 0.00 )

#define SAT_DarkGreen      SAT_Color( 0.00, 0.25, 0.00 )
#define SAT_Green          SAT_Color( 0.00, 0.50, 0.00 )
#define SAT_LightGreen     SAT_Color( 0.00, 0.75, 0.00 )
#define SAT_BrightGreen    SAT_Color( 0.00, 1.00, 0.00 )

#define SAT_DarkGreen2     SAT_Color( 0.00, 0.25, 0.12 )
#define SAT_Green2         SAT_Color( 0.00, 0.50, 0.35 )
#define SAT_LightGreen2    SAT_Color( 0.00, 0.75, 0.37 )
#define SAT_BrightGreen2   SAT_Color( 0.00, 1.00, 0.50 )

#define SAT_DarkCyan       SAT_Color( 0.00, 0.25, 0.25 )
#define SAT_Cyan           SAT_Color( 0.00, 0.50, 0.50 )
#define SAT_LightCyan      SAT_Color( 0.00, 0.75, 0.75 )
#define SAT_BrightCyan     SAT_Color( 0.00, 1.00, 1.00 )

#define SAT_DarkCyan2      SAT_Color( 0.00, 0.18, 0.25 )
#define SAT_Cyan2          SAT_Color( 0.00, 0.37, 0.50 )
#define SAT_LightCyan2     SAT_Color( 0.00, 0.50, 0.75 )
#define SAT_BrightCyan2    SAT_Color( 0.00, 0.75, 1.00 )

#define SAT_DarkBlue1      SAT_Color( 0.00, 0.12, 0.25 )
#define SAT_Blue1          SAT_Color( 0.00, 0.25, 0.50 )
#define SAT_LightBlue1     SAT_Color( 0.00, 0.37, 0.75 )
#define SAT_BrightBlue1    SAT_Color( 0.00, 0.50, 1.00 )

#define SAT_DarkBlue       SAT_Color( 0.00, 0.00, 0.25 )
#define SAT_Blue           SAT_Color( 0.00, 0.00, 0.50 )
#define SAT_LightBlue      SAT_Color( 0.00, 0.00, 0.75 )
#define SAT_BrightBlue     SAT_Color( 0.00, 0.00, 1.00 )

#define SAT_DarkBlue2      SAT_Color( 0.12, 0.00, 0.25 )
#define SAT_Blue2          SAT_Color( 0.25, 0.00, 0.50 )
#define SAT_LightBlue2     SAT_Color( 0.37, 0.00, 0.75 )
#define SAT_BrightBlue2    SAT_Color( 0.50, 0.00, 1.00 )

#define SAT_DarkMagenta    SAT_Color( 0.25, 0.00, 0.25 )
#define SAT_Magenta        SAT_Color( 0.50, 0.00, 0.50 )
#define SAT_LightMagenta   SAT_Color( 0.75, 0.00, 0.75 )
#define SAT_BrightMagenta  SAT_Color( 1.00, 0.00, 1.00 )

#define SAT_DarkRed2       SAT_Color( 0.25, 0.00, 0.12 )
#define SAT_Red2           SAT_Color( 0.50, 0.00, 0.25 )
#define SAT_LightRed2      SAT_Color( 0.75, 0.00, 0.37 )
#define SAT_BrightRed2     SAT_Color( 1.00, 0.00, 0.50 )

#define SAT_Black          SAT_Color( 0.00  )
#define SAT_DarkestGrey    SAT_Color( 0.125 )
#define SAT_DarkerGrey     SAT_Color( 0.25  )
#define SAT_DarkGrey       SAT_Color( 0.375 )
#define SAT_Grey           SAT_Color( 0.50  )
#define SAT_LightGrey      SAT_Color( 0.625 )
#define SAT_LighterGrey    SAT_Color( 0.75  )
#define SAT_LightestGrey   SAT_Color( 0.875 )
#define SAT_White          SAT_Color( 1.00  )

//----------------------------------------------------------------------
#endif

