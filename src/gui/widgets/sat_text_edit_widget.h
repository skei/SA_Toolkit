#ifndef sat_text_edit_widget_included
#define sat_text_edit_widget_included
//----------------------------------------------------------------------

#include "base/utils/sat_strutils.h"
#include "gui/sat_widget.h"

//#define sat_xcb_key_enter     65293 // 13
//#define sat_xcb_key_esc       65307 // 27
//#define sat_xcb_key_home      65360 // 1
//#define sat_xcb_key_end       65367 // 2
//#define sat_xcb_key_left      65361 // 3
//#define sat_xcb_key_right     65363 // 4
//#define sat_xcb_key_delete    65535 // 5
//#define sat_xcb_key_backspace 65288 // 6

class SAT_TextEditWidget
//: public SAT_TextWidget {
: public SAT_PanelWidget {

//------------------------------
private:
//------------------------------

  char      MText[256]  = {0};
  SAT_Color MTextColor  = SAT_Black;
  SAT_Color MBackColor  = SAT_LightGrey;
  SAT_Color MCaretColor = SAT_BrightRed;
  bool      MEditing    = false;
  int32_t   MCaretPos   = 0;

//------------------------------
public:
//------------------------------

  SAT_TextEditWidget(SAT_Rect ARect, const char* AText)
  //: SAT_TextWidget(ARect,AText) {
  : SAT_PanelWidget(ARect) {

    setName("SAT_TextEditWidget");
    //MHint = "textedit";
    setCursor(SAT_CURSOR_IBEAM);
    strncpy(MText,AText,255);
    //MTextSize = 10;
    MTextColor = SAT_Black;
  }

  //----------

  virtual ~SAT_TextEditWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setText(const char* AText) {
    memset(MText,0,sizeof(MText));
    strncpy(MText,AText,255);
  }

//------------------------------
private:
//------------------------------

  void start_edit() {
    MEditing = true;
    MCaretPos = strlen(MText);
    //do_widgetListener_update(this);
    do_widgetListener_want_keys(this);
    do_widgetListener_set_modal(this);
    do_widgetListener_redraw(this,0);
  }

  //----------

  void stop_edit() {
    SAT_PRINT;
    MEditing = false;
    //do_widgetListener_update(this);
    do_widgetListener_want_keys(nullptr);
    do_widgetListener_set_modal(nullptr);
    do_widgetListener_redraw(this,0);
  }

//------------------------------
public:
//------------------------------

  //void on_widget_paint(SAT_Painter* APainter, SAT_DRect ARect, uint32_t AMode) override {
  void on_widget_paint(SAT_PaintContext* AContext) override {
//    SAT_Widget::on_widget_paint(AContext);
    SAT_Painter* painter = AContext->painter;
    SAT_Rect mrect = getRect();
    SAT_Rect r;

//    //painter->fillRectangle(mrect,MBackColor);
//    painter->beginPath();
//    painter->rect(mrect.x,mrect.y,mrect.w,mrect.h);
//    painter->fillColor(MBackColor);
//    painter->fill();

    fillBackground(AContext);

    if (MEditing) {
      //APainter->setTextColor(MTextColor);
      //APainter->drawText(MRect.x+2,MRect.y,MRect.x2()-2,MRect.y2(),MText,SAT_TEXT_ALIGN_LEFT);
      r = mrect;
      r.shrink(2,0,2,0); // 2 = text border

      //APainter->setColor(MTextColor);
      //APainter->drawText(r,MText,SAT_TEXT_ALIGN_LEFT);

//      painter->beginPath();
//      painter->fontSize(MTextSize);
//      painter->drawTextBox(r,MText,SAT_TEXT_ALIGN_LEFT,MTextColor);

      painter->setTextColor(MTextColor);
      painter->drawTextBox(r,MText,SAT_TEXT_ALIGN_LEFT);

      char c = MText[MCaretPos];
      //MText[MCaretPos] = 0;
      MText[MCaretPos] = 0;

      //int32_t txtwidth = painter->getTextWidth(MText);
      float bounds[4];
      painter->getTextBounds(mrect.x,mrect.y,MText,nullptr,bounds);
      int32_t txtwidth  = bounds[2] - bounds[0];
      //height = bounds[3] - bounds[1];


      MText[MCaretPos] = c;
      int32_t x = getRect().x + 2 + txtwidth; // 2 = caret distance/width
      //APainter->setDrawColor(MCaretColor);
      //APainter->drawLine(x,MRect.y,x,MRect.y2());

      //APainter->moveTo(x,mrect.y);
      //APainter->lineTo(x,mrect.y2());
      //APainter->setColor(MCaretColor);
      //APainter->strokePath();

//      painter->beginPath();
//      painter->moveTo(x,mrect.y);
//      painter->lineTo(x,mrect.y2());
//      painter->strokeColor(MCaretColor);
//      painter->stroke();

        painter->setLineWidth(1);
        painter->setDrawColor(MCaretColor);
        painter->drawLine(x,mrect.y,x,mrect.y2());

    } // editing
    else {
      //APainter->setTextColor(MTextColor);
      //APainter->drawText(MRect.x+2,MRect.y,MRect.x2()-4,MRect.y2(),MText,SAT_TEXT_ALIGN_LEFT);
      r = mrect;
      r.shrink(2,0,4,0);

      //APainter->setColor(MTextColor);
      //APainter->drawText(r,MText,SAT_TEXT_ALIGN_LEFT);

//      painter->beginPath();
//      painter->fontSize(MTextSize);
//      painter->drawTextBox(r,MText,SAT_TEXT_ALIGN_LEFT,MTextColor);

      painter->setTextColor(MTextColor);
      painter->drawTextBox(r,MText,SAT_TEXT_ALIGN_LEFT);


    } // not editing

    drawBorder(AContext);

  }

  //----------

  void on_widget_mouse_click(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimestamp) override {
    //SAT_Widget::on_widget_mouseClick(AXpos,AYpos,AButton,AState);
    switch(AButton) {
      case SAT_BUTTON_LEFT:
        if (getRect().contains(AXpos,AYpos)) {
          if (!MEditing) start_edit();
        }
        else {
          stop_edit();
        }
        break;
      case SAT_BUTTON_RIGHT:
        if (MEditing) stop_edit();
        break;
    }
  }

  //----------

  // AKey = key code, not ascii..

  void on_widget_key_press(uint8_t AChar, uint32_t AKeySym, uint32_t AState, uint32_t ATimeStamp=0) override {
    //SAT_Print("AChar %i AKey %i AState %i\n",(int)AChar,AKey,AState);
    //SAT_DRect mrect = getRect();
    int32_t len;
    char  c;
    //SAT_Widget::on_keyPress(AChar,AKey,AState);
    //SAT_Print("key: %i, skift: %i\n",AKey,AState);
    switch(AKeySym) {
      case SAT_KEY_ENTER:
        stop_edit();
        break;
      case SAT_KEY_ESC:
        stop_edit();
        break;
      case SAT_KEY_HOME:
        MCaretPos = 0;
        do_widgetListener_update(this,0);
        do_widgetListener_redraw(this,0);
        break;
      case SAT_KEY_END:
        len = strlen(MText);
        MCaretPos = len;
        do_widgetListener_update(this,0);
        do_widgetListener_redraw(this,0);
        break;
      case SAT_KEY_LEFT:
        MCaretPos -= 1;
        if (MCaretPos < 0) MCaretPos = 0;
        do_widgetListener_update(this,0);
        do_widgetListener_redraw(this,0);
        break;
      case SAT_KEY_RIGHT:
        len = strlen(MText);
        MCaretPos += 1;
        if (MCaretPos > len) MCaretPos = len;
        do_widgetListener_update(this,0);
        do_widgetListener_redraw(this,0);
        break;
      case SAT_KEY_DELETE:
        len = strlen(MText);
        if ((uint32_t)MCaretPos < strlen(MText)) {
          SAT_DeleteChar(MText,MCaretPos);
          do_widgetListener_update(this,0);
          do_widgetListener_redraw(this,0);
        }
        break;
      case SAT_KEY_BACKSPACE:
        if (MCaretPos > 0) {
          MCaretPos -= 1;
          SAT_DeleteChar(MText,MCaretPos);
          do_widgetListener_update(this,0);
          do_widgetListener_redraw(this,0);
        }
        break;
      default:
        if ((AKeySym >= 32) && (AKeySym <= 127)) {
          c = AKeySym & 0xff;
          // hack-alert!
//          if ((c >= 'a') && (c <= 'z') && (AState == SAT_KEY_SHIFT)) { c -= 32; }
          SAT_InsertChar(MText,MCaretPos,c);
          MCaretPos += 1;
          do_widgetListener_update(this,0);
          do_widgetListener_redraw(this,0);
        }
        break;
    } // switch
  }

};

#undef kkc_enter
#undef kkc_esc
#undef kkc_home
#undef kkc_end
#undef kkc_left
#undef kkc_right
#undef kkc_delete
#undef kkc_backspace

//----------------------------------------------------------------------
#endif