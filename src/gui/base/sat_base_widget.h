#ifndef sat_base_widget_included
#define sat_base_widget_included
//----------------------------------------------------------------------


//----------------------------------------------------------------------

#include "sat.h"
#include "gui/base/sat_paint_context.h"
#include "gui/base/sat_widget_owner.h"

class SAT_Widget;
class SAT_TweenChain;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_BaseWidget {

//------------------------------
public:
//------------------------------

  SAT_BaseWidget() {}
  virtual ~SAT_BaseWidget() {}

//------------------------------
public:
//------------------------------

  virtual void      on_widget_open(SAT_WidgetOwner* AOwner) {}
  virtual void      on_widget_close(SAT_WidgetOwner* AOwner) {}
  virtual void      on_widget_move(int32_t AXpos, int32_t AYpos) {}
  virtual void      on_widget_resize(uint32_t AWidth, uint32_t AHeight) {}
  virtual SAT_Rect  on_widget_preAlign(SAT_Rect ARect) { return ARect; }
  virtual SAT_Rect  on_widget_postAlign(SAT_Rect ARect) { return ARect; }
  virtual void      on_widget_realign() {}
  virtual void      on_widget_paint(SAT_PaintContext* AContext) {}
  virtual void      on_widget_mouse_click(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) {}
  virtual void      on_widget_mouse_dblclick(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) {}
  virtual void      on_widget_mouse_release(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) {}
  virtual void      on_widget_mouse_move(int32_t AXpos, int32_t AYpos, uint32_t AState, uint32_t ATime) {}
  virtual void      on_widget_key_press(uint32_t AKey, uint32_t AChar, uint32_t AState, uint32_t ATime) {}
  virtual void      on_widget_key_release(uint32_t AKey, uint32_t AChar, uint32_t AState, uint32_t ATime) {}
  virtual void      on_widget_enter(SAT_Widget* AFrom, int32_t AXpos, int32_t AYpos, uint32_t ATime) {}
  virtual void      on_widget_leave(SAT_Widget* ATo, int32_t AXpos, int32_t AYpos, uint32_t ATime) {}
  virtual void      on_widget_tween(uint32_t AId, uint32_t AType, uint32_t ACount, double* AData) {}
  virtual void      on_widget_timer(double ADelta) {}
  virtual void      on_widget_hint(SAT_Widget* AWidget, const char* AHint) {}


//------------------------------
public:
//------------------------------

  virtual void do_widget_update(SAT_Widget* AWidget, uint32_t AIndex=0, uint32_t AMode=SAT_WIDGET_UPDATE_VALUE) {}
  virtual void do_widget_redraw(SAT_Widget* AWidget, uint32_t AIndex=0, uint32_t AMode=SAT_WIDGET_REDRAW_GUI) {}
  virtual void do_widget_realign(SAT_Widget* AWidget, uint32_t AMode=SAT_WIDGET_REALIGN_GUI) {}
  virtual void do_widget_notify(SAT_Widget* AWidget, int32_t AValue) {}
  virtual void do_widget_mouse_capture(SAT_Widget* AWidget) {}
  virtual void do_widget_key_capture(SAT_Widget* AWidget) {}
  virtual void do_widget_modal(SAT_Widget* AWidget) {}
  virtual void do_widget_set_cursor(SAT_Widget* AWidget, int32_t ACursor) {}
  virtual void do_widget_set_hint(SAT_Widget* AWidget, const char* AHint) {}
  virtual void do_widget_want_timer(SAT_Widget* AWidget, bool AWantTimer=true) {}
  virtual void do_widget_start_tween(SAT_Widget* AWidget, SAT_TweenChain* ATween) {}
  virtual void do_widget_resize(SAT_Widget* AWidget, double ADeltaX, double ADeltaY) {}
  virtual void do_widget_set_overlay(SAT_Widget* AWidget, SAT_Color AColor) {}

//------------------------------
public:
//------------------------------

  /*

  virtual SAT_Rect          getBaseRect()
  virtual SAT_Rect          getContentRect()
  virtual int32_t           getCursor()
  virtual const char*       getHint()
  virtual uint32_t          getIndex()
  virtual SAT_Rect          getInitialRect()
  virtual double            getModulation()
  virtual const char*       getName()
  virtual SAT_WidgetOwner*  getOwner()
  virtual uint32_t          getNumValues()
  virtual void*             getParameter()
  virtual void*             getParameter(uint32_t AIndex)
  virtual SAT_Widget*       getParent()
  virtual SAT_Rect          getRect()
  virtual double            getValue(uint32_t AIndex)
  virtual double            getValue()
  virtual uint32_t          getValueIndex()

  virtual bool              isRecursivelyActive()
  virtual bool              isRecursivelyVisible()
  virtual bool              isRecursivelyDisabled()
  virtual SAT_Widget*       findOpaqueParent()

  */

  /*

  virtual void setBaseRect(SAT_Rect ARect)
  virtual void setHint(const char* AHint)
  virtual void setIndex(uint32_t AIndex)
  virtual void setModulation(double AValue)
  virtual void setCursor(int32_t ACursor)
  virtual void setName(const char* AName)
  virtual void setNumValues(uint32_t ANum)
  virtual void setOwner(SAT_WidgetOwner* AOwner)
  virtual void setParameter(void* AParameter)
  virtual void setParameter(void* AParameter, uint32_t AIndex)
  virtual void setParameterIndex(uint32_t AIndex)
  virtual void setParent(SAT_Widget* AParent)
  virtual void setPos(SAT_Point APos)
  virtual void setRect(SAT_Rect ARect)
  virtual void setSize(SAT_Point ASize)
  virtual void setValue(double AValue, uint32_t AIndex)
  virtual void setValue(double AValue)
  virtual void setValueIndex(uint32_t AIndex)

  virtual void setMinPos(double AXpos, double AYpos)
  virtual void setMinSize(double AWidth, double AHeight)
  virtual void setMaxPos(double AXpos, double AYpos)
  virtual void setMaxSize(double AWidth, double AHeight)
  virtual void setSize(double AWidth, double AHeight)
  virtual void setWidth(double AWidth)
  virtual void setHeight(double AHeight)
  virtual void setActive(bool AState=true, bool ARecursive=true)
  virtual void setVisible(bool AState=true, bool ARecursive=true)
  virtual void setDisabled(bool AState=true, bool ARecursive=true)
  virtual void setRectAndBase(SAT_Rect ARect)

  */

//------------------------------
public:
//------------------------------

  /*

  virtual SAT_Widget*       appendChild(SAT_Widget* AWidget)
  virtual void              removeChild(SAT_Widget* AWidget)
  virtual void              deleteChildren()
  virtual uint32_t          getNumChildren()
  virtual SAT_Widget*       getChild(uint32_t AIndex)
  virtual SAT_WidgetArray*  getChildren()

  virtual void              ownerWindowOpened(SAT_WidgetOwner* AOwner)
  virtual void              ownerWindowClose(SAT_WidgetOwner* AOwner)
  virtual double            getWindowWidth()
  virtual double            getWindowHeight()
  virtual double            getWindowScale()

  virtual SAT_Widget*       findWidget(int32_t AXpos, int32_t AYpos, bool ARecursive=true)
  virtual SAT_Widget*       findWidget(const char* AName, bool ARecursive=true)
  virtual void              scrollChildren(double AOffsetX, double AOffsetY)
  virtual void              paintChildren(SAT_PaintContext* AContext)
  virtual void              realignChildren(bool ARecursive=true)
  virtual void              clearPrevTween()
  virtual void              handleTweening(uint32_t AId, uint32_t AType, uint32_t ACount, double* AData)

  */

};

//----------------------------------------------------------------------
#endif


  
