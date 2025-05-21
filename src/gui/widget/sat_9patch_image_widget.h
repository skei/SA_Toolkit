#ifndef sat_9patch_image_widget_included
#define sat_9patch_image_widget_included
//----------------------------------------------------------------------

      /*

          x1   x2         x3
           ____________________
      y1  |    |          |    |     |
          |____|__________|____|     | h1
      y2  |    |          |    |    |
          |    |          |    |    | h2
          |    |          |    |    |
          |____|__________|____|    |
      y3  |    |          |    |     | h3
          |____|__________|____|     |

          _____ __________ _____ 
            w1      w2      w3

      */

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

      // initialize image if needed

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

      // get image size
      int iw,ih;
      painter->imageSize(MImage,&iw,&ih);
      double image_width = (double)iw;
      double image_height = (double)ih;

      double left_edge    = MEdgeOffset.left;
      double right_edge   = MEdgeOffset.right;
      double top_edge     = MEdgeOffset.top;
      double bottom_edge  = MEdgeOffset.bottom;

      double center_width  = (image_width  - left_edge - right_edge);
      double center_height = (image_height - top_edge  - bottom_edge);

      double xcenter = mrect.w - ((left_edge + right_edge)  * S);
      double ycenter = mrect.h - ((top_edge  + bottom_edge) * S);

      double centerxscale = xcenter / center_width;
      double centeryscale = ycenter / center_height;

      double center_width_scaled  = center_width  * centerxscale;
      double center_height_scaled = center_height * centeryscale;

      SAT_Rect src,dst;

      // left

      src = SAT_Rect( 0,       0,       left_edge,     image_height );
      dst = SAT_Rect( mrect.x, mrect.y, left_edge * S, mrect.h      );
      painter->drawImage(dst,MImage,src);

      // center

      src = SAT_Rect( left_edge,                 0,       center_width,        image_height );
      dst = SAT_Rect( mrect.x + (left_edge * S), mrect.y, center_width_scaled, mrect.h      );
      painter->drawImage(dst,MImage,src);

      // right

      src = SAT_Rect( image_width - right_edge,      0,          right_edge,     image_height );
      dst = SAT_Rect( mrect.x2() - (right_edge * S), mrect.y,    right_edge * S, mrect.h      );
      painter->drawImage(dst,MImage,src);

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

