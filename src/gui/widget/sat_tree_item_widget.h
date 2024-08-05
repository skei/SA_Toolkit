#ifndef sat_tree_item_widget_included
#define sat_tree_item_widget_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/widget/sat_text_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_TreeItemWidget
: public SAT_TextWidget {

//------------------------------
private:
//------------------------------

  bool              MIsOpen   = false;
  double            MHeight   = 0.0;
  SAT_VisualWidget* MSubItems = nullptr;

//------------------------------
public:
//------------------------------

  SAT_TreeItemWidget(SAT_Rect ARect, const char* AText, bool AOpen=false)
  : SAT_TextWidget(ARect,AText) {
    setName("SAT_TreeItemWidget");
    setHint("SAT_TreeItemWidget");
    setCursor(SAT_CURSOR_FINGER);
    setFillBackground(true);
    setBackgroundColor(0.47);
    setDrawBorder(false);
    Layout.flags  = SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
    Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
    Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;
    //Layout.flags |= SAT_WIDGET_LAYOUT_CONTENT_SIZE;
    Options.realignInvisible = true;
    MHeight = getRect().h;
    MIsOpen = AOpen;
    MSubItems = new SAT_VisualWidget(MHeight);
    appendChild(MSubItems);
    //MSubItems->setName("SAT_TreeItemWidget");
    //MSubItems->setHint("SAT_TreeItemWidget");

    MSubItems->Layout.flags  = SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
    MSubItems->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
    MSubItems->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;
    if (MIsOpen) {
      MSubItems->setActive(true);
      MSubItems->setVisible(true);
    }
    else {
      MSubItems->setActive(false);
      MSubItems->setVisible(false);
    }
  }

  //----------

  virtual ~SAT_TreeItemWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void appendItem(const char* AText) {
    SAT_TreeItemWidget* item = new SAT_TreeItemWidget(MHeight,AText);
    MSubItems->appendChild(item);
    item->setName("SAT_TreeItemWidget");
    item->setHint("SAT_TreeItemWidget");

    if (MIsOpen) {
      item->State.active = true;
      item->State.visible = true;
    }
    else {
      item->State.active = false;
      item->State.visible = false;
    }
  }

  //----------

  virtual void open() {
    MIsOpen = true;
    uint32_t num_children = MSubItems->getNumChildren();
    SAT_Rect rect = getBaseRect();
    double height = MHeight * num_children;
    SAT_PRINT("height %.2f\n",height);
    rect.h = height;
    MSubItems->setBaseRect(rect);
    //for (uint32_t i=0; i<num_children; i++) {
    //  getChild(i)->State.active = true;
    //  getChild(i)->State.visible = true;
    //}
    MSubItems->setActive(true,true);
    MSubItems->setVisible(true,true);
    do_widget_realign(this,SAT_WIDGET_REALIGN_GUI);
    // do_widget_redraw(this);
  }

  //----------
  
  virtual void close() {
    MIsOpen = false;
    uint32_t num_children = MSubItems->getNumChildren();
    SAT_Rect rect = getBaseRect();
    double height = MHeight;
    SAT_PRINT("height %.2f\n",height);
    rect.h = height;
    setBaseRect(rect);
    //for (uint32_t i=0; i<num_children; i++) {
    //  getChild(i)->State.active = false;
    //  getChild(i)->State.visible = false;
    //}
    MSubItems->setActive(true,true);
    MSubItems->setVisible(true,true);
    do_widget_realign(this,SAT_WIDGET_REALIGN_GUI);
    // do_widget_redraw(this);
  }

  //----------
  
  virtual void toggle() {
    if (MIsOpen) close();
    else open();
  }

//------------------------------
public:
//------------------------------

  void on_widget_mouse_click(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    SAT_TRACE;
    if (AButton == SAT_BUTTON_LEFT) {
      if (getNumChildren() > 0) {
        toggle();
        do_widget_redraw(this);
      }
    }
  }

  // void on_widget_paint(SAT_PaintContext* AContext) override {
  // }

};

//----------------------------------------------------------------------
#endif
