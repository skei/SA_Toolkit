#ifndef sat_bitmap_included
#define sat_bitmap_included
//----------------------------------------------------------------------

//#define SAT_NO_BITMAP_PNG
//#define SAT_NO_BITMAP_PNG_FILE
//#define SAT_USE_CAIRO


//----------------------------------------------------------------------

#include "base/sat.h"
#include "base/utils/sat_integer_math.h"
#include "base/utils/sat_random.h"
#include "base/types/sat_color.h"
#include "gfx/sat_rgba.h"

//#include "gui/sat_paint_source.h"
//#include "gui/sat_paint_target.h"

//#ifdef SAT_USE_CAIRO
//  #include "gui/cairo/sat_cairo.h"
//#endif

#ifndef SAT_NO_BITMAP_PNG
  #ifndef STBI_INCLUDE_STB_IMAGE_H
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wmisleading-indentation"
    #define STB_IMAGE_IMPLEMENTATION
    #include "extern/stb/stb_image.h"
    #pragma GCC diagnostic pop
  #endif
#endif

//#ifndef SAT_NO_BITMAP_FONT
//  #define STB_TRUETYPE_IMPLEMENTATION
//  #include "extern/stb/stb_truetype.h"
//#endif

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Bitmap
/*: public SAT_PaintSource
, public SAT_PaintTarget*/ {

//------------------------------
private:
//------------------------------

  uint32_t  MWidth      = 0;
  uint32_t  MHeight     = 0;
  uint32_t  MDepth      = 0;
  uint32_t  MStride     = 0;
  uint32_t* MBuffer     = nullptr;
  uint32_t  MBufferSize = 0;
  bool      MAllocated  = false;

//------------------------------
public:
//------------------------------

  // Creates a null bitmap (using default values)

  SAT_Bitmap() {
  }

  //----------

  //----------

  // create new/empty

  SAT_Bitmap(uint32_t AWidth, uint32_t AHeight) {
    MWidth      = AWidth;
    MHeight     = AHeight;
    MStride     = MWidth * 4;
    MBufferSize = MStride * MHeight;
    MBuffer     = (uint32_t*)malloc(MBufferSize);
    MAllocated  = true;
  }

  //----------

  // create new, provide buffer

  SAT_Bitmap(uint32_t AWidth, uint32_t AHeight, uint32_t* ABuffer) {
    MWidth      = AWidth;
    MHeight     = AHeight;
    MStride     = MWidth * 4;
    MBufferSize = MStride * MHeight;
    MBuffer     = ABuffer;
    MAllocated  = false;
  }

  //----------

  // Clones/copies a part of another bitmap..
  // new memory is allocated, and bitmap is copied into it.

  SAT_Bitmap(SAT_Bitmap* ABitmap, uint32_t AX, uint32_t AY, uint32_t AW, uint32_t AH) {
    MWidth      = AW;
    MHeight     = AH;
    MStride     = MWidth * 4;
    MBufferSize = MStride * MHeight;
    MBuffer     = (uint32_t*)malloc(MBufferSize);
    MAllocated  = true;
    uint32_t* dst = (uint32_t*)MBuffer;
    for (uint32_t y=0; y<MHeight; y++) {
      uint32_t* src = (uint32_t*)ABitmap->getLinePtr(AY + y);
      memcpy(dst,src,MWidth*4);
      dst += MWidth;
    }
  }

  //----------

  // Decodes an image from memory (using stb_image)

  #ifndef SAT_NO_BITMAP_PNG
  SAT_Bitmap(const uint8_t* buffer, uint32_t length) {
    int x,y,n;
    unsigned char* data = stbi_load_from_memory(buffer,length,&x,&y,&n,4 /*0*/ );
    MWidth      = x;
    MHeight     = y;
    MStride     = MWidth  * 4; //n;
    MBufferSize = MStride * MHeight;
    MBuffer     = (uint32_t*)malloc(MBufferSize);
    MAllocated  = true;
    memcpy(MBuffer,data,MBufferSize);
    //convertRgbaBgra();
    swapLayer(0,2);
    stbi_image_free(data);
  }
  #endif

  //----------

  // Loads and decodes an image from a file (using stb_image)

  #ifndef SAT_NO_BITMAP_PNG_FILE
  SAT_Bitmap(const char* AFilename) {
    int x,y,n;
    unsigned char *data = stbi_load(AFilename, &x, &y, &n, 4 /*0*/); // 4 0 we want 32-bit
    MWidth  = x;
    MHeight = y;
    // returned n is bytes per pixel in image
    // but we'force' 32 bit
    /*
    switch(n) {
      case 0: MDepth = 0;  break;
      case 1: MDepth = 8;  break;
      case 2: MDepth = 16; break;
      case 3: MDepth = 24; break;
      case 4: MDepth = 32; break;
    }
    */
    MStride     = MWidth  * 4; //n;
    MBufferSize = MStride * MHeight;
    MBuffer     = (uint32_t*)malloc(MBufferSize);
    MAllocated  = true;
    memcpy(MBuffer,data,MBufferSize);
//    convertRgbaBgra();
    swapLayer(0,2);
    stbi_image_free(data);
  }
  #endif

  //----------

  virtual ~SAT_Bitmap() {
    if (MAllocated && MBuffer) free(MBuffer);
  }

//------------------------------
public: // paint src/tgt
//------------------------------

  /*
  bool      tgtIsBitmap()   override { return true; }
  int32_t   tgtGetWidth()   override { return MWidth; }
  int32_t   tgtGetHeight()  override { return MHeight; }
  int32_t   tgtGetDepth()   override { return MDepth; }
  int32_t   tgtGetStride()  override { return MStride; }
  uint32_t* tgtGetBuffer()  override { return MBuffer; }

  bool      srcIsBitmap()   override { return true; }
  int32_t   srcGetWidth()   override { return MWidth; }
  int32_t   srcGetHeight()  override { return MHeight; }
  int32_t   srcGetDepth()   override { return MDepth; }
  int32_t   srcGetStride()  override { return MStride; }
  uint32_t* srcGetBuffer()  override { return MBuffer; }
  */

//------------------------------
public:
//------------------------------

  virtual int32_t   getWidth()      { return MWidth; }
  virtual int32_t   getHeight()     { return MHeight; }
  virtual int32_t   getDepth()      { return MDepth; }
  virtual int32_t   getStride()     { return MStride; }
  virtual uint32_t* getBuffer()     { return MBuffer; }
  virtual uint32_t  getBufferSize() { return MBufferSize; }

  //uint32_t* getPixelPtr(uint32_t x, uint32_t y) {
  //  return MBuffer + (y * MStride) + x;
  //}

//------------------------------
public:
//------------------------------

  #ifdef SAT_USE_CAIRO

  /*
    cairo_image_surface_create_for_data()

    Creates an image surface for the provided pixel data. The output buffer
    must be kept around until the cairo_surface_t is destroyed or
    cairo_surface_finish() is called on the surface. The initial contents of
    data will be used as the initial image contents; you must explicitly clear
    the buffer, using, for example, cairo_rectangle() and cairo_fill() if you
    want it cleared.

    Note that the stride may be larger than width*bytes_per_pixel to provide
    proper alignment for each pixel and row. This alignment is required to
    allow high-performance rendering within cairo. The correct way to obtain a
    legal stride value is to call cairo_format_stride_for_width() with the
    desired format and maximum image width value, and then use the resulting
    stride value to allocate the data and to create the image surface. See
    cairo_format_stride_for_width() for example code.

    Returns a pointer to the newly created surface. The caller owns the surface
    and should call cairo_surface_destroy() when done with it.

    See cairo_surface_set_user_data() for a means of attaching a destroy-
    notification fallback to the surface if necessary.
  */

  cairo_surface_t* createCairoSurface() {
    cairo_surface_t* surface = cairo_image_surface_create_for_data(
      (uint8_t*)MBuffer,    // unsigned char *data,
      CAIRO_FORMAT_ARGB32,  // cairo_format_t format,
      MWidth,               // int width,
      MHeight,              // int height,
      MStride               // int stride);
    );
    return surface;
  }

  #endif

//------------------------------
public:
//------------------------------

  uint32_t* getLinePtr(uint32_t AYpos) {
    uint8_t* ptr = (uint8_t*)MBuffer + (MStride * AYpos);
    return (uint32_t*)ptr;
  }

  //----------

  uint32_t* getPixelPtr(int32_t X, int32_t Y) {
    uint32_t* ptr = getLinePtr(Y);
    ptr += X;
    return ptr;
  }

//------------------------------
public: // sub-bitmap
//------------------------------

  /*
    Creates a sub-bitmap that shares the same memory, but have a different
    'view' into it.. different size and offset.. for things like clipping,
    processing specific parts, etc..
  */

  SAT_Bitmap* createSubBitmap(int32_t x, int32_t y, int32_t w, int32_t h) {
    SAT_Bitmap* bitmap = new SAT_Bitmap();
    bitmap->MWidth      = w;
    bitmap->MHeight     = h;
    bitmap->MStride     = MStride;
    bitmap->MBufferSize = 0;
    bitmap->MBuffer     = getPixelPtr(x,y);
    bitmap->MAllocated  = false;
    return bitmap;
  }

  //----------

  /*
    Makes a sub-bitmap by filling in the member variables of the argument
    bitmap directly.. for speed, to avoid repeatedly allocating/freeing
    SAT_Bitmap classes if you need to repeatedly process parts of a bitmap.
    you can have a pool of sub-bitmaps, and call getSubBitmap to set them up..
  */

  SAT_Bitmap* getSubBitmap(SAT_Bitmap* ABitmap, int32_t x, int32_t y, int32_t w, int32_t h) {
    ABitmap->MWidth      = w;
    ABitmap->MHeight     = h;
    ABitmap->MStride     = MStride;
    ABitmap->MBufferSize = 0;
    ABitmap->MBuffer     = getPixelPtr(x,y);
    ABitmap->MAllocated  = false;
    return ABitmap;
  }

//------------------------------
public: // pixel
//------------------------------

  uint32_t getPixel(uint32_t x, uint32_t y) {
    if (x >= (uint32_t)MWidth) return 0;  // 0xff000000;
    if (y >= (uint32_t)MHeight) return 0; // 0xff000000;
    uint32_t* ptr = (uint32_t*)getPixelPtr(x,y);
    return *ptr;
  }

  //----------

  uint32_t getPixel_NoClip(uint32_t x, uint32_t y) {
    uint32_t* ptr = (uint32_t*)getPixelPtr(x,y);
    return *ptr;
  }

  //----------

  uint32_t getPixelWrap(uint32_t x, uint32_t y) {
    //if (x >= (uint32_t)MWidth) return 0;//x = MWidth - 1;
    //if (y >= (uint32_t)MHeight) return 0;//y = MHeight - 1;
    x %= MWidth;
    y %= MHeight;
    uint32_t* ptr = (uint32_t*)getPixelPtr(x,y);
    return *ptr;
  }

//------------------------------
public:
//------------------------------

  void setPixel(uint32_t x, uint32_t y, uint32_t c) {
    if (x>=(uint32_t)MWidth) return;
    if (y>=(uint32_t)MHeight) return;
    uint32_t* ptr = (uint32_t*)getPixelPtr(x,y);
    *ptr = c;
  }

  //----------

  void setPixel_NoClip(uint32_t x, uint32_t y, uint32_t c) {
    uint32_t* ptr = (uint32_t*)getPixelPtr(x,y);
    *ptr = c;
  }

  //----------

  #define _alpha(c,a) (((c * a) >> 8) & 0xff)

  void blendPixel(uint32_t x, uint32_t y, uint32_t c, uint8_t AAlpha) {
    if (x >= (uint32_t)MWidth) return;
    if (y >= (uint32_t)MHeight) return;
    uint8_t* ptr = (uint8_t*)getPixelPtr(x,y);
    uint8_t bb = ptr[0];
    uint8_t bg = ptr[1];
    uint8_t br = ptr[2];
    uint8_t cr = (c >> 16) & 0xff;
    uint8_t cg = (c >> 8 ) & 0xff;
    uint8_t cb = (c      ) & 0xff;
    *ptr++ = _alpha(cb,AAlpha) + _alpha(bb,(255-AAlpha));
    *ptr++ = _alpha(cg,AAlpha) + _alpha(bg,(255-AAlpha));
    *ptr++ = _alpha(cr,AAlpha) + _alpha(br,(255-AAlpha));
  }

  // blendPixel_NoClip ?

  #undef _alpha

  void blendPixelF(uint32_t x, uint32_t y, uint32_t c, float AAlpha) {
    uint8_t a = AAlpha = 255.0;
    blendPixel(x,y,c,a);
  }

//------------------------------
public:
//------------------------------

  //void clear() {
  //  for(uint32_t y=0; y<MHeight; y++) {
  //    uint32_t* ptr = getLinePtr(y);
  //    memset(ptr,0,MWidth*4);
  //  }
  //}

  //----------

  void fill(uint32_t AColor=0xff000000) {
    for(uint32_t y=0; y<MHeight; y++) {
      uint32_t* ptr = getLinePtr(y);
      for(uint32_t x=0; x<MWidth; x++) {
        *ptr++ = AColor;
      }
    }
  }

  //----------

  void convertRgbaToBgra(void) {
    swapLayer(0,2);
  }

  //----------

  //#define _alpha(c,a) (((c * a) >> 8) & 0xff)

  void premultAlpha(uint32_t AColor=0x000000) {
    if (MBuffer) {
      for(uint32_t y=0; y<MHeight; y++) {
        uint32_t* ptr = (uint32_t*)getLinePtr(y);
        for(uint32_t x=0; x<MWidth; x++) {
          //uint8_t b = ptr[0];
          //uint8_t g = ptr[1];
          //uint8_t r = ptr[2];
          //uint8_t a = ptr[3];
          //*ptr++ = _alpha(b,a);
          //*ptr++ = _alpha(g,a);
          //*ptr++ = _alpha(r,a);
          //*ptr++ = a;
          uint32_t c = *ptr;
          *ptr++ = SAT_RGBA_AlphaBlend(AColor,c);
        } //for x
      } //for y
    } //mBuffer
  }

  //#undef _alpha

  //----------

  /*
    matrix for color manipulation
    r = radd + r*rr + g*rg + b*rb + a*ra
    same for g, b, and a
    arguments from 0 to 1
  */

  #define _scale(c,n) SAT_MinI((uint8_t)255,uint8_t(n*(float)c))

  void colorMatrix( float radd, float rr, float rg, float rb, float ra,
                    float gadd, float gr, float gg, float gb, float ga,
                    float badd, float br, float bg, float bb, float ba,
                    float aadd, float ar, float ag, float ab, float aa) {
    if (MBuffer) {
      for (uint32_t y=0; y<MHeight; y++) {
        uint8_t* ptr = (uint8_t*)getLinePtr(y);
        for (uint32_t x=0; x<MWidth; x++) {
          uint8_t b = ptr[0];
          uint8_t g = ptr[1];
          uint8_t r = ptr[2];
          uint8_t a = ptr[3];
          *ptr++ = (badd * 255) + _scale(r,br) + _scale(g,bg) + _scale(b,bb) + _scale(a,ba);
          *ptr++ = (gadd * 255) + _scale(r,gr) + _scale(g,gg) + _scale(b,gb) + _scale(a,ga);
          *ptr++ = (radd * 255) + _scale(r,rr) + _scale(g,rg) + _scale(b,rb) + _scale(a,ra);
          *ptr++ = (aadd * 255) + _scale(r,ar) + _scale(g,ag) + _scale(b,ab) + _scale(a,aa);
        } //for x
      } //for y
    } //mBuffer
  }

  #undef _scale

//------------------------------
public:
//------------------------------

  uint8_t getLayerValue(uint32_t ch, uint32_t x, uint32_t y) {
    if (x >= (uint32_t)MWidth) return 0;
    if (y >= (uint32_t)MHeight) return 0;
    uint8_t* ptr = (uint8_t*)getPixelPtr(x,y);
    return ptr[ch];
  }

  //----------

  uint8_t getLayerValue_NoClip(uint32_t ch, uint32_t x, uint32_t y) {
    if (x >= (uint32_t)MWidth) return 0;
    if (y >= (uint32_t)MHeight) return 0;
    uint8_t* ptr = (uint8_t*)getPixelPtr(x,y);
    return ptr[ch];
  }

  //----------

  void setLayerValue(uint32_t ch, uint32_t x, uint32_t y, uint8_t v) {
    if (x >= (uint32_t)MWidth) return;//x = MWidth - 1;
    if (y >= (uint32_t)MHeight) return;//y = MHeight - 1;
    uint8_t* ptr = (uint8_t*)getPixelPtr(x,y);
    ptr[ch] = v;
  }

  //----------

  void setLayerValue_NoClip(uint32_t ch, uint32_t x, uint32_t y, uint8_t v) {
    uint8_t* ptr = (uint8_t*)getPixelPtr(x,y);
    ptr[ch] = v;
  }

  //----------

  void fillLayer(uint32_t ALayer, uint8_t AValue=0) {
    for (uint32_t y=0; y<MHeight; y++) {
      uint8_t* ptr = (uint8_t*)getLinePtr(y);
      ptr += ALayer;
      for (uint32_t x=0; x<MWidth; x++) {
        *ptr = AValue;
        ptr += 4;
      }
    }
  }

  //----------

  void swapLayer(uint32_t ALayer1, uint32_t ALayer2) {
    for (uint32_t y=0; y<MHeight; y++) {
      uint8_t* ptr = (uint8_t*)getLinePtr(y);
      for (uint32_t x=0; x<MWidth; x++) {
        uint8_t v1 = ptr[ALayer1];
        uint8_t v2 = ptr[ALayer2];
        ptr[ALayer1] =v2;
        ptr[ALayer2] =v1;
        ptr += 4;
      }
    }

  }

  //----------

  void copyLayer(uint32_t ADstLayer, uint32_t ASrcLayer) {
    for (uint32_t y=0; y<MHeight; y++) {
      uint8_t* ptr = (uint8_t*)getLinePtr(y);
      for (uint32_t x=0; x<MWidth; x++) {
        ptr[ADstLayer] = ptr[ASrcLayer];
        ptr += 4;
      }
    }
  }

  //----------

  void scaleLayer(uint32_t ALayer, uint8_t AScale) {
    for (uint32_t y=0; y<MHeight; y++) {
      uint8_t* ptr = (uint8_t*)getLinePtr(y);
      ptr += ALayer;
      for (uint32_t x=0; x<MWidth; x++) {
        uint8_t v = *ptr;
        v = (v*AScale) >> 8;
        *ptr = v;
        ptr += 4;
      }
    }
  }

  //----------

  void invertLayer(uint32_t ALayer) {
    for (uint32_t y=0; y<MHeight; y++) {
      uint8_t* ptr = (uint8_t*)getLinePtr(y);
      ptr += ALayer;
      for (uint32_t x=0; x<MWidth; x++) {
        uint8_t v = *ptr;
        *ptr = 255 - v;
        ptr += 4;
      }
    }
  }

//------------------------------
private:
//------------------------------

  /*
    calculates new clipped xpos and length
  */

  bool _clipLine(int32_t AXpos, int32_t ALength, int32_t* AXout, int32_t* ALout) {
    int32_t x = AXpos;
    int32_t l = ALength;
    int32_t w = (int32_t)MWidth;
    if ((x+l) <= 0) return false;
    if (x >= w) return false;
    if (x < 0) { l+=x; x=0; }
    if ((x+l) > w) {
      l = w - x;
    }
    if (l > 0) {
      *AXout = x;
      *ALout = l;
      return true;
    }
    else {
      *AXout = 0;
      *ALout = 0;
      return false;
    }
  }

  //----------

  /*
    'textured' line clipping..
    (for example, single line during bitmap copy)
  */

  bool _clipLine2(int32_t AXpos, int32_t ALength, int32_t AXsrc, int32_t* AXout, int32_t* ALout, int32_t* AX2out) {
    int32_t x  = AXpos;
    int32_t x2 = AXsrc;
    int32_t l  = ALength;
    int32_t w  = (int32_t)MWidth;
    if ((x+l) <= 0) return false;
    if (x >= w) return false;
    if (x < 0) {
      l  += x;
      x2 -= x;
      x   = 0;
    }
    if ((x + l) > w) {
      l = w - x;
    }
    if (l > 0) {
      *AXout  = x;
      *ALout  = l;
      *AX2out = x2;
      return true;
    }
    else {
      //*AXout  = 0;
      //*ALout  = 0;
      //*AX2out = 0;
      return false;
    }
  }

//------------------------------
public: // fill
//------------------------------

  void fillLine(int32_t AXpos, int32_t AYpos, int32_t ALength, uint32_t AColor) {
    if (AYpos < 0) return;
    if (AYpos >= (int32_t)MHeight) return;
    int32_t X,L;
    if (_clipLine(AXpos,ALength,&X,&L)) {
      uint32_t* dst = (uint32_t*)getPixelPtr(X,AYpos);
      for (int32_t x=0; x<L; x++) {
        *dst++ = AColor;
      }
    }
  }

  //----------

  void fillLine_NoClip(int32_t AXpos, int32_t AYpos, int32_t ALength, uint32_t AColor) {
    uint32_t* dst = (uint32_t*)getPixelPtr(AXpos,AYpos);
    for (int32_t x=0; x<ALength; x++) {
      *dst++ = AColor;
    }
  }

//------------------------------
public: // copy
//------------------------------

  void copyLine(int32_t AXpos, int32_t AYpos, int32_t ALength, SAT_Bitmap* ABitmap, int32_t AXsrc, int32_t AYsrc) {
    if (AYpos < 0) return;
    if (AYpos >= (int32_t)MHeight) return;
    if (AYsrc < 0) return;
    if (AYsrc >= (int32_t)ABitmap->MHeight) return;
    int32_t X,L,X2;
    if (_clipLine2(AXpos,ALength,AXsrc,&X,&L,&X2)) {
      uint32_t* dst = (uint32_t*)getPixelPtr(X,AYpos);
      uint32_t* src = (uint32_t*)ABitmap->getPixelPtr(X2,AYsrc);
      for (int32_t x=0; x<L; x++) {
        *dst++ = *src++;
      }
    }
  }

  //----------

  void copyLine_NoClip(int32_t AXpos, int32_t AYpos, int32_t ALength, SAT_Bitmap* ABitmap, int32_t AXsrc, int32_t AYsrc) {
    uint32_t* dst = (uint32_t*)getPixelPtr(AXpos,AYpos);
    uint32_t* src = (uint32_t*)ABitmap->getPixelPtr(AXsrc,AYsrc);
    for (int32_t x=0; x<ALength; x++) {
      *dst++ = *src++;
    }
  }

  //------------------------------

  /*
    todo:
    * the below is not very efficient, clipping is done for every line
    * clip src/dst rectangles first, then call these
  */

  //----------

  void fillArea(int32_t x1, int32_t y1, int32_t w, int32_t h, uint32_t color) {
    int32_t x2 = x1 + w - 1;
    int32_t y2 = y1 + h - 1;
    for (int32_t i=y1; i<=y2; i++) {
      fillLine(x1,i,(x2-x1+1),color);
    }
  }

  //----------

  void fillArea_NoClip(int32_t x1, int32_t y1, int32_t w, int32_t h, uint32_t color) {
    int32_t x2 = x1 + w - 1;
    int32_t y2 = y1 + h - 1;
    for (int32_t i=y1; i<=y2; i++) {
      fillLine_NoClip(x1,i,(x2-x1+1),color);
    }
  }

  //----------

  void copyBitmap(int32_t AXpos, int32_t AYpos, SAT_Bitmap* ABitmap) {
    for (uint32_t y=0; y<ABitmap->MHeight; y++) {
      copyLine(AXpos,AYpos+y,ABitmap->MWidth,ABitmap,0,y);
    }
  }

  //----------

  void copyBitmap_NoClip(int32_t AXpos, int32_t AYpos, SAT_Bitmap* ABitmap) {
    for (uint32_t y=0; y<ABitmap->MHeight; y++) {
      copyLine_NoClip(AXpos,AYpos+y,ABitmap->MWidth,ABitmap,0,y);
    }
  }

//------------------------------
public: // blend
//------------------------------

  void blendLine(int32_t AXpos, int32_t AYpos, int32_t ALength, SAT_Bitmap* ABitmap, int32_t AXsrc, int32_t AYsrc) {
    if (AYpos < 0) return;
    if (AYpos >= (int32_t)MHeight) return;
    if (AYsrc < 0) return;
    if (AYsrc >= (int32_t)ABitmap->MHeight) return;
    int32_t X,L,X2;
    if (_clipLine2(AXpos,ALength,AXsrc,&X,&L,&X2)) {
      uint32_t* dst = (uint32_t*)getPixelPtr(X,AYpos);
      uint32_t* src = (uint32_t*)ABitmap->getPixelPtr(X2,AYsrc);
      for (int32_t x=0; x<L; x++) {
        uint32_t  c1 = *dst;
        uint32_t  c2 = *src++;
        uint8_t   a  = c2 >> 24;
        uint32_t  c  = SAT_RGBA_AlphaBlend(c1,c2,a);
        *dst++ = c;
      }
    }
  }

  //----------

  void blendLine_NoClip(int32_t AXpos, int32_t AYpos, int32_t ALength, SAT_Bitmap* ABitmap, int32_t AXsrc, int32_t AYsrc) {
    uint32_t* dst = (uint32_t*)getPixelPtr(AXpos,AYpos);
    uint32_t* src = (uint32_t*)ABitmap->getPixelPtr(AXsrc,AYsrc);
    for (int32_t x=0; x<ALength; x++) {
      uint32_t  c1 = *dst;
      uint32_t  c2 = *src++;
      uint8_t   a  = c2 >> 24;
      uint32_t  c  = SAT_RGBA_AlphaBlend(c1,c2,a);
      *dst++ = c;
    }
  }

  //----------

  void blendLine(int32_t AXpos, int32_t AYpos, int32_t ALength, SAT_Bitmap* ABitmap, int32_t AXsrc, int32_t AYsrc, uint8_t AAlpha) {
    if (AYpos < 0) return;
    if (AYpos >= (int32_t)MHeight) return;
    if (AYsrc < 0) return;
    if (AYsrc >= (int32_t)ABitmap->MHeight) return;
    int32_t X,L,X2;
    if (_clipLine2(AXpos,ALength,AXsrc,&X,&L,&X2)) {
      uint32_t* dst = (uint32_t*)getPixelPtr(X,AYpos);
      uint32_t* src = (uint32_t*)ABitmap->getPixelPtr(X2,AYsrc);
      for (int32_t x=0; x<L; x++) {
        uint32_t  c1 = *dst;
        uint32_t  c2 = *src++;
        uint32_t  c  = SAT_RGBA_AlphaBlend(c1,c2,AAlpha);
        *dst++ = c;
      }
    }
  }

  //----------

  void blendLine_NoClip(int32_t AXpos, int32_t AYpos, int32_t ALength, SAT_Bitmap* ABitmap, int32_t AXsrc, int32_t AYsrc, uint8_t AAlpha) {
    uint32_t* dst = (uint32_t*)getPixelPtr(AXpos,AYpos);
    uint32_t* src = (uint32_t*)ABitmap->getPixelPtr(AXsrc,AYsrc);
    for (int32_t x=0; x<ALength; x++) {
      uint32_t  c1 = *dst;
      uint32_t  c2 = *src++;
      uint32_t  c  = SAT_RGBA_AlphaBlend(c1,c2,AAlpha);
      *dst++ = c;
    }
  }

  //------------------------------

  /*
    blends another bitmap on top of this bitmap
    alpha taken from ABitmap
      clipping is done for every line
      todo: clip src/dst rectangles first, then call these
  */

  void blendBitmap(int32_t AXpos, int32_t AYpos, SAT_Bitmap* ABitmap) {
    for (uint32_t y=0; y<ABitmap->MHeight; y++) {
      blendLine(AXpos,AYpos+y,ABitmap->MWidth,ABitmap,0,y);
    }
  }

  //----------

  void blendBitmap_NoClip(int32_t AXpos, int32_t AYpos, SAT_Bitmap* ABitmap) {
    for (uint32_t y=0; y<ABitmap->MHeight; y++) {
      blendLine_NoClip(AXpos,AYpos+y,ABitmap->MWidth,ABitmap,0,y);
    }
  }

  //----------

  // blends another bitmap on top of this bitmap
  // AAlpha = transparency (for entire bitmap)

  void blendBitmap(int32_t AXpos, int32_t AYpos, SAT_Bitmap* ABitmap, uint8_t AAlpha) {
    for (uint32_t y=0; y<ABitmap->MHeight; y++) {
      blendLine(AXpos,AYpos+y,ABitmap->MWidth,ABitmap,0,y,AAlpha);
    }
  }

  //----------

  void blendBitmap_NoClip(int32_t AXpos, int32_t AYpos, SAT_Bitmap* ABitmap, uint8_t AAlpha) {
    for (uint32_t y=0; y<ABitmap->MHeight; y++) {
      blendLine_NoClip(AXpos,AYpos+y,ABitmap->MWidth,ABitmap,0,y,AAlpha);
    }
  }

//------------------------------
public:
//------------------------------

  // https://hornet.org/code/effects/rotozoom/

  void rotozoom(SAT_Bitmap* ASrcBitmap, float AAngle, float AZoom, float ADstX=0, float ADstY=0, float ASrcX=0, float ASrcY=0, bool AWrap=false) {
    if (AZoom <= 0.0f) return;
    float zoom = 1.0f / AZoom;
    float ca = cosf(AAngle) * zoom;
    float sa = sinf(AAngle) * zoom;
    for (int32_t y=0; y<(int32_t)MHeight; y++) {
      uint32_t* ptr = (uint32_t*)getLinePtr(y);
      for (int32_t x=0; x<(int32_t)MWidth; x++) {
        float xc = (float)(x - ADstX - (int32_t)(MWidth / 2));
        float yc = (float)(y - ADstY - (int32_t)(MHeight / 2));
        float tx = ( (xc * ca) - (yc * sa) ) + (float)(ASrcBitmap->MWidth / 2) + ASrcX;
        float ty = ( (xc * sa) + (yc * ca) ) + (float)(ASrcBitmap->MHeight / 2) + ASrcY;
        uint32_t pixel;
        if (AWrap) {
          pixel = ASrcBitmap->getPixelWrap(tx,ty);
        }
        else {
          pixel = ASrcBitmap->getPixel(tx,ty);
        }
        *ptr++ = pixel;
      }
    }
  }

  //----------

  // http://www.drdobbs.com/architecture-and-design/fast-bitmap-rotation-and-scaling/184416337

  /*
    src           = source bitmap (to rotate/scale)
    dstCX, dstCY  = src bitmap rotation center
    srcCX, srcCY  = position of center in dst bitmap
    angle         = rotation angle (0..PI2, counter-clockwise, 0 = right
    scale         = scaling
    col           = color to fill empty areas
  */

  void rotate(SAT_Bitmap* src, float dstCX, float dstCY, float srcCX, float srcCY, float angle, float scale, uint32_t col=0xff000000) {
    float duCol = (float)sin(-angle) * (1.0f / scale);
    float dvCol = (float)cos(-angle) * (1.0f / scale);
    float duRow = dvCol;
    float dvRow = -duCol;
    float startingu = srcCX - (dstCX * dvCol + dstCY * duCol);
    float startingv = srcCY - (dstCX * dvRow + dstCY * duRow);
    float rowu = startingu;
    float rowv = startingv;
    for (uint32_t y=0; y<MHeight; y++) {
      float u = rowu;
      float v = rowv;
      uint32_t *pDst = (uint32_t*)getLinePtr(y);
      for (uint32_t x=0; x<MWidth ; x++) {
        //todo:
        //- mode: outside: none, color, texwrap
        //- blend with background
        if (u>0 && v>0 && u<src->getWidth() && v<src->getHeight()) {
          uint32_t *pSrc = (uint32_t*)src->getPixelPtr(u,v);
          *pDst++ = *pSrc++; // blend?
        }
        else {
          *pDst++ = col; // blend?
        }
        u += duRow;
        v += dvRow;
      }
      rowu += duCol;
      rowv += dvCol;
    }
  }

  //----------

  //void setPaletteEntry(uint32_t AIndex, uint32_t AColor) {
  //  MPalette[AIndex] = AColor;
  //}

  //void setPaletteEntry(uint32_t AIndex, SAT_Color AColor) {
  //  MPalette[AIndex] = SAT_RGBA(AColor);
  //}

  // todo: clipping

  void convertFrom8bit(uint8_t* ABuffer, uint32_t* APalette, uint32_t AWidth, uint32_t AHeight) {
    for (uint32_t y=0; y<AHeight; y++) {
      uint32_t* dst = (uint32_t*)getLinePtr(y);
      uint8_t* src = ABuffer + (y * AWidth);
      for (uint32_t x=0; x<AWidth; x++) {
        uint8_t c = *src++;
        *dst++ = APalette[c];
      }
    }
  }

  //----------

  // todo: clip

  void drawMask_NoClip(int32_t AXpos, int32_t AYpos, uint8_t* ABuffer, uint32_t AWidth, uint32_t AHeight, uint32_t AColor) {
    uint8_t a;
    uint8_t* ptr = ABuffer;
    //int32_t x,l;
    //if (_clipLine(AXpos,AWidth,&x,&l) ) {
      for (uint32_t y=0; y<AHeight; y++) {
        for (uint32_t x=0; x<AWidth; x++) {
          a = *ptr++;
          blendPixel(AXpos+x,AYpos+y,AColor,a);
        }
      }
    //}
  }

//------------------------------
public:
//------------------------------

//  #ifndef SAT_NO_BITMAP_DRAW
//    #include "gfx/sat_bitmap_draw.h"
//  #endif
//
//  #ifndef SAT_NO_BITMAP_FONT
//    #include "gfx/sat_bitmap_font.h"
//  #endif
//
//  #ifndef SAT_NO_BITMAP_PROC
//    #include "gfx/sat_bitmap_proc.h"
//  #endif
//
//  #ifndef SAT_NO_BITMAP_GEN
//    #include "gfx/sat_bitmap_gen.h"
//  #endif

//------------------------------

};

//----------------------------------------------------------------------
#endif
