#ifndef sat_editor_included
#define sat_editor_included
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/clap/sat_clap.h"
#include "plugin/sat_editor_listener.h"
#include "gui/sat_window.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Editor {

//------------------------------
private:
//------------------------------

  SAT_EditorListener* MListener = nullptr;

  double                MScale      = 1.0;
  uint32_t              MWidth      = 640;
  uint32_t              MHeight     = 480;
  const char*           MTitle      = "SAT_Editor";
  const clap_window_t*  MParent     = nullptr;
  const clap_window_t*  MTransient  = nullptr;
  SAT_Window*           MWindow     = nullptr;
  bool                  MIsOpen     = false;
  //sat_atomic_bool_t     MIsClosing  = false;

//------------------------------
public:
//------------------------------

  SAT_Editor(SAT_EditorListener* AListener) {
  }

  //----------

  virtual ~SAT_Editor() {
  }

//------------------------------
public:
//------------------------------

  virtual SAT_Window* getWindow() {
    return MWindow;
  }

//------------------------------
public:
//------------------------------

  //virtual bool is_api_supported(const char *api, bool is_floating) {
  //  return false;
  //}

  //----------

  //virtual bool get_preferred_api(const char **api, bool *is_floating) {
  //  return false;
  //}

  //----------

  //virtual bool create(const char *api, bool is_floating) {
  //  return false;
  //}

  //----------

  //virtual void destroy() {
  //}

  //----------

  virtual bool set_scale(double scale) {
    SAT_Print("scale %f\n",scale);
    MScale = scale;
    return true;
  }

  //----------

  virtual bool get_size(uint32_t *width, uint32_t *height) {
    SAT_Print("\n");
    *width = MWidth;
    *height = MHeight;
    return true;
  }

  //----------

  virtual bool can_resize() {
    SAT_Print("\n");
    return true;
  }

  //----------

  virtual bool get_resize_hints(clap_gui_resize_hints_t *hints) {
    SAT_Print("\n");
    hints->can_resize_horizontally  = true;
    hints->can_resize_vertically    = true;
    hints->preserve_aspect_ratio    = false;
    hints->aspect_ratio_width       = MWidth;
    hints->aspect_ratio_height      = MHeight;
    return true;
  }

  //----------

  virtual bool adjust_size(uint32_t *width, uint32_t *height) {
    SAT_Print("*width %i *height %i\n",*width,*height);
    return true;
  }

  //----------

  virtual bool set_size(uint32_t width, uint32_t height) {
    SAT_Print("width %i height %i\n",width,height);
    MWidth = width;
    MHeight = height;
    if (MWindow) {
      MWindow->setSize(width,height);
    }
    return true;
  }

  //----------

  virtual bool set_parent(const clap_window_t *window) {
    SAT_Print("\n");
    MParent = window;
    if (MWindow) {
      #if defined (SAT_GUI_WAYLAND)
        MWindow->setParent((intptr_t)window->ptr);
      #elif defined (SAT_GUI_WIN32)
        MWindow->setParent(window->win32);
      #elif defined (SAT_GUI_X11)
        MWindow->setParent(window->x11);
      #endif
    }
    else {
      #if defined (SAT_GUI_WAYLAND)
        MWindow = new SAT_Window(MWidth,MHeight,(intptr_t)MParent->ptr);
      #elif defined (SAT_GUI_WIN32)
        MWindow = new SAT_Window(MWidth,MHeight,MParent->win32);
      #elif defined (SAT_GUI_X11)
        MWindow = new SAT_Window(MWidth,MHeight,MParent->x11);
      #endif
    }
    return true;
  }

  //----------

  virtual bool set_transient(const clap_window_t *window) {
    SAT_Print("\n");
    MTransient = window;
    return true;
  }

  //----------

  virtual void suggest_title(const char *title) {
    SAT_Print("title %s\n",title);
    MTitle = title;
    if (MWindow) MWindow->setTitle(title);
  }

  //----------

  virtual bool show() {
    SAT_Print("\n");
     if (MWindow) {
      MWindow->open();
      MIsOpen = true;
      return true;
     }
     return false;
  }

  //----------

  virtual bool hide() {
    SAT_Print("\n");
    if (MWindow) {
      MWindow->close();
      MIsOpen = false;
      return true;
    }
    return false;
  }

};

//----------------------------------------------------------------------
#endif

