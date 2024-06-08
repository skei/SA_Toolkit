#ifndef sat_popup_widget_included
#define sat_popup_widget_included
//----------------------------------------------------------------------

#include "sat.h"
#include "base/utils/sat_tween_manager.h"
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

//  SAT_Rect        MSavedRectMSavedRect  = {};
//  SAT_TweenChain* MTween      = nullptr;

//------------------------------
public:
//------------------------------

  SAT_PopupWidget(SAT_Rect ARect)
  : SAT_PanelWidget(ARect) {
    setName("SAT_PopupWidget");
    setActive(false);
    setVisible(false);
//    MSavedRect = ARect;

    //MOptions.realignEvenIfNotVisible = true;
    getOptions()->realignEvenIfNotVisible = true;

  }

  //----------

  virtual ~SAT_PopupWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual bool open(SAT_Rect ARect) {

    SAT_Rect rect = ARect;
    double S = getWindowScale();

    //setRectAndBasis(MSavedRect);
    //MSavedRect = rect;
    uint32_t winw = getWindowWidth();
    uint32_t winh = getWindowHeight();
    
    //SAT_Print("winw %i winh %i,%i\n",winw,winh);
    if ((rect.x + rect.w) >= winw) rect.x = winw - rect.w - (5*S);
    if ((rect.y + rect.h) >= winh) rect.y = winh - rect.h - (5*S);
    
    //setRect(rect);
    setRectAndBase(rect);
    
    #ifdef SAT_POPUP_WIDGET_USE_TWEENING
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

    setActive(true);
    setVisible(true);
    do_widget_realign(this,0);
//    realignChildWidgets();
    do_widget_redraw(this,0,0);
    do_widget_setModal(this);

    return true;
  }

  //----------

  virtual void close() {
    //SAT_Print("close\n");
    //do_widgetListener_set_state(this,SAT_WIDGET_STATE_NORMAL);
    do_widget_setModal(nullptr);
    setActive(false);
    setVisible(false);
    //setRectAndBasis(MSavedRect);
    #ifdef SAT_POPUP_WIDGET_USE_TWEENING
      if (MTween) {
        if (MTween->isActive()) MTween->stop();
        MTween = nullptr; // see assert in open()
      }
    #endif
    //parentRealign();
    realignParent();
    redrawParent(); // redraw entire parent
    
  }

//------------------------------
public:
//------------------------------

  // called from window
  // (clicking outside of modal widget)

  void do_widget_close(SAT_Widget* ASender) override {
    //if (MListener) MListener->do_widgetListener_close(ASender);
    close();
  }
  
  //----------
  
  // void on_widget_tween(uint32_t AId, uint32_t AType, uint32_t ACount, double* AData) override {
  //   SAT_PanelWidget::on_widget_tween(AId,AType,ACount,AData);
  //   switch (AType) {
  //     case SAT_WIDGET_TWEEN_CLOSE: {
  //       close();
  //       break;
  //     }
      
  //   }
  // }
  

//------------------------------
public:
//------------------------------


};

//----------------------------------------------------------------------
#endif

