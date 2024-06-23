//#ifndef kode_bitmap_wu_included
//#define kode_bitmap_wu_included
//----------------------------------------------------------------------

#if 0

#define ipart_(X) ((int)(X))
#define round_(X) ((int)(((double)(X))+0.5))
#define fpart_(X) (((double)(X))-(double)ipart_(X))
#define rfpart_(X) (1.0-fpart_(X))
#define swap_(a, b) do{ __typeof__(a) tmp;  tmp = a; a = b; b = tmp; }while(0)
  
void drawLineAA(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t AColor) {
  double dx = (double)x2 - (double)x1;
  double dy = (double)y2 - (double)y1;
  if ( fabs(dx) > fabs(dy) ) {
    if ( x2 < x1 ) {
      swap_(x1, x2);
      swap_(y1, y2);
    }
    double gradient = dy / dx;
    double xend = round_(x1);
    double yend = y1 + gradient*(xend - x1);
    double xgap = rfpart_(x1 + 0.5);
    int xpxl1 = xend;
    int ypxl1 = ipart_(yend);
    blendPixelF(xpxl1, ypxl1,   AColor, rfpart_(yend)*xgap);
    blendPixelF(xpxl1, ypxl1+1, AColor, fpart_( yend)*xgap);
    double intery = yend + gradient;
     xend = round_(x2);
    yend = y2 + gradient*(xend - x2);
    xgap = fpart_(x2+0.5);
    int xpxl2 = xend;
    int ypxl2 = ipart_(yend);
    blendPixelF(xpxl2, ypxl2,     AColor, rfpart_(yend) * xgap);
    blendPixelF(xpxl2, ypxl2 + 1, AColor, fpart_(yend)  * xgap);
    int x;
    for(x=xpxl1+1; x < xpxl2; x++) {
      blendPixelF(x, ipart_(intery),     AColor, rfpart_(intery));
      blendPixelF(x, ipart_(intery) + 1, AColor, fpart_(intery));
      intery += gradient;
    }
  } else {
    if ( y2 < y1 ) {
      swap_(x1, x2);
      swap_(y1, y2);
    }
    double gradient = dx / dy;
    double yend = round_(y1);
    double xend = x1 + gradient*(yend - y1);
    double ygap = rfpart_(y1 + 0.5);
    int ypxl1 = yend;
    int xpxl1 = ipart_(xend);
    blendPixelF(xpxl1,     ypxl1, AColor, rfpart_(xend)*ygap);
    blendPixelF(xpxl1 + 1, ypxl1, AColor, fpart_(xend)*ygap);
    double interx = xend + gradient;
 
    yend = round_(y2);
    xend = x2 + gradient*(yend - y2);
    ygap = fpart_(y2+0.5);
    int ypxl2 = yend;
    int xpxl2 = ipart_(xend);
    blendPixelF(xpxl2,     ypxl2, AColor, rfpart_(xend) * ygap);
    blendPixelF(xpxl2 + 1, ypxl2, AColor, fpart_(xend) * ygap);
 
    int y;
    for(y=ypxl1+1; y < ypxl2; y++) {
      blendPixelF(ipart_(interx),     y, AColor, rfpart_(interx));
      blendPixelF(ipart_(interx) + 1, y, AColor, fpart_(interx));
      interx += gradient;
    }
  }
}

#undef ipart_
#undef round_
#undef fpart_
#undef rfpart_
#undef swap_

//----------------------------------------------------------------------

// http://landkey.net/d/L/J/RF/WUCircle/Step2Optimized/S/WUCircle.java.htm

//A - amplitude (Intensity).
//r - radius.

//Distance to ceil:
//double DC(int r, int y){
//  double x = Math.sqrt(r*r-y*y);
//  return Math.ceil(x) - x;
//}

/*
void drawArchAA(int cx, int cy, int r, uint32_t AColor) {
  int   y = 0;
  int   x = r;
  float d = 0;
  setPixel(x,y,AColor);
  while (x > y) {
    y++;
    //int dc = DC(r,y);
    double dcx = sqrtf(r*r-y*y);
    int dc = ceilf(dcx) - dcx;
    
    if (dc < d) x--;
    //blendPixelF(cx+x,   cy+y, AColor, (1-dc));
    //blendPixelF(cx+x-1, cy+y, AColor,    dc );
    blendPixelF(cx+x,   cy+y, AColor, dc );
    blendPixelF(cx+x+1, cy+y, AColor, (1.0f-dc) );
    d = dc;
  }
}
*/ 

//----------

// todo: horiz lines = filled ellipses/circles..

void stroke4Pixels(int32_t cx, int32_t cy, int32_t x, int32_t y, uint32_t c, float f)  {
  uint8_t a = (f * 255.0f);
  if (a==0) return;
  //int32_t w = (x * 2) - 2;
  //int32_t h = (y * 2) - 2;
  //int32_t sx = (cx - x) + 1;
  //int32_t sy = (cy - y) + 1;
  //int32_t i;
  if ((x > 0) && (y > 0)) {
    
    // horizontal line
    blendPixel(cx-x, cy-y, c, a);
    blendPixel(cx+x, cy-y, c, a);
    //for (i=0; i<w; i++) setPixel(sx+i,cy-y,c);

    // horizontal line
    blendPixel(cx-x, cy+y, c, a);
    blendPixel(cx+x, cy+y, c, a);
    //for (i=0; i<w; i++) setPixel(sx+i,cy+y,c);
    
  }
  else if (x == 0) {
    // vertical
    blendPixel(cx+x, cy-y, c, a);
    blendPixel(cx+x, cy+y, c, a);
    //for (i=0; i<h; i++) setPixel(cx+x,sy+i,c);
  }
  else if (y == 0) {
    // horizontal
    blendPixel(cx-x, cy+y, c, a);
    blendPixel(cx+x, cy+y, c, a);
    //for (i=0; i<w; i++) setPixel(sx+i,cy+y,c);
  }
}

//----------------------------------------------------------------------

// https://yellowsplash.wordpress.com/

/*
  int32_t xi, yi; // the integer iterators. I could have used 'i' for both x and y but this was less confusing.
  float   xj, yj; // 'single' is a float type. this is the real-value y calculated from the integer x (and vice-versa)
  int32_t ffd;    // the forty-five degree coordinate: stop drawing and switch from horizontal to vertical mode.
  float   rsq;    // for convenience: radius squared
  float   frc;    // the fractional part of the calculated real. its inverse will also be used.
  int32_t flr;    // xj or yj rounded down (the "floor"). We'll use this twice, so give it a variable.
*/

/*
  assuming momentarily that r is an integer, let's find center_pix (r,r)
  first get the row: it is at r*width. Then to get the column, add r.
  Thus (r,r) is at r*width+r = r*(width+1) -- this holds true for any r
  r is real, so take ceil(r); also add some wiggle room, so add 1: ceil(r)+1
  Thus: (ceil(r)+1) * (width+1)
*/

void drawCircleAA(int32_t x, int32_t y, float r, uint32_t AColor) {
  if (r <= 0) return;
  float   rsq = r*r;
  int32_t ffd = round(r / sqrtf(2.0f));
  for (int32_t xi=0; xi<ffd; xi++) {
    float   yj  = sqrtf(rsq - (xi*xi));
    int32_t flr = floorf(yj);
    float   frc = yj- flr;
    stroke4Pixels( x,y, xi, flr,   AColor, 1-frc);
    stroke4Pixels( x,y, xi, flr+1, AColor, frc);
  }
  for (int32_t yi=0; yi<ffd; yi++) {
    float   xj  = sqrtf(rsq-(yi*yi));
    int32_t flr = floorf(xj);
    float   frc = xj - flr;
    stroke4Pixels( x,y, flr,   yi, AColor, 1-frc);
    stroke4Pixels( x,y, flr+1, yi, AColor, frc);
  }
}

//----------

/*
  xi, yi    : longint;
  xj, yj    : single;
  a, b      : single;   // the defining characteristics of an ellipse, width/2 and height/2
  asq, bsq  : single;   // a squared and b squared
  ffd       : longint;  // forty-five-degree coord
  frc       : single;   // frac, as with the circle
  flr       : longint;  // floor, as with the circle
*/

void drawEllipseAA(int32_t x, int32_t y, float ell_width, float ell_height, uint32_t AColor) {
  if ((ell_width <= 0) || (ell_height <= 0)) return;
  if (ell_width == ell_height) return;
  float    a   = ell_width / 2.0f;
  float    asq = a*a;
  float    b   = ell_height / 2.0f;
  float    bsq = b*b;
  uint32_t ffd = round(asq / sqrtf(bsq + asq));
  for (uint32_t xi=0; xi<ffd; xi++) {
    float    yj  = b*sqrtf(1-(xi*xi)/asq);
    uint32_t flr = floorf(yj);
    float    frc = yj - flr;
    stroke4Pixels(x,y, xi, flr,   AColor, 1-frc);
    stroke4Pixels(x,y, xi, flr+1, AColor, frc);
  }
  ffd = round(bsq / sqrt(bsq + asq));
  for (uint32_t yi=0; yi<ffd; yi++) {
    float    xj  = a * sqrt(1 - (yi*yi) / bsq);
    uint32_t flr = floorf(xj);
    float    frc = xj - flr;
    stroke4Pixels(x,y, flr,   yi, AColor, 1-frc);
    stroke4Pixels(x,y, flr+1, yi, AColor, frc);
  }
}

#endif // 0

//----------------------------------------------------------------------
//#endif
