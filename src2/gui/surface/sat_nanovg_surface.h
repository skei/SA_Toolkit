#ifndef sat_nanovg_surface_included
#define sat_nanovg_surface_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/lib/sat_nanovg.h"
#include "gui/surface/sat_base_surface.h"
#include "gui/painter/sat_painter_owner.h"
#include "gui/painter/sat_paint_source.h"
#include "gui/painter/sat_paint_target.h"

//#include "gui/lib/sat_x11.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_NanoVGSurface
: public SAT_BaseSurface
, public SAT_PainterOwner
, public SAT_PaintSource
, public SAT_PaintTarget {


//------------------------------
private:
//------------------------------

  NVGcontext*       MContext      = nullptr;
  NVGLUframebuffer* MFrameBuffer  = nullptr;

//------------------------------
public:
//------------------------------

  /*
    NVGimageFlags
      NVG_IMAGE_GENERATE_MIPMAPS  Generate mipmaps during creation of the image.
      NVG_IMAGE_REPEATX           Repeat image in X direction.
      NVG_IMAGE_REPEATY           Repeat image in Y direction.
      NVG_IMAGE_FLIPY             Flips (inverses) image in Y direction when rendered.
      NVG_IMAGE_PREMULTIPLIED     Image data has premultiplied alpha.
      NVG_IMAGE_NEAREST           Image interpolation is Nearest instead Linear
    // additional flags on top of NVGimageFlags.
    NVGimageFlagsGL
      NVG_IMAGE_NODELETE          Do not delete GL texture handle.
  */

  SAT_NanoVGSurface(SAT_SurfaceOwner* AOwner, uint32_t AWidth, uint32_t AHeight, uint32_t ADepth=0)
  : SAT_BaseSurface(AOwner,AWidth,AHeight,ADepth) {
    SAT_TRACE;

    // if (AOwner->on_surfaceOwner_isNanoVG()) {
    //   MContext = AOwner->on_surfaceOwner_getNanoVGContext();
    //   SAT_Assert(MContext);
    //   if (MContext) {
    //     int flags = 0;
    //     MFrameBuffer = nvgluCreateFramebuffer(MContext,AWidth,AHeight,flags);
    //     SAT_Assert(MFrameBuffer);
    //   }
    // }

  }

  //----------

  virtual ~SAT_NanoVGSurface() {
    //NVGLUframebuffer* fb = (NVGLUframebuffer*)buffer;
    nvgluBindFramebuffer(nullptr);
    if (MFrameBuffer) nvgluDeleteFramebuffer(MFrameBuffer);
  }

//------------------------------
public: // owner
//------------------------------

  #ifdef SAT_PAINTER_NANOVG
  xcb_connection_t* on_painterOwner_getXcbConnection()  override { return nullptr; }  // MConnection; }
  xcb_visualid_t    on_painterOwner_getXcbVisual()      override { return XCB_NONE; } // MScreenVisual; }
  #endif

  #ifdef SAT_PAINTER_X11
  xcb_connection_t* on_painterOwner_getXcbConnection()  override { return nullptr; }  // MConnection; }
  xcb_visualid_t    on_painterOwner_getXcbVisual()      override { return XCB_NONE; } // MScreenVisual; }
  #endif

  //----------

  #ifdef SAT_PAINTER_NANOVG
  xcb_drawable_t    on_paintSource_getXcbDrawable()     override { return XCB_NONE; }
  #endif

  #ifdef SAT_PAINTER_X11
  xcb_drawable_t    on_paintSource_getXcbDrawable()     override { return XCB_NONE; }
  #endif

  //----------

  #ifdef SAT_PAINTER_NANOVG
  xcb_drawable_t    on_paintTarget_getXcbDrawable()     override { return XCB_NONE; }
  #endif

  #ifdef SAT_PAINTER_X11
  xcb_drawable_t    on_paintTarget_getXcbDrawable()     override { return XCB_NONE; }
  #endif


//------------------------------
public:
//------------------------------

  // get image from fbo/renderbuffer
  // (use as fillImage)
  
  int32_t getImage() {
    if (MFrameBuffer) return MFrameBuffer->image;
    return 0;
  }

  //----------

  // set fbo/renderbuffer as target for painting

  void select() {
    if (MFrameBuffer) nvgluBindFramebuffer(MFrameBuffer);
  }

  //----------

  void deselect() {
    nvgluBindFramebuffer(nullptr);
  }

};

//----------------------------------------------------------------------
#endif


#if 0

  //----------
  
  // set fbo/renderbuffer as target for painting
  // and sets opengl viewport (0,0,w,h)

//  void selectRenderBuffer(void* buffer, uint32_t width, uint32_t height) final {
//    MCurrentRenderBuffer = buffer;
//    NVGLUframebuffer* fb = (NVGLUframebuffer*)buffer;
//    nvgluBindFramebuffer(fb);
//    MOpenGL->setViewport(0,0,width,height);
//  }

  //----------

  // set fbo/renderbuffer as target for painting
  // and sets opengl viewport (x,y,w,h)

//  void selectRenderBuffer(void* buffer, uint32_t xpos, uint32_t ypos, uint32_t width, uint32_t height) final {
//    MCurrentRenderBuffer = buffer;
//    NVGLUframebuffer* fb = (NVGLUframebuffer*)buffer;
//    nvgluBindFramebuffer(fb);
//    MOpenGL->setViewport(xpos,ypos,width,height);
//  }


#endif // 0