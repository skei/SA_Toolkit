#ifndef sat_embedded_editor_included
#define sat_embedded_editor_included
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/editor/sat_editor_listener.h"
#include "plugin/editor/sat_base_editor.h"
#include "gui/window/sat_window_listener.h"
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
    MWidth          = AWidth;
    MHeight         = AHeight;
    MInitialWidth   = AWidth;
    MInitialHeight  = AHeight;
    MInitialScale   = AScale;
    MProportional   = AProportional;
  //MScale          = AScale;
  }

  //----------

  virtual ~SAT_EmbeddedEditor() {
  }

//------------------------------
public:
//------------------------------

  void on_windowListener_update(SAT_Widget* AWidget, uint32_t AMode=SAT_WIDGET_UPDATE_VALUE) override {
    //SAT_PRINT("%s\n",AWidget->getName());
    SAT_Parameter* param = (SAT_Parameter*)AWidget->getParameter();
    if (param) {
      uint32_t index = param->getIndex();
      sat_param_t value = AWidget->getValue();
      MListener->on_editorListener_update(index,value);
    }
  }

  //----------

  // void on_windowListener_redraw(SAT_Widget* AWidget, uint32_t AMode=SAT_WIDGET_REDRAW_PARAM) override {
  //   SAT_TRACE;
  // }

  //----------

  void on_windowListener_timer(SAT_Timer* ATimer, double ADelta) override {
    //SAT_TRACE;
    if (MListener) MListener->on_editorListener_timer(ATimer,ADelta);
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

  virtual bool connect(SAT_Widget* AWidget, SAT_Parameter* AParameter, uint32_t AIndex=0) {
    AWidget->setParameter(AParameter,AIndex);
    AParameter->setWidget(AWidget);
    return true;
  }

  //----------

  // called from SAT_Plugin.setEditorParameterValues
  // (which is called from SAT_Plugin.gui_create)
  // thread: main

  void setParameterValue(SAT_Parameter* AParameter) override {
    SAT_Widget* widget = (SAT_Widget*)AParameter->getWidget();
    if (widget) {
      sat_param_t value = AParameter->getValue();
      widget->setValue(value);
    }
  }

  //----------

  // called from
  //   SAT_Queues.flushParamFromHostToGui (todo: timer call this)
  //   SAT_Plugin.on_processorOwner_updateParamFromHostToGui
  // (do_widget_redraw doesn't redraw directly, sends invalidate event)

  void updateParameterFromHost(SAT_Parameter* AParameter, double AValue) override {
    //SAT_PRINT("%p = %.3f\n",AParameter,AValue);
    if (MWindow) {
      SAT_Widget* widget = (SAT_Widget*)AParameter->getWidget();
      if (widget) {
        sat_param_t value = AParameter->getValue();
        widget->setValue(value);
        widget->do_widget_redraw(widget,SAT_WIDGET_REDRAW_PARAM);
      }
    }
  }

  //----------

  void updateModulationFromHost(SAT_Parameter* AParameter, double AValue) override {
    if (MWindow) {
      SAT_Widget* widget = (SAT_Widget*)AParameter->getWidget();
      if (widget) {
        sat_param_t value = AParameter->getValue();
        widget->setModulation(value);
        widget->do_widget_redraw(widget,SAT_WIDGET_REDRAW_MOD);
      }
    }
  }

//------------------------------
public: // clap.gui
//------------------------------

  bool isApiSupported(const char *api, bool is_floating) override {
    //SAT_PRINT("api %s floating %i\n",api,is_floating);
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
    //SAT_PRINT("\n");
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
    //SAT_PRINT("api %s floating %i\n",api,is_floating);
    if (is_floating == true) return false;
    #ifdef SAT_LINUX
      if (strcmp(api,CLAP_WINDOW_API_X11) != 0) return false;
      // ask plugin to create window
      // if not, create it ourselves
      if (MListener) MWindow = MListener->on_editorListener_createWindow(MWidth,MHeight);
      else MWindow = new SAT_Window(MWidth,MHeight);
      MWindow->setListener(this);
    #endif
    #ifdef SAT_WIN32
      if (strcmp(api,CLAP_WINDOW_API_WIN32) != 0) return false;
    #endif
    // setupEditor(MEditor);
    return true;
  }
  
  //----------

  void destroy() override {
    //SAT_PRINT("\n");
    if (MWindow) {
      MWindow->hide();
      //hide();
      if (MListener) MListener->on_editorListener_deleteWindow(MWindow);
      else delete MWindow;
      MWindow = nullptr;
    }
  }

  //----------

  bool setScale(double scale) override {
    //SAT_PRINT("AScale %.3f\n",scale);
    // MScale = scale;
    // MScale *= scale;
    return true;
  }

  //----------

  bool getSize(uint32_t *width, uint32_t *height) override {
    //SAT_PRINT("\n");
    *width = MWidth;
    *height = MHeight;
    return true;
  }

  //----------

  bool canResize() override {
    //SAT_PRINT("\n");
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
    //SAT_PRINT("*width %i, *height %i\n",*width,*height);
    return true;
  }

  //----------

  /*
    when we have an embedded editor, we don't get the usual window resize events
    so we 'simulate' it
  */

  bool setSize(uint32_t width, uint32_t height) override {
    //SAT_PRINT("*width %i, *height %i\n",width,height);
    MWidth = width;
    MHeight = height;
    if (MWindow /*&& MWindowIsOpen*/) {
      MWindow->setSize(width,height);           // actually resizes the window (will it re-send resize event) (sets MWindowWidth/Height)
      MWindow->on_window_resize(width,height);  // tell the rest of the system about the new size (calc scale. root.on_widget_resize,realign)
    }
    return true;
  }

  //----------

  bool setParent(const clap_window_t *window) override {
    //SAT_PRINT("api %s ptr %p\n",window->api,window->ptr);
    #ifdef SAT_GUI_WIN32
      MParent = window->win32;
    #endif
    #ifdef SAT_GUI_X11
      MParent = window->x11;
    #endif

    if (MWindow) MWindow->reparent(MParent);
    // do we need to resize MWindow.renderer/painter?
    // buffer?

    // MWindow = new SAT_Window(MWidth,MHeight,MParent);
    // MWindowRenderer = new SAT_Renderer(MWindow);
    // MWindowPainter = new SAT_Painter(MWindow);
    // setupEditorWindow(MEditor,window);
    // setEditorParameterValues();

    return true;
  }

  //----------

  bool setTransient(const clap_window_t *window) override {
    //SAT_PRINT("\n");
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
    //SAT_PRINT("title: %s\n",title);
    MTitle = title;
    if (MWindow) MWindow->setTitle(title);
  }

  //----------

  bool show() override {
    //SAT_PRINT("\n");
    if (MWindow) {
      MWindow->on_window_show();
      MWindow->show();
    }
    MWindowIsOpen = true;
    return true;
  }

  //----------

  bool hide() override {
    //SAT_PRINT("\n");
    if (MWindow) {
      MWindow->on_window_hide();
      MWindow->hide();
    }
    MWindowIsOpen = false;
    return true;
  }
  
};

//----------------------------------------------------------------------
#endif
