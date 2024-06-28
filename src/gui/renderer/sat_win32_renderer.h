#ifndef sat_win32_renderer_included
#define sat_win32_renderer_included
//----------------------------------------------------------------------


//----------------------------------------------------------------------

#include "sat.h"
#include "gui/lib/sat_win32.h"
#include "gui/renderer/sat_base_renderer.h"
#include "gui/renderer/sat_renderer_owner.h"

class SAT_Renderer;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Win32Renderer
: public SAT_BaseRenderer {

//------------------------------
public:
//------------------------------

  SAT_Win32Renderer(SAT_RendererOwner* AOwner, SAT_RenderTarget* ATarget)
  : SAT_BaseRenderer(AOwner,ATarget) {
    SAT_TRACE;
  }

  virtual ~SAT_Win32Renderer() {
  }

//------------------------------
public:
//------------------------------

  // bool initialize() {
  //   return true;
  // }

  // void cleanup() {
  // }
  
  bool makeCurrent() override {
    return true;
  }

  bool resetCurrent() override {
    return true;
  }
  
  // bool isCurrent() override {
  //   return true;
  // }
  
  bool beginRendering() override {
    return true;
  }
  
  bool beginRendering(int32_t AWidth, int32_t AHeight) override {
    return true;
  }
  
  bool endRendering() override {
    return true;
  }
  
  bool setViewport(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) override {
    return true;
  }
  
  bool swapBuffers() override {
    return true;
  }
  
  bool enableVSync() override {
    return true;
  }
  
  bool disableVSync() override {
    return true;
  }
  
};

//----------------------------------------------------------------------
#endif