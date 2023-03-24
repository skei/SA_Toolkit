#ifndef sat_popup_widget_included
#define sat_popup_widget_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "gui/widgets/sat_panel_widget.h"
#include "gui/sat_window.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_PopupWidget
: public SAT_PanelWidget {

//------------------------------
private:
//------------------------------

  SAT_Window*     MWindow = nullptr;

//------------------------------
public:
//------------------------------

  SAT_PopupWidget(SAT_Rect ARect)
  : SAT_PanelWidget(ARect) {
    setActive(false);
    setVisible(false);
  }

  //----------

  virtual ~SAT_PopupWidget() {
  }

//------------------------------
public:
//------------------------------

  void prepare(SAT_WidgetListener* AWindow, bool ARecursive=true) override {
    //SAT_PRINT;
    SAT_PanelWidget::prepare(AWindow,ARecursive);
    MWindow = (SAT_Window*)AWindow;
  }

  //----------

  virtual bool open(SAT_Rect ARect) {
    double S = getWindowScale();
    SAT_Rect rect = ARect;
    uint32_t winw = MWindow->getWidth();
    uint32_t winh = MWindow->getHeight();
    if ((rect.x + rect.w) >= winw) rect.x = winw - rect.w - 5;
    if ((rect.y + rect.h) >= winh) rect.y = winh - rect.h - 5;
    
    setPos(rect.x,rect.y);
    SAT_Rect basisrect = getBasisRect();
    basisrect.x = rect.x / S;
    basisrect.y = rect.y / S;
    setBasisRect(basisrect);
    //SAT_Print("realigning\n");
    realignChildWidgets();
    do_widget_set_state(this,SAT_WIDGET_STATE_MODAL);
    setActive(true);
    setVisible(true);
    //setValue(1);
    //update();
    redraw();
    return true;
  }

  //----------

  virtual void close() {
    do_widget_set_state(this,SAT_WIDGET_STATE_NORMAL);
    setActive(false);
    setVisible(false);
    //setValue(0);
    //update();
    redrawAll(); // redraw entire parent
  }

//------------------------------
public:
//------------------------------

  // called from window
  // (clicking outside of modal widget)

  void on_widget_notify(uint32_t AReason=SAT_WIDGET_NOTIFY_NONE) override {
    //SAT_PanelWidget::on_widget_notify(AReason,AValue);
    if (AReason == SAT_WIDGET_NOTIFY_CLOSE) {
      close();
    }
  }

//------------------------------
public:
//------------------------------


};

//----------------------------------------------------------------------
#endif


