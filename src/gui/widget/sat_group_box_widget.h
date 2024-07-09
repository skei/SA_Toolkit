#ifndef sat_group_box_widget_included
#define sat_group_box_widget_included
//----------------------------------------------------------------------

#include "gui/widget/sat_button_widget.h"
#include "gui/widget/sat_symbol_widget.h"
#include "gui/widget/sat_visual_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_GroupBoxWidget
: public SAT_VisualWidget {

//------------------------------
private:
//------------------------------

  SAT_ButtonWidget* MHeader     = nullptr;
  SAT_VisualWidget* MContainer  = nullptr;
  SAT_SymbolWidget* MSymbol     = nullptr;

  double            MHeaderSize = 0;
  double            MFullSize   = 0;
  bool              MIsOpen     = true;
  bool              MIsClosable = true;

//------------------------------
public:
//------------------------------

  SAT_GroupBoxWidget(SAT_Rect ARect, double AHeaderSize=20, bool AOpen=true)
  : SAT_VisualWidget(ARect) {
    setName("SAT_GroupBoxWidget");
    setHint("SAT_GroupBoxWidget");
    MHeaderSize = AHeaderSize;
    MFullSize = ARect.h;

    // SAT_Rect header_rect = SAT_Rect(0,0,ARect.w,MHeaderSize);
    // SAT_Rect container_rect = SAT_Rect(0,MHeaderSize,ARect.w,ARect.h-MHeaderSize);
    // MHeader = new SAT_ButtonWidget(header_rect);
    // MHeader->setTexts("Closed","open");
    // MContainer = new SAT_VisualWidget(container_rect);
    // SAT_VisualWidget::appendChild(MHeader);
    // SAT_VisualWidget::appendChild(MContainer);

    MHeader = new SAT_ButtonWidget(MHeaderSize);
    SAT_VisualWidget::appendChild(MHeader);
    MHeader->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
    MHeader->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
    MHeader->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;
    MHeader->Layout.innerBorder = {0,0,3,3};
    MHeader->setTexts("Closed","open");

      //if (MClosable) {
      MSymbol = new SAT_SymbolWidget(7,SAT_SYMBOL_FILLED_TRI_DOWN);
      MHeader->appendChild(MSymbol);
      MSymbol->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_BOTTOM_RIGHT;
      MSymbol->setActive(false);
      MSymbol->setColor(SAT_DarkerGrey);
      //}

    MContainer = new SAT_VisualWidget(0);
    SAT_VisualWidget::appendChild(MContainer);
    MContainer->Options.realignInvisible = true;
    MContainer->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
    MContainer->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_ALL;
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
  SAT_SymbolWidget* getSymbolWidget() { return MSymbol; }

  bool isOpen() { return MIsOpen; }

  void setIsClosable(bool AClosable) { MIsClosable = AClosable; }

//------------------------------
public:
//------------------------------

  SAT_Widget* appendChild(SAT_Widget* AWidget) override {
    return MContainer->appendChild(AWidget);
  }

//------------------------------
public:
//------------------------------

  void do_widget_update(SAT_Widget* AWidget, uint32_t AIndex=0, uint32_t AMode=SAT_WIDGET_UPDATE_VALUE) override {
    if ((AWidget == MHeader) && MIsClosable) {
      double scale = getWindowScale();
      double value = MHeader->getValue();
      if (value > 0.5) {
        open();
        SAT_Rect rect = getRect();
        double pos = MFullSize - MHeaderSize;
        double starts[4]  = {0,0,0,0};
        double ends[4]    = {0,0,0,pos};
        SAT_TweenChain* chain = new SAT_TweenChain();
        SAT_TweenNode* node1 = new SAT_TweenNode(this,255,1.0,SAT_TWEEN_RECT,4,starts,ends,SAT_EASING_OUT_BOUNCE);
        chain->appendNode(node1);
        do_widget_start_tween(this,chain);
      }
      else {

        //close();
        MIsOpen = false;

        SAT_Rect rect = getRect();
        double pos = -(rect.h / scale) + MHeaderSize;
        double starts[4]  = {0,0,0,0};
        double ends[4]    = {0,0,0,pos};
        SAT_TweenChain* chain = new SAT_TweenChain();
        SAT_TweenNode* node1 = new SAT_TweenNode(this,255,1.0,SAT_TWEEN_RECT,4,starts,ends,SAT_EASING_OUT_BOUNCE);
        chain->appendNode(node1);
        do_widget_start_tween(this,chain);
      }
      SAT_VisualWidget::do_widget_update(this,AIndex,AMode);
    }
    else {
      SAT_VisualWidget::do_widget_update(AWidget,AIndex,AMode);
    }
  }

  //----------

  void on_widget_tween(uint32_t AId, uint32_t AType, uint32_t ACount, double* AData) {
    SAT_VisualWidget::on_widget_tween(AId,AType,ACount,AData);
    if (AId == 255) {
      if (AType == SAT_TWEEN_FINISHED) {
        //close();
        if (!MIsOpen) {
          SAT_PRINT("tween finished closing\n");
          close();
        }
      }
    }
  }

//------------------------------
public:
//------------------------------

  virtual void open() {
    MHeader->setValue(1);
    MIsOpen = true;
    // SAT_Rect rect = getBaseRect();
    // rect.h = MFullSize;
    // setBaseRect(rect);
    MContainer->State.active = true;
    MContainer->State.visible = true;
    MSymbol->setSymbol(SAT_SYMBOL_FILLED_TRI_UP);
    do_widget_realign(this,0);
    //do_widget_update(this);
    //do_widget_redraw(this);
  }

  //----------

  virtual void close() {
    MHeader->setValue(0);
    MIsOpen = false;
    // SAT_Rect rect = getBaseRect();
    // rect.h = MHeaderSize;
    // setBaseRect(rect);
    MContainer->State.active = false;
    MContainer->State.visible = false;
    MSymbol->setSymbol(SAT_SYMBOL_FILLED_TRI_DOWN);
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