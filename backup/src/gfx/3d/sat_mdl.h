#ifndef sat_mdl_included
#define sat_mdl_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gfx/3d/sat_3d.h"

struct SAT_MDLHeader {
  int         ident;                // magic number: "IDPO" (1330660425)
  int         version;              // version: 6
  SAT_Vector3 scale;                // scale factor
  SAT_Vector3 translate;            // translation vector
  float       boundingradius;       // bounding sphere radius
  SAT_Vector3 eyeposition;          // eyes' position
  int         num_skins;            // number of textures
  int         skinwidth;            // texture width
  int         skinheight;           // texture height
  int         num_verts;            // number of vertices
  int         num_tris;             // number of triangles
  int         num_frames;           // number of frames
  int         synctype;             // 0 = synchron, 1 = random
  int         flags;                // state flag
  float       size;
};

struct SAT_MDLSkin {
  int       group = 0;          // 0 = single, 1 = group
  uint8_t*  data  = nullptr;  // texture data
};

struct SAT_MDLGroupSkin {
  int       group = 1;          // 1 = group
  int       nb    = 0;          // number of pics
  float*    time  = nullptr;  // time duration for each pic
  uint8_t** data  = nullptr;  // texture data
};

struct SAT_MDLTexCoord {
  int onseam  = 0;
  int s       = 0;
  int t       = 0;
};

struct SAT_MDLTriangle {
  int facesfront  = 0;          // 0 = backface, 1 = frontface
  int vertex[3]   = {0};        // vertex indices
};

struct SAT_MDLVertex {
  uint8_t v[3]        = {0};
  uint8_t normalIndex = 0;
};

struct SAT_MDLSimpleFrame {
  SAT_MDLVertex   bboxmin;              // bouding box min
  SAT_MDLVertex   bboxmax;              // bouding box max
  char            name[16] = {0};
  SAT_MDLVertex*  verts    = nullptr; // vertex list of the frame
};

struct SAT_MDLFrame {
  int                 type  = 0;  // 0 = simple, !0 = group
  SAT_MDLSimpleFrame  frame;      // this program can't read models composed of group frames!
};

struct SAT_MDLGroupFrame {
  int                 type    = 0;          // !0 = group
  SAT_MDLVertex       min;                  // min pos in all simple frames
  SAT_MDLVertex       max;                  // max pos in all simple frames
  float*              time    = nullptr;  // time duration for each frame
  SAT_MDLSimpleFrame* frames  = nullptr;  // simple frame list
};

struct SAT_MDLModel {
  SAT_MDLHeader*    header    = nullptr;
  SAT_MDLSkin*      skins     = nullptr;
  SAT_MDLTexCoord*  texcoords = nullptr;
  SAT_MDLTriangle*  triangles = nullptr;
  SAT_MDLFrame*     frames    = nullptr;
};

//----------------------------------------------------------------------
#endif
