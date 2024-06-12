#ifndef sat_group_box_widget_included
#define sat_group_box_widget_included
//----------------------------------------------------------------------

#include "gui/widget/sat_button_widget.h"
#include "gui/widget/sat_visual_widget.h"

class SAT_GroupBoxWidget
: public SAT_VisualWidget {

//------------------------------
private:
//------------------------------

  SAT_ButtonWidget* MHeader     = nullptr;
  SAT_VisualWidget* MContainer  = nullptr;

  double            MHeaderSize = 0;
  double            MFullSize   = 0;
  bool              MIsOpen     = true;

//------------------------------
public:
//------------------------------

  SAT_GroupBoxWidget(SAT_Rect ARect, double AHeaderSize=20, bool AOpen=true)
  : SAT_VisualWidget(ARect) {
    setName("SAT_GroupBoxWidget");
    MHeaderSize = AHeaderSize;
    MFullSize = ARect.h;
    SAT_Rect header_rect = SAT_Rect(0,0,ARect.w,MHeaderSize);
    SAT_Rect container_rect = SAT_Rect(0,MHeaderSize,ARect.w,ARect.h-MHeaderSize);
    MHeader = new SAT_ButtonWidget(header_rect);
    MContainer = new SAT_VisualWidget(container_rect);
    SAT_VisualWidget::appendChild(MHeader);
    SAT_VisualWidget::appendChild(MContainer);
    MContainer->Options.realignInvisible = true;
    if (AOpen) open();
    else close();
  }

  //----------

  virtual ~SAT_GroupBoxWidget() {
  }


//------------------------------
public:
//------------------------------

  SAT_ButtonWidget* getHeaderWidget() { return MHeader; }
  SAT_VisualWidget* getContainerWidget() { return MContainer; }

  bool isOpen() { return MIsOpen; }

//------------------------------
public:
//------------------------------

  SAT_Widget* appendChild(SAT_Widget* AWidget) override {
    return MContainer->appendChild(AWidget);
  }

//------------------------------
public:
//------------------------------

  void do_widget_update(SAT_Widget* AWidget, uint32_t AMode=SAT_WIDGET_UPDATE_VALUE) override {
    SAT_VisualWidget::do_widget_update(AWidget,AMode);
    //SAT_TRACE;
    double value = MHeader->getValue();
    if (value > 0.5) {
      open();
    }
    else {
      close();
    }
  }


//------------------------------
public:
//------------------------------

  virtual void open() {
    MHeader->setValue(1);
    MIsOpen = true;
    SAT_Rect rect = getBaseRect();
    rect.h = MFullSize;
    setBaseRect(rect);
    MContainer->State.active = true;
    MContainer->State.visible = true;
    do_widget_realign(this,0);
    //do_widget_update(this,SAT_WIDGET_UPDATE_VALUE);
    //do_widget_redraw(this,SAT_WIDGET_REDRAW_PARAM);
  }

  //----------

  virtual void close() {
    MHeader->setValue(0);
    MIsOpen = false;
    SAT_Rect rect = getBaseRect();
    rect.h = MHeaderSize;
    setBaseRect(rect);
    MContainer->State.active = false;
    MContainer->State.visible = false;
    do_widget_realign(this,0);
  }

  //----------

  virtual void toggle() {
    if (MIsOpen) close();
    else open();
  }

  //----------

};


//----------------------------------------------------------------------
#endif