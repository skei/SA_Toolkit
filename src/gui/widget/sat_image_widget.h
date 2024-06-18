#ifndef sat_image_widget_included
#define sat_image_widget_included
//----------------------------------------------------------------------

//TODO: cleanup

#include "sat.h"
#include "gui/widget/sat_visual_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_ImageWidget
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

//------------------------------
public:
//------------------------------

  SAT_ImageWidget(SAT_Rect ARect, void* ABuffer, uint32_t ASize)
  : SAT_VisualWidget(ARect) {
    setName("SAT_ImageWidget");
    setHint("SAT_ImageWidget");
    setFillBackground(false);
    MBuffer = ABuffer;
    MBufferSize = ASize;
  }

  //----------

  SAT_ImageWidget(SAT_Rect ARect, const char* AFilename)
  : SAT_VisualWidget(ARect) {
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
  virtual void setScale(double AScale)                  { MScale = AScale; }

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

  void on_widget_close(SAT_WidgetOwner* AOwner) override {
    if (MInitialized) {
      //SAT_Window* window = AOwner->on_widgetOwner_getWindow();
      //SAT_Painter* painter = window->getPainter();
      SAT_Painter* painter = AOwner->on_widgetOwner_getPainter();
      painter->deleteImage(MImage);
      MInitialized = false;
    }
  }

  //----------

  void on_widget_paint(SAT_PaintContext* AContext) override {
    drawDropShadow(AContext);
    fillBackground(AContext);
    drawImage(AContext);
    paintChildren(AContext);
    drawBorder(AContext);
  }

};

//----------------------------------------------------------------------
#endif


