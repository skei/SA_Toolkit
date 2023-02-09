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
  //SAT_Widget*         MRootWidget     = nullptr;

  //uint32_t            MWindowWidth    = 0;
  //uint32_t            MWindowHeight   = 0;
  //const char*         MWindowTitle    = "SAT_Window";
  //intptr_t            MWindowParent   = 0;

  //uint32_t            MBufferWidth    = 0;
  //uint32_t            MBufferHeight   = 0;

  bool                MFillBackground   = true;
  SAT_Color           MBackgroundColor  = SAT_DarkGray;

//------------------------------
public:
//------------------------------

  SAT_Window(uint32_t AWidth, uint32_t AHeight, intptr_t AParent)
  : SAT_ImplementedWindow(AWidth,AHeight,AParent) {
    #ifdef SAT_LINUX
      Display*      display = getX11Display();
      xcb_window_t  window  = getX11Window();
      MOpenGL = new SAT_OpenGL(display,window);
    #endif
    MScreenPainter = new SAT_Painter(MOpenGL);
  }

  //----------

  virtual ~SAT_Window() {
    delete MOpenGL;
  }

//------------------------------
public:
//------------------------------

  void setFillBackground(bool AFill=true)   { MFillBackground = AFill; }
  void setBackgroundColor(SAT_Color AColor) { MBackgroundColor = AColor; }

//------------------------------
public:
//------------------------------

  void on_window_paint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) override {
    MOpenGL->beginPaint(0,0,MWidth,MHeight);
    MScreenPainter->beginFrame(MWidth,MHeight,1.0);
    if (MFillBackground) {
      MScreenPainter->setFillColor(MBackgroundColor);
      //MScreenPainter->fillRect(AXpos,AYpos,AWidth,AHeight);
      MScreenPainter->fillRect(0,0,MWidth,MHeight);
    }
    MScreenPainter->endFrame();
    MOpenGL->endPaint();
  }

};

//----------------------------------------------------------------------
#endif
