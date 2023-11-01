#ifndef sat_x11_window_included
#define sat_x11_window_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/base/sat_base_window.h"
#include "gui/base/sat_painter_owner.h"
#include "gui/base/sat_renderer_owner.h"
#include "gui/x11/sat_x11.h"
#include "gui/x11/sat_x11_utils.h"
#include "gui/sat_painter.h"
#include "gui/sat_renderer.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_X11Window
: public SAT_BaseWindow
, public SAT_SurfaceOwner
, public SAT_RendererOwner
, public SAT_PainterOwner {

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
  sat_atomic_bool_t           MIsEventThreadActive          {false};
  sat_atomic_bool_t           MIsPainting                   {false};
//sat_atomic_bool_t           MIsClosing                    {false};

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
  bool                        MIsCursorHidden               = false;          // TODO: sat_atomicbool_t ?

  SAT_Renderer*               MRenderer                     = nullptr;
  SAT_Painter*                MPainter                      = nullptr;

//------------------------------
//protected:
//------------------------------

   int32_t                    MWindowXpos                   = 0;
   int32_t                    MWindowYpos                   = 0;
  uint32_t                    MWindowWidth                  = 0;
  uint32_t                    MWindowHeight                 = 0;

//uint32_t                    MPreviousWindowWidth          = 0;
//uint32_t                    MPreviousWindowHeight         = 0;

//------------------------------
public:
//------------------------------

  SAT_X11Window(uint32_t AWidth, uint32_t AHeight, intptr_t AParent=0)
  : SAT_BaseWindow(AWidth,AHeight,AParent) {

    // x server connection

    //MDisplay = ARenderer->getX11Display();
    MDisplay = XOpenDisplay(nullptr);
    SAT_Print("MDisplay: %p\n",MDisplay);

    //MConnection = xcb_connect(ADisplayName,&MDefaultScreen);
    MConnection = XGetXCBConnection(MDisplay);
    SAT_Print("MConnection: %p\n",MConnection);

    XSetEventQueueOwner(MDisplay,XCBOwnsEventQueue);
    MDefaultScreen = DefaultScreen(MDisplay); // ???
    SAT_Print("MDefaultScreen: %i\n",MDefaultScreen);

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
    //SAT_Print("MWindowWidth/Height %i,%i\n",MWindowWidth,MWindowHeight);

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

    //

    MRenderer = new SAT_Renderer(this);
    MPainter = new SAT_Painter(this);

  } 

  //----------

  virtual ~SAT_X11Window() {

    delete MPainter;
    delete MRenderer;

    if (MIsMapped) close();
    // keyboard
    xcb_key_symbols_free(MKeySyms);
    // mouse
    xcb_free_pixmap(MConnection, MEmptyPixmap);
    xcb_free_cursor(MConnection,MHiddenCursor);
    if (MWindowCursor != XCB_NONE) xcb_free_cursor(MConnection,MWindowCursor);
    // window
    xcb_destroy_window(MConnection,MWindow);
    // screen gc
    xcb_free_gc(MConnection,MScreenGC);
    // screen
    // connection
    XSetEventQueueOwner(MDisplay,XlibOwnsEventQueue);
    XCloseDisplay(MDisplay);
    MDisplay = nullptr;
    MConnection = nullptr;
  }

//------------------------------
public:
//------------------------------

  SAT_BaseRenderer* getRenderer() {
    return MRenderer;
  }

  SAT_BasePainter* getPainter() {
    return MPainter;
  }

//------------------------------
public:
//------------------------------

  uint32_t getWindowType() override {
    return SAT_WINDOW_TYPE_X11;
  }

  const char* getWindowTypeName() override {
    return SAT_WINDOW_TYPE_NAME_X11;
  }

  uint32_t getScreenWidth() override {
    return MScreenWidth;
  }

  uint32_t getScreenHeight() override {
    return MScreenHeight;
  }

  uint32_t getScreenDepth() override {
    return MScreenDepth;
  }
  
  uint32_t getWidth() override {
    return MWindowWidth;
  }

  uint32_t getHeight() override {
    return MWindowWidth;
  }

//------------------------------
public: // SAT_RendererOwner
//------------------------------

  Display* on_rendererOwner_getX11Display() override {
    return MDisplay;
  }

  xcb_drawable_t on_rendererOwner_getDrawable() override {
    return MWindow;
  }

//------------------------------
public: // SAT_PainterOwner
//------------------------------

//------------------------------
public: // SAT_SurfaceOwner
//------------------------------

//------------------------------
public:
//------------------------------


//------------------------------
public: // SAT_BaseWindow
//------------------------------

  void open() override {
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

  void close() override {
    if (MIsEmbedded) stopEventThread();
    xcb_unmap_window(MConnection,MWindow);
    xcb_flush(MConnection);
    MIsMapped = false;
    MIsExposed = false;
  }

  //----------
  
  void setPos(int32_t AXpos, int32_t AYpos) override {
    MWindowXpos = AXpos;
    MWindowYpos = AYpos;
    uint32_t values[] = { AXpos, AYpos };
    xcb_configure_window(MConnection,MWindow,XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y,values);
    xcb_flush(MConnection);
  }

  //----------
  
  void setSize(int32_t AWidth, int32_t AHeight) override {
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
  
  void setMousePos(int32_t AXpos, int32_t AYpos) override {
    xcb_warp_pointer(MConnection,XCB_NONE,MWindow,0,0,0,0,AXpos,AYpos);
    xcb_flush(MConnection);
  }

  //----------
  
  void setMouseCursor(int32_t ACursor) override {
    setWMCursor(ACursor);
  }

  //----------
  
  void hideMouse() override {
    if (!MIsCursorHidden) {
      setXcbCursor(MHiddenCursor);
      MIsCursorHidden = true;
    }
  }

  //----------
  
  void showMouse() override {
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

  void grabMouse() override {
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

  void releaseMouse() override {
    xcb_ungrab_pointer(MConnection,XCB_CURRENT_TIME);
    xcb_flush(MConnection);
  }

  //----------

  //  void eventLoop() override {
  //    MQuitEventLoop = false;
  //    xcb_flush(MConnection);
  //    xcb_generic_event_t* event;// = xcb_wait_for_event(MConnection);
  //    //while (event) {
  //    while ((event = xcb_wait_for_event(MConnection))) {
  //      uint32_t e = event->response_type & ~0x80;
  //      if (e == XCB_CLIENT_MESSAGE) {
  //        xcb_client_message_event_t* client_message = (xcb_client_message_event_t*)event;
  //        xcb_atom_t  type = client_message->type;
  //        uint32_t      data = client_message->data.data32[0];
  //        if (type == MWMProtocolsAtom) {
  //          if (data == MWMDeleteWindowAtom) {
  //            free(event); // not malloc'ed
  //            //MQuitEventLoop = true;
  //            break;
  //          }
  //        }
  //      }
  //      eventHandler(event);
  //      free(event); // not malloc'ed
  //      if (MQuitEventLoop) break;
  //      //event = xcb_wait_for_event(MConnection);
  //    }
  //  }
  
  uint32_t eventLoop() override {
    xcb_generic_event_t* event = getEvent(true);
    while (event) {
    //do {
      bool result = processEvent(event);
      if (result == false) {
        //SAT_Print("processEvent returned false, returning..\n");
        //free(event); // not malloc'ed
        //break;
        return 1;
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
    return 0;
  }

  //----------
  
  void startEventThread() override {
    MIsEventThreadActive = true;
    pthread_create(&MEventThread,nullptr,xcb_event_thread_callback,this);
  }

  //----------

  /*
    the event thread is blocking when waiting for new events to arrive.
    to wake it up, we send a 'client message'.. when the thread receives this event,
    it will (should) kill itself..
  */
  
  void stopEventThread() override {
    void* ret;
    MIsEventThreadActive = false;
    sendClientMessage(SAT_WINDOW_THREAD_KILL,0);
    //xcb_flush(MConnection);
    pthread_join(MEventThread,&ret);
  }

  //----------
  
  void startTimer(uint32_t AMSInterval, bool AOneShot) override {
  }

  //----------
  
  void stopTimer() override {
  }

  //----------
  
  void setParent(intptr_t AParent) override {
    if (AParent != 0) {
      MIsEmbedded = true;
      xcb_reparent_window(MConnection,MWindow,AParent,0,0);
      xcb_flush(MConnection);
    }
  }

  //----------
  
  void invalidate(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) override {
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

  // void fill(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight, uint32_t AColor) override {
  //   // set color
  //   uint32_t mask = XCB_GC_FOREGROUND;
  //   uint32_t values[1];
  //   values[0] = AColor;
  //   xcb_change_gc(MConnection,MScreenGC,mask,values);
  //   // fill rectangle
  //   xcb_rectangle_t rectangles[] = {{
  //     (int16_t)AXpos,     //0,
  //     (int16_t)AYpos,     //0,
  //     (uint16_t)AWidth,   //MWindowWidth,
  //     (uint16_t)AHeight,  //MWindowHeight
  //   }};
  //   xcb_poly_fill_rectangle(MConnection,MWindow,MScreenGC,1,rectangles);
  //   xcb_flush(MConnection);
  // }

  //----------

  // void fill(uint32_t AColor) override {
  //   fill(0,0,MWindowWidth,MWindowHeight,AColor);
  // }

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
    //SAT_Print("AButton %i AState %i\n",AButton,AState);
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

  //  xcb_keycode_t* keycode = xcb_key_symbols_get_keycode(MKeySyms,keysym);
  //  SAT_Print("AKey %i AState %i keysym %i keycode %i\n",AKey,AState,keysym,keycode[0]);
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
    //uint32_t ks = 0;
    //free(keycode);
    
  /*
  if (sym == XKB_KEY_KP_Space) codepoint = XKB_KEY_space & 0x7f;
  else codepoint = sym & 0x7f;
  */    
  
  //

  // https://github.com/etale-cohomology/xcb/blob/master/loop.c

  uint32_t remapKey(uint32_t AKey, uint32_t AState) {
    int col = 0;
    xcb_keysym_t keysym = xcb_key_symbols_get_keysym(MKeySyms,AKey,col);
    // keycode = AKey ??
    //xcb_keycode_t* keycode  = xcb_key_symbols_get_keycode(MKeySyms,keysym);
    //free(keycode);
    //char buffer[256] = {0};
    //uint32_t num = xkb_keysym_to_utf8(keysym,buffer,255);
    //for (uint32_t i=0; i<num; i++) {
    //  SAT_DPrint("%i\n",buffer[i]);
    //}
    uint32_t ks = keysym;
//    switch (keysym) {
//      case XKB_KEY_Return:      ks = SAT_KEY_ENTER;     break;
//      case XKB_KEY_KP_Space:    ks = SAT_KEY_ESC;       break;
//      case XKB_KEY_Home:        ks = SAT_KEY_HOME;      break;
//      case XKB_KEY_End:         ks = SAT_KEY_END;       break;
//      case XKB_KEY_Left:        ks = SAT_KEY_LEFT;      break;
//      case XKB_KEY_Right:       ks = SAT_KEY_RIGHT;     break;
//      case XKB_KEY_Delete:      ks = SAT_KEY_DELETE;    break;
//      case XKB_KEY_BackSpace:   ks = SAT_KEY_BACKSPACE; break;
//    }
    return ks;
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
  // (we didn't allocate AEvent (it's coming from the event handler - xcb_wait_for_event)
  // so we call the 'non-intercepted' free, to avoid pestering our memleak detector)

  bool processEvent(xcb_generic_event_t* AEvent) {
    switch (AEvent->response_type & ~0x80) {

      case XCB_MAP_NOTIFY: {
        //SAT_Print("XCB_MAP_NOTIFY\n");
        MIsMapped = true;
        on_window_open();
        break;
      }

      case XCB_UNMAP_NOTIFY: {
        //SAT_Print("XCB_UNMAP_NOTIFY\n");
        MIsMapped = false;
        on_window_close();
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
        //SAT_Print("XCB_CONFIGURE_NOTIFY x %i y %i w %i h %i\n",x,y,w,h);
        if ((x != MWindowXpos) || (y != MWindowYpos)) {
          //SAT_Print("new MWindowXpos/Ypos %i,%i\n",MWindowXpos,MWindowYpos);
          on_window_move(x,y);
          MWindowXpos = x;
          MWindowYpos = y;
        }
        if ((w != MWindowWidth) || (h != MWindowHeight)) {
          //SAT_Print("new MWindowWidth/Height %i,%i\n",MWindowWidth,MWindowHeight);
          on_window_resize(w,h);
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
          //// https://cairographics.org/cookbook/xcbsurface.c/
          //// Avoid extra redraws by checking if this is the last expose event in the sequence
          //while(expose->count != 0) {
          //  xcb_generic_event_t* e2 = xcb_wait_for_event(MConnection);
          //  xcb_expose_event_t* ex2 = (xcb_expose_event_t *)e2;
          //  RECT.combine( SAT_Rect( ex2->x, ex2->y, ex2->width, ex2->height ) );
          //}
        //MIsPainting = true;

        MRenderer->beginRendering(0,0,w,h);
        on_window_paint(x,y,w,h);
        MRenderer->endRendering();

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
        uint32_t ks = remapKey(k,s);
        s = remapState(s);
        on_window_key_press(k,ks,s,ts);
       break;
      }

      case XCB_KEY_RELEASE: {
        //if (!MWindowMapped) break;
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
        //if (!MWindowMapped) break;
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
        //if (!MWindowMapped) break;
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
        //if (!MWindowMapped) break;
        xcb_motion_notify_event_t* motion_notify = (xcb_motion_notify_event_t*)AEvent;
        //uint32_t  b = motion_notify->detail;
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
        //uint32_t  m = enter_notify->mode;
        //uint32_t  s = enter_notify->state;
        int32_t   x = enter_notify->event_x;
        int32_t   y = enter_notify->event_y;
        uint32_t ts = enter_notify->time;
        on_window_mouse_enter(x,y,ts);
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
        on_window_mouse_leave(x,y,ts);
        break;
      }

      case XCB_CLIENT_MESSAGE: {
        xcb_client_message_event_t* client_message = (xcb_client_message_event_t*)AEvent;
        xcb_atom_t type = client_message->type;
        uint32_t data = client_message->data.data32[0];
        if (data == SAT_WINDOW_THREAD_KILL) {
          SAT_Print("SAT_WINDOW_THREAD_KILL\n");
          ::free(AEvent);
          return false;
        }
        if (type == MWMProtocolsAtom) { // why was this commented out?
          if (data == MWMDeleteWindowAtom) {
            SAT_Print("MWMDeleteWindowAtom\n");
            ::free(AEvent);
            return false;
          }
        }
        on_window_client_message(data);
        break;
      } // client message

    } // switch

    ::free(AEvent);
    return true; // still alive
    
  }

  //----------

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
        xcb_generic_event_t* event = xcb_wait_for_event(connection);
        if (event) {

          //if ((event->response_type & ~0x80) == XCB_CLIENT_MESSAGE) {
          //  xcb_client_message_event_t* client_message = (xcb_client_message_event_t*)event;
          //  xcb_atom_t type = client_message->type;
          //  uint32_t data = client_message->data.data32[0];
          //  if (data == SAT_WINDOW_THREAD_KILL) {
          //    SAT_Print("KILL\n");
          //    return nullptr;
          //  }
          //}

          // double-check (in case we have closed the window before processing all events in queue?
          // ??? is this needed, or did i pit it here for some debugging purposes?
          // ugh..
          if (window->MIsEventThreadActive) {
            
            if (!window->processEvent(event)) {
              //SAT_Print("window->processEvent returned false\n");
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
