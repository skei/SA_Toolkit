#ifndef kode_gfx_utils_included
#define kode_gfx_utils_included
//----------------------------------------------------------------------

/*
  http://www.freebasic.net/forum/viewtopic.php?f=7&t=24174

function CircleFrom3Points(byref a as v2, _
                           byref b as v2, _
                           byref c as v2, _
                           byref center as v2, _
                           byref radius as single) as boolean
  var bax = b.x-a.x : if bax=0 then return false
  var cbx = c.x-b.x : if cbx=0 then return false
  var abx = a.x+b.x, aby = a.y+b.y, bay = b.y-a.y
  var bcx = b.x+c.x, cay = c.y-a.y, cby = c.y-b.y
  var bayx = bay/bax, cbyx = cby/cbx, bacb = bayx-cbyx
  bacb+=bacb
  center.x = (bayx*cbyx*cay + bayx*bcx - cbyx*abx) / bacb
  abx*=.5 : aby*=.5
  center.y = aby - (center.x - abx) / bayx
  var dx = a.x-center.x
  var dy = a.y-center.y
  radius = sqr(dx*dx+dy*dy)
  return true
end function
*/

bool KODE_CircleFrom3Points(float ax, float ay, float bx, float by, float cx, float cy, float* centerx, float* centery, float* radius) {
  float bax = bx - ax;  if (bax==0) return false;
  float cbx = cx - bx;  if (cbx==0) return false;
  float abx = ax + bx;
  float aby = ay + by;
  float bay = by - ay;
  float bcx = bx + cx;
  float cay = cy - ay;
  float cby = cy - by;
  float bayx = bay / bax;
  float cbyx = cby / cbx;
  float bacb = bayx - cbyx;
  bacb += bacb;
  *centerx = (bayx*cbyx*cay + bayx*bcx - cbyx*abx) / bacb;
  abx *= 0.5;
  aby *= 0.5;
  *centery = aby - (*centerx - abx) / bayx;
  float dx = ax - *centerx;
  float dy = ay - *centery;
  *radius = sqrt(dx*dx+dy*dy);
  return true;
}

//----------------------------------------------------------------------
#endif