#ifndef sat_base_renderer_included
#define sat_base_renderer_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/base/sat_renderer_owner.h"

//----------------------------------------------------------------------

class SAT_BaseRenderer {

//------------------------------
public:
//------------------------------

  SAT_BaseRenderer(SAT_RendererOwner* AOwner) {
  }

  //----------

  virtual ~SAT_BaseRenderer() {
  }

//------------------------------
public:
//------------------------------

  virtual uint32_t    getType() { return 0; }
  virtual const char* getTypeName() { return ""; }

  // make the context current

  virtual bool        beginRendering() { return false; }

  // make the context current, and sets the viewport (0,0,w,h)
  
  virtual bool        beginRendering(int32_t AWidth, int32_t AHeight) { return false; }

  // swap buffers, and make context un-current

  virtual bool        endRendering() { return false; }

  //

  virtual bool        setViewport(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) { return false; }
  virtual bool        makeCurrent() { return false; }
  virtual bool        resetCurrent() { return false; }
  virtual bool        swapBuffers() { return false; }
  virtual bool        disableVSync() { return false; }
  virtual bool        enableVSync() { return false; }

};

//----------------------------------------------------------------------
#endif
