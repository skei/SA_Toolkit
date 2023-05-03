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

  //float     MTextHeight = 20.0;
  float     MTextSize       = 10.0;
  uint32_t  MTextAlignment  = SAT_TEXT_ALIGN_LEFT;
  uint32_t  MNumLines       = 0;
  uint32_t  MMaxLines       = 100;

//------------------------------
public:
//------------------------------

  SAT_TextBoxWidget(SAT_Rect ARect)
  //: SAT_ScrollBoxWidget(ARect,true,true) {
  : SAT_ScrollBoxWidget(ARect,true,false) {
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

//  void on_widget_config(SAT_Widget* AOwnerWindow) {
//    SAT_Window* window = (SAT_Window*)AOwnerWindow;
//    SAT_Painter* painter = window->getPainter();
//    uint32_t num = MContent->getNumChildWidgets();
//    for (uint32_t i=0; i<num; i++) {
//      SAT_TextWidget* textwidget = (SAT_TextWidget*)MContent->getChildWidget(i);
//      if (textwidget) {
//        textwidget->updateTextRect(painter);
//      }
//    }
//    SAT_ScrollBoxWidget::on_widget_config(AOwnerWindow);
//  }

//------------------------------
public:
//------------------------------

  virtual void setMaxLines(uint32_t ANum) { MMaxLines = ANum; }
  //virtual void setTextHeight(float ASize) { MTextHeight = ASize; }
  virtual void setTextSize(double ASize) { MTextSize = ASize; }

//------------------------------
public:
//------------------------------

  virtual void appendLine(const char* AText, bool ARedraw=true) {
    if (MNumLines >= MMaxLines) { removeOldestLine(); }
    //while (MNumLines >= MMaxLines) { removeOldestLine(); }
    SAT_TextWidget* textwidget = new SAT_TextWidget( SAT_Rect(600,MTextSize),AText);
    //textwidget->Layout.alignment = SAT_WIDGET_ALIGN_FILL_TOP;
    
//    textwidget->setAlignment(SAT_WIDGET_ALIGN_TOP_LEFT);

    textwidget->setAlignment(SAT_WIDGET_ALIGN_TOP);
    textwidget->setStretching(SAT_WIDGET_STRETCH_HORIZONTAL);

    textwidget->setFillBackground(false);
    textwidget->setDrawBorder(false);
    textwidget->setTextAlignment(MTextAlignment);
    textwidget->setTextColor(SAT_Color(0)/*SAT_COLOR_BLACK*/);
    textwidget->setTextSize(MTextSize);
    //textwidget->Options.autoSize = true;
    //textwidget->setFillBackground(true);
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

};

//----------------------------------------------------------------------
#endif

