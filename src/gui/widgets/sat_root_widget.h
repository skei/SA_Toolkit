#ifndef sat_root_widget_included
#define sat_root_widget_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/base/sat_widget_listener.h"
#include "gui/widgets/sat_panel_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_RootWidget
: public SAT_PanelWidget {

//------------------------------
private:
//------------------------------

  SAT_WidgetListener* MListener = nullptr;

//------------------------------
public:
//------------------------------

  SAT_RootWidget(SAT_WidgetListener* AListener=nullptr)
  : SAT_PanelWidget(SAT_Rect(0,0,0,0)) {
    MListener = AListener;
    setName("MRootWidget");
  }

  //----------

  virtual ~SAT_RootWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setListener(SAT_WidgetListener* AListener) {
    MListener = AListener;
  }

  virtual SAT_WidgetListener* getListener() {
    return MListener;
  }

  virtual void setListenerIfNull(SAT_WidgetListener* AListener) {
    if (!MListener) MListener = AListener;
  }

//------------------------------
public: // widget
//------------------------------

  void do_widget_update(SAT_Widget* AWidget, uint32_t AIndex, uint32_t AMode) override {
    if (MListener) MListener->on_widgetListener_update(AWidget,AIndex,AMode);
  }

  void do_widget_redraw(SAT_Widget* AWidget, uint32_t AIndex, uint32_t AMode) override {
    if (MListener) MListener->on_widgetListener_redraw(AWidget,AIndex,AMode);
  }

  void do_widget_realign(SAT_Widget* AWidget, uint32_t AMode) override {
    if (MListener) MListener->on_widgetListener_realign(AWidget,AMode);
  }

  void do_widget_setCursor(SAT_Widget* AWidget, int32_t ACursor) override {
    if (MListener) MListener->on_widgetListener_setCursor(AWidget,ACursor);
  }

  void do_widget_setHint(SAT_Widget* AWidget, const char* AHint) override {
    if (MListener) MListener->on_widgetListener_setHint(AWidget,AHint);
  }

  void do_widget_setModal(SAT_Widget* AWidget) override {
    if (MListener) MListener->on_widgetListener_setModal(AWidget);
  }

  void do_widget_captureKeys(SAT_Widget* AWidget) override {
    if (MListener) MListener->on_widgetListener_captureKeys(AWidget);
  }

  void do_widget_close(SAT_Widget* AWidget) override {
    if (MListener) MListener->on_widgetListener_close(AWidget);
  }

  void do_widget_select(SAT_Widget* AWidget, int32_t AIndex, int32_t ASubIndex=-1) override {
    if (MListener) MListener->on_widgetListener_select(AWidget,AIndex,ASubIndex);
  }

  //void do_widgetListener_resized(SAT_Widget* AWidget, double ADeltaX, double ADeltaY) override {
  //  if (MListener) MListener->on_widgetListener_resized(AWidget,ADeltaX,ADeltaY);
  //}

};



//----------------------------------------------------------------------
#endif
