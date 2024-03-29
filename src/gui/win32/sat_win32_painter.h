#ifndef sat_win32_painter_included
#define sat_win32_painter_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/base/sat_base_painter.h"

//----------------------------------------------------------------------

class SAT_Win32Painter
: public SAT_BasePainter {

//------------------------------
public:
//------------------------------

  SAT_Win32Painter(SAT_PainterOwner* AOwner)
  : SAT_BasePainter(AOwner) {
  }

  //----------

  virtual ~SAT_Win32Painter() {
  }


};

//----------------------------------------------------------------------
#endif
