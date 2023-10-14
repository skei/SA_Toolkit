#ifndef sat_base_renderer_included
#define sat_base_renderer_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/base/sat_base_surface.h"
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

  //virtual bool setSurface(SAT_BaseSurface* ASurface) { return false; }

  virtual bool beginRendering() { return false; }
  virtual bool beginRendering(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) { return false; }
  virtual bool endRendering() { return false; }

  virtual bool setViewport(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) { return false; }
  virtual bool makeCurrent() { return false; }
  virtual bool resetCurrent() { return false; }
  virtual bool swapBuffers() { return false; }
  virtual bool disableVSync() { return false; }
  virtual bool enableVSync() { return false; }

};

//----------------------------------------------------------------------
#endif
