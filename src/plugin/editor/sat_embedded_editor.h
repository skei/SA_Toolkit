
#include "sat.h"
//#include "plugin/editor/sat_editor_listener.h"
//#include "plugin/editor/sat_base_editor.h"
#include "gui/sat_gui_base.h"
#include "plugin/sat_plugin_base.h"
#include "gui/sat_window.h"


class SAT_EmbeddedEditor
: public SAT_BaseEditor
, public SAT_WindowListener {

//------------------------------
private:
//------------------------------

  SAT_EditorListener* MListener       = nullptr;

  SAT_Window*         MWindow         = nullptr;
  const char*         MTitle          = "";
  bool                MCanResize      = true;
  bool                MWindowIsOpen   = false;

  #ifdef SAT_GUI_WIN32
    void*             MParent         = nullptr;
    void*             MTransient      = nullptr;
  #endif

  #ifdef SAT_GUI_X11
    uint32_t          MParent         = 0;
    uint32_t          MTransient      = 0;
  #endif

  // from setInitialEditorSize(640,480,1.0,false);
  uint32_t            MInitialWidth   = 512;
  uint32_t            MInitialHeight  = 512;
  double              MInitialScale   = 1.0;
  bool                MProportional   = false;

  // runtime
  uint32_t            MWidth          = 512;
  uint32_t            MHeight         = 512;
//double              MScale          = 1.0;

//------------------------------
public:
//------------------------------

  SAT_EmbeddedEditor(SAT_EditorListener* AListener, uint32_t AWidth, uint32_t AHeight, double AScale=1.0, bool AProportional=false)
  : SAT_BaseEditor(AListener,AWidth,AHeight,AScale,AProportional) {
    MListener       = AListener;
    MInitialWidth   = AWidth;
    MInitialHeight  = AHeight;
    MInitialScale   = AScale;
    MProportional   = AProportional;
  //MScale          = AScale;
    MWidth          = AWidth * AScale;
    MHeight         = AHeight * AScale;


  }

  //----------

  virtual ~SAT_EmbeddedEditor() {
  }

//------------------------------
public:
//------------------------------

  void on_WindowListener_update(SAT_Widget* AWidget, uint32_t AIndex, uint32_t AMode) override {
    //SAT_TRACE;
    SAT_Parameter* param = (SAT_Parameter*)AWidget->getParameter(AIndex);
    if (param) {
      uint32_t index = param->getIndex();
      sat_param_t value = AWidget->getValue(AIndex);
      MListener->on_EditorListener_update(index,value);
    }
  }

  //----------

  // void on_windowListener_redraw(SAT_Widget* AWidget, uint32_t AMode=SAT_WIDGET_REDRAW_SELF) override {
  // }

  //----------

  void on_WindowListener_timer(double ADelta) override {
    if (MListener) MListener->on_EditorListener_timer(ADelta);
  }

//------------------------------
public:
//------------------------------

  SAT_Window* getWindow() {
    return MWindow;
  }

//------------------------------
public:
//------------------------------

  // called by SAT_Plugin.gui_create

  virtual bool setupOverlay() {
    if (MWindow) return MWindow->setupOverlay();
    return false;
  }

  //----------

  virtual bool connect(SAT_Widget* AWidget, SAT_Parameter* AParameter, uint32_t AIndex=0) {
    AWidget->setParameter(AParameter,AIndex);
    AParameter->setWidget(AWidget);
    return true;
  }

//------------------------------
public:
//------------------------------

  // called from SAT_Plugin.initEditorParameterValues
  // (which is called from SAT_Plugin.gui_create)
  // thread: main

  void initParameterValue(SAT_Parameter* AParameter) override {
    if (MWindow) {
      if (MWindow->isClosing()) return;
      SAT_Widget* widget = (SAT_Widget*)AParameter->getWidget();
      if (widget) {
        sat_param_t value = AParameter->getValue();
        widget->setValue(value);
      }
    }
  }

  //----------

  // called from
  //   SAT_Queues.flushParamFromHostToGui (timer)
  //   SAT_Plugin.on_processorOwner_updateParamFromHostToGui -> queue (audio)
  // (do_Widget_redraw doesn't redraw directly, ultimately sends invalidate event)

  // [AUDIO THREAD]

  void updateParameterFromHost(SAT_Parameter* AParameter, double AValue) override {
    if (MWindow && MWindowIsOpen) {
      if (MWindow->isClosing()) return;
      SAT_Widget* widget = (SAT_Widget*)AParameter->getWidget();
      if (widget) {
        sat_param_t value = AParameter->getValue();
        widget->setValue(value);
        widget->do_Widget_redraw(widget,0,SAT_WIDGET_REDRAW_SELF);
      }
    }
  }

  //----------

  // [AUDIO THREAD]

  void updateModulationFromHost(SAT_Parameter* AParameter, double AValue) override {
    if (MWindow && MWindowIsOpen) {
      if (MWindow->isClosing()) return;
      SAT_Widget* widget = (SAT_Widget*)AParameter->getWidget();
      if (widget) {
        sat_param_t value = AParameter->getValue();
        widget->setModulation(value);
        widget->do_Widget_redraw(widget,0,SAT_WIDGET_REDRAW_SELF);
      }
    }
  }

//------------------------------
public: // clap.gui
//------------------------------

  bool isApiSupported(const char *api, bool is_floating) override {
    if (is_floating) return false;
    #if defined(SAT_GUI_WAYLAND)
      if (strcmp(api,CLAP_WINDOW_API_WAYLAND) == 0) return true;
    #elif defined(SAT_GUI_WIN32)
      if (strcmp(api,CLAP_WINDOW_API_WIN32) == 0) return true;
    #elif defined(SAT_GUI_X11)
      if (strcmp(api,CLAP_WINDOW_API_X11) == 0) return true;
    #endif
    return false;
  }

  //----------

  bool getPreferredApi(const char **api, bool *is_floating) override {
    *is_floating = false;
    #if defined(SAT_GUI_WAYLAND)
      *api = CLAP_WINDOW_API_WAYLAND;
      return true;
    #elif defined(SAT_GUI_WIN32)
      *api = CLAP_WINDOW_API_WIN32;
      return true;
    #elif defined(SAT_GUI_X11)
      *api = CLAP_WINDOW_API_X11;
      return true;
    #endif
    return false;
  }

  //----------

  bool create(const char *api, bool is_floating) override {
    if (is_floating == true) return false;
    #ifdef SAT_LINUX
      if (strcmp(api,CLAP_WINDOW_API_X11) != 0) return false;
      //SAT_PRINT("MWidth %i MHeight %i\n",MWidth,MHeight);
      // ask plugin to create window.. if not, create it ourselves

      if (MListener) MWindow = MListener->on_EditorListener_createWindow(MWidth,MHeight,this);
      else MWindow = new SAT_Window(MWidth,MHeight,this);
      //MWindow->setListener(this); // not needed?

    #endif
    #ifdef SAT_WIN32
      //if (strcmp(api,CLAP_WINDOW_API_WIN32) != 0) return false;
      if (strcmp(api,CLAP_WINDOW_API_WIN32) != 0) return false;
      // ask plugin to create window.. if not, create it ourselves
      if (MListener) MWindow = MListener->on_EditorListener_createWindow(MWidth,MHeight,this);
      else MWindow = new SAT_Window(MWidth,MHeight,this);
      //MWindow->setListener(this);
    #endif
    return true;
  }
  
  //----------

  void destroy() override {
    if (MWindow) {
      MWindow->hide();
      //delete MWindow;
      if (MListener) MListener->on_EditorListener_deleteWindow(MWindow);
      else delete MWindow;
      MWindow = nullptr;
    }
  }

  //----------

  bool setScale(double scale) override {
    return true;
  }

  //----------

  bool getSize(uint32_t *width, uint32_t *height) override {
    *width = MWidth;
    *height = MHeight;
    return true;
  }

  //----------

  bool canResize() override {
    return MCanResize;
  }

  //----------

  bool getResizeHints(clap_gui_resize_hints_t *hints) override {
    hints->can_resize_horizontally = true;
    hints->can_resize_vertically = true;
    hints->preserve_aspect_ratio = MProportional;
    hints->aspect_ratio_width = MInitialWidth;
    hints->aspect_ratio_height = MInitialHeight;
    return true;
  }

  //----------

  bool adjustSize(uint32_t *width, uint32_t *height) override {
    return true;
  }

  //----------

  bool setSize(uint32_t width, uint32_t height) override {

    // in case a host tries to send us negative coords (but typecasted as uint)..
    if (width > 8192) width = 1;
    if (height > 8192) height = 1;

    MWidth = width;
    MHeight = height;
    if (MWindow /*&& MWindowIsOpen*/) {
      MWindow->setSize(width,height);           // actually resizes the window (will it re-send resize event) (sets MWindowWidth/Height)
      MWindow->on_Window_resize(width,height);  // tell the rest of the system about the new size (calc scale, root.on_Widget_resize, realign)
    }
    return true;
  }

  //----------

  bool setParent(const clap_window_t *window) override {
    #ifdef SAT_GUI_WIN32
      MParent = window->win32;
    #endif
    #ifdef SAT_GUI_X11
      MParent = window->x11;
    #endif
    if (MWindow) MWindow->reparent((intptr_t)MParent);
    // do we need to resize MWindow.renderer/painter? buffer?
    return true;
  }

  //----------

  bool setTransient(const clap_window_t *window) override {
    #ifdef SAT_GUI_WIN32
      MTransient = window->win32;
    #endif
    #ifdef SAT_GUI_X11
      MTransient = window->x11;
    #endif
    return true;
  }

  //----------

  void suggestTitle(const char *title) override {
    MTitle = title;
    if (MWindow) MWindow->setTitle(title);
  }

  //----------

  bool show() override {
    if (MWindow) {
      MWindow->on_Window_show();
      MWindow->show();
    }
    MWindowIsOpen = true;
    return true;
  }

  //----------

  bool hide() override {
    if (MWindow) {
      MWindow->on_Window_hide();
      MWindow->hide();
    }
    MWindowIsOpen = false;
    return true;
  }
  
};

