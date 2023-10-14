#ifndef sat_3d_included
#define sat_3d_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gfx/sat_bitmap.h"

struct SAT_Vector3 {
};

//----------------------------------------------------------------------

struct SAT_3DVertex {
  uint32_t              flags         = 0;
  SAT_Vector3          pos           = SAT_Vector3(0.0f, 0.0f, 0.0f);
  SAT_Vector3          normal        = SAT_Vector3(0.0f, 0.0f,-1.0f);
};

struct SAT_3DTexCoord {
  uint32_t              flags         = 0;
  float                 u             = 0.0f;
  float                 v             = 0.0f;
};

struct SAT_3DMorphTarget {
  SAT_3DVertex*        vertices      = SAT_NULL;
};

struct SAT_3DTriangle {
  uint32_t              flags         = 0;
  uint32_t              v1            = 0;
  uint32_t              v2            = 0;
  uint32_t              v3            = 0;
};

struct SAT_3DMaterial {
  uint32_t              flags         = 0;
  uint32_t              num_textures  = 0;
  SAT_Bitmap**         textures      = SAT_NULL;
};

struct SAT_3DMesh {
  uint32_t              num_vertices  = 0;
  uint32_t              num_triangles = 0;
  uint32_t              num_morphs    = 0;
  SAT_3DVertex*        vertices      = SAT_NULL;
  SAT_3DTexCoord*      texcoords     = SAT_NULL;
  SAT_3DTriangle*      triangles     = SAT_NULL;
  SAT_3DMorphTarget*   morphtargets  = SAT_NULL;
};

struct SAT_3DModel {
  SAT_3DMesh*          mesh          = SAT_NULL;;
  SAT_3DMaterial*      material      = SAT_NULL;;
};

//----------------------------------------------------------------------











//struct mdl_header_t {
//  int     ident;              // magic number: "IDPO" // 1330660425
//  int     version;            // version: 6
//  vec3_t  scale;              // scale factor
//  vec3_t  translate;          // translation vector
//  float   boundingradius;
//  vec3_t  eyeposition;        // eyes' position
//  int     num_skins;          // number of textures
//  int     skinwidth;          // texture width
//  int     skinheight;         // texture height
//  int     num_verts;          // number of vertices
//  int     num_tris;           // number of triangles
//  int     num_frames;         // number of frames
//  int     synctype;           // 0 = synchron, 1 = random
//  int     flags;              // state flag
//  float   size;
//};

//struct mdl_skin_t {
//  int       group;            // 0 = single, 1 = group
//  uint8_t*  data;             // texture data
//};

//struct mdl_groupskin_t {
//  int       group;            /* 1 = group */
//  int       nb;               /* number of pics */
//  float*    time;             /* time duration for each pic */
//  uint8_t** data;             /* texture data */
//};



//struct mdl_texcoord_t {
//  int onseam;
//  int s;
//  int t;
//};

//struct mdl_triangle_t {
//  int facesfront;             /* 0 = backface, 1 = frontface */
//  int vertex[3];              /* vertex indices */
//};


//struct mdl_vertex_t {
//  uint8_t v[3];
//  uint8_t normalIndex;
//};

//struct mdl_simpleframe_t {
//  mdl_vertex_t  bboxmin;      /* bouding box min */
//  mdl_vertex_t  bboxmax;      /* bouding box max */
//  char          name[16];
//  mdl_vertex_t* verts;        /* vertex list of the frame */
//};

//struct mdl_frame_t {
//  int               type;     /* 0 = simple, !0 = group */
//  mdl_simpleframe_t frame;    /* this program can't read models composed of group frames! */
//};

//struct mdl_groupframe_t {
//  int                 type;   /* !0 = group */
//  mdl_vertex_t        min;    /* min pos in all simple frames */
//  mdl_vertex_t        max;    /* max pos in all simple frames */
//  float*              time;   /* time duration for each frame */
//  mdl_simpleframe_t*  frames; /* simple frame list */
//};

//struct mdl_model_t {
//  mdl_header_t*   header;
//  mdl_skin_t*     skins;
//  mdl_texcoord_t* texcoords;
//  mdl_triangle_t* triangles;
//  mdl_frame_t*    frames;
//};



//----------------------------------------------------------------------
#endif
