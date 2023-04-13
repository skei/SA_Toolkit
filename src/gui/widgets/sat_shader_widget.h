#ifndef sat_shader_widget_included
#define sat_shader_widget_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "plugin/sat_parameter.h"
#include "gui/widgets/sat_panel_widget.h"

#ifdef SAT_LINUX
  //#include <GL/gl.h>
  #include "gui/sat_opengl.h"
#endif

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

  const char* vsh = \
    "#version 430\n"
    "layout (location=0) in vec2 inVer;"
    "out vec2 p;"
    "out gl_PerVertex"
    "{"
    "vec4 gl_Position;"
    "};"
    "void main()"
    "{"
    "gl_Position=vec4(inVer,0.0,1.0);"
    "p=inVer;"
    "}";

  const char* fsh = \
    "#version 430\n"
    "layout (location=0) uniform vec4 fpar[4];"
    "layout (location=0) out vec4 co;"
    "in vec2 p;"
    "float iSphere(in vec3 ro, in vec3 rd, in vec4 sph)"
    "{"
    "vec3  q = ro - sph.xyz;"
    "float b = dot( q, rd );"
    "float c = dot( q, q ) - sph.w*sph.w;"
    "float h = b*b - c;"
    "if( h>0.0 ) h = -b - sqrt( h );"
    "return h;"
    "}"
    "void main()"
    "{"
    "vec3 w=normalize(fpar[1].xyz-fpar[0].xyz);"
    "vec3 u=normalize(cross(w,vec3(0.0,1.0,0.0)));"
    "vec3 v=cross(u,w);"
    "vec3 ro = fpar[0].xyz;"
    "vec3 rd = normalize( p.x*u*1.77 + p.y*v + 0.75*w );"
    "float dif = dot( p, vec2(0.707) );"
    "float t = iSphere(ro,rd,fpar[2]);"
    "if(t>0.0)"
    "{"
    "vec3 pos = ro + t*rd;"
    "vec3 nor = normalize( pos - fpar[2].xyz );"
    "dif = dot(nor,vec3(0.57703));"
    "}"
    "co = dif*vec4(0.5,0.4,0.3,0.0) + vec4(0.5,0.5,0.5,1.0);"
    "}";

    const GLfloat g_vertex_buffer_data[] = {
//      -0.9f, -0.9f, 0.0f,
//       0.9f, -0.9f, 0.0f,
//       0.9f,  0.9f, 0.0f,
//      -0.9f,  0.9f, 0.0f,

       100, 100, 0,
       200, 100, 0,
       200, 200, 0,
       100, 200, 0,
    };

    const GLuint g_index_buffer_data[] = {
      0, 1, 2,
      0, 2, 3
    };

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_ShaderWidget
: public SAT_PanelWidget {

//------------------------------
private:
//------------------------------

  bool          MDrawShader = false;
  
  unsigned int  ProgramPipelineID;
  int           VertexShaderID;
  int           FragmentShaderID;
  
  GLuint        VertexBufferID;
  GLuint        IndexBufferID;    

//------------------------------
public:
//------------------------------

  SAT_ShaderWidget(SAT_Rect ARect)
  : SAT_PanelWidget(ARect) {
    setName("SAT_ShaderWidget");
    setFillBackground(true);
  }
  
  //----------

  virtual ~SAT_ShaderWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setDrawShader(bool ADraw=true) {
    MDrawShader = ADraw;
  }
  
  //----------

  void init() {
    
//    #if (SAT_OPENGL_MAJOR >= 4) and (SAT_OPENGL_MINOR >= 1)
//    
//      VertexShaderID = glCreateShaderProgramv( GL_VERTEX_SHADER,   1, &vsh );
//      FragmentShaderID = glCreateShaderProgramv( GL_FRAGMENT_SHADER, 1, &fsh );
//      
//      glGenProgramPipelines(1,&ProgramPipelineID);
//      glBindProgramPipeline(ProgramPipelineID);
//      glUseProgramStages(ProgramPipelineID, GL_VERTEX_SHADER_BIT, VertexShaderID);
//      glUseProgramStages(ProgramPipelineID, GL_FRAGMENT_SHADER_BIT, FragmentShaderID);
//      glBindProgramPipeline(0);
//      
//      glGenBuffers(1,&VertexBufferID);
//      glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
//      glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
//      glBindBuffer(GL_ARRAY_BUFFER, 0);    
//
//      glGenBuffers(1,&IndexBufferID);
//      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferID);
//      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_index_buffer_data), g_index_buffer_data, GL_DYNAMIC_DRAW);    
//      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
//    
//    #else
//    
//      #warning needs opengl 4.1
//      
//    #endif
    
  }
  
//------------------------------
public:
//------------------------------

  virtual void drawShader(SAT_PaintContext* AContext) {
    SAT_Assert(AContext);
    
    //SAT_PRINT;
    
    if (MDrawShader) {
      
//      glBindProgramPipeline(ProgramPipelineID);
//      glUseProgramStages(ProgramPipelineID, GL_VERTEX_SHADER_BIT, VertexShaderID);
//      glUseProgramStages(ProgramPipelineID, GL_FRAGMENT_SHADER_BIT, FragmentShaderID);
//    
//      glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
//      glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
//
//      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferID);
//      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_index_buffer_data), g_index_buffer_data, GL_DYNAMIC_DRAW);    
      
      //double S = getWindowScale();
      SAT_Painter* painter = AContext->painter;
      SAT_Assert(painter);
      SAT_Rect mrect = getRect();
      //shrink..
      if (mrect.w <= 0.0) return;
      if (mrect.h <= 0.0) return;
      
      //---

//      float fparams[4*4];
//      float t  = 0;
//      // camera position
//      fparams[ 0] = 2.0f * sinf(1.0f * t + 0.1f);
//      fparams[ 1] = 0.0f;
//      fparams[ 2] = 2.0f * cosf(1.0f * t);
//      // camera target
//      fparams[ 4] = 0.0f;
//      fparams[ 5] = 0.0f;
//      fparams[ 6] = 0.0f;
//      // sphere
//      fparams[ 8] = 0.0f;
//      fparams[ 9] = 0.0f;
//      fparams[10] = 0.0f;
//      fparams[11] = 1.0f;
//      glProgramUniform4fv(FragmentShaderID,0,4,fparams);

      //glRects( -1, -1, 1, 1 );

//      glBindProgramPipeline(ProgramPipelineID);
//      glUseProgramStages(ProgramPipelineID, GL_VERTEX_SHADER_BIT, VertexShaderID);
//      glUseProgramStages(ProgramPipelineID, GL_FRAGMENT_SHADER_BIT, FragmentShaderID);
//
//      glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
//      glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
//      glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
//
//      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferID);
//      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferID);
//      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_index_buffer_data), g_index_buffer_data, GL_DYNAMIC_DRAW);    
//      
//      glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,nullptr);   // crashes..
//      
//      glBindProgramPipeline(0);
//      glBindBuffer(GL_ARRAY_BUFFER, 0);    
//      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
      

      //---
      
    }
  }


//------------------------------
public:
//------------------------------

  void on_widget_paint(SAT_PaintContext* AContext) override {
    drawDropShadow(AContext);
    fillBackground(AContext);
    drawShader(AContext);
    paintChildWidgets(AContext);
    drawBorder(AContext);
  }

};

















//----------------------------------------------------------------------
#endif




#if 0

void intro_do( long time ) {
  const float t  = 0.001f*(float)time;
  // camera position
  fparams[ 0] = 2.0f*sinf(1.0f*t+0.1f);
  fparams[ 1] = 0.0f;
  fparams[ 2] = 2.0f*cosf(1.0f*t);
  // camera target
  fparams[ 4] = 0.0f;
  fparams[ 5] = 0.0f;
  fparams[ 6] = 0.0f;
  // sphere
  fparams[ 8] = 0.0f;
  fparams[ 9] = 0.0f;
  fparams[10] = 0.0f;
  fparams[11] = 1.0f;
  //--- render -----------------------------------------
  oglProgramUniform4fv( fsid, 0, 4, fparams );
  glRects( -1, -1, 1, 1 );
}

#define oglCreateShaderProgramv ((PFNGLCREATESHADERPROGRAMVPROC)myglfunc[0])
#define oglGenProgramPipelines  ((PFNGLGENPROGRAMPIPELINESPROC)myglfunc[1])
#define oglBindProgramPipeline  ((PFNGLBINDPROGRAMPIPELINEPROC)myglfunc[2])
#define oglUseProgramStages     ((PFNGLUSEPROGRAMSTAGESPROC)myglfunc[3])
#define oglProgramUniform4fv    ((PFNGLPROGRAMUNIFORM4FVPROC)myglfunc[4])

#ifdef DEBUG
#define oglGetProgramiv         ((PFNGLGETPROGRAMIVPROC)myglfunc[5])
#define oglGetProgramInfoLog    ((PFNGLGETPROGRAMINFOLOGPROC)myglfunc[6])
#endif

static char *glFuncNames[] = {
    "glCreateShaderProgramv",
    "glGenProgramPipelines",
    "glBindProgramPipeline",
    "glUseProgramStages",
    "glProgramUniform4fv" };

extern void *myglfunc[];

//...
  for( int i=0; i<5; i++ ) {
    myglfunc[i] = wglGetProcAddress( glFuncNames[i] );
    if ( !myglfunc[i] ) return;
  }
  if( !intro_init() ) return;
  // play intro
  long t;
  long to = timeGetTime();
  do {
    t = timeGetTime() - to;
    intro_do( t );
    wglSwapLayerBuffers( hDC, WGL_SWAP_MAIN_PLANE ); //SwapBuffers( hDC );
  } while ( !GetAsyncKeyState(VK_ESCAPE) && t<(MZK_DURATION*1000) );

  #ifdef CLEANDESTROY
    sndPlaySound(0,0);
    ChangeDisplaySettings( 0, 0 );
    ShowCursor(1);
  #endif

  ExitProcess(0);
}

#endif // 0
