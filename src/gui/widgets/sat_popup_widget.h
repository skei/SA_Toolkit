#ifndef sat_popup_widget_included
#define sat_popup_widget_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "gui/widgets/sat_panel_widget.h"
#include "gui/sat_window.h"

//#define SAT_POPUP_WIDGET_USE_TWEENING

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

  SAT_Window*     MWindow     = nullptr;
  SAT_Rect        MSavedRect  = {};
  SAT_TweenChain* MTween      = nullptr;

//------------------------------
public:
//------------------------------

  SAT_PopupWidget(SAT_Rect ARect)
  : SAT_PanelWidget(ARect) {
    setName("SAT_PopupWidget");
    setActive(false);
    setVisible(false);
    MSavedRect = ARect;
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
    SAT_Rect rect = ARect;
    //setRectAndBasis(MSavedRect);
    MSavedRect = rect;
    uint32_t winw = MWindow->getWidth();
    uint32_t winh = MWindow->getHeight();
    if ((rect.x + rect.w) >= winw) rect.x = winw - rect.w - 5;
    if ((rect.y + rect.h) >= winh) rect.y = winh - rect.h - 5;
    setRectAndBasis(rect);
    
    #ifdef SAT_POPUP_WIDGET_USE_TWEENING
      double S = getWindowScale();
      rect.scale(1.0/S);
      SAT_TweenManager* tweens = MWindow->getTweens();
      double startpos[4] = { rect.x+100, rect.y-100, 0,      0      };
      double endpos[4]   = { rect.x, rect.y, rect.w, rect.h };
      SAT_Assert(!MTween);
      MTween = tweens->appendChain( new SAT_TweenChain() );
      MTween->appendNode( new SAT_TweenNode(this,666,1.0,SAT_WIDGET_TWEEN_RECT,4,startpos,endpos,38) );
      //MTween->appendNode( new SAT_TweenNode(this,666,3.0,SAT_WIDGET_TWEEN_PAUSE) );
      //MTween->appendNode( new SAT_TweenNode(this,666,1.0,SAT_WIDGET_TWEEN_RECT,4,endpos,startpos,39) );
      //MTween->appendNode( new SAT_TweenNode(this,666,0,SAT_WIDGET_TWEEN_CLOSE) );
      MTween->start();
    #endif
    
    realignChildWidgets();
    do_widget_set_state(this,SAT_WIDGET_STATE_MODAL);
    setActive(true);
    setVisible(true);
    parentRedraw();
    return true;
  }

  //----------

  virtual void close() {
    //SAT_Print("close\n");
    do_widget_set_state(this,SAT_WIDGET_STATE_NORMAL);
    setActive(false);
    setVisible(false);
    setRectAndBasis(MSavedRect);
    if (MTween) {
      if (MTween->isActive()) MTween->stop();
      MTween = nullptr; // see assert in open()
    }
    parentRedrawAll(); // redraw entire parent
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
  
  //----------
  
  void on_widget_tween(uint32_t AId, uint32_t AType, uint32_t ACount, double* AData) override {
    SAT_PanelWidget::on_widget_tween(AId,AType,ACount,AData);
    switch (AType) {
      case SAT_WIDGET_TWEEN_CLOSE: {
        close();
        break;
      }
      
    }
  }
  

//------------------------------
public:
//------------------------------


};

//----------------------------------------------------------------------
#endif


