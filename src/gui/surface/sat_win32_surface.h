#ifndef sat_win32_surface_included
#define sat_win32_surface_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/lib/sat_win32.h"
#include "gui/surface/sat_base_surface.h"
#include "gui/painter/sat_paint_source.h"
#include "gui/painter/sat_paint_target.h"
#include "gui/painter/sat_painter_owner.h"
#include "gui/surface/sat_surface_owner.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Win32Surface
: public SAT_BaseSurface
, public SAT_PainterOwner
, public SAT_PaintSource
, public SAT_PaintTarget {

//------------------------------
private:
//------------------------------

  int32_t           MWidth            = 0;
  int32_t           MHeight           = 0;
  int32_t           MDepth            = 0;
  
//------------------------------
public:
//------------------------------

  SAT_Win32Surface(SAT_SurfaceOwner* AOwner, uint32_t AWidth, uint32_t AHeight, uint32_t ADepth=0)
  : SAT_BaseSurface(AOwner,AWidth,AHeight,ADepth) {
  }

  //----------

  virtual ~SAT_Win32Surface() {
  }

//------------------------------
public:
//------------------------------

  uint32_t getWidth()   override { return MWidth; }
  uint32_t getHeight()  override { return MHeight; }
  uint32_t getDepth()   override { return MDepth; }

//------------------------------
public: // owner
//------------------------------

  // xcb_connection_t* on_painterOwner_getXcbConnection()  override { return MConnection; }
  // xcb_visualid_t    on_painterOwner_getXcbVisual()      override { return MVisual; }
  // xcb_drawable_t    on_paintSource_getXcbDrawable()     override { return MPixmap; }
  // xcb_drawable_t    on_paintTarget_getXcbDrawable()     override { return MPixmap; }

};

//----------------------------------------------------------------------
#endif
















#if 0

#ifndef mip_gdi_surface_included
#define mip_gdi_surface_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/mip_drawable.h"
#include "gui/win32/mip_win32.h"
#include "gui/gdi/mip_gdi.h"

//#include "gui/base/s3_surface_base.h"
//#include "gui/s3_drawable.h"


//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_GdiSurface
: public MIP_Drawable {

  private:
    HBITMAP MHandle;

  private:
    int32_t   MWidth;
    int32_t   MHeight;
    int32_t   MDepth;

//------------------------------
public:
//------------------------------

    /*
      https://msdn.microsoft.com/en-us/library/windows/desktop/dd183488%28v=vs.85%29.aspx
      CreateCompatibleBitmap
      This bitmap can be selected into any memory device context that is
      compatible with the original device.
      Note: When a memory device context is created, it initially has a 1-by-1
      monochrome bitmap selected into it. If this memory device context is used
      in CreateCompatibleBitmap, the bitmap that is created is a monochrome
      bitmap. To create a color bitmap, use the HDC that was used to create the
      memory device context, as shown in the following code:
        HDC memDC = CreateCompatibleDC ( hDC );
        HBITMAP memBM = CreateCompatibleBitmap ( hDC, nWidth, nHeight );
        SelectObject ( memDC, memBM );
    */

    /*
      ADepth
        1   monochrome
        8   greyscale
        16
        24
        32  rgba
    */

  MIP_GdiSurface(MIP_Drawable* ATarget, uint32_t AWidth, uint32_t AHeight, uint32_t ADepth=0) {
    if (ADepth == 0) {
      HDC tempdc = GetDC(nullptr); // dc for the entire screen
      MWidth  = AWidth;
      MHeight = AHeight;
      MDepth  = GetDeviceCaps(tempdc,BITSPIXEL) * GetDeviceCaps(tempdc,PLANES);
      MHandle = CreateCompatibleBitmap(tempdc,MWidth,MHeight);
      ReleaseDC(0,tempdc);
    }
    else {
      HDC tempdc = GetDC(nullptr); // dc for the entire screen
      MWidth  = AWidth;
      MHeight = AHeight;
      MDepth  = ADepth;
      //MHandle = CreateCompatibleBitmap(tempdc,MWidth,MHeight);
      MHandle = CreateBitmap(
        MWidth,   // nWidth,
        MHeight,  // nHeight,
        1,        // cPlanes,
        MDepth,   // cBitsPerPel,
        nullptr   // VOID* lpvBits
      );
      ReleaseDC(0,tempdc);
    }
  }

  /*
    see also:
    // https://msdn.microsoft.com/en-us/library/windows/desktop/dd183485%28v=vs.85%29.aspx
    MHandle = CreateBitmap(
      MWidth, MHeight,
      1,      // planes
      32,     // bits per pixel
      S3_NULL // data
    );
    // https://msdn.microsoft.com/en-us/library/windows/desktop/dd183486%28v=vs.85%29.aspx
    MHandle = CreateBitmapIndirect( ABitmap->hbitmap() );
  */

  //----------

  virtual ~MIP_GdiSurface() {
  }

//------------------------------
public: // drawable
//------------------------------

//  int32   width(void)   override { return MWidth; }
//  int32   height(void)  override { return MHeight; }
//  int32   depth(void)   override { return MDepth; }
//  //void*   buffer(void)  override   { return S3_NULL; }
//  HBITMAP hbitmap(void) override { return MHandle; }

};

//----------------------------------------------------------------------
#endif

#endif // 0






























#if 0

#ifndef s3_image_gdi_included
#define s3_image_gdi_included
//----------------------------------------------------------------------

// HBITMAP = CreateDIBSection
// DIB : Device Independent Bitmap (ram/client)

/*
  COLORREF = 0xaabbggrr = rgba (little endian)
  RGBA macro
*/

//----------------------------------------------------------------------

#include "common/s3_memory_stdlib.h"
#include "gui/base/s3_image_base.h"
#include "gfx/s3_bitmap.h"
#include "gui/s3_drawable.h"

//class S3_Image_Win32;
//typedef S3_Image_Win32 S3_Image_Implementation;

//----------

class S3_Image_Gdi
: public S3_Image_Base
, public S3_Drawable {

  private:
    HBITMAP     MHandle;
    //S3_Bitmap*  MBitmap;
  private:
    int32     MWidth;
    int32     MHeight;
    int32     MDepth;
    //int32     MStride;
    void*     MBuffer;

  //------------------------------
  //
  //------------------------------

  public:

    S3_Image_Gdi(int32 AWidth, int32 AHeight, int32 ADepth=32)
    : S3_Image_Base(AWidth,AHeight,ADepth) {
      MWidth = AWidth;
      MHeight = AHeight;
      MDepth = ADepth;
      //switch (MDepth) {
      //  case 1:   MStride = MWidth >> 3; break;
      //  case 8:   MStride = MWidth;      break;
      //  case 16:  MStride = MWidth * 2;  break;
      //  case 24:  MStride = MWidth * 3;  break;
      //  case 32:  MStride = MWidth * 4;  break;
      //}
      BITMAPINFO bmi;
      S3_Memset(&bmi,0,sizeof(BITMAPINFO));
      bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
      bmi.bmiHeader.biWidth       = MWidth;     // width of the bitmap, in pixels.
      bmi.bmiHeader.biHeight      = -MHeight;   // height (negative)
      bmi.bmiHeader.biPlanes      = 1;          // number of planes for target device. must be set to 1.
      bmi.bmiHeader.biBitCount    = MDepth;     // bits per pixel
      bmi.bmiHeader.biCompression = BI_RGB;     // uncompressed
      bmi.bmiHeader.biSizeImage   = 0;          // size, in bytes, of the image. may be set to zero for BI_RGB bitmaps.
      HDC tempdc = GetDC(0);
      MHandle = CreateDIBSection(tempdc,&bmi,DIB_RGB_COLORS,&MBuffer,NULL,0);
      ReleaseDC(0,tempdc);
    }

    //----------

    /*
    */

    //----------

    virtual ~S3_Image_Gdi() {
      DeleteObject(MHandle);
    }

  //------------------------------
  //
  //------------------------------

  public:

  //------------------------------
  // S3_Drawable
  //------------------------------

  public:

    int32   width(void)   { return MWidth; }
    int32   height(void)  { return MHeight; }
    int32   depth(void)   { return MDepth; }
    int32   stride(void)   { return MDepth; }
    void*   buffer(void)  { return MBuffer; }
    HBITMAP hbitmap(void) { return MHandle; }

};

//----------------------------------------------------------------------
#endif

#endif // 0


















#if 0

#ifndef s3_surface_gdi_included
#define s3_surface_gdi_included
//----------------------------------------------------------------------

#include "gui/base/s3_surface_base.h"
#include "gui/s3_drawable.h"

//----------------------------------------------------------------------

class S3_Surface_Gdi
: public S3_Surface_Base
, public S3_Drawable {

  private:
    HBITMAP MHandle;
  private:
    int32   MWidth;
    int32   MHeight;
    int32   MDepth;

  //------------------------------
  //
  //------------------------------

  public:

    /*
      https://msdn.microsoft.com/en-us/library/windows/desktop/dd183488%28v=vs.85%29.aspx
      CreateCompatibleBitmap
      This bitmap can be selected into any memory device context that is
      compatible with the original device.
      Note: When a memory device context is created, it initially has a 1-by-1
      monochrome bitmap selected into it. If this memory device context is used
      in CreateCompatibleBitmap, the bitmap that is created is a monochrome
      bitmap. To create a color bitmap, use the HDC that was used to create the
      memory device context, as shown in the following code:
        HDC memDC = CreateCompatibleDC ( hDC );
        HBITMAP memBM = CreateCompatibleBitmap ( hDC, nWidth, nHeight );
        SelectObject ( memDC, memBM );
    */

    S3_Surface_Gdi(int32 AWidth, int32 AHeight)
    : S3_Surface_Base(AWidth,AHeight) {
      HDC tempdc = GetDC(S3_NULL); // dc for the entire screen
      MWidth  = AWidth;
      MHeight = AHeight;
      MDepth  = GetDeviceCaps(tempdc,BITSPIXEL) * GetDeviceCaps(tempdc,PLANES);
      MHandle = CreateCompatibleBitmap(tempdc,MWidth,MHeight);
      ReleaseDC(0,tempdc);
    }

    //----------

    /*
      ADepth
        1   monochrome
        8   greyscale
        16
        24
        32  rgba
    */

    S3_Surface_Gdi(int32 AWidth, int32 AHeight, int32 ADepth)
    : S3_Surface_Base(AWidth,AHeight,ADepth) {
      HDC tempdc = GetDC(S3_NULL); // dc for the entire screen
      MWidth  = AWidth;
      MHeight = AHeight;
      MDepth  = ADepth;
      //MHandle = CreateCompatibleBitmap(tempdc,MWidth,MHeight);
      MHandle = CreateBitmap(
        MWidth,   // nWidth,
        MHeight,  // nHeight,
        1,        // cPlanes,
        MDepth,   // cBitsPerPel,
        S3_NULL   // VOID* lpvBits
      );
      ReleaseDC(0,tempdc);
    }

    //----------

    /*
      see also:

      // https://msdn.microsoft.com/en-us/library/windows/desktop/dd183485%28v=vs.85%29.aspx
      MHandle = CreateBitmap(
        MWidth, MHeight,
        1,      // planes
        32,     // bits per pixel
        S3_NULL // data
      );

      // https://msdn.microsoft.com/en-us/library/windows/desktop/dd183486%28v=vs.85%29.aspx
      MHandle = CreateBitmapIndirect( ABitmap->hbitmap() );
    */

    //----------

    virtual ~S3_Surface_Gdi() {
      DeleteObject(MHandle);
    }

  //------------------------------
  // S3_Drawable
  //------------------------------

  public:

    virtual int32   width(void)   { return MWidth; }
    virtual int32   height(void)  { return MHeight; }
    virtual int32   depth(void)   { return MDepth; }
    //virtual void*   buffer(void)    { return S3_NULL; }
    virtual HBITMAP hbitmap(void) { return MHandle; }

  //------------------------------
  //
  //------------------------------

  public:


};

//----------------------------------------------------------------------
#endif

#endif // 0