#ifndef sat_x11_window_included
#define sat_x11_window_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "gui/sat_opengl.h"
#include "gui/x11/sat_x11.h"
//#include "gui/x11/sat_x11_opengl.h"
#include "gui/x11/sat_x11_utils.h"

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
  bool                        MIsEventThreadActive          = false;
  pthread_t                   MEventThread                  = 0;
  xcb_client_message_event_t* MClientMessageEvent           = (xcb_client_message_event_t*)MClientMessageEventBuffer;
  char*                       MClientMessageEventBuffer[32] = {0};
  xcb_expose_event_t*         MExposeEvent                  = (xcb_expose_event_t*)MExposeEventBuffer;
  char*                       MExposeEventBuffer[32]        = {0};
  bool                        MIsEmbedded                   = false;
  bool                        MIsMapped                     = false;
  bool                        MIsExposed                    = false;
  xcb_key_symbols_t*          MKeySyms                      = nullptr;

  //SAT_X11OpenGL*              MOpenGL                       = nullptr;
  //xcb_pixmap_t                MEmptyPixmap                  = XCB_NONE;
  //xcb_cursor_t                MHiddenCursor                 = XCB_NONE;
  //xcb_cursor_t                MWindowCursor                 = XCB_NONE;
  //uint32_t                    MBackgroundColor              = 0x800080;

//------------------------------
protected:
//------------------------------

  uint32_t                    MXpos                         = 0;
  uint32_t                    MYpos                         = 0;
  uint32_t                    MWidth                        = 0;
  uint32_t                    MHeight                       = 0;

//  bool                        MFillBackground               = true;
//  SAT_Color                   MBackgroundColor              = SAT_DarkGreen;

//------------------------------
public:
//------------------------------

  SAT_X11Window(uint32_t AWidth, uint32_t AHeight, intptr_t AParent) {

    //initConnection(nullptr);

    //MConnection = xcb_connect(ADisplayName,&MDefaultScreen);
    MDisplay = XOpenDisplay(nullptr);
    SAT_Log("MDisplay: %p\n",MDisplay);
    MConnection = XGetXCBConnection(MDisplay);
    SAT_Log("MConnection: %p\n",MConnection);
    XSetEventQueueOwner(MDisplay,XCBOwnsEventQueue);
    MDefaultScreen = DefaultScreen(MDisplay); // ???
    SAT_Log("MDefaultScreen: %i\n",MDefaultScreen);

    //initScreen();

    uint32_t screen_num = MDefaultScreen;
    xcb_screen_iterator_t iter;
    iter = xcb_setup_roots_iterator(xcb_get_setup(MConnection));
    for (; iter.rem; --screen_num, xcb_screen_next(&iter)) {
      if (screen_num == 0) {
        MScreen = iter.data;
        break;
      }
    }

    //MScreenWidth    = MScreen->width_in_pixels;
    //MScreenHeight   = MScreen->height_in_pixels;
    //MScreenDepth    = MScreen->root_depth;
    //MScreenWindow   = MScreen->root;
    //MScreenColormap = MScreen->default_colormap;
    //MScreenVisual   = MScreen->root_visual;
    //MScreenDrawable = MScreen->root;

    //initScreenGC();

    MScreenGC = xcb_generate_id(MConnection);
    xcb_drawable_t draw = MScreen->root;
    uint32_t mask = XCB_GC_FOREGROUND | XCB_GC_BACKGROUND;
    uint32_t values[2];
    values[0] = MScreen->black_pixel;
    values[1] = MScreen->white_pixel;
    xcb_create_gc(MConnection, MScreenGC, draw, mask, values);

    //initWindow(AWidth,AHeight);

    MXpos   = 0;
    MYpos   = 0;
    MWidth  = AWidth;
    MHeight = AHeight;

    uint32_t event_mask =
      XCB_EVENT_MASK_KEY_PRESS          |
      XCB_EVENT_MASK_KEY_RELEASE        |
      XCB_EVENT_MASK_BUTTON_PRESS       |
      XCB_EVENT_MASK_BUTTON_RELEASE     |
      XCB_EVENT_MASK_ENTER_WINDOW       |
      XCB_EVENT_MASK_LEAVE_WINDOW       |
      XCB_EVENT_MASK_POINTER_MOTION     |
      XCB_EVENT_MASK_EXPOSURE           |
      //XCB_EVENT_MASK_RESIZE_REDIRECT  |
      XCB_EVENT_MASK_STRUCTURE_NOTIFY;
    uint32_t window_mask =
      XCB_CW_BACK_PIXMAP    |
      XCB_CW_BORDER_PIXEL   |
      XCB_CW_EVENT_MASK     |
      XCB_CW_COLORMAP;
    uint32_t window_mask_values[4] = {
      XCB_NONE,
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

    //initMouseCursor();
    //initKeyboard();
    MKeySyms = xcb_key_symbols_alloc(MConnection);

    if (AParent) {
      MIsEmbedded = true;
      removeDecorations();
    }
    else {
      MIsEmbedded = false;
      wantQuitEvents();
    }

    //setTitle(MWindowTitle);
    //MGuiTimer = new MIP_Timer(this);

    //MOpenGL = new SAT_X11OpenGL(MDisplay,MWindow);//,AWidth,AHeight);

  }

  //----------

  virtual ~SAT_X11Window() {
    if (MIsMapped) hide();

    //delete MOpenGL;

    //if (MGuiTimer) {
    //  LOG.print("XCB Stopping gui timer (destructor)\n");
    //  MGuiTimer->stop();
    //  delete MGuiTimer;
    //}
    //cleanupKeyboard();
    xcb_key_symbols_free(MKeySyms);
    //cleanupMouseCursor();
    //cleanupWindow();
    xcb_destroy_window(MConnection,MWindow);
    //cleanupScreenGC();
    xcb_free_gc(MConnection,MScreenGC);
    //cleanupScreen();
    //cleanupConnection();
    XSetEventQueueOwner(MDisplay,XlibOwnsEventQueue);
    XCloseDisplay(MDisplay);
    MDisplay = nullptr;
    MConnection = nullptr;
  }

//------------------------------
public:
//------------------------------

//  void setFillBackground(bool AFill=true)   { MFillBackground = AFill; }
//  void setBackgroundColor(SAT_Color AColor) { MBackgroundColor = AColor; }

  Display*      getX11Display() { return MDisplay;}
  xcb_window_t  getX11Window()  { return MWindow;}

//------------------------------
public:
//------------------------------

  virtual void on_window_open() {}
  virtual void on_window_close() {}
  virtual void on_window_move(int32_t AXpos, int32_t AYpos) {}
  virtual void on_window_resize(int32_t AWidth, int32_t AHeight) {}
  virtual void on_window_paint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) {}
  virtual void on_window_key_press(uint32_t AKey, uint32_t AState, uint32_t ATime) {}
  virtual void on_window_key_release(uint32_t AKey, uint32_t AState, uint32_t ATime) {}
  virtual void on_window_mouse_click(uint32_t AButton, uint32_t AState, int32_t AXpos, int32_t AYpos, uint32_t ATime) {}
  virtual void on_window_mouse_release(uint32_t AButton, uint32_t AState, int32_t AXpos, int32_t AYpos, uint32_t ATime) {}
  virtual void on_window_mouse_move(uint32_t s, int32_t AXpos, int32_t AYpos, uint32_t ATime) {}
  virtual void on_window_enter(int32_t AXpos, int32_t AYpos, uint32_t ATime) {}
  virtual void on_window_leave(int32_t AXpos, int32_t AYpos, uint32_t ATime) {}
  virtual void on_window_client_message(uint32_t AData) {}

//------------------------------
public:
//------------------------------

  virtual void setPos(uint32_t AXpos, uint32_t AYpos) {
    MXpos = AXpos;
    MYpos = AYpos;
    uint32_t values[] = { AXpos, AYpos };
    xcb_configure_window(MConnection,MWindow,XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y,values);
    xcb_flush(MConnection);
  }

  //----------

  virtual void setSize(uint32_t AWidth, uint32_t AHeight) {
    MWidth = AWidth;
    MHeight = AHeight;
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
    #ifdef MIP_X11_WAIT_FOR_MAPNOTIFY
      waitForMapNotify();
    #endif
    startEventThread();
  }

  //----------

  virtual void hide() {
    stopEventThread();
    xcb_unmap_window(MConnection,MWindow);
    xcb_flush(MConnection);
    MIsMapped = false;
    MIsExposed = false;
  }

//------------------------------
public:
//------------------------------

  void reparent(intptr_t AParent) {
    if (AParent != 0) {
      MIsEmbedded = true;
      xcb_reparent_window(MConnection,MWindow,AParent,0,0);
      xcb_flush(MConnection);
    }
  }

  //----------

  void invalidate(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) {
    //MIP_PRINT;
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

  void sendClientMessage(uint32_t AData, uint32_t AType) {
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

  uint32_t eventLoop() {
    xcb_generic_event_t* event = getEvent(true);
    while (event) {
      bool quit = !processEvent(event);
      if (quit) break;
      event = getEvent(true);
    }
    return 0;
  }

//------------------------------
private:
//------------------------------

  void startEventThread() {
    MIsEventThreadActive = true;
    pthread_create(&MEventThread,nullptr,xcb_event_thread_proc,this);
  }

  //----------

  void stopEventThread() {
    void* ret;
    MIsEventThreadActive = false;
    sendClientMessage(SAT_WINDOW_THREAD_KILL,0);
    pthread_join(MEventThread,&ret);
  }

  //----------

  void beginPaint() {
  }

  //----------

  void endPaint() {
    xcb_flush(MConnection);
  }

  //----------

  void paint() {
    beginPaint();
    //uint32_t color = MBackgroundColor;
    //if (MFillBackground) fill(0,0,MWidth,MHeight,color);
    on_window_paint(0,0,MWidth,MHeight);
    endPaint();
  }

  //----------

//  void paint(int32_t x, int32_t y, int32_t w, int32_t h) {
//    beginPaint();
//    if (MFillBackground) fill(x,y,w,h,MBackgroundColor);
//    on_window_paint(x,y,w,h);
//    endPaint();
//  }

  //----------

  void fill(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight, uint32_t AColor) {
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

  void fill(uint32_t AColor) {
    fill(0,0,MWidth,MHeight,AColor);
  }

//------------------------------
private:
//------------------------------

  xcb_atom_t MWMProtocolsAtom    = XCB_NONE;
  xcb_atom_t MWMDeleteWindowAtom = XCB_NONE;

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

  void removeDecorations() {

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
    //if (event) {
    //  eventHandler(event);
    //  free(event); // not MALLOC'ed
    //}
    return event;
  }

  //----------

  // https://github.com/etale-cohomology/xcb/blob/master/loop.c

  uint32_t remapKey(uint32_t AKey, uint32_t AState) {
    int col = 0;
    xcb_keysym_t keysym = xcb_key_symbols_get_keysym(MKeySyms,AKey,col);
//    xcb_keycode_t* keycode = xcb_key_symbols_get_keycode(MKeySyms,keysym);
    //MIP_Print("AKey %i AState %i keysym %i keycode %i\n",AKey,AState,keysym,keycode[0]);
    /*
    //SAT_Print("AKey %i : keysym %i keycode ",AKey, keysym);
    if (*keycode != XCB_NO_SYMBOL) {
      uint32_t i = 0;
      while (keycode[i] != XCB_NO_SYMBOL) {
        uint8_t c = keycode[i];
        //SAT_DPrint("%i '%c' ",c,c);
        i++;
      }
      //SAT_DPrint(" (%i)\n",i);
    }
    */
    /*
    char keysym_name[64];
    xkb_keysym_get_name(keysym, keysym_name, sizeof(keysym_name));
    SAT_DPrint("  keysym_name '%s'\n",keysym_name);
    */
    uint32_t ks = 0;
    switch (keysym) {
      case XKB_KEY_Return:      ks = SAT_KEY_ENTER;     break;
      case XKB_KEY_space:       ks = SAT_KEY_ESC;       break;
      case XKB_KEY_Home:        ks = SAT_KEY_HOME;      break;
      case XKB_KEY_End:         ks = SAT_KEY_END;       break;
      case XKB_KEY_leftarrow:   ks = SAT_KEY_LEFT;      break;
      case XKB_KEY_rightarrow:  ks = SAT_KEY_RIGHT;     break;
      case XKB_KEY_Delete:      ks = SAT_KEY_DELETE;    break;
      case XKB_KEY_BackSpace:   ks = SAT_KEY_BACKSPACE; break;
    }
//    free(keycode);
    return ks;
  }

  //----------

  uint32_t remapButton(uint32_t AButton, uint32_t AState) {
    //MIP_Print("AButton %i AState %i\n",AButton,AState);
    uint32_t b = AButton;
    return b;
  }

  //----------

  uint32_t remapState(uint32_t AState) {
    uint32_t s = SAT_KEY_NONE;
    if (AState & XCB_MOD_MASK_SHIFT)    s += SAT_KEY_SHIFT;
    if (AState & XCB_MOD_MASK_LOCK)     s += SAT_KEY_CAPS;
    if (AState & XCB_MOD_MASK_CONTROL)  s += SAT_KEY_CTRL;
    if (AState & XCB_MOD_MASK_1)        s += SAT_KEY_ALT;
    if (AState & XCB_MOD_MASK_5)        s += SAT_KEY_ALTGR;
    //if (AState & XCB_MOD_MASK_1) MIP_Print("1\n");
    //if (AState & XCB_MOD_MASK_2) MIP_Print("2\n");
    //if (AState & XCB_MOD_MASK_3) MIP_Print("3\n");
    //if (AState & XCB_MOD_MASK_4) MIP_Print("4\n");
    //if (AState & XCB_MOD_MASK_5) MIP_Print("5\n");
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

      /*
        "Well... personally, I never needed these fancy functions.
        The most complicated that I needed was "only handle the last
        MotionNotify event in the queue and ignore earlier ones". For that,
        I had a variable xcb_generic_event_t *motion = NULL; before my loop.
        Each new motion notify event was not handled, but saved in this
        function. Other events were handled immediately. After the loop,
        if there was a motion notify event, it was handled."
      */

      case XCB_CONFIGURE_NOTIFY: {

        xcb_configure_notify_event_t* configure_notify = (xcb_configure_notify_event_t*)AEvent;
        int16_t x = configure_notify->x;
        int16_t y = configure_notify->y;
        int16_t w = configure_notify->width;
        int16_t h = configure_notify->height;
        if ((x != MXpos) || (y != MYpos)) {
          on_window_move(x,y);
          MXpos = x;
          MYpos = y;
        }
        if ((w != MWidth) || (h != MHeight)) {
          on_window_resize(w,h);
          MWidth  = w;
          MHeight = h;
        }
        break;
      }

      // https://cairographics.org/cookbook/xcbsurface.c/
      // Avoid extra redraws by checking if this is the last expose event in the sequence
      //
      //while(expose->count != 0) {
      //  xcb_generic_event_t* e2 = xcb_wait_for_event(MConnection);
      //  xcb_expose_event_t* ex2 = (xcb_expose_event_t *)e2;
      //  RECT.combine( KODE_Rect( ex2->x, ex2->y, ex2->width, ex2->height ) );
      //}

      case XCB_EXPOSE: {

        MIsExposed = true;
        xcb_expose_event_t* expose = (xcb_expose_event_t *)AEvent;

        int16_t x = expose->x;
        int16_t y = expose->y;
        int16_t w = expose->width;
        int16_t h = expose->height;

        //paint(x,y,w,h);
        beginPaint();
        //if (MFillBackground) fill(x,y,w,h,MBackgroundColor);
        on_window_paint(x,y,w,h);
        endPaint();

        break;
      }

      case XCB_KEY_PRESS: {
        //if (!MWindowMapped) break;
        xcb_key_press_event_t* key_press = (xcb_key_press_event_t*)AEvent;
        uint8_t  k  = key_press->detail;
        uint16_t s  = key_press->state;
        uint32_t ts = key_press->time;
        k = remapKey(k,s);
        s = remapState(s);
        on_window_key_press(k,s,ts);
       break;
      }

      case XCB_KEY_RELEASE: {
        //if (!MWindowMapped) break;
        xcb_key_release_event_t* key_release = (xcb_key_release_event_t*)AEvent;
        uint8_t  k  = key_release->detail;
        uint16_t s  = key_release->state;
        uint32_t ts = key_release->time;
        k = remapKey(k,s);
        s = remapState(s);
        on_window_key_release(k,s,ts);
        break;
      }

      case XCB_BUTTON_PRESS: {
        //if (!MWindowMapped) break;
        xcb_button_press_event_t* button_press = (xcb_button_press_event_t*)AEvent;
        uint8_t  b  = button_press->detail;
        uint32_t s  = button_press->state;
        int32_t  x  = button_press->event_x;
        int32_t  y  = button_press->event_y;
        uint32_t ts = button_press->time;
        b = remapButton(b,s);
        s = remapState(s);
        on_window_mouse_click(b,s,x,y,ts);
        break;
      }

      case XCB_BUTTON_RELEASE: {
        //if (!MWindowMapped) break;
        xcb_button_release_event_t* button_release = (xcb_button_release_event_t*)AEvent;
        uint32_t b  = button_release->detail;
        uint32_t s  = button_release->state;
        int32_t  x  = button_release->event_x;
        int32_t  y  = button_release->event_y;
        uint32_t ts = button_release->time;
        b = remapButton(b,s);
        s = remapState(s);
        on_window_mouse_release(b,s,x,y,ts);
        break;
      }

      case XCB_MOTION_NOTIFY: {
        //if (!MWindowMapped) break;
        xcb_motion_notify_event_t* motion_notify = (xcb_motion_notify_event_t*)AEvent;
        //uint32_t  b = motion_notify->detail;
        uint32_t  s = motion_notify->state;
        int32_t   x = motion_notify->event_x;
        int32_t   y = motion_notify->event_y;
        uint32_t ts = motion_notify->time;
        s = remapState(s);
        on_window_mouse_move(s,x,y,ts);
        break;
      }

      case XCB_ENTER_NOTIFY: {
        if (!MIsMapped) break;
        xcb_enter_notify_event_t* enter_notify = (xcb_enter_notify_event_t*)AEvent;
        //uint32_t  m = enter_notify->mode;
        //uint32_t  s = enter_notify->state;
        int32_t   x = enter_notify->event_x;
        int32_t   y = enter_notify->event_y;
        uint32_t ts = enter_notify->time;
        on_window_enter(x,y,ts);
        break;
      }

      case XCB_LEAVE_NOTIFY: {
        if (!MIsMapped) break;
        xcb_leave_notify_event_t* leave_notify = (xcb_leave_notify_event_t*)AEvent;
        //uint32_t  m = leave_notify->mode;
        //uint32_t  s = leave_notify->state;
        int32_t   x = leave_notify->event_x;
        int32_t   y = leave_notify->event_y;
        uint32_t ts = leave_notify->time;
        on_window_leave(x,y,ts);
        break;
      }

      case XCB_CLIENT_MESSAGE: {
        xcb_client_message_event_t* client_message = (xcb_client_message_event_t*)AEvent;
        xcb_atom_t type = client_message->type;
        uint32_t data = client_message->data.data32[0];
        on_window_client_message(data);
        switch(data) {
          case SAT_WINDOW_THREAD_KILL:
            free(AEvent); // not malloc'ed
            return false; // we re finished
          default:
            break;
          /*
          case MIP_THREAD_ID_TIMER:
            on_window_timer();
            break;
          case MIP_THREAD_ID_IDLE:
            if (MMapped && MExposed) {
              on_window_idle();
            }
            break;
          default:
            on_window_clientMessage(data,nullptr);
            break;
          */
        }
        if (type == MWMProtocolsAtom) {
          if (data == MWMDeleteWindowAtom) {
            free(AEvent); // not malloc'ed
            return false; // we re finished
          }
        }

        break;
      } // switch

    }
    free(AEvent);
    return true; // we are still alive
  }

  //----------

  static
  void* xcb_event_thread_proc(void* AWindow) {
    //LOG.print("XCB Started event thread\n");
    //mip_xcb_event_thread_pid = getpid();
    SAT_X11Window* window = (SAT_X11Window*)AWindow;
    if (window) {
      //window->xcb_event_thread_start_callback(window);
      xcb_connection_t* connection = window->MConnection;
      xcb_flush(connection);
      while (window->MIsEventThreadActive) {
        xcb_generic_event_t* event = xcb_wait_for_event(connection);
        if (event) {

          if ((event->response_type & ~0x80) == XCB_CLIENT_MESSAGE) {
            xcb_client_message_event_t* client_message = (xcb_client_message_event_t*)event;
            xcb_atom_t type = client_message->type;
            uint32_t data = client_message->data.data32[0];
            if (data == SAT_WINDOW_THREAD_KILL) {
              //SAT_Print("KILL\n");
              return nullptr;
            }
          }

          // double-check (in case we have closed the window before processing
          // all events in queue
          //MIP_PRINT;

          if (window->MIsEventThreadActive) {

            if (!window->processEvent(event)) {
              //window->xcb_event_thread_stop_callback(window);
              //LOG.print("XCB Returning from event thread\n");
              return nullptr;
            }
          } // active
        } // event
      } // while
      //window->xcb_event_thread_stop_callback(window);
      //LOG.print("XCB Returning from event thread\n");
      return nullptr;
    } // window
    //LOG.print("XCB Returning from event thread\n");
    return nullptr;
  }

};

//----------------------------------------------------------------------
#endif





































#if 0

/*
  - remember to stop timer
*/

//----------------------------------------------------------------------

#include "base/mip.h"
#include "base/system/mip_timer.h"
#include "gui/base/mip_base_window.h"
#include "gui/xcb/mip_xcb.h"
#include "gui/xcb/mip_xcb_utils.h"
#include "gui/mip_paint_source.h"
#include "gui/mip_paint_target.h"

// xlib: X11, X11-xcb
// xcb: xcb, xcb-util, xcb-image, xcb-cursor, xcb-keysyms, xkbcommon

//----------------------------------------------------------------------

// https://tronche.com/gui/x/xlib/appendix/b/

const char* MIP_XCB_WM_CURSORS[MIP_CURSOR_COUNT] = {

  "left_ptr",             // MIP_CURSOR_DEFAULT
  "left_ptr",             // MIP_CURSOR_ARROW

  "sb_up_arrow",          // MIP_CURSOR_ARROW_UP
  "sb_down_arrow",        // MIP_CURSOR_ARROW_DOWN
  "sb_left_arrow",        // MIP_CURSOR_ARROW_LEFT
  "sb_right_arrow",       // MIP_CURSOR_ARROW_RIGHT

  "sb_v_double_arrow",    // MIP_CURSOR_ARROW_UP_DOWN
  "sb_h_double_arrow",    // MIP_CURSOR_ARROW_LEFT_RIGHT

  "top_left_corner",      // MIP_CURSOR_ARROW_TOP_LEFT
  "top_right_corner",     // MIP_CURSOR_ARROW_TOP_RIGHT
  "bottom_left_corner",   // MIP_CURSOR_ARROW_BOTTOM_LEFT
  "bottom_right_corner",  // MIP_CURSOR_ARROW_BOTTOM_RIGHT

  "left_side",            // MIP_CURSOR_ARROW_LEFT_SIDE
  "right_side",           // MIP_CURSOR_ARROW_RIGHT_SIDE
  "top_side",             // MIP_CURSOR_ARROW_TOP_SIDE
  "bottom_side",          // MIP_CURSOR_ARROW_BOTTOM_SIDE

  "fleur",                // MIP_CURSOR_MOVE
  "watch",                // MIP_CURSOR_WAIT
  "clock",                // MIP_CURSOR_ARROW_WAIT    !!!
  "hand2",                // MIP_CURSOR_HAND
  "hand1",                // MIP_CURSOR_FINGER
  "crosshair",            // MIP_CURSOR_CROSS
  "cross",                // MIP_CURSOR_CROSS2
  "pencil",               // MIP_CURSOR_PENCIL
  "plus",                 // MIP_CURSOR_PLUS
  "question_arrow",       // MIP_CURSOR_QUESTION
  "xterm",                // MIP_CURSOR_IBEAM

  "circle",               // MIP_CURSOR_ARROW_INVALID
  "crossed_circle",       // MIP_CURSOR_INVALID

  "pirate"                // MIP_CURSOR_X

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------


class MIP_XcbWindow
: public MIP_BaseWindow
, public MIP_PaintSource
, public MIP_PaintTarget
, public MIP_TimerListener {

//------------------------------
private:
//------------------------------

  xcb_window_t                MWindow                       = XCB_NONE;

  bool                        MWindowMapped                 = false;
  bool                        MWindowExposed                = false;

  const char*                 MDisplayName                  = nullptr;
  Display*                    MDisplay                      = nullptr;
  xcb_connection_t*           MConnection                   = nullptr;

  int                         MDefaultScreen                = 0;
  xcb_screen_t*               MScreen                       = nullptr;
  xcb_window_t                MScreenWindow                 = XCB_NONE;
  xcb_colormap_t              MScreenColormap               = XCB_NONE;
  xcb_visualid_t              MScreenVisual                 = XCB_NONE;
  xcb_drawable_t              MScreenDrawable               = XCB_NONE;
  xcb_gcontext_t              MScreenGC                     = XCB_NONE;

  xcb_atom_t                  MWMProtocolsAtom              = XCB_NONE;
  xcb_atom_t                  MWMDeleteWindowAtom           = XCB_NONE;

  pthread_t                   MEventThread                  = 0;

  xcb_pixmap_t                MEmptyPixmap                  = XCB_NONE;
  xcb_cursor_t                MHiddenCursor                 = XCB_NONE;
  xcb_cursor_t                MWindowCursor                 = XCB_NONE;

  xcb_key_symbols_t*          MKeySyms                      = nullptr;

  xcb_expose_event_t*         MExposeEvent                  = (xcb_expose_event_t*)MExposeEventBuffer;
  char*                       MExposeEventBuffer[32]        = {0};
  xcb_client_message_event_t* MClientMessageEvent           = (xcb_client_message_event_t*)MClientMessageEventBuffer;
  char*                       MClientMessageEventBuffer[32] = {0};

//------------------------------
protected:
//------------------------------

  bool                        MIsEmbedded                   = false;
  bool                        MIsCursorHidden               = false;
  bool                        MIsEventThreadActive          = false;

//bool                        MFillBackground               = false;
//uint32_t                    MBackgroundColor              = 0x808080;

  int32_t                     MScreenWidth                  = 0;
  int32_t                     MScreenHeight                 = 0;
  int32_t                     MScreenDepth                  = 0;

  int32_t                     MWindowWidth                  = 0;
  int32_t                     MWindowHeight                 = 0;

  int32_t                     MWindowXpos                   = 0;
  int32_t                     MWindowYpos                   = 0;

  const char*                 MWindowTitle                  = "MIP_XcbWindow";

  MIP_Timer*                  MGuiTimer                     = nullptr;

//------------------------------
public:
//------------------------------

  MIP_XcbWindow(uint32_t AWidth, uint32_t AHeight, intptr_t AParent=0)
  : MIP_BaseWindow(AWidth,AHeight,AParent) {

    initConnection(nullptr);
    initScreen();
    initScreenGC();
    initWindow(AWidth,AHeight);
    initMouseCursor();
    initKeyboard();

    if (AParent) {
      MIsEmbedded = true;
      removeDecorations();
    }
    else {
      MIsEmbedded = false;
      wantQuitEvents();
    }

    setTitle(MWindowTitle);

    MGuiTimer = new MIP_Timer(this);

  }

  //----------

  virtual ~MIP_XcbWindow() {
    if (MGuiTimer) {
        LOG.print("XCB Stopping gui timer (destructor)\n");
      MGuiTimer->stop();
      delete MGuiTimer;
    }
    cleanupKeyboard();
    cleanupMouseCursor();
    cleanupWindow();
    cleanupScreenGC();
    cleanupScreen();
    cleanupConnection();
  }

//------------------------------
public:
//------------------------------

  uint32_t getScreenWidth()   { return MScreenWidth; }
  uint32_t getScreenHeight()  { return MScreenHeight; }
  int32_t  getScreenDepth()   { return MScreenDepth; }

//------------------------------
public:
//------------------------------

  //void setFillBackground(bool AFill=true) {
  //  MFillBackground = AFill;
  //}

  //void setBackgroundColor(uint32_t AColor) {
  //  MBackgroundColor = AColor;
  //}

//------------------------------
private:
//------------------------------

//--------------------
// connection

  bool initConnection(const char* ADisplayName=nullptr) {
    MDisplayName = ADisplayName;

    //MConnection = xcb_connect(ADisplayName,&MDefaultScreen);
    XInitThreads(); // is this needed ???

    MDisplay = XOpenDisplay(ADisplayName);
    LOG.print("X Display: %p\n",MDisplay);

    MConnection = XGetXCBConnection(MDisplay);
    LOG.print("XCB connection: %p\n",MConnection);

    XSetEventQueueOwner(MDisplay,XCBOwnsEventQueue);

    MDefaultScreen = DefaultScreen(MDisplay); // ???
    LOG.print("X DefaultScreen: %i\n",MDefaultScreen);

    return true;
  }

  //----------

  void cleanupConnection() {
    //xcb_disconnect(MConnection);
    XSetEventQueueOwner(MDisplay,XlibOwnsEventQueue);
    XCloseDisplay(MDisplay);
    MConnection = nullptr;
  }

//--------------------
// screen

  bool initScreen() {
    uint32_t screen_num = MDefaultScreen;
    xcb_screen_iterator_t iter;
    iter = xcb_setup_roots_iterator(xcb_get_setup(MConnection));
    for (; iter.rem; --screen_num, xcb_screen_next(&iter)) {
      if (screen_num == 0) {

        MScreen = iter.data;
        LOG.print("XCB Screen: %p\n",MScreen);

        break;
      }
    }
    MScreenWidth    = MScreen->width_in_pixels;
    MScreenHeight   = MScreen->height_in_pixels;
    MScreenDepth    = MScreen->root_depth;
    MScreenWindow   = MScreen->root;
    MScreenColormap = MScreen->default_colormap;
    MScreenVisual   = MScreen->root_visual;
    MScreenDrawable = MScreen->root;

    LOG.print("  ScreenWidth:    %i\n",MScreenWidth);
    LOG.print("  ScreenHeight:   %i\n",MScreenHeight);
    LOG.print("  ScreenDepth:    %i\n",MScreenDepth);
    LOG.print("  ScreenWindow:   %i\n",MScreenWidth);
    LOG.print("  ScreenColormap: %i\n",MScreenColormap);
    LOG.print("  ScreenVisual:   %i\n",MScreenVisual);
    LOG.print("  ScreenDrawable: %i\n",MScreenDrawable);

    return true;
  }

  //----------

  void cleanupScreen() {
  }

//--------------------
// gc

  bool initScreenGC() {

    MScreenGC = xcb_generate_id(MConnection);
    LOG.print("XCB ScreenGC: %i\n",MScreenGC);

    xcb_drawable_t draw = MScreen->root;
    uint32_t mask = XCB_GC_FOREGROUND | XCB_GC_BACKGROUND;
    uint32_t values[2];
    values[0] = MScreen->black_pixel;
    values[1] = MScreen->white_pixel;
    xcb_create_gc(MConnection, MScreenGC, draw, mask, values);
    return true;
  }

  //----------

  void cleanupScreenGC() {
    xcb_free_gc(MConnection,MScreenGC);
  }

//--------------------
// window

  bool initWindow(uint32_t AWidth, uint32_t AHeight) {
    MWindowWidth = AWidth;
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
      //XCB_EVENT_MASK_RESIZE_REDIRECT  |
      XCB_EVENT_MASK_STRUCTURE_NOTIFY;
    uint32_t window_mask =
      XCB_CW_BACK_PIXMAP    |
      XCB_CW_BORDER_PIXEL   |
      XCB_CW_EVENT_MASK     |
      XCB_CW_COLORMAP;
    uint32_t window_mask_values[4] = {
      XCB_NONE,
      0,
      event_mask,
      MScreen->default_colormap
    };

    MWindow = xcb_generate_id(MConnection);
    LOG.print("XCB Window: %i\n",MWindow);

    xcb_create_window(
      MConnection,                    // connection
      XCB_COPY_FROM_PARENT,           // depth (same as root)
      MWindow,                        // window Id
      MScreen->root,                  //MWindowParent,  // parent window
      0, 0,                           // x, y
      AWidth, AHeight,                // width, height
      0,                              // border_width
      XCB_WINDOW_CLASS_INPUT_OUTPUT,  // class
      MScreen->root_visual,           // visual
      window_mask,
      window_mask_values
    );
    return true;
  }

  //----------

  void cleanupWindow() {
    xcb_destroy_window(MConnection,MWindow);
  }

//--------------------
//

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
    xcb_atom_t prop = mip_xcb_get_intern_atom(MConnection,"_MOTIF_WM_HINTS");
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

//------------------------------
public:
//------------------------------

  void on_timer_callback(MIP_Timer* ATimer) override {
    if (ATimer == MGuiTimer) {
      //MIP_PRINT;
      on_window_timer();
    }
  }

//------------------------------
public: // paint source, target
//------------------------------

  bool              srcIsWindow()       override { return true; }
  bool              srcIsDrawable()     override { return true; }
  int32_t           srcGetWidth()       override { return MWindowWidth; }
  int32_t           srcGetHeight()      override { return MWindowHeight; }
  int32_t           srcGetDepth()       override { return MScreenDepth; }

  xcb_connection_t* srcGetConnection()  override { return MConnection; }
  xcb_visualid_t    srcGetVisual()      override { return MScreenVisual; }
  xcb_drawable_t    srcGetDrawable()    override { return MWindow; }
  xcb_window_t      srcGetWindow()      override { return MWindow; }

  #ifdef MIP_USE_XLIB
  Display*          srcGetDisplay()     override { return MDisplay; }
  #endif

  //----------

  bool              tgtIsWindow()       override { return true; }
  bool              tgtIsDrawable()     override { return true; }
  int32_t           tgtGetWidth()       override { return MWindowWidth; }
  int32_t           tgtGetHeight()      override { return MWindowHeight; }
  int32_t           tgtGetDepth()       override { return MScreenDepth; }

  xcb_connection_t* tgtGetConnection()  override { return MConnection; }
  xcb_visualid_t    tgtGetVisual()      override { return MScreenVisual; }
  xcb_drawable_t    tgtGetDrawable()    override { return MWindow; }
  xcb_window_t      tgtGetWindow()      override { return MWindow; }

  #ifdef MIP_USE_XLIB
  Display*          tgtGetDisplay()     override { return MDisplay; }
  #endif

//------------------------------
public:
//------------------------------

  void setPos(uint32_t AXpos, uint32_t AYpos) override {
    uint32_t values[] = { AXpos, AYpos };
    xcb_configure_window(MConnection,MWindow,XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y,values);
    xcb_flush(MConnection);
  }

  //----------

  void getSize(uint32_t* AWidth, uint32_t* AHeight) override {
    *AWidth = MWindowWidth;
    *AHeight = MWindowHeight;
  }

  //----------

  void setSize(uint32_t AWidth, uint32_t AHeight) override {
    MWindowWidth = AWidth;
    MWindowHeight = AHeight;
    uint32_t values[] = { AWidth, AHeight };
    xcb_configure_window(MConnection,MWindow,XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT,values);
    xcb_flush(MConnection);
  }

  //----------

  void setTitle(const char* ATitle) override {
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

  void open() override {
    xcb_map_window(MConnection,MWindow);
    xcb_flush(MConnection);
    #ifdef MIP_XCB_WAIT_FOR_MAPNOTIFY
      waitForMapNotify();
    #endif
//    #ifdef MIP_GUI_IDLE_TIMER
//      startTimer(MIP_GUI_IDLE_TIMER_MS,MIP_GUI_IDLE_TIMER_ID);
//    #endif
  }

  //----------

  void close() override {
//    #ifdef MIP_GUI_IDLE_TIMER
//      stopTimer(MIP_GUI_IDLE_TIMER_ID);
//    #endif
    xcb_unmap_window(MConnection,MWindow);
    xcb_flush(MConnection);
  }

  //----------

  //void flush(void) override {
  //  xcb_flush(MConnection);
  //}

  //----------

  //void sync(void) override {
  //  xcb_aux_sync(MConnection);
  //}

  //----------

  /*
    while (TRUE) {
      if (XPending(display) || !pendingRedraws) {
        // if an event is pending, fetch it and process it
        // otherwise, we have neither events nor pending redraws, so we can
        // safely block on the event queue
        XNextEvent (display, &ev);
        if (isExposeEvent(&ev)) {
          pendingRedraws = TRUE;
      }
      else {
        processEvent(&ev);
      }
    }
    else {
      // we must have a pending redraw
      redraw();
      pendingRedraws = FALSE;
    }
  }
  */

  uint32_t eventLoop() override {
    xcb_generic_event_t* event = getEvent(true);
    while (event) {
      bool quit = !processEvent(event);
      if (quit) break;
      event = getEvent(true);
      //MIP_PRINT;
    }
    return 0;
  }

  //----------

  void startEventThread() override {
    //MIP_PRINT;
    MIsEventThreadActive = true;
    pthread_create(&MEventThread,nullptr,xcb_event_thread_proc,this);
  }

  //----------

  void stopEventThread() override {
    //MIP_PRINT;
    void* ret;
    MIsEventThreadActive = false;
    sendClientMessage(MIP_THREAD_ID_KILL,0);
    pthread_join(MEventThread,&ret);
  }

  //----------

  void startTimer(uint32_t ms, uintptr_t id) override {
    //MIP_PRINT;
    if (MGuiTimer) {
      LOG.print("XCB Starting gui timer\n");
      MGuiTimer->start(ms);
    }
  }

  //----------

  void stopTimer(uintptr_t id) override {
    //MIP_PRINT;
    if (MGuiTimer) {
      LOG.print("XCB Stopping gui timer\n");
      MGuiTimer->stop();
    }
  }

  //----------

  void sendClientMessage(uint32_t AData, uint32_t AType) override {
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

  void invalidate(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) override {
    //MIP_PRINT;
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

  void reparent(intptr_t AParent) override {
    if (AParent != 0) {
      MIsEmbedded = true;
      xcb_reparent_window(MConnection,MWindow,AParent,0,0);
      xcb_flush(MConnection);
    }
  }

  //----------

  void beginPaint() override {
  }

  //----------

  void endPaint() override {
    xcb_flush(MConnection);
  }

  //--------------------
  // mouse

  void setCursor(int32_t ACursor) override {
    setWMCursor(ACursor);
  }

  //----------

  void setCursorPos(int32_t AXpos, int32_t AYpos) override {
    xcb_warp_pointer(MConnection,XCB_NONE,MWindow,0,0,0,0,AXpos,AYpos);
    xcb_flush(MConnection);
  }

  //----------

  void hideCursor(void) override {
    if (!MIsCursorHidden) {
      setXcbCursor(MHiddenCursor);
      MIsCursorHidden = true;
    }
  }

  //----------

  void showCursor(void) override {
    if (MIsCursorHidden) {
      setXcbCursor(MWindowCursor);
      MIsCursorHidden = false;
    }
  }

  //----------

  /*
    xcb_grab_pointer:
    Actively grabs control of the pointer. Further pointer events are reported
    only to the grabbing client. Overrides any active pointer grab by this
    client.
  */

  void grabCursor(void) override {
    int32_t event_mask =
      XCB_EVENT_MASK_BUTTON_PRESS   |
      XCB_EVENT_MASK_BUTTON_RELEASE |
      XCB_EVENT_MASK_POINTER_MOTION |
      XCB_EVENT_MASK_FOCUS_CHANGE   |
      XCB_EVENT_MASK_ENTER_WINDOW   |
      XCB_EVENT_MASK_LEAVE_WINDOW;
    xcb_grab_pointer(
      MConnection,
      0,                                      // owner_events
      MWindow, /*rootWindow,*/                // grab_window
      event_mask,                             // event_mask
      XCB_GRAB_MODE_ASYNC,                    // pointer_mode
      XCB_GRAB_MODE_ASYNC,                    // keyboard_mode
      XCB_NONE, /*rootWindow,*/               // confine_to
      XCB_NONE,                               // cursor
      XCB_CURRENT_TIME                        // timestamp
    );
    xcb_flush(MConnection);
  }

  //----------

  void releaseCursor(void) override {
    xcb_ungrab_pointer(MConnection,XCB_CURRENT_TIME);
    xcb_flush(MConnection);
  }


  //--------------------
  // paint


  void fill(uint32_t AColor) override {
    fill(0,0,MWindowWidth,MWindowHeight,AColor);
  }

  //----------

  void fill(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight, uint32_t AColor) override {
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

  void blit(int32_t ADstX, int32_t ADstY, void* AData, uint32_t AStride, int32_t ASrcW, int32_t ASrcH) override {
  }

  //----------

  void blit(int32_t ADstX, int32_t ADstY, MIP_PaintSource* ASource, int32_t ASrcX, int32_t ASrcY, int32_t ASrcW, int32_t ASrcH) override {
  }

  //----------

  void stretch(int32_t ADstX, int32_t ADstY, int32_t ADstW, int32_t ADstH, MIP_PaintSource* ASource, int32_t ASrcX, int32_t ASrcY, int32_t ASrcW, int32_t ASrcH) override {
  }

  //----------

  void blend(int32_t ADstX, int32_t ADstY, int32_t ADstW, int32_t ADstH, MIP_PaintSource* ASource, int32_t ASrcX, int32_t ASrcY, int32_t ASrcW, int32_t ASrcH) override {
  }

//------------------------------
private: // xcb
//------------------------------

  xcb_atom_t mip_xcb_get_intern_atom(xcb_connection_t *conn, const char *name) {
    xcb_intern_atom_cookie_t cookie = xcb_intern_atom(conn ,0, strlen(name), name);
    xcb_intern_atom_reply_t *reply = xcb_intern_atom_reply(conn, cookie, NULL);
    //return !reply ? XCB_ATOM_STRING : reply->atom;
    return reply->atom;
  }

  //----------

  xcb_window_t getXcbParent() {
    xcb_query_tree_cookie_t cookie = xcb_query_tree(MConnection,MWindow);
    xcb_query_tree_reply_t* reply = xcb_query_tree_reply(MConnection,cookie,nullptr);
    xcb_window_t parent = reply->parent;
    free(reply);
    return parent;
  }

  //----------

  uint32_t getXcbChildCount() {
    xcb_query_tree_cookie_t cookie = xcb_query_tree(MConnection,MWindow);
    xcb_query_tree_reply_t* reply;
    if ((reply = xcb_query_tree_reply(MConnection,cookie,nullptr))) {
      int num_children = xcb_query_tree_children_length(reply);
      free(reply);
      return num_children;
    }
    return 0;
  }

  //----------

  uint32_t getXcbChild(uint32_t AIndex) {
    xcb_query_tree_cookie_t cookie = xcb_query_tree(MConnection,MWindow);
    xcb_query_tree_reply_t* reply;
    if ((reply = xcb_query_tree_reply(MConnection,cookie,nullptr))) {
      xcb_window_t *children = xcb_query_tree_children(reply);
      xcb_window_t child = children[AIndex];
      free(reply);
      return child;
    }
    return 0;
  }

//------------------------------
private: // mouse
//------------------------------

  void initMouseCursor() {
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
  }

  //----------

  void cleanupMouseCursor() {
    xcb_free_pixmap(MConnection, MEmptyPixmap);
    xcb_free_cursor(MConnection,MHiddenCursor);
    if (MWindowCursor != XCB_NONE) xcb_free_cursor(MConnection,MWindowCursor);
  }

  //----------

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
      const char* name = MIP_XCB_WM_CURSORS[ACursor];
      xcb_cursor_t cursor = xcb_cursor_load_cursor(ctx, name);
      if (cursor != XCB_CURSOR_NONE) {
        xcb_change_window_attributes(MConnection, MWindow, XCB_CW_CURSOR, &cursor);
      }
    }
    xcb_cursor_context_free(ctx);
    xcb_flush(MConnection);
  }

//------------------------------
private: // keyboard
//------------------------------

  void initKeyboard() {
    MKeySyms = xcb_key_symbols_alloc(MConnection);
  }

  //----------

  void cleanupKeyboard() {
    xcb_key_symbols_free(MKeySyms);
  }

//------------------------------
private: // remap
//------------------------------

  // https://github.com/etale-cohomology/xcb/blob/master/loop.c

  uint32_t remapKey(uint32_t AKey, uint32_t AState) {
    uint32_t k = 0;
    int col = 0;
    xcb_keysym_t keysym = xcb_key_symbols_get_keysym(MKeySyms,AKey,col);
    xcb_keycode_t* keycode = xcb_key_symbols_get_keycode(MKeySyms,keysym);
    //MIP_Print("AKey %i AState %i keysym %i keycode %i\n",AKey,AState,keysym,keycode[0]);
    k = keysym;
//    switch (keysym) {
//      case mip_xcb_key_enter:      k = MIP_KEY_ENTER;     break;
//      case mip_xcb_key_esc:        k = MIP_KEY_ESC;       break;
//      case mip_xcb_key_home:       k = MIP_KEY_HOME;      break;
//      case mip_xcb_key_end:        k = MIP_KEY_END;       break;
//      case mip_xcb_key_left:       k = MIP_KEY_LEFT;      break;
//      case mip_xcb_key_right:      k = MIP_KEY_RIGHT;     break;
//      case mip_xcb_key_delete:     k = MIP_KEY_DELETE;    break;
//      case mip_xcb_key_backspace:  k = MIP_KEY_BACKSPACE; break;
//    }
    free(keycode);
    return k;
  }

  //----------

  uint32_t remapButton(uint32_t AButton, uint32_t AState) {
    //MIP_Print("AButton %i AState %i\n",AButton,AState);
    uint32_t b = AButton;
    return b;
  }

  //----------

  uint32_t remapState(uint32_t AState) {
    uint32_t s = MIP_KEY_NONE;
    if (AState & XCB_MOD_MASK_SHIFT)    s += MIP_KEY_SHIFT;
    if (AState & XCB_MOD_MASK_LOCK)     s += MIP_KEY_CAPS;
    if (AState & XCB_MOD_MASK_CONTROL)  s += MIP_KEY_CTRL;
    if (AState & XCB_MOD_MASK_1)        s += MIP_KEY_ALT;
    if (AState & XCB_MOD_MASK_5)        s += MIP_KEY_ALTGR;
    //if (AState & XCB_MOD_MASK_1) MIP_Print("1\n");
    //if (AState & XCB_MOD_MASK_2) MIP_Print("2\n");
    //if (AState & XCB_MOD_MASK_3) MIP_Print("3\n");
    //if (AState & XCB_MOD_MASK_4) MIP_Print("4\n");
    //if (AState & XCB_MOD_MASK_5) MIP_Print("5\n");
    return s;
  }

//------------------------------
private: // events
//------------------------------

  void waitForMapNotify() {
    xcb_flush(MConnection);
    while (1) {
      xcb_generic_event_t* event;
      event = xcb_wait_for_event(MConnection);
      uint8_t e = event->response_type & ~0x80;
      free(event); // not malloc'ed
      if (e == XCB_MAP_NOTIFY) break;
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
    //if (event) {
    //  eventHandler(event);
    //  free(event); // not MALLOC'ed
    //}
    return event;
  }

  //----------

  // returns false if we received MWMDeleteWindowAtom
  // -> we are done (quit)
  // frees AEvent

  bool processEvent(xcb_generic_event_t* AEvent) {
    switch (AEvent->response_type & ~0x80) {

      case XCB_MAP_NOTIFY: {
        MWindowMapped = true;
        on_window_open();
        break;
      }

      case XCB_UNMAP_NOTIFY: {
        MWindowMapped = false;
        on_window_close();
        break;
      }

      /*
        Well... personally, I never needed these fancy functions.
        The most complicated that I needed was "only handle the last
        MotionNotify event in the queue and ignore earlier ones". For that,
        I had a variable xcb_generic_event_t *motion = NULL; before my loop.
        Each new motion notify event was not handled, but saved in this
        function. Other events were handled immediately. After the loop,
        if there was a motion notify event, it was handled.
      */

      case XCB_CONFIGURE_NOTIFY: {
        xcb_configure_notify_event_t* configure_notify = (xcb_configure_notify_event_t*)AEvent;

        int16_t x = configure_notify->x;
        int16_t y = configure_notify->y;
        int16_t w = configure_notify->width;
        int16_t h = configure_notify->height;

//        MIP_Print("w,h: %i,%i MWindowWidth/Height: %i,%i  \n",w,h,MWindowWidth,MWindowHeight);

        if ((x != MWindowXpos) || (y != MWindowYpos)) {
//          MIP_Print("move\n");
          on_window_move(x,y);
          MWindowXpos = x;
          MWindowYpos = y;
        }

        if ((w != MWindowWidth) || (h != MWindowHeight)) {
//          MIP_Print("resize\n");
          on_window_resize(w,h);
          MWindowWidth  = w;
          MWindowHeight = h;
        }

        break;
      }

      case XCB_EXPOSE: {
        MWindowExposed = true;
        xcb_expose_event_t* expose = (xcb_expose_event_t *)AEvent;

        // https://cairographics.org/cookbook/xcbsurface.c/
        // Avoid extra redraws by checking if this is the last expose event in the sequence
        //
        //while(expose->count != 0) {
        //  xcb_generic_event_t* e2 = xcb_wait_for_event(MConnection);
        //  xcb_expose_event_t* ex2 = (xcb_expose_event_t *)e2;
        //  RECT.combine( KODE_Rect( ex2->x, ex2->y, ex2->width, ex2->height ) );
        //}

        int16_t x = expose->x;
        int16_t y = expose->y;
        int16_t w = expose->width;
        int16_t h = expose->height;

        beginPaint();
        //if (MFillBackground) { fill(x,y,w,h,MBackgroundColor); }
        on_window_paint(x,y,w,h);
        endPaint();

        break;
      }

      case XCB_KEY_PRESS: {
        //if (!MWindowMapped) break;
        xcb_key_press_event_t* key_press = (xcb_key_press_event_t*)AEvent;
        uint8_t  k  = key_press->detail;
        uint16_t s  = key_press->state;
        uint32_t ts = key_press->time;
        k = remapKey(k,s);
        s = remapState(s);
        on_window_key_press(k,s,ts);
        break;
      }

      case XCB_KEY_RELEASE: {
        //if (!MWindowMapped) break;
        xcb_key_release_event_t* key_release = (xcb_key_release_event_t*)AEvent;
        uint8_t  k  = key_release->detail;
        uint16_t s  = key_release->state;
        uint32_t ts = key_release->time;
        k = remapKey(k,s);
        s = remapState(s);
        on_window_key_release(k,s,ts);
        break;
      }

      case XCB_BUTTON_PRESS: {
        //if (!MWindowMapped) break;
        xcb_button_press_event_t* button_press = (xcb_button_press_event_t*)AEvent;
        uint8_t  b  = button_press->detail;
        uint32_t s  = button_press->state;
        int32_t  x  = button_press->event_x;
        int32_t  y  = button_press->event_y;
        uint32_t ts = button_press->time;
        b = remapButton(b,s);
        s = remapState(s);
        on_window_mouse_click(b,s,x,y,ts);
        break;
      }

      case XCB_BUTTON_RELEASE: {
        //if (!MWindowMapped) break;
        xcb_button_release_event_t* button_release = (xcb_button_release_event_t*)AEvent;
        uint32_t b  = button_release->detail;
        uint32_t s  = button_release->state;
        int32_t  x  = button_release->event_x;
        int32_t  y  = button_release->event_y;
        uint32_t ts = button_release->time;
        b = remapButton(b,s);
        s = remapState(s);
        on_window_mouse_release(b,s,x,y,ts);
        break;
      }

      case XCB_MOTION_NOTIFY: {
        //if (!MWindowMapped) break;
        xcb_motion_notify_event_t* motion_notify = (xcb_motion_notify_event_t*)AEvent;
        //uint32_t  b = motion_notify->detail;
        uint32_t  s = motion_notify->state;
        int32_t   x = motion_notify->event_x;
        int32_t   y = motion_notify->event_y;
        uint32_t ts = motion_notify->time;
        s = remapState(s);
        on_window_mouse_move(s,x,y,ts);
        break;
      }

      case XCB_ENTER_NOTIFY: {
        if (!MWindowMapped) break;
        xcb_enter_notify_event_t* enter_notify = (xcb_enter_notify_event_t*)AEvent;
        //uint32_t  m = enter_notify->mode;
        //uint32_t  s = enter_notify->state;
        int32_t   x = enter_notify->event_x;
        int32_t   y = enter_notify->event_y;
        uint32_t ts = enter_notify->time;
        on_window_enter(x,y,ts);
        break;
      }

      case XCB_LEAVE_NOTIFY: {
        if (!MWindowMapped) break;
        xcb_leave_notify_event_t* leave_notify = (xcb_leave_notify_event_t*)AEvent;
        //uint32_t  m = leave_notify->mode;
        //uint32_t  s = leave_notify->state;
        int32_t   x = leave_notify->event_x;
        int32_t   y = leave_notify->event_y;
        uint32_t ts = leave_notify->time;
        on_window_leave(x,y,ts);
        break;
      }

      case XCB_CLIENT_MESSAGE: {
        xcb_client_message_event_t* client_message = (xcb_client_message_event_t*)AEvent;
        xcb_atom_t type = client_message->type;
        uint32_t data = client_message->data.data32[0];
        on_window_client_message(data);
        switch(data) {
          /*
          case MIP_THREAD_ID_TIMER:
            on_window_timer();
            break;
          case MIP_THREAD_ID_IDLE:
            if (MWindowMapped && MWindowExposed) {
              on_window_idle();
            }
            break;
          default:
            on_window_clientMessage(data,nullptr);
            break;
          */
          case MIP_THREAD_ID_KILL:
            free(AEvent); // not malloc'ed
            return false; // we re finished
          default:
            break;
        }
        if (type == MWMProtocolsAtom) {
          if (data == MWMDeleteWindowAtom) {
            free(AEvent); // not malloc'ed
            return false; // we re finished
          }
        }

        break;
      } // switch

    }
    free(AEvent);
    return true; // we are still alive
  }

//------------------------------
//private: // event thread
//------------------------------

  /*
    while (TRUE) {
      if (XPending(display) || !pendingRedraws) {
        // if an event is pending, fetch it and process it
        // otherwise, we have neither events nor pending redraws, so we can
        // safely block on the event queue
        XNextEvent (display, &ev);
        if (isExposeEvent(&ev)) {
          pendingRedraws = TRUE;
      }
      else {
        processEvent(&ev);
      }
    }
    else {
      // we must have a pending redraw
      redraw();
      pendingRedraws = FALSE;
    }
  }
  */

  static
  void* xcb_event_thread_proc(void* AWindow) {
    LOG.print("XCB Started event thread\n");
    //mip_xcb_event_thread_pid = getpid();
    MIP_XcbWindow* window = (MIP_XcbWindow*)AWindow;
    if (window) {
//      window->xcb_event_thread_start_callback(window);
      xcb_connection_t* connection = window->MConnection;
      xcb_flush(connection);
      while (window->MIsEventThreadActive) {
        xcb_generic_event_t* event = xcb_wait_for_event(connection);
        if (event) {
          //MIP_Print("event!\n");
          // double-check (in case we have closed the window before processing
          // all events in queue
          //MIP_PRINT;
          if (window->MIsEventThreadActive) {
            //MIP_PRINT;
            if (!window->processEvent(event)) {
//              window->xcb_event_thread_stop_callback(window);
              LOG.print("XCB Returning from event thread\n");
              return nullptr;
            }
          } // active
        } // event
      } // while
//      window->xcb_event_thread_stop_callback(window);
      LOG.print("XCB Returning from event thread\n");
      return nullptr;
    } // window
    LOG.print("XCB Returning from event thread\n");
    return nullptr;
  }

  //----------

};

#endif // 0



