#ifndef sat_editor_included
#define sat_editor_included
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/clap/sat_clap.h"
#include "plugin/sat_editor_listener.h"
#include "gui/base/sat_window_listener.h"
#include "gui/sat_window.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Editor
: public SAT_WindowListener {

//------------------------------
private:
//------------------------------

  SAT_EditorListener*   MListener   = nullptr;
//sat_atomic_bool_t     MIsClosing  = false;

  double                MScale      = 1.0;
  uint32_t              MWidth      = 640;
  uint32_t              MHeight     = 480;
  const char*           MTitle      = "SAT_Editor";
  const clap_window_t*  MParent     = nullptr;
  const clap_window_t*  MTransient  = nullptr;
  SAT_Window*           MWindow     = nullptr;
  bool                  MIsOpen     = false;

//------------------------------
public:
//------------------------------

  SAT_Editor(SAT_EditorListener* AListener, uint32_t AWidth, uint32_t AHeight, intptr_t AParent=0) {
    //if (AListener) MListener = AListener;
    MListener = AListener;

    #if defined (SAT_GUI_WAYLAND)
      MWindow = new SAT_Window(AWidth,AHeight,this,AParent);
    #elif defined (SAT_GUI_WIN32)
      MWindow = new SAT_Window(AWidth,AHeight,this,AParent);
    #elif defined (SAT_GUI_X11)
      MWindow = new SAT_Window(AWidth,AHeight,this,AParent);
    #endif

    //MWindow->setListener(this);

  }

  //----------

  virtual ~SAT_Editor() {
    if (MWindow /*&& MIsOpen*/) {
      MWindow->close();
      delete MWindow;
    }
  }

//------------------------------
public:
//------------------------------

  virtual SAT_Window* getWindow() { return MWindow; }
  virtual bool        isOpen()    { return MIsOpen; }

//------------------------------
public:
//------------------------------

  virtual void connect(SAT_Widget* AWidget, SAT_Parameter* AParameter) {
    AWidget->setParameter(AParameter);
    AParameter->setWidget(AWidget);
  }

  //----------

  virtual void updateParameterFromHost(SAT_Parameter* AParameter) {
    // uint32_t index = AParameter->getIndex();
    // sat_param_t value = AParameter->getValue();
    // SAT_Print("index %i value %f\n",index,value);
  }

  //----------

  // parameters are in clap-space
  // widgets are 0..1
  
  virtual void initParameterValue(SAT_Parameter* AParam, uint32_t AIndex, uint32_t ASubIndex, sat_param_t AValue) {
    // //SAT_Print("param %p index %i value %.3f\n",AParam,AIndex,AValue);
    // SAT_Widget* widget = (SAT_Widget*)AParam->getWidget();
    // if (widget) {
    //   double v = AParam->normalizeValue(AValue);
    //   widget->setValue(v,ASubIndex);
    // }
  }

  //----------

  /*
    called from SAT_Plugin.flushParamFromHostToGui() - timer thread
    (AValue in clap-space)
  */

  // parameters are in clap-space
  // widgets are 0..1

  virtual void updateParameterFromHost(SAT_Parameter* AParameter, sat_param_t AValue) {
    // if (MIsOpen) {
    //   //SAT_PaintContext* pc = MWindow->getPaintContext();
    //   //uint32_t counter = pc->counter;
    //   SAT_Widget* widget = (SAT_Widget*)AParameter->getWidget();
    //   uint32_t index = AParameter->getWidgetIndex();
    //   if (widget) {
    //     sat_param_t normalized_value = AParameter->normalizeValue(AValue);
    //     widget->setValue(normalized_value,index);
    //     //widget->update();
    //     //parentRedraw();
    //     widget->do_widgetListener_redraw(widget,0);
    //   }
    // }
  }
  
  //----------

  // parameters are in clap-space
  // widgets are 0..1
  
  // AValue, clap-space

  virtual void updateModulationFromHost(SAT_Parameter* AParameter, sat_param_t AValue) {
    // if (MIsOpen) {
    //   SAT_Widget* widget = (SAT_Widget*)AParameter->getWidget();
    //   if (widget) {
    //     //sat_param_t normalized_modulation = AParameter->normalizeValue(AValue);
    //     //widget->setModulation(normalized_modulation);
    //     sat_param_t normalized_value = AParameter->normalizeValue(AValue);
    //     widget->setModulation(normalized_value);
    //     //widget->update();
    //     //widget->parentRedraw();
    //     widget->do_widgetListener_redraw(widget,0);
    //   }
    // }
  }

//------------------------------
public: // window listener
//------------------------------

  // void do_windowListener_update_widget(SAT_Widget* ASender, uint32_t AMode, uint32_t AIndex) override {
  //   if (MListener) {
  //     SAT_Parameter* param = (SAT_Parameter*)ASender->getParameter(AIndex);
  //     if (param) {
  //       double value = ASender->getValue(AIndex); // 0..1
  //       //uint32_t parm_index = ASender->getConnectionIndex();
  //       uint32_t param_index = param->getIndex();
  //       //SAT_Print("AIndex %i param_index %i value %f\n",AIndex,param_index,value);
  //       MListener->do_editorListener_parameter_update(param_index,value);
  //     }
  //   }
  // }

  void on_windowListener_update(SAT_Widget* AWidget) override {
    SAT_Print("%s\n",AWidget->getName());
    sat_param_t value = AWidget->getValue();
    SAT_Parameter* parameter = (SAT_Parameter*)AWidget->getParameter();
    if (parameter) {
      //SAT_Print("parameter %p\n",parameter);
      //SAT_Print("listener %p\n",MListener);
      if (MListener) {
        MListener->on_editorListener_update(parameter,value);
      }
    }
  }

  //----------

  //void on_windowListener_resize(SAT_Window* AWindow, int32_t AWidth, int32_t AHeight) override {
  //}

  //----------

  void on_windowListener_timer(SAT_Timer* ATimer, double AElapsed) override {
  }


//------------------------------
public: // clap_plugin
//------------------------------

  // handled in SAT_Plugin

  //virtual bool is_api_supported(const char *api, bool is_floating) { return false; }
  //virtual bool get_preferred_api(const char **api, bool *is_floating) { return false; }
  //virtual bool create(const char *api, bool is_floating) { return false; }
  //virtual void destroy() {}

  //----------

  virtual bool set_scale(double scale) {
    //SAT_Print("scale %f\n",scale);
    MScale = scale;
    return true;
  }

  //----------

  virtual bool get_size(uint32_t *width, uint32_t *height) {
    //SAT_Print("\n");
    *width = MWidth;
    *height = MHeight;
    return true;
  }

  //----------

  virtual bool can_resize() {
    //SAT_Print("\n");
    return true;
  }

  //----------

  virtual bool get_resize_hints(clap_gui_resize_hints_t *hints) {
    //SAT_Print("\n");
    hints->can_resize_horizontally  = true;
    hints->can_resize_vertically    = true;
    hints->preserve_aspect_ratio    = false;
    hints->aspect_ratio_width       = MWidth;
    hints->aspect_ratio_height      = MHeight;
    return true;
  }

  //----------

  virtual bool adjust_size(uint32_t *width, uint32_t *height) {
    //SAT_Print("*width %i *height %i\n",*width,*height);
    return true;
  }

  //----------

  /*
    we don't receive EXPOSE/WM_PAINT events if window is embedded
    (double check if this is actually/still true) -> yes..
    calling window->setSize will call xcb_configure_window (x11),
    which will result in an NOTIFY event (???)

  */

  virtual bool set_size(uint32_t width, uint32_t height) {
    //SAT_Print("width %i height %i\n",width,height);
    MWidth = width;
    MHeight = height;
    if (MWindow) {
      MWindow->setSize(width,height);
      //if (MIsOpen) { MWindow->on_window_resize(width,height); }
    }
    return true;
  }

  //----------

  /*
    todo: delay window creation until show()?
  */

  virtual bool set_parent(const clap_window_t *window) {
    //SAT_Print("\n");
    MParent = window;
    #if defined (SAT_GUI_WAYLAND)
      MWindow->setParent((intptr_t)MParent->ptr);
      return true;
    #elif defined (SAT_GUI_WIN32)
      MWindow->setParent(MParent->win32);
      return true;
    #elif defined (SAT_GUI_X11)
      SAT_PRINT;
      MWindow->setParent(MParent->x11);
      return true;
    #else
      return false;
    #endif
  }

  //----------

  virtual bool set_transient(const clap_window_t *window) {
    //SAT_Print("\n");
    MTransient = window;
    return true;
  }

  //----------

  virtual void suggest_title(const char *title) {
    //SAT_Print("title %s\n",title);
    MTitle = title;
    if (MWindow) MWindow->setTitle(title);
  }

  //----------

  virtual bool show() {
    //SAT_Print("\n");
     if (MWindow && !MIsOpen) {
      // MWindow->markRootWidgetDirty();
      MWindow->open();
      MIsOpen = true;
      return true;
     }
     return false;
  }

  //----------

  virtual bool hide() {
    //SAT_Print("\n");
    if (MWindow && MIsOpen) {
      MWindow->close();
      MIsOpen = false;
      return true;
    }
    return false;
  }

};

//----------------------------------------------------------------------
#endif

