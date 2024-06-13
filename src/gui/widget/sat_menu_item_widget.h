#ifndef sat_menu_item_widget_included
#define sat_menu_item_widget_included
//----------------------------------------------------------------------

#include "gui/widget/sat_text_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_MenuItemWidget
: public SAT_TextWidget {

//------------------------------
public:
//------------------------------

  SAT_MenuItemWidget(SAT_Rect ARect, const char* AText)
  : SAT_TextWidget(ARect,AText) {
    setName("SAT_MenuItemWidget");
    Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
    Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
    Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;
    setCursor(SAT_CURSOR_FINGER);
  }

  //----------

  virtual ~SAT_MenuItemWidget() {
  }

//------------------------------
public:
//------------------------------

  void on_widget_mouse_click(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    SAT_TRACE;
    int32_t index = getIndex();
    SAT_Rect rect = getRect();
    if (!rect.contains(AXpos,AYpos)) {
      do_widget_notify(this,-1);
      return;
    }
    switch (AButton) {
      case SAT_BUTTON_LEFT:
        do_widget_notify(this,index);
        break;;
      case SAT_BUTTON_RIGHT:
        do_widget_notify(this,-1);
        break;
    }
  }

  //----------

  void on_widget_key_press(uint32_t AKey, uint32_t AChar, uint32_t AState, uint32_t ATime) override {
    SAT_TRACE;
    // if ESC
    do_widget_notify(this,-1);
  }


};

//----------------------------------------------------------------------
#endif