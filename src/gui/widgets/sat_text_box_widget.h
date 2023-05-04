#ifndef sat_text_box_widget_included
#define sat_text_box_widget_included
//----------------------------------------------------------------------

// untested

#include "gui/widgets/sat_scroll_box_widget.h"

//----------------------------------------------------------------------

class SAT_TextBoxWidget
: public SAT_ScrollBoxWidget {

//------------------------------
protected:
//------------------------------

//SAT_Window* MWindow         = nullptr;
//float         MTextHeight = 20.0;

  SAT_Painter*  MPainter       = nullptr;
  float         MTextSize       = 10.0;
  uint32_t      MTextAlignment  = SAT_TEXT_ALIGN_LEFT;
  uint32_t      MNumLines       = 0;
  uint32_t      MMaxLines       = 100;

//------------------------------
public:
//------------------------------

  SAT_TextBoxWidget(SAT_Rect ARect)
  //: SAT_ScrollBoxWidget(ARect,true,true) {
  : SAT_ScrollBoxWidget(ARect,true,true) {
    setName("SAT_TextBoxWidget");
    //MContent->layout.innerBorder = 0;
    MContent->setDrawBorder(true);
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

  /*
    text might have been appended to the textbox during creation,
    before we opened the editor (and thus, before we have a painter
    so we can measure the string length)..
    so we go throgh them all here, and readjust their widths..
  */
  
  void prepare(SAT_WidgetOwner* AOwner) override {
    SAT_PRINT;
    SAT_ScrollBoxWidget::prepare(AOwner);
    SAT_Window* window = (SAT_Window*)AOwner;
    double S = getWindowScale();
    if (window) {
      MPainter = window->getPainter();
      if (MPainter) {
        MPainter->setTextSize(MTextSize * S);
        //double widest = 0.0;
        uint32_t num = MContent->getNumChildWidgets();
        for (uint32_t i=0; i<num; i++) {
          SAT_TextWidget* textwidget = (SAT_TextWidget*)MContent->getChildWidget(i);
          const char* text = textwidget->getText();
          double bounds[4];
          if (MPainter->getTextBounds(text,bounds)) {
            double width = bounds[2] - bounds[0];
            if (width > 0) width /= S;
            //if (width > widest) widest = width;
            textwidget->setWidth(width);
            textwidget->setBasisWidth(width);
          }
        }
//        SAT_Widget* context = getContentWidget();
      }
    }
  }

//------------------------------
public:
//------------------------------

  virtual void appendLine(const char* AText, bool ARedraw=true) {

    if (MNumLines >= MMaxLines) { removeOldestLine(); }
    
    double width = 600;//getWidth();
    double S = getWindowScale();

    SAT_Window* window = (SAT_Window*)getOwner();
    if (window) {
      SAT_Painter* painter = window->getPainter();
      painter->setTextSize(MTextSize * S);
      double bounds[4];
      if (painter->getTextBounds(AText,bounds)) {
        width = bounds[2] - bounds[0];
        if (width > 0) width /= S;
      }
    }
    
//    if (MPainter) {
//      SAT_Print("yes, we have a painter\n");
//    }
//    else {
//      SAT_Print("no painter\n");
//    }
    
    SAT_TextWidget* textwidget = new SAT_TextWidget( SAT_Rect(width,MTextSize*S),AText);
    textwidget->setAlignment(SAT_WIDGET_ALIGN_TOP);
    //textwidget->setStretching(SAT_WIDGET_STRETCH_HORIZONTAL);

    textwidget->setFillBackground(false);
    textwidget->setDrawBorder(false);
    textwidget->setTextAlignment(MTextAlignment);
    textwidget->setTextColor(SAT_Color(0)/*SAT_COLOR_BLACK*/);
    textwidget->setTextSize(MTextSize);
    //textwidget->Options.autoSize = true;
//    textwidget->setFillBackground(true);
//    textwidget->setBackgroundColor(SAT_LightGreen);
    //textwidget->setBackgroundColor(0.55);
    MContent->appendChildWidget(textwidget);
    MNumLines += 1;

    if (ARedraw) {
      MContent->realignChildWidgets();
      do_widgetListener_redraw(MContent,0); // only if visible?
    }
  }

  //----------

  void removeOldestLine() {
    if (MNumLines > 0) {
      SAT_WidgetArray* content = MContent->getChildWidgets();
      SAT_Widget* wdg = MContent->getChildWidget(0);
      content->remove((uint32_t)0);
      delete wdg;
      MNumLines -= 1;
    }
  }
  
  //----------
  
//  void on_widget_paint(SAT_PaintContext* AContext) override {
//    SAT_ScrollBoxWidget::on_widget_paint(AContext);
//  }

};

//----------------------------------------------------------------------
#endif

