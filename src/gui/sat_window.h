#ifndef sat_window_included
#define sat_window_included
//----------------------------------------------------------------------

// mouse events
// resizing
// buffer
// painting

#include "base/sat.h"
#include "gui/sat_surface.h"
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

  //SAT_Surface*        MBufferSurface  = nullptr;
  //SAT_Widget*         MRootWidget     = nullptr;

  //uint32_t            MWindowWidth    = 0;
  //uint32_t            MWindowHeight   = 0;
  //const char*         MWindowTitle    = "SAT_Window";
  //intptr_t            MWindowParent   = 0;

  //uint32_t            MBufferWidth    = 0;
  //uint32_t            MBufferHeight   = 0;

//------------------------------
public:
//------------------------------

  SAT_Window(uint32_t AWidth, uint32_t AHeight, intptr_t AParent)
  : SAT_ImplementedWindow(AWidth,AHeight,AParent) {
  }

  //----------

  virtual ~SAT_Window() {
  }

//------------------------------
public:
//------------------------------

  //void setPos(uint32_t AXpos, uint32_t AYpos)
  //void setSize(uint32_t AWidth, uint32_t AHeight)
  //void setTitle(const char* ATitle)
  //void setParent(intptr_t AParent)
  //void show()
  //void hide()

  //void reparent(intptr_t AParent)
  //void invalidate(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight)
  //void sendClientMessage(uint32_t AData, uint32_t AType)
  //void startEventThread()
  //void stopEventThread()
  //uint32_t eventLoop()
  //void beginPaint()
  //void endPaint()
  //void paint()
  //void paint(int32_t x, int32_t y, int32_t w, int32_t h)
  //void fill(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight, uint32_t AColor)
  //void fill(uint32_t AColor) {














};

//----------------------------------------------------------------------
#endif
