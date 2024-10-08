#ifndef sat_x11_window_included
#define sat_x11_window_included
//----------------------------------------------------------------------

// #define SAT_X11_WAIT_FOR_MAPNOTIFY
// #define SAT_WINDOW_THREAD_KILL 666

#include "sat.h"
#include "base/system/sat_time.h"
#include "gui/sat_gui_base.h"
#include "gui/x11/sat_x11.h"
#include "gui/x11/sat_x11_utils.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_X11Window
: public SAT_BaseWindow
, public SAT_PainterOwner
, public SAT_PaintTarget
, public SAT_RendererOwner
, public SAT_RenderTarget {

//------------------------------
private:
//------------------------------

  // connection
  Display*                    MDisplay                      = nullptr;
  xcb_connection_t*           MConnection                   = nullptr;
  int                         MDefaultScreen                = 0;
  
  // screen
  xcb_screen_t*               MScreen                       = nullptr;
  uint32_t                    MScreenWidth                  = 0;
  uint32_t                    MScreenHeight                 = 0;
  uint32_t                    MScreenDepth                  = 0;
  xcb_visualid_t              MScreenVisual                 = XCB_NONE;  
  
  // screen GC
  xcb_gcontext_t              MScreenGC                     = XCB_NONE;
  
  // window
  xcb_window_t                MWindow                       = XCB_NONE;
  int32_t                     MWindowXpos                   = 0;
  int32_t                     MWindowYpos                   = 0;
  uint32_t                    MWindowWidth                  = 0;
  uint32_t                    MWindowHeight                 = 0;
  
  // mouse
  xcb_pixmap_t                MEmptyPixmap                  = XCB_NONE;
  xcb_cursor_t                MHiddenCursor                 = XCB_NONE;
  xcb_cursor_t                MWindowCursor                 = XCB_NONE;
  
  // keyboard
  xcb_key_symbols_t*          MKeySyms                      = nullptr;

//------------------------------
private:
//------------------------------

  // wantQuitEvents()
  xcb_atom_t                  MWMProtocolsAtom              = XCB_NONE;
  xcb_atom_t                  MWMDeleteWindowAtom           = XCB_NONE;
  
  // invalidate()
  xcb_expose_event_t*         MExposeEvent                  = (xcb_expose_event_t*)MExposeEventBuffer;
  char*                       MExposeEventBuffer[32]        = {0};
  
  // sendClientMessage()
  xcb_client_message_event_t* MClientMessageEvent           = (xcb_client_message_event_t*)MClientMessageEventBuffer;
  char*                       MClientMessageEventBuffer[32] = {0};

  // event thread
  pthread_t                   MEventThread                  = 0;

  bool                        MIsEmbedded                   = false;
  bool                        MIsMapped                     = false;
  bool                        MIsExposed                    = false;
  bool                        MIsCursorHidden               = false;
  sat_atomic_bool_t           MIsEventThreadActive          { false };

  double                      MPrevTime                     = 0.0;

//------------------------------
public:
//------------------------------

  SAT_X11Window(uint32_t AWidth, uint32_t AHeight, intptr_t AParent=0)
  : SAT_BaseWindow(AWidth,AHeight,AParent) {

    // setupConnection();

    MDisplay = XOpenDisplay(nullptr);
    //MConnection = xcb_connect(ADisplayName,&MDefaultScreen);
    MConnection = XGetXCBConnection(MDisplay);
    XSetEventQueueOwner(MDisplay,XCBOwnsEventQueue);
    MDefaultScreen = DefaultScreen(MDisplay); // ???

    // setupScreen();

    uint32_t screen_num = MDefaultScreen;
    xcb_screen_iterator_t iter;
    iter = xcb_setup_roots_iterator(xcb_get_setup(MConnection));
    for (; iter.rem; --screen_num, xcb_screen_next(&iter)) {
      if (screen_num == 0) {
        MScreen = iter.data;
        break;
      }
    }
    MScreenWidth  = MScreen->width_in_pixels;
    MScreenHeight = MScreen->height_in_pixels;
    MScreenDepth  = MScreen->root_depth;
    MScreenVisual = MScreen->root_visual;

    // setupScreenGC();

    MScreenGC = xcb_generate_id(MConnection);
    xcb_drawable_t draw = MScreen->root;
    uint32_t mask = XCB_GC_FOREGROUND | XCB_GC_BACKGROUND;
    uint32_t values[2];
    values[0] = MScreen->black_pixel;
    values[1] = MScreen->white_pixel;
    xcb_create_gc(MConnection, MScreenGC, draw, mask, values);    

    // setupWindow(AWidth,AHeight,AParent);

    MWindowXpos   = 0;
    MWindowYpos   = 0;
    MWindowWidth  = AWidth;
    MWindowHeight = AHeight;
    //SAT_PRINT("MWindowWidth/Height %i,%i\n",MWindowWidth,MWindowHeight);
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
      //XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY|
      XCB_EVENT_MASK_STRUCTURE_NOTIFY;
    uint32_t window_mask =
      XCB_CW_BACK_PIXMAP    |
      XCB_CW_BORDER_PIXEL   |
      XCB_CW_EVENT_MASK     |
      XCB_CW_COLORMAP;
    uint32_t window_mask_values[4] = {
      XCB_NONE,                           // should this be XCB_BACK_PIXMAP_NONE ?
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

    // setupMouse();

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

    // setupKeyboard();

    MKeySyms = xcb_key_symbols_alloc(MConnection);    

    if (AParent) {
      MIsEmbedded = true;
      removeDecorations();
    }
    else {
      MIsEmbedded = false;
      wantQuitEvents();
    }

    //

    MPrevTime = SAT_GetTime();

  }

  //----------

  virtual ~SAT_X11Window() {
    if (MIsMapped) hide();
    // cleanupKeyboard();
    xcb_key_symbols_free(MKeySyms);
    // cleanupMouse();
    xcb_free_pixmap(MConnection, MEmptyPixmap);
    xcb_free_cursor(MConnection,MHiddenCursor);
    if (MWindowCursor != XCB_NONE) xcb_free_cursor(MConnection,MWindowCursor);    
    // cleanupWindow();
    xcb_destroy_window(MConnection,MWindow);
    // cleanupScreenGC();
    xcb_free_gc(MConnection,MScreenGC);
    // cleanupConnection();
    XSetEventQueueOwner(MDisplay,XlibOwnsEventQueue);
    XCloseDisplay(MDisplay);
    MDisplay = nullptr;
    MConnection = nullptr;
  }

//------------------------------
public:
//------------------------------

  xcb_connection_t* on_PainterOwner_getXcbConnection()  final { return MConnection; }
  xcb_visualid_t    on_PainterOwner_getXcbVisual()      final { return MScreenVisual; }
  xcb_drawable_t    on_PaintTarget_getXcbDrawable()     final { return MWindow; }

  Display*          on_RendererOwner_getX11Display()    final { return MDisplay; }
  xcb_drawable_t    on_RendererOwner_getXcbDrawable()   final { return MWindow; }
  xcb_drawable_t    on_RenderTarget_getXcbDrawable()    final { return MWindow; }

//------------------------------
public:
//------------------------------

  uint32_t  getWidth()  final { return MWindowWidth; }
  uint32_t  getHeight() final { return MWindowHeight; }
  uint32_t  getDepth()  final { return MScreenDepth; }

  xcb_window_t getXcbWindow() { return MWindow; }

  //----------

  void show() final {
    xcb_map_window(MConnection,MWindow);
    xcb_flush(MConnection);
    #ifdef SAT_X11_WAIT_FOR_MAPNOTIFY
      waitForMapNotify();
      MIsMapped = true;
      on_Window_show();
    #endif
    if (MIsEmbedded) startEventThread();
  }
  
  //----------

  void hide() final {
    //SAT_TRACE;
    on_Window_hide(); // check if not called multiple times!
    if (MIsEmbedded) stopEventThread();
    xcb_unmap_window(MConnection,MWindow);
    xcb_flush(MConnection);
    MIsMapped = false;
    MIsExposed = false;    
  }
  
  //----------

  void setPos(int32_t AXpos, int32_t AYpos) final {
    MWindowXpos = AXpos;
    MWindowYpos = AYpos;
    uint32_t values[] = { (uint32_t)AXpos, (uint32_t)AYpos };
    xcb_configure_window(MConnection,MWindow,XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y,values);
    xcb_flush(MConnection);
  }

  //----------

  void setSize(uint32_t AWidth, uint32_t AHeight) final {
    //SAT_TRACE;
    MWindowWidth = AWidth;
    MWindowHeight = AHeight;
    uint32_t values[] = { AWidth, AHeight };
    xcb_configure_window(MConnection,MWindow,XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT,values);
    xcb_flush(MConnection);
  }
  
  //----------

  void setTitle(const char* ATitle) final {
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

  void showMouseCursor() final {
    if (MIsCursorHidden) {
      setXcbCursor(MWindowCursor);
      MIsCursorHidden = false;
    }
  }
  
  //----------

  void hideMouseCursor() final {
    if (!MIsCursorHidden) {
      setXcbCursor(MHiddenCursor);
      MIsCursorHidden = true;
    }
  }

  //----------

  void grabMouseCursor() final {
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

  void releaseMouseCursor() override {
    xcb_ungrab_pointer(MConnection,XCB_CURRENT_TIME);
    xcb_flush(MConnection);
  }
  
  //----------

  void setMouseCursorPos(int32_t AXpos, int32_t AYpos) final {
    xcb_warp_pointer(MConnection,XCB_NONE,MWindow,0,0,0,0,AXpos,AYpos);
    xcb_flush(MConnection);
  }
  
  //----------

  void setMouseCursorShape(int32_t ACursor) final {
    setWMCursor(ACursor);
  }
  
  //----------

  void reparent(intptr_t AParent) final {
    if (AParent != 0) {
      MIsEmbedded = true;
      xcb_reparent_window(MConnection,MWindow,AParent,0,0);
      xcb_flush(MConnection);
    }    
  }

  //----------

  void invalidate(int32_t AXpos, int32_t AYpos, uint32_t AWidth, uint32_t AHeight) {
    // x11 doesn't like negative coords? (uint16_t)
    //AXpos = SAT_ClampI(AXpos,0,MWindowWidth);
    //AYpos = SAT_ClampI(AYpos,0,MWindowHeight);
    if (AXpos < 0) AXpos = 0;
    if (AYpos < 0) AYpos = 0;
    if (AXpos > (int)MWindowWidth)  AXpos = MWindowWidth;
    if (AYpos > (int)MWindowHeight) AYpos = MWindowHeight;
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

  void eventLoop() final {
    xcb_generic_event_t* event = getEvent(true);
    while (event) {
    //do {
      bool result = processEvent(event);
      if (result == false) {
        //SAT_PRINT("processEvent returned false, returning..\n");
        //free(event); // not malloc'ed
        //break;
        return;// 1;
      }
      //uint32_t e = event->response_type & ~0x80;
      //if (e == XCB_CLIENT_MESSAGE) {
      //  xcb_client_message_event_t* client_message = (xcb_client_message_event_t*)event;
      //  xcb_atom_t type = client_message->type;
      //  uint32_t data = client_message->data.data32[0];
      //  //if (type == MWMProtocolsAtom) {
      //    if (data == MWMDeleteWindowAtom) {
      //      //free(event); // not malloc'ed
      //      //MQuitEventLoop = true;
      //      break;
      //      return 2;
      //    }
      //  //}
      //}
      event = getEvent(true);
    //}
    } while (event);
    //return 0;
  }
  
  //----------

  void startEventThread() final {
    MIsEventThreadActive = true;
    pthread_create(&MEventThread,nullptr,xcb_event_thread_callback,this);
  }
  
  //----------

  /*
    the event thread is blocking when waiting for new events to arrive.
    to wake it up, we send a 'client message'.. when the thread receives this event,
    it will (should) kill itself..
  */

  /*
    had an editor close crash.. (couldn't reproduce it, needs investigation)
    had the templates audio_plugin + instrument + note_effect templates open at the same time.
    closed bitwig
    crashed in SAT_X11Window::stopEventThread(), called from from SAT_X11Window::close()
    called after/from two SAT_Editor::~SAT_Editor() calls in a row..
    threads closing, vs atomic bools?
    if the window is closing, will the SAT_WINDOW_THREAD_KILL client message reach it?
  */  

  void stopEventThread() final {
    void* ret;
    MIsEventThreadActive = false;
    sendClientMessage(SAT_WINDOW_THREAD_KILL,0);
    //xcb_flush(MConnection);
    pthread_join(MEventThread,&ret);
  }

//------------------------------
private:
//------------------------------

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
      const char* name = SAT_X11_WM_CURSORS[ACursor];
      xcb_cursor_t cursor = xcb_cursor_load_cursor(ctx, name);
      if (cursor != XCB_CURSOR_NONE) {
        xcb_change_window_attributes(MConnection, MWindow, XCB_CW_CURSOR, &cursor);
      }
    }
    xcb_cursor_context_free(ctx);
    xcb_flush(MConnection);
  }

  //----------
  
  uint32_t remapButton(uint32_t AButton, uint32_t AState) {
    //SAT_PRINT("AButton %i AState %i\n",AButton,AState);
    uint32_t b = AButton;
    return b;
  }

  //----------

  uint32_t remapState(uint32_t AState) {
    uint32_t s = 0;//SAT_STATE_NONE;
    if (AState & XCB_MOD_MASK_SHIFT)    s += SAT_STATE_SHIFT;
    if (AState & XCB_MOD_MASK_LOCK)     s += SAT_STATE_CAPS;
    if (AState & XCB_MOD_MASK_CONTROL)  s += SAT_STATE_CTRL;
    if (AState & XCB_MOD_MASK_1)        s += SAT_STATE_ALT;
    if (AState & XCB_MOD_MASK_5)        s += SAT_STATE_ALTGR;
    //if (AState & XCB_MOD_MASK_1) SAT_PRINT("1\n");
    //if (AState & XCB_MOD_MASK_2) SAT_PRINT("2\n");
    //if (AState & XCB_MOD_MASK_3) SAT_PRINT("3\n");
    //if (AState & XCB_MOD_MASK_4) SAT_PRINT("4\n");
    //if (AState & XCB_MOD_MASK_5) SAT_PRINT("5\n");
    return s;
  }

  //----------

  /*
  if (sym == XKB_KEY_KP_Space) codepoint = XKB_KEY_space & 0x7f;
  else codepoint = sym & 0x7f;
  */    
  
  // https://github.com/etale-cohomology/xcb/blob/master/loop.c
  // https://stackoverflow.com/questions/43004441/how-to-get-unicode-input-from-xcb-without-further-ado
  // https://opensource.apple.com/source/X11libs/X11libs-40/xcb-util/xcb-util-0.3.3/keysyms/keysyms.c.auto.html
  // https://dreamswork.github.io/qt4/qxcbkeyboard_8cpp_source.html
  // https://github.com/paul/awesome/blob/master/keyresolv.c

  /*
    https://tronche.com/gui/x/xlib/input/keyboard-encoding.html  

    AKey = key_press->detail (xcb_keycode_t)
    AState = key_press->state
  */

  uint32_t remapKey(uint32_t AKey, uint32_t AState/*, char* AChar*/) {
    int col = 0;
    xcb_keysym_t keysym = xcb_key_symbols_get_keysym(MKeySyms,AKey,col);
    
    // xcb_keycode_t* keycode  = xcb_key_symbols_get_keycode(MKeySyms,keysym);
    // free(keycode);
    // char buffer[256] = {0};
    // int32_t num = xkb_keysym_to_utf8(keysym,buffer,255);
    // char c = buffer[0];

    //SAT_DPRINT("%c\n",buffer[0]);
    //SAT_PRINT("AKey %i AState %i keysym %i c %i\n",AKey,AState,keysym,c);
    // "Alt_L, Return, etc"
    // num = xkb_keysym_get_name(keysym,buffer,255);
    // if (num > 0) {
    //   //for (int32_t i=0; i<num; i++) {  }
    //   SAT_DPRINT("%s\n",buffer);
    // }

    uint32_t ks = keysym;
    switch (keysym) {
      case XKB_KEY_Return:      ks = SAT_KEY_ENTER;     break;
      case XKB_KEY_KP_Space:    ks = SAT_KEY_SPACE;     break;
      case XKB_KEY_Home:        ks = SAT_KEY_HOME;      break;
      case XKB_KEY_End:         ks = SAT_KEY_END;       break;
      case XKB_KEY_Left:        ks = SAT_KEY_LEFT;      break;
      case XKB_KEY_Right:       ks = SAT_KEY_RIGHT;     break;
      case XKB_KEY_Delete:      ks = SAT_KEY_DELETE;    break;
      case XKB_KEY_BackSpace:   ks = SAT_KEY_BACKSPACE; break;
      case XKB_KEY_Escape:      ks = SAT_KEY_ESC;       break;
    }
    // *AChar = c;
    return ks;
    //return AKey;
  }

//------------------------------
private:
//------------------------------

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
  // (we didn't allocate AEvent (it's coming from the event handler - xcb_wait_for_event)
  // so we call the 'non-intercepted' free, to avoid pestering our memleak detector)

  bool processEvent(xcb_generic_event_t* AEvent) {
    switch (AEvent->response_type & ~0x80) {

      case XCB_MAP_NOTIFY: {
        //SAT_PRINT("XCB_MAP_NOTIFY\n");
        MIsMapped = true;
        on_Window_show();
        break;
      }

      case XCB_UNMAP_NOTIFY: {
        //SAT_PRINT("XCB_UNMAP_NOTIFY\n");
        MIsMapped = false;
        on_Window_hide();
        break;
      }

      /*
        (where is this from?)
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
        int16_t x  = configure_notify->x;
        int16_t y  = configure_notify->y;
        uint16_t w = configure_notify->width;
        uint16_t h = configure_notify->height;

        //SAT_PRINT("XCB_CONFIGURE_NOTIFY x %i y %i w %i h %i (MWindowWidth/Height %i,%i)\n",x,y,w,h,MWindowWidth,MWindowHeight);

        if ((x != MWindowXpos) || (y != MWindowYpos)) {
          on_Window_move(x,y);
          MWindowXpos = x;
          MWindowYpos = y;
        }

        if ((w != MWindowWidth) || (h != MWindowHeight)) {
          on_Window_resize(w,h);
          MWindowWidth  = w;
          MWindowHeight = h;
        }

        break;
      }

      case XCB_EXPOSE: {
        xcb_expose_event_t* expose = (xcb_expose_event_t *)AEvent;
        uint16_t x = expose->x;
        uint16_t y = expose->y;
        uint16_t w = expose->width;
        uint16_t h = expose->height;
        //SAT_PRINT("XCB_EXPOSE x %i y %i w %i h %i\n",x,y,w,h);
          //// https://cairographics.org/cookbook/xcbsurface.c/
          //// Avoid extra redraws by checking if this is the last expose event in the sequence
          //while(expose->count != 0) {
          //  xcb_generic_event_t* e2 = xcb_wait_for_event(MConnection);
          //  xcb_expose_event_t* ex2 = (xcb_expose_event_t *)e2;
          //  RECT.combine( SAT_Rect( ex2->x, ex2->y, ex2->width, ex2->height ) );
          //}
        //MIsPainting = true;
        on_Window_paint(x,y,w,h);
        xcb_flush(MConnection);
        //MIsPainting = false;
        MIsExposed = true;
        break;
      }

      case XCB_KEY_PRESS: {
        //if (!MWindowMapped) break;
        xcb_key_press_event_t* key_press = (xcb_key_press_event_t*)AEvent;

        uint8_t  k  = key_press->detail;
        uint16_t s  = key_press->state;
        uint32_t ts = key_press->time;
        //char c = 0;
        uint32_t ks = remapKey(k,s);//,&c);
        //SAT_PRINT("k %i s %i ts %i ks %i\n",k,s,ts,ks);

        s = remapState(s);
        on_Window_keyPress(ks,0/*c*/,s,ts);
       break;
      }

      case XCB_KEY_RELEASE: {
        //if (!MWindowMapped) break;
        xcb_key_release_event_t* key_release = (xcb_key_release_event_t*)AEvent;
        uint8_t  k  = key_release->detail;
        uint16_t s  = key_release->state;
        uint32_t ts = key_release->time;
        //char c = 0;
        uint32_t ks = remapKey(k,s);//,&c);
        s = remapState(s);
        //on_Window_keyRelease(k,ks,s,ts);
        on_Window_keyRelease(ks,0/*c*/,s,ts);
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
        on_Window_mouseClick(x,y,b,s,ts);
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
        on_Window_mouseRelease(x,y,b,s,ts);
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
        on_Window_mouseMove(x,y,s,ts);
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
        on_Window_mouseEnter(x,y,ts);
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
        on_Window_mouseLeave(x,y,ts);
        break;
      }

      case XCB_CLIENT_MESSAGE: {
        xcb_client_message_event_t* client_message = (xcb_client_message_event_t*)AEvent;
        xcb_atom_t type = client_message->type;
        uint32_t data = client_message->data.data32[0];

        if (data == SAT_WINDOW_THREAD_KILL) {
          //SAT_PRINT("SAT_WINDOW_THREAD_KILL\n");
          ::free(AEvent);
          return false;
        }
        else if (data == SAT_WINDOW_THREAD_TIMER) {
          //SAT_PRINT("SAT_WINDOW_THREAD_TIMER\n");
          double time = SAT_GetTime();
          double delta = time - MPrevTime;
          MPrevTime = time;
          on_Window_timer(delta);
          ::free(AEvent);
          return true;
        }

        if (type == MWMProtocolsAtom) { // why was this commented out?
          if (data == MWMDeleteWindowAtom) {
            //SAT_PRINT("MWMDeleteWindowAtom\n");
            ::free(AEvent);
            return false;
          }
        }
        on_Window_clientMessage(data);
        break;
      } // client message

    } // switch

    ::free(AEvent);
    return true; // still alive
    
  }

//------------------------------
private:
//------------------------------

  static
  void* xcb_event_thread_callback(void* AWindow) {
    //LOG.print("XCB Started event thread\n");
    //sat_xcb_event_thread_pid = getpid();
    SAT_X11Window* window = (SAT_X11Window*)AWindow;
    if (window) {
      //window->xcb_event_thread_start_callback(window);
      xcb_connection_t* connection = window->MConnection;
      xcb_flush(connection);
      while (window->MIsEventThreadActive) {

        // Returns the next event or error from the server, or returns null in the event of an I/O error.
        // Blocks until either an event or error arrive, or an I/O error occurs.

        xcb_generic_event_t* event = xcb_wait_for_event(connection);
        if (event) {

          // when you get an event with response_type == 0, cast the pointer to xcb_generic_error_t*.
          if (event->response_type == 0) {
            xcb_generic_error_t* error = (xcb_generic_error_t*)event;
            SAT_PRINT("xcb_wait_for_event: received X11 error %d\n", error->error_code);
            free(event);
            continue;
          }

          /*
          if ((event->response_type & ~0x80) == XCB_CLIENT_MESSAGE) {
            xcb_client_message_event_t* client_message = (xcb_client_message_event_t*)event;
            xcb_atom_t type = client_message->type;
            uint32_t data = client_message->data.data32[0];
            //if (data == SAT_WINDOW_THREAD_KILL) {
            //  SAT_PRINT("KILL\n");
            //  return nullptr;
            //}
          }
          */

          /*
          if ((event->response_type & ~0x80) == XCB_CLIENT_MESSAGE) {
            xcb_client_message_event_t* client_message = (xcb_client_message_event_t*)event;
            xcb_atom_t type = client_message->type;
            uint32_t data = client_message->data.data32[0];
            SAT_PRINT("type %i data %i\n",type,data);
            if (data == SAT_WINDOW_THREAD_TIMER) {
              SAT_PRINT("TIMER\n");
              //return nullptr;
            }
          }
          */

          // double-check (in case we have closed the window before processing all events in queue?
          // ??? is this needed, or did i put it here for some debugging purposes?
          // ugh..
          
          if (window->MIsEventThreadActive) {
            
            if (!window->processEvent(event)) {
              //SAT_PRINT("window->processEvent returned false\n");
              //window->xcb_event_thread_stop_callback(window);
              //LOG.print("XCB Returning from event thread\n");
              return nullptr;
            }
            
          } // active #2

        } // event
      } // while active #1
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
