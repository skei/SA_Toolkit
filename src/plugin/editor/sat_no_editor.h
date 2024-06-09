#ifndef sat_no_editor_included
#define sat_no_editor_included
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/editor/sat_editor_listener.h"
#include "plugin/editor/sat_base_editor.h"

class SAT_NoEditor
: public SAT_BaseEditor  {

//------------------------------
public:
//------------------------------

  SAT_NoEditor(SAT_EditorListener* AListener, uint32_t AWidth, uint32_t AHeight/*, double AScale=1.0, bool AProportional=false*/)
  : SAT_BaseEditor(AListener,AWidth,AHeight/*,AScale,AProportional*/) {
    SAT_TRACE;
  }

  //----------

  virtual ~SAT_NoEditor() {
    SAT_TRACE;
  }

};

//----------------------------------------------------------------------
#endif