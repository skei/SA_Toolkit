#ifndef sat_base_renderer_included
#define sat_base_renderer_included
//----------------------------------------------------------------------


//----------------------------------------------------------------------

#include "sat.h"
#include "gui/renderer/sat_render_target.h"
#include "gui/renderer/sat_renderer_owner.h"

class SAT_Renderer;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_BaseRenderer {

//------------------------------
public:
//------------------------------

  SAT_BaseRenderer(SAT_RendererOwner* AOwner, SAT_RenderTarget* ATarget) {}
  virtual ~SAT_BaseRenderer() {}

//------------------------------
public:
//------------------------------

  // virtual bool initialize() { return true; }
  // virtual void cleanup() {}
  
  virtual bool makeCurrent() { return false; }
  virtual bool resetCurrent() { return false;}
  // virtual bool isCurrent() { return false; }

  virtual bool beginRendering() { return true; }
  virtual bool beginRendering(int32_t AWidth, int32_t AHeight) { return true; }
  virtual bool endRendering() { return true; }
  
  virtual bool setViewport(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) { return true; }
  virtual bool swapBuffers() { return true; }
  virtual bool enableVSync() { return true; }
  virtual bool disableVSync() { return true; }
  
};

//----------------------------------------------------------------------
#endif