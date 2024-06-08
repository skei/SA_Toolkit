#ifndef sat_mdl_importer_included
#define sat_mdl_importer_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gfx/3d/sat_3d.h"
#include "gfx/3d/sat_mdl.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "extern/stb/stb_image_write.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_MdlConverter {

//------------------------------
private:
//------------------------------

  FILE*             read_fp       = nullptr;
  FILE*             write_fp      = nullptr;

  SAT_MDLHeader     mdl_header;

  SAT_MDLSkin*      mdl_skins     = nullptr;
  SAT_MDLFrame*     mdl_frames    = nullptr;
  SAT_MDLTexCoord*  mdl_texcoords = nullptr;
  SAT_MDLTriangle*  mdl_triangles = nullptr;

//------------------------------
public:
//------------------------------

  SAT_MdlConverter() {
  }

  //----------

  ~SAT_MdlConverter() {
    cleanup();
  }

//------------------------------
public:
//------------------------------

  bool read(const char* AFilename) {
    //printf("Reading '%s'\n",AFilename);
    read_fp = fopen(AFilename,"rb");
    if (!read_fp) {
      printf("* Error. Couldn't open '%s'\n",AFilename);
      return false;
    }
    read_mdl_header();
    if ((mdl_header.ident != 1330660425) || (mdl_header.version != 6)) {
      printf("* Error: bad version or identifier\n");
      fclose(read_fp);
      return false;
    }
    read_mdl_skins();
    read_mdl_texcoords();
    read_mdl_triangles();
    read_mdl_frames();
    fclose(read_fp);
    return true;
  }

  //----------

  bool write(const char* AName) {
    write_skins(AName);
    write_vertex_frames(AName);
    write_normal_frames(AName);
    write_mesh(AName);
    //write_shader();
    //write_material();
    //write_script(1);
    return true;
  };

  //----------

  void dump() {
    print_mdl_header();
  }

  //----------

  void cleanup() {
    for (int i=0; i<mdl_header.num_skins; i++) {
      if (mdl_skins && mdl_skins[i].data) {
        free(mdl_skins[i].data);
        mdl_skins[i].data = nullptr;
      }
    }
    free(mdl_skins);
    mdl_skins = nullptr;
    for (int i=0; i<mdl_header.num_frames; ++i) {
      if (mdl_frames && mdl_frames[i].frame.verts) {
        free(mdl_frames[i].frame.verts);
        mdl_frames[i].frame.verts = nullptr;
      }
    }
    free(mdl_frames);
    mdl_frames = nullptr;
    free(mdl_texcoords);
    mdl_texcoords = nullptr;
    free(mdl_triangles);
    mdl_triangles = nullptr;
    //mdl_header = nullptr;
  }

//------------------------------
public:
//------------------------------

  void print_mdl_header() {
    printf("header:\n");
    printf("  ident           %i\n",mdl_header.ident);
    printf("  scale           %.3f, %.3f, %.3f\n",mdl_header.scale.x, mdl_header.scale.y, mdl_header.scale.z);
    printf("  translate       %.3f, %.3f, %.3f\n",mdl_header.translate.x, mdl_header.translate.y, mdl_header.translate.z);
    printf("  bounding_radius %.3f\n",mdl_header.boundingradius);
    printf("  eye_position    %.3f, %.3f, %.3f\n",mdl_header.eyeposition.x,mdl_header.eyeposition.y,mdl_header.eyeposition.z);
    printf("  num_skins       %i\n",mdl_header.num_skins);
    printf("  skin_width      %i\n",mdl_header.skinwidth);
    printf("  skin_height     %i\n",mdl_header.skinheight);
    printf("  num_verts       %i\n",mdl_header.num_verts);
    printf("  num_tris        %i\n",mdl_header.num_tris);
    printf("  num_frames      %i\n",mdl_header.num_frames);
    printf("  sync_type       %i\n",mdl_header.synctype);
    printf("  flags           %i\n",mdl_header.flags);
    printf("  size            %.3f\n",mdl_header.size);
  }

//------------------------------
private:
//------------------------------

  uint8_t read_byte(void) {
    uint8_t b;
    fread(&b,sizeof(uint8_t),1,read_fp);
    return b;
  }

  //----------

  int read_int(void) {
    int i;
    fread(&i,sizeof(int),1,read_fp);
    return i;
  }

  //----------

  float read_float(void) {
    float f;
    fread(&f,sizeof(float),1,read_fp);
    return f;
  }

  //----------

  vec3_t read_vector(void) {
    vec3_t v;
    fread(&v.x,sizeof(float),1,read_fp);
    fread(&v.y,sizeof(float),1,read_fp);
    fread(&v.z,sizeof(float),1,read_fp);
    return v;
  }

  //----------

  char* read_string(char* ptr, int num) {
    fread(ptr,sizeof(char),num,read_fp);
    return ptr;

  }

  //----------

  mdl_vertex_t read_vertex() {
    mdl_vertex_t vertex;
    vertex.v[0] = read_byte();
    vertex.v[1] = read_byte();
    vertex.v[2] = read_byte();
    vertex.normalIndex = read_byte();
    return vertex;
  }

  //----------

  mdl_triangle_t read_triangle() {
    mdl_triangle_t triangle;
    triangle.facesfront = read_int();
    triangle.vertex[0]  = read_int(); // index 1
    triangle.vertex[1]  = read_int(); // index 2
    triangle.vertex[2]  = read_int(); // index 3
    return triangle;
  }

//------------------------------
//
//------------------------------

  uint8_t* read_bytes(int num) {
    uint8_t* buffer = (uint8_t*)malloc(num * sizeof(uint8_t));
    fread(buffer,sizeof(uint8_t),num,read_fp);
    return buffer;
  }

  //----------

  float* read_floats(int num) {
    float* buffer = (float*)malloc(num * sizeof(float));
    fread(buffer,sizeof(float),num,read_fp);
    return buffer;
  }

//------------------------------
//
//------------------------------

  /*
    scale and translate are needed to obtain the real vertex coordinates of the
    model. scale is a scale factor and translate a translation vector (or the
    origin of the model). You have to first multiply the respective value of
    scale with the vertex coordinate and then, add the respective value of
    translate to the result:
  */

  void read_mdl_header() {
    //mdl_header = (mdl_header_t*)malloc(sizeof(mdl_header_t));
    mdl_header.ident          = read_int();
    mdl_header.version        = read_int();
    mdl_header.scale          = read_vector();
    mdl_header.translate      = read_vector();
    mdl_header.boundingradius = read_float();
    mdl_header.eyeposition    = read_vector();
    mdl_header.num_skins      = read_int();
    mdl_header.skinwidth      = read_int();
    mdl_header.skinheight     = read_int();
    mdl_header.num_verts      = read_int();
    mdl_header.num_tris       = read_int();
    mdl_header.num_frames     = read_int();
    mdl_header.synctype       = read_int();
    mdl_header.flags          = read_int();
    mdl_header.size           = read_float();
  }

  //----------

  void read_mdl_skins() {
    mdl_skins = (mdl_skin_t *)malloc(sizeof(mdl_skin_t) * mdl_header.num_skins);
    for (int i=0; i<mdl_header.num_skins; i++) {
      int skinsize = mdl_header.skinwidth * mdl_header.skinheight;
      //uint8_t* temp = (uint8_t*)malloc(size);
      //mdl_skins[i].data = (uint8_t *)malloc(sizeof(uint8_t) * skinsize);
      int group = read_int();
      if (group == 0) {
        mdl_skins[i].group = group;
        mdl_skins[i].data = read_bytes(skinsize);
        //s.nb = 0;
        //s.time = nullptr;
        //temp = read_byte_buffer(size);
        //s.data  = convert_skin(temp,size);
      }
      else {
        printf("* error.. skin group %i (group_skin) not supported yet (skin %i)\n",group,i);
        exit(100);

        //s.group = 1;
        //s.nb    = read_int();
        //printf("    group: %i\n",s.nb);
        //s.time  = read_float_buffer(s.nb);
        ////s.data  = read_byte_buffer(s.nb * mdl_header.skin_width * mdl_header.skin_height);
        //temp = read_byte_buffer(s.nb * size);
        //s.data  = convert_skin(temp,s.nb * size);
      }
      //free(temp);
    }
  }

  //----------

  /*
    Texture are generally divided in two pieces: one for the frontface of the
    model, and one for the backface. The backface piece must be translated of
    skinwidth/2 from the frontface piece.

    onseam indicates if the vertex is on the boundary of two pieces.

    To obtain real (s, t) coordinates (ranging from 0.0 to 1.0), you have to
    add 0.5 to the coordinates and then divide the result by skinwidth for s
    and skinheight for t.
  */

  void read_mdl_texcoords() {
    mdl_texcoords = (mdl_texcoord_t *)malloc(sizeof(mdl_texcoord_t) * mdl_header.num_verts);
    for (int i=0; i<mdl_header.num_verts; i++) {
      mdl_texcoords[i].onseam = read_int();
      mdl_texcoords[i].s      = read_int();
      mdl_texcoords[i].t      = read_int();
      //printf("  texcoord %i onseam %i u %i v %i\n",i,mdl_texcoords[i].onseam,mdl_texcoords[i].s,mdl_texcoords[i].t);
    }
  }

  //----------

  /*
    If a vertex which belong to a backface triangle is on the boundary of two
    pieces (onseam is true), you have to add skinwidth/2 to s in order
    to correct texture coordinates.
  */

  void read_mdl_triangles() {
    mdl_triangles = (mdl_triangle_t *)malloc(sizeof(mdl_triangle_t) * mdl_header.num_tris);
    for (int i=0; i<mdl_header.num_tris; i++) {
      mdl_triangles[i] = read_triangle();
      //printf("  triangle %i front %i x %i y %i z %i\n",i,mdl_triangles[i].facesfront,mdl_triangles[i].vertex[0],mdl_triangles[i].vertex[1],mdl_triangles[i].vertex[2]);
    }
  }

  //----------

  /*
    Vertices are composed of “compressed” 3D coordinates, which are stored in
    one byte for each coordinate, and of a normal vector index.
  */

  mdl_vertex_t* read_mdl_vertices(int num) {
    mdl_vertex_t* vertices = (mdl_vertex_t *)malloc(sizeof(mdl_vertex_t) * num);
    for (int i=0; i<num; i++) {
      vertices[i] = read_vertex();
      //printf("  vertex %i v1 %i v2 %i v3 %i normal %i\n",i,vertices[i].v[0],vertices[i].v[1],vertices[i].v[2]);
    }
    return vertices;
  }

  //----------

  void read_mdl_frames() {
    mdl_frames = (mdl_frame_t *)malloc(sizeof(mdl_frame_t) * mdl_header.num_frames);
    for (int i=0; i<mdl_header.num_frames; ++i) {
      int type = read_int();
      if (type == 0) { // mdl_simpleframe_t
        mdl_frames[i].type          = type;
        mdl_frames[i].frame.bboxmin = read_vertex();
        mdl_frames[i].frame.bboxmax = read_vertex();
        read_string(mdl_frames[i].frame.name,16);
        //printf("  frame %i type %i name %s\n",i,mdl_frames[i].type,mdl_frames[i].frame.name);
        mdl_frames[i].frame.verts   = read_mdl_vertices(mdl_header.num_verts);
      }
      else { // mdl_groupframe_t
        printf("* error.. fram type %i (groupframe)  not supported yet (frame %i)\n",type,i);
        exit(100);
        //int num               = type;
        //mdl_frames[i].type    = type;               /* !0 = group */
        //mdl_frames[i].min     = read_vertex();      /* min pos in all simple frames */
        //mdl_frames[i].max     = read_vertex();      /* max pos in all simple frames */
        //mdl_frames[i].time    = read_floats(num);   /* time duration for each frame */
        //mdl_frames[i].frames  = read_frames(num);  /* simple frame list */
      }
    }
  }

//------------------------------
private:
//------------------------------

  const uint8_t mdl_colormap[256][3] = {
    {  0,   0,   0}, { 15,  15,  15}, { 31,  31,  31}, { 47,  47,  47},
    { 63,  63,  63}, { 75,  75,  75}, { 91,  91,  91}, {107, 107, 107},
    {123, 123, 123}, {139, 139, 139}, {155, 155, 155}, {171, 171, 171},
    {187, 187, 187}, {203, 203, 203}, {219, 219, 219}, {235, 235, 235},
    { 15,  11,   7}, { 23,  15,  11}, { 31,  23,  11}, { 39,  27,  15},
    { 47,  35,  19}, { 55,  43,  23}, { 63,  47,  23}, { 75,  55,  27},
    { 83,  59,  27}, { 91,  67,  31}, { 99,  75,  31}, {107,  83,  31},
    {115,  87,  31}, {123,  95,  35}, {131, 103,  35}, {143, 111,  35},
    { 11,  11,  15}, { 19,  19,  27}, { 27,  27,  39}, { 39,  39,  51},
    { 47,  47,  63}, { 55,  55,  75}, { 63,  63,  87}, { 71,  71, 103},
    { 79,  79, 115}, { 91,  91, 127}, { 99,  99, 139}, {107, 107, 151},
    {115, 115, 163}, {123, 123, 175}, {131, 131, 187}, {139, 139, 203},
    {  0,   0,   0}, {  7,   7,   0}, { 11,  11,   0}, { 19,  19,   0},
    { 27,  27,   0}, { 35,  35,   0}, { 43,  43,   7}, { 47,  47,   7},
    { 55,  55,   7}, { 63,  63,   7}, { 71,  71,   7}, { 75,  75,  11},
    { 83,  83,  11}, { 91,  91,  11}, { 99,  99,  11}, {107, 107,  15},
    {  7,   0,   0}, { 15,   0,   0}, { 23,   0,   0}, { 31,   0,   0},
    { 39,   0,   0}, { 47,   0,   0}, { 55,   0,   0}, { 63,   0,   0},
    { 71,   0,   0}, { 79,   0,   0}, { 87,   0,   0}, { 95,   0,   0},
    {103,   0,   0}, {111,   0,   0}, {119,   0,   0}, {127,   0,   0},
    { 19,  19,   0}, { 27,  27,   0}, { 35,  35,   0}, { 47,  43,   0},
    { 55,  47,   0}, { 67,  55,   0}, { 75,  59,   7}, { 87,  67,   7},
    { 95,  71,   7}, {107,  75,  11}, {119,  83,  15}, {131,  87,  19},
    {139,  91,  19}, {151,  95,  27}, {163,  99,  31}, {175, 103,  35},
    { 35,  19,   7}, { 47,  23,  11}, { 59,  31,  15}, { 75,  35,  19},
    { 87,  43,  23}, { 99,  47,  31}, {115,  55,  35}, {127,  59,  43},
    {143,  67,  51}, {159,  79,  51}, {175,  99,  47}, {191, 119,  47},
    {207, 143,  43}, {223, 171,  39}, {239, 203,  31}, {255, 243,  27},
    { 11,   7,   0}, { 27,  19,   0}, { 43,  35,  15}, { 55,  43,  19},
    { 71,  51,  27}, { 83,  55,  35}, { 99,  63,  43}, {111,  71,  51},
    {127,  83,  63}, {139,  95,  71}, {155, 107,  83}, {167, 123,  95},
    {183, 135, 107}, {195, 147, 123}, {211, 163, 139}, {227, 179, 151},
    {171, 139, 163}, {159, 127, 151}, {147, 115, 135}, {139, 103, 123},
    {127,  91, 111}, {119,  83,  99}, {107,  75,  87}, { 95,  63,  75},
    { 87,  55,  67}, { 75,  47,  55}, { 67,  39,  47}, { 55,  31,  35},
    { 43,  23,  27}, { 35,  19,  19}, { 23,  11,  11}, { 15,   7,   7},
    {187, 115, 159}, {175, 107, 143}, {163,  95, 131}, {151,  87, 119},
    {139,  79, 107}, {127,  75,  95}, {115,  67,  83}, {107,  59,  75},
    { 95,  51,  63}, { 83,  43,  55}, { 71,  35,  43}, { 59,  31,  35},
    { 47,  23,  27}, { 35,  19,  19}, { 23,  11,  11}, { 15,   7,   7},
    {219, 195, 187}, {203, 179, 167}, {191, 163, 155}, {175, 151, 139},
    {163, 135, 123}, {151, 123, 111}, {135, 111,  95}, {123,  99,  83},
    {107,  87,  71}, { 95,  75,  59}, { 83,  63,  51}, { 67,  51,  39},
    { 55,  43,  31}, { 39,  31,  23}, { 27,  19,  15}, { 15,  11,   7},
    {111, 131, 123}, {103, 123, 111}, { 95, 115, 103}, { 87, 107,  95},
    { 79,  99,  87}, { 71,  91,  79}, { 63,  83,  71}, { 55,  75,  63},
    { 47,  67,  55}, { 43,  59,  47}, { 35,  51,  39}, { 31,  43,  31},
    { 23,  35,  23}, { 15,  27,  19}, { 11,  19,  11}, {  7,  11,   7},
    {255, 243,  27}, {239, 223,  23}, {219, 203,  19}, {203, 183,  15},
    {187, 167,  15}, {171, 151,  11}, {155, 131,   7}, {139, 115,   7},
    {123,  99,   7}, {107,  83,   0}, { 91,  71,   0}, { 75,  55,   0},
    { 59,  43,   0}, { 43,  31,   0}, { 27,  15,   0}, { 11,   7,   0},
    {  0,   0, 255}, { 11,  11, 239}, { 19,  19, 223}, { 27,  27, 207},
    { 35,  35, 191}, { 43,  43, 175}, { 47,  47, 159}, { 47,  47, 143},
    { 47,  47, 127}, { 47,  47, 111}, { 47,  47,  95}, { 43,  43,  79},
    { 35,  35,  63}, { 27,  27,  47}, { 19,  19,  31}, { 11,  11,  15},
    { 43,   0,   0}, { 59,   0,   0}, { 75,   7,   0}, { 95,   7,   0},
    {111,  15,   0}, {127,  23,   7}, {147,  31,   7}, {163,  39,  11},
    {183,  51,  15}, {195,  75,  27}, {207,  99,  43}, {219, 127,  59},
    {227, 151,  79}, {231, 171,  95}, {239, 191, 119}, {247, 211, 139},
    {167, 123,  59}, {183, 155,  55}, {199, 195,  55}, {231, 227,  87},
    {127, 191, 255}, {171, 231, 255}, {215, 255, 255}, {103,   0,   0},
    {139,   0,   0}, {179,   0,   0}, {215,   0,   0}, {255,   0,   0},
    {255, 243, 147}, {255, 247, 199}, {255, 255, 255}, {159,  91,  83}
  };

  //----------

  uint8_t* convert_skin(uint8_t* skin, int size) {
    uint8_t* buffer = (uint8_t*)malloc(4 * size);
    for (int i=0; i<size; i++) {
      uint8_t c = skin[i];
      buffer[i*4  ] = mdl_colormap[c][0]; // b
      buffer[i*4+1] = mdl_colormap[c][1]; // g
      buffer[i*4+2] = mdl_colormap[c][2]; // r
      buffer[i*4+3] = 255;                // a
    }
    return buffer;
  }

  //----------

  //# v1,v2,v3 or v1,v3,v2 ??

  vec3_t* calc_vertex_normals(int fr) {
    int numver = mdl_header.num_verts;
    int numtri = mdl_header.num_tris;
    mdl_vertex_t* vertices = mdl_frames[fr].frame.verts;
    vec3_t* vert_normals = (vec3_t*)malloc(numver * sizeof(vec3_t));

    for (int i=0; i<numver; i++) {
      vert_normals[i] = vec3_t(0,0,0);
    }

    for (int i=0; i<numtri; i++) {
      mdl_triangle_t* tri = &mdl_triangles[i];

      int tri_v1 = tri->vertex[0];
      int tri_v2 = tri->vertex[1];  // 2
      int tri_v3 = tri->vertex[2];  // 1

      mdl_vertex_t* v1 = &vertices[tri_v1];
      mdl_vertex_t* v2 = &vertices[tri_v2];
      mdl_vertex_t* v3 = &vertices[tri_v3];

      vec3_t v_1 = vec3_t(v1->v[0], v1->v[1], v1->v[2]);
      vec3_t v_2 = vec3_t(v2->v[0], v2->v[1], v2->v[2]);
      vec3_t v_3 = vec3_t(v3->v[0], v3->v[1], v3->v[2]);

      vec3_t a = v_3 - v_1;
      vec3_t b = v_2 - v_1;
      vec3_t normal = a.cross(b);

      vert_normals[tri_v1] += normal;
      vert_normals[tri_v2] += normal;
      vert_normals[tri_v3] += normal;
    }

    for (int i=0; i<numver; i++) {
      vert_normals[i].normalize();
    }

    return vert_normals;
  }

  //----------

  //func get_description():
  //	var d = ""
  //	d += "skin_width: " + str(md2.header.skin_width) + "\n"
  //	d += "skin_height: " + str(md2.header.skin_height) + "\n"
  //	d += "frame_size: " + str(md2.header.frame_size) + "\n"
  //	d += "num_skins : " + str(md2.header.num_skins) + "\n"
  //	d += "num_vertices: " + str(md2.header.num_vertices) + "\n"
  //	d += "num_tris: " + str(md2.header.num_tris) + "\n"
  //	d += "num_glcmds: " + str(md2.header.num_glcmds) + "\n"
  //	d += "num_frames: " + str(md2.header.num_frames) + "\n"
  //	d += "skins...\n"
  //	for s in range(md2.header.num_skins):
  //		d += str(s) + ". "
  //		d += md2.skins[s].name
  //		d += " "
  //	d += "\n"
  //	d += "frames...\n"
  //	for f in range(md2.header.num_frames):
  //		d += str(f) + ":"
  //		d += md2.frames[f].name
  //		d += " "
  //	d += "\n"
  //	return d


//------------------------------
private:
//------------------------------

//  enum ResourceTypes {
//    RES_NONE = 0,
////    RES_SKIN_TEX, // model_skin.png
////    RES_VERT_TEX, // model_vert.png
////    RES_FACE_TEX, // model_face.png
////    RES_MESH_TEX, // model_mesh.png
//    RES_MESH,     // ArrayMesh
////    RES_MATERIAL, // ShaderMaterial
////    RES_SHADER,   // Shader
////    RES_SCRIPT,    // Script
//    RES_COUNT
//  };

  //----------

//  void write_tscn(const char* AName) {
//    fprintf(write_fp,"[gd_scene load_steps=%i format=2]\n",RES_COUNT+2);
//    fprintf(write_fp,"\n");
////    fprintf(write_fp,"[ext_resource path=\"res://mmm//%s_skin.png\" type=\"Texture\" id=%i]\n",AName,RES_SKIN_TEX);
////    fprintf(write_fp,"[ext_resource path=\"res://mmm//%s_vert.png\" type=\"Texture\" id=%i]\n",AName,RES_VERT_TEX);
////    fprintf(write_fp,"[ext_resource path=\"res://%s_face.png\" type=\"Texture\" id=%i]\n",AName,RES_FACE_TEX);
////    fprintf(write_fp,"[ext_resource path=\"res://%s_mesh.png\" type=\"Texture\" id=%i]\n",AName,RES_MESH_TEX);
////    fprintf(write_fp,"\n");
//    fprintf(write_fp,"[node name=\"%s\" type=\"MeshInstance\"]\n",AName);
//    fprintf(write_fp,"  mesh = SubResource( %i )\n",RES_MESH);
////    fprintf(write_fp,"  material/0 = SubResource( %i )\n",RES_MATERIAL);
//    fprintf(write_fp,"\n");
//    //  script = ExtResource( RES_SCRIPT )
//    //  __meta__ = {
//    //  "_editor_description_": "blablabla.."
//    //  }
//    fprintf(write_fp,"\n");
//  }

//------------------------------
//
//------------------------------

// x,y,z -> x,z,-y

  // returns 0..1
  // returns 0..255

  vec3_t get_vertex_pos(uint32_t AFrame, uint32_t ATriangle, uint32_t AVertex) {
    int vertex_index = mdl_triangles[ATriangle].vertex[AVertex];
    vec3_t vec = vec3_t(
      mdl_frames[AFrame].frame.verts[vertex_index].v[0],
      mdl_frames[AFrame].frame.verts[vertex_index].v[1],
      mdl_frames[AFrame].frame.verts[vertex_index].v[2]
    );
    //vec = ((vec * mdl_header.scale) + mdl_header.translate);
    //vec *= SAT_INV255F;
    return vec;
  }

  //----------

  // returns vec3 0..1

  vec3_t get_vertex_normal(vec3_t* ANormals, uint32_t ATriangle, uint32_t AVertex) {
    int vertex_index = mdl_triangles[ATriangle].vertex[AVertex];
    vec3_t norm = vec3_t(
      ANormals[vertex_index].x,
      ANormals[vertex_index].y,
      ANormals[vertex_index].z
    );
    return norm;
  }

  //----------

  // returns vec3 (color) 0..1

  vec3_t get_vertex_color(uint32_t ATriangle, uint32_t AVertex) {
    uint32_t vertex_index = (ATriangle * 3) + AVertex;
    uint32_t i1 = (vertex_index & 0xff);
    uint32_t i2 = (vertex_index & 0xff00) >> 8;
    uint32_t i3 = (vertex_index & 0xff0000) >> 16;
    float r = (float)i1 * SAT_INV255F;
    float g = (float)i2 * SAT_INV255F;
    float b = (float)i3 * SAT_INV255F;
    vec3_t vec = vec3_t(r,g,b);
    return vec;
  }

  //----------

  // returns 0..1

  vec2_t get_vertex_uv1(uint32_t ATriangle, uint32_t AVertex) {
    int vertex_index = mdl_triangles[ATriangle].vertex[AVertex];
    vec2_t uv = vec2_t(
      mdl_texcoords[vertex_index].s,
      mdl_texcoords[vertex_index].t
    );
    uv = (uv + 0.5f) / vec2_t(mdl_header.skinwidth,mdl_header.skinheight);
    return uv;
  }

  //----------

  // returns 0..1

  vec2_t get_vertex_uv2(uint32_t ATriangle, uint32_t AVertex) {
    //int vertex_index = mdl_triangles[ATriangle].vertex[AVertex];
    int vertex_index = (ATriangle * 3) + AVertex;
    vec2_t uv = vec2_t(vertex_index,0.0);
    return uv;
  }

  //----------

  // u = ((u + 0.5f) / mdl.header.skin_width);
  // v = 1.0f - ((v + 0.5f) / mdl.header.skin_height);

  vec2_t wrap_vertex_uv(vec2_t uv, uint32_t ATriangle, uint32_t AVertex) {
    int vertex_index = mdl_triangles[ATriangle].vertex[AVertex];
    if (mdl_triangles[ATriangle].facesfront <= 0) {
      if (mdl_texcoords[vertex_index].onseam > 0) {
        //uv.x += (mdl_header.skinwidth * 0.5f);
        uv.x += 0.5f;//(mdl_header.skinwidth * 0.5f);
      }
    }
    return uv;
  }

//------------------------------
//
//------------------------------

  // x,z,-1

  void write_mesh_vertices(uint32_t AFrame) {
    fprintf(write_fp,"    Vector3Array( ");
    for (int i=0; i<mdl_header.num_tris; i++) {
      vec3_t vec1 = get_vertex_pos(AFrame,i,0) * SAT_INV255F;
      vec3_t vec2 = get_vertex_pos(AFrame,i,1) * SAT_INV255F;
      vec3_t vec3 = get_vertex_pos(AFrame,i,2) * SAT_INV255F;
      fprintf(write_fp,"%f, %f, %f, ", vec1.x, vec1.y, vec1.z );
      fprintf(write_fp,"%f, %f, %f, ", vec2.x, vec2.y, vec2.z );
      fprintf(write_fp,"%f, %f, %f",   vec3.x, vec3.y, vec3.z );
      if (i < (mdl_header.num_tris-1)) fprintf(write_fp,", ");
    }
    fprintf(write_fp," ),\n");
  }

  //----------

  void write_mesh_normals(uint32_t AFrame) {
    fprintf(write_fp,"    Vector3Array( ");
    vec3_t* normals = calc_vertex_normals(AFrame);
    for (int i=0; i<mdl_header.num_tris; i++) {
      vec3_t n1 = get_vertex_normal(normals,i,0);
      vec3_t n2 = get_vertex_normal(normals,i,1);
      vec3_t n3 = get_vertex_normal(normals,i,2);
      fprintf(write_fp,"%f, %f, %f, ",n1.x,n1.y,n1.z);
      fprintf(write_fp,"%f, %f, %f, ",n2.x,n2.y,n2.z);
      fprintf(write_fp,"%f, %f, %f",  n3.x,n3.y,n3.z);
      if (i < (mdl_header.num_tris-1)) fprintf(write_fp,", ");
    }
    fprintf(write_fp," ),\n");
    free(normals);
  }

  //----------

  void write_mesh_tangents(uint32_t AFrame) {
    fprintf(write_fp,"    null,\n");
  }

  //----------

  void write_mesh_colors() {
    fprintf(write_fp,"    ColorArray( ");
    for (int32_t i=0; i<mdl_header.num_tris; i++) {
      vec3_t c1 = get_vertex_color(i,0);
      vec3_t c2 = get_vertex_color(i,1);
      vec3_t c3 = get_vertex_color(i,2);
      fprintf(write_fp,"%f, %f, %f, %f, ",c1.x,c1.y,c1.z,1.0f);
      fprintf(write_fp,"%f, %f, %f, %f, ",c2.x,c2.y,c2.z,1.0f);
      fprintf(write_fp,"%f, %f, %f, %f",  c3.x,c3.y,c3.z,1.0f);
      if (i < (mdl_header.num_tris-1)) fprintf(write_fp,", ");
    }
    fprintf(write_fp," ),\n");
  }

  //----------

  void write_mesh_uv1() {
    fprintf(write_fp,"    Vector2Array( ");
    for (int i=0; i<mdl_header.num_tris; i++) {
      vec2_t uv1 = get_vertex_uv1(i,0);
      vec2_t uv2 = get_vertex_uv1(i,1);
      vec2_t uv3 = get_vertex_uv1(i,2);
      uv1 = wrap_vertex_uv(uv1,i,0);
      uv2 = wrap_vertex_uv(uv2,i,1);
      uv3 = wrap_vertex_uv(uv3,i,2);
      fprintf(write_fp,"%f, %f, ",uv1.x,uv1.y);
      fprintf(write_fp,"%f, %f, ",uv2.x,uv2.y);
      fprintf(write_fp,"%f, %f",  uv3.x,uv3.y);
      if (i < (mdl_header.num_tris-1)) fprintf(write_fp,", ");
    }
    fprintf(write_fp," ),\n");
  }

  //----------

  void write_mesh_uv2() {
    fprintf(write_fp,"    Vector2Array( ");
    for (int i=0; i<mdl_header.num_tris; i++) {
      vec2_t uv1 = get_vertex_uv2(i,0);
      vec2_t uv2 = get_vertex_uv2(i,1);
      vec2_t uv3 = get_vertex_uv2(i,2);
      fprintf(write_fp,"%f, %f, ",uv1.x,uv1.y);
      fprintf(write_fp,"%f, %f, ",uv2.x,uv2.y);
      fprintf(write_fp,"%f, %f",  uv3.x,uv3.y);
      if (i < (mdl_header.num_tris-1)) fprintf(write_fp,", ");
    }
    fprintf(write_fp," ),\n");
  }

  //----------

  void write_mesh_bones() {
    fprintf(write_fp,"    null,\n");
  }

  //----------

  void write_mesh_weights() {
    fprintf(write_fp,"    null,\n");
  }

  //----------

  void write_mesh_indices() {
    fprintf(write_fp,"    IntArray( ");
    for (int i=0; i<mdl_header.num_tris; i++) {
      int v1 = (i * 3);
      int v2 = (i * 3) + 1;
      int v3 = (i * 3) + 2;
      fprintf(write_fp,"%i, %i, %i",v1,v2,v3);
      if (i < (mdl_header.num_tris-1)) fprintf(write_fp,", ");
    }
    fprintf(write_fp," )\n");
  }

  //----------

  void write_mesh(const char* AName) {
    //printf("Writing mesh: %s.tres\n",AName);
    char filename[256];
    sprintf(filename,"%s.tres",AName);
    write_fp = fopen(filename,"wt");
    fprintf(write_fp,"[gd_resource type=\"ArrayMesh\"]\n");
    fprintf(write_fp,"[resource]\n");
    fprintf(write_fp,"surfaces/0 = {\n");
    fprintf(write_fp,"  \"primitive\":4,\n");
    fprintf(write_fp,"  \"arrays\":[\n");
    write_mesh_vertices(0);
    write_mesh_normals(0);
    write_mesh_tangents(0);
    write_mesh_colors();
    write_mesh_uv1();
    write_mesh_uv2();
    write_mesh_bones();
    write_mesh_weights();
    write_mesh_indices();
    fprintf(write_fp,"  ],\n");
    fprintf(write_fp,"  \"morph_arrays\":[]\n");
    fprintf(write_fp,"}\n");
    fprintf(write_fp,"\n");
    fclose(write_fp);
  }

  //----------

//  void write_material() {
//    fprintf(write_fp,"\n");
//    fprintf(write_fp,"[sub_resource type=\"ShaderMaterial\" id=%i]\n",-1/*RES_MATERIAL*/);
//    //  shader = SubResource( 2 )
//    fprintf(write_fp,"  shader_param/scale     = Vector3( 1, 1, 1 )\n");
//    fprintf(write_fp,"  shader_param/translate = Vector3( 0, 0, 0 )\n");
////    fprintf(write_fp,"  shader_param/vat_skin  = ExtResource( %i )\n",RES_SKIN_TEX);
////    fprintf(write_fp,"  shader_param/vat_vert  = ExtResource( %i )\n",RES_VERT_TEX);
////    fprintf(write_fp,"  shader_param/vat_face  = ExtResource( %i )\n",RES_FACE_TEX);
////    fprintf(write_fp,"  shader_param/vat_mesh  = ExtResource( %i )\n",RES_MESH_TEX);
//    fprintf(write_fp,"\n");
//  }

  //----------

//  void write_script(int id) {
//    //  [ext_resource path="res://model.gd" type="Script" id=4]
//    //fprintf(write_fp,"[ext_resource path=\"res://model.gd\" type=\"Script\" id=%i]\n",id);
//  }

  //----------

  uint8_t* write_vec8(uint8_t* ptr, vec3_t vec) {
    ptr[0] = int(vec.x) & 0xff;
    ptr[1] = int(vec.y) & 0xff;
    ptr[2] = int(vec.z) & 0xff;
    ptr[3] = 0;//255;
    return (ptr + 4);
  }

  //----------

  void write_vertex_frames(const char* AName) {
    //printf("Writing vertex frames: %s_vertices.png\n",AName);
    char filename[256];
    sprintf(filename,"%s_vertex_frames.png",AName);
    uint32_t num_tris = mdl_header.num_tris;
    uint32_t num_frames = mdl_header.num_frames;
    uint8_t* buffer = (uint8_t*)malloc(num_tris*3*num_frames*4);
    uint8_t* ptr = buffer;
    for (int i=0; i<mdl_header.num_frames; i++) {
      for (int j=0; j<mdl_header.num_tris; j++) {
        vec3_t v1 = get_vertex_pos(i,j,0);
        vec3_t v2 = get_vertex_pos(i,j,1);
        vec3_t v3 = get_vertex_pos(i,j,2);
        ptr = write_vec8(ptr,v1);
        ptr = write_vec8(ptr,v2);
        ptr = write_vec8(ptr,v3);
      }
    }
    stbi_write_png(filename,num_tris*3,num_frames,4,buffer,(num_tris*3*4));
    free(buffer);
  }

  //----------

  void write_normal_frames(const char* AName) {
    //printf("Writing normal frames: %s_normal_frames.png\n",AName);
    char filename[256];
    sprintf(filename,"%s_normal_frames.png",AName);
    uint32_t num_tris = mdl_header.num_tris;
    uint32_t num_frames = mdl_header.num_frames;
    uint8_t* buffer = (uint8_t*)malloc(num_tris*3*4*num_frames);
    uint8_t* ptr = buffer;
    for (uint32_t i=0; i<num_frames; i++) {
      //SAT_Print("  - frame %i\n",i);
      vec3_t* normals = calc_vertex_normals(i);
      for (uint32_t j=0; j<num_tris; j++) {
        vec3_t n1 = (get_vertex_normal(normals,i,0) * 127.0f) + 128.0f;
        vec3_t n2 = (get_vertex_normal(normals,i,1) * 127.0f) + 128.0f;
        vec3_t n3 = (get_vertex_normal(normals,i,2) * 127.0f) + 128.0f;
        write_vec8(ptr,n1);
        ptr += 4;
        write_vec8(ptr,n2);
        ptr += 4;
        write_vec8(ptr,n3);
        ptr += 4;
      }
      free(normals);
    }
    stbi_write_png(filename,num_tris*3,num_frames,4,buffer,(num_tris*3*4));
    free(buffer);
  }

  //----------

//  void write_shader() {
//    fprintf(write_fp,"\n");
//    fprintf(write_fp,"[sub_resource type=\"Shader\" id=%i]\n",-1/*RES_SHADER*/);
//    //  shader = SubResource( 2 )
//    fprintf(write_fp,"  code = \"\n");
//    fprintf(write_fp,"    shader_type spatial;\n");
//    fprintf(write_fp,"\n");
//    fprintf(write_fp,"    uniform vec3       scale = vec3(1,1,1);\n");
//    fprintf(write_fp,"    uniform vec3       translate = vec3(0,0,0);\n");
//    fprintf(write_fp,"    uniform sampler2D  skin_texture;\n");
//    fprintf(write_fp,"    uniform sampler2D  vertex_texture;\n");
//    fprintf(write_fp,"    uniform sampler2D  normal_texture;\n");
//    fprintf(write_fp,"\n");
//    fprintf(write_fp,"    void vertex() {\n");
//    fprintf(write_fp,"\n");
//    fprintf(write_fp,"    }\n");
//    fprintf(write_fp,"\n");
//    fprintf(write_fp,"    void fragment() {\n");
//    fprintf(write_fp,"    }\n");
//    fprintf(write_fp,"  \"\n");
//  }

  //----------

  void write_skin(const char* AName, uint32_t ASkin) {
    //printf("Writing skin(%i): %s.png\n",ASkin,AName);
    char filename[256];
    sprintf(filename,"%s_vertices.png",AName);
    //int num = mdl_header.num_skins;
    //printf("writing %i skins to '%s'\n",num, filename);
    int width = mdl_header.skinwidth;
    int height = mdl_header.skinheight;
    uint8_t* buffer = convert_skin(mdl_skins[ASkin].data,(width*height));
    stbi_write_png(
      filename,
      width,
      height,
      4,          // 1=C, 2=CA, 3=RGB, 4=RGBA
      buffer,     //mdl_skins[num].data,
      width * 4   // stride (bytes)
    );
    free(buffer);
  }

  //----------

  void write_skins(const char* AName) {
    //printf("Writing skins: %s.png\n",AName);
    char filename[256];
    sprintf(filename,"%s_skins.png",AName);
    int num = mdl_header.num_skins;
    //printf("writing %i skins to '%s'\n",num, filename);
    int width = mdl_header.skinwidth;
    int height = mdl_header.skinheight;
    int size = width * height * 4;
    uint8_t* buffer = (uint8_t*)malloc(num * size);
    uint8_t* ptr = buffer;
    for (int i=0; i<num; i++) {
      uint8_t* temp = convert_skin(mdl_skins[i].data,(width*height));
      memcpy(ptr,temp,size);
      ptr += size;
      free(temp);
    }
    //int res =
    stbi_write_png(
      filename,
      width,
      height * num,
      4,          // 1=C, 2=CA, 3=RGB, 4=RGBA
      buffer,     //mdl_skins[num].data,
      width * 4   // stride (bytes)
    );
    free(buffer);
  }

  //----------

};

//----------------------------------------------------------------------
#endif
