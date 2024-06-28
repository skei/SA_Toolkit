#ifndef sat_win32_window_included
#define sat_win32_window_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/lib/sat_win32.h"
#include "gui/window/sat_base_window.h"
#include "gui/painter/sat_paint_source.h"
#include "gui/painter/sat_paint_target.h"
#include "gui/painter/sat_painter_owner.h"
#include "gui/surface/sat_surface_owner.h"
#include "gui/renderer/sat_render_source.h"
#include "gui/renderer/sat_render_target.h"
#include "gui/renderer/sat_renderer_owner.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Win32Window
: public SAT_BaseWindow
, public SAT_PainterOwner
, public SAT_RendererOwner
, public SAT_SurfaceOwner
, public SAT_PaintTarget
, public SAT_RenderTarget {

//------------------------------
private:
//------------------------------

  HWND        MWinHandle      = nullptr;

  uint32_t    MScreenWidth    = 0;
  uint32_t    MScreenHeight   = 0;
  uint32_t    MScreenDepth    = 0;
  int32_t     MWindowXpos     = 0;
  int32_t     MWindowYpos     = 0;
  uint32_t    MWindowWidth    = 0;
  uint32_t    MWindowHeight   = 0;

//------------------------------
public:
//------------------------------

  SAT_Win32Window(uint32_t AWidth, uint32_t AHeight, intptr_t AParent=0)
  : SAT_BaseWindow(AWidth,AHeight,AParent) {
    SAT_TRACE;
  }

  //----------

  virtual ~SAT_Win32Window() {
    SAT_TRACE;
  }

//------------------------------
public:
//------------------------------

  uint32_t  getWidth()        { return MWindowWidth; }
  uint32_t  getHeight()       { return MWindowHeight; }
  uint32_t  getDepth()        { return MScreenDepth; }

  HWND      getWin32Window()  { return MWinHandle; }

//------------------------------
public:
//------------------------------

  /*

  #ifdef SAT_PAINTER_NANOVG
  xcb_connection_t* on_painterOwner_getXcbConnection()  override { return MConnection; }
  xcb_visualid_t    on_painterOwner_getXcbVisual()      override { return MScreenVisual; }
  #endif

  #ifdef SAT_PAINTER_X11
  xcb_connection_t* on_painterOwner_getXcbConnection()  override { return MConnection; }
  xcb_visualid_t    on_painterOwner_getXcbVisual()      override { return MScreenVisual; }
  #endif

  //----------

  #ifdef SAT_PAINTER_NANOVG
  xcb_drawable_t    on_paintTarget_getXcbDrawable()     override { return MWindow; }
  #endif

  #ifdef SAT_PAINTER_X11
  xcb_drawable_t    on_paintTarget_getXcbDrawable()     override { return MWindow; }
  #endif

  //----------

  #ifdef SAT_RENDERER_GLX
  Display*          on_rendererOwner_getX11Display()    override { return MDisplay; }
  xcb_drawable_t    on_rendererOwner_getXcbDrawable()   override { return MWindow; }
  #endif

  #ifdef SAT_RENDERER_X11
  Display*          on_rendererOwner_getX11Display()    override { return MDisplay; }
  xcb_drawable_t    on_rendererOwner_getXcbDrawable()   override { return MWindow; }
  #endif

  //----------

  #ifdef SAT_RENDERER_GLX
  xcb_drawable_t    on_renderTarget_getXcbDrawable()    override { return MWindow; }
  #endif

  #ifdef SAT_RENDERER_X11
  xcb_drawable_t    on_renderTarget_getXcbDrawable()    override { return MWindow; }
  #endif

  */

  //----------

  uint32_t          on_surfaceOwner_getWidth()          override { return MWindowWidth; }
  uint32_t          on_surfaceOwner_getHeight()         override { return MWindowHeight; }
  uint32_t          on_surfaceOwner_getDepth()          override { return MScreenDepth; }
  
  #ifdef SAT_SURFACE_NANOVG
  // xcb_connection_t* on_surfaceOwner_getXcbConnection()  override { return MConnection; }
  // xcb_drawable_t    on_surfaceOwner_getXcbDrawable()    override { return MWindow; }
  // xcb_visualid_t    on_surfaceOwner_getXcbVisual()      override { return MScreenVisual; }
  #endif

  #ifdef SAT_SURFACE_WIN32
  // xcb_connection_t* on_surfaceOwner_getXcbConnection()  override { return MConnection; }
  // xcb_drawable_t    on_surfaceOwner_getXcbDrawable()    override { return MWindow; }
  // xcb_visualid_t    on_surfaceOwner_getXcbVisual()      override { return MScreenVisual; }
  #endif

//------------------------------
public: // SAT_BaseWindow
//------------------------------

  void show() override {
  }

  //----------

  void hide() override {
  }

  //----------

  void setPos(int32_t AXpos, int32_t AYpos) override {
  }

  //----------

  void setSize(uint32_t AWidth, uint32_t AHeight) override {
  }

  //----------

  void setTitle(const char* ATitle) override {
  }

  //----------

  void setMousePos(int32_t AXpos, int32_t AYpos) override{
  }

  //----------

  void setMouseCursor(int32_t ACursor) override {
  }

  //----------

  void hideMouse() override {
  }

  //----------

  void showMouse() override {
  }

  //----------

  void grabMouse() override {
  }

  //----------

  void releaseMouse() override {
  }

  //----------

  void reparent(intptr_t AParent) override {
  }

  //----------

  void invalidate(int32_t AXpos, int32_t AYpos, uint32_t AWidth, uint32_t AHeight) override {
  }

  //----------

  void sendClientMessage(uint32_t AData, uint32_t AType) override {
  }

  //----------

  uint32_t eventLoop() override {
    return 0;
  }

  //----------

  void startEventThread() override {
  }

  //----------

  void stopEventThread() override {
  }

//------------------------------
public:
//------------------------------

  // void fill(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight, uint32_t AColor) { //override {
  // }

  // void fill(uint32_t AColor) { //override {
  // }
    
  //----------

};

//----------------------------------------------------------------------
#endif
































#if 0

#ifndef mip_win32_window_included
#define mip_win32_window_included
//----------------------------------------------------------------------

/*
  - the HINSTANCE is saved when DllMain is called..
    after global variable initialization, but before main()
  - window class is registered the first time a window is created
    (see bottom of this file)

  ----------

  1. Set a flag when you see the WM_ENTERSIZEMOVE.
  2. Change your WM_SIZE handler to check the flag and do nothing if set.
  3. Change your WM_PAINT handler to check the flag and do a simple, fast
     fill of the window in a solid color if it's set.
  4. Clear the flag when you see WM_EXITSIZEMOVE, and then trigger your
     layout code and invalidate your window so that everything gets
     updated based on the final size.
  If your slow window is a child rather than your application's top-level
  window, you'll have to signal the child window when the top-level
  window gets the WM_ENTERSIZEMOVE and WM_EXITSIZEMOVE in order to
  implement steps 1 and 4.

*/

//----------------------------------------------------------------------

#include "mip.h"
#include "gui/base/mip_base_window.h"
#include "gui/win32/mip_win32.h"

//----------------------------------------------------------------------

/*
  CS_OWNDC    Allocates a unique device context for each window in the class
  CS_HREDRAW  Redraws the entire window if a movement or size adjustment
              changes the width of the client area.
  CS_VREDRAW  Redraws the entire window if a movement or size adjustment
              changes the height of the client area.
*/

//#define MIP_WIN32_CLASS_STYLE         ( CS_OWNDC )
#define MIP_WIN32_CLASS_STYLE         ( CS_OWNDC | CS_HREDRAW | CS_VREDRAW )

#define MIP_WIN32_STANDALONE_STYLE    ( WS_OVERLAPPEDWINDOW )
#define MIP_WIN32_EMBEDDED_STYLE      ( WS_CHILD )

#define MIP_WIN32_EX_STANDALONE_STYLE ( 0 /*WS_EX_OVERLAPPEDWINDOW*/ )
#define MIP_WIN32_EX_EMBEDDED_STYLE   ( 0 /* WS_EX_TOOLWINDOW */ )


//----------

char* MIP_Win32ClassName(void);
LRESULT CALLBACK mip_eventproc_win32(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//----------


const char* mip_win32_cursors[] = {
  IDC_ARROW,        // MIP_CURSOR_DEFAULT
  IDC_ARROW,        // MIP_CURSOR_ARROW
  IDC_UPARROW,      // MIP_CURSOR_ARROW_UP
  IDC_ARROW,        // MIP_CURSOR_ARROW_DOWN
  IDC_ARROW,        // MIP_CURSOR_ARROW_LEFT
  IDC_ARROW,        // MIP_CURSOR_ARROW_RIGHT
  IDC_SIZENS,       // MIP_CURSOR_ARROW_UP_DOWN
  IDC_SIZEWE,       // MIP_CURSOR_ARROW_LEFT_RIGHT
  IDC_SIZENWSE,     // MIP_CURSOR_ARROW_TOP_LEFT
  IDC_SIZENESW,     // MIP_CURSOR_ARROW_TOP_RIGHT
  IDC_SIZENESW,     // MIP_CURSOR_ARROW_BOTTOM_LEFT
  IDC_SIZENWSE,     // MIP_CURSOR_ARROW_BOTTOM_RIGHT
  IDC_ARROW,        // MIP_CURSOR_ARROW_LEFT_SIDE
  IDC_ARROW,        // MIP_CURSOR_ARROW_RIGHT_SIDE
  IDC_ARROW,        // MIP_CURSOR_ARROW_TOP_SIDE
  IDC_ARROW,        // MIP_CURSOR_ARROW_BOTTOM_SIDE
  IDC_SIZEALL,      // MIP_CURSOR_MOVE
  IDC_WAIT,         // MIP_CURSOR_WAIT
  IDC_APPSTARTING,  // MIP_CURSOR_ARROW_WAIT
  IDC_HAND,         // MIP_CURSOR_HAND
  IDC_HAND,         // MIP_CURSOR_FINGER
  IDC_CROSS,        // MIP_CURSOR_CROSS
  IDC_ARROW,        // MIP_CURSOR_CROSS2
  IDC_ARROW,        // MIP_CURSOR_PENCIL
  IDC_ARROW,        // MIP_CURSOR_PLUS
  IDC_HELP,         // MIP_CURSOR_QUESTION
  IDC_IBEAM,        // MIP_CURSOR_IBEAM
  IDC_ARROW,        // MIP_CURSOR_ARROW_INVALID
  IDC_ARROW,        // MIP_CURSOR_INVALID
  IDC_ARROW         // MIP_CURSOR_X
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Win32Window
: public MIP_BaseWindow
, public MIP_Drawable {

  friend LRESULT CALLBACK mip_win32_eventproc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//------------------------------
private:
//------------------------------

//HDC         MScreenDC                 = nullptr;
//HDC         MWindowDC                 = nullptr;

  HCURSOR     MDefaultCursor            = nullptr;
  HWND        MWinHandle                = nullptr;
  HCURSOR     MWinCursor                = nullptr;
  PAINTSTRUCT MWinPaintStruct           = {};
  HDC         MWinPaintDC               = nullptr;
  HCURSOR     MUserCursors[128]         = {0};

  const char* MWindowTitle              = "";
  void*       MWindowParent             = nullptr;
  bool        MEmbedded                 = false;
  int32_t     MMouseXpos                = 0;
  int32_t     MMouseYpos                = 0;
  int32_t     MCurrentCursor            = 0;

//------------------------------
private:
//------------------------------

  int32_t     MWindowWidth              = 0;
  int32_t     MWindowHeight             = 0;
  uint32_t    MScreenDepth              = 0;
  double      MWindowWidthScale         = 1.0;
  double      MWindowHeightScale        = 1.0;

  bool        MFillBackground           = false;
  uint32_t    MBackgroundColor          = 0x808080;

  int32_t MAdjustedWidth = 0;
  int32_t MAdjustedHeight = 0;

//------------------------------
public:
//------------------------------

  MIP_Win32Window(uint32_t AWidth, uint32_t AHeight, intptr_t AParent=0)
  : MIP_BaseWindow(AWidth,AHeight,AParent) {
    MWindowType = "";
    setWindowTitle("MIP_Win32Window");
    MMouseXpos  = -1;
    MMouseYpos  = -1;
    memset(MUserCursors,0,sizeof(MUserCursors));
    MCurrentCursor = -1;
    setMouseCursor(MIP_CURSOR_DEFAULT);
    if (AParent == 0) {
      createStandaloneWindow(AWidth,AHeight);
    }
    else {
      createEmbeddedWindow(AWidth,AHeight,AParent);
    }
    SetWindowLongPtr(MWinHandle,GWLP_USERDATA,(LONG_PTR)this);
  }

  //----------

  virtual ~MIP_Win32Window() {
    //destroyUserCursors();
    //ReleaseDC(0,MWindowDC);
    //ReleaseDC(0,MScreenDC);
    DestroyWindow(MWinHandle);
  }

//------------------------------
private:
//------------------------------

  void createStandaloneWindow(uint32_t AWidth, uint32_t AHeight) {
    MEmbedded       = false;
    MWindowType     = "STANDALONE";
    int32_t width   = AWidth;
    int32_t height  = AHeight;
    adjustStandaloneSize(&width,&height);
    MAdjustedWidth  = width  - AWidth;
    MAdjustedHeight = height - AHeight;
    MIP_Print("%i,%i Adjusted %i,%i\n",AWidth,AHeight,MAdjustedWidth,MAdjustedHeight);
    MWinHandle = CreateWindowEx(
      MIP_WIN32_EX_STANDALONE_STYLE,
      MIP_Win32ClassName(),
      "MIP_Win32Window (standalone)",
      MIP_WIN32_STANDALONE_STYLE,
      0,
      0,
      width,
      height,
      NULL,
      0,
      MIP_GLOBAL_WIN32_INSTANCE,
      0
    );
    SetFocus(MWinHandle);
  }

  //----------

  void createEmbeddedWindow(uint32_t AWidth, uint32_t AHeight, intptr_t AParent) {
    MEmbedded       = true;
    MWindowType     = "EMBEDDED";
    int32_t width   = AWidth;
    int32_t height  = AHeight;
    adjustEmbeddedSize(&width,&height);
    MAdjustedWidth  = width  - AWidth;
    MAdjustedHeight = height - AHeight;
    MIP_Print("%i,%i Adjusted %i,%i\n",AWidth,AHeight,MAdjustedWidth,MAdjustedHeight);
    MWinHandle = CreateWindowEx(
      MIP_WIN32_EX_EMBEDDED_STYLE,
      MIP_Win32ClassName(),
      nullptr,                    // title
      MIP_WIN32_EMBEDDED_STYLE,
      0,                          // x
      0,                          // y
      width,
      height,
      (HWND)AParent,
      0,                          // menu
      MIP_GLOBAL_WIN32_INSTANCE,
      0                           // params
    );
    SetFocus(MWinHandle);
  }

  //----------

  /*
    AdjustWindowRectEx
    Calculates the required size of the window rectangle, based on the desired
    size of the client rectangle. The window rectangle can then be passed to
    the CreateWindowEx function to create a window whose client area is the
    desired size.

    - rect: A pointer to a RECT structure that contains the coordinates of the
      top-left and bottom-right corners of the desired client area. When the
      function returns, the structure contains the coordinates of the top-left
      and bottom-right corners of the window to accommodate the desired client
      area.
    - style: The window style of the window whose required size is to be
      calculated. Note that you cannot specify the WS_OVERLAPPED style.
    - menu:
    - ex style
  */

  // No WS_OVERLAPPED? :-/

  void adjustStandaloneSize(int32_t* AWidth, int32_t* AHeight) {
    RECT rect = { 0, 0, (long int)*AWidth, (long int)*AHeight };
    AdjustWindowRectEx(&rect,MIP_WIN32_STANDALONE_STYLE,FALSE,MIP_WIN32_EX_STANDALONE_STYLE);
    //AdjustWindowRect(&rect,MIP_WIN32_STANDALONE_STYLE,FALSE);
    *AWidth  = rect.right  - rect.left;
    *AHeight = rect.bottom - rect.top;
  }

  //----------

  void adjustEmbeddedSize(int32_t* AWidth, int32_t* AHeight) {
    RECT rect = { 0, 0, (long int)*AWidth, (long int)*AHeight };
    AdjustWindowRectEx(&rect,MIP_WIN32_EMBEDDED_STYLE,FALSE,MIP_WIN32_EX_EMBEDDED_STYLE);
    //AdjustWindowRect(&rect,MIP_WIN32_EMBEDDED_STYLE,FALSE);
    *AWidth  = rect.right  - rect.left;
    *AHeight = rect.bottom - rect.top;
  }

//------------------------------
public:
//------------------------------

  HWND  getWinHandle(void)    { return MWinHandle; }
  HDC   getWinPaintDC(void)   { return MWinPaintDC; }

//HDC   getScreenDC(void)     { return MScreenDC; }
//HDC   getWindowDC(void)     { return MWindowDC; }

//------------------------------
public:
//------------------------------

  virtual int32_t getWindowWidth()                          { return MWindowWidth; }
  virtual int32_t getWindowHeight()                         { return MWindowHeight; }
  virtual double  getWindowWidthScale()                     { return MWindowWidthScale; }
  virtual double  getWindowHeightScale()                    { return MWindowHeightScale; }
  virtual void    setWindowFillBackground(bool AFill=true)  { MFillBackground = AFill; }
  virtual void    setWindowBackgroundColor(uint32_t AColor) { MBackgroundColor = AColor; }

//------------------------------
public: // drawable
//------------------------------

  bool                drawable_isWindow()          final { return true; }
  bool                drawable_isDrawable()        final { return true; }
  uint32_t            drawable_getWidth()          final { return MWindowWidth; }
  uint32_t            drawable_getHeight()         final { return MWindowHeight; }
  uint32_t            drawable_getDepth()          final { return MScreenDepth; }
  HWND                drawable_getWin32Hwnd()      final { return MWinHandle; }
  HDC                 drawable_getWin32PaintDC()   final { return MWinPaintDC; }

//------------------------------
public:
//------------------------------

  void setWindowPos(uint32_t AXpos, uint32_t AYpos) override {
    SetWindowPos(MWinHandle,0,AXpos,AYpos,0,0,SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOZORDER);
  }

  //----------

  void setWindowSize(uint32_t AWidth, uint32_t AHeight) override {
    SetWindowPos(MWinHandle,HWND_TOP,0,0,AWidth,AHeight, SWP_NOMOVE);
    //MRect.w = w;
    //MRect.h = h;
    MWindowWidth = AWidth;
    MWindowHeight = AHeight;
  }

  //----------

  void setWindowTitle(const char* ATitle) override {
    SetWindowText(MWinHandle, ATitle);
  }

  //----------

  void openWindow() override {
    ShowWindow(MWinHandle,SW_SHOW);
    #ifdef MIP_WIN32_IDLE_TIMER
      SetTimer(MWinHandle,s3_ts_idle,S3_WIN32_IDLE_MS,S3_NULL);
    #endif
  }

  //----------

  void closeWindow() override {
    #ifdef MIP_WIN32_IDLE_TIMER
      KillTimer(MWinHandle,s3_ts_idle);
    #endif
    ShowWindow(MWinHandle,SW_HIDE);
  }

  //----------

  void flush(void) override {
    GdiFlush();
  }

  //----------

  //void sync(void) override {
  //}

  //----------

  //MSG msg;
  //BOOL bRet;
  //while((bRet = GetMessage(&msg,NULL,0,0)) != 0) {
  //  if (bRet == -1) {
  //    // handle the error and possibly exit
  //  }
  //  else {
  //    TranslateMessage(&msg);
  //    DispatchMessage(&msg);
  //  }
  //}

  void eventLoop() override {
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
    //while (GetMessage(&msg, MWinHandle,0,0)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }


  //----------

  //void startEventThread() override {
  //}

  //----------

  //void stopEventThread() override {
  //}

  //----------

  //void sendClientMessage(uint32_t AData, uint32_t AType) override {
  //}

  //----------

  /*
    The invalidated areas accumulate in the update region until the region is
    processed when the next WM_PAINT message occurs or until the region is
    validated by using the ValidateRect or ValidateRgn function.
    The system sends a WM_PAINT message to a window whenever its update region
    is not empty and there are no other messages in the application queue for
    that window.
    If the bErase parameter is TRUE for any part of the update region, the
    background is erased in the entire region, not just in the specified part.
  */

  /*
    The UpdateWindow function updates the client area of the specified window
    by sending a WM_PAINT message to the window if the window's update region
    is not empty. The function sends a WM_PAINT message directly to the window
    procedure of the specified window, bypassing the application queue. If the
    update region is empty, no message is sent.
  */

  /*
    The RedrawWindow function updates the specified rectangle or region in a
    window's client area.
  */

  void invalidateRegion(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) override {
    RECT R;
    R.left   = AXpos;
    R.top    = AYpos;
    R.right  = AXpos + AWidth;  // - 1;
    R.bottom = AYpos + AHeight; // - 1;
    InvalidateRect(MWinHandle,&R,false);
    //UpdateWindow(MWinHandle);
    //RedrawWindow(MWinHandle,&R,NULL,RDW_UPDATENOW);
  }

  //----------

  //void redrawRegion(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) override {
  //}

  //----------

  /*
    AParent: A handle to the new parent window. If this parameter is NULL, the
    desktop window becomes the new parent window. If this parameter is
    HWND_MESSAGE, the child window becomes a message-only window.

    An application can use the SetParent function to set the parent window of a
    pop-up, overlapped, or child window.

    If the window identified by the hWndChild parameter is visible, the system
    performs the appropriate redrawing and repainting.

    For compatibility reasons, SetParent does not modify the WS_CHILD or
    WS_POPUP window styles of the window whose parent is being changed.
    Therefore:
    - if hWndNewParent is NULL, you should also clear the WS_CHILD bit
    and set the WS_POPUP style after (!!) calling SetParent.
    - Conversely, if hWndNewParent is not NULL and the window was previously a
    child of the desktop, you should clear the WS_POPUP style and set the
    WS_CHILD style before  (!!) calling SetParent.

    When you change the parent of a window, you should synchronize the UISTATE
    of both windows. For more information, see WM_CHANGEUISTATE and
    WM_UPDATEUISTATE.
  */

  void reparentWindow(intptr_t AParent) override {

    LONG_PTR style   = GetWindowLongPtr(MWinHandle,GWL_STYLE);
    LONG_PTR exstyle = GetWindowLongPtr(MWinHandle,GWL_EXSTYLE);

    if (AParent == 0) {
      //MIP_Print("reparenting %s -> STANDALONE\n",MWindowType);
      MWindowType = "STANDALONE";
      MEmbedded = false;

      int32_t w = MWindowWidth;
      int32_t h = MWindowHeight;
      adjustStandaloneSize(&w,&h);
      MIP_Print("%i,%i Adjusted %i,%i\n",MWindowWidth,MWindowHeight,w,h);
      setWindowSize(w,h);
      //MIP_Print("%i,%i -> %i,%i\n",MWindowWidth,MWindowHeight,w,h);

      SetParent(MWinHandle,(HWND)AParent);

      //style &= ~WS_CHILD;
      //style |= WS_POPUP;
      style   &= ~MIP_WIN32_EMBEDDED_STYLE;
      style   |=  MIP_WIN32_STANDALONE_STYLE;
      exstyle &= ~MIP_WIN32_EX_EMBEDDED_STYLE;
      exstyle |=  MIP_WIN32_EX_STANDALONE_STYLE;
      SetWindowLongPtr( MWinHandle, GWL_STYLE,   style   );
      SetWindowLongPtr( MWinHandle, GWL_EXSTYLE, exstyle );


    }

    //else {
    if (AParent != 0) {
      //MIP_Print("reparenting %s -> EMBEDDED\n",MWindowType);
      MWindowType = "EMBEDDED";
      MEmbedded = true;

      int32_t w = MWindowWidth;
      int32_t h = MWindowHeight;
      adjustEmbeddedSize(&w,&h);
      MIP_Print("%i,%i Adjusted %i,%i\n",MWindowWidth,MWindowHeight,w,h);
      setWindowSize(w,h);
      setWindowPos(0,0);
      //MIP_Print("%i,%i -> %i,%i\n",MWindowWidth,MWindowHeight,w,h);

      //style |= WS_CHILD;
      //style &= ~WS_POPUP;
      style   &= ~MIP_WIN32_STANDALONE_STYLE;
      style   |=  MIP_WIN32_EMBEDDED_STYLE;
      exstyle &= ~MIP_WIN32_EX_STANDALONE_STYLE;
      exstyle |=  MIP_WIN32_EX_EMBEDDED_STYLE;
      SetWindowLongPtr( MWinHandle, GWL_STYLE,   style   );
      SetWindowLongPtr( MWinHandle, GWL_EXSTYLE, exstyle );

      SetParent(MWinHandle,(HWND)AParent);


    }

//    SetWindowLongPtr( MWinHandle, GWL_STYLE,   style   );
//    SetWindowLongPtr( MWinHandle, GWL_EXSTYLE, exstyle );
//    SetParent(MWinHandle,(HWND)AParent);

  }

  //----------

  /*
    The BeginPaint function prepares the specified window for painting and
    fills a PAINTSTRUCT structure with information about the painting.

    The BeginPaint function automatically sets the clipping region of the
    device context to exclude any area outside the update region. The update
    region is set by the InvalidateRect or InvalidateRgn function and by the
    system after sizing, moving, creating, scrolling, or any other operation
    that affects the client area.
  */

  void beginPaint()  override {
    MWinPaintDC = BeginPaint(MWinHandle,&MWinPaintStruct);
  }

  //----------

  /*
    The EndPaint function marks the end of painting in the specified window.
    This function is required for each call to the BeginPaint function, but
    only after painting is complete.

    EndPaint releases the display device context that BeginPaint retrieved.
  */

  void endPaint()  override {
    //flush();
    EndPaint(MWinHandle,&MWinPaintStruct);
    //UpdateWindow(MWinHandle);
  }

//------------------------------
public: // mouse
//------------------------------

  void setMouseCursor(int32_t ACursor) override {
    if (ACursor != MCurrentCursor) {
      MCurrentCursor = ACursor;
      if (ACursor>=128) MWinCursor = MUserCursors[ACursor-128];
      else MWinCursor = LoadCursor(NULL,mip_win32_cursors[ACursor]);
      SetCursor(MWinCursor);
    }
  }

  //----------

  // Moves the cursor to the specified screen coordinates
  // will fire a WM_MOUSEMOVE event..

  void setMouseCursorPos(int32_t AXpos, int32_t AYpos) override {
    POINT pos;
    pos.x = AXpos;
    pos.y = AYpos;
    ClientToScreen(MWinHandle,&pos);
    int32_t x = pos.x;
    int32_t y = pos.y;
    SetCursorPos(x,y);
  }

  //----------

  void hideMouseCursor(void) override {
    ShowCursor(false);
  }

  //----------

  void showMouseCursor(void) override {
    ShowCursor(true);
  }

  //----------

  void grabMouseCursor(void) override {
    SetCapture(MWinHandle);
  }

  //----------

  void releaseMouseCursor(void) override {
    ReleaseCapture();
  }

//------------------------------
public: // paint
//------------------------------

  void fillColor(uint32_t AColor) override {
  }

  //----------

  void fillColor(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight, uint32_t AColor) override {
    //MWinPaintDC
    //MWinPaintStruct

    RECT R;
    R.left    = AXpos;
    R.top     = AYpos;
    R.right   = AXpos + AWidth;
    R.bottom  = AYpos + AHeight;

    //HBRUSH brush = (HBRUSH)GetStockObject(DC_BRUSH);
    //FillRect(MHandle,&R,brush);
    //FillRect(MHandle,&R,MBrushHandle);
    //SelectObject(MHandle,MNullPen);
    //SelectObject(MHandle,MNullBrush);

//    uint8_t r = (255.0 * AColor.r);
//    uint8_t g = (255.0 * AColor.g);
//    uint8_t b = (255.0 * AColor.b);
//    //uint8_t a = (255.0 * AColor.a);
//    uint32_t color = RGB(r,g,b);

//    HBRUSH brush = CreateSolidBrush(MIP_GdiColor(color));

    HPEN pen = CreatePen(PS_NULL,0,0);
    HGDIOBJ old_pen = SelectObject(MWinPaintDC,pen);
    //DeleteObject(old_pen);

    HBRUSH brush = CreateSolidBrush(AColor);
    HGDIOBJ old_brush = SelectObject(MWinPaintDC,brush);
    //DeleteObject(old_brush);

    Rectangle(MWinPaintDC,R.left,R.top,R.right,R.bottom);

    SelectObject(MWinPaintDC,old_pen);
    DeleteObject(pen);

    SelectObject(MWinPaintDC,old_brush);
    DeleteObject(brush);

  }

  //----------

  void blitBuffer(int32_t ADstX, int32_t ADstY, void* AData, uint32_t AStride, int32_t ASrcW, int32_t ASrcH) override {
  }

  //----------

  // only valid dugring begin/endPaint...
  // needs MWinPaintDC

  //  void blit(int32 dst_x, int32 dst_y, S3_Painter* APainter, int32 src_x, int32 src_y, int32 src_w, int32 src_h) {
  //    BitBlt(MWinPaintDC,dst_x,dst_y,src_w,src_h,APainter->hdc(),src_x,src_y,SRCCOPY);
  //  }

  /*
    a bitmap can't be attached to more than one dc at a time..
    and it's currently (probably) attached to the painter
    so, we would need to detach it from there first if we want to
    use this method..
  */

  //void blit(int32 ADstX, int32 ADstY, S3_Drawable* ASource, int32 ASrcX, int32 ASrcY, int32 ASrcW, int32 ASrcH) {
  //  HDC tempdc = CreateCompatibleDC(MWinPaintDC);
  //  HGDIOBJ oldbitmap = SelectObject(tempdc,ASource->hbitmap());
  //  uint32 rop = s3_ro_src;//MSrcRasterOp;//SRCCOPY;
  //  BitBlt(MWinPaintDC,ADstX,ADstY,ASrcW,ASrcH,tempdc,ASrcX,ASrcY,rop);
  //  SelectObject(tempdc,oldbitmap);
  //  DeleteDC(tempdc);
  //}

  void blitDrawable(int32_t ADstX, int32_t ADstY, MIP_Drawable* ADrawable, int32_t ASrcX, int32_t ASrcY, int32_t ASrcW, int32_t ASrcH) override {
  }

  //----------

//
//    //----------------------------------------
//    // bitmap
//    //----------------------------------------
//
//    //BOOL BitBlt(
//    //  HDC hdcDest,  // Handle to the destination device context.
//    //  int nXDest,   // logical x-coordinate of the upper-left corner of the destination rectangle.
//    //  int nYDest,   // logical y-coordinate of the upper-left corner of the destination rectangle.
//    //  int nWidth,   // logical width of the source and destination rectangles.
//    //  int nHeight,  // logical height of the source and the destination rectangles.
//    //  HDC hdcSrc,   // Handle to the source device context.
//    //  int nXSrc,    // logical x-coordinate of the upper-left corner of the source rectangle.
//    //  int nYSrc,    // logical y-coordinate of the upper-left corner of the source rectangle.
//    //  DWORD dwRop   // raster-operation code.
//    //);
//
//    virtual void drawBitmap(h_Bitmap* aBitmap, int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
//      {
//        HDC tempdc = CreateCompatibleDC(m_DC);
//        SelectObject(tempdc,/*(HBITMAP)*/aBitmap->getBitmap());
//        BitBlt(m_DC,aX,aY,aSrcW,aSrcH,tempdc,aSrcX,aSrcY,SRCCOPY);
//        DeleteDC(tempdc);
//      }
//
//    //--------------------------------------------------
//    // surface
//    //--------------------------------------------------
//
//    //virtual void drawImage(axImage* aImage, int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
//    virtual void drawSurface(h_PaintSource* aSource, int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
//      {
//        //HDC tempdc = (HDC)aImage->getHandle();
//        //HDC tempdc = (HDC)aSurface->getHandle();
//        HDC tempdc = aSource->getDC();
//        BitBlt(m_DC,aX,aY,aSrcW,aSrcH,tempdc,aSrcX,aSrcY,SRCCOPY);
//      }
//
//    //----------
//
//    //virtual void renderBitmap(axBitmap* aBitmap, int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
//    //  {
//    //    drawBitmap(aBitmap,aX,aY,aSrcX,aSrcY,aSrcW,aSrcH);
//    //  }
//
//    //typedef struct _BLENDFUNCTION {
//    //  BYTE BlendOp;
//    //  BYTE BlendFlags;
//    //  BYTE SourceConstantAlpha;
//    //  BYTE AlphaFormat;
//    //} BLENDFUNCTION, *PBLENDFUNCTION, *LPBLENDFUNCTION;
//
//    //virtual void renderImage( axImage*  aImage,  int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
//    virtual void blendSurface( h_PaintSource* aSource,  int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
//      {
//        //HDC tempdc = (HDC)aImage->getHandle();
//        HDC tempdc = /*(HDC)*/aSource->getDC();
//        AlphaBlend(m_DC,aX,aY,aSrcW,aSrcH,tempdc,aSrcX,aSrcY,aSrcW,aSrcH,m_BlendFunc);
//        // link with: libmsimg32
//      }
//
//    //BOOL StretchBlt(
//    //  __in  HDC hdcDest,
//    //  __in  int nXOriginDest,
//    //  __in  int nYOriginDest,
//    //  __in  int nWidthDest,
//    //  __in  int nHeightDest,
//    //  __in  HDC hdcSrc,
//    //  __in  int nXOriginSrc,
//    //  __in  int nYOriginSrc,
//    //  __in  int nWidthSrc,
//    //  __in  int nHeightSrc,
//    //  __in  DWORD dwRop
//    //);
//
//    //virtual void stretchImage( axImage*  aImage,  int aX, int aY, int aW, int aH, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
//    virtual void stretchSurface( h_PaintSource* aSource, int aX, int aY, int aW, int aH, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
//      {
//        //HDC tempdc = (HDC)aImage->getHandle();
//        HDC tempdc = (HDC)aSource->getDC();
//        AlphaBlend(m_DC,aX,aY,aW,aH,tempdc,aSrcX,aSrcY,aSrcW,aSrcH,m_BlendFunc);
//        // link with: libmsimg32
//      }
//

//------------------------------
private: // event handler
//------------------------------

  int32_t remapMouseButton(int32_t AButton) {
    int32_t result = MIP_BUTTON_NONE;
    switch(AButton) {
      case MK_LBUTTON:  result = MIP_BUTTON_LEFT;      break;
      case MK_MBUTTON:  result = MIP_BUTTON_MIDDLE;    break;
      case MK_RBUTTON:  result = MIP_BUTTON_RIGHT;     break;
      //case MK_XBUTTON1: S3_Trace("xbutton1\n"); /*result = smb_x1;*/        break;
      //case MK_XBUTTON2: S3_Trace("xbutton2\n"); /*result = smb_x2;*/        break;
      //case WM_LBUTTONDOWN: result = smb_left;      break;
      //case WM_MBUTTONDOWN: result = smb_middle;    break;
      //case WM_RBUTTONDOWN: result = smb_right;     break;
      //case 4: result = smb_wheelUp;   break;
      //case 5: result = smb_wheelDown; break;
      //case 8: result = smb_backward;  break;
      //case 9: result = smb_forward;   break;
    }
    return result;
  }

  //----------

  int32_t remapMouseKey(int32_t AState) {
    int32_t result = MIP_KEY_NONE;
    if ((AState & MK_SHIFT  ) != 0) result |= MIP_KEY_SHIFT;
    //if ((AState & ) != 0) result |= MIP_KEY_CAPS;
    if ((AState & MK_CONTROL  ) != 0) result |= MIP_KEY_CTRL;
    //if ((AState & ) != 0) result |= MIP_KEY_ALT;
    //if ((AState & 128) != 0) result |= MIP_KEY_ALTGR;
    return result;
  }

  //----------

  /*
  WM_KEYDOWN

  wParam  The virtual-key code of the nonsystem key. See Virtual-Key Codes.
  lParam  The repeat count, scan code, extended-key flag, context code,
  previous key-state flag, and transition-state flag, as shown following.

  Bits	Meaning
  0-15  The repeat count for the current message. The value is the number of
        times the keystroke is autorepeated as a result of the user holding
        down the key. If the keystroke is held long enough, multiple messages
        are sent. However, the repeat count is not cumulative.
  16-23 The scan code. The value depends on the OEM.
  24    Indicates whether the key is an extended key, such as the right-hand
        ALT and CTRL keys that appear on an enhanced 101- or 102-key keyboard.
        The value is 1 if it is an extended key; otherwise, it is 0.
  25-28 Reserved; do not use.
  29    The context code. The value is always 0 for a WM_KEYDOWN message.
  30    The previous key state. The value is 1 if the key is down before the
        message is sent, or it is zero if the key is up.
  31    The transition state. The value is always 0 for a WM_KEYDOWN message.
  */

  /*
  )int WINAPI ToAscii(
    _In_           UINT   uVirtKey,
    _In_           UINT   uScanCode,
    _In_opt_ const BYTE   *lpKeyState,
    _Out_          LPWORD lpChar,
    _In_           UINT   uFlags
  );
  */

  int32_t remapKeyCode(WPARAM wParam, LPARAM lParam) {
    switch(wParam) {
     case VK_BACK: return MIP_KEY_BACKSPACE;
    }
    return 0;
  }

  //------------------------------
  //
  //------------------------------

  // note:
  // see ExeWindow, one window embedded into another

  LRESULT eventHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

    // NULL for mouse events?
    //if (hWnd != MWinHandle) {
    //  //MIP_Print("wrong window\n");
    //  return DefWindowProc(hWnd,message,wParam,lParam);
    //}

    LRESULT result = 0;
    MIP_DRect rc;
    int32_t x,y,b,w,h,d;//,k;
    switch (message) {

      case WM_DESTROY: {
        if (!MEmbedded) PostQuitMessage(0);
        break;
      }

      case WM_GETDLGCODE: {
        // we want keypresses
        result = DLGC_WANTALLKEYS;
        break;
      }

      case WM_ERASEBKGND: {
        // tell window not to erase background
        result = 1;
        break;
      }

      case WM_SETCURSOR: {
        // called every mouse move
        if (LOWORD(lParam)==HTCLIENT) {
          SetCursor(MWinCursor);
          //setCursor(MCurrentCursor);
          result = 1;
        }
        else result = DefWindowProc(hWnd,message,wParam,lParam);
        break;
      }

      /*
        Sent to a window when the size or position of the window is about to
        change. An application can use this message to override the window's
        default maximized size and position, or its default minimum or maximum
        tracking size.

        typedef struct tagMINMAXINFO {
          POINT ptReserved;
          POINT ptMaxSize;
          POINT ptMaxPosition;
          POINT ptMinTrackSize;
          POINT ptMaxTrackSize;
        } MINMAXINFO, *PMINMAXINFO, *LPMINMAXINFO;
      */

      case WM_GETMINMAXINFO: {
        MIP_Print("WM_GETMINMAXINFO\n");
        //MINMAXINFO* info = (MINMAXINFO*)lParam;
        break;
      }

      /*
        Sent one time to a window after it enters the moving or sizing modal
        loop. The window enters the moving or sizing modal loop when the user
        clicks the window's title bar or sizing border, or when the window
        passes the WM_SYSCOMMAND message to the DefWindowProc function and the
        wParam parameter of the message specifies the SC_MOVE or SC_SIZE value.
        The operation is complete when DefWindowProc returns.
        The system sends the WM_ENTERSIZEMOVE message regardless of whether the
        dragging of full windows is enabled.
      */

      case WM_ENTERSIZEMOVE: {
        MIP_Print("WM_ENTERSIZEMOVE\n");
        break;
      }

      /*
        Sent one time to a window, after it has exited the moving or sizing
        modal loop. The window enters the moving or sizing modal loop when the
        user clicks the window's title bar or sizing border, or when the window
        passes the WM_SYSCOMMAND message to the DefWindowProc function and the
        wParam parameter of the message specifies the SC_MOVE or SC_SIZE value.
        The operation is complete when DefWindowProc returns.
      */

      case WM_EXITSIZEMOVE: {
        MIP_Print("WM_EXITSIZEMOVE\n");
        break;
      }

      /*
        Sent to a window that the user is resizing. By processing this message,
        an application can monitor the size and position of the drag rectangle
        and, if needed, change its size or position.
        wParam  The edge of the window that is being sized.
          case WMSZ_BOTTOM:       break; // Bottom edge
          case WMSZ_BOTTOMLEFT:   break; // Bottom-left corner
          case WMSZ_BOTTOMRIGHT:  break; // Bottom-right corner
          case WMSZ_LEFT:         break; // Left edge
          case WMSZ_RIGHT:        break; // Right edge
          case WMSZ_TOP:          break; // Top edge
          case WMSZ_TOPLEFT:      break; // Top-left corner
          case WMSZ_TOPRIGHT:     break; // Top-right corner
        lParam  A pointer to a RECT structure with the screen coordinates of
                the drag rectangle. To change the size or position of the drag
                rectangle, an application must change the members of this
                structure.
        An application should return TRUE if it processes this message.
      */

      case WM_SIZING: {
        RECT* R = (RECT*)lParam;
        int32_t x = R->left;
        int32_t y = R->top;
        int32_t w = R->right - R->left;
        int32_t h = R->bottom - R->top;
        MIP_Print("WM_SIZING x %i y %i w %i h %i\n",x,y,w,h);
        break;
      }

      /*
        Sent to a window after its size has changed.
        wParam: The type of resizing requested.
          case SIZE_MAXHIDE:    // Message is sent to all pop-up windows when some other window is maximized.
          case SIZE_MAXIMIZED:  // The window has been maximized.
          case SIZE_MAXSHOW:    // Message is sent to all pop-up windows when some other window has been restored to its former size.
          case SIZE_MINIMIZED:  // The window has been minimized.
          case SIZE_RESTORED:   // The window has been resized, but neither the SIZE_MINIMIZED nor SIZE_MAXIMIZED value applies.
        The low-order word of lParam specifies the new width of the client area.
        The high-order word of lParam specifies the new height of the client area.
      */

      case WM_SIZE: {
        w = short(LOWORD(lParam));
        h = short(HIWORD(lParam));
        MIP_Print("WM_SIZE %s w %i h %i (type %i)\n",MWindowType,w,h,wParam);
        //if ( (w != MRect.w) || (h != MRect.h) ) {
        //if ( (w != MWindowWidth) || (h != MWindowHeight) ) {
          //MIP_Print("-> on_window_resize\n");
          MWindowWidth  = w;
          MWindowHeight = h;
          on_window_resize(w,h);
          //invalidateRegion(0,0,w,h);
          //if (MFlags & s3_wf_autoalign) on_widgetAlign(this);
          //#ifndef S3_NO_WINDOW_BACKBUFFER
          //if (MListener) MListener->on_bufferPaint(this,S3_NULL,s3_pm_normal); //redraw;
          //#endif
        //}
        break;
      }

      case WM_PAINT: {
        beginPaint();
        int32_t x = MWinPaintStruct.rcPaint.left;
        int32_t y = MWinPaintStruct.rcPaint.top;
        int32_t w = MWinPaintStruct.rcPaint.right  - MWinPaintStruct.rcPaint.left;// + 1;
        int32_t h = MWinPaintStruct.rcPaint.bottom - MWinPaintStruct.rcPaint.top;// + 1;
        MIP_Print("WM_PAINT %s x %i y %i w %i h %i\n",MWindowType,x,y,w,h);

        if (MFillBackground) fillColor(x,y,w,h,MBackgroundColor);

        on_window_paint(x,y,w,h);
        endPaint();
        break;
      }

      case WM_MOUSEMOVE: {
        x = short(LOWORD(lParam));
        y = short(HIWORD(lParam));
        //if (MListener) MListener->on_mouseMove(this,x,y,remapKey(wParam));
        on_window_mouse_move(remapMouseKey(wParam),x,y,0);
        MMouseXpos = x;
        MMouseYpos = y;
        break;
      }

      case WM_XBUTTONDOWN:
      case WM_LBUTTONDOWN:
      case WM_RBUTTONDOWN:
      case WM_MBUTTONDOWN: {
        switch (message) {
          case WM_LBUTTONDOWN:  b = MIP_BUTTON_LEFT;   break;
          case WM_MBUTTONDOWN:  b = MIP_BUTTON_MIDDLE; break;
          case WM_RBUTTONDOWN:  b = MIP_BUTTON_RIGHT;  break;
          case WM_XBUTTONDOWN:
            if ((wParam & MK_XBUTTON1) != 0) b = MIP_BUTTON_BACKWARD;
            if ((wParam & MK_XBUTTON2) != 0) b = MIP_BUTTON_FORWARD;
            break;
          default:              b = MIP_BUTTON_NONE;   break;
        }
        x = short(LOWORD(lParam));
        y = short(HIWORD(lParam));
        //if (MListener) MListener->on_mouseDown(this,x,y,b,remapKey(wParam));
        on_window_mouse_click(b,remapMouseKey(wParam),x,y,0);
        //        if (MFlags & s3_wf_capture) grabCursor();
        break;
      }

      case WM_XBUTTONUP:
      case WM_LBUTTONUP:
      case WM_RBUTTONUP:
      case WM_MBUTTONUP: {
        switch (message) {
          case WM_LBUTTONUP:  b = MIP_BUTTON_LEFT;   break;
          case WM_MBUTTONUP:  b = MIP_BUTTON_MIDDLE; break;
          case WM_RBUTTONUP:  b = MIP_BUTTON_RIGHT;  break;
          //case WM_XBUTTONUP:  b = MIP_BUTTON_s3_mb_side;   break;
          default:            b = MIP_BUTTON_NONE;   break;
        }
        x = short(LOWORD(lParam));
        y = short(HIWORD(lParam));
        //if (MListener) MListener->on_mouseUp(this,x,y,b,remapKey(wParam));
        on_window_mouse_release(b,remapMouseKey(wParam),x,y,0);
        //        if (MFlags&s3_wf_capture) releaseCursor();
        break;
      }

      /*
        Only windows that have the CS_DBLCLKS style can receive
        WM_LBUTTONDBLCLK messages, which the system generates whenever the
        user presses, releases, and again presses the left mouse button
        within the system's double-click time limit. Double-clicking the left
        mouse button actually generates a sequence of four messages:
        WM_LBUTTONDOWN, WM_LBUTTONUP, WM_LBUTTONDBLCLK, and WM_LBUTTONUP.
      */

      #ifdef MIP_MOUSE_DOUBLECLICK
      /*
      case WM_LBUTTONDBLCLK:
      case WM_MBUTTONDBLCLK:
      case WM_RBUTTONDBLCLK:
      case WM_XBUTTONDBLCLK: {
        switch (message) {
          case WM_LBUTTONDBLCLK:  b = s3_mb_left;   break;
          case WM_MBUTTONDBLCLK:  b = s3_mb_middle; break;
          case WM_RBUTTONDBLCLK:  b = s3_mb_right;  break;
          case WM_XBUTTONDBLCLK:  b = s3_mb_side;   break;
          default:                b = s3_mb_none;   //break;
        }
        if (b==s3_mb_side) {
          if ((wParam & MK_XBUTTON1) != 0) b = s3_mb_backward;
          if ((wParam & MK_XBUTTON2) != 0) b = s3_mb_forward;
        }
        //S3_Trace("dblclick: %i\n",b);
        x = short(LOWORD(lParam));
        y = short(HIWORD(lParam));
        //x = GET_X_LPARAM(lParam);
        //y = GET_Y_LPARAM(lParam);
        //if (MListener) MListener->on_mouseUp(this,x,y,b,remapKey(wParam));
        on_widgetMouseDoubleClick(this,x,y,b,remapMouseKey(wParam));
        break;
      }
      */
      #endif // MIP_MOUSE_DOUBLECLICK

      case WM_MOUSEWHEEL: {
        d = GET_WHEEL_DELTA_WPARAM(wParam);
        //if (d>0) { if (MListener) MListener->on_mouseDown(this,MMouseXpos,MMouseYpos,smb_wheelUp,  smb_none); }
        //if (d<0) { if (MListener) MListener->on_mouseDown(this,MMouseXpos,MMouseYpos,smb_wheelDown,smb_none); }
        if (d > 0) { on_window_mouse_click(MIP_BUTTON_SCROLL_UP,   MIP_KEY_NONE, MMouseXpos,MMouseYpos,0); }
        if (d < 0) { on_window_mouse_click(MIP_BUTTON_SCROLL_DOWN, MIP_KEY_NONE, MMouseXpos,MMouseYpos,0); }
        break;
      }

      case WM_KEYDOWN: {
        //if (MListener) MListener->on_keyDown(this,wParam,lParam);
        //k = remapKeyCode(wParam,lParam);
        on_window_key_press(wParam,lParam,0);
        break;
      }

      case WM_KEYUP: {
        //if (MListener) MListener->on_keyUp(this,wParam,lParam);
        //k = remapKeyCode(wParam,lParam);
        on_window_key_release(wParam,lParam,0);
        break;
      }

      //case WM_CHAR:
      //  break;

      /*
        The WM_PAINT message is sent when the system or another application
        makes a request to paint a portion of an application's window.
        The message is sent when the UpdateWindow or RedrawWindow function is
        called, or by the DispatchMessage function when the application obtains
        a WM_PAINT message by using the GetMessage or PeekMessage function.

        //is it possible to receive a WM_PAINT when some other thread is
        //drawing to the backbuffer?
      */

      case WM_TIMER: {
        MIP_Print("WM_TIMER\n");
        //if (MListener) {
        //  if (wParam == MGuiTimer.getId()) MListener->on_timerCallback(this);
        //  //  if (wParam==s3_ts_idle) MListener->on_windowIdle(this);
        //}
        break;
      }

      default: {
        result = DefWindowProc(hWnd,message,wParam,lParam);
        break;
      }
    }
    return result;
  }

//------------------------------
private:
//------------------------------


  /*

  virtual // S3_Window_Base
  void startTimer(int32 ms) {
    SetTimer(MWinHandle,s3_ts_timer,ms,S3_NULL);
  }

  //----------

  // The KillTimer function does not remove WM_TIMER messages already posted
  // to the message queue.

  virtual // S3_Window_Base
  void stopTimer(void) {
    KillTimer(MWinHandle,s3_ts_timer);
  }

  //----------

    virtual // S3_Window_Base
    void renderLoop(void) {
      MSG msg;
      while (true) {
        while (PeekMessage(&msg,S3_NULL,0,0,PM_REMOVE)) {
          TranslateMessage(&msg);
          DispatchMessage(&msg);
        }
        if (msg.message == WM_QUIT) break;
        if (MListener) MListener->on_windowRenderFrame(this);
      }
    }

    virtual // S3_Window_Base
    void createUserCursor(uint32 AIndex, int32 AWidth, int32 AHeight, const uint8* AAndMask, const uint8* AXorMask, int32 AXhotspot, int32 AYhotspot) {
      if ((AIndex>=128) && (AIndex<=255)) {
        HCURSOR cursor = CreateCursor(s3_global_WinInstance,AXhotspot,AYhotspot,AWidth,AHeight,AAndMask,AXorMask);
        MUserCursors[AIndex-128] = cursor;
      }
    }

    virtual // S3_Window_Base
    void destroyUserCursors(void) {
      for (uint32 i=0; i<128; i++) {
        HCURSOR cursor = MUserCursors[i];
        if (cursor) DestroyCursor(cursor);
      }
    }

    //virtual // S3_Window_Base
    //void getCursorPos(int32* AXpos, int32* AYpos) {
    //  *AXpos = 0;
    //  *AYpos = 0;
    //}

    virtual // S3_Window_Base
    void warpCursor(int32 AXpos, int32 AYpos, int32 AWidth, int32 AHeight) {
      RECT rc;
      POINT pt = { AXpos, AYpos };
      POINT pt2 = { AXpos+AWidth-1, AYpos+AHeight-1 };
      ClientToScreen(MWinHandle, &pt);
      ClientToScreen(MWinHandle, &pt2);
      SetRect(&rc, pt.x, pt.y, pt2.x, pt2.y);
      ClipCursor(&rc);
    }

    //----------

    virtual // S3_Window_Base
    void unwarpCursor(void) {
      ClipCursor(NULL);
    }

  */

};

//----------------------------------------------------------------------
//
// event proc
//
//----------------------------------------------------------------------

LRESULT CALLBACK mip_win32_eventproc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  MIP_Win32Window* window = (MIP_Win32Window*)GetWindowLongPtr(hWnd,GWLP_USERDATA);
	if (!window) return DefWindowProc(hWnd,message,wParam,lParam);
  return window->eventHandler(hWnd, message, wParam, lParam);
}

//----------------------------------------------------------------------
// window class
//----------------------------------------------------------------------

/*
  https://msdn.microsoft.com/en-us/library/windows/desktop/ms633586%28v=vs.85%29.aspx

  All window classes that an application registers are unregistered when it
  terminates.
  No window classes registered by a DLL are unregistered when the DLL is
  unloaded. A DLL must explicitly unregister its classes when it is unloaded.
*/

//----------

/*
  this one is a bit messy..
  we need:
  - HINSTANCE (from DLL)
  - callback function (which calls the window class)
  it should be registered only once, even for multiple instances on a plugin,
  so the plugin can't destroy the window class when it dies, since another
  instance might still use the window class for its editor..
  so, we do something singleton-ish..
  a static class with a constructor that is called before DllMain and any
  other function/method/class..
  and a destructor that is called when the global variable goes out of scope,
  when the dll is unloaded..
*/

class MIP_Win32WindowClass {

//------------------------------
private:
//------------------------------

  bool     MRegistered                  = false;
  char     MName[MIP_MAX_STRING_LENGTH] = "mip_window_0";
  ATOM     MAtom                        = 0;
  WNDCLASS MClass                       = {};

//------------------------------
public:
//------------------------------

  MIP_Win32WindowClass() {
    MRegistered = false;
  }

  //----------

  ~MIP_Win32WindowClass() {
    if (MRegistered) {
      UnregisterClass(MName,MIP_GLOBAL_WIN32_INSTANCE);
    }
  }

  //----------

  char* getWindowClass() {
    //MIP_PRINT;
    if (!MRegistered) {
      MIP_CreateUniqueString(MName,(char*)"mip_window_",&MIP_GLOBAL_WIN32_INSTANCE);
      //MIP_Print("window name: %s\n",MName);
      memset(&MClass,0,sizeof(MClass));
      MClass.style          = MIP_WIN32_CLASS_STYLE;
      MClass.lpfnWndProc    = &mip_win32_eventproc;
      MClass.hInstance      = MIP_GLOBAL_WIN32_INSTANCE;
      MClass.lpszClassName  = MName;
      //#ifdef MIP_MOUSE_DOUBLECLICK
      //MClass.style |= CS_DBLCLKS;
      //#endif
      MAtom = RegisterClass(&MClass);
      MRegistered = true;
    }
    return MName;
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

MIP_Win32WindowClass MIP_GLOBAL_WIN32_WINDOW_CLASS = {};

//----------

char* MIP_Win32ClassName() {
  return MIP_GLOBAL_WIN32_WINDOW_CLASS.getWindowClass();
}

//----------------------------------------------------------------------
#endif

#endif // 0