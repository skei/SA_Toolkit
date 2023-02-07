#ifndef sat_editor_included
#define sat_editor_included
//----------------------------------------------------------------------

class SAT_Editor {

//------------------------------
private:
//------------------------------

  uint32_t              MWidth        = 0;
  uint32_t              MHeight       = 0;
  bool                  MProportional = false;
  double                MScale        = 1.0;
  const clap_window_t*  MParent       = nullptr;
  const char*           MTitle        = "";
  bool                  MIsOpen       = false;

//------------------------------
public:
//------------------------------

  SAT_Editor() {
  }

  //----------

  virtual ~SAT_Editor() {
  }

//------------------------------
public:
//------------------------------

  //bool create() {
  //  return true;
  //}

  //----------

  //void destroy() {
  //}

  //----------

  bool set_scale(double scale) {
    MScale = scale;
    return true;
  }

  //----------

  bool get_size(uint32_t *width, uint32_t *height) {
    *width = MWidth;
    *height = MHeight;
    return true;
  }

  //----------

  bool can_resize() {
    return true;
  }

  //----------

  bool get_resize_hints(clap_gui_resize_hints_t *hints) {
    hints->can_resize_horizontally  = true;
    hints->can_resize_vertically    = true;
    hints->preserve_aspect_ratio    = false;
    hints->aspect_ratio_width       = MWidth;
    hints->aspect_ratio_height      = MWidth;
    return true;
  }

  //----------

  bool adjust_size(uint32_t *width, uint32_t *height) {
    return true;
  }

  //----------

  bool set_size(uint32_t width, uint32_t height) {
    MWidth = width;
    MHeight = height;
    return true;
  }

  //----------

  bool set_parent(const clap_window_t *window) {
    MParent = window;
    return true;
  }

  //----------

  bool set_transient(const clap_window_t *window) {
    return true;
  }

  //----------

  void suggest_title(const char *title) {
    MTitle = title;
  }

  //----------

  bool show() {
    MIsOpen = true;
    return true;
  }

  //----------

  bool hide() {
    MIsOpen = false;
    return true;
  }

};

//----------------------------------------------------------------------
#endif
