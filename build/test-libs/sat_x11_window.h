#ifndef sat_x11_window_included
#define sat_x11_window_included
//----------------------------------------------------------------------

//#include <math.h>
//#include <memory.h>
//#include <stdint.h>
//#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/Xlib-xcb.h>
#include <xcb/xcb.h>
#include <xcb/xcb_util.h>
#include <xcb/xcb_cursor.h>
#include <xcb/xcb_image.h>
#include <xcb/xcb_keysyms.h>
#include <xkbcommon/xkbcommon.h>

#define explicit explicit_
#include <xcb/xkb.h>
#undef explicit

//----------

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

#define SAT_KEY_NONE                0
#define SAT_KEY_BACKSPACE           8
#define SAT_KEY_ENTER               13
#define SAT_KEY_ESC                 27
#define SAT_KEY_END                 35
#define SAT_KEY_HOME                36
#define SAT_KEY_LEFT                37
#define SAT_KEY_RIGHT               39
#define SAT_KEY_DELETE              46

#define SAT_STATE_NONE              0
#define SAT_STATE_SHIFT             1
#define SAT_STATE_CAPS              2
#define SAT_STATE_CTRL              4
#define SAT_STATE_ALT               8
#define SAT_STATE_ALTGR             16

#define SAT_WINDOW_THREAD_KILL      666
#define SAT_X11_WAIT_FOR_MAPNOTIFY

xcb_atom_t sat_xcb_get_intern_atom(xcb_connection_t *conn, const char *name);

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_X11Window {

//------------------------------
private:
//------------------------------

  Display*                    MDisplay                      = nullptr;
  xcb_connection_t*           MConnection                   = nullptr;
  int                         MDefaultScreen                = 0;
  xcb_screen_t*               MScreen                       = nullptr;
  xcb_gcontext_t              MScreenGC                     = XCB_NONE;
  xcb_window_t                MWindow                       = XCB_NONE;
  uint32_t                    MScreenWidth                  = 0;
  uint32_t                    MScreenHeight                 = 0;
  uint32_t                    MScreenDepth                  = 0;
  pthread_t                   MEventThread                  = 0;
  xcb_client_message_event_t* MClientMessageEvent           = (xcb_client_message_event_t*)MClientMessageEventBuffer;
  char*                       MClientMessageEventBuffer[32] = {0};
  xcb_expose_event_t*         MExposeEvent                  = (xcb_expose_event_t*)MExposeEventBuffer;
  char*                       MExposeEventBuffer[32]        = {0};
  bool                        MIsEmbedded                   = false;
  bool                        MIsMapped                     = false;
  bool                        MIsExposed                    = false;
  xcb_key_symbols_t*          MKeySyms                      = nullptr;
  xcb_atom_t                  MWMProtocolsAtom              = XCB_NONE;
  xcb_atom_t                  MWMDeleteWindowAtom           = XCB_NONE;
  xcb_pixmap_t                MEmptyPixmap                  = XCB_NONE;
  xcb_cursor_t                MHiddenCursor                 = XCB_NONE;
  xcb_cursor_t                MWindowCursor                 = XCB_NONE;
  bool                        MIsCursorHidden               = false;
  bool                        MIsEventThreadActive          = false;
  //std::atomic<bool>           MIsEventThreadActive          {false};

//------------------------------
protected:
//------------------------------

  int32_t                     MWindowXpos                   = 0;
  int32_t                     MWindowYpos                   = 0;
  uint32_t                    MWindowWidth                  = 0;
  uint32_t                    MWindowHeight                 = 0;

//------------------------------
public:
//------------------------------

  SAT_X11Window(uint32_t AWidth, uint32_t AHeight, intptr_t AParent=0) {

    // x server connection

    //MConnection = xcb_connect(ADisplayName,&MDefaultScreen);
    MDisplay = XOpenDisplay(nullptr);
    MConnection = XGetXCBConnection(MDisplay);
    XSetEventQueueOwner(MDisplay,XCBOwnsEventQueue);
    MDefaultScreen = DefaultScreen(MDisplay); // ???

    // screen

    uint32_t screen_num = MDefaultScreen;
    xcb_screen_iterator_t iter;
    iter = xcb_setup_roots_iterator(xcb_get_setup(MConnection));
    for (; iter.rem; --screen_num, xcb_screen_next(&iter)) {
      if (screen_num == 0) {
        MScreen = iter.data;
        break;
      }
    }

    MScreenWidth    = MScreen->width_in_pixels;
    MScreenHeight   = MScreen->height_in_pixels;
    MScreenDepth    = MScreen->root_depth;
    //MScreenWindow   = MScreen->root;
    //MScreenColormap = MScreen->default_colormap;
    //MScreenVisual   = MScreen->root_visual;
    //MScreenDrawable = MScreen->root;

    // screen gc

    MScreenGC = xcb_generate_id(MConnection);
    xcb_drawable_t draw = MScreen->root;
    uint32_t mask = XCB_GC_FOREGROUND | XCB_GC_BACKGROUND;
    uint32_t values[2];
    values[0] = MScreen->black_pixel;
    values[1] = MScreen->white_pixel;
    xcb_create_gc(MConnection, MScreenGC, draw, mask, values);

    // window

    MWindowXpos   = 0;
    MWindowYpos   = 0;
    MWindowWidth  = AWidth;
    MWindowHeight = AHeight;

    uint32_t event_mask =
      XCB_EVENT_MASK_KEY_PRESS          |
      XCB_EVENT_MASK_KEY_RELEASE        |
      XCB_EVENT_MASK_BUTTON_PRESS       |
      XCB_EVENT_MASK_BUTTON_RELEASE     |
      XCB_EVENT_MASK_ENTER_WINDOW       |
      XCB_EVENT_MASK_LEAVE_WINDOW       |
      XCB_EVENT_MASK_POINTER_MOTION     |
      XCB_EVENT_MASK_EXPOSURE           |
      //XCB_EVENT_MASK_RESIZE_REDIRECT    |
      //XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY|
      XCB_EVENT_MASK_STRUCTURE_NOTIFY;
      
    uint32_t window_mask =
      XCB_CW_BACK_PIXMAP  |
      XCB_CW_BORDER_PIXEL |
      XCB_CW_EVENT_MASK   |
      XCB_CW_COLORMAP;
    uint32_t window_mask_values[4] = {
      XCB_NONE,                           // shouldn't this be XCB_BACK_PIXMAP_NONE ??
      0,
      event_mask,
      MScreen->default_colormap
    };

    MWindow = xcb_generate_id(MConnection);

    xcb_create_window(
      MConnection,                        // connection
      XCB_COPY_FROM_PARENT,               // depth (same as root)
      MWindow,                            // window Id
      AParent ? AParent : MScreen->root,  // parent window
      0, 0,                               // x, y
      AWidth, AHeight,                    // width, height
      0,                                  // border_width
      XCB_WINDOW_CLASS_INPUT_OUTPUT,      // class
      MScreen->root_visual,               // visual
      window_mask,
      window_mask_values
    );

    // mouse

    MEmptyPixmap = xcb_generate_id(MConnection);
    MHiddenCursor = xcb_generate_id(MConnection);
    xcb_create_pixmap(
      MConnection,
      1,
      MEmptyPixmap,
      MWindow,
      1,
      1
    );
    xcb_create_cursor(
      MConnection,
      MHiddenCursor,
      MEmptyPixmap,
      MEmptyPixmap,
      0, 0, 0,  // fore color
      0, 0, 0,  // back color
      0, 0      // x,y
    );

    // keyboard

    MKeySyms = xcb_key_symbols_alloc(MConnection);

    //

    if (AParent) {
      MIsEmbedded = true;
      removeDecorations();
    }
    else {
      MIsEmbedded = false;
      wantQuitEvents();
    }

  }

  //----------

  virtual ~SAT_X11Window() {
    if (MIsMapped) hide();
    xcb_key_symbols_free(MKeySyms);
    xcb_free_pixmap(MConnection, MEmptyPixmap);
    xcb_free_cursor(MConnection,MHiddenCursor);
    if (MWindowCursor != XCB_NONE) xcb_free_cursor(MConnection,MWindowCursor);
    xcb_destroy_window(MConnection,MWindow);
    xcb_free_gc(MConnection,MScreenGC);
    XSetEventQueueOwner(MDisplay,XlibOwnsEventQueue);
    XCloseDisplay(MDisplay);
    MDisplay = nullptr;
    MConnection = nullptr;
  }

//------------------------------
public:
//------------------------------

  virtual void on_window_open() {}
  virtual void on_window_close() {}
  virtual void on_window_move(int32_t AXpos, int32_t AYpos) {}
  virtual void on_window_resize(int32_t AWidth, int32_t AHeight) {}
  virtual void on_window_paint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) {}
  virtual void on_window_key_press(uint8_t AChar, uint32_t AKeySym, uint32_t AState, uint32_t ATime) {}
  virtual void on_window_key_release(uint8_t AChar, uint32_t AKeySym, uint32_t AState, uint32_t ATime) {}
  virtual void on_window_mouse_click(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) {}
  virtual void on_window_mouse_release(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) {}
  virtual void on_window_mouse_move(int32_t AXpos, int32_t AYpos, uint32_t AState, uint32_t ATime) {}
  virtual void on_window_mouse_enter(int32_t AXpos, int32_t AYpos, uint32_t ATime) {}
  virtual void on_window_mouse_leave(int32_t AXpos, int32_t AYpos, uint32_t ATime) {}
  virtual void on_window_client_message(uint32_t AData) {}

//------------------------------
public:
//------------------------------

  Display*      getX11Display() { return MDisplay;}
  xcb_window_t  getX11Window()  { return MWindow;}
  
//------------------------------
public:
//------------------------------

  virtual uint32_t getScreenWidth()   { return MScreenWidth; }
  virtual uint32_t getScreenHeight()  { return MScreenHeight; }
  virtual uint32_t getScreenDepth()   { return MScreenDepth; }
  
  //----------

  virtual uint32_t getWidth() {
    return MWindowWidth;
  }
  
  //----------

  virtual uint32_t getHeight() {
    return MWindowWidth;
  }

  //----------

  virtual void setPos(uint32_t AXpos, uint32_t AYpos) {
    MWindowXpos = AXpos;
    MWindowYpos = AYpos;
    uint32_t values[] = { AXpos, AYpos };
    xcb_configure_window(MConnection,MWindow,XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y,values);
    xcb_flush(MConnection);
  }

  //----------

  virtual void setSize(uint32_t AWidth, uint32_t AHeight) {
    MWindowWidth = AWidth;
    MWindowHeight = AHeight;
    uint32_t values[] = { AWidth, AHeight };
    xcb_configure_window(MConnection,MWindow,XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT,values);
    xcb_flush(MConnection);
  }

  //----------

  virtual void setTitle(const char* ATitle) {
    xcb_change_property(
      MConnection,
      XCB_PROP_MODE_REPLACE,
      MWindow,
      XCB_ATOM_WM_NAME,
      XCB_ATOM_STRING,
      8,
      strlen(ATitle),
      ATitle
    );
    xcb_flush(MConnection);
  }

  //----------

  virtual void show() {
    xcb_map_window(MConnection,MWindow);
    xcb_flush(MConnection);
    #ifdef SAT_X11_WAIT_FOR_MAPNOTIFY
      waitForMapNotify();
      MIsMapped = true;
      on_window_open();
    #endif
    if (MIsEmbedded) startEventThread();
  }

  //----------

  virtual void hide() {
    if (MIsEmbedded) stopEventThread();
    xcb_unmap_window(MConnection,MWindow);
    xcb_flush(MConnection);
    MIsMapped = false;
    MIsExposed = false;
  }

//------------------------------
public:
//------------------------------

  virtual void reparent(intptr_t AParent) {
    if (AParent != 0) {
      MIsEmbedded = true;
      xcb_reparent_window(MConnection,MWindow,AParent,0,0);
      xcb_flush(MConnection);
    }
  }

  //----------

  virtual void invalidate(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) {
    // x11 doesn't like negative coords (uint16_t)
    AXpos = max(0,min(AXpos,MWindowWidth));
    AYpos = max(0,min(AYpos,MWindowHeight));
    memset(MExposeEventBuffer,0,sizeof(MExposeEventBuffer));
    MExposeEvent->window        = MWindow;
    MExposeEvent->response_type = XCB_EXPOSE;
    MExposeEvent->x             = AXpos;
    MExposeEvent->y             = AYpos;
    MExposeEvent->width         = AWidth;
    MExposeEvent->height        = AHeight;
    xcb_send_event(
      MConnection,
      false,
      MWindow,
      XCB_EVENT_MASK_EXPOSURE,
      (char*)MExposeEvent
    );
    xcb_flush(MConnection);
  }

  //----------

  virtual void sendClientMessage(uint32_t AData, uint32_t AType) {
    memset(MClientMessageEventBuffer,0,sizeof(MClientMessageEventBuffer));
    MClientMessageEvent->window         = MWindow;
    MClientMessageEvent->response_type  = XCB_CLIENT_MESSAGE;
    MClientMessageEvent->format         = 32; // didn't work without this
    MClientMessageEvent->type           = AType;
    MClientMessageEvent->data.data32[0] = AData;
    xcb_send_event(
      MConnection,
      false,
      MWindow,
      XCB_EVENT_MASK_NO_EVENT,
      (char*)MClientMessageEvent
    );
    xcb_flush(MConnection);
  }

  //----------

  virtual uint32_t eventLoop() {
    xcb_generic_event_t* event = getEvent(true);
    while (event) {
      bool result = processEvent(event);
      if (result == false) return 1;
      event = getEvent(true);
    }
    return 0;
  }

  //----------

  virtual void startEventThread() {
    MIsEventThreadActive = true;
    pthread_create(&MEventThread,nullptr,xcb_event_thread_callback,this);
  }

  //----------

  virtual void stopEventThread() {
    void* ret;
    MIsEventThreadActive = false;
    sendClientMessage(SAT_WINDOW_THREAD_KILL,0);
    //xcb_flush(MConnection);
    pthread_join(MEventThread,&ret);
  }

//------------------------------
public:
//------------------------------

  virtual void fill(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight, uint32_t AColor) {
    // set color
    uint32_t mask = XCB_GC_FOREGROUND;
    uint32_t values[1];
    values[0] = AColor;
    xcb_change_gc(MConnection,MScreenGC,mask,values);
    // fill rectangle
    xcb_rectangle_t rectangles[] = {{
      (int16_t)AXpos,     //0,
      (int16_t)AYpos,     //0,
      (uint16_t)AWidth,   //MWindowWidth,
      (uint16_t)AHeight,  //MWindowHeight
    }};
    xcb_poly_fill_rectangle(MConnection,MWindow,MScreenGC,1,rectangles);
    xcb_flush(MConnection);
  }

  //----------

  virtual void fill(uint32_t AColor) {
    fill(0,0,MWindowWidth,MWindowHeight,AColor);
  }

//------------------------------
public: // mouse
//------------------------------

  virtual void setMouseCursor(int32_t ACursor) {
    setWMCursor(ACursor);
  }

  //----------

  virtual void setMouseCursorPos(int32_t AXpos, int32_t AYpos) {
    xcb_warp_pointer(MConnection,XCB_NONE,MWindow,0,0,0,0,AXpos,AYpos);
    xcb_flush(MConnection);
  }

  //----------

  virtual void hideMouseCursor(void) {
    if (!MIsCursorHidden) {
      setXcbCursor(MHiddenCursor);
      MIsCursorHidden = true;
    }
  }

  //----------

  virtual void showMouseCursor(void) {
    if (MIsCursorHidden) {
      setXcbCursor(MWindowCursor);
      MIsCursorHidden = false;
    }
  }

  //----------

  virtual void grabMouseCursor(void) {
    int32_t event_mask =
      XCB_EVENT_MASK_BUTTON_PRESS   |
      XCB_EVENT_MASK_BUTTON_RELEASE |
      XCB_EVENT_MASK_POINTER_MOTION |
      XCB_EVENT_MASK_FOCUS_CHANGE   |
      XCB_EVENT_MASK_ENTER_WINDOW   |
      XCB_EVENT_MASK_LEAVE_WINDOW;
    xcb_grab_pointer(
      MConnection,
      0,                    // owner_events
      MWindow,              // grab_window
      event_mask,           // event_mask
      XCB_GRAB_MODE_ASYNC,  // pointer_mode
      XCB_GRAB_MODE_ASYNC,  // keyboard_mode
      XCB_NONE,             // confine_to
      XCB_NONE,             // cursor
      XCB_CURRENT_TIME      // timestamp
    );
    xcb_flush(MConnection);
  }
  
  //----------

  virtual void releaseMouseCursor(void) {
    xcb_ungrab_pointer(MConnection,XCB_CURRENT_TIME);
    xcb_flush(MConnection);
  }

//------------------------------
private: // mouse
//------------------------------

  void setXcbCursor(xcb_cursor_t ACursor) {
    uint32_t mask;
    uint32_t values;
    mask   = XCB_CW_CURSOR;
    values = ACursor;
    xcb_change_window_attributes(MConnection,MWindow,mask,&values);
    // without xcb_flush, the mouse cursor wasn't updating properly (standalone)..
    xcb_flush(MConnection);
  }

  //----------

  void freeXcbCursor(void) {
    if (MWindowCursor != XCB_NONE) {
      xcb_free_cursor(MConnection,MWindowCursor);
      MWindowCursor = XCB_NONE;
      //xcb_flush(MConnection);
    }
  }

  //----------

  void setWMCursor(uint32_t ACursor) {
    xcb_cursor_context_t *ctx;
    if (xcb_cursor_context_new(MConnection, MScreen, &ctx) >= 0) {
      const char* name = "left_ptr"; //SAT_XCB_WM_CURSORS[ACursor];
      xcb_cursor_t cursor = xcb_cursor_load_cursor(ctx, name);
      if (cursor != XCB_CURSOR_NONE) {
        xcb_change_window_attributes(MConnection, MWindow, XCB_CW_CURSOR, &cursor);
      }
    }
    xcb_cursor_context_free(ctx);
    xcb_flush(MConnection);
  }

//------------------------------
private:
//------------------------------

  void wantQuitEvents() {
    xcb_intern_atom_cookie_t  protocol_cookie = xcb_intern_atom_unchecked(MConnection, 1, 12, "WM_PROTOCOLS");
    xcb_intern_atom_cookie_t  close_cookie    = xcb_intern_atom_unchecked(MConnection, 0, 16, "WM_DELETE_WINDOW");
    xcb_intern_atom_reply_t*  protocol_reply  = xcb_intern_atom_reply(MConnection, protocol_cookie, 0);
    xcb_intern_atom_reply_t*  close_reply     = xcb_intern_atom_reply(MConnection, close_cookie, 0);
    MWMProtocolsAtom    = protocol_reply->atom;
    MWMDeleteWindowAtom = close_reply->atom;
    xcb_change_property(MConnection, XCB_PROP_MODE_REPLACE, MWindow, MWMProtocolsAtom, 4, 32, 1, &MWMDeleteWindowAtom);
    //xcb_flush(MConnection);
    free(protocol_reply); // note malloc'ed ??
    free(close_reply);
  }

  //----------

  const unsigned long MWM_HINTS_FUNCTIONS   = 1 << 0;
  const unsigned long MWM_HINTS_DECORATIONS = 1 << 1;
  //const unsigned long MWM_DECOR_ALL         = 1 << 0;
  const unsigned long MWM_DECOR_BORDER      = 1 << 1;
  const unsigned long MWM_DECOR_RESIZEH     = 1 << 2;
  const unsigned long MWM_DECOR_TITLE       = 1 << 3;
  const unsigned long MWM_DECOR_MENU        = 1 << 4;
  const unsigned long MWM_DECOR_MINIMIZE    = 1 << 5;
  const unsigned long MWM_DECOR_MAXIMIZE    = 1 << 6;
  //const unsigned long MWM_FUNC_ALL          = 1 << 0;
  const unsigned long MWM_FUNC_RESIZE       = 1 << 1;
  const unsigned long MWM_FUNC_MOVE         = 1 << 2;
  const unsigned long MWM_FUNC_MINIMIZE     = 1 << 3;
  const unsigned long MWM_FUNC_MAXIMIZE     = 1 << 4;
  const unsigned long MWM_FUNC_CLOSE        = 1 << 5;

  struct WMHints {
    uint32_t flags;
    uint32_t functions;
    uint32_t decorations;
    int32_t  inputMode;
    uint32_t state;
  };

  //----------

  void removeDecorations() {
    xcb_atom_t prop = sat_xcb_get_intern_atom(MConnection,"_MOTIF_WM_HINTS");
    if (prop) {
      WMHints hints;
      hints.flags = MWM_HINTS_DECORATIONS;
      hints.decorations = 0;
      const unsigned char* ptr = (const unsigned char*)(&hints);
      xcb_change_property(
        MConnection,
        XCB_PROP_MODE_REPLACE,
        MWindow,
        prop,     // hintsAtomReply->atom,
        prop,     // XCB_ATOM_WM_HINTS,
        32,
        5,        // PROP_MOTIF_WM_HINTS_ELEMENTS
        ptr
      );
    }
  }

  //----------

  void waitForMapNotify() {
    xcb_flush(MConnection);
    while (1) {
      xcb_generic_event_t* event;
      event = xcb_wait_for_event(MConnection);
      uint8_t e = event->response_type & ~0x80;
      free(event); // not malloc'ed
      if (e == XCB_MAP_NOTIFY) {
        MIsMapped = true;
        break;
      }
    }
  }

  //----------

  // returns null if no events
  // caller must free(event) !!!!

  xcb_generic_event_t* getEvent(bool ABlock=false) {
    //xcb_flush(MConnection);
    xcb_generic_event_t* event = nullptr;
    if (ABlock) event = xcb_wait_for_event(MConnection);
    else event = xcb_poll_for_event(MConnection);
    return event;
  }

  //----------
  
  uint32_t remapKey(uint32_t AKey, uint32_t AState) {
    int col = 0;
    xcb_keysym_t keysym = xcb_key_symbols_get_keysym(MKeySyms,AKey,col);
    uint32_t ks = keysym;
    switch (keysym) {
      case XKB_KEY_Return:      ks = SAT_KEY_ENTER;     break;
      case XKB_KEY_KP_Space:    ks = SAT_KEY_ESC;       break;
      case XKB_KEY_Home:        ks = SAT_KEY_HOME;      break;
      case XKB_KEY_End:         ks = SAT_KEY_END;       break;
      case XKB_KEY_Left:        ks = SAT_KEY_LEFT;      break;
      case XKB_KEY_Right:       ks = SAT_KEY_RIGHT;     break;
      case XKB_KEY_Delete:      ks = SAT_KEY_DELETE;    break;
      case XKB_KEY_BackSpace:   ks = SAT_KEY_BACKSPACE; break;
    }
    return ks;
  }

  //----------

  uint32_t remapButton(uint32_t AButton, uint32_t AState) {
    uint32_t b = AButton;
    return b;
  }

  //----------

  uint32_t remapState(uint32_t AState) {
    uint32_t s = SAT_STATE_NONE;
    if (AState & XCB_MOD_MASK_SHIFT)    s += SAT_STATE_SHIFT;
    if (AState & XCB_MOD_MASK_LOCK)     s += SAT_STATE_CAPS;
    if (AState & XCB_MOD_MASK_CONTROL)  s += SAT_STATE_CTRL;
    if (AState & XCB_MOD_MASK_1)        s += SAT_STATE_ALT;
    if (AState & XCB_MOD_MASK_5)        s += SAT_STATE_ALTGR;
    //if (AState & XCB_MOD_MASK_1) SAT_Print("1\n");
    //if (AState & XCB_MOD_MASK_2) SAT_Print("2\n");
    //if (AState & XCB_MOD_MASK_3) SAT_Print("3\n");
    //if (AState & XCB_MOD_MASK_4) SAT_Print("4\n");
    //if (AState & XCB_MOD_MASK_5) SAT_Print("5\n");
    return s;
  }

  //----------

  // returns false if we received MWMDeleteWindowAtom
  // -> we are done (quit)
  // frees AEvent

  bool processEvent(xcb_generic_event_t* AEvent) {
    switch (AEvent->response_type & ~0x80) {

      case XCB_MAP_NOTIFY: {
        MIsMapped = true;
        on_window_open();
        break;
      }

      case XCB_UNMAP_NOTIFY: {
        MIsMapped = false;
        on_window_close();
        break;
      }

      case XCB_CONFIGURE_NOTIFY: {
        xcb_configure_notify_event_t* configure_notify = (xcb_configure_notify_event_t*)AEvent;
        int16_t x  = configure_notify->x;
        int16_t y  = configure_notify->y;
        uint16_t w = configure_notify->width;
        uint16_t h = configure_notify->height;
        if ((x != MWindowXpos) || (y != MWindowYpos)) {
          MWindowXpos = x;
          MWindowYpos = y;
          on_window_move(x,y);
        }
        if ((w != MWindowWidth) || (h != MWindowHeight)) {
          MWindowWidth  = w;
          MWindowHeight = h;
          on_window_resize(w,h);
        }
        break;
      }

      case XCB_EXPOSE: {
        xcb_expose_event_t* expose = (xcb_expose_event_t *)AEvent;
        uint16_t x = expose->x;
        uint16_t y = expose->y;
        uint16_t w = expose->width;
        uint16_t h = expose->height;
        on_window_paint(x,y,w,h);
        xcb_flush(MConnection);
        MIsExposed = true;
        break;
      }

      case XCB_KEY_PRESS: {
        xcb_key_press_event_t* key_press = (xcb_key_press_event_t*)AEvent;
        uint8_t  k  = key_press->detail;
        uint16_t s  = key_press->state;
        uint32_t ts = key_press->time;
        uint32_t ks = remapKey(k,s);
        s = remapState(s);
        on_window_key_press(k,ks,s,ts);
       break;
      }

      case XCB_KEY_RELEASE: {
        xcb_key_release_event_t* key_release = (xcb_key_release_event_t*)AEvent;
        uint8_t  k  = key_release->detail;
        uint16_t s  = key_release->state;
        uint32_t ts = key_release->time;
        uint32_t ks = remapKey(k,s);
        s = remapState(s);
        on_window_key_release(k,ks,s,ts);
        break;
      }

      case XCB_BUTTON_PRESS: {
        xcb_button_press_event_t* button_press = (xcb_button_press_event_t*)AEvent;
        uint8_t  b  = button_press->detail;
        uint32_t s  = button_press->state;
        int32_t  x  = button_press->event_x;
        int32_t  y  = button_press->event_y;
        uint32_t ts = button_press->time;
        b = remapButton(b,s);
        s = remapState(s);
        on_window_mouse_click(x,y,b,s,ts);
        break;
      }

      case XCB_BUTTON_RELEASE: {
        xcb_button_release_event_t* button_release = (xcb_button_release_event_t*)AEvent;
        uint32_t b  = button_release->detail;
        uint32_t s  = button_release->state;
        int32_t  x  = button_release->event_x;
        int32_t  y  = button_release->event_y;
        uint32_t ts = button_release->time;
        b = remapButton(b,s);
        s = remapState(s);
        on_window_mouse_release(x,y,b,s,ts);
        break;
      }

      case XCB_MOTION_NOTIFY: {
        xcb_motion_notify_event_t* motion_notify = (xcb_motion_notify_event_t*)AEvent;
        uint32_t  s = motion_notify->state;
        int32_t   x = motion_notify->event_x;
        int32_t   y = motion_notify->event_y;
        uint32_t ts = motion_notify->time;
        s = remapState(s);
        on_window_mouse_move(x,y,s,ts);
        break;
      }

      case XCB_ENTER_NOTIFY: {
        if (!MIsMapped) break;
        xcb_enter_notify_event_t* enter_notify = (xcb_enter_notify_event_t*)AEvent;
        int32_t   x = enter_notify->event_x;
        int32_t   y = enter_notify->event_y;
        uint32_t ts = enter_notify->time;
        on_window_mouse_enter(x,y,ts);
        break;
      }

      case XCB_LEAVE_NOTIFY: {
        if (!MIsMapped) break;
        xcb_leave_notify_event_t* leave_notify = (xcb_leave_notify_event_t*)AEvent;
        int32_t   x = leave_notify->event_x;
        int32_t   y = leave_notify->event_y;
        uint32_t ts = leave_notify->time;
        on_window_mouse_leave(x,y,ts);
        break;
      }

      case XCB_CLIENT_MESSAGE: {
        xcb_client_message_event_t* client_message = (xcb_client_message_event_t*)AEvent;
        uint32_t data = client_message->data.data32[0];
        on_window_client_message(data);
        if (data == SAT_WINDOW_THREAD_KILL) {
          free(AEvent); // not malloc'ed
          return false; // we' re finished
        }
        if (data == MWMDeleteWindowAtom) {
          free(AEvent); // not malloc'ed
          return false; // we' re finished
        }
        break;
      }

    }
    free(AEvent);
    return true; // we are still alive
  }

  //----------

  static
  void* xcb_event_thread_callback(void* AWindow) {
    SAT_X11Window* window = (SAT_X11Window*)AWindow;
    if (window) {
      xcb_connection_t* connection = window->MConnection;
      xcb_flush(connection);
      while (window->MIsEventThreadActive) {
        xcb_generic_event_t* event = xcb_wait_for_event(connection);
        if (event) {
          if (window->MIsEventThreadActive) {
            if (!window->processEvent(event)) {
              return nullptr;
            }
          }
        }
      }
      return nullptr;
    }
    return nullptr;
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

// is this needed ???

bool sat_initialize_xlib() {
  XInitThreads();
  return true;
}

//static
bool sat_xlib_is_initialized = sat_initialize_xlib();

xcb_atom_t sat_xcb_get_intern_atom(xcb_connection_t *conn, const char *name) {
  xcb_intern_atom_cookie_t cookie = xcb_intern_atom(conn ,0, strlen(name), name);
  xcb_intern_atom_reply_t *reply = xcb_intern_atom_reply(conn, cookie, NULL);
  return reply->atom;
}

//----------

#undef min
#undef max

//----------------------------------------------------------------------
#endif

