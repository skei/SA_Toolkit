#ifndef sat_base_renderer_included
#define sat_base_renderer_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/sat_renderer_owner.h"

//----------------------------------------------------------------------

class SAT_BaseRenderer {

//------------------------------
public:
//------------------------------

  SAT_BaseRenderer() {
  }

  //----------

  virtual ~SAT_BaseRenderer() {
  }

//------------------------------
public:
//------------------------------

  virtual bool initialize(SAT_RendererOwner* AOwner) { return false; }
  virtual void cleanup() {}

  virtual void beginRendering() {}
  virtual void beginRendering(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) {}
  virtual void endRendering() {}

  //virtual void setViewport(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) {}
  //virtual void makeCurrent() {}
  //virtual void resetCurrent() {}
  //virtual void swapBuffers() {}
  //virtual void disableVSync() {}
  //virtual void enableVSync() {}

};

//----------------------------------------------------------------------
#endif
