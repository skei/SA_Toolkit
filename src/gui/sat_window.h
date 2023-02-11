#ifndef sat_window_included
#define sat_window_included
//----------------------------------------------------------------------

// mouse events
// resizing
// buffer
// painting

/*
  to redraw a widget: call queueDirtyWidget(widget), and it will be
  picked up next time a on_window_paint() is called
*/

#include "base/sat.h"
#include "gui/sat_painter.h"
#include "gui/sat_widget.h"
#include "gui/sat_widget_listener.h"
#include "gui/sat_window_listener.h"

#define SAT_WINDOW_MAX_DIRTY_WIDGETS 1024

typedef SAT_SPSCQueue<SAT_Widget*,SAT_WINDOW_MAX_DIRTY_WIDGETS> SAT_DirtyWidgetsQueue;
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

  SAT_OpenGL*           MOpenGL         = nullptr;
  SAT_Painter*          MWindowPainter  = nullptr;
  void*                 MRenderBuffer   = nullptr;
  uint32_t              MBufferWidth    = 0;
  uint32_t              MBufferHeight   = 0;
  SAT_Widget*           MRootWidget     = nullptr;
  SAT_PaintContext      MPaintContext   = {};
  SAT_WindowListener*   MListener       = nullptr;
  SAT_DirtyWidgetsQueue MDirtyWidgets   = {};

  //SAT_SPSCQueue<SAT_Widget*,SAT_WINDOW_MAX_DIRTY_WIDGETS>  MDirtyWidgets = {};

//------------------------------
public:
//------------------------------

  SAT_Window(uint32_t AWidth, uint32_t AHeight, intptr_t AParent, SAT_WindowListener* AListener)
  : SAT_ImplementedWindow(AWidth,AHeight,AParent) {

    MListener = AListener;

    #ifdef SAT_LINUX
      Display* display = getX11Display();
      xcb_window_t window  = getX11Window();
      MOpenGL = new SAT_OpenGL(display,window);
    #endif

    #ifdef SAT_WIN32
    #endif

    MOpenGL->makeCurrent();
      MWindowPainter = new SAT_Painter(MOpenGL);
      createBuffer(AWidth,AHeight);
    MOpenGL->resetCurrent();

    //setFillBackground(false);
  }

  //----------

  virtual ~SAT_Window() {
    deleteBuffer();
    delete MWindowPainter;
    delete MOpenGL;
  }

//------------------------------
private: // buffer
//------------------------------

  void createBuffer(uint32_t AWidth, uint32_t AHeight) {
    uint32_t width = SAT_NextPowerOfTwo(AWidth);
    uint32_t height = SAT_NextPowerOfTwo(AHeight);
    MBufferWidth = width;
    MBufferHeight = height;
    MRenderBuffer = MWindowPainter->createRenderBuffer(width,height);
  }

  //----------

  void deleteBuffer() {
    if (MRenderBuffer) {
      MWindowPainter->deleteRenderBuffer(MRenderBuffer);
      MRenderBuffer = nullptr;
    }
  }

  //----------

  void resizeBuffer(uint32_t AWidth, uint32_t AHeight, bool ACopyBuffer=true) {
    if (MRenderBuffer) {
      createBuffer(AWidth,AHeight);
    }
    else {
      uint32_t new_width  = SAT_NextPowerOfTwo(AWidth);
      uint32_t new_height = SAT_NextPowerOfTwo(AHeight);
      if ((new_width != MBufferWidth) || (new_height != MBufferHeight)) {
        void* new_buffer = MWindowPainter->createRenderBuffer(new_width,new_height);
        if (ACopyBuffer) {
          copyBuffer(new_buffer,0,0,new_width,new_height,MRenderBuffer,0,0,MBufferWidth,MBufferHeight);
        }
        MWindowPainter->deleteRenderBuffer(MRenderBuffer);
        MRenderBuffer = new_buffer;
        MBufferWidth  = new_width;
        MBufferHeight = new_height;
      }
    }
  }

  //----------

  void copyBuffer(void* ADst, uint32_t ADstXpos, uint32_t ADstYpos, uint32_t ADstWidth, uint32_t ADstHeight, void* ASrc, uint32_t ASrcXpos, uint32_t ASrcYpos, uint32_t ASrcWidth, uint32_t ASrcHeight) {
    MWindowPainter->selectRenderBuffer(ADst,ADstWidth,ADstHeight);
    MWindowPainter->beginFrame(ADstWidth,ADstHeight);
    int32_t image = MWindowPainter->getImageFromRenderBuffer(ASrc);
    MWindowPainter->setFillImage(image, ASrcXpos,ASrcYpos, 1,1, 1.0, 0.0);
    MWindowPainter->fillRect(ASrcXpos,ASrcYpos,ASrcWidth,ASrcHeight);
    MWindowPainter->endFrame();
  }

  //----------

  /*
    called at the start of SAT_Window.on_window_paint(),
    to see if we need to paint any dirty widgets..
  */

  void checkBufferSize() {
    if ((MWindowWidth != MBufferWidth) || (MWindowHeight != MBufferHeight)) {
      resizeBuffer(MWindowWidth,MWindowHeight);
      if (MRootWidget) queueDirtyWidget(MRootWidget);
      //invalidate(R.x,R.y,R.w,R.h);
      //invalidate(0,0,MWindowWidth,MWindowHeight);
    }
  }

//------------------------------
public: // widgets
//------------------------------

  void setRootWidget(SAT_Widget* AWidget, SAT_WidgetListener* AListener=nullptr) {
    MRootWidget = AWidget;
    if (AListener) AWidget->setListener(AListener);
    else AWidget->setListener(this);
  }

  //----------

  // called from
  // - SAT_Window.on_window_open()
  // - SA_Editor.show()

  virtual void prepareWidgets() {
    if (MRootWidget) {
      MRootWidget->prepare(this,true);
      // set root widget as dirty, to force filling the back buffer
      SAT_Rect R = MRootWidget->getRect();
      queueDirtyWidget(MRootWidget);
      invalidateWidget(MRootWidget);
    }
  }

  //----------

  /*
    add widget to dirty widgets queue

    called from
    - SAT_Window.prepareWidgets()
    - SAT_Window.checkBufferSize()
  */


  virtual void queueDirtyWidget(SAT_Widget* AWidget) {
    if (MDirtyWidgets.write(AWidget)) {
    }
  }

  //----------

  /*
    redraw all widgets marked/queued as dirty

    called from:
    - SAT_Window.on_window_paint() -> paintWidgets()
  */

  virtual void flushDirtyWidgets() {
    SAT_Widget* widget = nullptr;
    while (MDirtyWidgets.read(&widget)) {
      //SAT_PRINT;
      widget->on_widget_paint(&MPaintContext);
    }
  }

  //----------

  /*
    flush dirty widgets (draw to buffer)
    (before drawing buffer (update_rect) to screen)

    called from
    - SAT_Window.on_window_paint()
  */

  virtual void paintWidgets(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) {
    //if (!MRootWidget) return;
    //MRootWidget->on_widget_paint(&MPaintContext);
    flushDirtyWidgets();
  }

  //----------

  /*
    called from:
    - SAT_Window.prepareWidgets()
  */

  virtual void invalidateWidget(SAT_Widget* AWidget) {
    SAT_Rect R = AWidget->getRect();
    invalidate(R.x,R.y,R.w,R.h);
  }


//------------------------------
public: // window
//------------------------------

  /*
    called from:
    - SAT_X11Window.processEvent(), XCB_MAP_NOTIFY
    - SAT_Editor.show()
  */

  void on_window_open() override {
    prepareWidgets();
  }

  //----------

  /*
    called from:
    - SAT_X11Window.processEvent(), XCB_UNMAP_NOTIFY
    - ~SAT_Editor()
  */

  void on_window_close() override {
  }

  //----------

  void on_window_paint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) override {

    MPaintContext.painter       = MWindowPainter;
    MPaintContext.update_rect.x = AXpos;
    MPaintContext.update_rect.y = AYpos;
    MPaintContext.update_rect.w = AWidth;
    MPaintContext.update_rect.h = AHeight;
    MPaintContext.window_width  = MWindowWidth;
    MPaintContext.window_height = MWindowHeight;

    MOpenGL->makeCurrent();

    checkBufferSize();

    MWindowPainter->selectRenderBuffer(MRenderBuffer,MBufferWidth,MBufferHeight);
    MWindowPainter->beginFrame(MBufferWidth,MBufferHeight);

    paintWidgets(AXpos,AYpos,AWidth,AHeight);

    MWindowPainter->endFrame();
    //copyBuffer(nullptr,MWindowWidth,MWindowHeight,MRenderBuffer,0,0,MWindowWidth,MWindowHeight);
    copyBuffer(nullptr,0,0,MWindowWidth,MWindowHeight,MRenderBuffer,AXpos,AYpos,AWidth,AHeight);

    MOpenGL->swapBuffers();
    MOpenGL->resetCurrent();

    MPaintContext.counter += 1;

  }

  //----------

  void on_window_resize(int32_t AWidth, int32_t AHeight) override {
    //SAT_Print("\n");
    if (MRootWidget) MRootWidget->setSize(AWidth,AHeight);
  }

//------------------------------
public: // widget listener
//------------------------------

  void do_widget_update(SAT_Widget* ASender, uint32_t AMode, uint32_t AIndex=0) override {
    if (MListener) MListener->do_window_listener_update_widget(ASender,AMode,AIndex);
  }

  void do_widget_redraw(SAT_Widget* ASender, uint32_t AMode, uint32_t AIndex=0) override {
    if (MListener) MListener->do_window_listener_redraw_widget(ASender,AMode,AIndex);
  }

  void do_widget_set_state(SAT_Widget* ASender, uint32_t AState) override {
    if (MListener) MListener->do_window_listener_set_widget_state(ASender,AState);
  }

  void do_widget_set_cursor(SAT_Widget* ASender, uint32_t ACursor) override {
    if (MListener) MListener->do_window_listener_set_cursor(ASender,ACursor);
  }

//------------------------------
public:
//------------------------------

  void do_timer_callback(SAT_Timer* ATimer) final {
    //SAT_PRINT;
    if (MListener) MListener->do_window_listener_timer(this);
    //on_window_timer();
  }


};

//----------------------------------------------------------------------
#endif
