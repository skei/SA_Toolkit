#ifndef sat_image_strip_widget_included
#define sat_image_strip_widget_included
//----------------------------------------------------------------------

#include "gui/sat_bitmap.h"
#include "gui/widget/sat_drag_value_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_ImageStripWidget
: public SAT_DragValueWidget {

//------------------------------
protected:
//------------------------------

  bool          MDrawTile     = true;

  uint32_t      MTileCount    = 1;
  uint32_t      MTileXCount   = 1;
  uint32_t      MTileYCount   = 1;
  float         MTileWidth    = 0.0;
  float         MTileHeight   = 0.0;
  uint32_t      MTile         = 0;

  int           MImage        = -1;
  SAT_Bitmap*   MBitmap       = nullptr;
  uint32_t      MImageWidth   = 0;
  uint32_t      MImageHeight  = 0;
  uint32_t*     MImageBuffer  = nullptr;
  bool          MImageValid   = false;

//------------------------------
public:
//------------------------------

  SAT_ImageStripWidget(SAT_Rect ARect, SAT_Bitmap* ABitmap, const char* AText="", double AValue=0.0)
  : SAT_DragValueWidget(ARect,AText,AValue) {
    setName("SAT_ImageStripWidget");
    setHint("SAT_ImageStripWidget");
    MBitmap = ABitmap;
    MImageWidth = MBitmap->getWidth();
    MImageHeight = MBitmap->getHeight();
    MImageBuffer = MBitmap->getBuffer();
  }

  //----------

  virtual ~SAT_ImageStripWidget() {
    #ifndef SAT_NO_AUTODELETE
      if (MBitmap) delete MBitmap;
    #endif
  }

//------------------------------
public:
//------------------------------

  // virtual void setupImage(uint32_t* buffer, uint32_t width, uint32_t height) {
  //   MImageBuffer = buffer;
  //   MImageWidth  = width;
  //   MImageHeight = height;
  //   MImageValid  = false;
  // }

  //----------

  // virtual void setupImage(SAT_Bitmap* ABitmap) {
  //   MImageBuffer = ABitmap->getBuffer();
  //   MImageWidth  = ABitmap->getWidth();
  //   MImageHeight = ABitmap->getHeight();
  //   MImageValid  = false;
  // }

  //----------

  // virtual void validateImage(SAT_Painter* APainter) {
  //   if (!MImageValid) {
  //     MImage = APainter->loadImage(MImageWidth,MImageHeight,MImageBuffer);
  //     //APainter->updateImage(MImage,MImageBuffer);
  //     MImageValid = true;
  //   }
  // }

  //----------

  // virtual void invalidateImage() {
  //   MImageValid = false;
  // }

//------------------------------
public:
//------------------------------

  virtual void setupTiles(uint32_t AXcount, uint32_t AYcount) {
    MTileXCount   = AXcount;
    MTileYCount   = AYcount;
    MTileCount    = AXcount * AYcount;
    MTileWidth    = (float)MImageWidth / AXcount;
    MTileHeight   = (float)MImageHeight / AYcount;
  }

  //----------

  virtual SAT_Rect getTileRect(uint32_t AIndex) {
    float x = /*MImageRect.x*/ 0  + (floorf(AIndex % MTileXCount) * MTileWidth);
    float y = /*MImageRect.y*/ 0  + (floorf(AIndex / MTileXCount) * MTileHeight);
    float w = MTileWidth;
    float h = MTileHeight;
    return SAT_Rect(x,y,w,h);
  }

  //----------

  virtual void drawTile(SAT_PaintContext* AContext) {
    if (MDrawTile) {
      SAT_Painter* painter = AContext->painter;
      SAT_Rect rect = getRect();
      double scale = getWindowScale();
      //validateImage(painter);

      if (!MImageValid && MBitmap) {
        //SAT_PRINT("loading image\n");
        MImage = painter->loadImage(MImageWidth,MImageHeight,MImageBuffer);
        MImageValid = true;
      }

      double index = getValue() * (double)(MTileCount - 1);
      uint32_t iindex = SAT_Trunc(index);

      SAT_Rect tilerect = getTileRect(iindex);
      //SAT_PRINT("tilerect x %.2f y %.2f w %.2f h %.2f\n",tilerect.x,tilerect.y,tilerect.w,tilerect.h);

      float xscale = rect.w / MTileWidth;
      float yscale = rect.h / MTileHeight;
      float ox = rect.x - (tilerect.x * xscale);
      float oy = rect.y - (tilerect.y * yscale);
      float ex = MImageWidth * xscale;
      float ey = MImageHeight * yscale;

//      SAT_PRINT("iindex %i ox %.2f oy %.2f ex %.2f ey %.2f\n",iindex,ox,oy,ex,ey);

//      SAT_PaintSource paint = painter->imagePattern(ox,oy,ex,ey,0,MImage,1.0);
//      painter->beginPath();
//      painter->rect(rect.x,rect.y,rect.w,rect.h);
//      painter->fillPaint(paint);
//      painter->fill();

      //painter->drawBitmap(rect.x,rect.y,MBitmap,tilerect);
      painter->setFillImage(MImage,ox,oy,scale,scale,1.0,0.0);
      painter->fillRect(rect.x,rect.y,rect.w,rect.h);      


    }
  }

//------------------------------
public: // parent to child
//------------------------------

  void on_widget_paint(SAT_PaintContext* AContext) override {
    //fillBackground(AContext);
    //paintChildWidgets(AContext);
    drawTile(AContext);
    //drawBorder(AContext);
  }

  //----------

  void on_widget_open(SAT_WidgetOwner* AOwner) override {
    //MImageValid = false;
    //invalidateImage();
    SAT_DragValueWidget::on_widget_open(AOwner);
  }

  //----------

  void on_widget_close(SAT_WidgetOwner* AOwner) override {
    if (MImageValid) {
      SAT_Painter* painter = AOwner->on_widgetOwner_getPainter();
      painter->deleteImage(MImage);
      MImageValid = false;
    }
    SAT_DragValueWidget::on_widget_close(AOwner);
  }

};

//----------------------------------------------------------------------
#endif
















#if 0

// deprecated?
// use MIP_ImageWidget and the tiling features instead

#include "gui/widgets/mip_drag_value_widget.h"

MIP_Surface*    MSurface           = nullptr;
//bool            MImageSurfaceAllocated  = false;
//uint32_t        MTileCount              = 0;
uint32_t        MTileXCount             = 0;
uint32_t        MTileYCount             = 0;
uint32_t        MTileWidth              = 0;
uint32_t        MTileHeight             = 0;


//----------

class MIP_ImageStripWidget
: public MIP_DragValueWidget {

//------------------------------
public:
//------------------------------

  MIP_ImageStripWidget(MIP_FRect ARect)
  : MIP_DragValueWidget(ARect) {
    setName("MIP_ImageStripWidget");
    setHint("imagestrip");
    setFillBackground(false);
    setDrawBorder(false);
  }

//------------------------------
public:
//------------------------------

//  virtual void setImage(MIP_Drawable* ATarget, MIP_Surface* ASurface) {
//    MImageSurface = ASurface;
//    MImageSurfaceAllocated = false;
//  }
//
//  virtual void setImage(MIP_Drawable* ATarget, MIP_Bitmap* ABitmap) {
//    MImageSurface = new MIP_Surface(ATarget,ABitmap->getWidth(),ABitmap->getHeight());
//    MImageSurfaceAllocated = true;
//    MIP_Painter* painter = new MIP_Painter(MImageSurface);
//    painter->drawBitmap(0,0,ABitmap);
//    //painter->flush();
//    delete painter;
//  }
//
//  virtual void setImage(MIP_Drawable* ATarget, const uint8_t* ABuffer, uint32_t ASize, MIP_Color ABackground) {
//    MIP_Bitmap* bitmap = new MIP_Bitmap(ABuffer,ASize);
//    bitmap->premultAlpha( (uint32_t)ABackground );
//    setImage(ATarget,bitmap);
//    delete bitmap;
//  }
//
//  virtual void setImage(MIP_Drawable* ATarget, const char* AFilename, MIP_Color ABackground) {
//    MIP_Bitmap* bitmap = new MIP_Bitmap(AFilename);
//    bitmap->premultAlpha( (uint32_t)ABackground );
//    setImage(ATarget,bitmap);
//    delete bitmap;
//  }

  //----------

  virtual void setup(uint32_t AXcount, uint32_t AYcount, MIP_Surface* ASurface=nullptr) {
    if (ASurface) MSurface = ASurface;
    MTileXCount = AXcount;
    MTileYCount = AYcount;
    MTileWidth  = MSurface->getWidth() / AXcount;
    MTileHeight = MSurface->getHeight() / AYcount;
  }

  MIP_FRect getTileRect(uint32_t AIndex) {
    float x = floorf(AIndex % MTileXCount) * MTileWidth;
    float y = floorf(AIndex / MTileXCount) * MTileHeight;
    float w = MTileWidth - 1;
    float h = MTileHeight - 1;
    return MIP_FRect(x,y,w,h);
  }

  virtual void drawTile(MIP_Painter* APainter) {
    MIP_FRect mrect = getRect();
    //MIP_Surface* surface = getImageSurface();
    if (MSurface) {
      uint32_t num_tiles = MTileXCount * MTileYCount;
      if (num_tiles > 0) {
        float v = (float)getValue() * (float)num_tiles;
        uint32_t tile = MIP_MinI( num_tiles - 1, floorf(v) );
        MIP_FRect rect = getTileRect(tile);
        APainter->drawImage(mrect.x,mrect.y,MSurface,rect);
      }
    }
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode) override {
    fillBackground(APainter,ARect,AMode);
    drawTile(APainter);
    drawBorder(APainter,ARect,AMode);
  }

};

#endif // 0