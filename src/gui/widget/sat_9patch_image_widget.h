#ifndef sat_9patch_image_widget_included
#define sat_9patch_image_widget_included
//----------------------------------------------------------------------

//TODO: cleanup

#include "sat.h"
#include "gui/widget/sat_visual_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_9PatchImageWidget
: public SAT_VisualWidget {

//------------------------------
protected:
//------------------------------

  bool        MInitialized    = false;
  void*       MBuffer         = nullptr;
  uint32_t    MBufferSize     = 0;
  const char* MFilename       = nullptr;

  int32_t     MImage          = -1;

  bool        MDrawImage      = true;
  SAT_Rect    MImageOffset    = SAT_Rect(0);
  double      MAlpha          = 1.0;
  double      MAngle          = 0.0;
  double      MScale          = 1.0;
  bool        MMipMapped      = false;

  SAT_Rect  MEdgeOffset     = {0,0,0,0};
  bool      MFillCenter     = true;
  bool      MStretchCenter  = true;
  bool      MStretchEdges   = true;

//------------------------------
public:
//------------------------------

  SAT_9PatchImageWidget(SAT_Rect ARect, void* ABuffer, uint32_t ASize)
  : SAT_VisualWidget(ARect) {
    setName("SAT_9PatchImageWidget");
    setHint("SAT_9PatchImageWidget");
    setFillBackground(false);
    MBuffer = ABuffer;
    MBufferSize = ASize;
  }

  //----------

  SAT_9PatchImageWidget(SAT_Rect ARect, const char* AFilename)
  : SAT_VisualWidget(ARect) {
    setFillBackground(false);
    MFilename = AFilename;
  }

  //----------

  virtual ~SAT_9PatchImageWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setDrawImage(bool ADraw=true)            { MDrawImage = ADraw; }
  virtual void setImage(char* ABuffer, uint32_t ASize)  { MBuffer = ABuffer; MBufferSize = ASize; }
  virtual void setImageOffset(SAT_Rect AOffset)         { MImageOffset = AOffset; }
  virtual void setAlpha(double AAlpha)                  { MAlpha = AAlpha; }
  virtual void setAngle(double AAngle)                  { MAlpha = AAngle; }
  virtual void setScale(double AScale)                  { MScale = AScale; }

  virtual void setEdgeOffset(SAT_Rect ARect) { MEdgeOffset = ARect; }
  virtual void setfillCenter(bool AFill=true) {  MFillCenter = AFill; }
  virtual void setStretchCenter(bool AStretch=true) {  MStretchCenter = AStretch; }
  virtual void setStretchEdges(bool AStretch=true) { MStretchEdges = AStretch; }

//------------------------------
public:
//------------------------------

  virtual void drawImage(SAT_PaintContext* AContext) {
    if (MDrawImage) {
      double S = getWindowScale();
      S *= MScale;
      SAT_Painter* painter = AContext->painter;
      SAT_Rect mrect = getRect();

      if (!MInitialized) {
        if (MBuffer && (MBufferSize > 0)) {
          MImage = painter->loadImage(MBuffer,MBufferSize);
          MInitialized = true;
        }
        else if (MFilename) {
          MImage = painter->loadImage(MFilename);
          MInitialized = true;
        }
      }

      double src_w1 = MEdgeOffset.left * S;
      double src_w3 = MEdgeOffset.right * S;

      double src_h1 = MEdgeOffset.top * S;
      double src_h3 = MEdgeOffset.bottom * S;

      //double dst_w2 = mrect.w - src_w1 - src_w3;
      //double dst_h2 = mrect.h - src_h1 - src_h3;

      double dst_x1 = mrect.x;
      double dst_x2 = mrect.x + src_w1;
      double dst_x3 = mrect.x2() - src_w3;
      double dst_x4 = mrect.x2();

      double dst_y1 = mrect.y;
      double dst_y2 = mrect.y + src_h1;
      double dst_y3 = mrect.y2() - src_h3;
      double dst_y4 = mrect.y2();

      // upper left
      painter->setFillImage(MImage,mrect.x,mrect.y,S,S,MAlpha,MAngle);
      painter->fillRect(dst_x1,dst_y1,src_w1,src_h1);

      // upper right
      painter->setFillImage(MImage,mrect.x,mrect.y,S,S,MAlpha,MAngle);
      painter->fillRect(dst_x3,dst_y1,src_w3,src_h1);

      // lower left
      painter->setFillImage(MImage,mrect.x,mrect.y,S,S,MAlpha,MAngle);
      painter->fillRect(dst_x1,dst_y3,src_w1,src_h3);

      // lower right
      painter->setFillImage(MImage,mrect.x,mrect.y,S,S,MAlpha,MAngle);
      painter->fillRect(dst_x3,dst_y3,src_w3,src_h3);

      // if (MMipMapped) {
      // }
      // else {
      //   painter->setFillImage(MImage,mrect.x,mrect.y,S,S,MAlpha,MAngle);
      //   painter->fillRect(mrect.x,mrect.y,mrect.w,mrect.h);
      // }

    }
  }

//------------------------------
public:
//------------------------------

  void on_Widget_close(SAT_WidgetOwner* AOwner) override {
    if (MInitialized) {
      //SAT_Window* window = AOwner->on_widgetOwner_getWindow();
      //SAT_Painter* painter = window->getPainter();
      SAT_Painter* painter = AOwner->on_WidgetOwner_getPainter();
      painter->deleteImage(MImage);
      MInitialized = false;
    }
  }

  //----------

  void on_Widget_paint(SAT_PaintContext* AContext) override {
    drawDropShadow(AContext);
    fillBackground(AContext);
    drawImage(AContext);
    paintChildren(AContext);
    drawBorder(AContext);
  }

};


//----------------------------------------------------------------------
#endif

