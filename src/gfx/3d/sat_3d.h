#ifndef sat_3d_included
#define sat_3d_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gfx/sat_bitmap.h"

struct SAT_Vector3 {
  double x,y,z;
};

//----------------------------------------------------------------------

struct SAT_3DVertex {
  uint32_t            flags         = 0;
  SAT_Vector3         pos           = SAT_Vector3(0.0f, 0.0f, 0.0f);
  SAT_Vector3         normal        = SAT_Vector3(0.0f, 0.0f,-1.0f);
};

struct SAT_3DTexCoord {
  uint32_t            flags         = 0;
  float               u             = 0.0f;
  float               v             = 0.0f;
};

struct SAT_3DMorphTarget {
  SAT_3DVertex*       vertices      = SAT_NULL;
};

struct SAT_3DTriangle {
  uint32_t            flags         = 0;
  uint32_t            v1            = 0;
  uint32_t            v2            = 0;
  uint32_t            v3            = 0;
};

struct SAT_3DMaterial {
  uint32_t            flags         = 0;
  uint32_t            num_textures  = 0;
  SAT_Bitmap**        textures      = SAT_NULL;
};

struct SAT_3DMesh {
  uint32_t            num_vertices  = 0;
  uint32_t            num_triangles = 0;
  uint32_t            num_morphs    = 0;
  SAT_3DVertex*       vertices      = SAT_NULL;
  SAT_3DTexCoord*     texcoords     = SAT_NULL;
  SAT_3DTriangle*     triangles     = SAT_NULL;
  SAT_3DMorphTarget*  morphtargets  = SAT_NULL;
};

struct SAT_3DModel {
  SAT_3DMesh*         mesh          = SAT_NULL;;
  SAT_3DMaterial*     material      = SAT_NULL;;
};

//----------------------------------------------------------------------
#endif
