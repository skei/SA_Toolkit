//#ifndef kode_bitmap_bresenham_included
//#define kode_bitmap_bresenham_included
////----------------------------------------------------------------------

#if 0

/*
  be careful...
  this is included directly into KBitmap (kodes_bitmap.h) if
  KODE_BITMAP_BRESENHAM is defined

  http://members.chello.at/~easyfilter/bresenham.html
  http://members.chello.at/~easyfilter/Bresenham.pdf
  http://members.chello.at/~easyfilter/bresenham.c
  author; Zingl Alois, 17.12.2012, version 1.1
*/

//----------------------------------------------------------------------

/*

// primitive

void drawLine(int x0, int y0, int x1, int y1, KODE_Color AColor);
void drawCircle(int xm, int ym, int r, KODE_Color AColor);
void drawEllipse(int xm, int ym, int a, int b, KODE_Color AColor);
void drawOptimizedEllipse(int xm, int ym, int a, int b, KODE_Color AColor);
void drawEllipseRect(int x0, int y0, int x1, int y1, KODE_Color AColor);
void drawQuadBezierSeg(int x0, int y0, int x1, int y1, int x2, int y2, KODE_Color AColor);
void drawQuadRationalBezierSeg(int x0, int y0, int x1, int y1, int x2, int y2, float w, KODE_Color AColor);
void drawCubicBezierSeg(int x0, int y0, float x1, float y1, float x2, float y2, int x3, int y3, KODE_Color AColor);

// anti-aliased

void drawLineAA(int x0, int y0, int x1, int y1, KODE_Color AColor);
void drawLineWidthAA(int x0, int y0, int x1, int y1, float wd, KODE_Color AColor);
void drawCircleAA(int xm, int ym, int r, KODE_Color AColor);
void drawEllipseRectAA(int x0, int y0, int x1, int y1, KODE_Color AColor);
void drawQuadBezierSegAA(int x0, int y0, int x1, int y1, int x2, int y2, KODE_Color AColor);
void drawQuadRationalBezierSegAA(int x0, int y0, int x1, int y1, int x2, int y2, float w, KODE_Color AColor);
void drawCubicBezierSegAA(int x0, int y0, float x1, float y1, float x2, float y2, int x3, int y3, KODE_Color AColor);

// rational / ellipse
void drawRotatedEllipseRectAA(int x0, int y0, int x1, int y1, long zd, KODE_Color AColor);
void drawRotatedEllipseAA(int x, int y, int a, int b, float angle, KODE_Color AColor);
void drawQuadRationalBezierAA(int x0, int y0, int x1, int y1, int x2, int y2, float w, KODE_Color AColor);

// bezier

void drawQuadBezierAA(int x0, int y0, int x1, int y1, int x2, int y2, KODE_Color AColor);
void drawCubicBezierAA(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, KODE_Color AColor);

// spline

void drawQuadSplineAA(int n, int x[], int y[], KODE_Color AColor);
void drawCubicSplineAA(int n, int x[], int y[], KODE_Color AColor);
 * 
 * */

//----------------------------------------------------------------------
//
// primitive
//
//----------------------------------------------------------------------

void drawLine(int x0, int y0, int x1, int y1, KODE_Color AColor) {
  uint32_t color = KODE_BGRA(AColor);
  int dx =  abs(x1-x0);
  int sx = x0<x1 ? 1 : -1;
  int dy = -abs(y1-y0);
  int sy = y0<y1 ? 1 : -1;
  int err = dx+dy;
  int e2;
  for (;;) {
    setPixel(x0,y0,color);
    e2 = 2 * err;
    if (e2 >= dy) {
      if (x0 == x1) break;
      err += dy;
      x0 += sx;
    }
    if (e2 <= dx) {
      if (y0 == y1) break;
      err += dx;
      y0 += sy;
    }
  }
}

void drawLineNoclip(int x0, int y0, int x1, int y1, KODE_Color AColor) {
  uint32_t color = KODE_BGRA(AColor);
  int dx =  abs(x1-x0);
  int sx = x0<x1 ? 1 : -1;
  int dy = -abs(y1-y0);
  int sy = y0<y1 ? 1 : -1;
  int err = dx+dy;
  int e2;
  for (;;) {
    setPixelNoclip(x0,y0,color);
    e2 = 2 * err;
    if (e2 >= dy) {
      if (x0 == x1) break;
      err += dy;
      x0 += sx;
    }
    if (e2 <= dx) {
      if (y0 == y1) break;
      err += dx;
      y0 += sy;
    }
  }
}

//----------

void drawCircle(int xm, int ym, int r, KODE_Color AColor) {
  uint32_t color = KODE_BGRA(AColor);
  int x = -r;
  int y = 0;
  int err = 2 - 2 * r;
  do {
    setPixel(xm-x, ym+y, color);
    setPixel(xm-y, ym-x, color);
    setPixel(xm+x, ym-y, color);
    setPixel(xm+y, ym+x, color);
    r = err;
    if (r <= y) err += ++y*2+1;
    if (r > x || err > y)
     err += ++x*2+1;
  } while (x < 0);
}

void drawCircleNoclip(int xm, int ym, int r, KODE_Color AColor) {
  uint32_t color = KODE_BGRA(AColor);
  int x = -r;
  int y = 0;
  int err = 2 - 2 * r;
  do {
    setPixelNoclip(xm-x, ym+y, color);
    setPixelNoclip(xm-y, ym-x, color);
    setPixelNoclip(xm+x, ym-y, color);
    setPixelNoclip(xm+y, ym+x, color);
    r = err;
    if (r <= y) err += ++y*2+1;
    if (r > x || err > y)
     err += ++x*2+1;
  } while (x < 0);
}

//----------

void drawEllipse(int xm, int ym, int a, int b, KODE_Color AColor) {
  uint32_t color = KODE_BGRA(AColor);
  int x = -a;
  int y = 0;
  long e2 = (long)b*b;
  int err = (long)x*(2*e2+x)+e2;
  do {
    setPixel(xm-x, ym+y, color);
    setPixel(xm+x, ym+y, color);
    setPixel(xm+x, ym-y, color);
    setPixel(xm-x, ym-y, color);
    e2 = 2*err;
    if (e2 >= (x*2+1)*(long)b*b) err += (++x*2+1)*(long)b*b;
    if (e2 <= (y*2+1)*(long)a*a) err += (++y*2+1)*(long)a*a;
  } while (x <= 0);
  while (y++ < b) {
   setPixel(xm, ym+y, color);
   setPixel(xm, ym-y, color);
  }
}

void drawEllipseNoclip(int xm, int ym, int a, int b, KODE_Color AColor) {
  uint32_t color = KODE_BGRA(AColor);
  int x = -a;
  int y = 0;
  long e2 = (long)b*b;
  int err = (long)x*(2*e2+x)+e2;
  do {
    setPixelNoclip(xm-x, ym+y, color);
    setPixelNoclip(xm+x, ym+y, color);
    setPixelNoclip(xm+x, ym-y, color);
    setPixelNoclip(xm-x, ym-y, color);
    e2 = 2*err;
    if (e2 >= (x*2+1)*(long)b*b) err += (++x*2+1)*(long)b*b;
    if (e2 <= (y*2+1)*(long)a*a) err += (++y*2+1)*(long)a*a;
  } while (x <= 0);
  while (y++ < b) {
   setPixelNoclip(xm, ym+y, color);
   setPixelNoclip(xm, ym-y, color);
  }
}

//----------

void drawOptimizedEllipse(int xm, int ym, int a, int b, KODE_Color AColor) {
  uint32_t color = KODE_BGRA(AColor);
  long x = -a, y = 0;
  long e2 = b, dx = (1+2*x)*e2*e2;
  long dy = x*x, err = dx+dy;
  do {
    setPixel(xm-x, ym+y, color);
    setPixel(xm+x, ym+y, color);
    setPixel(xm+x, ym-y, color);
    setPixel(xm-x, ym-y, color);
    e2 = 2*err;
    if (e2 >= dx) {
      x++;
      err += dx += 2*(long)b*b;
    }
    if (e2 <= dy) {
      y++;
      err += dy += 2*(long)a*a;
    }
  } while (x <= 0);
  while (y++ < b) {
    setPixel(xm, ym+y, color);
    setPixel(xm, ym-y, color);
  }
}

//----------

/* rectangular parameter enclosing the ellipse */

void drawEllipseRect(int x0, int y0, int x1, int y1, KODE_Color AColor) {
  uint32_t color = KODE_BGRA(AColor);
  long a = abs(x1-x0);
  long b = abs(y1-y0);
  long b1 = b&1;
  double dx = 4*(1.0-a)*b*b;
  double dy = 4*(b1+1)*a*a;
  double err = dx+dy+b1*a*a;
  double e2;
  if (x0 > x1) {
    x0 = x1;
    x1 += a;
  }
  if (y0 > y1) y0 = y1;
  y0 += (b+1)/2;
  y1 = y0-b1;
  a = 8*a*a;
  b1 = 8*b*b;
  do {
    setPixel(x1, y0, color);
    setPixel(x0, y0, color);
    setPixel(x0, y1, color);
    setPixel(x1, y1, color);
    e2 = 2*err;
    if (e2 <= dy) {
      y0++;
      y1--;
      err += dy += a;
    }
    if (e2 >= dx || 2*err > dy) {
      x0++;
      x1--;
      err += dx += b1;
    }
  } while (x0 <= x1);
  while (y0-y1 <= b) {
    setPixel(x0-1, y0, color);
    setPixel(x1+1, y0++, color);
    setPixel(x0-1, y1, color);
    setPixel(x1+1, y1--, color);
  }
}

//----------

/* draw a limited quadratic Bezier segment */

void drawQuadBezierSeg(int x0, int y0, int x1, int y1, int x2, int y2, KODE_Color AColor) {
  uint32_t color = KODE_BGRA(AColor);
  int sx = x2-x1, sy = y2-y1;
  long xx = x0-x1, yy = y0-y1, xy;
  double dx, dy, err, cur = xx*sy-yy*sx;
  //assert(xx*sx <= 0 && yy*sy <= 0); // sign of gradient must not change
  //KODE_Assert((xx*sx <= 0) && (yy*sy <= 0));
  if (sx*(long)sx+sy*(long)sy > xx*xx+yy*yy) {
    x2 = x0; x0 = sx+x1; y2 = y0; y0 = sy+y1; cur = -cur;
  }
  if (cur != 0) {
    xx += sx; xx *= sx = x0 < x2 ? 1 : -1;
    yy += sy; yy *= sy = y0 < y2 ? 1 : -1;
    xy = 2*xx*yy; xx *= xx; yy *= yy;
    if (cur*sx*sy < 0) {
      xx = -xx; yy = -yy; xy = -xy; cur = -cur;
    }
    dx = 4.0*sy*cur*(x1-x0)+xx-xy;
    dy = 4.0*sx*cur*(y0-y1)+yy-xy;
    xx += xx; yy += yy; err = dx+dy+xy;
    do {
      setPixel(x0,y0, color);
      if (x0 == x2 && y0 == y2) return;
      y1 = 2*err < dx;
      if (2*err > dy) { x0 += sx; dx -= xy; err += dy += yy; }
      if (    y1    ) { y0 += sy; dy -= xy; err += dx += xx; }
    } while (dy < 0 && dx > 0);
  }
  drawLine(x0,y0, x2,y2,AColor);
}

//----------

/* draw a limited rational Bezier segment, squared weight */

void drawQuadRationalBezierSeg(int x0, int y0, int x1, int y1, int x2, int y2, float w, KODE_Color AColor) {
  uint32_t color = KODE_BGRA(AColor);
  int sx = x2-x1, sy = y2-y1;
  double dx = x0-x2, dy = y0-y2, xx = x0-x1, yy = y0-y1;
  double xy = xx*sy+yy*sx, cur = xx*sy-yy*sx, err;
  //assert(xx*sx <= 0.0 && yy*sy <= 0.0);   /* sign of gradient must not change */
  //KODE_Assert((xx*sx <= 0.0) && (yy*sy <= 0.0));
  if (cur != 0.0 && w > 0.0) {
    if (sx*(long)sx+sy*(long)sy > xx*xx+yy*yy) {
      x2 = x0; x0 -= dx; y2 = y0; y0 -= dy; cur = -cur;
    }
    xx = 2.0*(4.0*w*sx*xx+dx*dx);
    yy = 2.0*(4.0*w*sy*yy+dy*dy);
    sx = x0 < x2 ? 1 : -1;
    sy = y0 < y2 ? 1 : -1;
    xy = -2.0*sx*sy*(2.0*w*xy+dx*dy);
    if (cur*sx*sy < 0.0) {
      xx = -xx; yy = -yy; xy = -xy; cur = -cur;
    }
    dx = 4.0*w*(x1-x0)*sy*cur+xx/2.0+xy;
    dy = 4.0*w*(y0-y1)*sx*cur+yy/2.0+xy;
    if (w < 0.5 && (dy > xy || dx < xy)) {
       cur = (w+1.0)/2.0; w = sqrt(w); xy = 1.0/(w+1.0);
       sx = floor((x0+2.0*w*x1+x2)*xy/2.0+0.5);
       sy = floor((y0+2.0*w*y1+y2)*xy/2.0+0.5);
       dx = floor((w*x1+x0)*xy+0.5); dy = floor((y1*w+y0)*xy+0.5);
       drawQuadRationalBezierSeg(x0,y0, dx,dy, sx,sy, cur,AColor);
       dx = floor((w*x1+x2)*xy+0.5); dy = floor((y1*w+y2)*xy+0.5);
       drawQuadRationalBezierSeg(sx,sy, dx,dy, x2,y2, cur,AColor);
       return;
    }
    err = dx+dy-xy;
    do {
      setPixel(x0,y0,color);
      if (x0 == x2 && y0 == y2) return;
      x1 = 2*err > dy; y1 = 2*(err+yy) < -dy;
      if (2*err < dx || y1) { y0 += sy; dy += xy; err += dx += xx; }
      if (2*err > dx || x1) { x0 += sx; dx += xy; err += dy += yy; }
    } while (dy <= xy && dx >= xy);
  }
  drawLine(x0,y0, x2,y2,AColor);
}

//----------

/* draw limited cubic Bezier segment */

void drawCubicBezierSeg(int x0, int y0, float x1, float y1, float x2, float y2, int x3, int y3, KODE_Color AColor) {
  uint32_t color = KODE_BGRA(AColor);
  int f, fx, fy, leg = 1;
  int sx = x0 < x3 ? 1 : -1, sy = y0 < y3 ? 1 : -1;
  float xc = -fabs(x0+x1-x2-x3), xa = xc-4*sx*(x1-x2), xb = sx*(x0-x1-x2+x3);
  float yc = -fabs(y0+y1-y2-y3), ya = yc-4*sy*(y1-y2), yb = sy*(y0-y1-y2+y3);
  double ab, ac, bc, cb, xx, xy, yy, dx, dy, ex, *pxy, EP = 0.01;
  /* check for curve restrains */
  /* slope P0-P1 == P2-P3    and  (P0-P3 == P1-P2      or   no slope change) */
  //assert((x1-x0)*(x2-x3) < EP && ((x3-x0)*(x1-x2) < EP || xb*xb < xa*xc+EP));
  //assert((y1-y0)*(y2-y3) < EP && ((y3-y0)*(y1-y2) < EP || yb*yb < ya*yc+EP));
  // if i comment away these lines, it doesn't crash...
  //   assert( ((x1-x0)*(x2-x3) < EP) && (((x3-x0)*(x1-x2) < EP) || (xb*xb < xa*xc+EP)) );
  //   assert( ((y1-y0)*(y2-y3) < EP) && (((y3-y0)*(y1-y2) < EP) || (yb*yb < ya*yc+EP)) );
  if (xa == 0 && ya == 0) {                              /* quadratic Bezier */
    sx = floor((3*x1-x0+1)/2); sy = floor((3*y1-y0+1)/2);
    return drawQuadBezierSeg(x0,y0, sx,sy, x3,y3,AColor);
  }
  x1 = (x1-x0)*(x1-x0)+(y1-y0)*(y1-y0)+1;
  x2 = (x2-x3)*(x2-x3)+(y2-y3)*(y2-y3)+1;
  do {
    ab = xa*yb-xb*ya; ac = xa*yc-xc*ya; bc = xb*yc-xc*yb;
    ex = ab*(ab+ac-3*bc)+ac*ac;
    f = ex > 0 ? 1 : sqrt(1+1024/x1);
    ab *= f; ac *= f; bc *= f; ex *= f*f;
    xy = 9*(ab+ac+bc)/8; cb = 8*(xa-ya);
    dx = 27*(8*ab*(yb*yb-ya*yc)+ex*(ya+2*yb+yc))/64-ya*ya*(xy-ya);
    dy = 27*(8*ab*(xb*xb-xa*xc)-ex*(xa+2*xb+xc))/64-xa*xa*(xy+xa);
    xx = 3*(3*ab*(3*yb*yb-ya*ya-2*ya*yc)-ya*(3*ac*(ya+yb)+ya*cb))/4;
    yy = 3*(3*ab*(3*xb*xb-xa*xa-2*xa*xc)-xa*(3*ac*(xa+xb)+xa*cb))/4;
    xy = xa*ya*(6*ab+6*ac-3*bc+cb); ac = ya*ya; cb = xa*xa;
    xy = 3*(xy+9*f*(cb*yb*yc-xb*xc*ac)-18*xb*yb*ab)/8;
    if (ex < 0) {
      dx = -dx; dy = -dy; xx = -xx; yy = -yy; xy = -xy; ac = -ac; cb = -cb;
    }
    ab = 6*ya*ac; ac = -6*xa*ac; bc = 6*ya*cb; cb = -6*xa*cb;
    dx += xy; ex = dx+dy; dy += xy;
    for (pxy = &xy, fx = fy = f; x0 != x3 && y0 != y3; ) {
      setPixel(x0,y0,color);
      do {
        if (dx > *pxy || dy < *pxy) goto exit;
        y1 = 2*ex-dy;
        if (2*ex >= dx) {
          fx--; ex += dx += xx; dy += xy += ac; yy += bc; xx += ab;
        }
        if (y1 <= 0) {
          fy--; ex += dy += yy; dx += xy += bc; xx += ac; yy += cb;
        }
      } while (fx > 0 && fy > 0);
      if (2*fx <= f) { x0 += sx; fx += f; }
      if (2*fy <= f) { y0 += sy; fy += f; }
      if (pxy == &xy && dx < 0 && dy > 0) pxy = &EP;
    }
exit:
    xx = x0; x0 = x3; x3 = xx; sx = -sx; xb = -xb;
    yy = y0; y0 = y3; y3 = yy; sy = -sy; yb = -yb; x1 = x2;
  } while (leg--);
  drawLine(x0,y0, x3,y3,AColor);
}

//----------------------------------------------------------------------
//
// anti-aliased
//
//----------------------------------------------------------------------

/* draw a black (0) anti-aliased line on white (255) background */

void drawLineAA(int x0, int y0, int x1, int y1, KODE_Color AColor) {
  uint32_t color = KODE_BGRA(AColor);
  int sx = x0 < x1 ? 1 : -1, sy = y0 < y1 ? 1 : -1, x2;
  long dx = abs(x1-x0), dy = abs(y1-y0), err = dx*dx+dy*dy;
  long e2 = err == 0 ? 1 : 0xffff7fl/sqrt(err);
  dx *= e2; dy *= e2; err = dx-dy;
  for (;;) {
    blendPixel(x0,y0,color,255-(abs(err-dx+dy)>>16));
    e2 = err; x2 = x0;
    if (2*e2 >= -dx) {
      if (x0 == x1) break;
      if (e2+dy < 0xff0000l) blendPixel(x0,y0+sy,color,255-((e2+dy)>>16));
      err -= dy; x0 += sx;
    }
    if (2*e2 <= dy) {
      if (y0 == y1) break;
      if (dx-e2 < 0xff0000l) blendPixel(x2+sx,y0,color,255-((dx-e2)>>16));
      err += dx; y0 += sy;
    }
  }
}

//----------

/* draw an anti-aliased line of width wd */

void drawLineWidthAA(int x0, int y0, int x1, int y1, float wd, KODE_Color AColor) {
  uint32_t color = KODE_BGRA(AColor);
  int dx = abs(x1-x0), sx = x0 < x1 ? 1 : -1;
  int dy = abs(y1-y0), sy = y0 < y1 ? 1 : -1;
  int err = dx-dy, e2, x2, y2;
  float ed = dx+dy == 0 ? 1 : sqrt((float)dx*dx+(float)dy*dy);
  for (wd = (wd+1)/2; ; ) {
    blendPixel/*Color*/(x0, y0, color, 255-KODE_Max(0,255*int(abs(err-dx+dy)/ed-wd+1)) );
    e2 = err; x2 = x0;
    if (2*e2 >= -dx) {
      for (e2 += dy, y2 = y0; e2 < ed*wd && (y1 != y2 || dx > dy); e2 += dx)
        blendPixel/*Color*/(x0, y2 += sy, color, 255-KODE_Max(0,255*int(abs(e2)/ed-wd+1)) );
      if (x0 == x1) break;
      e2 = err; err -= dy; x0 += sx;
    }
    if (2*e2 <= dy) {
      for (e2 = dx-e2; e2 < ed*wd && (x1 != x2 || dx < dy); e2 += dy)
        blendPixel/*Color*/(x2 += sx, y0, color, 255-KODE_Max(0,255*int(abs(e2)/ed-wd+1)) );
      if (y0 == y1) break;
      err += dx; y0 += sy;
    }
  }
}

//----------

/* draw a black anti-aliased circle on white background */

void drawCircleAA(int xm, int ym, int r, KODE_Color AColor) {
  uint32_t color = KODE_BGRA(AColor);
  int x = -r, y = 0;
  int i, x2, e2, err = 2-2*r;
  r = 1-err;
  do {
    i = 255*abs(err-2*(x+y)-2)/r;
    blendPixel(xm-x, ym+y,color, 255-i);
    blendPixel(xm-y, ym-x,color, 255-i);
    blendPixel(xm+x, ym-y,color, 255-i);
    blendPixel(xm+y, ym+x,color, 255-i);
    e2 = err; x2 = x;
    if (err+y > 0) {
      i = 255*(err-2*x-1)/r;
      if (i < 256) {
        blendPixel(xm-x, ym+y+1, color,255-i);
        blendPixel(xm-y-1, ym-x, color,255-i);
        blendPixel(xm+x, ym-y-1, color,255-i);
        blendPixel(xm+y+1, ym+x, color,255-i);
      }
      err += ++x*2+1;
    }
    if (e2+x2 <= 0) {
      i = 255*(2*y+3-e2)/r;
      if (i < 256) {
        blendPixel(xm-x2-1, ym+y, color, 255-i);
        blendPixel(xm-y, ym-x2-1, color, 255-i);
        blendPixel(xm+x2+1, ym-y, color, 255-i);
        blendPixel(xm+y, ym+x2+1, color, 255-i);
      }
      err += ++y*2+1;
    }
  } while (x < 0);
}

//----------

/* draw a black anti-aliased rectangular ellipse on white background */

void drawEllipseRectAA(int x0, int y0, int x1, int y1, KODE_Color AColor) {
  uint32_t color = KODE_BGRA(AColor);
  long a = abs(x1-x0), b = abs(y1-y0), b1 = b&1;
  float dx = 4*(a-1.0)*b*b, dy = 4*(b1+1)*a*a;
  float ed, i, err = b1*a*a-dx+dy;
  bool f;
  if (a == 0 || b == 0) return drawLine(x0,y0, x1,y1,AColor);
  if (x0 > x1) { x0 = x1; x1 += a; }
  if (y0 > y1) y0 = y1;
  y0 += (b+1)/2; y1 = y0-b1;
  a = 8*a*a; b1 = 8*b*b;
  for (;;) {
    i = KODE_Min(dx,dy); ed = KODE_Max(dx,dy);
    if (y0 == y1+1 && err > dy && a > b1) ed = 255*4./a;
    else ed = 255/(ed+2*ed*i*i/(4*ed*ed+i*i));
    i = ed*fabs(err+dx-dy);
    blendPixel(x0,y0, color,255-i);
    blendPixel(x0,y1, color,255-i);
    blendPixel(x1,y0, color,255-i);
    blendPixel(x1,y1, color,255-i);
    // warning: comparison of constant '0' with boolean expression is always true
    //SKEI
    //if ( ( f = 2*err+dy ) >= 0 ) {
    f = 2*err+dy;
    if (f) {
      //f = 2*err+dy;
      //if (f) {
      if (x0 >= x1) break;
      i = ed*(err+dx);
      if (i < 255) {
        blendPixel(x0,y0+1, color,255-i);
        blendPixel(x0,y1-1, color,255-i);
        blendPixel(x1,y0+1, color,255-i);
        blendPixel(x1,y1-1, color,255-i);
      }
    }
    if (2*err <= dx) {
      i = ed*(dy-err);
      if (i < 255) {
        blendPixel(x0+1,y0, color,255-i);
        blendPixel(x1-1,y0, color,255-i);
        blendPixel(x0+1,y1, color,255-i);
        blendPixel(x1-1,y1, color,255-i);
      }
      y0++; y1--; err += dy += a;
    }
    if (f) { x0++; x1--; err -= dx -= b1; }
  }
  if (--x0 == x1++)
    while (y0-y1 < b) {
      i = 255*4*fabs(err+dx)/b1;
      blendPixel(x0,++y0,color,255-i);
      blendPixel(x1,y0, color,255-i);
      blendPixel(x0,--y1,color,255-i);
      blendPixel(x1,y1, color,255-i);
      err += dy += a;
    }
}

//----------

/* draw an limited anti-aliased quadratic Bezier segment */

void drawQuadBezierSegAA(int x0, int y0, int x1, int y1, int x2, int y2, KODE_Color AColor) {
  uint32_t color = KODE_BGRA(AColor);
  int sx = x2-x1, sy = y2-y1;
  long xx = x0-x1, yy = y0-y1, xy;
  double dx, dy, err, ed, cur = xx*sy-yy*sx;
  //assert(xx*sx <= 0 && yy*sy <= 0);      /* sign of gradient must not change */
  //KODE_Assert( (xx*sx <= 0) && (yy*sy <= 0) );
  if (sx*(long)sx+sy*(long)sy > xx*xx+yy*yy) {
    x2 = x0; x0 = sx+x1; y2 = y0; y0 = sy+y1; cur = -cur;
  }
  if (cur != 0)
  {
    xx += sx; xx *= sx = x0 < x2 ? 1 : -1;
    yy += sy; yy *= sy = y0 < y2 ? 1 : -1;
    xy = 2*xx*yy; xx *= xx; yy *= yy;
    if (cur*sx*sy < 0) {
      xx = -xx; yy = -yy; xy = -xy; cur = -cur;
    }
    dx = 4.0*sy*(x1-x0)*cur+xx-xy;
    dy = 4.0*sx*(y0-y1)*cur+yy-xy;
    xx += xx; yy += yy; err = dx+dy+xy;
    do {
      cur = fmin(dx+xy,-xy-dy);
      ed = fmax(dx+xy,-xy-dy);
      ed += 2*ed*cur*cur/(4*ed*ed+cur*cur);
      blendPixel(x0,y0, color,255-(255*fabs(err-dx-dy-xy)/ed));
      if (x0 == x2 || y0 == y2) break;
      x1 = x0; cur = dx-err; y1 = 2*err+dy < 0;
      if (2*err+dx > 0) {
        if (err-dy < ed) blendPixel(x0,y0+sy, color, 255-(255*fabs(err-dy)/ed));
          x0 += sx; dx -= xy; err += dy += yy;
      }
      if (y1) {
        if (cur < ed) blendPixel(x1+sx,y0, color,255-(255*fabs(cur)/ed));
        y0 += sy; dy -= xy; err += dx += xx;
      }
    } while (dy < dx);
  }
  drawLineAA(x0,y0, x2,y2,AColor);
}

//----------

/* draw an anti-aliased rational quadratic Bezier segment, squared weight */

void drawQuadRationalBezierSegAA(int x0, int y0, int x1, int y1, int x2, int y2, float w, KODE_Color AColor) {
   uint32_t color = KODE_BGRA(AColor);
   int sx = x2-x1, sy = y2-y1;
   double dx = x0-x2, dy = y0-y2, xx = x0-x1, yy = y0-y1;
   double xy = xx*sy+yy*sx, cur = xx*sy-yy*sx, err, ed;
   bool f;
   //assert(xx*sx <= 0.0 && yy*sy <= 0.0);  /* sign of gradient must not change */
   //KODE_Assert( (xx*sx <= 0.0) && (yy*sy <= 0.0) );
   if (cur != 0.0 && w > 0.0) {
      if (sx*(long)sx+sy*(long)sy > xx*xx+yy*yy) {
         x2 = x0; x0 -= dx; y2 = y0; y0 -= dy; cur = -cur;
      }
      xx = 2.0*(4.0*w*sx*xx+dx*dx);
      yy = 2.0*(4.0*w*sy*yy+dy*dy);
      sx = x0 < x2 ? 1 : -1;
      sy = y0 < y2 ? 1 : -1;
      xy = -2.0*sx*sy*(2.0*w*xy+dx*dy);
      if (cur*sx*sy < 0) {
         xx = -xx; yy = -yy; cur = -cur; xy = -xy;
      }
      dx = 4.0*w*(x1-x0)*sy*cur+xx/2.0+xy;
      dy = 4.0*w*(y0-y1)*sx*cur+yy/2.0+xy;
      if (w < 0.5 && dy > dx) {
         cur = (w+1.0)/2.0; w = sqrt(w); xy = 1.0/(w+1.0);
         sx = floor((x0+2.0*w*x1+x2)*xy/2.0+0.5);
         sy = floor((y0+2.0*w*y1+y2)*xy/2.0+0.5);
         dx = floor((w*x1+x0)*xy+0.5); dy = floor((y1*w+y0)*xy+0.5);
         drawQuadRationalBezierSegAA(x0,y0, dx,dy, sx,sy, cur,AColor);
         dx = floor((w*x1+x2)*xy+0.5); dy = floor((y1*w+y2)*xy+0.5);
         return drawQuadRationalBezierSegAA(sx,sy, dx,dy, x2,y2, cur,AColor);
      }
      err = dx+dy-xy;
      do {
         cur = fmin(dx-xy,xy-dy); ed = fmax(dx-xy,xy-dy);
         ed += 2*ed*cur*cur/(4.*ed*ed+cur*cur);
         x1 = 255*fabs(err-dx-dy+xy)/ed;
         if (x1 < 256) blendPixel(x0,y0, color,255-x1);
         //SKEI
         //if ( (f = 2*err+dy) < 0) {
         f = 2*err+dy;
         if (!f) {
            if (y0 == y2) return;
            if (dx-err < ed) blendPixel(x0+sx,y0, color,255-(255*fabs(dx-err)/ed));
         }
         if (2*err+dx > 0) {
            if (x0 == x2) return;
            if (err-dy < ed) blendPixel(x0,y0+sy, color,255-(255*fabs(err-dy)/ed));
            x0 += sx; dx += xy; err += dy += yy;
         }
         if (f) { y0 += sy; dy += xy; err += dx += xx; }
      } while (dy < dx);
   }
   drawLineAA(x0,y0, x2,y2,AColor);
}

//----------

/* draw limited anti-aliased cubic Bezier segment */

void drawCubicBezierSegAA(int x0, int y0, float x1, float y1, float x2, float y2, int x3, int y3, KODE_Color AColor) {
  uint32_t color = KODE_BGRA(AColor);
  int f, fx, fy, leg = 1;
  int sx = x0 < x3 ? 1 : -1, sy = y0 < y3 ? 1 : -1;        /* step direction */
  float xc = -fabs(x0+x1-x2-x3), xa = xc-4*sx*(x1-x2), xb = sx*(x0-x1-x2+x3);
  float yc = -fabs(y0+y1-y2-y3), ya = yc-4*sy*(y1-y2), yb = sy*(y0-y1-y2+y3);
  double ab, ac, bc, ba, xx, xy, yy, dx, dy, ex, px, py, ed, ip, EP = 0.01;
  /* check for curve restrains */
  /* slope P0-P1 == P2-P3     and  (P0-P3 == P1-P2      or  no slope change) */
  //KODE_Assert((x1-x0)*(x2-x3) < EP && ((x3-x0)*(x1-x2) < EP || xb*xb < xa*xc+EP));
  //KODE_Assert((y1-y0)*(y2-y3) < EP && ((y3-y0)*(y1-y2) < EP || yb*yb < ya*yc+EP));
   if (xa == 0 && ya == 0) {
      sx = floor((3*x1-x0+1)/2); sy = floor((3*y1-y0+1)/2);
      return drawQuadBezierSegAA(x0,y0, sx,sy, x3,y3,AColor);
   }
   x1 = (x1-x0)*(x1-x0)+(y1-y0)*(y1-y0)+1;
   x2 = (x2-x3)*(x2-x3)+(y2-y3)*(y2-y3)+1;
   do {
      ab = xa*yb-xb*ya; ac = xa*yc-xc*ya; bc = xb*yc-xc*yb;
      ip = 4*ab*bc-ac*ac;
      ex = ab*(ab+ac-3*bc)+ac*ac;
      f = ex > 0 ? 1 : sqrt(1+1024/x1);
      ab *= f; ac *= f; bc *= f; ex *= f*f;
      xy = 9*(ab+ac+bc)/8; ba = 8*(xa-ya);
      dx = 27*(8*ab*(yb*yb-ya*yc)+ex*(ya+2*yb+yc))/64-ya*ya*(xy-ya);
      dy = 27*(8*ab*(xb*xb-xa*xc)-ex*(xa+2*xb+xc))/64-xa*xa*(xy+xa);
      xx = 3*(3*ab*(3*yb*yb-ya*ya-2*ya*yc)-ya*(3*ac*(ya+yb)+ya*ba))/4;
      yy = 3*(3*ab*(3*xb*xb-xa*xa-2*xa*xc)-xa*(3*ac*(xa+xb)+xa*ba))/4;
      xy = xa*ya*(6*ab+6*ac-3*bc+ba); ac = ya*ya; ba = xa*xa;
      xy = 3*(xy+9*f*(ba*yb*yc-xb*xc*ac)-18*xb*yb*ab)/8;
      if (ex < 0) {
         dx = -dx; dy = -dy; xx = -xx; yy = -yy; xy = -xy; ac = -ac; ba = -ba;
      }
      ab = 6*ya*ac; ac = -6*xa*ac; bc = 6*ya*ba; ba = -6*xa*ba;
      dx += xy; ex = dx+dy; dy += xy;
      for (fx = fy = f; x0 != x3 && y0 != y3; ) {
         y1 = fmin(fabs(xy-dx),fabs(dy-xy));
         ed = fmax(fabs(xy-dx),fabs(dy-xy));
         ed = f*(ed+2*ed*y1*y1/(4*ed*ed+y1*y1));
         y1 = 255*fabs(ex-(f-fx+1)*dx-(f-fy+1)*dy+f*xy)/ed;
         if (y1 < 256) blendPixel(x0, y0, color,255-y1);
         px = fabs(ex-(f-fx+1)*dx+(fy-1)*dy);
         py = fabs(ex+(fx-1)*dx-(f-fy+1)*dy);
         y2 = y0;
         do {
            if (ip >= -EP)
               if (dx+xx > xy || dy+yy < xy) goto exit;
            y1 = 2*ex+dx;
            if (2*ex+dy > 0) {
               fx--; ex += dx += xx; dy += xy += ac; yy += bc; xx += ab;
            } else if (y1 > 0) goto exit;
            if (y1 <= 0) {
               fy--; ex += dy += yy; dx += xy += bc; xx += ac; yy += ba;
            }
         } while (fx > 0 && fy > 0);
         if (2*fy <= f) {
            if (py < ed) blendPixel(x0+sx, y0, color, 255-(255*py/ed));
            y0 += sy; fy += f;
         }
         if (2*fx <= f) {
            if (px < ed) blendPixel(x0, y2+sy, color, 255-(255*px/ed));
            x0 += sx; fx += f;
         }
      }
      break;
exit:
      if (2*ex < dy && 2*fy <= f+2) {
         if (py < ed) blendPixel(x0+sx, y0, color, 255-(255*py/ed));
         y0 += sy;
      }
      if (2*ex > dx && 2*fx <= f+2) {
         if (px < ed) blendPixel(x0, y2+sy, color, 255-(255*px/ed));
         x0 += sx;
      }
      xx = x0; x0 = x3; x3 = xx; sx = -sx; xb = -xb;
      yy = y0; y0 = y3; y3 = yy; sy = -sy; yb = -yb; x1 = x2;
   } while (leg--);
   drawLineAA(x0,y0, x3,y3,AColor);
}

//----------------------------------------------------------------------
//
// rational / ellipse
//
//----------------------------------------------------------------------

/* rectangle enclosing the ellipse, integer rotation angle */

void drawRotatedEllipseRectAA(int x0, int y0, int x1, int y1, long zd, KODE_Color AColor) {
  //uint32_t color = KODE_BGRA(AColor);
  int xd = x1-x0, yd = y1-y0;
  float w = xd*(long)yd;
  if (zd == 0) return drawEllipseRectAA(x0,y0, x1,y1,AColor);
  if (w != 0.0) w = (w-zd)/(w+w);
  //assert(w <= 1.0 && w >= 0.0); /* limit angle to |zd|<=xd*yd */
  //KODE_Assert( (w <= 1.0) && (w >= 0.0) );
  xd = floor(xd*w+0.5); yd = floor(yd*w+0.5);
  drawQuadRationalBezierSegAA(x0,y0+yd, x0,y0, x0+xd,y0, 1.0-w,AColor);
  drawQuadRationalBezierSegAA(x0,y0+yd, x0,y1, x1-xd,y1, w,AColor);
  drawQuadRationalBezierSegAA(x1,y1-yd, x1,y1, x1-xd,y1, 1.0-w,AColor);
  drawQuadRationalBezierSegAA(x1,y1-yd, x1,y0, x0+xd,y0, w,AColor);
}

//----------

/* draw ellipse rotated by angle (radian) */

void drawRotatedEllipseAA(int x, int y, int a, int b, float angle, KODE_Color AColor) {
  //uint32_t color = KODE_BGRA(AColor);
  float xd = (long)a*a, yd = (long)b*b;
  float s = sin(angle), zd = (xd-yd)*s;
  xd = sqrt(xd-zd*s), yd = sqrt(yd+zd*s);
  a = xd+0.5; b = yd+0.5; zd = zd*a*b/(xd*yd);
  drawRotatedEllipseRectAA(x-a,y-b, x+a,y+b, (long)(4*zd*cos(angle)),AColor);
}

//----------

/* draw any quadratic rational Bezier curve */

void drawQuadRationalBezierAA(int x0, int y0, int x1, int y1, int x2, int y2, float w, KODE_Color AColor) {
  int x = x0-2*x1+x2, y = y0-2*y1+y2;
  double xx = x0-x1, yy = y0-y1, ww, t, q;
  //KODE_Assert(w >= 0.0);
  if (xx*(x2-x1) > 0) {
    if (yy*(y2-y1) > 0)
      if (fabs(xx*y) > fabs(yy*x)) {
        x0 = x2; x2 = xx+x1; y0 = y2; y2 = yy+y1;
      }
    if (x0 == x2 || w == 1.0) t = (x0-x1)/(double)x;
    else {
      q = sqrt(4.0*w*w*(x0-x1)*(x2-x1)+(x2-x0)*(long)(x2-x0));
      if (x1 < x0) q = -q;
      t = (2.0*w*(x0-x1)-x0+x2+q)/(2.0*(1.0-w)*(x2-x0));
    }
    q = 1.0/(2.0*t*(1.0-t)*(w-1.0)+1.0);
    xx = (t*t*(x0-2.0*w*x1+x2)+2.0*t*(w*x1-x0)+x0)*q;
    yy = (t*t*(y0-2.0*w*y1+y2)+2.0*t*(w*y1-y0)+y0)*q;
    ww = t*(w-1.0)+1.0; ww *= ww*q;
    w = ((1.0-t)*(w-1.0)+1.0)*sqrt(q);
    x = floor(xx+0.5); y = floor(yy+0.5);
    yy = (xx-x0)*(y1-y0)/(x1-x0)+y0;
    //drawQuadRationalBezierSeg(x0,y0, x,floor(yy+0.5), x,y, ww, AColor);
    drawQuadRationalBezierSegAA(x0,y0, x,floor(yy+0.5), x,y, ww, AColor);
    yy = (xx-x2)*(y1-y2)/(x1-x2)+y2;
    y1 = floor(yy+0.5); x0 = x1 = x; y0 = y;
  }
  if ((y0-y1)*(long)(y2-y1) > 0) {
    if (y0 == y2 || w == 1.0) t = (y0-y1)/(y0-2.0*y1+y2);
    else {
      q = sqrt(4.0*w*w*(y0-y1)*(y2-y1)+(y2-y0)*(long)(y2-y0));
      if (y1 < y0) q = -q;
      t = (2.0*w*(y0-y1)-y0+y2+q)/(2.0*(1.0-w)*(y2-y0));
    }
    q = 1.0/(2.0*t*(1.0-t)*(w-1.0)+1.0);
    xx = (t*t*(x0-2.0*w*x1+x2)+2.0*t*(w*x1-x0)+x0)*q;
    yy = (t*t*(y0-2.0*w*y1+y2)+2.0*t*(w*y1-y0)+y0)*q;
    ww = t*(w-1.0)+1.0; ww *= ww*q;
    w = ((1.0-t)*(w-1.0)+1.0)*sqrt(q);
    x = floor(xx+0.5); y = floor(yy+0.5);
    xx = (x1-x0)*(yy-y0)/(y1-y0)+x0;
    //drawQuadRationalBezierSeg(x0,y0, floor(xx+0.5),y, x,y, ww, AColor);
    drawQuadRationalBezierSegAA(x0,y0, floor(xx+0.5),y, x,y, ww, AColor);
    xx = (x1-x2)*(yy-y2)/(y1-y2)+x2;
    x1 = floor(xx+0.5); x0 = x; y0 = y1 = y;
  }
  //drawQuadRationalBezierSeg(x0,y0, x1,y1, x2,y2, w*w, AColor);
  drawQuadRationalBezierSegAA(x0,y0, x1,y1, x2,y2, w*w, AColor);
}

//----------------------------------------------------------------------
//
// bezier
//
//----------------------------------------------------------------------

/* draw any quadratic Bezier curve */

void drawQuadBezierAA(int x0, int y0, int x1, int y1, int x2, int y2, KODE_Color AColor) {
  int x = x0-x1, y = y0-y1;
  double t = x0-2*x1+x2, r;
  if ((long)x*(x2-x1) > 0) {
    if ((long)y*(y2-y1) > 0)
      if (fabs((y0-2*y1+y2)/t*x) > abs(y)) {
        x0 = x2; x2 = x+x1; y0 = y2; y2 = y+y1;
      }
    t = (x0-x1)/t;
    r = (1-t)*((1-t)*y0+2.0*t*y1)+t*t*y2;
    t = (x0*x2-x1*x1)*t/(x0-x1);
    x = floor(t+0.5); y = floor(r+0.5);
    r = (y1-y0)*(t-x0)/(x1-x0)+y0;
    //drawQuadBezierSeg(x0,y0, x,floor(r+0.5), x,y,AColor);
    drawQuadBezierSegAA(x0,y0, x,floor(r+0.5), x,y,AColor);
    r = (y1-y2)*(t-x2)/(x1-x2)+y2;
    x0 = x1 = x; y0 = y; y1 = floor(r+0.5);
  }
  if ((long)(y0-y1)*(y2-y1) > 0) {
    t = y0-2*y1+y2; t = (y0-y1)/t;
    r = (1-t)*((1-t)*x0+2.0*t*x1)+t*t*x2;
    t = (y0*y2-y1*y1)*t/(y0-y1);
    x = floor(r+0.5); y = floor(t+0.5);
    r = (x1-x0)*(t-y0)/(y1-y0)+x0;
    //drawQuadBezierSeg(x0,y0, floor(r+0.5),y, x,y,AColor);
    drawQuadBezierSegAA(x0,y0, floor(r+0.5),y, x,y,AColor);
    r = (x1-x2)*(t-y2)/(y1-y2)+x2;
    x0 = x; x1 = floor(r+0.5); y0 = y1 = y;
  }
  //drawQuadBezierSeg(x0,y0, x1,y1, x2,y2,AColor);
  drawQuadBezierSegAA(x0,y0, x1,y1, x2,y2,AColor);
}

//----------

/* draw any cubic Bezier curve */

void drawCubicBezierAA(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, KODE_Color AColor) {
  int n = 0, i = 0;
  long xc = x0+x1-x2-x3, xa = xc-4*(x1-x2);
  long xb = x0-x1-x2+x3, xd = xb+4*(x1+x2);
  long yc = y0+y1-y2-y3, ya = yc-4*(y1-y2);
  long yb = y0-y1-y2+y3, yd = yb+4*(y1+y2);
  float fx0 = x0, fx1, fx2, fx3, fy0 = y0, fy1, fy2, fy3;
  double t1 = xb*xb-xa*xc, t2, t[5];
  if (xa == 0) {
    if (abs(xc) < 2*abs(xb)) t[n++] = xc/(2.0*xb);
  }
  else if (t1 > 0.0) {
    t2 = sqrt(t1);
    t1 = (xb-t2)/xa; if (fabs(t1) < 1.0) t[n++] = t1;
    t1 = (xb+t2)/xa; if (fabs(t1) < 1.0) t[n++] = t1;
  }
  t1 = yb*yb-ya*yc;
  if (ya == 0) {
    if (abs(yc) < 2*abs(yb)) t[n++] = yc/(2.0*yb);
  }
  else if (t1 > 0.0) {
    t2 = sqrt(t1);
    t1 = (yb-t2)/ya; if (fabs(t1) < 1.0) t[n++] = t1;
    t1 = (yb+t2)/ya; if (fabs(t1) < 1.0) t[n++] = t1;
  }
  for (i = 1; i < n; i++)
    if ((t1 = t[i-1]) > t[i]) { t[i-1] = t[i]; t[i] = t1; i = 0; }
  t1 = -1.0; t[n] = 1.0;
  for (i = 0; i <= n; i++) {
    t2 = t[i];
    fx1 = (t1*(t1*xb-2*xc)-t2*(t1*(t1*xa-2*xb)+xc)+xd)/8-fx0;
    fy1 = (t1*(t1*yb-2*yc)-t2*(t1*(t1*ya-2*yb)+yc)+yd)/8-fy0;
    fx2 = (t2*(t2*xb-2*xc)-t1*(t2*(t2*xa-2*xb)+xc)+xd)/8-fx0;
    fy2 = (t2*(t2*yb-2*yc)-t1*(t2*(t2*ya-2*yb)+yc)+yd)/8-fy0;
    fx0 -= fx3 = (t2*(t2*(3*xb-t2*xa)-3*xc)+xd)/8;
    fy0 -= fy3 = (t2*(t2*(3*yb-t2*ya)-3*yc)+yd)/8;
    x3 = floor(fx3+0.5); y3 = floor(fy3+0.5);
    if (fx0 != 0.0) { fx1 *= fx0 = (x0-x3)/fx0; fx2 *= fx0; }
    if (fy0 != 0.0) { fy1 *= fy0 = (y0-y3)/fy0; fy2 *= fy0; }
    if (x0 != x3 || y0 != y3)
      //drawCubicBezierSeg(x0,y0, x0+fx1,y0+fy1, x0+fx2,y0+fy2, x3,y3,AColor);
      drawCubicBezierSegAA(x0,y0, x0+fx1,y0+fy1, x0+fx2,y0+fy2, x3,y3,AColor);
    x0 = x3;
    y0 = y3;
    fx0 = fx3;
    fy0 = fy3;
    t1 = t2;
  }
}

//----------------------------------------------------------------------
//
// spline
//
//----------------------------------------------------------------------

/* draw quadratic spline, destroys input arrays x,y */

void drawQuadSplineAA(int n, int x[], int y[], KODE_Color AColor) {
  #define M_MAX 6
  float mi = 1;
  float m[M_MAX];
  int i, x0, y0, x1, y1;
  int x2 = x[n];
  int y2 = y[n];
  //KODE_Assert(n > 1); // need at least 3 points P[0]..P[n]
  x[1] = x0 = 8*x[1]-2*x[0];
  y[1] = y0 = 8*y[1]-2*y[0];
  for (i = 2; i < n; i++) {
    if (i-2 < M_MAX) m[i-2] = mi = 1.0/(6.0-mi);
    x[i] = x0 = floor(8*x[i]-x0*mi+0.5);
    y[i] = y0 = floor(8*y[i]-y0*mi+0.5);
  }
  x1 = floor((x0-2*x2)/(5.0-mi)+0.5);
  y1 = floor((y0-2*y2)/(5.0-mi)+0.5);
  for (i = n-2; i > 0; i--) {
    if (i <= M_MAX) mi = m[i-1];
    x0 = floor((x[i]-x1)*mi+0.5);
    y0 = floor((y[i]-y1)*mi+0.5);
    //drawQuadBezier((x0+x1)/2,(y0+y1)/2, x1,y1, x2,y2,AColor);
    drawQuadBezierAA((x0+x1)/2,(y0+y1)/2, x1,y1, x2,y2,AColor);
    x2 = (x0+x1)/2; x1 = x0;
    y2 = (y0+y1)/2; y1 = y0;
  }
  drawQuadBezierAA(x[0],y[0], x1,y1, x2,y2, AColor);
  #undef M_MAX
}

//----------

/* draw cubic spline, destroys input arrays x,y */

void drawCubicSplineAA(int n, int x[], int y[], KODE_Color AColor) {
  #define M_MAX 6
  float mi = 0.25;
  float m[M_MAX];
  int x3 = x[n-1];
  int y3 = y[n-1];
  int x4 = x[n];
  int y4 = y[n];
  int i, x0, y0, x1, y1, x2, y2;
  //KODE_Assert(n > 2); // need at least 4 points P[0]..P[n]
  x[1] = x0 = 12*x[1]-3*x[0];
  y[1] = y0 = 12*y[1]-3*y[0];
  for (i=2; i<n; i++) {
    if (i-2 < M_MAX) m[i-2] = mi = 0.25/(2.0-mi);
    x[i] = x0 = floor(12*x[i]-2*x0*mi+0.5);
    y[i] = y0 = floor(12*y[i]-2*y0*mi+0.5);
  }
  x2 = floor((x0-3*x4)/(7-4*mi)+0.5);
  y2 = floor((y0-3*y4)/(7-4*mi)+0.5);
  drawCubicBezierAA(x3,y3, (x2+x4)/2,(y2+y4)/2, x4,y4, x4,y4, AColor);
  if (n-3 < M_MAX) mi = m[n-3];
  x1 = floor((x[n-2]-2*x2)*mi+0.5);
  y1 = floor((y[n-2]-2*y2)*mi+0.5);
  for (i=n-3; i>0; i--) {
    if (i <= M_MAX) mi = m[i-1];
    x0 = floor((x[i]-2*x1)*mi+0.5);
    y0 = floor((y[i]-2*y1)*mi+0.5);
    x4 = floor((x0+4*x1+x2+3)/6.0);
    y4 = floor((y0+4*y1+y2+3)/6.0);
    drawCubicBezierAA(
      x4, y4,
      floor((2*x1+x2)/3+0.5), floor((2*y1+y2)/3+0.5),
      floor((x1+2*x2)/3+0.5), floor((y1+2*y2)/3+0.5),
      x3, y3,
      AColor
    );
    x3 = x4;
    y3 = y4;
    x2 = x1;
    y2 = y1;
    x1 = x0;
    y1 = y0;
   }
   x0 = x[0]; x4 = floor((3*x0+7*x1+2*x2+6)/12.0);
   y0 = y[0]; y4 = floor((3*y0+7*y1+2*y2+6)/12.0);
   drawCubicBezierAA(
     x4, y4,
     floor((2*x1+x2)/3+0.5), floor((2*y1+y2)/3+0.5),
     floor((x1+2*x2)/3+0.5), floor((y1+2*y2)/3+0.5),
     x3, y3,
     AColor
   );
   drawCubicBezierAA(x0,y0, x0,y0, (x0+x1)/2,(y0+y1)/2, x4,y4, AColor);
   #undef M_MAX
}

#endif // 0

////----------------------------------------------------------------------
//#endif
