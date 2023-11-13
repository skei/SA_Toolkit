#ifndef sat_image_widget_included
#define sat_image_widget_included
//----------------------------------------------------------------------

//TODO: cleanup

#include "base/sat.h"
#include "gui/widgets/sat_panel_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_ImageWidget
: public SAT_PanelWidget {

//------------------------------
protected:
//------------------------------

  bool        MInitialized    = false;
  bool        MDrawImage      = true;
  SAT_Rect    MImageOffset    = SAT_Rect(0);
  int32_t     MImage          = -1;
  void*       MBuffer         = nullptr;
  uint32_t    MBufferSize     = 0;
  const char* MFilename       = nullptr;
  double      MAlpha          = 1.0;
  double      MAngle          = 0.0;
  bool        MMipMapped      = false;

//------------------------------
public:
//------------------------------

  SAT_ImageWidget(SAT_Rect ARect, void* ABuffer, uint32_t ASize)
  : SAT_PanelWidget(ARect) {
    setName("SAT_ImageWidget");
    setFillBackground(false);
    MBuffer = ABuffer;
    MBufferSize = ASize;
  }

  //----------

  SAT_ImageWidget(SAT_Rect ARect, const char* AFilename)
  : SAT_PanelWidget(ARect) {
    setFillBackground(false);
    MFilename = AFilename;
  }

  //----------

  virtual ~SAT_ImageWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setDrawImage(bool ADraw=true)            { MDrawImage = ADraw; }
  virtual void setImage(char* ABuffer, uint32_t ASize)  { MBuffer = ABuffer; MBufferSize = ASize; }
  virtual void setImageOffset(SAT_Rect AOffset)         { MImageOffset = AOffset; }
  virtual void setAlpha(double AAlpha)                  { MAlpha = AAlpha; }
  virtual void setAngle(double AAngle)                  { MAlpha = AAngle; }

//------------------------------
public:
//------------------------------


//  void cleanup(SAT_Painter* APainter, bool ARecursive=true) override {
//    if (MInitialized) {
//      APainter->deleteImage(MImage);
//      MInitialized = false;
//    }
//    SAT_PanelWidget::cleanup(APainter,ARecursive);
//  }

//------------------------------
public:
//------------------------------

  virtual void drawImage(SAT_PaintContext* AContext) {
    if (MDrawImage) {
      double S = getWindowScale();
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

      if (MMipMapped) {
      }
      else {
        painter->setFillImage(MImage,mrect.x,mrect.y,S,S,MAlpha,MAngle);
        painter->fillRect(mrect.x,mrect.y,mrect.w,mrect.h);
      }
    }
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(SAT_PaintContext* AContext) override {
    fillBackground(AContext);
    drawImage(AContext);
    paintChildWidgets(AContext);
    drawBorder(AContext);
  }

};

//----------------------------------------------------------------------
#endif



