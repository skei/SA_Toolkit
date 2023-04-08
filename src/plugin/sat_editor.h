#ifndef sat_editor_included
#define sat_editor_included
//----------------------------------------------------------------------

#include "plugin/sat_editor_listener.h"
#include "gui/sat_window.h"
#include "gui/sat_window_listener.h"

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

  uint32_t              MWidth            = 0;
  uint32_t              MHeight           = 0;
  bool                  MProportional     = false;
  double                MScale            = 1.0;
  const clap_window_t*  MParent           = nullptr;
  const char*           MTitle            = "";
  bool                  MIsOpen           = false;
  SAT_EditorListener*   MListener         = nullptr;
  SAT_Window*           MWindow           = nullptr;
  bool                  MPreparedWidgets  = false;

//------------------------------
public:
//------------------------------

  SAT_Editor(SAT_EditorListener* AListener, uint32_t AWidth, uint32_t AHeight) {
    //SAT_PRINT;
    MListener = AListener;
    MWidth    = AWidth;
    MHeight   = AHeight;
  }

  //----------

  /*
    when our window is embedded, we don't get on_window_close events
  */

  virtual ~SAT_Editor() {
    if (MWindow) {
      MWindow->on_window_close();
      delete MWindow;
    }
  }

//------------------------------
public:
//------------------------------

  bool isOpen() {
    return MIsOpen;
  }

  SAT_Window* getWindow() {
    return MWindow;
  }

  //----------

  /*
    called from SAT_Plugin.updateEditorParameterValues()
    - before editor is opened
    - todo: SAT_Plugin.state_load ???
  */

  //void updateEditorParameterValue(uint32_t AIndex, sat_param_t AValue, bool ARedraw) {
  //  //SAT_Print("%i = %.3f %s\n",AIndex,AValue,ARedraw?" (redraw)":"");
  //}
  
  void updateParameterValue(SAT_Parameter* AParam, uint32_t AIndex, sat_param_t AValue) {
    //SAT_Print("param %p index %i value %.3f\n",AParam,AIndex,AValue);
    SAT_Widget* widget = (SAT_Widget*)AParam->getConnection();
    if (widget) {
      widget->setValue(AValue);
    }
  }

  //----------

  /*
    called from flushParamFromHostToGui()
    timer thread
  */

  void updateParameterFromHost(SAT_Parameter* AParameter, sat_param_t AValue) {
    if (MIsOpen) {
      //SAT_PaintContext* pc = MWindow->getPaintContext();
      //uint32_t counter = pc->counter;
      SAT_Widget* widget = (SAT_Widget*)AParameter->getConnection();
      if (widget) {
        widget->setValue(AValue);
        //widget->update();
        //parentRedraw();
        widget->do_widget_redraw(widget,0);
      }
    }
  }
  
  //----------

  void updateModulationFromHost(SAT_Parameter* AParameter, sat_param_t AValue) {
    if (MIsOpen) {
      SAT_Widget* widget = (SAT_Widget*)AParameter->getConnection();
      if (widget) {
        widget->setModulation(AValue);
        //widget->update();
        //widget->parentRedraw();
        widget->do_widget_redraw(widget,0);
      }
    }
  }

  //----------

  /*
    called from
    - SAT_Editor.set_parent()
  */

  virtual SAT_Window* createWindow(uint32_t AWidth, uint32_t AHeight, intptr_t AParent) {
    //SAT_Print("creating plugin window\n");
    SAT_Window* window = new SAT_Window(AWidth,AHeight,AParent,this);
    //SAT_Print("plugin window created\n");
    return window;
  }

  //----------

  virtual void connect(SAT_Widget* AWidget, SAT_Parameter* AParameter, uint32_t AIndex=0) {
    AWidget->connect(AParameter,AIndex);
    AParameter->connect(AWidget);
  }

//------------------------------
public: // window listener
//------------------------------

  /*
    this is called from whatever thread the timer is called from
    push update into gui thread somwhow..
  */

  void do_windowListener_timer(SAT_Window* ASender, double AElapsed) override { // final
    //SAT_PRINT;
    if (MListener) MListener->do_editor_listener_timer();
    //on_window_timer();
  }

  //----------

  void do_windowListener_update_widget(SAT_Widget* ASender, uint32_t AMode, uint32_t AIndex) override {
    if (MListener) {
      SAT_Parameter* param = (SAT_Parameter*)ASender->getConnection();
      if (param) {
        uint32_t index = param->getIndex();
        double value = ASender->getValue();
        MListener->do_editor_listener_parameter_update(index,value);
      }
    }
  }

  //----------

  void do_windowListener_redraw_widget(SAT_Widget* ASender, uint32_t AMode, uint32_t AIndex) override {
    //SAT_PRINT;
  }

  //----------

  //void do_window_listener_set_widget_state(SAT_Widget* ASender, uint32_t AState) override {
  //  //SAT_PRINT;
  //}

  //----------

  //void do_window_listener_set_cursor(SAT_Widget* ASender, uint32_t ACursor) override {
  //  //SAT_PRINT;
  //}

//------------------------------
public: // clap
//------------------------------

  bool create() {
    //SAT_Print("\n");
    return true;
  }

  //----------

  void destroy() {
    //SAT_Print("\n");
    //if (MWindow) MWindow->on_window_close();
    MIsOpen = false;
  }

  //----------

  bool set_scale(double scale) {
    //SAT_Print("scale %f\n",scale);
    MScale = scale;     // win32: crashes.. huh?
    //SAT_Print("...\n"); // win32; not printed..
    return true;
  }

  //----------

  bool get_size(uint32_t *width, uint32_t *height) {
    //SAT_Print("-> %i,%i\n",MWidth,MHeight);
    *width = MWidth;
    *height = MHeight;
    //SAT_PRINT;
    return true;
  }

  //----------

  bool can_resize() {
    //SAT_Print("\n");
    return true;
  }

  //----------

  bool get_resize_hints(clap_gui_resize_hints_t *hints) {
    //SAT_Print("\n");
    hints->can_resize_horizontally  = true;
    hints->can_resize_vertically    = true;
    hints->preserve_aspect_ratio    = false;
    hints->aspect_ratio_width       = MWidth;
    hints->aspect_ratio_height      = MHeight;
    return true;
  }

  //----------

  /*
    ?? *width / *height contains 'suggested' (resized) size
    return wanted/new size
    bitwig seems to call this repeatedly until the suggested, and returned
    sizes are equal, so be careful with rounding/scaling..
  */

  bool adjust_size(uint32_t *width, uint32_t *height) {
    //SAT_Print("(%i,%i)\n",*width,*height);
    return true;
  }

  //----------

  /*
    when our window is embedded, we don't get any on_window_resize
    event, so we simulate one from here..
  */

  bool set_size(uint32_t width, uint32_t height) {
    //SAT_Print("width %i height %i\n",width,height);
    MWidth = width;
    MHeight = height;
    if (MWindow) {
      MWindow->setSize(width,height);
      MWindow->on_window_resize(width,height);
    }
    return true;
  }

  //----------

  bool set_parent(const clap_window_t *window) {
    //SAT_Print("\n");
    MParent = window;
    if (!MWindow) {
      MWindow = createWindow(MWidth,MHeight,window->x11);
    }
    else {
      MWindow->reparent(window->x11);
    }
    return true;
  }

  //----------

  bool set_transient(const clap_window_t *window) {
    //SAT_Print("\n");
    return true;
  }

  //----------

  void suggest_title(const char *title) {
    //SAT_Print("title %s\n",title);
    MTitle = title;
    if (MWindow) MWindow->setTitle(title);
  }

  //----------

  bool show() {
    //SAT_Print("\n");
    if (MWindow && !MIsOpen) {
      // when our window is embedded, we don't get on_window_open events ??

//      if (!MPreparedWidgets) {
//        MWindow->on_window_open();
//        MPreparedWidgets = true;
//      }

      MWindow->show();
    }
    MIsOpen = true;
    return true;
  }

  //----------

  bool hide() {
    //SAT_Print("\n");
    if (MWindow && MIsOpen) MWindow->hide();
    MIsOpen = false;
    return true;
  }

};

//----------------------------------------------------------------------
#endif
