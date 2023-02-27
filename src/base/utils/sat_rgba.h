#ifndef sat_rgba_included
#define sat_rgba_included
//----------------------------------------------------------------------

#include "base/utils/sat_math.h"
#include "base/types/sat_color.h"
//typedef uint32_t SAT_RGBA;

//----------

// typedef uint32_t SAT_RGBA; // 0xaabbggrr
// typedef uint32_t SAT_BGRA; // 0xaarrggbb

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

uint32_t SAT_RGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a=255) {
  return
      ((uint32_t)a << 24)
    + ((uint32_t)r << 16)
    + ((uint32_t)g << 8 )
    + b;
}

uint32_t SAT_RGBA(SAT_Color AColor) {
  uint8_t r = 255 * AColor.r;
  uint8_t g = 255 * AColor.g;
  uint8_t b = 255 * AColor.b;
  uint8_t a = 255 * AColor.a;
  return SAT_RGBA(r,g,b,a);
}

/*
uint32_t SAT_RGBA(float r, float g, float b, float a=1.0f) {
  uint8_t R = 255.0f * r;
  uint8_t G = 255.0f * g;
  uint8_t B = 255.0f * b;
  uint8_t A = 255.0f * a;
  return
      ((uint32_t)a << 24)
    + ((uint32_t)r << 16)
    + ((uint32_t)g << 8 )
    + b;
}
*/

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

/*
  http://www.java-gaming.org/topics/fast-24-bit-rgb-addition-without-overflow-checks/37699/view.html

  While working on fast bilinear filtering, I found that this adds up 4 colors
  without checking for overflow. It works perfectly with bilinear texture
  filtering because overflow shouldn't occur anyways.
*/

uint32_t SAT_RGBA_Add4(uint32_t c1, uint32_t c2, uint32_t c3, uint32_t c4) {
  int sum = (c1 & 0xfefeff) + (c2 & 0xfefeff) + (c3 & 0xfefeff) + (c4 & 0xfefeff);
  return sum | (-(sum & 0x100) >> 24);
}

//----------

uint32_t SAT_RGBA_Average(uint32_t c1, uint32_t c2) {
  //uint32_t a =  c1 & 0xff000000;
  return ((c1>>1) & 0x7f7f7f) + ((c2>>1) & 0x7f7f7f);
}

//----------

/*
  http://www.java-gaming.org/topics/fastest-rgb-24bit-color-blend-and-others/21434/view.html
*/

uint32_t SAT_RGBA_Average2(uint32_t c1, uint32_t c2) {
  return ((c1 & 0xfefeff) + (c2 & 0xfefeff)) >> 1;
}

//----------------------------------------------------------------------
//
// scale
//
//----------------------------------------------------------------------

uint32_t SAT_RGBA_ScaleAlpha(uint32_t c) {
  uint8_t a = (c & 0xff000000) >> 24;
  //a = 255 - a;
  uint32_t r = (c & 0xff0000) >> 16;
  uint32_t g = (c & 0x00ff00) >> 8;
  uint32_t b = (c & 0x0000ff);
  r = (r*a) >> 8;
  g = (g*a) >> 8;
  b = (b*a) >> 8;
  return SAT_RGBA(r,g,b,a);
}

//----------

uint32_t SAT_RGBA_Scale(uint32_t c, uint8_t a) {
  uint8_t alpha = (c & 0xff000000) >> 24;
  //a = 255 - a;
  uint32_t r = (c & 0xff0000) >> 16;
  uint32_t g = (c & 0x00ff00) >> 8;
  uint32_t b = (c & 0x0000ff);
  r = (r*a) >> 8;
  g = (g*a) >> 8;
  b = (b*a) >> 8;
  return SAT_RGBA(r,g,b,alpha);
}

//----------

uint32_t SAT_RGBA_Scale0(uint32_t c, uint8_t a) {
  //uint8_t alpha = (c & 0xff000000) >> 24;
  //a = 255 - a;
  uint32_t r = (c & 0xff0000) >> 16;
  uint32_t g = (c & 0x00ff00) >> 8;
  uint32_t b = (c & 0x0000ff);
  r = (r*a) >> 8;
  g = (g*a) >> 8;
  b = (b*a) >> 8;
  return SAT_RGBA(r,g,b,255);
}

//----------

uint32_t SAT_RGBA_Scale2(uint32_t c, uint8_t a) {
  uint8_t alpha = (c & 0xff000000) >> 24;
  a = (a*alpha) >> 8;
  //a = 255 - a;
  uint32_t r = (c & 0xff0000) >> 16;
  uint32_t g = (c & 0x00ff00) >> 8;
  uint32_t b = (c & 0x0000ff);
  r = (r*a) >> 8;
  g = (g*a) >> 8;
  b = (b*a) >> 8;
  return SAT_RGBA(r,g,b,alpha);
}

//----------

/*
  used in (SAT_RGBA_Alpha):
    SAT_Bitmap.blendPixel
    SAT_Bitmap.premultAlpha
*/

//uint8_t SAT_RGBA_ScaleElement(uint32_t c, uint32_t a) {
//  uint32_t ret = (c*a) >> 8;
//  return ret & 0xff;
//}

//----------

/*
  used in  (SAT_RGBA_Scale):
    SAT_Bitmap.colorMatrix
*/

//uint8_t SAT_RGBA_ScaleF(uint8_t c, float n ) {
//  float nc = n * (float)c;
//  return SAT_MinI((uint8_t)255,(uint8_t)nc);
//}

//----------

//uint32_t SAT_RGBA_ScaleF2(uint32_t c, float a) {
//  uint8_t alpha = (c & 0xff000000) >> 24;
//  //a = 255 - a;
//  uint32_t r = (c & 0xff0000) >> 16;
//  uint32_t g = (c & 0x00ff00) >> 8;
//  uint32_t b = (c & 0x0000ff);
//  r = ((float)r*a);
//  g = ((float)g*a);
//  b = ((float)b*a);
//  return SAT_RGBA(r,g,b,alpha);
//}

//----------------------------------------------------------------------
//
// alpha blend
//
//----------------------------------------------------------------------

/*
  c1 = screen
  c2 = sprite
  blend with c2.alpha
*/

uint32_t SAT_RGBA_AlphaBlend(uint32_t c1, uint32_t c2) {
  //uint8_t a = (c1 & 0xff000000) >> 24;
  uint8_t a = (c2 & 0xff000000) >> 24;
  //a = 255 - a;
  uint32_t r = (c2 & 0x00ff0000) >> 16;
  uint32_t g = (c2 & 0x0000ff00) >> 8;
  uint32_t b = (c2 & 0x000000ff);
  r = (r*a) >> 8;
  g = (g*a) >> 8;
  b = (b*a) >> 8;
  c2 = SAT_RGBA(r,g,b,a); // was RGB
  a = 255-a;
  r = (c1 & 0x00ff0000) >> 16;
  g = (c1 & 0x0000ff00) >> 8;
  b = (c1 & 0x000000ff);
  r = (r*a) >> 8;
  g = (g*a) >> 8;
  b = (b*a) >> 8;
  c1 = SAT_RGBA(r,g,b,a); // was BGR
  return c1+c2;
}

//----------

uint32_t SAT_RGBA_AlphaBlend(uint32_t c1, uint32_t c2, uint8_t a) {
  a = 255 - a;
  uint32_t r = (c1 & 0x00ff0000) >> 16;
  uint32_t g = (c1 & 0x0000ff00) >> 8;
  uint32_t b = (c1 & 0x000000ff);
  r = (r*a) >> 8;
  g = (g*a) >> 8;
  b = (b*a) >> 8;
  c1 = SAT_RGBA(r,g,b); // was RGB
  a = 255-a;
  r = (c2 & 0x00ff0000) >> 16;
  g = (c2 & 0x0000ff00) >> 8;
  b = (c2 & 0x000000ff);
  r = (r*a) >> 8;
  g = (g*a) >> 8;
  b = (b*a) >> 8;
  c2 = SAT_RGBA(r,g,b); // was BGR
  return c1 + c2;
}

//----------

uint32_t SAT_RGBA_AlphaBlend2(uint32_t c1, uint32_t c2, uint8_t a) {
  uint8_t alpha = (c1 & 0xff000000) >> 24;
  a = 255 - a;
  uint32_t r = (c1 & 0xff0000) >> 16;
  uint32_t g = (c1 & 0x00ff00) >> 8;
  uint32_t b = (c1 & 0x0000ff);
  r = (r*a) >> 8;
  g = (g*a) >> 8;
  b = (b*a) >> 8;
  c1 = SAT_RGBA(r,g,b,alpha); // was RGB
  a = 255-a;
  r = (c2 & 0xff0000) >> 16;
  g = (c2 & 0x00ff00) >> 8;
  b = (c2 & 0x0000ff);
  r = (r*a) >> 8;
  g = (g*a) >> 8;
  b = (b*a) >> 8;
  c2 = SAT_RGBA(r,g,b,alpha); // was BGR
  return c1+c2;
}

//----------

uint32_t SAT_RGBA_AlphaBlend3(uint32_t c1, uint32_t c2, uint8_t alpha) {
  alpha = 255-alpha;
  uint32_t a =  c1 & 0xff000000;
  uint32_t rb = ((c2 & 0xFF00FF) + ((c1 & 0xFF00FF) - (c2 & 0xFF00FF)) * alpha) >> 8;
  uint32_t g = ((c2 & 0xFF00) + ((c1 & 0xFF00) - (c2 & 0xFF00)) * alpha) >> 8;
  rb &= 0xFF00FF;
  g &= 0xFF00;
  return rb | g | a;
}

//----------

// alpha is multiplied by c1's alpha..

uint32_t SAT_RGBA_AlphaBlend4(uint32_t c1, uint32_t c2, uint8_t alpha) {
  uint8_t a = (c2 & 0xff000000) >> 24;
  alpha = (alpha*a) >> 8;
  //a = 255 - a;
  uint32_t r = (c2 & 0xff0000) >> 16;
  uint32_t g = (c2 & 0x00ff00) >> 8;
  uint32_t b = (c2 & 0x0000ff);
  r = (r*alpha) >> 8;
  g = (g*alpha) >> 8;
  b = (b*alpha) >> 8;
  c2 = SAT_RGBA(r,g,b,a); // was RGB
  alpha = 255 - alpha;
  r = (c1 & 0xff0000) >> 16;
  g = (c1 & 0x00ff00) >> 8;
  b = (c1 & 0x0000ff);
  r = (r*alpha) >> 8;
  g = (g*alpha) >> 8;
  b = (b*alpha) >> 8;
  c1 = SAT_RGBA(r,g,b,a); // was BGR
  return c1+c2;
}

//----------

// alpha replaces c1's alpha..

uint32_t SAT_RGBA_AlphaBlend5(uint32_t c1, uint32_t c2, uint8_t alpha) {
  uint8_t a = (c2 & 0xff000000) >> 24;
  //alpha = (alpha*a) >> 8;
  //a = 255 - a;
  uint32_t r = (c2 & 0xff0000) >> 16;
  uint32_t g = (c2 & 0x00ff00) >> 8;
  uint32_t b = (c2 & 0x0000ff);
  r = (r*alpha) >> 8;
  g = (g*alpha) >> 8;
  b = (b*alpha) >> 8;
  c2 = SAT_RGBA(r,g,b,a); // was RGB
  alpha = 255 - alpha;
  r = (c1 & 0xff0000) >> 16;
  g = (c1 & 0x00ff00) >> 8;
  b = (c1 & 0x0000ff);
  r = (r*alpha) >> 8;
  g = (g*alpha) >> 8;
  b = (b*alpha) >> 8;
  c1 = SAT_RGBA(r,g,b,a); // was BGR
  return c1+c2;
}

//----------

/*
  http://www.java-gaming.org/topics/super-fast-tween-color/27738/view.html

  Took me a while to figure out how to tween colors really fast, so maybe some
  more people can use this. Drawing 262144 pixes from perlin noise, went from
  40 ms to 6 ms, and it does not have any disadvantages (exept color maps may
  be a bit more complex because it uses ints). For maximum speed it ignores the
  alpha of both colors, and uses the parameter alpha to set the amount of the
  second color to blend.
*/

uint32_t SAT_RGBA_AlphaBlend6(uint32_t c1, uint32_t c2, uint32_t a) {
  uint32_t r = (c1 & 0xFF0000) >> 16;
  uint32_t g = (c1 & 0x00FF00) >> 8;
  uint32_t b = c1 & 0x0000FF;
  uint32_t rd = r + (((((c2 & 0xFF0000) >> 16) - r) * a) >> 8);
  uint32_t gd = g + (((((c2 & 0x00FF00) >> 8) - g) * a) >> 8);
  uint32_t bd = b + (((((c2 & 0x0000FF)) - b) * a) >> 8);
  return 0xFF000000 | (rd << 16) | (gd << 8) | bd;
}

//----------

/*
  http://www.java-gaming.org/topics/fastest-rgb-blend-24-bits/24198/view.html
*/

uint32_t SAT_RGBA_AlphaBlend7(uint32_t c1, uint32_t c2, uint32_t factor) {
  uint32_t f1 = 256 - factor;
  return ((((c1 & 0xFF00FF) * f1 + (c2 & 0xFF00FF) * factor ) & 0xFF00FF00) |
          (((c1 & 0x00FF00) * f1 + (c2 & 0x00FF00) * factor ) & 0x00FF0000)) >> 8;
}

//----------------------------------------------------------------------
//
// bilinear
//
//----------------------------------------------------------------------

/*
  http://www.java-gaming.org/index.php?topic=22121.0

  Perform a bilinear interpolation of 4 RGB/vector each packed in an int as
  three unsigned bytes 0xXXYYZZ or 0xRRGGBB.
  can be either RGB color for image bicubic filtering or vector XYZ
  (only positive)

  c1 value/color 1 (upper left value/RGB pixel)
  c2 value/color 2 (upper right value/RGB pixel)
  c3 value/color 3 (lower left value/RGB pixel)
  c4 value/color 4 (lower right value/RGB pixel)
  bX x interpolation factor (range 0-256)
  bY y interpolation factor (range 0-256)

  return interpolated value(packed RGB pixel) of c1,c2,c3,c4 for given factors
  bX & bY as three packed unsigned bytes

  @author Bruno Augier http://dzzd.net/
*/

uint32_t SAT_RGBA_BilinearAlphaBlend(uint32_t c1, uint32_t c2, uint32_t c3, uint32_t c4, int32_t bX, int32_t bY) {
  uint32_t f24 = (bX * bY) >> 8;
  uint32_t f23 = bX - f24;
  uint32_t f14 = bY - f24;
  uint32_t f13 = ((256 - bX) * (256 - bY)) >> 8; // this one can be computed faster
  uint32_t c = (((  (c1 & 0xFF00FF) * f13
                  + (c2 & 0xFF00FF) * f23
                  + (c3 & 0xFF00FF) * f14
                  + (c4 & 0xFF00FF) * f24 ) & 0xFF00FF00 ) |
              ((  (c1 & 0x00FF00) * f13
                  + (c2 & 0x00FF00) * f23
                  + (c3 & 0x00FF00) * f14
                  + (c4 & 0x00FF00) * f24 ) & 0x00FF0000 )
             ) >> 8;
  uint8_t a1 = c1 >> 24;
  uint8_t a2 = c2 >> 24;
  uint8_t a3 = c3 >> 24;
  uint8_t a4 = c4 >> 24;
  uint32_t a = ((a1 * f13)>>8)
             + ((a2 * f23)>>8)
             + ((a3 * f14)>>8)
             + ((a4 * f24)>>8);
  a <<= 24;
  return c | a;//0xff000000;
}

//----------

/*
  https://cboard.cprogramming.com/game-programming/19926-super-fast-bilinear-interpolation.html
*/

uint32_t SAT_RGBA_BilinearAlphaBlend2(uint32_t c1, uint32_t c2, uint32_t c3, uint32_t c4, int32_t bX, int32_t bY) {
  int h = (bX & 0xff);
	int i = (bY & 0xff);
	bX = bX >> 8;
	bY = bY >> 8;
  int a = (0x100 - h) * (0x100 - i);
  int b = (0x000 + h) * (0x100 - i);
	int c = (0x000 + h) * (0x000 + i);
	int d = 65536 - a - b - c;
	unsigned int R = 0x00ff0000 & (((c1 >> 16)      * a) + ((c2 >> 16)      * b) + ((c3 >> 16)      * c) + ((c4 >> 16)      * d));
	unsigned int G = 0xff000000 & (((c1 & 0x00ff00) * a) + ((c2 & 0x00ff00) * b) + ((c3 & 0x00ff00) * c) + ((c4 & 0x00ff00) * d));
	unsigned int B = 0x00ff0000 & (((c1 & 0x0000ff) * a) + ((c2 & 0x0000ff) * b) + ((c3 & 0x0000ff) * c) + ((c4 & 0x0000ff) * d));
	return R|((G|B)>>16);
}

//----------------------------------------------------------------------
//
// saturate
//
//----------------------------------------------------------------------

uint32_t SAT_RGBA_AddSaturate(uint32_t c1, uint32_t c2) {
  uint32_t r1 = (c1 & 0xff0000) >> 16;
  uint32_t g1 = (c1 & 0x00ff00) >> 8;
  uint32_t b1 = (c1 & 0x0000ff);
  uint32_t r2 = (c2 & 0xff0000) >> 16;
  uint32_t g2 = (c2 & 0x00ff00) >> 8;
  uint32_t b2 = (c2 & 0x0000ff);
  uint32_t r = (r1+r2); if (r>255) r = 255;
  uint32_t g = (g1+g2); if (g>255) g = 255;
  uint32_t b = (b1+b2); if (b>255) b = 255;
  return SAT_RGBA(r,g,b);
}

//----------

/*
  http://www.hugi.scene.org/online/coding/hugi%2021%20-%20co32bcol.htm
*/

uint32_t SAT_RGBA_AddSaturate2(uint32_t c1, uint32_t c2) {
  //uint32_t a =  c1 & 0xff000000;
  uint32_t a =  c2 & 0xff000000;
  uint32_t b = (c1 & 0xff)     + (c2 & 0xff);
  uint32_t g = (c1 & 0xff00)   + (c2 & 0xff00);
  uint32_t r = (c1 & 0xff0000) + (c2 & 0xff0000);
  if (b > 0xff)     b = 0xff;
  if (g > 0xff00)   g = 0xff00;
  if (r > 0xff0000) r = 0xff0000;
  return b | g | r | a;
}

//----------

/*
  http://www.java-gaming.org/topics/fastest-rgb-24-color-addition/24199/view.html
*/

uint32_t SAT_RGBA_AddSaturate3(uint32_t color1, uint32_t color2) {
  color1 &= 0xFEFEFE; //set some lsb to 0
  color2 &= 0xFEFEFE; //set some lsb to 0
  //add color
  uint32_t color = color1 + color2;
  //clamp color to 0 - 255
  color |= ((color >> 8) & 0x010101) * 0xFF;
  return color;
}

//----------

uint32_t SAT_RGBA_SubSaturate(uint32_t c1, uint32_t c2) {
  uint32_t a =  c2 & 0xff000000;
  int32_t b = (c1 & 0xff)     - (c2 & 0xff);
  int32_t g = (c1 & 0xff00)   - (c2 & 0xff00);
  int32_t r = (c1 & 0xff0000) - (c2 & 0xff0000);
  if (b <     0x00) b = 0;
  if (g <   0x0100) g = 0;
  if (r < 0x010000) r = 0;
  return b | g | r | a;
}

//----------

uint32_t SAT_RGBA_MulSaturate(uint32_t c1, uint32_t c2) {
  uint32_t a =  c2 & 0xff000000;
  //c1 &= 0xffffff;
  //c2 &= 0xffffff;
  uint32_t b = (((c1 & 0xff)     * (c2 & 0xff))     >> 8 );//& 0xff;
  uint32_t g = (((c1 & 0xff00)   * (c2 & 0xff00))   >> 16) & 0xff00;
  uint32_t r = (((c1 & 0xff0000) * (c2 & 0xff0000)) >> 24) & 0xff0000;
  return b | g | r | a;
}

//----------------------------------------------------------------------
//
// conversion
//
//----------------------------------------------------------------------

uint32_t SAT_RGBA_Grey(uint32_t c) {
  uint32_t a = (c & 0xff000000) >> 24;
  uint32_t r = (c & 0xff0000) >> 16;
  uint32_t g = (c & 0x00ff00) >> 8;
  uint32_t b = (c & 0x0000ff);
  float fr = (float)r * 0.299f;
  float fg = (float)g * 0.587f;
  float fb = (float)b * 0.114f;
  float n = fr + fg + fb / 3.0f;
  return SAT_RGBA(n,n,n,a);
}

//----------

// http://www.quasimondo.com/archives/000696.php

/*
  h will be between -Pi and Pi
  s will be between 0 and 1/sqrt(2), so you might want to multiply the
  s by sqrt(2) to get it in a range between 0 and 1.
*/

uint32_t SAT_RGBA_RgbToHsl(uint32_t color) {
  float r = (float)((color & 0xff0000) >> 16) * SAT_INV255F;
  float g = (float)((color & 0xff00  ) >> 8 ) * SAT_INV255F;
  float b = (float)((color & 0xff    )      ) * SAT_INV255F;
  // r,b and b are assumed to be in the range 0...1
  //float l =  r * 0.299 + g * 0.587 + b * 0.114;
  float u = - r * 0.1471376975169300226 - g * 0.2888623024830699774 + b * 0.436;
  float v =   r * 0.615                 - g * 0.514985734664764622  - b * 0.100014265335235378;
  float h = atan2(v,u);
  h += SAT_PI;      // 0..pi2
  h *= SAT_INVPI2;  // 0..1
  float s = sqrtf( u*u + v*v );
  s *= SAT_SQRT2;
  //printf("hsl: %.2f, %.2f, %.2f\n",h,s,l);
  return SAT_RGBA(h*256,s*256,v*256);
}

//----------

/*
  hue is an angle in radians (-Pi...Pi)
  for saturation the range 0...1/sqrt(2) equals 0% ... 100%
  luminance is in the range 0...1
*/

uint32_t SAT_RGBA_HslToRgb(uint32_t color) {
  float h = (float)((color & 0xff )) * SAT_INV255F;  // 0..1
  h *= SAT_PI2;                                               // 0..PI2
  h -= SAT_PI;                                                // -PI..PI
  float s = (float)((color & 0xff00 ) >> 8 ) * SAT_INV255F;  // 0..1
  s = s / SAT_SQRT2;                                          // 0..1/sqrt(2)
  float l = (float)((color & 0xff0000 ) >> 16 ) * SAT_INV255F;
  float u = cosf( h ) * s;
  float v = sinf( h ) * s;
  float r = l + 1.139837398373983740  * v;
  float g = l - 0.3946517043589703515 * u - 0.5805986066674976801 * v;
  float b = l + 2.03211091743119266   * u;
  //printf("rgb: %.2f, %.2f, %.2f\n",r,g,b);
  return SAT_RGBA(r*256,g*256,b*256);
}

//----------

uint32_t SAT_RGBA_RgbToHsv(uint32_t color) {
  uint32_t a_ = (color & 0xff000000) >> 24;
  uint32_t r_ = (color & 0x00ff0000) >> 16;
  uint32_t g_ = (color & 0x0000ff00) >> 8;
  uint32_t b_ = (color & 0x000000ff);
  float r = (float)r_ * SAT_INV255F;
  float g = (float)g_ * SAT_INV255F;
  float b = (float)b_ * SAT_INV255F;
  //local c=r+g+b
  uint32_t c = r + g + b;
  //if c<1e-4 then return 0,2/3,0
  if (c < 0.001) { // 1e-4
    return SAT_RGBA(0,(2.0f/3.0f)*255,0,a_);
  }
  else {
    //float p = 2.0f * (b*b+g*g+r*r-g*r-b*g-b*r) ^ 0.5f;
    //float h = atan2(b-g,(2*r-b-g)/3^0.5);
    float p = powf( 0.5f, 2.0f * (b*b+g*g+r*r-g*r-b*g-b*r));
    float h = atan2( b-g, powf(0.5f, (2*r-b-g) / 3.0f));
    // hue = 0..360 ?
    h /= SAT_PI2;//360.0f;
    float s = p / (c+p);
    float v = (c+p) / 3.0f;
    return SAT_RGBA(h*255,s*255,v*255,a_);
  }
}

//----------

uint32_t SAT_RGBA_HsvToRgb(uint32_t color) {
  uint32_t a_ = (color & 0xff000000) >> 24;
  uint32_t h_ = (color & 0x00ff0000) >> 16;
  uint32_t s_ = (color & 0x0000ff00) >> 8;
  uint32_t v_ = (color & 0x000000ff);
  float h = (float)h_ * SAT_INV255F;
  h *= SAT_PI2;//360.0f;
  float s = (float)s_ * SAT_INV255F;
  float v = (float)v_ * SAT_INV255F;
  float r = v * (1 + s * (cosf(h) - 1));
  float g = v * (1 + s * (cosf(h - 2.09439) - 1));
  float b = v * (1 + s * (cosf(h + 2.09439) - 1));
  return SAT_RGBA(r*255,g*255,b*255,a_);
}

//----------

//function SAT_RGBA_HsvTween(h0,s0,v0,h1,s1,v1,t)
//  local dh=(h1-h0+3.14159)%6.28318-3.14159
//  local h=h0+t*dh
//  local s=s0+t*(s1-s0)
//  local v=v0+t*(v1-v0)
//  return h,s,v
//end

//----------------------------------------------------------------------
#endif
