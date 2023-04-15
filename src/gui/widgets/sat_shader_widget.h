#ifndef sat_shader_widget_included
#define sat_shader_widget_included
//----------------------------------------------------------------------

/*
  https://gist.github.com/kazt81/1526616
  
  Triangle_opengl_3_1
  A cross platform version of
  http://www.opengl.org/wiki/Tutorial:_OpenGL_3.1_The_First_Triangle_%28C%2B%2B/Win%29
  with some code from http://www.lighthouse3d.com/opengl/glsl/index.php?oglexample1
  and from the book OpenGL Shading Language 3rd Edition, p215-216
  Daniel Livingstone, October 2010
*/  

//----------------------------------------------------------------------

#include "base/sat.h"
#include "plugin/sat_parameter.h"
#include "gui/widgets/sat_panel_widget.h"

#ifdef SAT_LINUX
  #include "gui/sat_opengl.h"
#endif

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

  bool MDrawShader = true;
  
  //----------
  
  const char* vertex_shader_source =
    "//#version 130"
    "in vec3 in_Position;"
    "in vec3 in_Color;"
    "out vec3 ex_Color;"
    "void main(void) {"
    "  ex_Color = in_Color;"
    "  gl_Position = vec4(in_Position, 1.0);"
    "}";

  const char* fragment_shader_source =
    "//#version 130"
    "precision highp float; // needed only for version 1.30"
    "in vec3 ex_Color;"
    "out vec4 out_Color;"
    "void main(void) {"
    "  out_Color = vec4(ex_Color,1.0);"
    "}";

  GLfloat vertices[9] = {
    0.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f
  };
  
  GLfloat vertices2[9] = {
       0.0f,   0.0f, 0.0f,
       0.0f, 100.0f, 0.0f,
     100.0f,   0.0f, 0.0f
  };

  GLfloat colours[9] = {
       1.0f,   0.0f,   0.0f,
       0.0f,   1.0f,   0.0f,
       0.0f,   0.0f,   1.0f
  };
  
  unsigned int  vertex_array_object[2];
  unsigned int  vertex_buffer_object[3];

  GLuint        vertex_shader;
  GLuint        fragment_shader;
  GLuint        shader_program;
  

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
    // cleanup shaders..
  }

//------------------------------
public:
//------------------------------


//------------------------------
public:
//------------------------------

  virtual void setDrawShader(bool ADraw=true) {
    MDrawShader = ADraw;
  }
  
  //----------

  // Display (hopefully) useful error messages if shader fails to compile
  
  void printShaderInfoLog(GLint shader) {
    int infoLogLen = 0;
    int charsWritten = 0;
    GLchar *infoLog;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);
    // should additionally check for OpenGL errors here
    if (infoLogLen > 0) {
      infoLog = new GLchar[infoLogLen];
      // error check for fail to allocate memory omitted
      glGetShaderInfoLog(shader,infoLogLen, &charsWritten, infoLog);
      SAT_Print("InfoLog: %s\n",infoLog);
      delete [] infoLog;
    }
    // should additionally check for OpenGL errors here
  }
  
  //----------

  void init(void) {
    // Allocate Vertex Array Objects
    
    glGenVertexArrays(2,&vertex_array_object[0]);
    
    // first Vertex Array Object
    glBindVertexArray(vertex_array_object[0]);
    glGenBuffers(2,vertex_buffer_object);
    // VBO for vertex data
    glBindBuffer(GL_ARRAY_BUFFER,vertex_buffer_object[0]);
    glBufferData(GL_ARRAY_BUFFER,9*sizeof(GLfloat),vertices,GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)0,3,GL_FLOAT,GL_FALSE,0,0); 
    glEnableVertexAttribArray(0);
    // VBO for colour data
    glBindBuffer(GL_ARRAY_BUFFER,vertex_buffer_object[1]);
    glBufferData(GL_ARRAY_BUFFER,9*sizeof(GLfloat),colours,GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)1,3,GL_FLOAT,GL_FALSE,0,0);
    glEnableVertexAttribArray(1);
    // second Vertex Array Object
    glBindVertexArray(vertex_array_object[1]);
    glGenBuffers(1,&vertex_buffer_object[2]);
    // VBO for vertex data
    glBindBuffer(GL_ARRAY_BUFFER,vertex_buffer_object[2]);
    glBufferData(GL_ARRAY_BUFFER,9*sizeof(GLfloat),vertices2,GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)0,3,GL_FLOAT,GL_FALSE,0,0); 
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
  }

  //----------

  void initShaders(void) {
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);	
    GLint vlen = strlen(vertex_shader_source);
    GLint flen = strlen(fragment_shader_source);
    glShaderSource(vertex_shader,1,&vertex_shader_source,&vlen);
    glShaderSource(fragment_shader,1,&fragment_shader_source,&flen);

    GLint compiled;

    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader,GL_COMPILE_STATUS,&compiled);
    if (!compiled) {
      SAT_Print("Vertex shader not compiled.\n");
      printShaderInfoLog(vertex_shader);
    } 
    else {
      SAT_Print("Vertex shader compiled.\n");
    }

    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
      SAT_Print("Fragment shader not compiled.\n");
      printShaderInfoLog(fragment_shader);
    } 
    else {
      SAT_Print("Fragment shader compiled.\n");
    }
    
    shader_program = glCreateProgram();
    glBindAttribLocation(shader_program,0, "in_Position");
    glBindAttribLocation(shader_program,1, "in_Color");
    glAttachShader(shader_program,vertex_shader);
    glAttachShader(shader_program,fragment_shader);
    glLinkProgram(shader_program);

//    glUseProgram(shader_program);
//    glUseProgram(0);

  }
  
  //----------

  void draw_using_shader() {
    SAT_Print("shading!\n");
    //glViewport(0,0,1000,1000);
    //glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shader_program);
    glBindVertexArray(vertex_array_object[0]);	// First VAO
    glDrawArrays(GL_TRIANGLES, 0, 3);	          // draw first object
    glBindVertexArray(vertex_array_object[1]);	// select second VAO
    glVertexAttrib3f((GLuint)1, 1.0, 0.0, 0.0); // set constant color attribute
    glDrawArrays(GL_TRIANGLES, 0, 3);	          // draw second object
    glBindVertexArray(0);
    //glUseProgram(0);
    //glutSwapBuffers();
  }

  //----------


  //----------

  //int main (int argc, char* argv[]) {
  //  glutInit(&argc, argv);
  //  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  //  glutInitWindowSize(600,600);
  //  glutCreateWindow("Triangle Test");
  //  glewInit();
  //  GLenum err = glewInit();
  //  if (GLEW_OK != err) {
  //    /* Problem: glewInit failed, something is seriously wrong. */
  //    cout << "glewInit failed, aborting." << endl;
  //    exit (1);
  //  }
  //  cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << endl;
  //  cout << "OpenGL version " << glGetString(GL_VERSION) << " supported" << endl;
  //  init();
  //  initShaders();
  //  glutDisplayFunc(display);
  //  glutReshapeFunc(reshape);	
  //  glutMainLoop();
  //  return 0;
  //}
  
//------------------------------
public:
//------------------------------

  virtual void drawShader(SAT_PaintContext* AContext) {
    SAT_Assert(AContext);
    if (MDrawShader) {
      //double S = getWindowScale();
      SAT_Painter* painter = AContext->painter;
      SAT_Assert(painter);
      SAT_Rect mrect = getRect();
      //shrink..
      if (mrect.w <= 0.0) return;
      if (mrect.h <= 0.0) return;
      
      //painter->setFillColor(SAT_White);
      //painter->fillRect(mrect.x,mrect.y,mrect.w,mrect.h);
      
      draw_using_shader();
      
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
