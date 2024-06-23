#ifndef kode_gfx_outline_included
#define kode_gfx_outline_included
//----------------------------------------------------------------------

/*
  based on this:
    https://sourceforge.net/projects/outline/
*/

class KODE_Outline {

  #define MAX_POINTS 300

  //------------------------------
  //
  //------------------------------

  private:

    typedef struct {
      float vertex_start[2];
      float vertex_end[2];
    } segment_type;

    typedef struct {
      float vertex[2];
      int line1,line2;
      float t1,t2;
      bool valid;
      bool used;
    } intersect_type;

    typedef struct {
      int seg_ID;
      //float corner[2];
      int corner_ID;
    } triangle_type;

  //------------------------------
  //
  //------------------------------

  private:

    float           OutlineDistance;// = 50.0;
    int             InputPointNumber;// = 0;
    float           InputPoints[MAX_POINTS][2];// = {};
    int             ControlPointNumber;// = 0;
    float           ControlPoints[MAX_POINTS*2][2];// = {};
    float           LeftSideLine[MAX_POINTS*2][2];
    float           RightSideLine[MAX_POINTS*2][2];
    int             SideLineNum;
    segment_type    SideLines[MAX_POINTS*4];
    int             OutlineInterNum;
    intersect_type  OutlineInterPoints[MAX_POINTS*MAX_POINTS*4];
    int             OutlineValidSegmentNum;
    int             OutlineValidSegmentEnds[MAX_POINTS*MAX_POINTS*4][2];
    int             OutlineVertNum;
    int             OutlineVertIndex[MAX_POINTS*MAX_POINTS*4];
    int             TriangleNum;
    triangle_type   TriangleList[MAX_POINTS*2+10];

  //------------------------------
  //
  //------------------------------

  public:

    KODE_Outline() {
    }

    virtual ~KODE_Outline() {
    }

  //------------------------------
  //
  //------------------------------

  private:

    void vekt_szorz(float *a,float *b,float *out) {
      out[0] = a[1]*b[2] - a[2]*b[1];
      out[1] = a[2]*b[0] - a[0]*b[2];
      out[2] = a[0]*b[1] - a[1]*b[0];
    }

    //----------

    float skal_szorz(float *a,float *b) {
      return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
    }

    //----------

    void PerpVectRight(float *in, float *out) {
      out[0] =  in[1];
      out[1] = -in[0];
    }

    //----------

    void PerpVectLeft(float *in, float *out) {
      out[0] = -in[1];
      out[1] =  in[0];
    }

    //----------

    float Dot2D(float *v1, float *v2) {
      return v1[0]*v2[0]+v1[1]*v2[1];
    }

    //----------

    void Norm2D(float *v) {
      float l = sqrt(Dot2D(v,v));
      if (abs(l) > 0.0000001) {
        v[0] /= l;
        v[1] /= l;
      }
    }

    float Cross2D(float *v1, float *v2) {
      return v1[0]*v2[1] - v2[0]*v1[1];
    }

  //------------------------------
  //
  //------------------------------

  private:

    void IntersectSegment(segment_type *seg1,segment_type *seg2, float *t1, float *t2, float *P) {
      float x4_x3,x2_x1;
      float y4_y3,y2_y1;
      x4_x3 = seg2->vertex_end[0] - seg2->vertex_start[0];
      x2_x1 = seg1->vertex_end[0] - seg1->vertex_start[0];
      y4_y3 = seg2->vertex_end[1] - seg2->vertex_start[1];
      y2_y1 = seg1->vertex_end[1] - seg1->vertex_start[1];
      *t1 = x4_x3*(seg1->vertex_start[1] - seg2->vertex_start[1])
          - y4_y3*(seg1->vertex_start[0] - seg2->vertex_start[0]);
      if( abs(y4_y3*x2_x1 - x4_x3*y2_y1) < 0.01 ) {
        *t1 = -1;
        return;
      }
      *t1 /= y4_y3*x2_x1 - x4_x3*y2_y1;
      *t2 = x2_x1*(seg1->vertex_start[1] - seg2->vertex_start[1])
          - y2_y1*(seg1->vertex_start[0] - seg2->vertex_start[0]);
      if( abs(y4_y3*x2_x1 - x4_x3*y2_y1) < 0.01 ) {
        *t1 = -1;
        return;
      }
      *t2 /= y4_y3*x2_x1 - x4_x3*y2_y1;
      P[0] = seg1->vertex_start[0] + (*t1)*x2_x1;
      P[1] = seg1->vertex_start[1] + (*t1)*y2_y1;
    }

    //----------

    bool Poin_On_Poly(int sides,float *point,float *poly_pts, float *norm) {
      float v1[3], v2[3], n[3];
      float skal;
      for( int i = 0; i < sides; i++ ) {
        v1[0] = poly_pts[  i          *3 + 0] - point[0];
        v1[1] = poly_pts[  i          *3 + 1] - point[1];
        v1[2] = poly_pts[  i          *3 + 2] - point[2];
        v2[0] = poly_pts[((i+1)%sides)*3 + 0] - point[0];
        v2[1] = poly_pts[((i+1)%sides)*3 + 1] - point[1];
        v2[2] = poly_pts[((i+1)%sides)*3 + 2] - point[2];
        vekt_szorz(v1,v2,n);
        skal = skal_szorz(n,norm);
        if (skal > -0.1) return 0;
      }
      return 1;
    }

    //----------

    bool Point_On_trapez(int side_seg,int contol_seg, float *pt) {
      float point[12];
      float pt3[3] = {pt[0],pt[1],0};
      float dir1[2],dir2[2];
      float norm[] = {0,0,-1};
      dir1[0] = SideLines[side_seg].vertex_end[0] - SideLines[side_seg].vertex_start[0];
      dir1[1] = SideLines[side_seg].vertex_end[1] - SideLines[side_seg].vertex_start[1];
      dir2[0] = ControlPoints[contol_seg+1][0] -  ControlPoints[contol_seg][0];
      dir2[1] = ControlPoints[contol_seg+1][1] -  ControlPoints[contol_seg][1];
      point[0*3+0] = SideLines[side_seg].vertex_start[0];
      point[0*3+1] = SideLines[side_seg].vertex_start[1];
      point[0*3+2] = 0;
      point[1*3+0] = SideLines[side_seg].vertex_end[0];
      point[1*3+1] = SideLines[side_seg].vertex_end[1];
      point[1*3+2] = 0;
      if( Dot2D(dir1,dir2) > 0 ) {
        point[2*3+0] = ControlPoints[contol_seg+1][0];
        point[2*3+1] = ControlPoints[contol_seg+1][1];
        point[2*3+2] = 0;
        point[3*3+0] = ControlPoints[contol_seg][0];
        point[3*3+1] = ControlPoints[contol_seg][1];
        point[3*3+2] = 0;
      }
      else {
        point[3*3+0] = ControlPoints[contol_seg+1][0];
        point[3*3+1] = ControlPoints[contol_seg+1][1];
        point[3*3+2] = 0;
        point[2*3+0] = ControlPoints[contol_seg][0];
        point[2*3+1] = ControlPoints[contol_seg][1];
        point[2*3+2] = 0;
      }
      return Poin_On_Poly(4,pt3,point,norm);
    }

    //----------

    bool Point_On_Tri(int tri_ID, float *pt) {
      float point[9];
      float pt3[3] = {pt[0],pt[1],0};
      float norm[] = {0,0,-1};
      point[0*3+0] = ControlPoints[TriangleList[tri_ID].corner_ID][0];
      point[0*3+1] = ControlPoints[TriangleList[tri_ID].corner_ID][1];
      point[0*3+2] = 0;
      point[1*3+0] = SideLines[TriangleList[tri_ID].seg_ID].vertex_start[0];
      point[1*3+1] = SideLines[TriangleList[tri_ID].seg_ID].vertex_start[1];
      point[1*3+2] = 0;
      point[2*3+0] = SideLines[TriangleList[tri_ID].seg_ID].vertex_end[0];
      point[2*3+1] = SideLines[TriangleList[tri_ID].seg_ID].vertex_end[1];
      point[2*3+2] = 0;
      return Poin_On_Poly(3,pt3,point,norm);
    }

    //----------

    void SortIndex(int *index,float *val,int max) {
      for( int i = 0; i < max-1; i++ ) {
        for( int k = i+1; k < max; k++ ) {
          int store;
          float f;
          if( val[k] < val[i] ) {
            store = index[i];
            index[i] = index[k];
            index[k] = store;
            f = val[i];
            val[i] = val[k];
            val[k] = f;
          }
        }
      }
    }

  //------------------------------
  //
  //------------------------------

  private:

  // InputPoints -> ControlPoints

    void ValidateControlLine() {
      float cur_dir[2],next_dir[2];
      float prev_pt[2];
      float len1,len2;
      ControlPointNumber = 0;
      TriangleNum = 0;
      if( InputPointNumber < 2 ) return;
      ControlPoints[0][0] = prev_pt[0] = InputPoints[0][0];
      ControlPoints[0][1] = prev_pt[1] = InputPoints[0][1];
      ControlPointNumber ++;
      for (int i=1; i<InputPointNumber; i++) {
        //float len1,len2;
        cur_dir[0] = InputPoints[i][0] - prev_pt[0];
        cur_dir[1] = InputPoints[i][1] - prev_pt[1];
        len1 = sqrt(Dot2D(cur_dir,cur_dir));
        if( len1 < 2.0 ) continue;
        cur_dir[0] /= len1;
        cur_dir[1] /= len1;
        if( i < InputPointNumber-1 ) {
          next_dir[0] = InputPoints[i+1][0] - InputPoints[i][0];
          next_dir[1] = InputPoints[i+1][1] - InputPoints[i][1];
          len2 = sqrt(Dot2D(next_dir,next_dir));
          if( len2 < 2.0 ) continue;
          next_dir[0] /= len2;
          next_dir[1] /= len2;
          if( Dot2D(next_dir,cur_dir) > 0 && abs(Cross2D(next_dir,cur_dir)) < 0.02 ) continue;
        }
        prev_pt[0] = InputPoints[i][0];
        prev_pt[1] = InputPoints[i][1];
        ControlPoints[ControlPointNumber][0] = InputPoints[i][0];
        ControlPoints[ControlPointNumber][1] = InputPoints[i][1];
        ControlPointNumber ++;
      }
      if (ControlPointNumber < 2) return;
      float ratio;
      float perp[2];
      cur_dir[0] = ControlPoints[2][0] - ControlPoints[1][0];
      cur_dir[1] = ControlPoints[2][1] - ControlPoints[1][1];
      next_dir[0] = ControlPoints[0][0] - ControlPoints[1][0];
      next_dir[1] = ControlPoints[0][1] - ControlPoints[1][1];
      PerpVectRight(cur_dir,perp);
      Norm2D(perp);
      len1 = sqrt(Dot2D(next_dir,next_dir));
      Norm2D(next_dir);
      perp[0] *= OutlineDistance;
      perp[1] *= OutlineDistance;
      ratio = abs(Dot2D(perp,next_dir));
      if( ratio*1.1 > len1 ) {
        next_dir[0] *= ratio*1.1;
        next_dir[1] *= ratio*1.1;
        ControlPoints[0][0] = ControlPoints[1][0] + next_dir[0];
        ControlPoints[0][1] = ControlPoints[1][1] + next_dir[1];
      }
      cur_dir[0]  = ControlPoints[ControlPointNumber-3][0] - ControlPoints[ControlPointNumber-2][0];
      cur_dir[1]  = ControlPoints[ControlPointNumber-3][1] - ControlPoints[ControlPointNumber-2][1];
      next_dir[0] = ControlPoints[ControlPointNumber-1][0] - ControlPoints[ControlPointNumber-2][0];
      next_dir[1] = ControlPoints[ControlPointNumber-1][1] - ControlPoints[ControlPointNumber-2][1];
      PerpVectRight(cur_dir,perp);
      Norm2D(perp);
      len1 = sqrt(Dot2D(next_dir,next_dir));
      Norm2D(next_dir);
      perp[0] *= OutlineDistance;
      perp[1] *= OutlineDistance;
      ratio = abs(Dot2D(perp,next_dir));
      if( ratio*1.1 > len1 ) {
        next_dir[0] *= ratio*1.1;
        next_dir[1] *= ratio*1.1;
        ControlPoints[ControlPointNumber-1][0] =
        ControlPoints[ControlPointNumber-2][0] + next_dir[0];
        ControlPoints[ControlPointNumber-1][1] =
        ControlPoints[ControlPointNumber-2][1] + next_dir[1];
      }
    }

  //------------------------------
  //
  //------------------------------

    void CalcTrapezoidCorner(float *vert, float *perp_edge_dir, float *tr_dir, float distance, float *tr_corner) {
      float dot_product = Dot2D(tr_dir,perp_edge_dir);
      tr_corner[0] = vert[0] + distance / dot_product * tr_dir[0];
      tr_corner[1] = vert[1] + distance / dot_product * tr_dir[1];
    }

    //----------

    // RightSideLine

    void CalculateRightLines() {
      float trap_dir[2],trap_dir_2[2];
      float control_dir1[2];
      float control_dir2[2];
      float perp[2];
      bool is_new_triangle;
      if( ControlPointNumber < 2 ) return;
      control_dir1[0] = ControlPoints[1][0] - ControlPoints[0][0];
      control_dir1[1] = ControlPoints[1][1] - ControlPoints[0][1];
      PerpVectRight(control_dir1, perp);
      Norm2D(perp);
      RightSideLine[0][0] = ControlPoints[0][0] + perp[0]*OutlineDistance;
      RightSideLine[0][1] = ControlPoints[0][1] + perp[1]*OutlineDistance;
      ///////////
      control_dir1[0] = ControlPoints[ControlPointNumber-1][0] - ControlPoints[ControlPointNumber-2][0];
      control_dir1[1] = ControlPoints[ControlPointNumber-1][1] - ControlPoints[ControlPointNumber-2][1];
      PerpVectRight(control_dir1, perp);
      Norm2D(perp);
      RightSideLine[2*ControlPointNumber-3][0] = ControlPoints[ControlPointNumber-1][0] + perp[0]*OutlineDistance;
      RightSideLine[2*ControlPointNumber-3][1] = ControlPoints[ControlPointNumber-1][1] + perp[1]*OutlineDistance;
      for( int i = 1; i < ControlPointNumber-1; i++ ) {
        is_new_triangle = false;
        control_dir1[0] = ControlPoints[i-1][0] - ControlPoints[i][0];
        control_dir1[1] = ControlPoints[i-1][1] - ControlPoints[i][1];
        control_dir2[0] = ControlPoints[i+1][0] - ControlPoints[i][0];
        control_dir2[1] = ControlPoints[i+1][1] - ControlPoints[i][1];
        Norm2D(control_dir1);
        Norm2D(control_dir2);
        trap_dir_2[0] = trap_dir[0] = control_dir1[0] + control_dir2[0];
        trap_dir_2[1] = trap_dir[1] = control_dir1[1] + control_dir2[1];
        PerpVectRight(control_dir2, perp);
        Norm2D(perp);
        if( Dot2D(control_dir1, perp) > 0 ) {
          trap_dir[0] = perp[0];
          trap_dir[1] = perp[1];
        }
        else if( Dot2D(control_dir2, trap_dir) > 1.0 ) {
          trap_dir[0] = perp[0] - control_dir2[0];
          trap_dir[1] = perp[1] - control_dir2[1];
          is_new_triangle = true;
        }
        CalcTrapezoidCorner(ControlPoints[i], perp, trap_dir, OutlineDistance, RightSideLine[i*2]);
        if( is_new_triangle ) {
          SideLines[ControlPointNumber*2+TriangleNum].vertex_end[0] = RightSideLine[i*2][0];
          SideLines[ControlPointNumber*2+TriangleNum].vertex_end[1] = RightSideLine[i*2][1];
        }
        ////
        PerpVectLeft(control_dir1, perp);
        Norm2D(perp);
        if( Dot2D(control_dir2, perp) > 0 ) {
          trap_dir_2[0] = perp[0];
          trap_dir_2[1] = perp[1];
        }
        else if( Dot2D(control_dir1, trap_dir_2) > 1.0 ) {
          trap_dir_2[0] = perp[0] - control_dir1[0];
          trap_dir_2[1] = perp[1] - control_dir1[1];
        }
        CalcTrapezoidCorner(ControlPoints[i], perp, trap_dir_2, OutlineDistance, RightSideLine[i*2-1]);
        if( is_new_triangle ) {
          SideLines[ControlPointNumber*2+TriangleNum].vertex_start[0] = RightSideLine[i*2-1][0];
          SideLines[ControlPointNumber*2+TriangleNum].vertex_start[1] = RightSideLine[i*2-1][1];
          TriangleList[TriangleNum].corner_ID = i;
          TriangleList[TriangleNum].seg_ID = ControlPointNumber*2+TriangleNum;
          TriangleNum ++;
        }
      }
    }

    //----------

    // LeftSideLine

    void CalculateLeftLines() {
      float trap_dir[2],trap_dir_2[2];
      float control_dir1[2];
      float control_dir2[2];
      float perp[2];
      bool is_new_triangle;
      if( ControlPointNumber < 2 ) return;
      control_dir1[0] = ControlPoints[1][0] - ControlPoints[0][0];
      control_dir1[1] = ControlPoints[1][1] - ControlPoints[0][1];
      PerpVectLeft(control_dir1, perp);
      Norm2D(perp);
      LeftSideLine[0][0] = ControlPoints[0][0] + perp[0]*OutlineDistance;
      LeftSideLine[0][1] = ControlPoints[0][1] + perp[1]*OutlineDistance;
      ///////////
      control_dir1[0] = ControlPoints[ControlPointNumber-1][0] - ControlPoints[ControlPointNumber-2][0];
      control_dir1[1] = ControlPoints[ControlPointNumber-1][1] - ControlPoints[ControlPointNumber-2][1];
      PerpVectLeft(control_dir1, perp);
      Norm2D(perp);
      LeftSideLine[2*ControlPointNumber-3][0] = ControlPoints[ControlPointNumber-1][0] + perp[0]*OutlineDistance;
      LeftSideLine[2*ControlPointNumber-3][1] = ControlPoints[ControlPointNumber-1][1] + perp[1]*OutlineDistance;
      for( int i = 1; i < ControlPointNumber-1; i++ ) {
        is_new_triangle = false;
        control_dir1[0] = ControlPoints[i][0] - ControlPoints[i-1][0];
        control_dir1[1] = ControlPoints[i][1] - ControlPoints[i-1][1];
        control_dir2[0] = ControlPoints[i][0] - ControlPoints[i+1][0];
        control_dir2[1] = ControlPoints[i][1] - ControlPoints[i+1][1];
        Norm2D(control_dir1);
        Norm2D(control_dir2);
        trap_dir_2[0] = trap_dir[0] = control_dir1[0] + control_dir2[0];
        trap_dir_2[1] = trap_dir[1] = control_dir1[1] + control_dir2[1];
        PerpVectRight(control_dir2, perp);
        Norm2D(perp);
        if( Dot2D(control_dir1, perp) < 0 ) {
          trap_dir[0] = perp[0];
          trap_dir[1] = perp[1];
        }
        else if( Dot2D(control_dir2, trap_dir) > 1.0 ) {
          trap_dir[0] = perp[0] + control_dir2[0];
          trap_dir[1] = perp[1] + control_dir2[1];
          is_new_triangle = true;
        }
        CalcTrapezoidCorner(ControlPoints[i], perp, trap_dir, OutlineDistance, LeftSideLine[i*2]);
        if( is_new_triangle ) {
          SideLines[ControlPointNumber*2+TriangleNum].vertex_start[0] = LeftSideLine[i*2][0];
          SideLines[ControlPointNumber*2+TriangleNum].vertex_start[1] = LeftSideLine[i*2][1];
        }
        ////
        PerpVectLeft(control_dir1, perp);
        Norm2D(perp);
        if( Dot2D(control_dir2, perp) < 0 ) {
          trap_dir_2[0] = perp[0];
          trap_dir_2[1] = perp[1];
        }
        else if( Dot2D(control_dir1, trap_dir_2) > 1.0 ) {
          trap_dir_2[0] = perp[0] + control_dir1[0];
          trap_dir_2[1] = perp[1] + control_dir1[1];
        }
        CalcTrapezoidCorner(ControlPoints[i], perp, trap_dir_2, OutlineDistance, LeftSideLine[i*2-1]);
        if( is_new_triangle ) {
          SideLines[ControlPointNumber*2+TriangleNum].vertex_end[0] = LeftSideLine[i*2-1][0];
          SideLines[ControlPointNumber*2+TriangleNum].vertex_end[1] = LeftSideLine[i*2-1][1];
          TriangleList[TriangleNum].corner_ID = i;
          TriangleList[TriangleNum].seg_ID = ControlPointNumber*2+TriangleNum;
          TriangleNum ++;
        }
      }
    }

  //------------------------------

    // SideLines

    void UpdateSideLineBuf() {
      SideLineNum = 0;
      if( ControlPointNumber < 2 ) return;
      for( int i = 0; i < ControlPointNumber*2-2; i += 2 ) {
        SideLines[SideLineNum].vertex_start[0] = RightSideLine[i][0];
        SideLines[SideLineNum].vertex_start[1] = RightSideLine[i][1];
        SideLines[SideLineNum].vertex_end[0] = RightSideLine[i+1][0];
        SideLines[SideLineNum].vertex_end[1] = RightSideLine[i+1][1];
        SideLineNum ++;
        SideLines[SideLineNum].vertex_start[0] = LeftSideLine[i+1][0];
        SideLines[SideLineNum].vertex_start[1] = LeftSideLine[i+1][1];
        SideLines[SideLineNum].vertex_end[0] = LeftSideLine[i][0];
        SideLines[SideLineNum].vertex_end[1] = LeftSideLine[i][1];
        SideLineNum ++;
      }
      SideLines[SideLineNum].vertex_start[0] = LeftSideLine[0][0];
      SideLines[SideLineNum].vertex_start[1] = LeftSideLine[0][1];
      SideLines[SideLineNum].vertex_end[0] = RightSideLine[0][0];
      SideLines[SideLineNum].vertex_end[1] = RightSideLine[0][1];
      SideLineNum ++;
      SideLines[SideLineNum].vertex_start[0] = RightSideLine[2*ControlPointNumber-3][0];
      SideLines[SideLineNum].vertex_start[1] = RightSideLine[2*ControlPointNumber-3][1];
      SideLines[SideLineNum].vertex_end[0] = LeftSideLine[2*ControlPointNumber-3][0];
      SideLines[SideLineNum].vertex_end[1] = LeftSideLine[2*ControlPointNumber-3][1];
      SideLineNum ++;
      SideLineNum += TriangleNum;
    }

  //------------------------------
  //
  //------------------------------

  private:

    // OutlineInterPoints

    void CalculateInterPoints() {
      float t1,t2;
      OutlineInterNum = 0;
      if( SideLineNum < 4 ) return;
      for( int i = 0; i < SideLineNum-1; i++ ) {
        for( int k = i+1; k < SideLineNum; k++ ) {
          IntersectSegment(	SideLines+i,SideLines+k,&t1, &t2,
          OutlineInterPoints[OutlineInterNum].vertex);
          if( t1 >= -0.00001 && t1 <= 1.00001 && t2 >= -0.00001 && t2 <= 1.00001) {
            OutlineInterPoints[OutlineInterNum].line1 = i;
            OutlineInterPoints[OutlineInterNum].line2 = k;
            OutlineInterPoints[OutlineInterNum].t1 = t1;
            OutlineInterPoints[OutlineInterNum].t2 = t2;
            OutlineInterNum ++;
          }
        }
      }
    }

  //------------------------------
  //
  //------------------------------

  private:

    bool IsValidPoint(float *p,int line1,int line2) {
      SideLineNum -= TriangleNum;
      bool valid = true;
      int k = 0;
      int end = SideLineNum-2;
      int control_seg,side_seg;
      if( SideLineNum-2 == line1 || SideLineNum-2 == line2 ) k = 2;
      if( SideLineNum-1 == line1 || SideLineNum-1 == line2 ) end = SideLineNum-4;
      for( ; k < end; k++ ) {
        if( k == line1 || k == line2 ) continue;
        control_seg = k/2;
        side_seg = k;
        if( Point_On_trapez(side_seg,control_seg, p) ) {
          valid = false;
          //break;
          goto END_OF_VALID_POINT;
        }
      }
      for( k = 0; k < TriangleNum; k++ ) {
        if( SideLineNum+k == line1 || SideLineNum+k == line2 ) continue;
        if ( Point_On_Tri(k, p) ) {
          valid = false;
          break;
        }
      }
      END_OF_VALID_POINT:
      SideLineNum += TriangleNum;
      return valid;
    }

    //----------

    void ValidateInterPoints() {
      for( int i = 0; i < OutlineInterNum; i++ ) {
        OutlineInterPoints[i].used = false;
        OutlineInterPoints[i].valid = IsValidPoint(	OutlineInterPoints[i].vertex, OutlineInterPoints[i].line1, OutlineInterPoints[i].line2);
      }
    }

  //------------------------------
  //
  //------------------------------

  private:

    /*
    int debug_foo;

    void MarchingEdges() {
      int valid_index = 0;
      int new_index = 0;
      int cur_segment = 0, new_segment;
      //float cur_t = 0,new_t;
      float dist, min_dist;
      float current_dir[2];
      float new_dir[2];
      float perp[2];
      bool is_right = false;
      OutlineVertNum = 0;
      if( ControlPointNumber < 2 ) return;
      while( !OutlineInterPoints[valid_index].valid )
        valid_index++;
      cur_segment = OutlineInterPoints[valid_index].line2;
      OutlineInterPoints[valid_index].used = true;
      OutlineVertIndex[OutlineVertNum] = valid_index;
      OutlineVertNum ++;
      current_dir[0] = SideLines[cur_segment].vertex_end[0] - SideLines[cur_segment].vertex_start[0];
      current_dir[1] = SideLines[cur_segment].vertex_end[1] - SideLines[cur_segment].vertex_start[1];
      PerpVectRight(current_dir,perp);
      Norm2D(perp);
      debug_foo = 0;
      while(1) {
        int k;
        glPushMatrix();
        glLoadIdentity();
        glPrint(10,940-debug_foo,"current: %d, line1: %d, line2: %d", cur_segment, OutlineInterPoints[valid_index].line1, OutlineInterPoints[valid_index].line2);
        glPopMatrix();
        debug_foo += 20;
        if( OutlineInterPoints[valid_index].line1 == cur_segment ) {
          new_segment = OutlineInterPoints[valid_index].line2;
          //cur_t = OutlineInterPoints[valid_index].t2;
        }
        else {
          new_segment = OutlineInterPoints[valid_index].line1;
          //cur_t = OutlineInterPoints[valid_index].t1;
        }
        PerpVectRight(current_dir,perp);
        Norm2D(perp);
        glBegin(GL_LINES);
        glColor4f(0,0,1,1);
        glVertex2fv(OutlineInterPoints[valid_index].vertex);
        glVertex2f(	OutlineInterPoints[valid_index].vertex[0] + perp[0]*30,
        OutlineInterPoints[valid_index].vertex[1] + perp[1]*30);
        glEnd();
        min_dist = dist = 10000000.0;
        is_right = false;
        for( k = 0; k < OutlineInterNum; k++ ) {
          if( k == valid_index || !OutlineInterPoints[k].valid ) continue;
          if( new_segment == OutlineInterPoints[k].line1 || new_segment == OutlineInterPoints[k].line2 ) {
            new_dir[0] = OutlineInterPoints[k].vertex[0] - OutlineInterPoints[valid_index].vertex[0];
            new_dir[1] = OutlineInterPoints[k].vertex[1] - OutlineInterPoints[valid_index].vertex[1];
            dist = Dot2D(perp,new_dir);
            if( dist > 0 && !is_right ) {
              is_right = true;
              min_dist = dist;
            }
            if( is_right && dist < 0 ) continue;
            if( abs(min_dist) > abs(dist) ) {
              min_dist = dist;
              new_index = k;
              current_dir[0] = new_dir[0];
              current_dir[1] = new_dir[1];
            }
          }
        }
        valid_index = new_index;
        cur_segment = new_segment;
        //current_dir[0] = new_dir[0];
        //current_dir[1] = new_dir[1];
        glPushMatrix();
        glLoadIdentity();
        glPrint(200,700-OutlineVertNum*20,"min_dist: %f",min_dist);
        glPopMatrix();
        glBegin(GL_LINES);
        glColor4f(1,0,1,1);
        glVertex2f( OutlineInterPoints[valid_index].vertex[0] + 2,
        OutlineInterPoints[valid_index].vertex[1] + 2 );
        glVertex2f(	OutlineInterPoints[valid_index].vertex[0] + current_dir[0]*0.3+2,
        OutlineInterPoints[valid_index].vertex[1] + current_dir[1]*0.3+2);
        glEnd();
        OutlineVertIndex[OutlineVertNum] = valid_index;
        OutlineVertNum++;
        if( OutlineVertNum > 20 ) //OutlineInterPoints[valid_index].used )
          break;
        OutlineInterPoints[valid_index].used = true;
      }
    }
    */

  //------------------------------
  //
  //------------------------------

  private:

    void SortPointsOnLine(int *list, int seg,int max) {
      float t[2*MAX_POINTS+10];
      for( int i = 0; i < max; i++ ) {
        if (OutlineInterPoints[list[i]].line1 == seg)       t[i] = OutlineInterPoints[list[i]].t1;
        else if (OutlineInterPoints[list[i]].line2 == seg)  t[i] = OutlineInterPoints[list[i]].t2;
      }
      SortIndex(list,t,max);
    }

    //----------

    // OutlineValidSegmentNum
    // OutlineValidSegmentEnds[ OutlineValidSegmentNum ] [ 0 ]
    // OutlineValidSegmentEnds[ OutlineValidSegmentNum ] [ 1 ]

    void ValidateSegments() {
      int Line[2*MAX_POINTS+10];
      int index;
      bool valid;
      OutlineValidSegmentNum = 0;
      for( int i = 0; i < SideLineNum; i++ ) {
        int j = 0;
        index = 0;
        for( int k = 0; k < OutlineInterNum; k++ ) {
          if( (OutlineInterPoints[k].line1 == i || OutlineInterPoints[k].line2 == i ) && OutlineInterPoints[k].valid )
            Line[index++] = k;
        }
        SortPointsOnLine(Line, i,index);
        //val = true;
        while( j < index-1 ) {
          valid = true;
          for( int k = 1; k < 10; k++ ) {
            float p[2];
            p[0] =	OutlineInterPoints[Line[j+1]].vertex[0]*k/10.0 + OutlineInterPoints[Line[j]].vertex[0]*(10.0-k)/10.0;
            p[1] =	OutlineInterPoints[Line[j+1]].vertex[1]*k/10.0 + OutlineInterPoints[Line[j]].vertex[1]*(10.0-k)/10.0;
            if( !IsValidPoint(p,i,i) ) {
              valid = false;
              break;
            }
          }
          if( valid ) /* IsValidPoint(p,i,i) */ {
            OutlineValidSegmentEnds[OutlineValidSegmentNum][0] = Line[j];
            OutlineValidSegmentEnds[OutlineValidSegmentNum][1] = Line[j+1];
            OutlineValidSegmentNum ++;
          }
          j++;
          /*
          float p[2];
          p[0] =	OutlineInterPoints[Line[j+1]].vertex[0]*1.0/3.0 + OutlineInterPoints[Line[j]].vertex[0]*2.0/3.0;
          p[1] =	OutlineInterPoints[Line[j+1]].vertex[1]*1.0/3.0 + OutlineInterPoints[Line[j]].vertex[1]*2.0/3.0;
          if( IsValidPoint(p,i,i) ) {
            OutlineValidSegmentEnds[OutlineValidSegmentNum][0] = Line[j];
            OutlineValidSegmentEnds[OutlineValidSegmentNum][1] = Line[j+1];
            OutlineValidSegmentNum ++;
          }
          j+=1;
          */
        }
      }
    }

  //------------------------------
  //
  //------------------------------

  private:

    void CalculateOutline(void) {
      ValidateControlLine();
      CalculateRightLines();
      CalculateLeftLines();
      UpdateSideLineBuf();
      CalculateInterPoints();
      ValidateInterPoints();
      //MarchingEdges();
      ValidateSegments();
    }

    /*
      result:

      contour vertices

      glBegin(GL_POINTS);
      glColor4f(1,1,0,1);
      for( int i = 0; i < OutlineInterNum; i ++ ) {
        if( OutlineInterPoints[i].valid ) glVertex2fv(OutlineInterPoints[i].vertex);
      }

      outline - green

      glBegin(GL_LINES);
      glColor4f(0,1,0,1);
      for( int i = 0; i < OutlineValidSegmentNum; i++ ) {
        glVertex2fv( OutlineInterPoints[OutlineValidSegmentEnds[i][0]].vertex );
        glVertex2fv( OutlineInterPoints[OutlineValidSegmentEnds[i][1]].vertex );
      }

    */

  //------------------------------
  //
  //------------------------------

  public:

    void reset(void) {
      OutlineDistance = 50.0;
      InputPointNumber = 0;
      S3_Memset(InputPoints,0,sizeof(InputPoints));
      ControlPointNumber = 0;
      S3_Memset(ControlPoints,0,sizeof(InputPoints));
    }

    //----------

    /*
      input:  ANumInputs * (x,y)
      output: ANumOutputs * (x0,y0,x1,y1)
    */

    void process(uint32 ANumInputs, float* AInputs, uint32* ANumOutputs, float* AOutputs, float AWidth) {
      reset();
      OutlineDistance = AWidth;
      float* ptr = AInputs;
      for (uint32 i=0; i<ANumInputs; i++) {
        InputPoints[i][0] = *ptr++;
        InputPoints[i][1] = *ptr++;
      }
      InputPointNumber = ANumInputs;
      CalculateOutline();
      ptr = AOutputs;
      for( int i = 0; i < OutlineValidSegmentNum; i++ ) {
        *ptr++ = OutlineInterPoints[OutlineValidSegmentEnds[i][0]].vertex[0];
        *ptr++ = OutlineInterPoints[OutlineValidSegmentEnds[i][0]].vertex[1];
        *ptr++ = OutlineInterPoints[OutlineValidSegmentEnds[i][1]].vertex[0];
        *ptr++ = OutlineInterPoints[OutlineValidSegmentEnds[i][1]].vertex[1];
      }
      *ANumOutputs = OutlineValidSegmentNum;
    }

};

//----------------------------------------------------------------------
#endif