#ifndef sat_text_box_widget_included
#define sat_text_box_widget_included
//----------------------------------------------------------------------

// untested

#include "gui/widget/sat_scroll_box_widget.h"

//----------------------------------------------------------------------

class SAT_TextBoxWidget
: public SAT_ScrollBoxWidget {

//------------------------------
protected:
//------------------------------

  SAT_Painter*  MPainter        = nullptr;
  float         MTextSize       = 12.0;
  uint32_t      MTextAlignment  = SAT_TEXT_ALIGN_LEFT;
  uint32_t      MNumLines       = 0;
  uint32_t      MMaxLines       = 100;

//------------------------------
public:
//------------------------------

  SAT_TextBoxWidget(SAT_Rect ARect)
  : SAT_ScrollBoxWidget(ARect,true,true) {
    setName("SAT_TextBoxWidget");
    setHint("SAT_TextBoxWidget");
    //MContent->layout.innerBorder = 0;
    MContent->setDrawBorder(true);
    getContentWidget()->Layout.spacing = {0,3};

  }

  //----------

  //virtual ~SAT_TextBoxWidget() {
  //  #ifndef SAT_NO_AUTODELETE
  //  deleteLines();
  //  #endif
  //}

//------------------------------
public:
//------------------------------

  virtual void setMaxLines(uint32_t ANum)   { MMaxLines = ANum; }
  virtual void setTextSize(double ASize)    { MTextSize = ASize; }
//virtual void setTextHeight(float ASize)   { MTextHeight = ASize; }

//------------------------------
public:
//------------------------------

  virtual void appendLine(const char* AText, bool ARedraw=false) {
    if (MNumLines >= MMaxLines) {
      removeOldestLine();
    }
    SAT_TextWidget* textwidget = new SAT_TextWidget( SAT_Rect(MTextSize));
    MContent->appendChild(textwidget);
    MNumLines += 1;
    textwidget->setText(AText);
    textwidget->Layout.flags  = SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
    textwidget->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;
    textwidget->setFillBackground(false);
    textwidget->setDrawBorder(false);
    textwidget->setTextAlignment(MTextAlignment);
    textwidget->setTextColor(0);
    textwidget->setTextSize(MTextSize);
  }

  //----------

  void removeOldestLine() {
    if (MNumLines > 0) {
      SAT_WidgetArray* content = MContent->getChildren();
      SAT_Widget* wdg = MContent->getChild(0);
      content->remove((uint32_t)0);
      delete wdg;
      MNumLines -= 1;
    }
  }

  //----------

  void calcLineWidths(SAT_Painter* APainter, double AScale=1.0) {
    uint32_t num_children = MContent->getNumChildren();
    for (uint32_t i=0; i<num_children; i++) {
      SAT_TextWidget* textwidget = (SAT_TextWidget*)MContent->getChild(i);
      SAT_Rect rect = textwidget->getRect();
      const char* text = textwidget->getText();
      double bounds[4];
      APainter->setTextSize(MTextSize * AScale);
      double advance = APainter->getTextBounds(text,bounds);
      //SAT_PRINT("bounds %.2f, %.2f, %.2f, %.2f\n",bounds[0],bounds[1],bounds[2],bounds[3]);
      double width = bounds[2] - bounds[0];
      rect.x = 0;
      rect.y = 0;
      rect.w = width;
      rect.h = MTextSize;
      //SAT_PRINT("%s = %f (%.2f, %.2f, %.2f, %.2f)\n",text,width,rect.x,rect.y,rect.w,rect.h);
      textwidget->setRectAndBase(rect);
      //textwidget->setWidth(width);
    }
  }

//------------------------------
public:
//------------------------------

  void on_widget_open(SAT_WidgetOwner* AOwner) override {
    //SAT_TRACE;
    MPainter = AOwner->on_widgetOwner_getPainter();
  }
    
  //----------

  SAT_Rect on_widget_preAlign(SAT_Rect ARect) override {
    if (MPainter) {
      double scale = getWindowScale();
      //SAT_PRINT("MPainter %p scale %f\n",MPainter,scale);
      calcLineWidths(MPainter,scale);
    }
    return ARect;
  }

  //----------

  // SAT_Rect on_widget_postAlign(SAT_Rect ARect) override {
  //   return ARect;
  // }

  //----------

};

//----------------------------------------------------------------------
#endif
  






#if 0

  /*
    text might have been appended to the textbox during creation,
    before we opened the editor (and thus, before we have a painter
    so we can measure the string length)..
    so we go throgh them all here, and readjust their widths..
  */

  // void on_widget_open(SAT_WidgetOwner* AOwner) override {
  //   //SAT_PRINT;
  //   //SAT_ScrollBoxWidget::prepare(AOwner);
  //   SAT_Window* window = (SAT_Window*)AOwner;
  //   double S = getWindowScale();
  //   if (window) {
  //     MPainter = window->getPainter();
  //     if (MPainter) {
  //       MPainter->setTextSize(MTextSize * S);
  //       //double widest = 0.0;
  //       uint32_t num = MContent->getNumChildren();
  //       for (uint32_t i=0; i<num; i++) {
  //         SAT_TextWidget* textwidget = (SAT_TextWidget*)MContent->getChild(i);
  //         SAT_Rect rect = textwidget->getRect();
  //         const char* text = textwidget->getText();
  //         double bounds[4];
  //         //if (MPainter->getTextBounds(text,bounds)) {
  //         //  double width = bounds[2] - bounds[0];
  //           double advance = MPainter->getTextBounds(text,bounds);
  //           SAT_PRINT("bounds %.2f, %.2f, %.2f, %.2f\n",bounds[0],bounds[1],bounds[2],bounds[3]);
  //           double width = bounds[2] - bounds[0];
  //
  //           //textwidget->setWidth(width);
  //           rect.w = width;
  //           //textwidget->setRectAndBase(rect);
  //
  //           // textwidget->setWidth(width);
  //           // if (width > 0) width /= S;
  //           // //if (width > widest) widest = width;
  //           // textwidget->setBaseWidth(width);
  //
  //           //textwidget->setWidthAndBase(width);
  //
  //         //}
  //       }
  //       //SAT_Widget* context = getContentWidget();
  //     }
  //   }
  // }

//------------------------------
public:
//------------------------------

  virtual void appendLine(const char* AText, bool ARedraw=false) {
    if (MNumLines >= MMaxLines) { removeOldestLine(); }
    double width = 600;//getWidth();
    double S = getWindowScale();
    // SAT_Window* window = (SAT_Window*)getOwner();       // !!!
    // if (window) {
    //   SAT_Painter* painter = window->getPainter();
    //   painter->setTextSize(MTextSize * S);
    //   double bounds[4];
    //   if (painter->getTextBounds(AText,bounds)) {
    //     width = bounds[2] - bounds[0];
    //     if (width > 0) width /= S;
    //   }
    // }
    /*
    SAT_WidgetOwner* owner = getOwner();
    SAT_Painter* painter = owner->on_widgetOwner_getPainter();
    if (painter) {
      double bounds[4] = {0,0,0,0};
      painter->setTextSize(MTextSize * S);
      double advance = painter->getTextBounds(AText,bounds);
      //SAT_PRINT("text %s size %.2f scale %.2f bounds %.2f,%.2f,%.2f,%.2f advance %.2f\n",text,textsize,scale,bounds[0],bounds[1],bounds[2],bounds[3],advance);
      width = advance;
    }
    */    
    // SAT_Rect rect = item->getBaseRect();
    // rect.w = advance;
    // rect.w /= scale;
    // item->setBaseRect(rect);
    //    if (MPainter) {
    //      SAT_Print("yes, we have a painter\n");
    //    }
    //    else {
    //      SAT_Print("no painter\n");
    //    }
    //    SAT_Print("%.2f, %.2f\n",width,MTextSize*S);
    SAT_TextWidget* textwidget = new SAT_TextWidget( SAT_Rect(width,MTextSize*S));
    MContent->appendChild(textwidget);
    MNumLines += 1;
    textwidget->setText(AText);
    textwidget->Layout.flags  = SAT_WIDGET_LAYOUT_ANCHOR_TOP;
    textwidget->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;
    //textwidget->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_LEFT;
    //    textwidget->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
    textwidget->setFillBackground(false);
    textwidget->setDrawBorder(false);
    textwidget->setTextAlignment(MTextAlignment);
    textwidget->setTextColor(SAT_Color(0)/*SAT_COLOR_BLACK*/);
    textwidget->setTextSize(MTextSize);
    //textwidget->Options.autoSize = true;
    //    textwidget->setFillBackground(true);
    //    textwidget->setBackgroundColor(SAT_LightGreen);
    //textwidget->setBackgroundColor(0.55);
    //    MContent->realignChildren();
    //    if (ARedraw) do_widget_redraw(MContent); // only if visible?
  }

  //----------

  void removeOldestLine() {
    if (MNumLines > 0) {
      SAT_WidgetArray* content = MContent->getChildren();
      SAT_Widget* wdg = MContent->getChild(0);
      content->remove((uint32_t)0);
      delete wdg;
      MNumLines -= 1;
    }
  }
  
  //----------
  
//  void on_widget_paint(SAT_PaintContext* AContext) override {
//    SAT_ScrollBoxWidget::on_widget_paint(AContext);
//  }


#endif // 0