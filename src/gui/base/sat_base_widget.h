#ifndef sat_base_widget_included
#define sat_base_widget_included
//----------------------------------------------------------------------

// not used..

#include "sat.h"

//----------------------------------------------------------------------

class SAT_BaseWidget {

//------------------------------
public:
//------------------------------

  SAT_BaseWidget(SAT_Rect ARect) {
  }

  //----------

  virtual ~SAT_BaseWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void        realignParent()
  virtual void        redrawParent()
  virtual void        ownerWindowOpened(SAT_WidgetOwner* AOwner, bool ARecursive=true)
  virtual void        ownerWindowClosed(SAT_WidgetOwner* AOwner, bool ARecursive=true)
  virtual void        setSkin(SAT_Skin* ASkin, bool AOnlyIfNull=true, bool ARecursive=true)
  virtual void        setParent(SAT_Widget* AWidget)
  virtual void        setParentIndex(uint32_t AIndex)
  virtual void        setActive(bool AState=true, bool ARecursive=true)
  virtual bool        isRecursivelyActive()
  virtual void        setVisible(bool AState=true, bool ARecursive=true)
  virtual bool        isRecursivelyVisible()
  virtual void        setEnabled(bool AState=true, bool ARecursive=true)
  virtual bool        isRecursivelyEnabled()
  virtual void        setRectAndBase(SAT_Rect ARect)
  virtual void        setWidthAndBase(double AWidth)
  virtual void        connect(void* AParameter, uint32_t AIndex=0)
  virtual double      getWindowWidth()
  virtual double      getWindowHeight()
  virtual double      getWindowScale()
  virtual SAT_Widget* appendChildWidget(SAT_Widget* AWidget)
  virtual void        deleteChildWidgets()
  virtual SAT_Widget* findChildWidget(uint32_t AXpos, uint32_t AYpos, bool ARecursive=true)
  virtual SAT_Widget* findChildWidget(const char* AName, bool ARecursive=true)
  virtual void        scrollChildWidgets(double AOffsetX, double AOffsetY)
  virtual void        realignChildWidgets(bool ARecursive=true)
  virtual void        paintChildWidgets(SAT_PaintContext* AContext)

//------------------------------
public: // on_ (downwards)
//------------------------------

  virtual void on_widget_open(SAT_WidgetOwner* AOwner)
  virtual void on_widget_close(SAT_WidgetOwner* AOwner)
  virtual void on_widget_prealign()
  virtual void on_widget_postalign()
  virtual void on_widget_paint(SAT_PaintContext* AContext)
  virtual void on_widget_timer(SAT_Timer* ATimer, double AElapsed)
  virtual void on_widget_tween(uint32_t AId, uint32_t AType, uint32_t ANum, double* AData)
  virtual void on_widget_mouseClick(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp)
  virtual void on_widget_mouseDblClick(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp)
  virtual void on_widget_mouseRelease(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp)
  virtual void on_widget_mouseMove(double AXpos, double AYpos, uint32_t AState, uint32_t ATimeStamp)
  virtual void on_widget_keyPress(char AChar, uint32_t AKey, uint32_t AState, uint32_t ATimeStamp)
  virtual void on_widget_keyRelease(char AChar, uint32_t AKey, uint32_t AState, uint32_t ATimeStamp)
  virtual void on_widget_enter(SAT_Widget* AFrom, double AXpos, double AYpos)
  virtual void on_widget_leave(SAT_Widget* ATo, double AXpos, double AYpos)

//------------------------------
public: // do_ (upwards)
//------------------------------

  virtual void do_widget_update(SAT_Widget* AWidget, uint32_t AMode=0)
  virtual void do_widget_redraw(SAT_Widget* AWidget)
  virtual void do_widget_realign(SAT_Widget* AWidget)
  virtual void do_widget_setCursor(SAT_Widget* AWidget, int32_t ACursor)
  virtual void do_widget_setHint(SAT_Widget* AWidget, const char* AHint)
  virtual void do_widget_setModal(SAT_Widget* AWidget)
  virtual void do_widget_captureKeys(SAT_Widget* AWidget)
  virtual void do_widget_close(SAT_Widget* AWidget)
  virtual void do_widget_select(SAT_Widget* AWidget, int32_t AIndex, int32_t ASubIndex=-1)

};

//----------------------------------------------------------------------
#endif
