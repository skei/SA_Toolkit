#ifndef kode_rasterizer_included
#define kode_rasterizer_included
//----------------------------------------------------------------------

#include "base/kode.h"

//----------------------------------------------------------------------

//#ifdef _MSC_VER
//  #define STBTT__NOTUSED(v)  (void)(v)
//#else
//  #define STBTT__NOTUSED(v)  (void)sizeof(v)
//#endif

#include <stdlib.h>
#define STBTT_malloc(x,u)  ((void)(u),malloc(x))
#define STBTT_free(x,u)    ((void)(u),free(x))

#include <assert.h>
#define STBTT_assert(x)    assert(x)

#include <math.h>
#define STBTT_fabs(x)      fabs(x)
#define STBTT_sqrt(x)      sqrt(x)
#define STBTT_pow(x,y)     pow(x,y)

#include <string.h>
#define STBTT_memcpy       memcpy
#define STBTT_memset       memset
   
//----------

//----------
   
typedef struct {
  float x,y;
} stbtt__point;

//----------

typedef struct {
  int w,h,stride;
  // skei
  unsigned char *pixels;
  //uint32_t* pixels;
} stbtt__bitmap;

//----------

typedef struct stbtt__edge {
  float x0,y0, x1,y1;
  int invert;
} stbtt__edge;

//----------

// vertex types
enum {
  STBTT_vmove=1,
  STBTT_vline,
  STBTT_vcurve,
  STBTT_vcubic
};

// can't use stbtt_int16 because that's not visible in the header file
#define stbtt_vertex_type short

typedef struct {
  stbtt_vertex_type x,y,cx,cy,cx1,cy1;
  unsigned char type, padding;
} stbtt_vertex;

//----------

typedef struct stbtt__active_edge {
  struct stbtt__active_edge *next;
  float fx,fdx,fdy;
  float direction;
  float sy;
  float ey;
} stbtt__active_edge;

//----------

typedef struct stbtt__hheap_chunk {
  struct stbtt__hheap_chunk *next;
} stbtt__hheap_chunk;

//----------

typedef struct stbtt__hheap {
  struct stbtt__hheap_chunk *head;
  void   *first_free;
  int    num_remaining_in_head_chunk;
} stbtt__hheap;

//----------------------------------------------------------------------

void *stbtt__hheap_alloc(stbtt__hheap *hh, size_t size, void *userdata) {
  if (hh->first_free) {
    void *p = hh->first_free;
    hh->first_free = * (void **) p;
    return p;
  } else {
    if (hh->num_remaining_in_head_chunk == 0) {
      int count = (size < 32 ? 2000 : size < 128 ? 800 : 100);
      stbtt__hheap_chunk *c = (stbtt__hheap_chunk *) STBTT_malloc(sizeof(stbtt__hheap_chunk) + size * count, userdata);
      if (c == NULL)
        return NULL;
      c->next = hh->head;
      hh->head = c;
      hh->num_remaining_in_head_chunk = count;
    }
    --hh->num_remaining_in_head_chunk;
    return (char *) (hh->head) + sizeof(stbtt__hheap_chunk) + size * hh->num_remaining_in_head_chunk;
  }
}

//----------

void stbtt__hheap_free(stbtt__hheap *hh, void *p) {
  *(void **) p = hh->first_free;
  hh->first_free = p;
}

//----------

void stbtt__hheap_cleanup(stbtt__hheap *hh, void *userdata) {
  stbtt__hheap_chunk *c = hh->head;
  while (c) {
    stbtt__hheap_chunk *n = c->next;
    STBTT_free(c, userdata);
    c = n;
  }
}

//----------

stbtt__active_edge *stbtt__new_active(stbtt__hheap *hh, stbtt__edge *e, int off_x, float start_point, void *userdata) {
  stbtt__active_edge *z = (stbtt__active_edge *) stbtt__hheap_alloc(hh, sizeof(*z), userdata);
  float dxdy = (e->x1 - e->x0) / (e->y1 - e->y0);
  STBTT_assert(z != NULL);
  //STBTT_assert(e->y0 <= start_point);
  if (!z) return z;
  z->fdx = dxdy;
  z->fdy = dxdy != 0.0f ? (1.0f/dxdy) : 0.0f;
  z->fx = e->x0 + dxdy * (start_point - e->y0);
  z->fx -= off_x;
  z->direction = e->invert ? 1.0f : -1.0f;
  z->sy = e->y0;
  z->ey = e->y1;
  z->next = 0;
  return z;
}

//----------------------------------------------------------------------
// sort
//----------------------------------------------------------------------

#define STBTT__COMPARE(a,b)  ((a)->y0 < (b)->y0)

void stbtt__sort_edges_ins_sort(stbtt__edge *p, int n) {
  int i,j;
  for (i=1; i < n; ++i) {
    stbtt__edge t = p[i], *a = &t;
    j = i;
    while (j > 0) {
      stbtt__edge *b = &p[j-1];
      int c = STBTT__COMPARE(a,b);
      if (!c) break;
      p[j] = p[j-1];
      --j;
    }
    if (i != j)
      p[j] = t;
  }
}

//----------

void stbtt__sort_edges_quicksort(stbtt__edge *p, int n) {
  /* threshold for transitioning to insertion sort */
  while (n > 12) {
    stbtt__edge t;
    int c01,c12,c,m,i,j;
    /* compute median of three */
    m = n >> 1;
    c01 = STBTT__COMPARE(&p[0],&p[m]);
    c12 = STBTT__COMPARE(&p[m],&p[n-1]);
    /* if 0 >= mid >= end, or 0 < mid < end, then use mid */
    if (c01 != c12) {
      /* otherwise, we'll need to swap something else to middle */
      int z;
      c = STBTT__COMPARE(&p[0],&p[n-1]);
      /* 0>mid && mid<n:  0>n => n; 0<n => 0 */
      /* 0<mid && mid>n:  0>n => 0; 0<n => n */
      z = (c == c12) ? 0 : n-1;
      t = p[z];
      p[z] = p[m];
      p[m] = t;
    }
    /* now p[m] is the median-of-three */
    /* swap it to the beginning so it won't move around */
    t = p[0];
    p[0] = p[m];
    p[m] = t;
    /* partition loop */
    i=1;
    j=n-1;
    for(;;) {
      /* handling of equality is crucial here */
      /* for sentinels & efficiency with duplicates */
      for (;;++i) {
        if (!STBTT__COMPARE(&p[i], &p[0])) break;
      }
      for (;;--j) {
        if (!STBTT__COMPARE(&p[0], &p[j])) break;
      }
      /* make sure we haven't crossed */
      if (i >= j) break;
      t = p[i];
      p[i] = p[j];
      p[j] = t;
      ++i;
      --j;
    }
    /* recurse on smaller side, iterate on larger */
    if (j < (n-i)) {
      stbtt__sort_edges_quicksort(p,j);
      p = p+i;
      n = n-i;
    } else {
      stbtt__sort_edges_quicksort(p+i, n-i);
      n = j;
    }
  }
}

//----------

void stbtt__sort_edges(stbtt__edge *p, int n) {
  stbtt__sort_edges_quicksort(p, n);
  stbtt__sort_edges_ins_sort(p, n);
}

//----------------------------------------------------------------------
// curves
//----------------------------------------------------------------------

void stbtt__add_point(stbtt__point *points, int n, float x, float y) {
  if (!points) return; // during first pass, it's unallocated
  points[n].x = x;
  points[n].y = y;
}

//----------

// tessellate until threshold p is happy...
// TODO warped to compensate for non-linear stretching

int stbtt__tesselate_curve(stbtt__point *points, int *num_points, float x0, float y0, float x1, float y1, float x2, float y2, float objspace_flatness_squared, int n) {
  // midpoint
  float mx = (x0 + 2*x1 + x2)/4;
  float my = (y0 + 2*y1 + y2)/4;
  // versus directly drawn line
  float dx = (x0+x2)/2 - mx;
  float dy = (y0+y2)/2 - my;
  if (n > 16) // 65536 segments on one curve better be enough!
    return 1;
  if (dx*dx+dy*dy > objspace_flatness_squared) { // half-pixel error allowed... need to be smaller if AA
    stbtt__tesselate_curve(points, num_points, x0,y0, (x0+x1)/2.0f,(y0+y1)/2.0f, mx,my, objspace_flatness_squared,n+1);
    stbtt__tesselate_curve(points, num_points, mx,my, (x1+x2)/2.0f,(y1+y2)/2.0f, x2,y2, objspace_flatness_squared,n+1);
  } else {
    stbtt__add_point(points, *num_points,x2,y2);
    *num_points = *num_points+1;
  }
  return 1;
}

//----------

void stbtt__tesselate_cubic(stbtt__point *points, int *num_points, float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, float objspace_flatness_squared, int n) {
  // @TODO this "flatness" calculation is just made-up nonsense that seems to work well enough
  float dx0 = x1-x0;
  float dy0 = y1-y0;
  float dx1 = x2-x1;
  float dy1 = y2-y1;
  float dx2 = x3-x2;
  float dy2 = y3-y2;
  float dx = x3-x0;
  float dy = y3-y0;
  float longlen = (float) (STBTT_sqrt(dx0*dx0+dy0*dy0)+STBTT_sqrt(dx1*dx1+dy1*dy1)+STBTT_sqrt(dx2*dx2+dy2*dy2));
  float shortlen = (float) STBTT_sqrt(dx*dx+dy*dy);
  float flatness_squared = longlen*longlen-shortlen*shortlen;
  if (n > 16) // 65536 segments on one curve better be enough!
    return;
  if (flatness_squared > objspace_flatness_squared) {
    float x01 = (x0+x1)/2;
    float y01 = (y0+y1)/2;
    float x12 = (x1+x2)/2;
    float y12 = (y1+y2)/2;
    float x23 = (x2+x3)/2;
    float y23 = (y2+y3)/2;
    float xa = (x01+x12)/2;
    float ya = (y01+y12)/2;
    float xb = (x12+x23)/2;
    float yb = (y12+y23)/2;
    float mx = (xa+xb)/2;
    float my = (ya+yb)/2;
    stbtt__tesselate_cubic(points, num_points, x0,y0, x01,y01, xa,ya, mx,my, objspace_flatness_squared,n+1);
    stbtt__tesselate_cubic(points, num_points, mx,my, xb,yb, x23,y23, x3,y3, objspace_flatness_squared,n+1);
  } else {
    stbtt__add_point(points, *num_points,x3,y3);
    *num_points = *num_points+1;
  }
}

//----------

// returns number of contours

stbtt__point *stbtt_FlattenCurves(stbtt_vertex *vertices, int num_verts, float objspace_flatness, int** contour_lengths, int* num_contours, void* userdata) {
  stbtt__point *points=0;
  int num_points=0;
  float objspace_flatness_squared = objspace_flatness * objspace_flatness;
  int i,n=0,start=0, pass;
  // count how many "moves" there are to get the contour count
  for (i=0; i < num_verts; ++i)
    if (vertices[i].type == STBTT_vmove)
      ++n;
  *num_contours = n;
  if (n == 0) return 0;
  *contour_lengths = (int *) STBTT_malloc(sizeof(**contour_lengths) * n, userdata);
  if (*contour_lengths == 0) {
    *num_contours = 0;
    return 0;
  }
  // make two passes through the points so we don't need to realloc
  for (pass=0; pass < 2; ++pass) {
    float x=0,y=0;
    if (pass == 1) {
      points = (stbtt__point *) STBTT_malloc(num_points * sizeof(points[0]), userdata);
      if (points == NULL) goto error;
    }
    num_points = 0;
    n= -1;
    for (i=0; i < num_verts; ++i) {
      switch (vertices[i].type) {
        case STBTT_vmove:
          // start the next contour
          if (n >= 0)
            (*contour_lengths)[n] = num_points - start;
          ++n;
          start = num_points;
          x = vertices[i].x, y = vertices[i].y;
          stbtt__add_point(points, num_points++, x,y);
          break;
        case STBTT_vline:
          x = vertices[i].x, y = vertices[i].y;
          stbtt__add_point(points, num_points++, x, y);
          break;
        case STBTT_vcurve:
          stbtt__tesselate_curve(points, &num_points, x,y,
                                 vertices[i].cx, vertices[i].cy,
                                 vertices[i].x,  vertices[i].y,
                                 objspace_flatness_squared, 0);
          x = vertices[i].x, y = vertices[i].y;
          break;
        case STBTT_vcubic:
          stbtt__tesselate_cubic(points, &num_points, x,y,
                                 vertices[i].cx, vertices[i].cy,
                                 vertices[i].cx1, vertices[i].cy1,
                                 vertices[i].x,  vertices[i].y,
                                 objspace_flatness_squared, 0);
          x = vertices[i].x, y = vertices[i].y;
          break;
      }
    }
    (*contour_lengths)[n] = num_points - start;
  }
  return points;
error:
  STBTT_free(points, userdata);
  STBTT_free(*contour_lengths, userdata);
  *contour_lengths = 0;
  *num_contours = 0;
  return NULL;
}

//----------------------------------------------------------------------
// rasterize
//----------------------------------------------------------------------

// the edge passed in here does not cross the vertical line at x or the vertical line at x+1
// (i.e. it has already been clipped to those)

void stbtt__handle_clipped_edge(float *scanline, int x, stbtt__active_edge *e, float x0, float y0, float x1, float y1) {
  if (y0 == y1) return;
  STBTT_assert(y0 < y1);
  STBTT_assert(e->sy <= e->ey);
  if (y0 > e->ey) return;
  if (y1 < e->sy) return;
  if (y0 < e->sy) {
    x0 += (x1-x0) * (e->sy - y0) / (y1-y0);
    y0 = e->sy;
  }
  if (y1 > e->ey) {
    x1 += (x1-x0) * (e->ey - y1) / (y1-y0);
    y1 = e->ey;
  }
  if (x0 == x)
    STBTT_assert(x1 <= x+1);
  else if (x0 == x+1)
    STBTT_assert(x1 >= x);
  else if (x0 <= x)
    STBTT_assert(x1 <= x);
  else if (x0 >= x+1)
    STBTT_assert(x1 >= x+1);
  else
    STBTT_assert(x1 >= x && x1 <= x+1);
  if (x0 <= x && x1 <= x)
    scanline[x] += e->direction * (y1-y0);
  else if (x0 >= x+1 && x1 >= x+1)
    ;
  else {
    STBTT_assert(x0 >= x && x0 <= x+1 && x1 >= x && x1 <= x+1);
    scanline[x] += e->direction * (y1-y0) * (1-((x0-x)+(x1-x))/2); // coverage = 1 - average x position
  }
}

//----------

void stbtt__fill_active_edges_new(float *scanline, float *scanline_fill, int len, stbtt__active_edge *e, float y_top) {
  float y_bottom = y_top+1;
  while (e) {
    // brute force every pixel
    // compute intersection points with top & bottom
    STBTT_assert(e->ey >= y_top);
    if (e->fdx == 0) {
      float x0 = e->fx;
      if (x0 < len) {
        if (x0 >= 0) {
          stbtt__handle_clipped_edge(scanline,(int) x0,e, x0,y_top, x0,y_bottom);
          stbtt__handle_clipped_edge(scanline_fill-1,(int) x0+1,e, x0,y_top, x0,y_bottom);
        } else {
          stbtt__handle_clipped_edge(scanline_fill-1,0,e, x0,y_top, x0,y_bottom);
        }
      }
    } else {
      float x0 = e->fx;
      float dx = e->fdx;
      float xb = x0 + dx;
      float x_top, x_bottom;
      float sy0,sy1;
      float dy = e->fdy;
      STBTT_assert(e->sy <= y_bottom && e->ey >= y_top);
      // compute endpoints of line segment clipped to this scanline (if the
      // line segment starts on this scanline. x0 is the intersection of the
      // line with y_top, but that may be off the line segment.
      if (e->sy > y_top) {
        x_top = x0 + dx * (e->sy - y_top);
        sy0 = e->sy;
      } else {
        x_top = x0;
        sy0 = y_top;
      }
      if (e->ey < y_bottom) {
        x_bottom = x0 + dx * (e->ey - y_top);
        sy1 = e->ey;
      } else {
        x_bottom = xb;
        sy1 = y_bottom;
      }
      if (x_top >= 0 && x_bottom >= 0 && x_top < len && x_bottom < len) {
        // from here on, we don't have to range check x values
        if ((int) x_top == (int) x_bottom) {
          float height;
          // simple case, only spans one pixel
          int x = (int) x_top;
          height = sy1 - sy0;
          STBTT_assert(x >= 0 && x < len);
          scanline[x] += e->direction * (1-((x_top - x) + (x_bottom-x))/2)  * height;
          scanline_fill[x] += e->direction * height; // everything right of this pixel is filled
        } else {
          int x,x1,x2;
          float y_crossing, step, sign, area;
          // covers 2+ pixels
          if (x_top > x_bottom) {
            // flip scanline vertically; signed area is the same
            float t;
            sy0 = y_bottom - (sy0 - y_top);
            sy1 = y_bottom - (sy1 - y_top);
            t = sy0, sy0 = sy1, sy1 = t;
            t = x_bottom, x_bottom = x_top, x_top = t;
            dx = -dx;
            dy = -dy;
            t = x0, x0 = xb, xb = t;
          }
          x1 = (int) x_top;
          x2 = (int) x_bottom;
          // compute intersection with y axis at x1+1
          y_crossing = (x1+1 - x0) * dy + y_top;
          sign = e->direction;
          // area of the rectangle covered from y0..y_crossing
          area = sign * (y_crossing-sy0);
          // area of the triangle (x_top,y0), (x+1,y0), (x+1,y_crossing)
          scanline[x1] += area * (1-((x_top - x1)+(x1+1-x1))/2);
          step = sign * dy;
          for (x = x1+1; x < x2; ++x) {
            scanline[x] += area + step/2;
            area += step;
          }
          y_crossing += dy * (x2 - (x1+1));
          STBTT_assert(STBTT_fabs(area) <= 1.01f);
          scanline[x2] += area + sign * (1-((x2-x2)+(x_bottom-x2))/2) * (sy1-y_crossing);
          scanline_fill[x2] += sign * (sy1-sy0);
        }
      } else {
        // if edge goes outside of box we're drawing, we require
        // clipping logic. since this does not match the intended use
        // of this library, we use a different, very slow brute
        // force implementation
        int x;
        for (x=0; x < len; ++x) {
          // cases:
          //
          // there can be up to two intersections with the pixel. any intersection
          // with left or right edges can be handled by splitting into two (or three)
          // regions. intersections with top & bottom do not necessitate case-wise logic.
          //
          // the old way of doing this found the intersections with the left & right edges,
          // then used some simple logic to produce up to three segments in sorted order
          // from top-to-bottom. however, this had a problem: if an x edge was epsilon
          // across the x border, then the corresponding y position might not be distinct
          // from the other y segment, and it might ignored as an empty segment. to avoid
          // that, we need to explicitly produce segments based on x positions.
          // rename variables to clearly-defined pairs
          float y0 = y_top;
          float x1 = (float) (x);
          float x2 = (float) (x+1);
          float x3 = xb;
          float y3 = y_bottom;
          // x = e->x + e->dx * (y-y_top)
          // (y-y_top) = (x - e->x) / e->dx
          // y = (x - e->x) / e->dx + y_top
          float y1 = (x - x0) / dx + y_top;
          float y2 = (x+1 - x0) / dx + y_top;
          if (x0 < x1 && x3 > x2) {         // three segments descending down-right
            stbtt__handle_clipped_edge(scanline,x,e, x0,y0, x1,y1);
            stbtt__handle_clipped_edge(scanline,x,e, x1,y1, x2,y2);
            stbtt__handle_clipped_edge(scanline,x,e, x2,y2, x3,y3);
          } else if (x3 < x1 && x0 > x2) {  // three segments descending down-left
            stbtt__handle_clipped_edge(scanline,x,e, x0,y0, x2,y2);
            stbtt__handle_clipped_edge(scanline,x,e, x2,y2, x1,y1);
            stbtt__handle_clipped_edge(scanline,x,e, x1,y1, x3,y3);
          } else if (x0 < x1 && x3 > x1) {  // two segments across x, down-right
            stbtt__handle_clipped_edge(scanline,x,e, x0,y0, x1,y1);
            stbtt__handle_clipped_edge(scanline,x,e, x1,y1, x3,y3);
          } else if (x3 < x1 && x0 > x1) {  // two segments across x, down-left
            stbtt__handle_clipped_edge(scanline,x,e, x0,y0, x1,y1);
            stbtt__handle_clipped_edge(scanline,x,e, x1,y1, x3,y3);
          } else if (x0 < x2 && x3 > x2) {  // two segments across x+1, down-right
            stbtt__handle_clipped_edge(scanline,x,e, x0,y0, x2,y2);
            stbtt__handle_clipped_edge(scanline,x,e, x2,y2, x3,y3);
          } else if (x3 < x2 && x0 > x2) {  // two segments across x+1, down-left
            stbtt__handle_clipped_edge(scanline,x,e, x0,y0, x2,y2);
            stbtt__handle_clipped_edge(scanline,x,e, x2,y2, x3,y3);
          } else {  // one segment
            stbtt__handle_clipped_edge(scanline,x,e, x0,y0, x3,y3);
          }
        }
      }
    }
    e = e->next;
  }
}

//----------

// directly AA rasterize edges w/o supersampling
void stbtt__rasterize_sorted_edges(stbtt__bitmap *result, stbtt__edge *e, int n, int vsubsample, int off_x, int off_y, void *userdata) {
  stbtt__hheap hh = { 0, 0, 0 };
  stbtt__active_edge *active = NULL;
  int y,j=0, i;
  float scanline_data[129], *scanline, *scanline2;
  //#define STBTT__NOTUSED(v) (void)sizeof(v)
  //STBTT__NOTUSED(vsubsample);
  if (result->w > 64)
    scanline = (float *) STBTT_malloc((result->w*2+1) * sizeof(float), userdata);
  else
    scanline = scanline_data;
  scanline2 = scanline + result->w;
  y = off_y;
  e[n].y0 = (float) (off_y + result->h) + 1;
  while (j < result->h) {
    // find center of pixel for this scanline
    float scan_y_top    = y + 0.0f;
    float scan_y_bottom = y + 1.0f;
    stbtt__active_edge **step = &active;
    STBTT_memset(scanline , 0, result->w*sizeof(scanline[0]));
    STBTT_memset(scanline2, 0, (result->w+1)*sizeof(scanline[0]));
    // update all active edges;
    // remove all active edges that terminate before the top of this scanline
    while (*step) {
      stbtt__active_edge * z = *step;
      if (z->ey <= scan_y_top) {
        *step = z->next; // delete from list
        STBTT_assert(z->direction);
        z->direction = 0;
        stbtt__hheap_free(&hh, z);
      } else {
        step = &((*step)->next); // advance through list
      }
    }
    // insert all edges that start before the bottom of this scanline
    while (e->y0 <= scan_y_bottom) {
      if (e->y0 != e->y1) {
        stbtt__active_edge *z = stbtt__new_active(&hh, e, off_x, scan_y_top, userdata);
        if (z != NULL) {
          if (j == 0 && off_y != 0) {
            if (z->ey < scan_y_top) {
              // this can happen due to subpixel positioning and some kind of fp rounding error i think
              z->ey = scan_y_top;
            }
          }
          STBTT_assert(z->ey >= scan_y_top); // if we get really unlucky a tiny bit of an edge can be out of bounds
          // insert at front
          z->next = active;
          active = z;
        }
      }
      ++e;
    }
    // now process all active edges
    if (active)
      stbtt__fill_active_edges_new(scanline, scanline2+1, result->w, active, scan_y_top);
    {
      float sum = 0;
      for (i=0; i < result->w; ++i) {
        float k;
        int m;
        sum += scanline2[i];
        k = scanline[i] + sum;
        k = (float) STBTT_fabs(k)*255 + 0.5f;
        m = (int) k;
        if (m > 255) m = 255;
        result->pixels[j*result->stride + i] = (unsigned char) m;
      }
    }
    // advance all the edges
    step = &active;
    while (*step) {
      stbtt__active_edge *z = *step;
      z->fx += z->fdx; // advance to position for current scanline
      step = &((*step)->next); // advance through list
    }
    ++y;
    ++j;
  }
  stbtt__hheap_cleanup(&hh, userdata);
  if (scanline != scanline_data)
    STBTT_free(scanline, userdata);
}


//----------

void stbtt__rasterize(stbtt__bitmap *result, stbtt__point *pts, int *wcount, int windings, float scale_x, float scale_y, float shift_x, float shift_y, int off_x, int off_y, int invert, void *userdata) {
  float y_scale_inv = invert ? -scale_y : scale_y;
  stbtt__edge *e;
  int n,i,j,k,m;
  int vsubsample = 1;
  // vsubsample should divide 255 evenly; otherwise we won't reach full opacity
  // now we have to blow out the windings into explicit edge lists
  n = 0;
  for (i=0; i < windings; ++i)
    n += wcount[i];
  e = (stbtt__edge *) STBTT_malloc(sizeof(*e) * (n+1), userdata); // add an extra one as a sentinel
  if (e == 0) return;
  n = 0;
  m=0;
  for (i=0; i < windings; ++i) {
    stbtt__point *p = pts + m;
    m += wcount[i];
    j = wcount[i]-1;
    for (k=0; k < wcount[i]; j=k++) {
      int a=k,b=j;
      // skip the edge if horizontal
      if (p[j].y == p[k].y)
        continue;
      // add edge from j to k to the list
      e[n].invert = 0;
      if (invert ? p[j].y > p[k].y : p[j].y < p[k].y) {
        e[n].invert = 1;
        a=j,b=k;
      }
      e[n].x0 = p[a].x * scale_x + shift_x;
      e[n].y0 = (p[a].y * y_scale_inv + shift_y) * vsubsample;
      e[n].x1 = p[b].x * scale_x + shift_x;
      e[n].y1 = (p[b].y * y_scale_inv + shift_y) * vsubsample;
      ++n;
    }
  }
  // now sort the edges by their highest point (should snap to integer, and then by x)
  //STBTT_sort(e, n, sizeof(e[0]), stbtt__edge_compare);
  stbtt__sort_edges(e, n);
  // now, traverse the scanlines and find the intersections on each scanline, use xor winding rule
  stbtt__rasterize_sorted_edges(result, e, n, vsubsample, off_x, off_y, userdata);
  STBTT_free(e, userdata);
}

//----------

/*
result              // 1-channel bitmap to draw into
flatness_in_pixels  // allowable error of curve in pixels
vertices            // array of vertices defining shape
num_verts           // number of vertices in above array
scale_x, scale_y    // scale applied to input vertices
shift_x, shift_y    // translation applied to input vertices
x_off, y_off        // another translation applied to input
invert              // if non-zero, vertically flip shape
userdata            // context for to STBTT_MALLOC
*/

void stbtt_Rasterize(stbtt__bitmap *result, float flatness_in_pixels, stbtt_vertex *vertices, int num_verts, float scale_x, float scale_y, float shift_x, float shift_y, int x_off, int y_off, int invert, void *userdata) {
  float scale            = scale_x > scale_y ? scale_y : scale_x;
  int winding_count      = 0;
  int *winding_lengths   = NULL;
  stbtt__point *windings = stbtt_FlattenCurves(vertices, num_verts, flatness_in_pixels / scale, &winding_lengths, &winding_count, userdata);
  if (windings) {
    stbtt__rasterize(result, windings, winding_lengths, winding_count, scale_x, scale_y, shift_x, shift_y, x_off, y_off, invert, userdata);
    STBTT_free(winding_lengths, userdata);
    STBTT_free(windings, userdata);
  }
}



//----------------------------------------------------------------------
#endif

