#ifndef sat_editor_included
#define sat_editor_included
//----------------------------------------------------------------------

#include "plugin/sat_parameter.h"

#ifndef SAT_NO_GUI
  #include "gui/sat_gui_base.h"
  #include "gui/sat_widget.h"
#endif

//----------------------------------------------------------------------

#ifdef SAT_NO_GUI

  #include "plugin/editor/sat_no_editor.h"
  typedef SAT_NoEditor SAT_ImplementedEditor;

#else

  #ifdef SAT_EDITOR_EMBEDDED
    #include "plugin/editor/sat_embedded_editor.h"
    typedef SAT_EmbeddedEditor SAT_ImplementedEditor;
  #endif

  #ifdef SAT_EDITOR_REMOTE
    #include "plugin/editor/sat_remote_editor.h"
    typedef SAT_RemoteEditor SAT_ImplementedEditor;
  #endif

#endif

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Editor
: public SAT_ImplementedEditor {

//------------------------------
public:
//------------------------------

  SAT_Editor(SAT_EditorListener* AListener/*, uint32_t AWidth, uint32_t AHeight, double AScale=1.0, bool AProportional=false*/)
  : SAT_ImplementedEditor(AListener/*,AWidth,AHeight,AScale,AProportional*/) {
  }

  //----------

  virtual ~SAT_Editor() {
  }

};

//----------------------------------------------------------------------
#endif