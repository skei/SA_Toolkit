#ifndef sat_window_included
#define sat_window_included
//----------------------------------------------------------------------

// mouse events
// resizing
// buffer
// painting

#include "base/sat.h"
#include "gui/sat_painter.h"
#include "gui/sat_widget.h"
#include "gui/sat_widget_listener.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#ifdef SAT_LINUX
  #include "gui/x11/sat_x11_window.h"
  typedef SAT_X11Window SAT_ImplementedWindow;
#endif

#ifdef SAT_WIN32
  #include "gui/win32/sat_win32_window.h"
  typedef SAT_Win32Window SAT_ImplementedWindow;
#endif

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Window
: public SAT_ImplementedWindow
, public SAT_WidgetListener {

//------------------------------
private:
//------------------------------

  SAT_OpenGL*         MOpenGL         = nullptr;
  SAT_Painter*        MScreenPainter  = nullptr;
  void*               MRenderBuffer   = nullptr;


  //SAT_Widget*         MRootWidget     = nullptr;

  //uint32_t            MWindowWidth    = 0;
  //uint32_t            MWindowHeight   = 0;
  //const char*         MWindowTitle    = "SAT_Window";
  //intptr_t            MWindowParent   = 0;

  uint32_t            MBufferWidth    = 0;
  uint32_t            MBufferHeight   = 0;

  bool                MFillBackground   = true;
  SAT_Color           MBackgroundColor  = SAT_Grey;

//------------------------------
public:
//------------------------------

  SAT_Window(uint32_t AWidth, uint32_t AHeight, intptr_t AParent)
  : SAT_ImplementedWindow(AWidth,AHeight,AParent) {

//    MBufferWidth = AWidth;
//    MBufferHeight = AHeight;

    #ifdef SAT_LINUX
      Display* display = getX11Display();
      xcb_window_t window  = getX11Window();
      MOpenGL = new SAT_OpenGL(display,window);
    #endif

    MScreenPainter = new SAT_Painter(MOpenGL);

    MOpenGL->makeCurrent();
    //MRenderBuffer = MScreenPainter->createRenderBuffer(AWidth,AHeight);
    createBuffer(AWidth,AHeight);
    MOpenGL->resetCurrent();

    //SAT_Print("MRenderBuffer %p\n",MRenderBuffer);
  }

  //----------

  virtual ~SAT_Window() {
    //MScreenPainter->deleteRenderBuffer(MRenderBuffer);
    deleteBuffer();
    delete MScreenPainter;
    delete MOpenGL;
  }

//------------------------------
public:
//------------------------------

  void createBuffer(uint32_t AWidth, uint32_t AHeight) {
    uint32_t width = SAT_NextPowerOfTwo(AWidth);
    uint32_t height = SAT_NextPowerOfTwo(AHeight);
    MBufferWidth = width;
    MBufferHeight = height;
    MRenderBuffer = MScreenPainter->createRenderBuffer(width,height);
  }

  //----------

  void deleteBuffer() {
    if (MRenderBuffer) {
      MScreenPainter->deleteRenderBuffer(MRenderBuffer);
      MRenderBuffer = nullptr;
    }
  }

  //----------

  void resizeBuffer(uint32_t AWidth, uint32_t AHeight) {
    if (!MRenderBuffer) {
      createBuffer(AWidth,AHeight);
    }
    else {
      uint32_t new_width  = SAT_NextPowerOfTwo(AWidth);
      uint32_t new_height = SAT_NextPowerOfTwo(AHeight);
      if ((new_width != MBufferWidth) || (new_height != MBufferHeight)) {
        // create new buffer
        void* new_buffer = MScreenPainter->createRenderBuffer(new_width,new_height);
        // copy old to new buffer
        MScreenPainter->selectRenderBuffer(new_buffer,new_width,new_height);
        MScreenPainter->beginFrame(new_width,new_height,1.0);
        int32_t image = MScreenPainter->getImageFromRenderBuffer(MRenderBuffer);
        MScreenPainter->setFillImage(image, 0,0, 1,1, 1.0, 0.0);
        MScreenPainter->fillRect(0,0,MBufferWidth,MBufferHeight);
        MScreenPainter->endFrame();
        MScreenPainter->selectRenderBuffer(nullptr);
        // delete old buffer
        MScreenPainter->deleteRenderBuffer(MRenderBuffer);
        // switch
        MRenderBuffer = new_buffer;
        MBufferWidth  = new_width;
        MBufferHeight = new_height;
      }
    }
  }

//------------------------------
public:
//------------------------------

  void setFillBackground(bool AFill=true)   { MFillBackground = AFill; }
  void setBackgroundColor(SAT_Color AColor) { MBackgroundColor = AColor; }

//------------------------------
public:
//------------------------------

//  void on_window_resize(int32_t AWidth, int32_t AHeight) override {
//    SAT_Print("%i,%i\n",AWidth,AHeight);
//    //MOpenGL->makeCurrent();
//    //resizeBuffer(AWidth,AHeight);
//    //MOpenGL->resetCurrent();
//  }

  //----------

  void on_window_paint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) override {
    MOpenGL->makeCurrent();

    //SAT_Print("window %i,%i buffer %i,%i\n",MWindowWidth,MWindowHeight,MBufferWidth,MBufferHeight);
    if ((MWindowWidth != MBufferWidth) || (MWindowHeight != MBufferHeight)) {
      resizeBuffer(MWindowWidth,MWindowHeight);
    }

    MScreenPainter->selectRenderBuffer(MRenderBuffer,MBufferWidth,MBufferHeight);
    MScreenPainter->beginFrame(MBufferWidth,MBufferHeight,1.0);

      if (MFillBackground) {
        MScreenPainter->setFillColor(MBackgroundColor);
        MScreenPainter->fillRect(0,0,MWindowWidth,MWindowHeight);
      }

      //paintChildren();
      {
        // cross
        MScreenPainter->setDrawColor(SAT_White);
        MScreenPainter->setLineWidth(5);
        double w = MWindowWidth;
        double h = MWindowHeight;
        MScreenPainter->drawLine(0,0, w,h);
        MScreenPainter->drawLine(w,0, 0,h);
        // rect
        MScreenPainter->setFillColor(SAT_Red);
        MScreenPainter->fillRect(50,50,100,100);
      }

    MScreenPainter->endFrame();

    // paint render buffer to screen
    MScreenPainter->selectRenderBuffer(nullptr,MWindowWidth,MWindowHeight);
    MScreenPainter->beginFrame(MWindowWidth,MWindowHeight,1.0);
    int32_t image = MScreenPainter->getImageFromRenderBuffer(MRenderBuffer);
    MScreenPainter->setFillImage(image, 0,0, 1,1, 1.0, 0.0);
    MScreenPainter->fillRect(0,0,MWindowWidth,MWindowHeight);
    MScreenPainter->endFrame();

    MOpenGL->swapBuffers();
    MOpenGL->resetCurrent();

  }

};

//----------------------------------------------------------------------
#endif
